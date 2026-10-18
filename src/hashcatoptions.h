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
    HybridWordMask = 6,
    HybridMaskWord = 7,
    Association = 9
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

    AttackMode attackMode = AttackMode::Straight;

    bool remove = false;
    bool ignoreUsername = false;

    // Files selected for -r, in list order. Only filled for attack modes
    // that accept rules.
    QStringList rulesFiles;
    // Number of generated rules for -g, 0 means "do not pass -g"
    int generateRules = 0;

    // Mask of the mask based attack modes, empty means "no mask"
    QString mask;

    bool speedOnly = false;
    // Empty means "do not pass -w"
    QString workloadProfile;
    bool optimizedKernel = false;

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

    QString cpuAffinity;
    QString backendDevices = AppConstants::Defaults::BackendDevices;

    int segmentSize = AppConstants::Defaults::SegmentSize;

    QString hashFile;

    // The ticked wordlists, in list order
    QStringList wordlists;
};

#endif // HASHCATOPTIONS_H
