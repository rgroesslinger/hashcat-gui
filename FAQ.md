# Frequently Asked Questions

- [Where are application settings stored?](#where-are-application-settings-stored)
- [Where is the session saved?](#where-is-the-session-saved)
- [Why does reading hash types time out on Windows?](#why-does-reading-hash-types-time-out-on-windows)

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


<a name="why-does-reading-hash-types-time-out-on-windows"></a>
## Why does reading hash types time out on Windows?

Windows Defender real-time protection can significantly delay the process of retrieving hash types. To resolve this, add an exclusion for the hashcat process.
Open PowerShell as an administrator and run

```
Add-MpPreference -ExclusionProcess 'C:\path\to\hashcat.exe'
```

*Note: Ensure you use the correct path to the hashcat executable, not the GUI.
Alternatively, you can temporarily disable Windows Defender real-time protection.*


