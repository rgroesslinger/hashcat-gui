/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef EXAMPLES_H
#define EXAMPLES_H

#include <QLatin1StringView>

#include "hashcatoptions.h"

// The bundled example sessions, one per supported attack mode.
//
// The table lives here rather than in appconstants.h because the attack mode
// is an AttackMode, and hashcatoptions.h - which defines it - includes
// appconstants.h. Keeping the two apart avoids that cycle. The data files are
// resources: they are unpacked to the application data directory before a
// session that names them is applied, because hashcat can only read real
// paths.
namespace Examples
{
struct Entry {
    AttackMode attackMode;
    int hashType;
    QLatin1StringView resource;
    QLatin1StringView password;
};

// Ordered like AllAttackModes and the attack mode combo.
inline constexpr Entry All[] = {
    {AttackMode::Straight, 0, QLatin1StringView{":/sessions/example_straight.json"},
     QLatin1StringView{"Sunshine42!"}},
    {AttackMode::Combination, 100, QLatin1StringView{":/sessions/example_combination.json"},
     QLatin1StringView{"Winter2026!"}},
    {AttackMode::BruteForce, 1400, QLatin1StringView{":/sessions/example_bruteforce.json"},
     QLatin1StringView{"Nice!98"}},
    {AttackMode::Pcfg, 1700, QLatin1StringView{":/sessions/example_pcfg.json"},
     QLatin1StringView{"Meadow42!"}},
    {AttackMode::Table, 10, QLatin1StringView{":/sessions/example_table.json"},
     QLatin1StringView{"pl4n3t!"}},
    {AttackMode::HybridWordMask, 20, QLatin1StringView{":/sessions/example_hybrid_word_mask.json"},
     QLatin1StringView{"Dragon2026!"}},
    {AttackMode::HybridMaskWord, 500, QLatin1StringView{":/sessions/example_hybrid_mask_word.json"},
     QLatin1StringView{"87Autumn!"}},
    {AttackMode::Generic, 1000, QLatin1StringView{":/sessions/example_generic.json"},
     QLatin1StringView{"Copper27!"}},
    {AttackMode::Association, 1800, QLatin1StringView{":/sessions/example_association.json"},
     QLatin1StringView{"Harper"}},
    {AttackMode::Hybrid, 3200, QLatin1StringView{":/sessions/example_hybrid.json"},
     QLatin1StringView{"Coffee2Go!"}},
};

// A path in a session file starts with this token; the loader replaces it
// with the native path of DataDirectory once the files are unpacked.
inline constexpr QLatin1StringView DataPrefix{"<examples>/"};
inline constexpr QLatin1StringView DataDirectory{"examples"};

inline constexpr QLatin1StringView DataFiles[] = {
    QLatin1StringView{":/sessions/data/straight_words.txt"},
    QLatin1StringView{":/sessions/data/straight.rule"},
    QLatin1StringView{":/sessions/data/combination_left.txt"},
    QLatin1StringView{":/sessions/data/combination_right.txt"},
    QLatin1StringView{":/sessions/data/table_words.txt"},
    QLatin1StringView{":/sessions/data/substitutions.table"},
    QLatin1StringView{":/sessions/data/hybrid_word_mask_words.txt"},
    QLatin1StringView{":/sessions/data/hybrid_mask_word_words.txt"},
    QLatin1StringView{":/sessions/data/generic_words_a.txt"},
    QLatin1StringView{":/sessions/data/generic_words_b.txt"},
    QLatin1StringView{":/sessions/data/association.hash"},
    QLatin1StringView{":/sessions/data/hybrid_left.txt"},
    QLatin1StringView{":/sessions/data/hybrid_right.txt"},
};
} // namespace Examples

#endif // EXAMPLES_H
