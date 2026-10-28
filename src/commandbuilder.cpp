/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "commandbuilder.h"
#include "appconstants.h"

#include <QDateTime>
#include <QFileInfo>
#include <QMap>

bool CommandBuilder::attackUsesMask(AttackMode mode)
{
    return mode == AttackMode::BruteForce || mode == AttackMode::HybridWordMask
           || mode == AttackMode::HybridMaskWord || mode == AttackMode::Hybrid;
}

// -i walks a mask up from its minimum length. Hashcat takes it in mode 1 as
// well, but that mode has no mask in the GUI - the increment widgets live in
// the mask group - and mode 12 conflicts with it upstream, so the flag is
// offered where a mask is in use and incrementing is accepted.
bool CommandBuilder::attackUsesIncrement(AttackMode mode)
{
    return mode == AttackMode::BruteForce || mode == AttackMode::HybridWordMask
           || mode == AttackMode::HybridMaskWord;
}

// Modes 3, 4 and 8 make their candidates without a word list; every other
// mode reads at least one. Which of them read exactly one is a question of
// ModeRequirements, not of this predicate.
bool CommandBuilder::attackUsesWordlists(AttackMode mode)
{
    return mode != AttackMode::BruteForce && mode != AttackMode::Pcfg
           && mode != AttackMode::Generic;
}

// Rules apply in every attack mode since hashcat 8.0.0 (upstream AM_ALL);
// rules now work in 1, 3, 6, 7 and 12 as well, where the GUI used to leave
// the group box empty.
bool CommandBuilder::attackUsesRules(AttackMode mode)
{
    return mode == AttackMode::Straight || mode == AttackMode::Combination
           || mode == AttackMode::BruteForce || mode == AttackMode::Pcfg
           || mode == AttackMode::Table || mode == AttackMode::HybridWordMask
           || mode == AttackMode::HybridMaskWord || mode == AttackMode::Generic
           || mode == AttackMode::Association || mode == AttackMode::Hybrid;
}

// -j applies to the wordlist on the left. That is the only wordlist in modes
// 0, 6 and 9, and the first one in combination mode. Modes 4 and 5 are left
// out on purpose: their candidates come from the device engine, which refuses
// --rule-left late, and upstream's own advice there is to use -r.
bool CommandBuilder::attackUsesRuleLeft(AttackMode mode)
{
    return mode == AttackMode::Straight || mode == AttackMode::Combination
           || mode == AttackMode::HybridWordMask || mode == AttackMode::HybridMaskWord
           || mode == AttackMode::Generic || mode == AttackMode::Association
           || mode == AttackMode::Hybrid;
}

// -k applies to the wordlist on the right, which only combination mode (two
// wordlists) and the mask + wordlist modes provide.
bool CommandBuilder::attackUsesRuleRight(AttackMode mode)
{
    return mode == AttackMode::Combination || mode == AttackMode::HybridMaskWord
           || mode == AttackMode::Hybrid;
}

// Custom charsets name parts of a mask (-1 ... -4). The PCFG feed takes the
// same markers in its mask= setting, so mode 4 needs them as well; mode 8
// has no mask field of its own.
bool CommandBuilder::attackUsesCustomCharset(AttackMode mode)
{
    return mode == AttackMode::BruteForce || mode == AttackMode::Pcfg
           || mode == AttackMode::HybridWordMask || mode == AttackMode::HybridMaskWord
           || mode == AttackMode::Hybrid;
}

bool CommandBuilder::attackUsesHexWordlist(AttackMode mode)
{
    return mode == AttackMode::Straight || mode == AttackMode::Combination
           || mode == AttackMode::Table || mode == AttackMode::HybridWordMask
           || mode == AttackMode::HybridMaskWord || mode == AttackMode::Generic
           || mode == AttackMode::Association || mode == AttackMode::Hybrid;
}

// The table feed reads its wordlist itself and refuses the encoding pair;
// every other wordlist mode takes it.
bool CommandBuilder::attackUsesWordlistEncoding(AttackMode mode)
{
    return mode == AttackMode::Straight || mode == AttackMode::Combination
           || mode == AttackMode::HybridWordMask || mode == AttackMode::HybridMaskWord
           || mode == AttackMode::Generic || mode == AttackMode::Association
           || mode == AttackMode::Hybrid;
}

QString CommandBuilder::expandOutfileTemplate(const QString &tmplate, const QString &hashFile)
{
    QString outfile = tmplate;
    outfile.replace(
        AppConstants::Placeholders::UnixTime,
        QString::number(QDateTime::currentMSecsSinceEpoch() / AppConstants::MsecsPerSecond));
    outfile.replace(AppConstants::Placeholders::Hash, QFileInfo(hashFile).fileName(),
                    Qt::CaseInsensitive);
    return outfile;
}

