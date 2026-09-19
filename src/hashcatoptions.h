/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef HASHCATOPTIONS_H
#define HASHCATOPTIONS_H

#include <QString>
#include <QStringList>

#include "appconstants.h"

// Supported attack modes, as numbered by hashcat (-a).
enum class AttackMode : int
{
    Straight       = 0,
    Combination    = 1,
    BruteForce     = 3,
    HybridWordMask = 6,
    HybridMaskWord = 7,
    Association    = 9
};

// One UI independent description of a hashcat invocation.
//
// MainWindow fills it from the widgets, CommandBuilder turns it into an
// argument vector. Splitting the two makes the argument rules testable
// without constructing a window, and it makes the gating rules - which attack
// mode contributes a wordlist, a mask or a custom charset - the builder's
// business rather than a side effect of whether a group box happens to be
// enabled at the time.
//
// The member defaults are what the widgets in mainwindow.ui start with, so a
// default constructed HashcatOptions produces exactly the command line the
// freshly opened main window previews.
struct HashcatOptions
{
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

    struct Charset
    {
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
