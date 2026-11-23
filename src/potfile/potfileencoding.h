/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef POTFILEENCODING_H
#define POTFILEENCODING_H

#include <QByteArray>
#include <QString>

// Byte-level encoding helpers shared by the potfile parsers. hashcat stores
// anything that is not printable UTF-8 as the literal $HEX[…] in a potfile.
namespace PotfileEncoding
{

bool isHexDigit(QChar character);

// Decodes an even-length hex string. Returns false for odd lengths or
// non-hex characters, leaving bytes untouched.
bool decodeHex(const QString &text, QByteArray *bytes);

// Decodes the inner layer of a $HEX[…] plaintext. Anything that is not a
// well-formed $HEX[…] token is returned unchanged.
QString decodePlain(const QString &plain);

} // namespace PotfileEncoding

#endif // POTFILEENCODING_H