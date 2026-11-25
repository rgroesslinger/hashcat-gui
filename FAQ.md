# Frequently Asked Questions

- [Where are application settings stored?](#where-are-application-settings-stored)
- [Where is the session saved?](#where-is-the-session-saved)
- [Why does loading the hash types take so long on Windows?](#why-does-loading-the-hash-types-take-so-long-on-windows)
- [Why does the potfile viewer show only the first 100,000 entries?](#why-does-the-potfile-viewer-show-only-the-first-100000-entries)
- [Why are ptyxis and cmd.exe not supported?](#why-are-ptyxis-and-cmdexe-not-supported)

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


<a name="why-does-the-potfile-viewer-show-only-the-first-100000-entries"></a>
## Why does the potfile viewer show only the first 100,000 entries?

The file is read in full, so the status line always reports the complete
counts, including duplicates and malformed lines. Only the rows kept for
display are capped: at most 100,000 entries are held in memory, and the
status line says `showing first X of Y` when there are more.

Search, copy, statistics and export work on the displayed rows only;
the export confirmation and the statistics dialog name the full total.
The cap keeps the table responsive and memory use bounded — keeping a
million rows would cost seconds of GUI time and over a gigabyte of RAM.


<a name="why-are-ptyxis-and-cmdexe-not-supported"></a>
## Why are ptyxis and cmd.exe not supported?

Both would alter a hash, mask or path before hashcat sees it.

**cmd.exe** re-parses `%`, `&`, `^`, `<` and `>` in every command line it
is given. Those characters occur in hashes, masks and file names, and
there is no quoting that preserves every possible value.

**ptyxis** hands the command to `systemd-run`, which expands `$`-words a
second time. In a test a bcrypt hash disappeared; ptyxis has no
option to pass the command verbatim. It will be re-enabled once an
[upstream fix](https://gitlab.gnome.org/chergert/ptyxis/-/merge_requests/168)
has landed.


