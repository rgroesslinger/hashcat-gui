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
// Sorted alphabetically by language code (ISO 639-1, with region suffix).
// Keep this order when adding new languages.
inline constexpr Language Available[] = {
    {"de", "Deutsch"},    {"en", "English"},     {"es", "Español"},
    {"fr", "Français"},   {"it", "Italiano"},    {"ja", "日本語"},
    {"ko", "한국어"},     {"pl", "Polski"},      {"pt_BR", "Português (Brasil)"},
    {"ru", "Русский"},    {"sr", "Srpski"},      {"tr", "Türkçe"},
    {"uk", "Українська"}, {"zh_CN", "简体中文"}, {"zh_TW", "繁體中文"},
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
// The bundled brute-force example, compiled in through resources.qrc.
inline constexpr QLatin1StringView ExampleSession{":/sessions/example_bruteforce.json"};
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
// .ui files (lineEdit_outfile_format = "1,2", lineEdit_backend_devices is empty),
// so these constants only describe what code constructing the widgets itself
// gets.
namespace Defaults
{
inline constexpr QLatin1StringView OutfileFormat{"1,2"};
// Empty means "let hashcat use every backend device". Hashcat device ids
// start at 1, so there is no valid zero value to store here.
inline constexpr QLatin1StringView BackendDevices{};
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
// -D takes a number list of the device types 1 (CPU) and 2 (GPU); 3, the
// accelerators, is gone with hashcat 8.0.0. Spaces around the numbers are
// fine, as in hashcat's own number list.
inline constexpr QLatin1StringView DeviceTypes{"^\\s*([12]\\s*(,\\s*[12]\\s*)*)?$"};
} // namespace Validation

// The positional settings of the feed modes (-a 4, -a 5) and of the
// association attack, written as key=value after the positional inputs.
// Protocol tokens, so they are never translated.
namespace HashcatSettings
{
inline constexpr QLatin1StringView Scale{"scale"};
inline constexpr QLatin1StringView Costmax{"costmax"};
inline constexpr QLatin1StringView Weights{"weights"};
inline constexpr QLatin1StringView Threads{"threads"};
inline constexpr QLatin1StringView Kbits{"kbits"};
inline constexpr QLatin1StringView Maxword{"maxword"};
inline constexpr QLatin1StringView Maxgain{"maxgain"};
inline constexpr QLatin1StringView Walk{"walk"};
inline constexpr QLatin1StringView Mask{"mask"};
inline constexpr QLatin1StringView Identity{"identity"};
inline constexpr QLatin1StringView Template{"template"};
inline constexpr QLatin1StringView Single{"single"};
inline constexpr QLatin1StringView Cap{"cap"};
inline constexpr QLatin1StringView Maxperm{"maxperm"};
} // namespace HashcatSettings

// Placeholders the user can type into the outfile field.
namespace Placeholders
{
inline constexpr QLatin1StringView UnixTime{"<unixtime>"};
inline constexpr QLatin1StringView Hash{"<hash>"};
} // namespace Placeholders

// Fixed reference tables for the help tooltips. Hashcat syntax, so they are
// non-translatable constants here. UTF-8 rather than Latin-1: the charset
// table carries typographic punctuation, so the caller converts it where it
// shows it.
namespace HelpText
{
inline constexpr const char *BuiltinCharsets = "  ?l = abcdefghijklmnopqrstuvwxyz [a-z]\n"
                                               "  ?u = ABCDEFGHIJKLMNOPQRSTUVWXYZ [A-Z]\n"
                                               "  ?d = 0123456789 [0-9]\n"
                                               "  ?h = 0123456789abcdef [0-9a-f]\n"
                                               "  ?H = 0123456789ABCDEF [0-9A-F]\n"
                                               "  ?s =  !\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~\n"
                                               "  ?a = ?l?u?d?s\n"
                                               "  ?b = 0x00–0xff";
inline constexpr const char *OutfileFormats = "  1 = hash[:salt]\n"
                                              "  2 = plain\n"
                                              "  3 = hex_plain\n"
                                              "  4 = crack_pos\n"
                                              "  5 = timestamp absolute\n"
                                              "  6 = timestamp relative";
inline constexpr const char *OpenClDeviceTypes = "  1 = CPU\n"
                                                 "  2 = GPU\n"
                                                 "  Separate multiple values with commas.";
inline constexpr const char *CommonEncodings = "  utf-8, utf-16le, utf-16be,\n"
                                               "  iso-8859-1, iso-8859-15, windows-1252";
} // namespace HelpText

// Fixed external links. Only the label around a link is translatable.
namespace Urls
{
inline constexpr QLatin1StringView Hashcat{"https://hashcat.net"};
inline constexpr QLatin1StringView HashcatFaq{
    "https://hashcat.net/wiki/doku.php?id=frequently_asked_questions"};
inline constexpr QLatin1StringView Project{"https://github.com/rgroesslinger/hashcat-gui"};
inline constexpr QLatin1StringView Issues{"https://github.com/rgroesslinger/hashcat-gui/issues"};
// The latest release as the GitHub API reports it, and the page a user can
// open to read about it.
inline constexpr QLatin1StringView LatestReleaseApi{
    "https://api.github.com/repos/rgroesslinger/hashcat-gui/releases/latest"};
inline constexpr QLatin1StringView LatestReleasePage{
    "https://github.com/rgroesslinger/hashcat-gui/releases/latest"};
// The license of the application.
inline constexpr QLatin1StringView License{
    "https://github.com/rgroesslinger/hashcat-gui/blob/master/LICENSE"};
} // namespace Urls

// How the About dialog asks GitHub for a newer release.
namespace UpdateCheck
{
// The progress indicator stays on screen at least this long before the
// request starts, so a fast answer cannot make it flash.
inline constexpr int IndicatorDelayMs = 1000;
// A request that hangs must not keep the dialog busy forever.
inline constexpr int RequestTimeoutMs = 10000;
} // namespace UpdateCheck

// Command line tokens that belong to hashcat itself.
namespace Hashcat
{
// The executable looked up on PATH when no path is configured. Fixed hashcat
// names: the Windows build ships hashcat.exe, the Unix builds hashcat.bin.
#ifdef Q_OS_WIN
inline constexpr QLatin1StringView ExecutableName{"hashcat.exe"};
#else
inline constexpr QLatin1StringView ExecutableName{"hashcat.bin"};
#endif
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
