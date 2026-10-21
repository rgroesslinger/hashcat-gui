/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "commandbuilder.h"
#include "appconstants.h"

#include <QDateTime>
#include <QFileInfo>
#include <QMap>

// True when the attack mode reads a mask
bool CommandBuilder::attackUsesMask(AttackMode mode)
{
    return mode == AttackMode::BruteForce || mode == AttackMode::HybridWordMask
           || mode == AttackMode::HybridMaskWord;
}

// True when the attack mode takes dictionaries (attack mode 3 takes only the
// mask, everything else takes at least one wordlist)
bool CommandBuilder::attackUsesWordlists(AttackMode mode)
{
    return mode != AttackMode::BruteForce;
}

// True when the attack mode accepts -r / -g
bool CommandBuilder::attackUsesRules(AttackMode mode)
{
    return mode == AttackMode::Straight || mode == AttackMode::Association;
}

// -j applies to the wordlist on the left. That is the only wordlist in modes
// 0, 6 and 9, and the first one in combination mode. Hashcat accepts -j in
// every attack mode and silently ignores it where there is no left wordlist,
// so each side is gated on its own instead of being tied to mode 1.
bool CommandBuilder::attackUsesRuleLeft(AttackMode mode)
{
    return mode == AttackMode::Straight || mode == AttackMode::Combination
           || mode == AttackMode::HybridWordMask || mode == AttackMode::Association;
}

// -k applies to the wordlist on the right, which only combination mode (two
// wordlists) and mask + wordlist mode provide.
bool CommandBuilder::attackUsesRuleRight(AttackMode mode)
{
    return mode == AttackMode::Combination || mode == AttackMode::HybridMaskWord;
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

    if (mode == AttackMode::Combination) {
        // Combination multiplies two word lists. Hashcat answers every other
        // number with a usage error.
        requirements.minWordlists = 2;
        requirements.maxWordlists = 2;
    } else if (attackUsesWordlists(mode)) {
        // Any number of word lists, but at least one.
        requirements.minWordlists = 1;
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
        {Parameter::SegmentSize, {QStringLiteral("-c"), QStringLiteral("--segment-size")}},
        {Parameter::Session, {QString(), QStringLiteral("--session")}},
        {Parameter::Skip, {QStringLiteral("-s"), QStringLiteral("--skip")}},
        {Parameter::SpeedOnly, {QString(), QStringLiteral("--speed-only")}},
        {Parameter::Status, {QString(), QStringLiteral("--status")}},
        {Parameter::StatusTimer, {QString(), QStringLiteral("--status-timer")}},
        {Parameter::Username, {QString(), QStringLiteral("--username")}},
        {Parameter::WorkloadProfile, {QStringLiteral("-w"), QStringLiteral("--workload-profile")}},
    };

    auto it = parameterMap.constFind(key);
    if (it == parameterMap.constEnd()) {
        // Every enumerator has an entry above, but dereferencing end() would
        // be undefined behaviour, so fail soft instead.
        return {};
    }
    const auto &pair = it.value();

    if (useShort && !pair.first.isEmpty()) {
        // short form
        return pair.first;
    }

    // long form (default)
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
    default:
        break;
    }

    if (options.speedOnly) {
        arguments << parameterName(Parameter::SpeedOnly, useShort);
    }

    if (!options.workloadProfile.isEmpty()) {
        arguments << parameterName(Parameter::WorkloadProfile, useShort) << options.workloadProfile;
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

    // Incrementing only makes sense while a mask is in use. The two bounds
    // are part of the same option: without the flag they would be ignored.
    if (attackUsesMask(mode) && options.increment) {
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
    if (attackUsesMask(mode)) {
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
    if (attackUsesMask(mode) && options.hexCharset) {
        arguments << parameterName(Parameter::HexCharset, useShort);
    }

    if (options.hexSalt) {
        arguments << parameterName(Parameter::HexSalt, useShort);
    }

    // Wordlist interpretation only applies while a wordlist is read.
    if (attackUsesWordlists(mode)) {
        if (options.hexWordlist) {
            arguments << parameterName(Parameter::HexWordlist, useShort);
        }
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

    if (options.segmentSize != AppConstants::Defaults::SegmentSize) {
        arguments << parameterName(Parameter::SegmentSize, useShort)
                  << QString::number(options.segmentSize);
    }

    if (!options.hashFile.isEmpty()) {
        arguments << options.hashFile;
    }

    if (!maskBeforeDict.isEmpty()) {
        arguments << maskBeforeDict;
    }

    if (attackUsesWordlists(mode)) {
        arguments << options.wordlists;
    }

    if (!maskAfterDict.isEmpty()) {
        arguments << maskAfterDict;
    }

    return arguments;
}
