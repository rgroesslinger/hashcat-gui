/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef APPCONSTANTS_H
#define APPCONSTANTS_H

#include <QtGlobal>

// Application wide constants.
//
// Settings keys, file names, default values and the literals handed to
// hashcat used to be spelled out at every use site, where a typo in a settings
// key would silently create a second key and a typo in a command line token
// would silently change the command. Keeping them together also documents
// which defaults are duplicated in the .ui files.
namespace AppConstants {

// Keys passed to SettingsManager. SettingsManager takes plain strings, so a
// mistyped key does not fail - it simply stores the value somewhere else.
namespace SettingsKeys {
inline constexpr const char *HashcatPath = "hashcatPath";
inline constexpr const char *Terminal = "terminal";
inline constexpr const char *UseShortParameters = "useShortParameters";
// Locale code of the UI language, see the Languages namespace below.
inline constexpr const char *Language = "language";
} // namespace SettingsKeys

// The languages the settings dialog offers.
//
// What gets stored is always the locale code, never the display text, so
// relabeling an entry cannot orphan a saved setting. The names are each
// language's own name on purpose: whoever reads "Deutsch" knows it is their
// language, "German" only helps English speakers.
namespace Languages {
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

namespace Files {
// Saved by MainWindow::saveDefaultProfile(), relative to
// QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
inline constexpr const char *DefaultProfile = "default_profile.json";
// The command preview: it is regenerated on every change, so it must never
// be part of a stored profile.
inline constexpr const char *PreviewWidget = "lineEdit_command";
} // namespace Files

// Values the widgets start with. The identical defaults are baked into the
// .ui files (lineEdit_outfile_format = "1,2", lineEdit_devices = "0",
// spinBox_segment = 32, comboBox_workload_profile items "1".."4"), so these
// constants only describe what code constructing the widgets itself gets.
namespace Defaults {
inline constexpr const char *OutfileFormat = "1,2";
inline constexpr const char *BackendDevices = "0";
inline constexpr int SegmentSize = 32; // MB
inline constexpr const char *OutfileSuffix = ".out";
} // namespace Defaults

// Placeholders the user can type into the outfile field.
namespace Placeholders {
inline constexpr const char *UnixTime = "<unixtime>";
inline constexpr const char *Hash = "<hash>";
} // namespace Placeholders

// Command line tokens that belong to hashcat itself.
namespace Hashcat {
inline constexpr const char *Quiet = "--quiet";
inline constexpr const char *ExampleHashes = "--example-hashes";
inline constexpr const char *MachineReadable = "--machine-readable";
inline constexpr const char *Version = "--version";
// How long hashcat is given to answer a query such as --example-hashes
inline constexpr int QueryTimeoutMs = 20000;
} // namespace Hashcat

// Conversion factor for the <unixtime> placeholder, which is in seconds.
inline constexpr qint64 MsecsPerSecond = 1000;

} // namespace AppConstants

#endif // APPCONSTANTS_H
