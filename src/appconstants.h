/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * SPDX-FileCopyrightText: Rainer Größlinger
 */

#ifndef APPCONSTANTS_H
#define APPCONSTANTS_H

#include <QLatin1StringView>
#include <QString>
#include <QtGlobal>

#include <type_traits>

// Application wide constants.
//
// Everything that is a protocol token - a settings key, a file name, a
// hashcat option, a placeholder - is a QLatin1StringView: ASCII, allocation
// free, and still convertible to QString. User-visible text that may be
// non-ASCII (the language names below) stays a UTF-8 const char * converted
// with QString::fromUtf8() at the use site.
namespace AppConstants
{

// The languages the settings dialog offers, sorted by each name in its own
// language so the combo reads naturally for every user.
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
// Ordered by each name's native spelling in codepoint order: Latin names
// interleave alphabetically, non-Latin names cluster after them by codepoint.
inline constexpr Language Available[] = {
    {"de", "Deutsch"},  {"en", "English"},     {"es", "Español"},
    {"fr", "Français"}, {"it", "Italiano"},    {"sr", "Srpski"},
    {"ja", "日本語"},   {"zh_CN", "简体中文"}, {"ko", "한국어"},
};
// What a fresh install gets before anything was ever saved, and what the
// dialog falls back to when the key holds a code that is no longer offered:
// System default - follow the environment's locale, which lands on English
// whenever no catalog answers for it.
inline constexpr const char *Default = "";
} // namespace Languages

// Keys passed to SettingsManager. SettingsManager takes plain strings, so a
// mistyped key does not fail - it simply stores the value somewhere else.
namespace SettingsKeys
{
inline constexpr QLatin1StringView HashcatPath{"hashcatPath"};
inline constexpr QLatin1StringView Terminal{"terminal"};
inline constexpr QLatin1StringView UseShortParameters{"useShortParameters"};
// Whether the application restores the last session at the next start.
inline constexpr QLatin1StringView RestoreSession{"restoreSession"};
// The color scheme the user chose, see the Appearance namespace below.
inline constexpr QLatin1StringView ColorScheme{"colorScheme"};
// Locale code of the UI language, see the Languages namespace above.
inline constexpr QLatin1StringView Language{"language"};

// The type of value a stored setting takes. The import checks a value against
// the type of its key before it applies it.
enum class Type : quint8 {
    String,
    Boolean,
};

struct Entry {
    QLatin1StringView key;
    Type type;
    // What the setting starts with: the text for a string setting, the
    // switch for a boolean one. The text is UTF-8, as everywhere else in the
    // application.
    const char *textDefault;
    bool boolDefault;
};

// Every key the settings are made of, with the type of value it takes and the
// value it starts with. The export and the import both read this table, so a
// new setting is one entry here; the tests then say what else is missing.
inline constexpr Entry All[] = {
    {HashcatPath, Type::String, "", false},
    {Terminal, Type::String, "", false},
    {UseShortParameters, Type::Boolean, "", false},
    {RestoreSession, Type::Boolean, "", true},
    {ColorScheme, Type::String, "", false},
    {Language, Type::String, Languages::Default, false},
};

// The default of a setting, in the type the setting takes.
template <typename T> T defaultOf(QLatin1StringView key)
{
    for (const Entry &entry : All) {
        if (entry.key != key) {
            continue;
        }
        if constexpr (std::is_same_v<T, bool>) {
            return entry.boolDefault;
        } else {
            return QString::fromUtf8(entry.textDefault);
        }
    }
    return T();
}
} // namespace SettingsKeys

// The version of the settings a file holds.
//
// The widget names of the session and the keys of the settings change
// independently of each other, so each of them has its own version.
namespace Settings
{
inline constexpr int Version = 1;
} // namespace Settings

// The names of the sections a file can hold.
//
// They are constants and not the name of a class: the section of the session
// used to be keyed by the class name, so renaming that class would orphan
// every session that was ever written.
namespace FileSections
{
inline constexpr QLatin1StringView Session{"session"};
inline constexpr QLatin1StringView Settings{"settings"};
} // namespace FileSections

// The color schemes the settings dialog offers.
//
// The empty scheme is the system default: the desktop decides, and the
// application keeps out of it. The other two are the values that get stored.
namespace Appearance
{
inline constexpr QLatin1StringView ColorSchemeLight{"light"};
inline constexpr QLatin1StringView ColorSchemeDark{"dark"};
} // namespace Appearance

