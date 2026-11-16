# Frequently Asked Questions

- [Why are ptyxis and cmd.exe not supported?](#why-are-ptyxis-and-cmdexe-not-supported)
- [Where are application settings stored?](#where-are-application-settings-stored)
- [Where is the session saved?](#where-is-the-session-saved)
- [Why does loading the hash types take so long on Windows?](#why-does-loading-the-hash-types-take-so-long-on-windows)

<a name="why-are-ptyxis-and-cmdexe-not-supported"></a>
## Why are ptyxis and cmd.exe not supported?

Both would alter a hash, mask or path before hashcat sees it.

**cmd.exe** re-parses `%`, `&`, `^`, `<` and `>` in every command line it
is given. Those characters occur in hashes, masks and file names, and
there is no quoting that preserves every possible value. On Windows
hashcat-gui starts hashcat through a PowerShell script that passes the
arguments unchanged instead, so cmd.exe is not offered as a terminal.
Windows lists only `powershell.exe` and `pwsh`; the other terminals in
the list are Unix terminals that need the `sh` wrapper.

**ptyxis** hands the command to `systemd-run`, which expands `$`-words a
second time. A bcrypt hash (`$2a$10$...`), a `$HOME` or a PowerShell
variable therefore arrived at hashcat truncated or expanded, and ptyxis
has no option to pass the command verbatim. The terminal is no longer
offered; every other supported terminal receives the command line
unchanged.


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


