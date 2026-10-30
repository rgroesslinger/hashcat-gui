/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "helperutils.h"
#include "settingsmanager.h"
#include "appconstants.h"
#include "commandbuilder.h"
#include <QProcess>
#include <QRegularExpression>
#include <QString>
#include <QStandardPaths>
#include <QFileInfo>
#include <QFuture>
#include <QGuiApplication>
#include <QStyleHints>
#include <QtConcurrent/QtConcurrentRun>

#include <algorithm>
#include <utility>

// Runs hashcat asynchronously; the caller attaches a QFutureWatcher and reads
// the result in its finished handler:
//   auto *w = new QFutureWatcher<HashcatResult>(this);
//   connect(w, &QFutureWatcher<HashcatResult>::finished, this, ...);
//   w->setFuture(HelperUtils::executeHashcat({"--help"}));
QFuture<HashcatResult> HelperUtils::executeHashcat(const QStringList &args, int timeoutMs)
{
    // QSettings is only reentrant. The lambda below runs on the thread pool,
    // where reading the settings could race with the GUI thread writing them,
    // so read the path once on the calling thread and capture it by value.
    const QString hashcatPath =
        SettingsManager::instance().getKey<QString>(AppConstants::SettingsKeys::HashcatPath);

    return QtConcurrent::run([args, timeoutMs, hashcatPath]() -> HashcatResult {
        HashcatResult result;
        QProcess proc;
        QStringList cmdArgs = args;

        if (hashcatPath.isEmpty()) {
            result.standardError = tr("The hashcat path is not configured.");
            return result;
        }

        // Always run in quiet mode when reading output
        cmdArgs << AppConstants::Hashcat::Quiet;

        proc.setProgram(hashcatPath);
        proc.setArguments(cmdArgs);
        proc.setWorkingDirectory(QFileInfo(hashcatPath).absolutePath());

        proc.start();

        if (!proc.waitForStarted()) {
            result.standardError = tr("Failed to start hashcat\n%1").arg(proc.errorString());
            return result;
        }

        if (!proc.waitForFinished(timeoutMs)) {
            proc.kill();
            // kill() is asynchronous: the wait blocks until the process has
            // actually exited, so the destructor of proc is not left with a
            // process that is still running.
            proc.waitForFinished(5000);
            result.standardError = tr("hashcat timed out\n%1").arg(proc.errorString());
            return result;
        }

        result.exitStatus = proc.exitStatus();
        result.exitCode = proc.exitCode();
        // QProcess hands back raw bytes; hashcat speaks UTF-8, so decode them
        // as such rather than through the conversion this project compiles without.
        result.standardOutput = QString::fromUtf8(proc.readAllStandardOutput());
        result.standardError = QString::fromUtf8(proc.readAllStandardError());

        return result;
    });
}

