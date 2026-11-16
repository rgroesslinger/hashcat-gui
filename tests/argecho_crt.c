/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

// Test helper: writes its argv (minus argv[0]) as one "A <hex>" line per
// argument. The launch round-trip tests start it through a real shell and
// compare the bytes the process received with the bytes that were sent.
//
// It is plain C on purpose: argv is filled by the C runtime's own command-
// line parser, which is the one hashcat's main() gets, not Qt's. Hex framing
// keeps every byte visible - an empty argument is an "A " line with no hex -
// and avoids a code-page round trip. Not a Qt Test binary.

#include <stdio.h>

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i) {
        fputs("A ", stdout);
        for (const unsigned char *byte = (const unsigned char *)argv[i]; *byte; ++byte) {
            printf("%02x", *byte);
        }
        putchar('\n');
    }
    return 0;
}