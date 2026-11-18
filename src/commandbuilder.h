/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef COMMANDBUILDER_H
#define COMMANDBUILDER_H

#include "hashcatoptions.h"

#include <QString>
#include <QStringList>
#include <cstdint>

// Turns a HashcatOptions into the hashcat argument vector.
//
// The order is stable: the preview, the clipboard and the tests depend on it.
// Options at their default are left out, so the preview stays readable.
class CommandBuilder
{
public:
    // The hashcat options the builder knows, with their short and long form.
    enum class Parameter : std::uint8_t {
        AttackMode,
        BackendDevices,
        CachePath,
        CpuAffinity,
        CustomCharset1,
        CustomCharset2,
        CustomCharset3,
        CustomCharset4,
        EncodingFrom,
        EncodingTo,
        Force,
        GenerateRules,
        HashType,
        HashRecipe,
        HexCharset,
        HexSalt,
        HexWordlist,
        HwmonTempAbort,
        Increment,
        IncrementMax,
        IncrementMin,
        Limit,
        OpenclDeviceTypes,
        OptimizedKernel,
        Outfile,
        OutfileFormat,
        OutfileJson,
        PotfileDisable,
        Remove,
        RuleLeft,
        RuleRight,
        RulesConcat,
        RulesFile,
        Runtime,
        Session,
        Skip,
        SpeedOnly,
        Status,
        StatusTimer,
        Username,
        // Not a parameter: the number of enumerators, for the table tests.
        // It has no name-table entry and is never accepted from a session
        // or a command line.
        Count,
    };

    // What an attack mode needs from the user. The numbers come from hashcat's
    // own validation: combination multiplies two word lists, the table feed
    // takes one wordlist and every table, the mask modes a mask.
    struct ModeRequirements {
        // How many word lists the mode reads at least.
        int minWordlists = 0;
        // How many at most; -1 means "as many as the user selects".
        int maxWordlists = -1;
        // True when the mode needs a mask.
        bool needsMask = false;
    };

    // The markers of attack mode 12: ?w names the word position, ?q the
    // second word that only exists with two word lists.
    struct MaskMarkers {
        // How many unescaped ?w the mask holds.
        int word = 0;
        // How many unescaped ?q it holds.
        int second = 0;
        // True when a ?q appears before the first ?w, which hashcat refuses:
        // the second word has no first word to follow.
        bool secondBeforeWord = false;
    };

    static QStringList build(const HashcatOptions &options);

    // Substitutes <unixtime> (from the collected seconds) and <hash> in the
    // outfile template. The caller owns the clock read, so build() stays
    // reproducible.
    static QString expandOutfileTemplate(const QString &tmplate, const QString &hashFile,
                                         qint64 unixTime);

    // The ?w and ?q markers in a mask. ?? is a literal question mark and
    // every other ?x is a charset token, so the count cannot be derived
    // from the plain number of question marks.
    static MaskMarkers maskMarkers(const QString &mask);

    // Which attack modes consume which part of the UI. These are the same
    // predicates that decide the group box states in the main window, so the
    // preview and the widgets can never disagree.
    static bool attackUsesMask(AttackMode mode);
    static bool attackUsesIncrement(AttackMode mode);
    static bool attackUsesWordlists(AttackMode mode);
    static bool attackUsesRules(AttackMode mode);
    static bool attackUsesRuleLeft(AttackMode mode);
    static bool attackUsesRuleRight(AttackMode mode);
    static bool attackUsesCustomCharset(AttackMode mode);
    static bool attackUsesHexWordlist(AttackMode mode);
    static bool attackUsesWordlistEncoding(AttackMode mode);

    // What the attack mode needs, see ModeRequirements.
    static ModeRequirements modeRequirements(AttackMode mode);

    // The command line spelling of a hashcat option: the short form when
    // useShort is set and the option has one, the long form otherwise.
    static QString parameterName(Parameter key, bool useShort = false);
};

#endif // COMMANDBUILDER_H