HelperUtils::LaunchError HelperUtils::validateLaunch(const QString &hashFile,
                                                     const QString &hashcatPath,
                                                     const QString &configuredTerminal,
                                                     const QStringList &availableTerminals,
                                                     QString *detail)
{
    if (detail) {
        detail->clear();
    }

    if (hashFile.isEmpty()) {
        return LaunchError::NoHashFile;
    }

    if (hashcatPath.isEmpty()) {
        return LaunchError::NoHashcatPath;
    }

    // Only absolute paths are checked here. A bare name is left to the shell
    // search of the terminal, and checking it would need the executable bit,
    // which QFileInfo does not report reliably for .exe files.
    const QFileInfo hashcatInfo(hashcatPath);
    if (hashcatInfo.isAbsolute() && !hashcatInfo.isFile()) {
        if (detail) {
            *detail = hashcatPath;
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

namespace
{
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
    return std::any_of(tokens.cbegin(), tokens.cend(),
                       [&prefix](const QString &token) { return token.startsWith(prefix); });
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
    return QString();
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
    for (const QString &token : tokens) {
        if (!token.contains(QLatin1Char('='))) {
            if (tokenOut) {
                *tokenOut = token;
            }
            return true;
        }
    }
    return false;
}

// The checks of the mode 4 (PCFG) settings. The feed parses them only after
// the backend devices are set up, so a bad value would surface in the
// terminal long after the GUI is out of the picture - and maxgain's declared
// 1.0-64.0 range is never enforced upstream, so its check exists only here.
HelperUtils::LaunchError checkPcfgSettings(const HashcatOptions &options, QString *detail)
{
    const auto fail = [detail](const QString &what) -> HelperUtils::LaunchError {
        if (detail) {
            *detail = what;
        }
        return HelperUtils::LaunchError::InvalidPcfgSetting;
    };
    const auto named = [](QLatin1StringView name, const QString &value) {
        return QString(name) + QLatin1Char('=') + value.trimmed();
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
    const Bound bounds[] = {
        {options.pcfgScale, AppConstants::HashcatSettings::Scale, 1, 64},
        {options.pcfgCostmax, AppConstants::HashcatSettings::Costmax, 1, 64},
        {options.pcfgThreads, AppConstants::HashcatSettings::Threads, 0, 16},
        {options.pcfgKbits, AppConstants::HashcatSettings::Kbits, 0, 27},
        {options.pcfgWalk, AppConstants::HashcatSettings::Walk, 0, 1},
    };
    for (const Bound &bound : bounds) {
        if (!bound.value.trimmed().isEmpty() && !numberInRange(bound.value, bound.min, bound.max)) {
            return fail(named(bound.name, bound.value));
        }
    }

    // maxword is a multiple of 16 that the kernel builds on: exactly 0, 16
    // or 32 (PCFG_DEV_MAXWORD_HI).
    if (!options.pcfgMaxword.trimmed().isEmpty()) {
        const QString maxword = options.pcfgMaxword.trimmed();
        if (maxword != QLatin1String("0") && maxword != QLatin1String("16")
            && maxword != QLatin1String("32")) {
            return fail(named(AppConstants::HashcatSettings::Maxword, maxword));
        }
    }

    if (!options.pcfgMaxgain.trimmed().isEmpty()) {
        bool ok = false;
        const double maxgain = options.pcfgMaxgain.trimmed().toDouble(&ok);
        if (!ok || maxgain < 1.0 || maxgain > 64.0) {
            return fail(named(AppConstants::HashcatSettings::Maxgain, options.pcfgMaxgain));
        }
    }

    if (!options.pcfgWeights.trimmed().isEmpty()) {
        const QString weights = options.pcfgWeights.trimmed();
        const QRegularExpression shape{QString(AppConstants::Validation::WeightsList)};
        const QStringList values = weights.split(QLatin1Char(':'));
        // A weight per ruleset; an empty list is the one bundled default.
        const qsizetype expected = std::max<qsizetype>(1, options.pcfgRulesets.size());
        if (!shape.match(weights).hasMatch() || values.size() != expected
            || std::any_of(values.cbegin(), values.cend(),
                           [](const QString &value) { return value.toLongLong() <= 0; })) {
            return fail(named(AppConstants::HashcatSettings::Weights, weights));
        }
    }

    // The mask may come from the typed field or from the extra settings line;
    // the feed sees one key either way, so every check below has to as well.
    QString mask = options.pcfgMask.trimmed();
    if (mask.isEmpty()) {
        mask =
            settingValue(options.pcfgExtraSettings, AppConstants::HashcatSettings::Mask).trimmed();
    }

    if (!mask.isEmpty()) {
        // The ?w/?q markers belong to the hybrid mode 12, where hashcat
        // rewrites them; the PCFG grammar would take them literally.
        const CommandBuilder::MaskMarkers markers = CommandBuilder::maskMarkers(mask);
        if (markers.word > 0 || markers.second > 0) {
            return fail(named(AppConstants::HashcatSettings::Mask, mask));
        }
        // The feed refuses the pair with omen=1, and it refuses the hint
        // words from the accounts while the grammar is held to the mask -
        // which is exactly the case without a named source; hintaccount=0
        // is no conflict.
        const QString omen =
            QString(AppConstants::HashcatSettings::Omen) + QLatin1Char('=') + QStringLiteral("1");
        if (options.pcfgExtraSettings.contains(omen)) {
            return fail(named(AppConstants::HashcatSettings::Mask, mask));
        }
        for (const QString &token : options.pcfgExtraSettings) {
            if (!token.startsWith(QString(AppConstants::HashcatSettings::Hintaccount)
                                  + QLatin1Char('='))) {
                continue;
            }
            bool ok = false;
            const qlonglong hintaccount =
                token.mid(token.indexOf(QLatin1Char('=')) + 1).toLongLong(&ok);
            if (ok && hintaccount > 0
                && !hasSetting(options.pcfgExtraSettings, AppConstants::HashcatSettings::Hintwords)
                && !hasSetting(options.pcfgExtraSettings,
                               AppConstants::HashcatSettings::Hintfile)) {
                return fail(named(AppConstants::HashcatSettings::Mask, mask));
            }
        }
    }

    // The feed refuses a named charset without a mask rather than ignoring it.
    // The builder emits one only while its checkbox is on and it has a value.
    if (mask.isEmpty()) {
        for (const auto &charset : options.customCharsets) {
            if (charset.enabled && !charset.value.isEmpty()) {
                return fail(named(AppConstants::HashcatSettings::Mask, mask));
            }
        }
    }

    QString token;
    if (bareExtraToken(options.pcfgExtraSettings, &token)) {
        return fail(token);
    }
    QString repeated;
    const QStringList keys =
        typedSettingKeys({{options.pcfgScale, AppConstants::HashcatSettings::Scale},
                          {options.pcfgCostmax, AppConstants::HashcatSettings::Costmax},
                          {options.pcfgWeights, AppConstants::HashcatSettings::Weights},
                          {options.pcfgThreads, AppConstants::HashcatSettings::Threads},
                          {options.pcfgKbits, AppConstants::HashcatSettings::Kbits},
                          {options.pcfgMaxword, AppConstants::HashcatSettings::Maxword},
                          {options.pcfgMaxgain, AppConstants::HashcatSettings::Maxgain},
                          {options.pcfgWalk, AppConstants::HashcatSettings::Walk},
                          {options.pcfgMask, AppConstants::HashcatSettings::Mask}});
    if (repeatedSetting(keys, options.pcfgExtraSettings, &repeated)) {
        return fail(repeated + QStringLiteral(": given more than once"));
    }

    return HelperUtils::LaunchError::None;
}

// The checks of the mode 5 (table) settings, for the same reason as the
// PCFG ones above.
HelperUtils::LaunchError checkTableSettings(const HashcatOptions &options, QString *detail)
{
    const auto fail = [detail](const QString &what) -> HelperUtils::LaunchError {
        if (detail) {
            *detail = what;
        }
        return HelperUtils::LaunchError::InvalidTableSetting;
    };
    const auto named = [](QLatin1StringView name, const QString &value) {
        return QString(name) + QLatin1Char('=') + value.trimmed();
    };

    // The three flags: empty (the default), 0 or 1.
    const std::pair<QString, QLatin1StringView> flags[] = {
        {options.tableIdentity, AppConstants::HashcatSettings::Identity},
        {options.tableTemplate, AppConstants::HashcatSettings::Template},
        {options.tableSingle, AppConstants::HashcatSettings::Single},
    };
    for (const auto &[value, name] : flags) {
        const QString trimmed = value.trimmed();
        if (!trimmed.isEmpty() && trimmed != QLatin1String("0") && trimmed != QLatin1String("1")) {
            return fail(named(name, value));
        }
    }
    // One substitution needs the other positions left alone.
    if (options.tableSingle.trimmed() == QLatin1String("1")
        && options.tableIdentity.trimmed() == QLatin1String("0")) {
        return fail(named(AppConstants::HashcatSettings::Identity, options.tableIdentity));
    }

    // cap and maxperm reach beyond every Qt integer type (0...0xffffffff
    // and a full u64), so the digits are filtered with their own bounds.
    if (!options.tableCap.trimmed().isEmpty()) {
        bool ok = false;
        const qlonglong cap = options.tableCap.trimmed().toLongLong(&ok);
        if (!ok || cap < 0 || cap > Q_INT64_C(0xffffffff)) {
            return fail(named(AppConstants::HashcatSettings::Cap, options.tableCap));
        }
    }
    if (!options.tableMaxperm.trimmed().isEmpty()) {
        bool ok = false;
        options.tableMaxperm.trimmed().toULongLong(&ok);
        if (!ok) {
            return fail(named(AppConstants::HashcatSettings::Maxperm, options.tableMaxperm));
        }
    }

    QString token;
    if (bareExtraToken(options.tableExtraSettings, &token)) {
        return fail(token);
    }
    QString repeated;
    const QStringList keys =
        typedSettingKeys({{options.tableIdentity, AppConstants::HashcatSettings::Identity},
                          {options.tableTemplate, AppConstants::HashcatSettings::Template},
                          {options.tableSingle, AppConstants::HashcatSettings::Single},
                          {options.tableCap, AppConstants::HashcatSettings::Cap},
                          {options.tableMaxperm, AppConstants::HashcatSettings::Maxperm}});
    if (repeatedSetting(keys, options.tableExtraSettings, &repeated)) {
        return fail(repeated + QStringLiteral(": given more than once"));
    }

    return HelperUtils::LaunchError::None;
}
} // namespace

HelperUtils::LaunchError HelperUtils::validateOptions(const HashcatOptions &options,
                                                      QString *detail)
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
                // that names the setting. A token that fails the shape is not
                // refused at all: upstream would read it as a wordlist.
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

    // hashcat refuses a cache path that is not an existing directory; the
    // browse dialog only offers directories, so this one is a hand edit or
    // a directory that has gone away.
    if (!options.cachePath.trimmed().isEmpty() && !QFileInfo(options.cachePath).isDir()) {
        return LaunchError::InvalidCachePath;
    }

    return LaunchError::None;
}

// The arguments that belong between the terminal program and the hashcat
// command line, for every supported terminal. The entries are pure data -
// nothing here looks at the filesystem. Terminals whose option takes a single
// shell-quoted string (terminator -e) are deliberately not listed.
//
// PowerShell's -Command must come last: it takes the rest of the command line
// as the command text, which is what startClicked() builds. The & is the call
// operator and needs an entry of its own, because a hashcat path with spaces
// arrives quoted - and a bare quoted string is only a string in PowerShell.
//
// gnome-terminal, ptyxis and wezterm have no flag to stay open after the
// command exits; holding them would need the command wrapped in a shell
// string, which the line above rules out. cmd.exe /k stays open, but breaks as
// soon as two arguments need quotes: /K then strips the first and last quote
// of the whole command line.
const QMap<QString, QStringList> &HelperUtils::terminalArguments()
{
    static const QMap<QString, QStringList> table = {
        {QStringLiteral("cmd.exe"), {QStringLiteral("/k")}},
        // No .exe on pwsh so the lookup also resolves on Linux and macOS.
        {QStringLiteral("powershell.exe"),
         {QStringLiteral("-NoExit"), QStringLiteral("-NoProfile"), QStringLiteral("-Command"),
          QStringLiteral("&")}},
        {QStringLiteral("pwsh"),
         {QStringLiteral("-NoExit"), QStringLiteral("-NoProfile"), QStringLiteral("-Command"),
          QStringLiteral("&")}},
        {QStringLiteral("xterm"), {QStringLiteral("-hold"), QStringLiteral("-e")}},
        {QStringLiteral("gnome-terminal"), {QStringLiteral("--wait"), QStringLiteral("--")}},
        {QStringLiteral("ptyxis"), {QStringLiteral("--")}},
        {QStringLiteral("konsole"), {QStringLiteral("--hold"), QStringLiteral("-e")}},
        {QStringLiteral("xfce4-terminal"), {QStringLiteral("--hold"), QStringLiteral("-x")}},
        {QStringLiteral("alacritty"), {QStringLiteral("--hold"), QStringLiteral("-e")}},
        {QStringLiteral("kitty"), {QStringLiteral("--hold")}},
        {QStringLiteral("foot"), {QStringLiteral("--hold")}},
        {QStringLiteral("wezterm"), {QStringLiteral("start"), QStringLiteral("--")}},
    };
    return table;
}

QMap<QString, QStringList> HelperUtils::availableTerminals()
{
    QMap<QString, QStringList> terminals;

    const QMap<QString, QStringList> &table = terminalArguments();
    for (auto it = table.constBegin(); it != table.constEnd(); ++it) {
        QString fullPath = QStandardPaths::findExecutable(it.key());
        if (!fullPath.isEmpty()) {
            terminals.insert(it.key(), it.value());
        }
    }

    return terminals;
}

QString HelperUtils::findHashcatInPath()
{
    // QStandardPaths checks that the file exists and can be executed; the
    // platform's executable name already carries its suffix.
    return QStandardPaths::findExecutable(QString(AppConstants::Hashcat::ExecutableName));
}

QString HelperUtils::noSupportedTerminalMessage()
{
    return tr("No supported terminal was found on this system. "
              "hashcat-gui needs one to show the hashcat output.");
}

void HelperUtils::applyColorScheme(const QString &scheme)
{
    QStyleHints *hints = QGuiApplication::styleHints();
    if (!hints) {
        return;
    }

    if (scheme == AppConstants::Appearance::ColorSchemeDark) {
        hints->setColorScheme(Qt::ColorScheme::Dark);
    } else if (scheme == AppConstants::Appearance::ColorSchemeLight) {
        hints->setColorScheme(Qt::ColorScheme::Light);
    } else {
        // The system default: the desktop decides again.
        hints->unsetColorScheme();
    }
}
