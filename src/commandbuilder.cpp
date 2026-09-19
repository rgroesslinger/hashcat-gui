/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#include "commandbuilder.h"
#include "helperutils.h"
#include "appconstants.h"

#include <QDateTime>
#include <QFileInfo>

namespace {

// Short or long form of a hashcat option
QString parameter(HelperUtils::Parameter key, bool useShort)
{
    return HelperUtils::getParameter(key, useShort);
}

} // namespace

// True when the attack mode reads a mask
bool CommandBuilder::attackUsesMask(AttackMode mode)
{
    return mode == AttackMode::BruteForce
        || mode == AttackMode::HybridWordMask
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

QString CommandBuilder::expandOutfileTemplate(const QString &tmplate, const QString &hashFile)
{
    QString outfile = tmplate;
    outfile.replace(AppConstants::Placeholders::UnixTime,
                     QString::number(QDateTime::currentMSecsSinceEpoch() / AppConstants::MsecsPerSecond));
    outfile.replace(AppConstants::Placeholders::Hash, QFileInfo(hashFile).fileName(), Qt::CaseInsensitive);
    return outfile;
}

QStringList CommandBuilder::build(const HashcatOptions &options)
{
    const bool useShort = options.useShortParameters;
    const AttackMode mode = options.attackMode;
    QStringList arguments;

    // hash type - left out while none is selected
    if (options.hashType >= 0) {
        arguments << parameter(HelperUtils::Parameter::HashType, useShort) << QString::number(options.hashType);
    }

    arguments << parameter(HelperUtils::Parameter::AttackMode, useShort) << QString::number(static_cast<int>(mode));

    if (options.remove) {
        arguments << parameter(HelperUtils::Parameter::Remove, useShort);
    }

    if (options.ignoreUsername) {
        arguments << parameter(HelperUtils::Parameter::Username, useShort);
    }

    // rules - either existing rule files or a number of generated rules
    if (attackUsesRules(mode)) {
        for (const QString &rulesFile : options.rulesFiles) {
            arguments << parameter(HelperUtils::Parameter::RulesFile, useShort) << rulesFile;
        }
        if (options.rulesFiles.isEmpty() && options.generateRules > 0) {
            arguments << parameter(HelperUtils::Parameter::GenerateRules, useShort) << QString::number(options.generateRules);
        }
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
        arguments << parameter(HelperUtils::Parameter::SpeedOnly, useShort);
    }

    if (!options.workloadProfile.isEmpty()) {
        arguments << parameter(HelperUtils::Parameter::WorkloadProfile, useShort) << options.workloadProfile;
    }

    if (options.optimizedKernel) {
        arguments << parameter(HelperUtils::Parameter::OptimizedKernel, useShort);
    }

    // Custom charsets are only meaningful together with a mask. The UI
    // disables the group box for the other attack modes; here that gating is
    // expressed directly so a stale checkbox cannot leak -1 ... -4 into a
    // command that does not use them.
    if (attackUsesMask(mode)) {
        static const HelperUtils::Parameter charsetParameters[4] = {
            HelperUtils::Parameter::CustomCharset1,
            HelperUtils::Parameter::CustomCharset2,
            HelperUtils::Parameter::CustomCharset3,
            HelperUtils::Parameter::CustomCharset4,
        };
        for (int i = 0; i < 4; ++i) {
            const auto &charset = options.customCharsets[i];
            if (charset.enabled && !charset.value.isEmpty()) {
                arguments << parameter(charsetParameters[i], useShort) << charset.value;
            }
        }
    }

    if (options.hexCharset) {
        arguments << parameter(HelperUtils::Parameter::HexCharset, useShort);
    }

    if (options.hexSalt) {
        arguments << parameter(HelperUtils::Parameter::HexSalt, useShort);
    }

    if (options.outfileEnabled && !options.outfile.isEmpty()) {
        arguments << parameter(HelperUtils::Parameter::Outfile, useShort)
                  << expandOutfileTemplate(options.outfile, options.hashFile);
    }

    if (options.outfileFormat != AppConstants::Defaults::OutfileFormat) {
        arguments << parameter(HelperUtils::Parameter::OutfileFormat, useShort) << options.outfileFormat;
    }

    if (!options.cpuAffinity.isEmpty()) {
        arguments << parameter(HelperUtils::Parameter::CpuAffinity, useShort) << options.cpuAffinity;
    }

    if (!options.backendDevices.isEmpty() && options.backendDevices != AppConstants::Defaults::BackendDevices) {
        arguments << parameter(HelperUtils::Parameter::BackendDevices, useShort) << options.backendDevices;
    }

    if (options.segmentSize != AppConstants::Defaults::SegmentSize) {
        arguments << parameter(HelperUtils::Parameter::SegmentSize, useShort) << QString::number(options.segmentSize);
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