// The markers of attack mode 12. ?? is a literal question mark, every other
// ?x is a charset token, and the markers exist only in lower case - ?W and
// ?Q are not them.
CommandBuilder::MaskMarkers CommandBuilder::maskMarkers(const QString &mask)
{
    MaskMarkers markers;

    for (int i = 0; i < mask.size(); ++i) {
        if (mask.at(i) != QLatin1Char('?')) {
            continue;
        }
        if (i + 1 >= mask.size()) {
            break; // a trailing ? stands for itself
        }

        const QChar token = mask.at(++i);
        if (token == QLatin1Char('?')) {
            continue; // the literal pair is consumed
        }
        if (token == QLatin1Char('w')) {
            ++markers.word;
        } else if (token == QLatin1Char('q')) {
            ++markers.second;
            if (markers.word == 0) {
                markers.secondBeforeWord = true;
            }
        }
    }

    return markers;
}

namespace
{

// One positional key=value setting of the feed modes. An empty value means
// "not set" and stays out of the command line; hashcat refuses a key it is
// given more than once, so there is exactly one field per key.
void appendSetting(QStringList &arguments, QLatin1StringView key, const QString &value)
{
    if (!value.isEmpty()) {
        arguments << QStringLiteral("%1=%2").arg(QString(key), value);
    }
}

} // namespace

// What the user has to provide for the attack mode: hashcat's own argument
// validation decides which numbers of word lists a mode accepts.
//
// The mask and the word lists come from the predicates above, so the group
// boxes, the mode hint and the launch validation cannot tell the user
// different things about the same mode.
CommandBuilder::ModeRequirements CommandBuilder::modeRequirements(AttackMode mode)
{
    ModeRequirements requirements;

    requirements.needsMask = attackUsesMask(mode);

    switch (mode) {
    case AttackMode::Combination:
        // Combination multiplies two word lists. Hashcat answers every other
        // number with a usage error.
        requirements.minWordlists = 2;
        requirements.maxWordlists = 2;
        break;
    case AttackMode::Table:
        // The table feed treats every argument after the wordlist as a
        // table, so exactly one wordlist and at least one table.
        requirements.minWordlists = 1;
        requirements.maxWordlists = 1;
        break;
    case AttackMode::Association:
        // The GUI caps the pairing at one wordlist; hashcat reads two.
        // Without one the words come from the hash file, so zero is valid.
        requirements.minWordlists = 0;
        requirements.maxWordlists = 1;
        break;
    case AttackMode::Hybrid:
        // The ?w/?q hybrid pairs one or two wordlists with the mask.
        requirements.minWordlists = 1;
        requirements.maxWordlists = 2;
        break;
    case AttackMode::BruteForce:
    case AttackMode::Pcfg:
    case AttackMode::Generic:
        // These modes never read the wordlist list: it is hidden in the UI
        // and the builder does not emit it, so a row left checked there must
        // not count towards a limit. max stays -1, "unchecked".
        requirements.minWordlists = 0;
        break;
    default:
        // Any number of word lists, but at least one.
        requirements.minWordlists = 1;
        break;
    }

    return requirements;
}

