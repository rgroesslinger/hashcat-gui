# Manual escaping/quoting test session

`escaping_session.json` is a deliberately hostile session for checking the
command-line quoting added for terminal launches. Every free-text field that
can carry a command-line argument is filled with a value containing at least
one character a shell would split, expand or reinterpret: spaces, ASCII single
and double quotes, smart quotes (U+2019, U+201C/U+201D), `$`, `;`, `&`, `|`,
backticks, `\`, `?`, `*`, `<`, `>`, `#`, `@`, commas and non-ASCII letters.
The remaining smart-quote variants (U+2018, U+201A, U+201B) are covered by the
"smart quotes" rows in `tst_terminalcatalog`.

It is checked automatically by `tst_session::theEscapingSessionRoundTrips`
(byte-identical save, exact argv, preview, both copy actions and PowerShell
script) and by the "escaping session" rows in `tst_terminalcatalog`, which
read every hostile value from this file and round-trip them through real
`/bin/sh` and PowerShell processes.

## How to use it

1. Build and start the GUI, then **File → Import Session…** and select
   `tests/data/escaping_session.json`.
2. Configure a hashcat binary in Settings (a stub is enough — nothing is
   cracked in this test) and a terminal.
3. Inspect the **Command** preview. Switch the configured terminal between a
   POSIX terminal (`xterm`, `konsole`, …) and `pwsh`/`powershell.exe` in
   Settings: the preview must re-quote for the chosen shell. Flags and plain
   values stay bare; metacharacters, spaces and non-ASCII text get quoted; the
   value with a smart single quote is carried as base64 in PowerShell.
4. **Tools → Copy as PowerShell Script**, paste it into a PowerShell or pwsh
   window and confirm hashcat receives the values as printed in the preview —
   no shell expansion, no split arguments, no injected commands.
5. **Start** the job in a terminal. The Unix wrapper passes the arguments as
   positional parameters and the PowerShell script uses `ProcessStartInfo`, so
   no value may be parsed as shell code. In particular `$(touch /tmp/x)` and
   `$(id)` must arrive literally: `/tmp/x` must not be created and no command
   output may appear.

The session uses attack mode 1 (Combination) with hash mode 4000 so that the
recipe, both rule fields, three rules files, two wordlists and most global
options are on the command line at once. Wordlists, tables, rulesets and feed
fields also hold hostile values for when you switch attack modes. `--cache-path`
is intentionally empty because hashcat requires an existing directory there;
point it at a directory with a space in its name to exercise that field too.
