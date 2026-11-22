/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "launchvalidator.h"

#include "appconstants.h"
#include "commandbuilder.h"

#include <QDir>
#include <QFileInfo>
#include <QRegularExpression>
#include <algorithm>
#include <array>
#include <cmath>
#include <optional>
#include <utility>

namespace LaunchValidator
{
namespace
{
LaunchError validateLaunch(const LaunchContext &context, QString *detail)
{
    const QString &hashFile = context.options.hashFile;
    const QString &hashcatPath = context.hashcatPath;
    const QString &resolvedHashcatPath = context.resolvedHashcatPath;
    const QString &configuredTerminal = context.configuredTerminal;
    const QStringList &availableTerminals = context.availableTerminals;

    if (detail) {
        detail->clear();
    }

    if (hashFile.isEmpty()) {
        return LaunchError::NoHashFile;
    }

    if (hashcatPath.isEmpty()) {
        return LaunchError::NoHashcatPath;
    }

    // The caller resolves the stored value to the absolute program the
    // launch starts. Only that one may reach a process; a value that did not
    // resolve is reported with the reason the resolver could not use it.
    if (resolvedHashcatPath.isEmpty() || !QDir::isAbsolutePath(resolvedHashcatPath)) {
        if (detail) {
            *detail = hashcatPath;
        }
        // A non-absolute "resolved" value is a caller bug; an empty one is
        // either a path that is gone or one that exists but cannot run.
        if (!resolvedHashcatPath.isEmpty() || QFileInfo::exists(hashcatPath)) {
            return LaunchError::HashcatNotExecutable;
        }
        return LaunchError::HashcatPathMissing;
    }

    if (configuredTerminal.isEmpty()) {
        return LaunchError::NoTerminal;
    }

    if (availableTerminals.isEmpty()) {
        return LaunchError::NoTerminals;
    }

    if (!availableTerminals.contains(configuredTerminal)) {
        if (detail) {
            *detail = availableTerminals.join(QStringLiteral(", "));
        }
        return LaunchError::UnknownTerminal;
    }

    return LaunchError::None;
}

// The value of a numeric setting when its text is a number in the given
// range. A session file sets widget text directly and bypasses the input
// validators, so every non-empty field is checked here again.
bool numberInRange(const QString &text, qlonglong min, qlonglong max)
{
    bool ok = false;
    const qlonglong value = text.trimmed().toLongLong(&ok);
    return ok && value >= min && value <= max;
}

// The key of a key=value token: everything before the first '='.
QString settingsKey(const QString &token)
{
    return token.left(token.indexOf(QLatin1Char('=')));
}

// True when the extra tokens hold a setting with the given key.
bool hasSetting(const QStringList &tokens, QLatin1StringView key)
{
    const QString prefix = QString(key) + QLatin1Char('=');
    return std::ranges::any_of(
        tokens, [&prefix](const QString &token) { return token.startsWith(prefix); });
}

// The value of the first token with the given key, or an empty string. A
// typed field and an extra token are the same setting to the feed, so the
// checks have to read either of them.
QString settingValue(const QStringList &tokens, QLatin1StringView key)
{
    const QString prefix = QString(key) + QLatin1Char('=');
    for (const QString &token : tokens) {
        if (token.startsWith(prefix)) {
            return token.mid(prefix.size());
        }
    }
    return {};
}

// The value a setting takes: the typed field first, the extra tokens second.
// Every cross-field check reads this, so a value cannot hide in either place.
QString effectiveSetting(const QString &typed, const QStringList &tokens, QLatin1StringView key)
{
    const QString value = typed.trimmed();
    return value.isEmpty() ? settingValue(tokens, key).trimmed() : value;
}

// The feed's boolean spellings (feed_param_store, FEED_PARAM_TYPE_BOOL):
// exactly these eight, case-sensitive. Anything else is no boolean at all.
std::optional<bool> feedBoolean(const QString &value)
{
    if (value == QLatin1String("1") || value == QLatin1String("yes")
        || value == QLatin1String("true") || value == QLatin1String("on")) {
        return true;
    }
    if (value == QLatin1String("0") || value == QLatin1String("no")
        || value == QLatin1String("false") || value == QLatin1String("off")) {
        return false;
    }
    return std::nullopt;
}

// The keys of the settings a typed field holds, counted once per field that
// is not empty. The field's own name is the key it is emitted under.
QStringList
typedSettingKeys(const std::initializer_list<std::pair<QString, QLatin1StringView>> &typed)
{
    QStringList keys;
    for (const auto &[value, name] : typed) {
        if (!value.trimmed().isEmpty()) {
            keys << QString(name);
        }
    }
    return keys;
}

// A key both a typed field and an extra token (or two tokens) hold: every
// feed refuses one ("<key>: given more than once"), but only after backend
// device init - and the GUI can create the collision itself, from a typed
// field plus an "Extra settings" token.
bool repeatedSetting(const QStringList &keys, const QStringList &tokens, QString *repeated)
{
    QStringList seen = keys;
    for (const QString &token : tokens) {
        const QString key = settingsKey(token);
        if (seen.contains(key)) {
            if (repeated) {
                *repeated = key;
            }
            return true;
        }
        seen << key;
    }
    return false;
}

// The wordlist hashcat multiplies (its ?q slot): the second row of combination
// and of mode 12. Everything else is a base wordlist and takes a folder - mode 7
// spells its mask first, but hashcat reads the dictionary as the base, and its
// rewrite leaves no second list at all.
int amplifierWordlistRow(AttackMode mode)
{
    switch (mode) {
    case AttackMode::Combination:
    case AttackMode::Hybrid:
        return 1;
    default:
        return -1;
    }
}

// Every extra token of the PCFG and table feeds must be a setting: the
// positional arguments end at the first key=value, so a bare token after one
// would be read as another ruleset or table.
bool bareExtraToken(const QStringList &tokens, QString *tokenOut)
{
    const auto bare = std::ranges::find_if(
        tokens, [](const QString &token) { return !token.contains(QLatin1Char('=')); });
    if (bare == tokens.cend()) {
        return false;
    }
    if (tokenOut) {
        *tokenOut = *bare;
    }
    return true;
}

QString namedSetting(QLatin1StringView name, const QString &value)
{
    return QString(name) + QLatin1Char('=') + value.trimmed();
}

// The extra-settings tail shared by both feeds: a bare token would be read as
// another positional file, and a key set in the UI must not be named twice.
LaunchError checkExtraSettings(const QStringList &keys, const QStringList &tokens,
                               LaunchError error, QString *detail)
{
    QString token;
    if (bareExtraToken(tokens, &token)) {
        if (detail) {
            *detail = token;
        }
        return error;
    }
    QString repeated;
    if (repeatedSetting(keys, tokens, &repeated)) {
        if (detail) {
            *detail = repeated + QStringLiteral(": given more than once");
        }
        return error;
    }
    return LaunchError::None;
}

// The checks of the mode 4 (PCFG) settings. The feed parses them only after
// the backend devices are set up, so a bad value would surface in the
// terminal long after the GUI is out of the picture - and maxgain's declared
// 1.0-64.0 range is never enforced upstream, so its check exists only here.
LaunchError checkPcfgSettings(const HashcatOptions &options, QString *detail)
{
    // The feed reads at most PCFG_MAXROOT rulesets and refuses more only
    // after backend device init.
    if (options.pcfgRulesets.size() > AppConstants::Hashcat::MaxPcfgRulesets) {
        if (detail) {
            *detail = QString::number(options.pcfgRulesets.size());
        }
        return LaunchError::TooManyPcfgRulesets;
    }

    const auto fail = [detail](const QString &what) -> LaunchError {
        if (detail) {
            *detail = what;
        }
        return LaunchError::InvalidPcfgSetting;
    };

    struct Bound {
        QString value;
        QLatin1StringView name;
        qlonglong min;
        qlonglong max;
    };
    // These are the feed's own parameter bounds. The auto default for threads
    // (16 on a fast hash, 8 on a slow one, capped by the machine) only fills
    // an absent setting; it does not widen the range a named one may take.
    const auto bounds = std::to_array<Bound>({
        {options.pcfgScale, AppConstants::HashcatSettings::Scale, 1, 64},
        {options.pcfgCostmax, AppConstants::HashcatSettings::Costmax, 1, 64},
        {options.pcfgThreads, AppConstants::HashcatSettings::Threads, 0, 16},
        {options.pcfgKbits, AppConstants::HashcatSettings::Kbits, 0, 27},
        {options.pcfgWalk, AppConstants::HashcatSettings::Walk, 0, 1},
    });
    for (const Bound &bound : bounds) {
        if (!bound.value.trimmed().isEmpty() && !numberInRange(bound.value, bound.min, bound.max)) {
            return fail(namedSetting(bound.name, bound.value));
        }
    }

    // maxword is a multiple of 16 that the kernel builds on: exactly 0, 16
    // or 32 (PCFG_DEV_MAXWORD_HI).
    if (!options.pcfgMaxword.trimmed().isEmpty()) {
        const QString maxword = options.pcfgMaxword.trimmed();
        if (maxword != QLatin1String("0") && maxword != QLatin1String("16")
            && maxword != QLatin1String("32")) {
            return fail(namedSetting(AppConstants::HashcatSettings::Maxword, maxword));
        }
    }

    if (!options.pcfgMaxgain.trimmed().isEmpty()) {
        bool ok = false;
        const double maxgain = options.pcfgMaxgain.trimmed().toDouble(&ok);
        if (!ok || maxgain < 1.0 || maxgain > 64.0) {
            return fail(namedSetting(AppConstants::HashcatSettings::Maxgain, options.pcfgMaxgain));
        }
    }

    if (!options.pcfgWeights.trimmed().isEmpty()) {
        const QString weights = options.pcfgWeights.trimmed();
        const QRegularExpression shape{QString(AppConstants::Validation::WeightsList)};
        const QStringList values = weights.split(QLatin1Char(':'));
        // A weight per ruleset; an empty list is the one bundled default.
        // The feed parses each share with strtod() and only requires it
        // above zero - finite, where strtod() would also take inf and nan.
        const qsizetype expected = std::max<qsizetype>(1, options.pcfgRulesets.size());
        if (!shape.match(weights).hasMatch() || values.size() != expected
            || std::ranges::any_of(values, [](const QString &value) {
                   bool ok = false;
                   const double share = value.toDouble(&ok);
                   return !ok || !std::isfinite(share) || share <= 0.0;
               })) {
            return fail(namedSetting(AppConstants::HashcatSettings::Weights, weights));
        }
    }

    // The mask may come from the typed field or from the extra settings line;
    // the feed sees one key either way, so every check below has to as well.
    const QString mask = effectiveSetting(options.pcfgMask, options.pcfgExtraSettings,
                                          AppConstants::HashcatSettings::Mask);

    if (!mask.isEmpty()) {
        // The ?w/?q markers belong to the hybrid mode 12, where hashcat
        // rewrites them; the PCFG grammar would take them literally.
        const CommandBuilder::MaskMarkers markers = CommandBuilder::maskMarkers(mask);
        if (markers.word > 0 || markers.second > 0) {
            return fail(namedSetting(AppConstants::HashcatSettings::Mask, mask));
        }
        // The feed refuses a named, non-zero omen with the mask. omen is a
        // U64, so omen=01 is omen=1; a value that is not a number is the
        // feed's own refusal, not this conflict.
        const QString omen = effectiveSetting(QString(), options.pcfgExtraSettings,
                                              AppConstants::HashcatSettings::Omen);
        bool omenOk = false;
        const qulonglong omenValue = omen.toULongLong(&omenOk, 10);
        if (omenOk && omenValue != 0) {
            return fail(namedSetting(AppConstants::HashcatSettings::Mask, mask));
        }
        // The hint words from the accounts have no source while the grammar
        // is held to the mask - which is exactly the case without a named
        // source; hintaccount=0 is no conflict.
        const QString hintaccount = effectiveSetting(QString(), options.pcfgExtraSettings,
                                                     AppConstants::HashcatSettings::Hintaccount);
        bool hintOk = false;
        const qlonglong hintCount = hintaccount.toLongLong(&hintOk);
        if (hintOk && hintCount > 0
            && !hasSetting(options.pcfgExtraSettings, AppConstants::HashcatSettings::Hintwords)
            && !hasSetting(options.pcfgExtraSettings, AppConstants::HashcatSettings::Hintfile)) {
            return fail(namedSetting(AppConstants::HashcatSettings::Mask, mask));
        }
    }

    // The feed refuses a named charset without a mask rather than ignoring it.
    // The builder emits one only while its checkbox is on and it has a value.
    // This is not a bad setting value: the mask is missing entirely, which is
    // what the dedicated launch error says.
    if (mask.isEmpty()) {
        for (const auto &charset : options.customCharsets) {
            if (charset.enabled && !charset.value.isEmpty()) {
                return LaunchError::PcfgCharsetWithoutMask;
            }
        }
    }

    return checkExtraSettings(
        typedSettingKeys({{options.pcfgScale, AppConstants::HashcatSettings::Scale},
                          {options.pcfgCostmax, AppConstants::HashcatSettings::Costmax},
                          {options.pcfgWeights, AppConstants::HashcatSettings::Weights},
                          {options.pcfgThreads, AppConstants::HashcatSettings::Threads},
                          {options.pcfgKbits, AppConstants::HashcatSettings::Kbits},
                          {options.pcfgMaxword, AppConstants::HashcatSettings::Maxword},
                          {options.pcfgMaxgain, AppConstants::HashcatSettings::Maxgain},
                          {options.pcfgWalk, AppConstants::HashcatSettings::Walk},
                          {options.pcfgMask, AppConstants::HashcatSettings::Mask}}),
        options.pcfgExtraSettings, LaunchError::InvalidPcfgSetting, detail);
}

// The checks of the mode 5 (table) settings, for the same reason as the
// PCFG ones above.
LaunchError checkTableSettings(const HashcatOptions &options, QString *detail)
{
    const auto fail = [detail](const QString &what) -> LaunchError {
        if (detail) {
            *detail = what;
        }
        return LaunchError::InvalidTableSetting;
    };

    // The three flags: empty (the default) or one of the feed's boolean
    // spellings - the typed field takes the same ones as the extra line.
    const auto flags = std::to_array<std::pair<QString, QLatin1StringView>>({
        {options.tableIdentity, AppConstants::HashcatSettings::Identity},
        {options.tableTemplate, AppConstants::HashcatSettings::Template},
        {options.tableSingle, AppConstants::HashcatSettings::Single},
    });
    for (const auto &[value, name] : flags) {
        const QString trimmed = value.trimmed();
        if (!trimmed.isEmpty() && !feedBoolean(trimmed)) {
            return fail(namedSetting(name, value));
        }
    }
    // One substitution needs the other positions left alone. Either flag may
    // arrive typed or as an extra token, in any boolean spelling.
    const QString single = effectiveSetting(options.tableSingle, options.tableExtraSettings,
                                            AppConstants::HashcatSettings::Single);
    const QString identity = effectiveSetting(options.tableIdentity, options.tableExtraSettings,
                                              AppConstants::HashcatSettings::Identity);
    if (feedBoolean(single) == true && feedBoolean(identity) == false) {
        return fail(namedSetting(AppConstants::HashcatSettings::Identity, identity));
    }

    // cap and maxperm reach beyond every Qt integer type (0...0xffffffff
    // and a full u64), so the digits are filtered with their own bounds.
    if (!options.tableCap.trimmed().isEmpty()) {
        bool ok = false;
        const qlonglong cap = options.tableCap.trimmed().toLongLong(&ok);
        if (!ok || cap < 0 || cap > Q_INT64_C(0xffffffff)) {
            return fail(namedSetting(AppConstants::HashcatSettings::Cap, options.tableCap));
        }
    }
    if (!options.tableMaxperm.trimmed().isEmpty()) {
        bool ok = false;
        options.tableMaxperm.trimmed().toULongLong(&ok);
        if (!ok) {
            return fail(namedSetting(AppConstants::HashcatSettings::Maxperm, options.tableMaxperm));
        }
    }

    return checkExtraSettings(
        typedSettingKeys({{options.tableIdentity, AppConstants::HashcatSettings::Identity},
                          {options.tableTemplate, AppConstants::HashcatSettings::Template},
                          {options.tableSingle, AppConstants::HashcatSettings::Single},
                          {options.tableCap, AppConstants::HashcatSettings::Cap},
                          {options.tableMaxperm, AppConstants::HashcatSettings::Maxperm}}),
        options.tableExtraSettings, LaunchError::InvalidTableSetting, detail);
}

// NUL and every C0/C1 control except tab. A NUL would silently end the token
// at exec, so the string hashcat receives is not the string the preview shows;
// the other controls are passed through to the terminal and the clipboard.
bool hasForbiddenControlCharacter(const QString &token)
{
    return std::ranges::any_of(token, [](QChar c) {
        return c.unicode() == 0 || (c.category() == QChar::Other_Control && c != QLatin1Char('\t'));
    });
}

// The offending token with its controls spelled out, so the message names the
// value without putting a control character into the dialog text.
QString showControlCharacters(const QString &token)
{
    QString shown;
    shown.reserve(token.size());
    for (const QChar c : token) {
        const ushort code = c.unicode();
        if (code == 0 || (c.category() == QChar::Other_Control && c != QLatin1Char('\t'))) {
            shown += QStringLiteral("\\x%1").arg(int(code), 2, 16, QLatin1Char('0'));
        } else {
            shown += c;
        }
    }
    return shown;
}

LaunchError validateOptions(const HashcatOptions &options, QString *detail)
{
    if (detail) {
        detail->clear();
    }
    const CommandBuilder::ModeRequirements requirements =
        CommandBuilder::modeRequirements(options.attackMode);
    const int wordlistCount = static_cast<int>(options.wordlists.size());

    // Combination is the one mode that pairs two named lists: any other
    // number is a wrong count, in both directions. The other modes with a
    // cap report a missing list below the minimum and a too-many above it.
    const bool exactCount = options.attackMode == AttackMode::Combination;

    if (wordlistCount < requirements.minWordlists) {
        if (exactCount) {
            if (detail) {
                *detail = QString::number(wordlistCount);
            }
            return LaunchError::WrongWordlistCount;
        }
        return LaunchError::NoWordlist;
    }

    if (requirements.maxWordlists >= 0 && wordlistCount > requirements.maxWordlists) {
        // A range, not an exact count: the message says at most, and for
        // mode 9 it names the GUI restriction (hashcat pairs two files).
        if (exactCount) {
            if (detail) {
                *detail = QString::number(wordlistCount);
            }
            return LaunchError::WrongWordlistCount;
        }
        if (detail) {
            *detail = QString::number(wordlistCount);
        }
        return LaunchError::TooManyWordlists;
    }

    // A mask mode without a mask would run an unintended default mask.
    if (requirements.needsMask && options.mask.trimmed().isEmpty()) {
        return LaunchError::NoMask;
    }

    if (options.attackMode == AttackMode::Table && options.tableFiles.isEmpty()) {
        return LaunchError::NoTable;
    }

    if (options.attackMode == AttackMode::Generic && options.feedName.trimmed().isEmpty()) {
        return LaunchError::NoFeed;
    }

    // The settings of the feed modes hashcat parses itself, and only after
    // it has set up the backend devices - by then the run has left the GUI.
    if (options.attackMode == AttackMode::Pcfg) {
        const LaunchError error = checkPcfgSettings(options, detail);
        if (error != LaunchError::None) {
            return error;
        }
    }

    if (options.attackMode == AttackMode::Table) {
        const LaunchError error = checkTableSettings(options, detail);
        if (error != LaunchError::None) {
            return error;
        }
    }

    // The mode 9 settings exist only while the words come from the hash
    // file. With a wordlist hashcat checks a token like phases=words,rules
    // as a path and stops with an error naming the setting, not the problem.
    if (options.attackMode == AttackMode::Association && !options.associationSettings.isEmpty()) {
        if (!options.wordlists.isEmpty()) {
            return LaunchError::AssociationSettingsWithWordlist;
        }
        const QRegularExpression shape{QString(AppConstants::Validation::AssociationSetting)};
        for (const QString &token : options.associationSettings) {
            if (!shape.match(token).hasMatch()) {
                // The value is not inspected: an unknown key or a phase that
                // is not running is refused by hashcat itself, with a message
                // that names the setting. A token that fails this shape would
                // be read as a wordlist upstream, so the GUI refuses it here.
                if (detail) {
                    *detail = token;
                }
                return LaunchError::InvalidAssociationSetting;
            }
        }
    }

    // Hash mode 4000 describes its hashes as a recipe; without one hashcat
    // refuses the run, and the row that takes the recipe only exists for
    // this type - so the combination cannot be fixed from a visible control.
    if (options.hashType == AppConstants::Hashcat::RecipeHashType
        && options.hashRecipe.trimmed().isEmpty()) {
        return LaunchError::NoHashRecipe;
    }

    // The hybrid mode 12 pairs the wordlists with the mask by markers; the
    // markers have to line up with what is selected.
    if (options.attackMode == AttackMode::Hybrid) {
        const CommandBuilder::MaskMarkers markers = CommandBuilder::maskMarkers(options.mask);
        if (markers.word == 0) {
            return LaunchError::MaskNeedsWordMarker;
        }
        if (markers.word > 1) {
            return LaunchError::MaskMultipleWordMarkers;
        }
        if (markers.second > 1) {
            return LaunchError::MaskMultipleQMarks;
        }
        if (markers.secondBeforeWord) {
            return LaunchError::MaskQBeforeWord;
        }
        if (wordlistCount == 1 && markers.second == 1) {
            return LaunchError::MaskQWithoutWordlist;
        }
        if (wordlistCount == 2 && markers.second == 0) {
            return LaunchError::SecondWordlistWithoutQ;
        }
    }

    // The other modes would take the markers for their literal ?w/?q or
    // rewrite them: hashcat only understands them in the hybrid mode 12.
    if (options.attackMode == AttackMode::BruteForce
        || options.attackMode == AttackMode::HybridWordMask
        || options.attackMode == AttackMode::HybridMaskWord) {
        const CommandBuilder::MaskMarkers markers = CommandBuilder::maskMarkers(options.mask);
        if (markers.word > 0 || markers.second > 0) {
            return LaunchError::MaskMarkersNotAllowed;
        }
    }

    // A folder is a base wordlist; the list hashcat multiplies is read by one
    // feed instance and has to be a file. The row follows from the mode, so the
    // dialog can name the one that has to be changed.
    const int amplifier = amplifierWordlistRow(options.attackMode);
    if (amplifier >= 0 && amplifier < wordlistCount) {
        const QString &path = options.wordlists.at(amplifier);
        if (!path.isEmpty() && QFileInfo(path).isDir()) {
            if (detail) {
                *detail = path;
            }
            return LaunchError::WordlistMustBeFile;
        }
    }

    // Hashcat refuses a minimum length that is larger than the maximum. The
    // builder emits the flag and its bounds only in the increment modes, so
    // a stale pair in a disabled group box must not block the other modes.
    if (options.increment && CommandBuilder::attackUsesIncrement(options.attackMode)
        && options.incrementMin > 0 && options.incrementMax > 0
        && options.incrementMin > options.incrementMax) {
        return LaunchError::InvalidIncrement;
    }

    // Two combinations hashcat answers with a usage error. The increment one
    // is gated like the bounds above, for the same reason: only an increment
    // mode puts -i on the command line. The mode 12 refusal is defence in
    // depth: the widget is hidden there, and collect drops the flag.
    if (options.increment && CommandBuilder::attackUsesIncrement(options.attackMode)
        && options.speedOnly) {
        return LaunchError::IncrementWithSpeedOnly;
    }
    if (options.increment && options.attackMode == AttackMode::Hybrid) {
        return LaunchError::IncrementInMode12;
    }

    if (options.remove && options.attackMode == AttackMode::Association) {
        return LaunchError::RemoveInAssociation;
    }

    // hashcat refuses the pair: removing a hash ends the guessing.
    if (options.keepGuessing && options.remove) {
        return LaunchError::KeepGuessingWithRemove;
    }

    // hashcat refuses a cache path that is not an existing directory; the
    // browse dialog only offers directories, so this one is a hand edit or
    // a directory that has gone away. Raw like the builder: whitespace is
    // emitted too.
    if (!options.cachePath.isEmpty() && !QFileInfo(options.cachePath).isDir()) {
        if (detail) {
            *detail = options.cachePath;
        }
        return LaunchError::InvalidCachePath;
    }

    // The format field is free text and reaches hashcat whenever the builder
    // emits --outfile-format (i.e. not with JSON and not at its default). The
    // widget's validator can be bypassed by a session, so check here too;
    // empty is invalid as well.
    if (!options.outfileJson && options.outfileFormat != AppConstants::Defaults::OutfileFormat) {
        const QRegularExpression format{QString(AppConstants::Validation::OutfileFormatList)};
        if (!format.match(options.outfileFormat).hasMatch()) {
            if (detail) {
                *detail = options.outfileFormat;
            }
            return LaunchError::InvalidOutfileFormat;
        }
    }

    // What the builder would hand to hashcat must be the text the user sees.
    // The check uses the built argv, so a value the mode leaves out of the
    // command line is not refused.
    const QStringList arguments = CommandBuilder::build(options);
    for (const QString &token : arguments) {
        if (hasForbiddenControlCharacter(token)) {
            if (detail) {
                *detail = showControlCharacters(token);
            }
            return LaunchError::InvalidCharacter;
        }
    }

    return LaunchError::None;
}

} // namespace

LaunchProblem validate(const LaunchContext &context)
{
    LaunchProblem problem;
    problem.code = validateOptions(context.options, &problem.detail);
    if (problem.code != LaunchError::None) {
        return problem;
    }
    problem.code = validateLaunch(context, &problem.detail);
    return problem;
}
} // namespace LaunchValidator
