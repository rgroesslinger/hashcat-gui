/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef HASHCATOPTIONS_H
#define HASHCATOPTIONS_H

#include <QString>
#include <QStringList>
#include <cstdint>

#include "appconstants.h"

// Supported attack modes, as numbered by hashcat (-a).
enum class AttackMode : std::uint8_t {
    Straight = 0,
    Combination = 1,
    BruteForce = 3,
    Pcfg = 4,
    Table = 5,
    HybridWordMask = 6,
    HybridMaskWord = 7,
    Generic = 8,
    Association = 9,
    // The ?w/?q hybrid; 1, 6 and 7 are rewritten into it internally.
    Hybrid = 12
};

// The attack modes in the order the combo box and the example menu show them.
inline constexpr AttackMode AllAttackModes[] = {
    AttackMode::Straight,       AttackMode::Combination, AttackMode::BruteForce,
    AttackMode::Pcfg,           AttackMode::Table,       AttackMode::HybridWordMask,
    AttackMode::HybridMaskWord, AttackMode::Generic,     AttackMode::Association,
    AttackMode::Hybrid,
};

// One UI independent description of a hashcat invocation.
//
// MainWindow fills it from the widgets, CommandBuilder turns it into an
// argument vector, which makes the argument rules testable without a window
// and keeps the gating rules - which attack mode contributes a wordlist, a
// mask or a custom charset - in the builder instead of in the group boxes.
//
// The member defaults are what the widgets in mainwindow.ui start with.
struct HashcatOptions {
    bool useShortParameters = false;

    // -1 while no hash type is selected: -m is then left out
    int hashType = -1;

    // --hash-recipe, the recipe of hash mode 4000. Emitted only for that
    // type, so a stale value a session carries never reaches the command.
    QString hashRecipe;

    AttackMode attackMode = AttackMode::Straight;

    bool remove = false;
    bool ignoreUsername = false;

    // Files selected for -r, in list order. Only filled for attack modes
    // that accept rules.
    QStringList rulesFiles;
    // --rules-concat passes every rules file to hashcat in one run. Only
    // emitted together with the files it joins.
    bool rulesConcat = false;
    // Number of generated rules for -g, 0 means "do not pass -g"
    int generateRules = 0;

    // Mask of the mask based attack modes, empty means "no mask"
    QString mask;

    bool speedOnly = false;
    bool optimizedKernel = false;

    // Execution scheduling and output state, independent of the attack mode.
    QString session;
    int runtime = AppConstants::Defaults::Runtime; // seconds, 0 = no limit
    QString skip;
    QString limit;
    bool status = false;
    int statusTimer = AppConstants::Defaults::StatusTimer; // seconds
    bool force = false;
    bool potfileDisable = false;

    // Mask incrementing, only meaningful in the mask based attack modes.
    bool increment = false;
    int incrementMin = AppConstants::Defaults::IncrementMin; // 0 = hashcat default
    int incrementMax = AppConstants::Defaults::IncrementMax; // 0 = hashcat default

    // Wordlist interpretation, only meaningful in the wordlist modes.
    bool hexWordlist = false;
    QString encodingFrom;
    QString encodingTo;

    // Hardware selection and safety.
    QString openclDeviceTypes;
    bool hwmonTempAbort = false;
    int hwmonTempAbortCelsius = AppConstants::Defaults::HwmonTempAbort;

    // The single rules of the two wordlist sides (-j / -k). Which side exists
    // depends on the attack mode, see CommandBuilder::attackUsesRuleLeft() and
    // attackUsesRuleRight(); -r / -g are not allowed in combination mode.
    QString ruleLeft;
    QString ruleRight;

    struct Charset {
        bool enabled = false;
        QString value;
    };
    Charset customCharsets[4];

    bool hexCharset = false;
    bool hexSalt = false;

    // Passing an outfile is opt-in, not just a matter of the field being
    // filled: hashcat then writes to it behind the user's back.
    bool outfileEnabled = false;
    // The template as typed, with <unixtime> and <hash> still in it
    QString outfile;
    QString outfileFormat = AppConstants::Defaults::OutfileFormat;
    // --outfile-json wins over --outfile-format: the two are exclusive.
    bool outfileJson = false;
    // --cache-path, an existing directory hashcat keeps its cache in
    QString cachePath;

    QString cpuAffinity;
    QString backendDevices = AppConstants::Defaults::BackendDevices;

    QString hashFile;

    // The ticked wordlists, in list order
    QStringList wordlists;

    // -a 4 (PCFG): the rulesets as positional arguments, then key=value
    // settings. Every value defaults to "not set" and stays out of the
    // command line; an empty ruleset list means the one bundled default.
    QStringList pcfgRulesets;
    QString pcfgScale;
    QString pcfgCostmax;
    QString pcfgWeights;
    QString pcfgThreads;
    QString pcfgKbits;
    QString pcfgMaxword;
    QString pcfgMaxgain;
    QString pcfgWalk;
    QString pcfgMask;
    // Settings the GUI has no field for, passed through as typed.
    QStringList pcfgExtraSettings;

    // -a 5 (table): the one wordlist (wordlists above), then the tables,
    // then key=value settings.
    QStringList tableFiles;
    QString tableIdentity;
    QString tableTemplate;
    QString tableSingle;
    QString tableCap;
    QString tableMaxperm;
    QStringList tableExtraSettings;

    // -a 8 (generic feed): the feed name (a bundled name or a path) and its
    // positional arguments, in order. Their count is feed specific.
    QString feedName;
    QStringList feedArguments;

    // -a 9 (association): key=value settings, only in the form without a
    // wordlist - with one, hashcat reads the settings line as a path.
    QStringList associationSettings;
};

#endif // HASHCATOPTIONS_H
