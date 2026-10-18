/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef APPCONSTANTS_H
#define APPCONSTANTS_H

#include <QLatin1StringView>
#include <QtGlobal>

// Application wide constants.
//
// Everything that is a protocol token - a settings key, a file name, a
// hashcat option, a placeholder - is a QLatin1StringView: ASCII, allocation
// free, and still convertible to QString. User-visible text that may be
// non-ASCII (the language names below) stays a UTF-8 const char * converted
// with QString::fromUtf8() at the use site.
namespace AppConstants
{

// Keys passed to SettingsManager. SettingsManager takes plain strings, so a
// mistyped key does not fail - it simply stores the value somewhere else.
namespace SettingsKeys
{
inline constexpr QLatin1StringView HashcatPath{"hashcatPath"};
inline constexpr QLatin1StringView Terminal{"terminal"};
inline constexpr QLatin1StringView UseShortParameters{"useShortParameters"};
// Locale code of the UI language, see the Languages namespace below.
inline constexpr QLatin1StringView Language{"language"};
} // namespace SettingsKeys

// The languages the settings dialog offers.
//
// What gets stored is always the locale code, never the display text, so
// relabeling an entry cannot orphan a saved setting. Both members hold UTF-8
// and are converted with QString::fromUtf8() where they are shown.
namespace Languages
{
struct Language {
    const char *code;
    const char *name;
};
inline constexpr Language Available[] = {
    {"en", "English"},
    {"de", "Deutsch"},
    {"sr", "Srpski"},
};
// What a fresh install gets before anything was ever saved, and what the
// dialog falls back to when the key holds a code that is no longer offered:
// System default - follow the environment's locale, which lands on English
// whenever no catalog answers for it.
inline constexpr const char *Default = "";
} // namespace Languages

namespace Files
{
// Saved by MainWindow::saveDefaultProfile(), relative to
// QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
inline constexpr QLatin1StringView DefaultProfile{"default_profile.json"};
// The command preview: it is regenerated on every change, so it must never
// be part of a stored profile.
inline constexpr QLatin1StringView PreviewWidget{"lineEdit_command"};
} // namespace Files

// Values the widgets start with. The identical defaults are baked into the
// .ui files (lineEdit_outfile_format = "1,2", lineEdit_devices = "0",
// spinBox_segment = 32, comboBox_workload_profile items "1".."4"), so these
// constants only describe what code constructing the widgets itself gets.
namespace Defaults
{
inline constexpr QLatin1StringView OutfileFormat{"1,2"};
inline constexpr QLatin1StringView BackendDevices{"0"};
inline constexpr int SegmentSize = 32; // MB
inline constexpr QLatin1StringView OutfileSuffix{".out"};
} // namespace Defaults

// Placeholders the user can type into the outfile field.
namespace Placeholders
{
inline constexpr QLatin1StringView UnixTime{"<unixtime>"};
inline constexpr QLatin1StringView Hash{"<hash>"};
} // namespace Placeholders

// Command line tokens that belong to hashcat itself.
namespace Hashcat
{
inline constexpr QLatin1StringView Quiet{"--quiet"};
inline constexpr QLatin1StringView ExampleHashes{"--example-hashes"};
inline constexpr QLatin1StringView MachineReadable{"--machine-readable"};
inline constexpr QLatin1StringView Version{"--version"};
// How long hashcat is given to answer a query such as --example-hashes
inline constexpr int QueryTimeoutMs = 20000;
} // namespace Hashcat

// Conversion factor for the <unixtime> placeholder, which is in seconds.
inline constexpr qint64 MsecsPerSecond = 1000;

} // namespace AppConstants

#endif // APPCONSTANTS_H
