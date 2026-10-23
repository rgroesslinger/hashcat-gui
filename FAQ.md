# Frequently Asked Questions

- [Where are application settings stored?](#where-are-application-settings-stored)
- [Where is the session saved?](#where-is-the-session-saved)
- [Why does loading the hash types take so long on Windows?](#why-does-loading-the-hash-types-take-so-long-on-windows)

<a name="where-are-application-settings-stored"></a>
## Where are application settings stored?

Application settings are stored using [QSettings::IniFormat](https://doc.qt.io/qt-6/qsettings.html#Format-enum).

| **Operating System** | **Location** |
|-----------|-------------------------------------|
| Linux     | ~/.config/hashcat-gui/settings.ini |
| Windows   | %APPDATA%\hashcat-gui\settings.ini |


<a name="where-is-the-session-saved"></a>
## Where is the session saved?

The session is saved at [QStandardPaths::AppDataLocation](https://doc.qt.io/qt-6/qstandardpaths.html#StandardLocation-enum).

| **Operating System** | **Location** |
|-----------|-------------------------------------|
| Linux     | ~/.local/share/hashcat-gui/session.json |
| Windows   | %APPDATA%\hashcat-gui\session.json |


<a name="why-does-loading-the-hash-types-take-so-long-on-windows"></a>
## Why does loading the hash types take so long on Windows?

On start-up the GUI asks hashcat for the list of supported hash types
(`--example-hashes --machine-readable`). Hashcat answers this by
loading every hash module it ships — on Windows one DLL per module — and
Windows Defender's real-time protection scans each of them as it is
loaded. The query can therefore take very long, up to the point where it
reaches its timeout.

To speed this up, add an exclusion for the hashcat process. Open
PowerShell as an administrator and run

```
Add-MpPreference -ExclusionProcess 'C:\path\to\hashcat.exe'
```

Alternatively, you can temporarily disable Windows Defender real-time protection.


