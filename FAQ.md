# Frequently Asked Questions

- [Where are application settings stored?](#where-are-application-settings-stored)
- [Where is the default profile saved?](#where-is-the-default-profile-saved)

<a name="where-are-application-settings-stored"></a>
## Where are application settings stored?

Application settings are stored using [QSettings::NativeFormat](https://doc.qt.io/qt-6/qsettings.html#Format-enum).

| **Operating System** | **Location** |
|-----------|-------------------------------------|
| Linux     | ~/.config/hashcat-gui/settings.conf |
| Windows   | HKEY_CURRENT_USER\Software\hashcat-gui\settings |


<a name="where-is-the-default-profile-saved"></a>
## Where is the default profile saved?

The default profile is saved at [QStandardPaths::AppDataLocation](https://doc.qt.io/qt-6/qstandardpaths.html#StandardLocation-enum).

| **Operating System** | **Location** |
|-----------|-------------------------------------|
| Linux     | ~/.local/share/hashcat-gui/default_profile.json |
| Windows   | %APPDATA%\hashcat-gui\default_profile.json |


