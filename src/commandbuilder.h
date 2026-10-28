/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef COMMANDBUILDER_H
#define COMMANDBUILDER_H

#include <QString>
#include <QStringList>

#include <cstdint>

#include "hashcatoptions.h"

// Turns a HashcatOptions into the hashcat argument vector.
//
// The order of the arguments is stable on purpose: it is what the command
// preview shows, what gets copied to the clipboard and what the tests assert
// on. Every option that the UI leaves at its default value is left out, so
// the preview does not grow a pile of "1,2", "-c 32" style noise.
class CommandBuilder
{
public:
    // The hashcat options the builder knows, with their short and long form.
    enum class Parameter : std::uint8_t {
        AttackMode,
        BackendDevices,
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
        PotfileDisable,
        Remove,
        RuleLeft,
        RuleRight,
        RulesFile,
        Runtime,
        Session,
        Skip,
        SpeedOnly,
        Status,
        StatusTimer,
        Username,
    };

    // What an attack mode needs from the user before hashcat can run.
    //
    // The numbers come from hashcat's own argument validation: combination
    // mode multiplies two word lists and refuses every other number, the
    // table feed takes exactly one wordlist and every table after it, the
    // mask based modes brute-force a mask.
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

    // Substitutes <unixtime> and <hash> in the outfile template.
    static QString expandOutfileTemplate(const QString &tmplate, const QString &hashFile);

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

    // What the attack mode needs from the user, see ModeRequirements.
    //
    // The answer is derived from the predicates above, so the group boxes,
    // the mode hint and the launch validation cannot tell the user different
    // things about the same mode.
    static ModeRequirements modeRequirements(AttackMode mode);

    // The command line spelling of a hashcat option: the short form when
    // useShort is set and the option has one, the long form otherwise.
    static QString parameterName(Parameter key, bool useShort = false);
};

#endif // COMMANDBUILDER_H