namespace Files
{
// Saved by MainWindow::saveSession(), relative to
// QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
inline constexpr QLatin1StringView SessionFile{"session.json"};
// Suggested names for the files the File menu exports.
inline constexpr QLatin1StringView SessionExport{"hashcat-gui-session.json"};
inline constexpr QLatin1StringView SettingsExport{"hashcat-gui-settings.json"};
// The command preview: it is regenerated on every change, so it must never
// be part of a stored session.
inline constexpr QLatin1StringView PreviewWidget{"lineEdit_command"};
} // namespace Files

// The session format.
//
// A session stores the widgets by their object name, so the names are part of
// the format: a session written by another version of hashcat-gui can name
// widgets that no longer exist. An older file is applied and the values it
// cannot use are named; a file of a newer format is refused, because a name
// that survived may mean something else there.
namespace Sessions
{
inline constexpr int Version = 1;
} // namespace Sessions

// Values the widgets start with. The identical defaults are baked into the
// .ui files (lineEdit_outfile_format = "1,2", lineEdit_backend_devices is empty,
// spinBox_segment_size = 32, comboBox_workload_profile items "1".."4"), so these
// constants only describe what code constructing the widgets itself gets.
namespace Defaults
{
inline constexpr QLatin1StringView OutfileFormat{"1,2"};
// Empty means "let hashcat use every backend device". Hashcat device ids
// start at 1, so there is no valid zero value to store here.
inline constexpr QLatin1StringView BackendDevices{};
inline constexpr int SegmentSize = 32; // MB
inline constexpr QLatin1StringView OutfileSuffix{".out"};
inline constexpr int Runtime = 0;         // seconds, 0 = unlimited
inline constexpr int StatusTimer = 10;    // seconds, hashcat's own default
inline constexpr int HwmonTempAbort = 90; // degrees Celsius
inline constexpr int IncrementMin = 0;    // 0 = hashcat default
inline constexpr int IncrementMax = 0;    // 0 = hashcat default
} // namespace Defaults

// The pattern the keyspace bounds -s and -l have to match. Hashcat takes them
// as plain unsigned numbers, and they reach far beyond what a spin box can
// hold, so the two fields filter on this pattern instead of on a numeric
// validator.
namespace Validation
{
inline constexpr QLatin1StringView Digits{"^[0-9]*$"};
} // namespace Validation

// Placeholders the user can type into the outfile field.
namespace Placeholders
{
inline constexpr QLatin1StringView UnixTime{"<unixtime>"};
inline constexpr QLatin1StringView Hash{"<hash>"};
} // namespace Placeholders

// Fixed reference tables for the help tooltips. Hashcat syntax, so they are
// non-translatable constants here.
namespace HelpText
{
inline constexpr QLatin1StringView BuiltinCharsets{"  ?l = abcdefghijklmnopqrstuvwxyz [a-z]\n"
                                                   "  ?u = ABCDEFGHIJKLMNOPQRSTUVWXYZ [A-Z]\n"
                                                   "  ?d = 0123456789 [0-9]\n"
                                                   "  ?h = 0123456789abcdef [0-9a-f]\n"
                                                   "  ?H = 0123456789ABCDEF [0-9A-F]\n"
                                                   "  ?s =  !\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~\n"
                                                   "  ?a = ?l?u?d?s\n"
                                                   "  ?b = 0x00 - 0xff"};
inline constexpr QLatin1StringView OutfileFormats{"  1 = hash[:salt]\n"
                                                  "  2 = plain\n"
                                                  "  3 = hex_plain\n"
                                                  "  4 = crack_pos\n"
                                                  "  5 = timestamp absolute\n"
                                                  "  6 = timestamp relative"};
inline constexpr QLatin1StringView OpenClDeviceTypes{"  1 = CPU\n"
                                                     "  2 = GPU\n"
                                                     "  Separate multiple values with commas."};
inline constexpr QLatin1StringView CommonEncodings{"  utf-8, utf-16le, utf-16be,\n"
                                                   "  iso-8859-1, iso-8859-15, windows-1252"};
} // namespace HelpText

// Fixed external links. Only the label around a link is translatable.
namespace Urls
{
inline constexpr QLatin1StringView Hashcat{"https://hashcat.net"};
inline constexpr QLatin1StringView HashcatFaq{
    "https://hashcat.net/wiki/doku.php?id=frequently_asked_questions"};
inline constexpr QLatin1StringView Project{"https://github.com/rgroesslinger/hashcat-gui"};
inline constexpr QLatin1StringView Issues{"https://github.com/rgroesslinger/hashcat-gui/issues"};
// The license of the application.
inline constexpr QLatin1StringView License{
    "https://github.com/rgroesslinger/hashcat-gui/blob/master/LICENSE"};
} // namespace Urls

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