QString CommandBuilder::parameterName(Parameter key, bool useShort)
{
    // Built once, on first call: a namespace-scope QMap with runtime
    // initialization could throw during pre-main static initialization and
    // end in std::terminate, so the table is a deferred function-local static.
    static const QMap<Parameter, QPair<QString, QString>> parameterMap = {
        {Parameter::AttackMode, {QStringLiteral("-a"), QStringLiteral("--attack-mode")}},
        {Parameter::BackendDevices, {QStringLiteral("-d"), QStringLiteral("--backend-devices")}},
        {Parameter::CpuAffinity, {QString(), QStringLiteral("--cpu-affinity")}},
        {Parameter::CustomCharset1, {QStringLiteral("-1"), QStringLiteral("--custom-charset1")}},
        {Parameter::CustomCharset2, {QStringLiteral("-2"), QStringLiteral("--custom-charset2")}},
        {Parameter::CustomCharset3, {QStringLiteral("-3"), QStringLiteral("--custom-charset3")}},
        {Parameter::CustomCharset4, {QStringLiteral("-4"), QStringLiteral("--custom-charset4")}},
        {Parameter::EncodingFrom, {QString(), QStringLiteral("--encoding-from")}},
        {Parameter::EncodingTo, {QString(), QStringLiteral("--encoding-to")}},
        {Parameter::Force, {QString(), QStringLiteral("--force")}},
        {Parameter::GenerateRules, {QStringLiteral("-g"), QStringLiteral("--generate-rules")}},
        {Parameter::HashType, {QStringLiteral("-m"), QStringLiteral("--hash-type")}},
        {Parameter::HashRecipe, {QString(), QString(AppConstants::Hashcat::HashRecipe)}},
        {Parameter::HexCharset, {QString(), QStringLiteral("--hex-charset")}},
        {Parameter::HexSalt, {QString(), QStringLiteral("--hex-salt")}},
        {Parameter::HexWordlist, {QString(), QStringLiteral("--hex-wordlist")}},
        {Parameter::HwmonTempAbort, {QString(), QStringLiteral("--hwmon-temp-abort")}},
        {Parameter::Increment, {QStringLiteral("-i"), QStringLiteral("--increment")}},
        {Parameter::IncrementMax, {QString(), QStringLiteral("--increment-max")}},
        {Parameter::IncrementMin, {QString(), QStringLiteral("--increment-min")}},
        {Parameter::Limit, {QStringLiteral("-l"), QStringLiteral("--limit")}},
        {Parameter::OpenclDeviceTypes,
         {QStringLiteral("-D"), QStringLiteral("--opencl-device-types")}},
        {Parameter::OptimizedKernel,
         {QStringLiteral("-O"), QStringLiteral("--optimized-kernel-enable")}},
        {Parameter::Outfile, {QStringLiteral("-o"), QStringLiteral("--outfile")}},
        {Parameter::OutfileFormat, {QString(), QStringLiteral("--outfile-format")}},
        {Parameter::PotfileDisable, {QString(), QStringLiteral("--potfile-disable")}},
        {Parameter::Remove, {QString(), QStringLiteral("--remove")}},
        {Parameter::RuleLeft, {QStringLiteral("-j"), QStringLiteral("--rule-left")}},
        {Parameter::RuleRight, {QStringLiteral("-k"), QStringLiteral("--rule-right")}},
        {Parameter::RulesFile, {QStringLiteral("-r"), QStringLiteral("--rules-file")}},
        {Parameter::Runtime, {QString(), QStringLiteral("--runtime")}},
        {Parameter::Session, {QString(), QStringLiteral("--session")}},
        {Parameter::Skip, {QStringLiteral("-s"), QStringLiteral("--skip")}},
        {Parameter::SpeedOnly, {QString(), QStringLiteral("--speed-only")}},
        {Parameter::Status, {QString(), QStringLiteral("--status")}},
        {Parameter::StatusTimer, {QString(), QStringLiteral("--status-timer")}},
        {Parameter::Username, {QString(), QStringLiteral("--username")}},
    };

    auto it = parameterMap.constFind(key);
    if (it == parameterMap.constEnd()) {
        // Every enumerator has an entry above, but dereferencing end() would
        // be undefined behavior, so fail soft instead.
        return {};
    }
    const auto &pair = it.value();

    if (useShort && !pair.first.isEmpty()) {
        return pair.first;
    }

    return pair.second;
}

QStringList CommandBuilder::build(const HashcatOptions &options)
{
    const bool useShort = options.useShortParameters;
    const AttackMode mode = options.attackMode;
    QStringList arguments;

    // hash type - left out while none is selected
    if (options.hashType >= 0) {
        arguments << parameterName(Parameter::HashType, useShort)
                  << QString::number(options.hashType);
    }

    // The recipe belongs to hash mode 4000; hashcat refuses the flag for
    // every other type, so a stale value a session carried is left out
    // instead of breaking the run.
    if (options.hashType == AppConstants::Hashcat::RecipeHashType
        && !options.hashRecipe.trimmed().isEmpty()) {
        arguments << parameterName(Parameter::HashRecipe, useShort) << options.hashRecipe;
    }

    arguments << parameterName(Parameter::AttackMode, useShort)
              << QString::number(static_cast<int>(mode));

    if (options.force) {
        arguments << parameterName(Parameter::Force, useShort);
    }

    if (options.remove) {
        arguments << parameterName(Parameter::Remove, useShort);
    }

    if (options.ignoreUsername) {
        arguments << parameterName(Parameter::Username, useShort);
    }

    if (!options.session.isEmpty()) {
        arguments << parameterName(Parameter::Session, useShort) << options.session;
    }

    if (options.runtime > 0) {
        arguments << parameterName(Parameter::Runtime, useShort)
                  << QString::number(options.runtime);
    }

    if (options.status) {
        arguments << parameterName(Parameter::Status, useShort);
        // hashcat's own default interval is 10 s, so the GUI always passes its
        // own: omitting it would silently update every 10 s instead of the
        // interval the spin box shows.
        arguments << parameterName(Parameter::StatusTimer, useShort)
                  << QString::number(options.statusTimer);
    }

    if (options.potfileDisable) {
        arguments << parameterName(Parameter::PotfileDisable, useShort);
    }

    // rules - either existing rule files or a number of generated rules
    if (attackUsesRules(mode)) {
        for (const QString &rulesFile : options.rulesFiles) {
            arguments << parameterName(Parameter::RulesFile, useShort) << rulesFile;
        }
        if (options.rulesFiles.isEmpty() && options.generateRules > 0) {
            arguments << parameterName(Parameter::GenerateRules, useShort)
                      << QString::number(options.generateRules);
        }
    }

    if (attackUsesRuleLeft(mode) && !options.ruleLeft.isEmpty()) {
        arguments << parameterName(Parameter::RuleLeft, useShort) << options.ruleLeft;
    }
    if (attackUsesRuleRight(mode) && !options.ruleRight.isEmpty()) {
        arguments << parameterName(Parameter::RuleRight, useShort) << options.ruleRight;
    }

    // mask - its position relative to the dictionaries depends on the mode
    QString maskBeforeDict;
    QString maskAfterDict;
    switch (mode) {
    case AttackMode::BruteForce:
        maskBeforeDict = options.mask;
        break;
    case AttackMode::HybridWordMask:
        maskAfterDict = options.mask;
        break;
    case AttackMode::HybridMaskWord:
        maskBeforeDict = options.mask;
        break;
    case AttackMode::Hybrid:
        // hash mask wordlists - the ?w in the mask names where a word from
        // the list goes.
        maskBeforeDict = options.mask;
        break;
    default:
        break;
    }

    if (options.speedOnly) {
        arguments << parameterName(Parameter::SpeedOnly, useShort);
    }

    if (options.optimizedKernel) {
        arguments << parameterName(Parameter::OptimizedKernel, useShort);
    }

    if (!options.skip.isEmpty()) {
        arguments << parameterName(Parameter::Skip, useShort) << options.skip;
    }

    if (!options.limit.isEmpty()) {
        arguments << parameterName(Parameter::Limit, useShort) << options.limit;
    }

    // Incrementing only makes sense while a mask is in use, and only in the
    // modes that accept the flag: the two bounds are part of the same option,
    // and without the flag they would be ignored.
    if (attackUsesIncrement(mode) && options.increment) {
        arguments << parameterName(Parameter::Increment, useShort);
        if (options.incrementMin > 0) {
            arguments << parameterName(Parameter::IncrementMin, useShort)
                      << QString::number(options.incrementMin);
        }
        if (options.incrementMax > 0) {
            arguments << parameterName(Parameter::IncrementMax, useShort)
                      << QString::number(options.incrementMax);
        }
    }

    // Custom charsets are only meaningful together with a mask. The UI
    // disables the group box for the other attack modes; here that gating is
    // expressed directly so a stale checkbox cannot leak -1 ... -4 into a
    // command that does not use them.
    if (attackUsesCustomCharset(mode)) {
        static const Parameter charsetParameters[4] = {
            Parameter::CustomCharset1,
            Parameter::CustomCharset2,
            Parameter::CustomCharset3,
            Parameter::CustomCharset4,
        };
        for (int i = 0; i < 4; ++i) {
            const auto &charset = options.customCharsets[i];
            if (charset.enabled && !charset.value.isEmpty()) {
                arguments << parameterName(charsetParameters[i], useShort) << charset.value;
            }
        }
    }

    // --hex-charset describes the masks and custom charsets only; in the
    // other attack modes hashcat would accept and ignore it.
    if (attackUsesCustomCharset(mode) && options.hexCharset) {
        arguments << parameterName(Parameter::HexCharset, useShort);
    }

    if (options.hexSalt) {
        arguments << parameterName(Parameter::HexSalt, useShort);
    }

    // Wordlist interpretation only applies while a wordlist is read, and the
    // table feed takes neither: it reads its wordlist itself.
    if (attackUsesHexWordlist(mode) && options.hexWordlist) {
        arguments << parameterName(Parameter::HexWordlist, useShort);
    }
    if (attackUsesWordlistEncoding(mode)) {
        if (!options.encodingFrom.isEmpty()) {
            arguments << parameterName(Parameter::EncodingFrom, useShort) << options.encodingFrom;
        }
        if (!options.encodingTo.isEmpty()) {
            arguments << parameterName(Parameter::EncodingTo, useShort) << options.encodingTo;
        }
    }

    if (options.outfileEnabled && !options.outfile.isEmpty()) {
        arguments << parameterName(Parameter::Outfile, useShort)
                  << expandOutfileTemplate(options.outfile, options.hashFile);
    }

    if (options.outfileFormat != AppConstants::Defaults::OutfileFormat) {
        arguments << parameterName(Parameter::OutfileFormat, useShort) << options.outfileFormat;
    }

    if (!options.cpuAffinity.isEmpty()) {
        arguments << parameterName(Parameter::CpuAffinity, useShort) << options.cpuAffinity;
    }

    if (!options.backendDevices.isEmpty()) {
        arguments << parameterName(Parameter::BackendDevices, useShort) << options.backendDevices;
    }

    if (!options.openclDeviceTypes.isEmpty()) {
        arguments << parameterName(Parameter::OpenclDeviceTypes, useShort)
                  << options.openclDeviceTypes;
    }

    if (options.hwmonTempAbort) {
        arguments << parameterName(Parameter::HwmonTempAbort, useShort)
                  << QString::number(options.hwmonTempAbortCelsius);
    }

    if (!options.hashFile.isEmpty()) {
        arguments << options.hashFile;
    }

    // The positional tail, which hashcat parses per attack mode: the word
    // lists around a mask in the hybrid modes, key=value settings behind the
    // positional inputs of the feed modes, and the feed itself in mode 8.
    switch (mode) {
    case AttackMode::Pcfg:
        // The rulesets come first, the settings behind them; an empty list
        // of rulesets means the one bundled default.
        arguments << options.pcfgRulesets;
        appendSetting(arguments, AppConstants::HashcatSettings::Scale, options.pcfgScale);
        appendSetting(arguments, AppConstants::HashcatSettings::Costmax, options.pcfgCostmax);
        appendSetting(arguments, AppConstants::HashcatSettings::Weights, options.pcfgWeights);
        appendSetting(arguments, AppConstants::HashcatSettings::Threads, options.pcfgThreads);
        appendSetting(arguments, AppConstants::HashcatSettings::Kbits, options.pcfgKbits);
        appendSetting(arguments, AppConstants::HashcatSettings::Maxword, options.pcfgMaxword);
        appendSetting(arguments, AppConstants::HashcatSettings::Maxgain, options.pcfgMaxgain);
        appendSetting(arguments, AppConstants::HashcatSettings::Walk, options.pcfgWalk);
        appendSetting(arguments, AppConstants::HashcatSettings::Mask, options.pcfgMask);
        for (const QString &token : options.pcfgExtraSettings) {
            if (!token.isEmpty()) {
                arguments << token;
            }
        }
        break;
    case AttackMode::Table:
        // The table feed stops its positional scan at the first key=value
        // token, so the wordlist and every table have to come before the
        // settings.
        arguments << options.wordlists;
        arguments << options.tableFiles;
        appendSetting(arguments, AppConstants::HashcatSettings::Identity, options.tableIdentity);
        appendSetting(arguments, AppConstants::HashcatSettings::Template, options.tableTemplate);
        appendSetting(arguments, AppConstants::HashcatSettings::Single, options.tableSingle);
        appendSetting(arguments, AppConstants::HashcatSettings::Cap, options.tableCap);
        appendSetting(arguments, AppConstants::HashcatSettings::Maxperm, options.tableMaxperm);
        for (const QString &token : options.tableExtraSettings) {
            if (!token.isEmpty()) {
                arguments << token;
            }
        }
        break;
    case AttackMode::Generic:
        // The feed name and its arguments; the count and meaning of the
        // arguments are the feed's own business.
        if (!options.feedName.isEmpty()) {
            arguments << options.feedName;
        }
        arguments << options.feedArguments;
        break;
    case AttackMode::Association:
        // Either the wordlist the hashes are paired with, or the settings
        // that name the phases to run; hashcat reads the settings line as a
        // path when a wordlist is there.
        arguments << options.wordlists;
        arguments << options.associationSettings;
        break;
    default:
        if (!maskBeforeDict.isEmpty()) {
            arguments << maskBeforeDict;
        }

        if (attackUsesWordlists(mode)) {
            arguments << options.wordlists;
        }

        if (!maskAfterDict.isEmpty()) {
            arguments << maskAfterDict;
        }
        break;
    }

    return arguments;
}
