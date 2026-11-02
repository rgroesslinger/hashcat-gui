<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="de" sourcelanguage="en">
<context>
    <name>AboutDialog</name>
    <message>
        <source>About</source>
        <translation>Über hashcat-gui</translation>
    </message>
    <message>
        <source>Graphical user interface for hashcat, the password recovery utility</source>
        <translation>Grafische Benutzeroberfläche für hashcat, das Tool zur Passwortwiederherstellung</translation>
    </message>
    <message>
        <source>Version information</source>
        <translation>Versionsinformationen</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Unbekannt</translation>
    </message>
    <message>
        <source>Updates</source>
        <translation>Updates</translation>
    </message>
    <message>
        <source>Checking for updates…</source>
        <translation>Es wird nach Updates gesucht…</translation>
    </message>
    <message>
        <source>System information</source>
        <translation>Systeminformationen</translation>
    </message>
    <message>
        <source>Operating system</source>
        <translation>Betriebssystem</translation>
    </message>
    <message>
        <source>Links</source>
        <translation>Links</translation>
    </message>
    <message>
        <source>License</source>
        <translation>Lizenz</translation>
    </message>
    <message>
        <source>Copy the application, toolkit, system and hashcat versions to the clipboard</source>
        <translation>Versionen von Anwendung, Toolkit, System und hashcat in die Zwischenablage kopieren</translation>
    </message>
    <message>
        <source>Copy details</source>
        <translation>Details kopieren</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Copied to the clipboard</source>
        <translation>In die Zwischenablage kopiert</translation>
    </message>
    <message>
        <source>Project page</source>
        <translation>Projektseite</translation>
    </message>
    <message>
        <source>Report an issue</source>
        <translation>Problem melden</translation>
    </message>
    <message>
        <source>hashcat</source>
        <translation>hashcat</translation>
    </message>
    <message>
        <source>hashcat FAQ</source>
        <translation>hashcat-FAQ</translation>
    </message>
    <message>
        <source>hashcat-gui is licensed under the %1.</source>
        <translation>hashcat-gui steht unter der %1.</translation>
    </message>
    <message>
        <source>GNU GPL v3.0 or later</source>
        <translation>GNU GPL v3.0 oder neuer</translation>
    </message>
    <message>
        <source>Not configured</source>
        <translation>Nicht konfiguriert</translation>
    </message>
    <message>
        <source>Error: %1</source>
        <translation>Fehler: %1</translation>
    </message>
    <message>
        <source>A newer version is available: %1.</source>
        <translation>Eine neuere Version ist verfügbar: %1.</translation>
    </message>
    <message>
        <source>Release page</source>
        <translation>Release-Seite</translation>
    </message>
    <message>
        <source>No updates available.</source>
        <translation>Keine Updates verfügbar.</translation>
    </message>
    <message>
        <source>Could not check for updates.</source>
        <translation>Die Suche nach Updates ist fehlgeschlagen.</translation>
    </message>
    <message>
        <source>Could not check for updates: %1</source>
        <translation>Die Suche nach Updates ist fehlgeschlagen: %1</translation>
    </message>
</context>
<context>
    <name>HashcatInfoParser</name>
    <message>
        <source>Invalid JSON returned from hashcat.</source>
        <translation>hashcat hat ungültiges JSON zurückgegeben.</translation>
    </message>
    <message>
        <source>hashcat did not report any hash types.</source>
        <translation>hashcat hat keine Hash-Typen gemeldet.</translation>
    </message>
</context>
<context>
    <name>HelperUtils</name>
    <message>
        <source>The hashcat path is not configured.</source>
        <translation>Der Pfad zu hashcat ist nicht konfiguriert.</translation>
    </message>
    <message>
        <source>Failed to start hashcat
%1</source>
        <translation>hashcat konnte nicht gestartet werden
%1</translation>
    </message>
    <message>
        <source>hashcat timed out
%1</source>
        <translation>hashcat hat das Zeitlimit erreicht
%1</translation>
    </message>
    <message>
        <source>No supported terminal was found on this system. hashcat-gui needs one to show the hashcat output.</source>
        <translation>Auf diesem System wurde kein unterstütztes Terminal gefunden. hashcat-gui benötigt eines, um die Ausgabe von hashcat anzuzeigen.</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <source>Hash file or hash:</source>
        <translation>Hash-Datei oder Hash:</translation>
    </message>
    <message>
        <source>The hash, a file of hashes, or a .hc22000/.hccapx WPA capture.</source>
        <translation>Der Hash, eine Datei mit Hashes oder ein WPA-Capture (.hc22000/.hccapx).</translation>
    </message>
    <message>
        <source>Choose a file of hashes.</source>
        <translation>Eine Datei mit Hashes auswählen.</translation>
    </message>
    <message>
        <source>Salt is hex</source>
        <translation>Salt ist Hex</translation>
    </message>
    <message>
        <source>Rewrite the hash file without cracked hashes, including potfile-known ones; the potfile is untouched. Refused in attack mode 9.</source>
        <translation>Die Hash-Datei ohne geknackte Hashes neu schreiben, einschließlich der bereits in der Potfile bekannten; die Potfile bleibt unverändert. Wird im Angriffsmodus 9 abgelehnt.</translation>
    </message>
    <message>
        <source>Remove hash from hash list once it is cracked</source>
        <translation>Hash nach dem Knacken aus der Liste entfernen</translation>
    </message>
    <message>
        <source>Ignore the username before the mode’s separator (usually “:”), so accounts sharing a hash are cracked once.</source>
        <translation>Den Benutzernamen vor dem Trennzeichen des Modus (meist „:“) ignorieren, damit Konten mit demselben Hash nur einmal geknackt werden.</translation>
    </message>
    <message>
        <source>Hash recipe, e.g. “md5(md5(salt) . pass)”. Wrap it in the shell’s outer quotes and put any inner strings in the other kind of quotes; PowerShell needs a backtick before $.</source>
        <translation>Hash-Rezept, z. B. „md5(md5(salt) . pass)“. Setzen Sie es in die äußeren Anführungszeichen der Shell und innere Zeichenketten in die andere Art von Anführungszeichen; PowerShell benötigt vor $ ein Backtick.</translation>
    </message>
    <message>
        <source>Move item up</source>
        <translation>Eintrag nach oben verschieben</translation>
    </message>
    <message>
        <source>Move selected wordlist up</source>
        <translation>Ausgewählte Wortliste nach oben verschieben</translation>
    </message>
    <message>
        <source>Move item down</source>
        <translation>Eintrag nach unten verschieben</translation>
    </message>
    <message>
        <source>Move selected wordlist down</source>
        <translation>Ausgewählte Wortliste nach unten verschieben</translation>
    </message>
    <message>
        <source>Add one or more wordlist files. Each checked row is passed to hashcat.</source>
        <translation>Eine oder mehrere Wortlisten-Dateien hinzufügen. Jede markierte Zeile wird an hashcat übergeben.</translation>
    </message>
    <message>
        <source>Remove the selected wordlists.</source>
        <translation>Die ausgewählten Wortlisten entfernen.</translation>
    </message>
    <message>
        <source>Remove wordlists</source>
        <translation>Wortlisten entfernen</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Entfernen</translation>
    </message>
    <message>
        <source>Wordlist is hex</source>
        <translation>Wortliste ist Hex</translation>
    </message>
    <message>
        <source>Encoding from:</source>
        <translation>Kodierung von:</translation>
    </message>
    <message>
        <source>Encoding to:</source>
        <translation>Kodierung nach:</translation>
    </message>
    <message>
        <source>Space-separated key=value settings for attack mode 9, e.g. phases=words,rules or rules.rulefile=…. Only used without a wordlist.</source>
        <translation>Durch Leerzeichen getrennte key=value-Einstellungen für Angriffsmodus 9, z. B. phases=words,rules oder rules.rulefile=…. Wird nur ohne Wortliste verwendet.</translation>
    </message>
    <message>
        <source>A ruleset file, a folder with Grammar/grammar.txt, or a bundled name. Unchecked rows are skipped; none runs default-passwords.</source>
        <translation>Eine Regelsatz-Datei, ein Ordner mit Grammar/grammar.txt oder ein mitgelieferter Name. Nicht markierte Zeilen werden übersprungen; ist keine Zeile markiert, läuft default-passwords.</translation>
    </message>
    <message>
        <source>Add one or more PCFG ruleset files.</source>
        <translation>Eine oder mehrere PCFG-Regelsatz-Dateien hinzufügen.</translation>
    </message>
    <message>
        <source>Add PCFG rulesets</source>
        <translation>PCFG-Regelsätze hinzufügen</translation>
    </message>
    <message>
        <source>Remove the selected rulesets.</source>
        <translation>Die ausgewählten Regelsätze entfernen.</translation>
    </message>
    <message>
        <source>Remove rulesets</source>
        <translation>Regelsätze entfernen</translation>
    </message>
    <message>
        <source>Move selected ruleset up</source>
        <translation>Ausgewählten Regelsatz nach oben verschieben</translation>
    </message>
    <message>
        <source>Move selected ruleset down</source>
        <translation>Ausgewählten Regelsatz nach unten verschieben</translation>
    </message>
    <message>
        <source>One plain number per ruleset, colon-separated, e.g. 2:1; the count must match the number of rulesets.</source>
        <translation>Eine einfache Zahl pro Regelsatz, durch Doppelpunkte getrennt, z. B. 2:1; die Anzahl muss mit der Anzahl der Regelsätze übereinstimmen.</translation>
    </message>
    <message>
        <source>Maximum candidate length the kernel builds, in 4-byte words: 0 (auto), 16 or 32.</source>
        <translation>Maximale Kandidatenlänge, die der Kernel aufbaut, in 4-Byte-Wörtern: 0 (auto), 16 oder 32.</translation>
    </message>
    <message>
        <source>1 steps to the next base word where the order allows instead of unranking it; the candidates are identical, only the fast path is off.</source>
        <translation>1 geht, wo die Reihenfolge es zulässt, zum nächsten Basiswort weiter, statt es per Unranking zu bestimmen; die Kandidaten sind identisch, nur der schnelle Pfad ist deaktiviert.</translation>
    </message>
    <message>
        <source>Only enumerate candidates this mask admits; excludes omen=1, and hintaccount unless hint words come from hintwords or hintfile.</source>
        <translation>Nur Kandidaten aufzählen, die diese Maske zulässt; schließt omen=1 aus sowie hintaccount, sofern die Hinweiswörter nicht aus hintwords oder hintfile stammen.</translation>
    </message>
    <message>
        <source>Space-separated key=value settings: omen, cache, lookup, pwmin, pwmax, hintaccount, hintwords, hintfile, hintrank, hintrepeat. Rulesets go first.</source>
        <translation>Durch Leerzeichen getrennte key=value-Einstellungen: omen, cache, lookup, pwmin, pwmax, hintaccount, hintwords, hintfile, hintrank, hintrepeat. Regelsätze kommen zuerst.</translation>
    </message>
    <message>
        <source>Add one or more table files.</source>
        <translation>Eine oder mehrere Tabellendateien hinzufügen.</translation>
    </message>
    <message>
        <source>Remove the selected tables.</source>
        <translation>Die ausgewählten Tabellen entfernen.</translation>
    </message>
    <message>
        <source>Remove tables</source>
        <translation>Tabellen entfernen</translation>
    </message>
    <message>
        <source>1 (default) keeps a token unchanged as a choice, 0 drops it so the table only yields variations. identity=0 excludes single=1.</source>
        <translation>1 (Standard) behält ein Token unverändert als Option bei, 0 verwirft es, sodass die Tabelle nur Varianten liefert. identity=0 schließt single=1 aus.</translation>
    </message>
    <message>
        <source>1 reads the wordlist as patterns: no word is returned unchanged and unmatched words are dropped. 0 (default) keeps them.</source>
        <translation>1 liest die Wortliste als Muster: Kein Wort wird unverändert zurückgegeben, und nicht passende Wörter werden verworfen. 0 (Standard) behält sie bei.</translation>
    </message>
    <message>
        <source>single=1: one substitution per candidate instead of the cross product; fewer candidates, more host work on fast hashes.</source>
        <translation>single=1: eine Ersetzung pro Kandidat statt des Kreuzprodukts; weniger Kandidaten, mehr Host-Arbeit bei schnellen Hashes.</translation>
    </message>
    <message>
        <source>Keep only the first N replacements of each source word; 0 means no limit. Applies to single=1 too.</source>
        <translation>Nur die ersten N Ersetzungen jedes Quellworts behalten; 0 bedeutet kein Limit. Gilt auch für single=1.</translation>
    </message>
    <message>
        <source>How much of a word’s cross product the host enumerates before giving up; 0 means no limit.</source>
        <translation>Wie viel vom Kreuzprodukt eines Worts der Host aufzählt, bevor er aufgibt; 0 bedeutet kein Limit.</translation>
    </message>
    <message>
        <source>Key=value settings separated by spaces; everything after the first one is a setting, so tables come first.</source>
        <translation>Key=value-Einstellungen, durch Leerzeichen getrennt; alles nach der ersten ist eine Einstellung, daher kommen die Tabellen zuerst.</translation>
    </message>
    <message>
        <source>Feed name or path. A slash means a path; otherwise hashcat tries feed_&lt;name&gt;, rust_&lt;name&gt;, then &lt;name&gt; in its feeds directory.</source>
        <translation>Feed-Name oder -Pfad. Ein Schrägstrich bedeutet einen Pfad; andernfalls probiert hashcat feed_&lt;name&gt;, rust_&lt;name&gt; und dann &lt;name&gt; in seinem Feed-Verzeichnis.</translation>
    </message>
    <message>
        <source>Browse for a feed plugin instead of naming it.</source>
        <translation>Nach einem Feed-Plugin suchen, statt es zu benennen.</translation>
    </message>
    <message>
        <source>Browse for a feed plugin</source>
        <translation>Nach einem Feed-Plugin suchen</translation>
    </message>
    <message>
        <source>Argument passed to the feed; meaning and count depend on the feed (the wordlist feed takes wordlist paths).</source>
        <translation>An den Feed übergebenes Argument; Bedeutung und Anzahl hängen vom Feed ab (der wordlist-Feed nimmt Wortlisten-Pfade entgegen).</translation>
    </message>
    <message>
        <source>Add an argument for the feed, in order.</source>
        <translation>Ein Argument für den Feed hinzufügen, der Reihe nach.</translation>
    </message>
    <message>
        <source>Remove the selected feed arguments.</source>
        <translation>Die ausgewählten Feed-Argumente entfernen.</translation>
    </message>
    <message>
        <source>Remove feed arguments</source>
        <translation>Feed-Argumente entfernen</translation>
    </message>
    <message>
        <source>Move selected feed argument up</source>
        <translation>Ausgewähltes Feed-Argument nach oben verschieben</translation>
    </message>
    <message>
        <source>Move selected feed argument down</source>
        <translation>Ausgewähltes Feed-Argument nach unten verschieben</translation>
    </message>
    <message>
        <source>Read rules from the files below instead of generating them.</source>
        <translation>Regeln aus den unten stehenden Dateien lesen, statt sie zu erzeugen.</translation>
    </message>
    <message>
        <source>Rules file 1:</source>
        <translation>Regeldatei 1:</translation>
    </message>
    <message>
        <source>Choose the rules file for slot 1.</source>
        <translation>Die Regeldatei für Slot 1 auswählen.</translation>
    </message>
    <message>
        <source>Open rules file 1</source>
        <translation>Regeldatei 1 öffnen</translation>
    </message>
    <message>
        <source>Rules file 2:</source>
        <translation>Regeldatei 2:</translation>
    </message>
    <message>
        <source>Choose the rules file for slot 2.</source>
        <translation>Die Regeldatei für Slot 2 auswählen.</translation>
    </message>
    <message>
        <source>Open rules file 2</source>
        <translation>Regeldatei 2 öffnen</translation>
    </message>
    <message>
        <source>Rules file 3:</source>
        <translation>Regeldatei 3:</translation>
    </message>
    <message>
        <source>Choose the rules file for slot 3.</source>
        <translation>Die Regeldatei für Slot 3 auswählen.</translation>
    </message>
    <message>
        <source>Open rules file 3</source>
        <translation>Regeldatei 3 öffnen</translation>
    </message>
    <message>
        <source>Number of random rules hashcat generates (-g). Only used with Generate rules; no rules file is read.</source>
        <translation>Anzahl der Zufallsregeln, die hashcat erzeugt (-g). Wird nur mit „Regeln erzeugen“ verwendet; es wird keine Regeldatei gelesen.</translation>
    </message>
    <message>
        <source>Treat every -r file as one list: rule counts add instead of multiplying.</source>
        <translation>Jede -r-Datei als eine Liste behandeln: Regelanzahlen werden addiert statt multipliziert.</translation>
    </message>
    <message>
        <source>Wordlist rules</source>
        <translation>Wortlisten-Regeln</translation>
    </message>
    <message>
        <source>Left rule:</source>
        <translation>Linke Regel:</translation>
    </message>
    <message>
        <source>Right rule:</source>
        <translation>Rechte Regel:</translation>
    </message>
    <message>
        <source>Interpret the mask and custom charsets as hexadecimal bytes.</source>
        <translation>Die Maske und die eigenen Zeichensätze als hexadezimale Bytes interpretieren.</translation>
    </message>
    <message>
        <source>Charset is hex</source>
        <translation>Zeichensatz ist Hex</translation>
    </message>
    <message>
        <source>Ctrl+Shift+I</source>
        <translation>Ctrl+Shift+I</translation>
    </message>
    <message>
        <source>Increment mask length</source>
        <translation>Maskenlänge erhöhen</translation>
    </message>
    <message>
        <source>Min:</source>
        <translation>Min:</translation>
    </message>
    <message>
        <source>Max:</source>
        <translation>Max:</translation>
    </message>
    <message>
        <source>Use hashcat’s optimized kernels (-O); they cap the maximum password length.</source>
        <translation>Die optimierten Kernel von hashcat verwenden (-O); sie begrenzen die maximale Passwortlänge.</translation>
    </message>
    <message>
        <source>Pin hashcat to these CPU devices, separated by commas. Leave empty to let hashcat decide.</source>
        <translation>hashcat an diese CPU-Geräte binden, durch Kommas getrennt. Leer lassen, damit hashcat entscheidet.</translation>
    </message>
    <message>
        <source>Devices to use, separated by commas. Leave empty for all devices.</source>
        <translation>Zu verwendende Geräte, durch Kommas getrennt. Leer lassen für alle Geräte.</translation>
    </message>
    <message>
        <source>Abort above</source>
        <translation>Abbruch über</translation>
    </message>
    <message>
        <source>Outfile</source>
        <translation>Ausgabedatei</translation>
    </message>
    <message>
        <source>Save…</source>
        <translation>Speichern…</translation>
    </message>
    <message>
        <source>Write the outfile as JSON. The outfile format is ignored.</source>
        <translation>Die Ausgabedatei als JSON schreiben. Das Format der Ausgabedatei wird ignoriert.</translation>
    </message>
    <message>
        <source>Write JSON</source>
        <translation>JSON schreiben</translation>
    </message>
    <message>
        <source>Do not write recovered hashes to the potfile.</source>
        <translation>Gefundene Hashes nicht in die Potfile schreiben.</translation>
    </message>
    <message>
        <source>Load Example Session</source>
        <translation>Beispielsitzung laden</translation>
    </message>
    <message>
        <source>Reset Session</source>
        <translation>Sitzung zurücksetzen</translation>
    </message>
    <message>
        <source>Mode:</source>
        <translation>Modus:</translation>
    </message>
    <message>
        <source>Attack</source>
        <translation>Angriff</translation>
    </message>
    <message>
        <source>Open…</source>
        <translation>Öffnen…</translation>
    </message>
    <message>
        <source>Assume the salt is given in hexadecimal.</source>
        <translation>Annehmen, dass der Salt hexadezimal angegeben ist.</translation>
    </message>
    <message>
        <source>Attack mode. Each choice reveals its own options below.</source>
        <translation>Angriffsmodus. Jede Auswahl blendet unten eigene Optionen ein.</translation>
    </message>
    <message>
        <source>Hash type:</source>
        <translation>Hash-Typ:</translation>
    </message>
    <message>
        <source>Hash recipe:</source>
        <translation>Hash-Rezept:</translation>
    </message>
    <message>
        <source>Add files…</source>
        <translation>Dateien hinzufügen…</translation>
    </message>
    <message>
        <source>hashcat reads every file directly inside the folder as one wordlist.</source>
        <translation>hashcat liest jede Datei direkt im Ordner als eine Wortliste.</translation>
    </message>
    <message>
        <source>Add folder…</source>
        <translation>Ordner hinzufügen…</translation>
    </message>
    <message>
        <source>Only checked rows are used. A folder is one wordlist: hashcat reads the files directly inside it.</source>
        <translation>Es werden nur markierte Zeilen verwendet. Ein Ordner ist eine Wortliste: hashcat liest die Dateien direkt darin.</translation>
    </message>
    <message>
        <source>Assume words in the wordlist are given in hexadecimal.</source>
        <translation>Annehmen, dass die Wörter der Wortliste hexadezimal angegeben sind.</translation>
    </message>
    <message>
        <source>Settings (no wordlist):</source>
        <translation>Einstellungen (keine Wortliste):</translation>
    </message>
    <message>
        <source>PCFG</source>
        <translation>PCFG</translation>
    </message>
    <message>
        <source>A ruleset folder needs a Grammar/grammar.txt.</source>
        <translation>Ein Regelsatz-Ordner benötigt eine Grammar/grammar.txt.</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Scale:</translation>
    </message>
    <message>
        <source>Quantization steps per bit (1–64)</source>
        <translation>Quantisierungsschritte pro Bit (1–64)</translation>
    </message>
    <message>
        <source>Costmax:</source>
        <translation>Costmax:</translation>
    </message>
    <message>
        <source>Highest cost level to enumerate (1–64)</source>
        <translation>Höchste aufzuzählende Kostenstufe (1–64)</translation>
    </message>
    <message>
        <source>Weights:</source>
        <translation>Weights:</translation>
    </message>
    <message>
        <source>Threads:</source>
        <translation>Threads:</translation>
    </message>
    <message>
        <source>Cores that generate base words (0–16); 0 runs on the calling thread.</source>
        <translation>Kerne, die Basiswörter erzeugen (0–16); 0 läuft im aufrufenden Thread.</translation>
    </message>
    <message>
        <source>auto</source>
        <translation>auto</translation>
    </message>
    <message>
        <source>Kbits:</source>
        <translation>Kbits:</translation>
    </message>
    <message>
        <source>Bits one inner loop may span (0–27)</source>
        <translation>Bits, die eine innere Schleife umfassen darf (0–27)</translation>
    </message>
    <message>
        <source>Maxword:</source>
        <translation>Maxword:</translation>
    </message>
    <message>
        <source>Maxgain:</source>
        <translation>Maxgain:</translation>
    </message>
    <message>
        <source>1.0–64.0; decimal point is a dot, not the locale separator.</source>
        <translation>1.0–64.0; als Dezimaltrennzeichen dient ein Punkt, nicht das Trennzeichen der Regionaleinstellung.</translation>
    </message>
    <message>
        <source>Walk:</source>
        <translation>Walk:</translation>
    </message>
    <message>
        <source>Mask:</source>
        <translation>Maske:</translation>
    </message>
    <message>
        <source>Extra settings (key=value):</source>
        <translation>Zusätzliche Einstellungen (key=value):</translation>
    </message>
    <message>
        <source>Table</source>
        <translation>Tabelle</translation>
    </message>
    <message>
        <source>Every checked row after the wordlist is a table.</source>
        <translation>Jede markierte Zeile nach der Wortliste ist eine Tabelle.</translation>
    </message>
    <message>
        <source>Add tables…</source>
        <translation>Tabellen hinzufügen…</translation>
    </message>
    <message>
        <source>Identity:</source>
        <translation>Identity:</translation>
    </message>
    <message>
        <source>Template:</source>
        <translation>Template:</translation>
    </message>
    <message>
        <source>Single:</source>
        <translation>Single:</translation>
    </message>
    <message>
        <source>Cap:</source>
        <translation>Cap:</translation>
    </message>
    <message>
        <source>Maxperm:</source>
        <translation>Maxperm:</translation>
    </message>
    <message>
        <source>Generic feed</source>
        <translation>Generischer Feed</translation>
    </message>
    <message>
        <source>Feed:</source>
        <translation>Feed:</translation>
    </message>
    <message>
        <source>wordlist, stdin, random, association, pcfg, table, hybrid, mask, or a feed path</source>
        <translation>wordlist, stdin, random, association, pcfg, table, hybrid, mask oder ein Feed-Pfad</translation>
    </message>
    <message>
        <source>Browse…</source>
        <translation>Durchsuchen…</translation>
    </message>
    <message>
        <source>Argument:</source>
        <translation>Argument:</translation>
    </message>
    <message>
        <source>Add</source>
        <translation>Hinzufügen</translation>
    </message>
    <message>
        <source>Arguments the feed reads, in order; only checked rows are used.</source>
        <translation>Argumente, die der Feed in dieser Reihenfolge liest; es werden nur markierte Zeilen verwendet.</translation>
    </message>
    <message>
        <source>Rules</source>
        <translation>Regeln</translation>
    </message>
    <message>
        <source>Use rules files:</source>
        <translation>Regeldateien verwenden:</translation>
    </message>
    <message>
        <source>Enable rules file slot 1 for this run.</source>
        <translation>Regeldatei-Slot 1 für diesen Lauf aktivieren.</translation>
    </message>
    <message>
        <source>Rules file for slot 1; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>Regeldatei für Slot 1; wird auf jedes Wort angewendet. Leer lassen, um den Slot nicht zu verwenden.</translation>
    </message>
    <message>
        <source>Enable rules file slot 2 for this run.</source>
        <translation>Regeldatei-Slot 2 für diesen Lauf aktivieren.</translation>
    </message>
    <message>
        <source>Rules file for slot 2; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>Regeldatei für Slot 2; wird auf jedes Wort angewendet. Leer lassen, um den Slot nicht zu verwenden.</translation>
    </message>
    <message>
        <source>Enable rules file slot 3 for this run.</source>
        <translation>Regeldatei-Slot 3 für diesen Lauf aktivieren.</translation>
    </message>
    <message>
        <source>Rules file for slot 3; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>Regeldatei für Slot 3; wird auf jedes Wort angewendet. Leer lassen, um den Slot nicht zu verwenden.</translation>
    </message>
    <message>
        <source>Generate the rules automatically instead of reading them from a file.</source>
        <translation>Die Regeln automatisch erzeugen, statt sie aus einer Datei zu lesen.</translation>
    </message>
    <message>
        <source>Generate rules:</source>
        <translation>Regeln erzeugen:</translation>
    </message>
    <message>
        <source>Join -r files into one list</source>
        <translation>-r-Dateien zu einer Liste zusammenfassen</translation>
    </message>
    <message>
        <source>Single rule applied to each word from the left wordlist.</source>
        <translation>Einzelne Regel, die auf jedes Wort der linken Wortliste angewendet wird.</translation>
    </message>
    <message>
        <source>Single rule applied to each word from the right wordlist.</source>
        <translation>Einzelne Regel, die auf jedes Wort der rechten Wortliste angewendet wird.</translation>
    </message>
    <message>
        <source>Mask</source>
        <translation>Maske</translation>
    </message>
    <message>
        <source>Built-in charsets:</source>
        <translation>Eingebaute Zeichensätze:</translation>
    </message>
    <message>
        <source>Custom charsets</source>
        <translation>Eigene Zeichensätze</translation>
    </message>
    <message>
        <source>Charset 1:</source>
        <translation>Zeichensatz 1:</translation>
    </message>
    <message>
        <source>Charset 2:</source>
        <translation>Zeichensatz 2:</translation>
    </message>
    <message>
        <source>Charset 3:</source>
        <translation>Zeichensatz 3:</translation>
    </message>
    <message>
        <source>Charset 4:</source>
        <translation>Zeichensatz 4:</translation>
    </message>
    <message>
        <source>Session and limits</source>
        <translation>Sitzung und Limits</translation>
    </message>
    <message>
        <source>Session name:</source>
        <translation>Sitzungsname:</translation>
    </message>
    <message>
        <source>Name hashcat uses for its restore file and status output.</source>
        <translation>Name, den hashcat für seine Wiederherstellungsdatei und die Statusausgabe verwendet.</translation>
    </message>
    <message>
        <source>Runtime limit:</source>
        <translation>Laufzeitlimit:</translation>
    </message>
    <message>
        <source>unlimited</source>
        <translation>unbegrenzt</translation>
    </message>
    <message>
        <source>Skip:</source>
        <translation>Überspringen:</translation>
    </message>
    <message>
        <source>Limit:</source>
        <translation>Limit:</translation>
    </message>
    <message>
        <source>Keep updating the run status (speed, progress, ETA) instead of printing one final line.</source>
        <translation>Den Laufstatus (Geschwindigkeit, Fortschritt, ETA) fortlaufend aktualisieren, statt nur eine letzte Zeile auszugeben.</translation>
    </message>
    <message>
        <source>Update status automatically</source>
        <translation>Status automatisch aktualisieren</translation>
    </message>
    <message>
        <source>Seconds between status updates; only used when automatic status is enabled.</source>
        <translation>Sekunden zwischen Statusaktualisierungen; wird nur verwendet, wenn der automatische Status aktiviert ist.</translation>
    </message>
    <message>
        <source>Ignore warnings and start hashcat anyway. Use with care.</source>
        <translation>Warnungen ignorieren und hashcat trotzdem starten. Mit Vorsicht verwenden.</translation>
    </message>
    <message>
        <source>Force (ignore warnings)</source>
        <translation>Erzwingen (Warnungen ignorieren)</translation>
    </message>
    <message>
        <source>Performance</source>
        <translation>Leistung</translation>
    </message>
    <message>
        <source>Cache path:</source>
        <translation>Cache-Pfad:</translation>
    </message>
    <message>
        <source>Directory hashcat keeps its cache in. Must exist.</source>
        <translation>Verzeichnis, in dem hashcat seinen Cache ablegt. Muss existieren.</translation>
    </message>
    <message>
        <source>Choose the cache directory.</source>
        <translation>Das Cache-Verzeichnis auswählen.</translation>
    </message>
    <message>
        <source>Choose cache directory</source>
        <translation>Cache-Verzeichnis auswählen</translation>
    </message>
    <message>
        <source>Resources</source>
        <translation>Ressourcen</translation>
    </message>
    <message>
        <source>CPU affinity:</source>
        <translation>CPU-Affinität:</translation>
    </message>
    <message>
        <source>Backend devices:</source>
        <translation>Backend-Geräte:</translation>
    </message>
    <message>
        <source>Device types:</source>
        <translation>Gerätetypen:</translation>
    </message>
    <message>
        <source>Abort when any backend device exceeds this temperature in °C; 0 disables it.</source>
        <translation>Abbrechen, wenn ein Backend-Gerät diese Temperatur in °C überschreitet; 0 deaktiviert die Funktion.</translation>
    </message>
    <message>
        <source>Write recovered hashes to the file below, in the selected formats.</source>
        <translation>Die gefundenen Hashes in die unten angegebene Datei schreiben, in den ausgewählten Formaten.</translation>
    </message>
    <message>
        <source>Write recovered hashes to file:</source>
        <translation>Gefundene Hashes in Datei schreiben:</translation>
    </message>
    <message>
        <source>The file the recovered hashes are written to. Format variables:

&lt;hash&gt; = current hash filename or hash
&lt;unixtime&gt; = current Unix timestamp</source>
        <translation>Die Datei, in die die gefundenen Hashes geschrieben werden. Formatvariablen:

&lt;hash&gt; = aktueller Hash-Dateiname oder Hash
&lt;unixtime&gt; = aktueller Unix-Zeitstempel</translation>
    </message>
    <message>
        <source>Choose the outfile path.</source>
        <translation>Den Pfad der Ausgabedatei auswählen.</translation>
    </message>
    <message>
        <source>Choose outfile</source>
        <translation>Ausgabedatei auswählen</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Format:</translation>
    </message>
    <message>
        <source>Do not write the potfile</source>
        <translation>Potfile nicht schreiben</translation>
    </message>
    <message>
        <source>Run hashcat in a terminal with these options.</source>
        <translation>hashcat mit diesen Optionen in einem Terminal ausführen.</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>Start</translation>
    </message>
    <message>
        <source>Copy command to clipboard</source>
        <translation>Befehl in die Zwischenablage kopieren</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Allgemein</translation>
    </message>
    <message>
        <source>Ignore username in hash file</source>
        <translation>Benutzernamen in der Hash-Datei ignorieren</translation>
    </message>
    <message>
        <source>Wordlists</source>
        <translation>Wortlisten</translation>
    </message>
    <message>
        <source>Increment the mask length from the minimum to the maximum.</source>
        <translation>Die Maskenlänge vom Minimum bis zum Maximum erhöhen.</translation>
    </message>
    <message>
        <source>Minimum mask length; 0 uses hashcat’s default.</source>
        <translation>Minimale Maskenlänge; 0 verwendet den Standardwert von hashcat.</translation>
    </message>
    <message>
        <source>Maximum mask length; 0 uses hashcat’s default.</source>
        <translation>Maximale Maskenlänge; 0 verwendet den Standardwert von hashcat.</translation>
    </message>
    <message>
        <source>Abort the session after this many seconds; 0 runs until hashcat finishes.</source>
        <translation>Die Sitzung nach dieser Anzahl Sekunden abbrechen; 0 läuft, bis hashcat fertig ist.</translation>
    </message>
    <message>
        <source>&#xa0;s</source>
        <translation>&#xa0;s</translation>
    </message>
    <message>
        <source>Skip this many candidates from the start.</source>
        <translation>Diese Anzahl Kandidaten vom Anfang überspringen.</translation>
    </message>
    <message>
        <source>Only test this many candidates after the skipped ones.</source>
        <translation>Nur diese Anzahl Kandidaten nach den übersprungenen testen.</translation>
    </message>
    <message>
        <source>Enable optimized kernels (limits password length)</source>
        <translation>Optimierte Kernel aktivieren (begrenzt die Passwortlänge)</translation>
    </message>
    <message>
        <source>Print only the attack’s speed, then quit; no cracking.</source>
        <translation>Nur die Geschwindigkeit des Angriffs ausgeben und beenden; kein Knacken.</translation>
    </message>
    <message>
        <source>Return expected speed of the attack, then quit</source>
        <translation>Erwartete Angriffsgeschwindigkeit ausgeben und beenden</translation>
    </message>
    <message>
        <source>&#xa0;°C</source>
        <translation>&#xa0;°C</translation>
    </message>
    <message>
        <source>Copy command</source>
        <translation>Befehl kopieren</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>&amp;Hilfe</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Werkzeuge</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Datei</translation>
    </message>
    <message>
        <source>Ctrl+Q</source>
        <translation>Ctrl+Q</translation>
    </message>
    <message>
        <source>Settings…</source>
        <translation>Einstellungen…</translation>
    </message>
    <message>
        <source>Ctrl+,</source>
        <translation>Ctrl+,</translation>
    </message>
    <message>
        <source>Import Settings…</source>
        <translation>Einstellungen importieren…</translation>
    </message>
    <message>
        <source>Ctrl+Shift+O</source>
        <translation>Ctrl+Shift+O</translation>
    </message>
    <message>
        <source>Export Settings…</source>
        <translation>Einstellungen exportieren…</translation>
    </message>
    <message>
        <source>Ctrl+Shift+S</source>
        <translation>Ctrl+Shift+S</translation>
    </message>
    <message>
        <source>Export Session…</source>
        <translation>Sitzung exportieren…</translation>
    </message>
    <message>
        <source>Ctrl+Shift+E</source>
        <translation>Ctrl+Shift+E</translation>
    </message>
    <message>
        <source>Import Session…</source>
        <translation>Sitzung importieren…</translation>
    </message>
    <message>
        <source>About</source>
        <translation>Über hashcat-gui</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation>Beenden</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>Über Qt</translation>
    </message>
    <message>
        <source>JSON Files (*.json)</source>
        <translation>JSON-Dateien (*.json)</translation>
    </message>
    <message>
        <source>The session file could not be read, so the session was not restored. The file was left in place.

%1</source>
        <translation>Die Sitzungsdatei konnte nicht gelesen werden, daher wurde die Sitzung nicht wiederhergestellt. Die Datei blieb unverändert.

%1</translation>
    </message>
    <message>
        <source>Save failed</source>
        <translation>Speichern fehlgeschlagen</translation>
    </message>
    <message>
        <source>Outfile format to use, separated by commas.
Default: 1,2</source>
        <translation>Zu verwendendes Format der Ausgabedatei, durch Kommas getrennt.
Standard: 1,2</translation>
    </message>
    <message>
        <source>Common encodings:</source>
        <translation>Gängige Kodierungen:</translation>
    </message>
    <message>
        <source>Force the internal wordlist encoding from this code. Leave empty to let hashcat decide.</source>
        <translation>Interne Kodierung der Wortliste von diesem Code aus erzwingen. Leer lassen, damit hashcat entscheidet.</translation>
    </message>
    <message>
        <source>Force the internal wordlist encoding to this code. Leave empty to let hashcat decide.</source>
        <translation>Interne Kodierung der Wortliste auf diesen Code erzwingen. Leer lassen, damit hashcat entscheidet.</translation>
    </message>
    <message>
        <source>Exported</source>
        <translation>Exportiert</translation>
    </message>
    <message>
        <source>Export failed</source>
        <translation>Export fehlgeschlagen</translation>
    </message>
    <message>
        <source>Import failed</source>
        <translation>Import fehlgeschlagen</translation>
    </message>
    <message>
        <source>Imported</source>
        <translation>Importiert</translation>
    </message>
    <message>
        <source>The file was made by an older version of hashcat-gui (format version %1; this version reads format version %2).</source>
        <translation>Die Datei wurde von einer älteren Version von hashcat-gui erstellt (Formatversion %1; diese Version liest Formatversion %2).</translation>
    </message>
    <message>
        <source>The file holds sections this import does not use: %1.</source>
        <translation>Die Datei enthält Abschnitte, die dieser Import nicht verwendet: %1.</translation>
    </message>
    <message>
        <source>The file is in format version %1, but this version reads format version %2, so it was not applied. Update hashcat-gui to read it.</source>
        <translation>Die Datei hat die Formatversion %1, diese Version liest aber Formatversion %2, daher wurde sie nicht angewendet. Aktualisieren Sie hashcat-gui, um sie zu lesen.</translation>
    </message>
    <message>
        <source>The session file is in format version %1, but this version reads format version %2, so the session was not restored. The file was left as it is, so the session of this run is not stored either. Update hashcat-gui to read it.</source>
        <translation>Die Sitzungsdatei hat die Formatversion %1, diese Version liest aber Formatversion %2, daher wurde die Sitzung nicht wiederhergestellt. Die Datei blieb unverändert, daher wird auch die Sitzung dieses Laufs nicht gespeichert. Aktualisieren Sie hashcat-gui, um sie zu lesen.</translation>
    </message>
    <message>
        <source>Settings exported to %1.</source>
        <translation>Einstellungen nach %1 exportiert.</translation>
    </message>
    <message>
        <source>OpenCL device types to use, separated by commas.</source>
        <translation>Zu verwendende OpenCL-Gerätetypen, durch Kommas getrennt.</translation>
    </message>
    <message>
        <source>Export session</source>
        <translation>Sitzung exportieren</translation>
    </message>
    <message>
        <source>Session exported to “%1”.</source>
        <translation>Sitzung nach „%1“ exportiert.</translation>
    </message>
    <message>
        <source>Import session</source>
        <translation>Sitzung importieren</translation>
    </message>
    <message>
        <source>Session imported from “%1”.</source>
        <translation>Sitzung aus „%1“ importiert.</translation>
    </message>
    <message>
        <source>Session imported from “%1”.

%2</source>
        <translation>Sitzung aus „%1“ importiert.

%2</translation>
    </message>
    <message>
        <source>Export settings</source>
        <translation>Einstellungen exportieren</translation>
    </message>
    <message>
        <source>Import settings</source>
        <translation>Einstellungen importieren</translation>
    </message>
    <message>
        <source>Settings loaded from “%1”.</source>
        <translation>Einstellungen aus „%1“ geladen.</translation>
    </message>
    <message>
        <source>Review them and save to apply them.</source>
        <translation>Prüfen Sie die Einstellungen und speichern Sie, um sie anzuwenden.</translation>
    </message>
    <message>
        <source>Load example session</source>
        <translation>Beispielsitzung laden</translation>
    </message>
    <message>
        <source>Reset session</source>
        <translation>Sitzung zurücksetzen</translation>
    </message>
    <message>
        <source>Reset every field to its default? The current session is lost.</source>
        <translation>Jedes Feld auf den Standardwert zurücksetzen? Die aktuelle Sitzung geht verloren.</translation>
    </message>
    <message>
        <source>Session warning</source>
        <translation>Sitzungswarnung</translation>
    </message>
    <message>
        <source>Updating…</source>
        <translation>Wird aktualisiert…</translation>
    </message>
    <message>
        <source>Failed to load hash types</source>
        <translation>Hash-Typen konnten nicht geladen werden</translation>
    </message>
    <message>
        <source>This mode needs a wordlist and a mask.</source>
        <translation>Dieser Modus benötigt eine Wortliste und eine Maske.</translation>
    </message>
    <message>
        <source>This mode needs at least one wordlist.</source>
        <translation>Dieser Modus benötigt mindestens eine Wortliste.</translation>
    </message>
    <message>
        <source>Open hash file</source>
        <translation>Hash-Datei öffnen</translation>
    </message>
    <message>
        <source>Save output file</source>
        <translation>Ausgabedatei speichern</translation>
    </message>
    <message>
        <source>Add wordlists</source>
        <translation>Wortlisten hinzufügen</translation>
    </message>
    <message>
        <source>Open rules file</source>
        <translation>Regeldatei öffnen</translation>
    </message>
    <message>
        <source>Could not start “%1”: %2</source>
        <translation>„%1“ konnte nicht gestartet werden: %2</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not available. Available terminals: %2
Navigate to %3 → %4 to change it.</source>
        <translation>Das konfigurierte Terminal „%1“ ist nicht verfügbar. Verfügbare Terminals: %2
Gehen Sie zu %3 → %4, um es zu ändern.</translation>
    </message>
    <message>
        <source>This mode needs exactly %1 wordlists (left, right).
Currently selected: %2.</source>
        <translation>Dieser Modus benötigt genau %1 Wortlisten (links, rechts).
Aktuell ausgewählt: %2.</translation>
    </message>
    <message>
        <source>The session names the hash type %1, which this hashcat version does not support.</source>
        <translation>Die Sitzung nennt den Hash-Typ %1, den diese hashcat-Version nicht unterstützt.</translation>
    </message>
    <message>
        <source>Password candidate mask. ?1–?4 insert a custom charset, ?w inserts the wordlist word and ?q a second wordlist word in the Hybrid (mask + ?w/?q) mode, and ?? is a literal ?.</source>
        <translation>Maske für Passwortkandidaten. ?1–?4 fügen einen eigenen Zeichensatz ein, ?w fügt das Wort der Wortliste ein und ?q ein zweites Wort der Wortliste im Modus „Hybrid (Maske + ?w/?q)“, und ?? ist ein literales ?.</translation>
    </message>
    <message>
        <source>Load the %1 example? The current session is lost.

The example cracks a test hash; hashcat should recover the password %2.</source>
        <translation>Das Beispiel %1 laden? Die aktuelle Sitzung geht verloren.

Das Beispiel knackt einen Test-Hash; hashcat sollte das Passwort %2 wiederherstellen.</translation>
    </message>
    <message>
        <source>Could not create the example directory “%1”.</source>
        <translation>Das Beispielverzeichnis „%1“ konnte nicht erstellt werden.</translation>
    </message>
    <message>
        <source>Could not read the bundled file “%1”: %2</source>
        <translation>Die mitgelieferte Datei „%1“ konnte nicht gelesen werden: %2</translation>
    </message>
    <message>
        <source>Could not write “%1”: %2</source>
        <translation>„%1“ konnte nicht geschrieben werden: %2</translation>
    </message>
    <message>
        <source>Straight</source>
        <translation>Wörterbuch</translation>
    </message>
    <message>
        <source>Combination</source>
        <translation>Kombination</translation>
    </message>
    <message>
        <source>Brute-force</source>
        <translation>Brute-Force</translation>
    </message>
    <message>
        <source>Hybrid Wordlist + Mask</source>
        <translation>Hybrid: Wortliste + Maske</translation>
    </message>
    <message>
        <source>Hybrid Mask + Wordlist</source>
        <translation>Hybrid: Maske + Wortliste</translation>
    </message>
    <message>
        <source>Generic (feed)</source>
        <translation>Generisch (Feed)</translation>
    </message>
    <message>
        <source>Association</source>
        <translation>Assoziation</translation>
    </message>
    <message>
        <source>Hybrid (mask + ?w/?q)</source>
        <translation>Hybrid (Maske + ?w/?q)</translation>
    </message>
    <message>
        <source>Failed to obtain supported hash types.
Error: %1</source>
        <translation>Die unterstützten Hash-Typen konnten nicht ermittelt werden.
Fehler: %1</translation>
    </message>
    <message>
        <source>The hash types could not be loaded. Check the hashcat path in Settings.</source>
        <translation>Die Hash-Typen konnten nicht geladen werden. Prüfen Sie den hashcat-Pfad in den Einstellungen.</translation>
    </message>
    <message>
        <source>

This can be caused by Windows Defender or other antivirus software. See the &lt;a href=&quot;%1&quot;&gt;FAQ entry&lt;/a&gt; for a remedy.</source>
        <translation>

Dies kann durch Windows Defender oder andere Antivirensoftware verursacht werden. Abhilfe finden Sie im &lt;a href=&quot;%1&quot;&gt;FAQ-Eintrag&lt;/a&gt;.</translation>
    </message>
    <message>
        <source>Set the hashcat path under %1 → %2 to load the available hash types.</source>
        <translation>Legen Sie den hashcat-Pfad unter %1 → %2 fest, um die verfügbaren Hash-Typen zu laden.</translation>
    </message>
    <message>
        <source>This mode needs exactly 2 wordlists (left, right).</source>
        <translation>Dieser Modus benötigt genau 2 Wortlisten (links, rechts).</translation>
    </message>
    <message>
        <source>This mode needs a mask.</source>
        <translation>Dieser Modus benötigt eine Maske.</translation>
    </message>
    <message>
        <source>This mode runs hashcat’s bundled default-passwords ruleset; name one or more to use your own.</source>
        <translation>Dieser Modus führt den mitgelieferten Regelsatz default-passwords von hashcat aus; nennen Sie einen oder mehrere, um Ihre eigenen zu verwenden.</translation>
    </message>
    <message>
        <source>This mode needs one wordlist and at least one table.</source>
        <translation>Dieser Modus benötigt eine Wortliste und mindestens eine Tabelle.</translation>
    </message>
    <message>
        <source>This mode needs a feed. Arguments depend on the feed: random and stdin need none, wordlist needs a path.</source>
        <translation>Dieser Modus benötigt einen Feed. Die Argumente hängen vom Feed ab: random und stdin brauchen keine, wordlist braucht einen Pfad.</translation>
    </message>
    <message>
        <source>This mode needs at most one wordlist. Without one the words come from the hash file and the settings line can bound the run; with one the two files are paired by line.</source>
        <translation>Dieser Modus benötigt höchstens eine Wortliste. Ohne Wortliste stammen die Wörter aus der Hash-Datei, und die Einstellungszeile kann den Lauf begrenzen; mit Wortliste werden die beiden Dateien zeilenweise gepaart.</translation>
    </message>
    <message>
        <source>This mode needs a mask with exactly one ?w; a ?q after ?w names a second wordlist.</source>
        <translation>Dieser Modus benötigt eine Maske mit genau einem ?w; ein ?q nach ?w benennt eine zweite Wortliste.</translation>
    </message>
    <message>
        <source>Open cache directory</source>
        <translation>Cache-Verzeichnis öffnen</translation>
    </message>
    <message>
        <source>Add wordlist folder</source>
        <translation>Wortlisten-Ordner hinzufügen</translation>
    </message>
    <message>
        <source>Add rulesets</source>
        <translation>Regelsätze hinzufügen</translation>
    </message>
    <message>
        <source>Add ruleset folder</source>
        <translation>Regelsatz-Ordner hinzufügen</translation>
    </message>
    <message>
        <source>Add tables</source>
        <translation>Tabellen hinzufügen</translation>
    </message>
    <message>
        <source>Open feed file</source>
        <translation>Feed-Datei öffnen</translation>
    </message>
    <message>
        <source>This folder has no wordlists. hashcat reads the regular files directly inside it, not subfolders.</source>
        <translation>Dieser Ordner enthält keine Wortlisten. hashcat liest die regulären Dateien direkt darin, keine Unterordner.</translation>
    </message>
    <message>
        <source>This wordlist does not exist:
%1</source>
        <translation>Diese Wortliste existiert nicht:
%1</translation>
    </message>
    <message>
        <source>This ruleset folder has no Grammar/grammar.txt:
%1</source>
        <translation>Dieser Regelsatz-Ordner enthält keine Grammar/grammar.txt:
%1</translation>
    </message>
    <message>
        <source>This ruleset does not exist:
%1</source>
        <translation>Dieser Regelsatz existiert nicht:
%1</translation>
    </message>
    <message>
        <source>A table must be a file; hashcat does not read a folder:
%1</source>
        <translation>Eine Tabelle muss eine Datei sein; hashcat liest keinen Ordner:
%1</translation>
    </message>
    <message>
        <source>This table does not exist:
%1</source>
        <translation>Diese Tabelle existiert nicht:
%1</translation>
    </message>
    <message>
        <source>Copied to the clipboard</source>
        <translation>In die Zwischenablage kopiert</translation>
    </message>
    <message>
        <source>Launch failed</source>
        <translation>Start fehlgeschlagen</translation>
    </message>
    <message>
        <source>Please choose a hash file.</source>
        <translation>Bitte wählen Sie eine Hash-Datei aus.</translation>
    </message>
    <message>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to configure the path to the hashcat executable.</source>
        <translation>Gehen Sie zu &lt;b&gt;%1 → %2&lt;/b&gt;, um den Pfad zum hashcat-Programm festzulegen.</translation>
    </message>
    <message>
        <source>The configured hashcat executable does not exist:
%1
Navigate to %2 → %3 to change it.</source>
        <translation>Das konfigurierte hashcat-Programm existiert nicht:
%1
Gehen Sie zu %2 → %3, um es zu ändern.</translation>
    </message>
    <message>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to select the terminal used for launching.</source>
        <translation>Gehen Sie zu &lt;b&gt;%1 → %2&lt;/b&gt;, um das zum Starten verwendete Terminal auszuwählen.</translation>
    </message>
    <message>
        <source>Maximum number of wordlists for this mode: %1.
Currently selected: %2.</source>
        <translation>Maximale Anzahl an Wortlisten für diesen Modus: %1.
Aktuell ausgewählt: %2.</translation>
    </message>
    <message>
        <source>hashcat pairs the two files by line here; this GUI offers only one wordlist for that role.</source>
        <translation>hashcat paart die beiden Dateien hier zeilenweise; diese GUI bietet für diese Rolle nur eine Wortliste an.</translation>
    </message>
    <message>
        <source>This wordlist has to be a file: hashcat reads it as the multiplied wordlist (the ?q side), and a folder cannot be used there:
%1</source>
        <translation>Diese Wortliste muss eine Datei sein: hashcat liest sie als die multiplizierte Wortliste (die ?q-Seite), und ein Ordner kann dort nicht verwendet werden:
%1</translation>
    </message>
    <message>
        <source>This mode needs at least one table.</source>
        <translation>Dieser Modus benötigt mindestens eine Tabelle.</translation>
    </message>
    <message>
        <source>This mode needs a feed: a bundled name (wordlist, stdin, random, …) or the path of a feed file.</source>
        <translation>Dieser Modus benötigt einen Feed: einen mitgelieferten Namen (wordlist, stdin, random, …) oder den Pfad einer Feed-Datei.</translation>
    </message>
    <message>
        <source>This mode pairs the wordlists with the mask; the mask needs a ?w for the word.</source>
        <translation>Dieser Modus paart die Wortlisten mit der Maske; die Maske benötigt ein ?w für das Wort.</translation>
    </message>
    <message>
        <source>The mask contains more than one ?w.</source>
        <translation>Die Maske enthält mehr als ein ?w.</translation>
    </message>
    <message>
        <source>The mask contains a ?q before the first ?w.</source>
        <translation>Die Maske enthält ein ?q vor dem ersten ?w.</translation>
    </message>
    <message>
        <source>The mask contains more than one ?q.</source>
        <translation>Die Maske enthält mehr als ein ?q.</translation>
    </message>
    <message>
        <source>The mask names a second word with ?q, but only one wordlist is selected.</source>
        <translation>Die Maske benennt mit ?q ein zweites Wort, aber es ist nur eine Wortliste ausgewählt.</translation>
    </message>
    <message>
        <source>Two wordlists are selected, but the mask has no ?q for the second one.</source>
        <translation>Es sind zwei Wortlisten ausgewählt, aber die Maske hat kein ?q für die zweite.</translation>
    </message>
    <message>
        <source>The increment is not offered in this hybrid mode.</source>
        <translation>Das Inkrement wird in diesem Hybridmodus nicht angeboten.</translation>
    </message>
    <message>
        <source>The ?w and ?q markers are only offered in the hybrid mode (mask + wordlists).</source>
        <translation>Die Markierungen ?w und ?q werden nur im Hybridmodus (Maske + Wortlisten) angeboten.</translation>
    </message>
    <message>
        <source>The setting “%1” is not valid for this mode.</source>
        <translation>Die Einstellung „%1“ ist für diesen Modus nicht gültig.</translation>
    </message>
    <message>
        <source>The token “%1” is not a setting; settings are written as key=value.</source>
        <translation>Das Token „%1“ ist keine Einstellung; Einstellungen werden als key=value geschrieben.</translation>
    </message>
    <message>
        <source>The settings line applies only while the words come from the hash file. Clear either the settings line or the wordlist list.</source>
        <translation>Die Einstellungszeile gilt nur, solange die Wörter aus der Hash-Datei stammen. Leeren Sie entweder die Einstellungszeile oder die Wortlistenliste.</translation>
    </message>
    <message>
        <source>The hash type 4000 describes its hashes as a recipe, for example
md5(md5(salt) . pass). Enter it in the hash recipe field.</source>
        <translation>Der Hash-Typ 4000 beschreibt seine Hashes als Rezept, zum Beispiel
md5(md5(salt) . pass). Geben Sie es im Feld „Hash-Rezept“ ein.</translation>
    </message>
    <message>
        <source>The minimum increment length is larger than the maximum.</source>
        <translation>Die minimale Inkrementlänge ist größer als die maximale.</translation>
    </message>
    <message>
        <source>The cache path must be an existing directory:
%1</source>
        <translation>Der Cache-Pfad muss ein vorhandenes Verzeichnis sein:
%1</translation>
    </message>
    <message>
        <source>The increment cannot be combined with the speed-only option.</source>
        <translation>Das Inkrement kann nicht mit der Option „nur Geschwindigkeit“ kombiniert werden.</translation>
    </message>
    <message>
        <source>Removing cracked hashes is not possible in the association attack mode.</source>
        <translation>Das Entfernen geknackter Hashes ist im Angriffsmodus „Assoziation“ nicht möglich.</translation>
    </message>
</context>
<context>
    <name>SettingsDialog</name>
    <message>
        <source>Settings</source>
        <translation>Einstellungen</translation>
    </message>
    <message>
        <source>hashcat executable</source>
        <translation>hashcat-Programm</translation>
    </message>
    <message>
        <source>Not configured</source>
        <translation>Nicht konfiguriert</translation>
    </message>
    <message>
        <source>hashcat</source>
        <translation>hashcat</translation>
    </message>
    <message>
        <source>Select…</source>
        <translation>Auswählen…</translation>
    </message>
    <message>
        <source>&amp;Terminal</source>
        <translation>&amp;Terminal</translation>
    </message>
    <message>
        <source>&amp;Language</source>
        <translation>&amp;Sprache</translation>
    </message>
    <message>
        <source>&amp;Color scheme</source>
        <translation>&amp;Farbschema</translation>
    </message>
    <message>
        <source>Not all color schemes are available on all platforms.</source>
        <translation>Nicht alle Farbschemata sind auf allen Plattformen verfügbar.</translation>
    </message>
    <message>
        <source>&amp;Parameters</source>
        <translation>&amp;Parameter</translation>
    </message>
    <message>
        <source>&amp;Use short parameters</source>
        <translation>&amp;Kurze Parameter verwenden</translation>
    </message>
    <message>
        <source>Session and files</source>
        <translation>Sitzung und Dateien</translation>
    </message>
    <message>
        <source>&amp;Restore the last session at startup</source>
        <translation>Letzte Sitzung beim Start &amp;wiederherstellen</translation>
    </message>
    <message>
        <source>&amp;Session file</source>
        <translation>S&amp;itzungsdatei</translation>
    </message>
    <message>
        <source>The file the last session is stored in</source>
        <translation>Die Datei, in der die letzte Sitzung gespeichert wird</translation>
    </message>
    <message>
        <source>Open folder</source>
        <translation>Ordner öffnen</translation>
    </message>
    <message>
        <source>Open the folder the session is stored in</source>
        <translation>Den Ordner öffnen, in dem die Sitzung gespeichert wird</translation>
    </message>
    <message>
        <source>Settings &amp;file</source>
        <translation>Ei&amp;nstellungsdatei</translation>
    </message>
    <message>
        <source>The file the settings are stored in</source>
        <translation>Die Datei, in der die Einstellungen gespeichert werden</translation>
    </message>
    <message>
        <source>Open the folder the settings are stored in</source>
        <translation>Den Ordner öffnen, in dem die Einstellungen gespeichert werden</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Allgemein</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Speichern</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <source>Download from %1</source>
        <translation>Download von %1</translation>
    </message>
    <message>
        <source>Use the short option names (-m) instead of the long ones (--hash-type) in the command.</source>
        <translation>Die kurzen Optionsnamen (-m) statt der langen (--hash-type) im Befehl verwenden.</translation>
    </message>
    <message>
        <source>Select a terminal</source>
        <translation>Terminal auswählen</translation>
    </message>
    <message>
        <source>System default</source>
        <translation>Systemstandard</translation>
    </message>
    <message>
        <source>Light</source>
        <translation>Hell</translation>
    </message>
    <message>
        <source>Dark</source>
        <translation>Dunkel</translation>
    </message>
    <message>
        <source>The hashcat path “%1” was not found on this computer.</source>
        <translation>Der hashcat-Pfad „%1“ wurde auf diesem Computer nicht gefunden.</translation>
    </message>
    <message>
        <source>The terminal “%1” is not installed and was not applied.</source>
        <translation>Das Terminal „%1“ ist nicht installiert und wurde nicht übernommen.</translation>
    </message>
    <message>
        <source>The color scheme “%1” is not offered and was not applied.</source>
        <translation>Das Farbschema „%1“ wird nicht angeboten und wurde nicht übernommen.</translation>
    </message>
    <message>
        <source>The language “%1” is not offered and was not applied.</source>
        <translation>Die Sprache „%1“ wird nicht angeboten und wurde nicht übernommen.</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not installed.</source>
        <translation>Das konfigurierte Terminal „%1“ ist nicht installiert.</translation>
    </message>
    <message>
        <source>Executable Files (*.exe *.bin hashcat);;All Files (*)</source>
        <translation>Programme (*.exe *.bin hashcat);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>No hashcat executable is configured; hashcat cannot be started.</source>
        <translation>Es ist kein hashcat-Programm konfiguriert; hashcat kann nicht gestartet werden.</translation>
    </message>
    <message>
        <source>The configured hashcat executable was not found on this computer.</source>
        <translation>Das konfigurierte hashcat-Programm wurde auf diesem Computer nicht gefunden.</translation>
    </message>
    <message>
        <source>Select the terminal hashcat is started in.</source>
        <translation>Wählen Sie das Terminal aus, in dem hashcat gestartet wird.</translation>
    </message>
    <message>
        <source>Select the hashcat executable</source>
        <translation>hashcat-Programm auswählen</translation>
    </message>
    <message>
        <source>Invalid file</source>
        <translation>Ungültige Datei</translation>
    </message>
    <message>
        <source>The selected file is not an executable.</source>
        <translation>Die ausgewählte Datei ist kein Programm.</translation>
    </message>
    <message>
        <source>Restart required</source>
        <translation>Neustart erforderlich</translation>
    </message>
    <message>
        <source>Restart hashcat-gui to apply the new language.</source>
        <translation>Starten Sie hashcat-gui neu, um die neue Sprache zu übernehmen.</translation>
    </message>
</context>
<context>
    <name>StateFile</name>
    <message>
        <source>The file is not valid JSON: %1</source>
        <translation>Die Datei ist kein gültiges JSON: %1</translation>
    </message>
    <message>
        <source>The file does not contain a JSON object.</source>
        <translation>Die Datei enthält kein JSON-Objekt.</translation>
    </message>
    <message>
        <source>The file does not contain any sections.</source>
        <translation>Die Datei enthält keine Abschnitte.</translation>
    </message>
    <message>
        <source>Could not open “%1” for writing: %2</source>
        <translation>„%1“ konnte nicht zum Schreiben geöffnet werden: %2</translation>
    </message>
    <message>
        <source>Could not write to “%1”: %2</source>
        <translation>Auf „%1“ konnte nicht geschrieben werden: %2</translation>
    </message>
    <message>
        <source>Could not open “%1” for reading: %2</source>
        <translation>„%1“ konnte nicht zum Lesen geöffnet werden: %2</translation>
    </message>
    <message>
        <source>The file does not contain a “%1” section.</source>
        <translation>Die Datei enthält keinen Abschnitt „%1“.</translation>
    </message>
    <message>
        <source>The file does not contain a “%1” section; it holds %2.</source>
        <translation>Die Datei enthält keinen Abschnitt „%1“; sie enthält %2.</translation>
    </message>
    <message>
        <source>The “%1” section does not contain any values.</source>
        <translation>Der Abschnitt „%1“ enthält keine Werte.</translation>
    </message>
    <message>
        <source> and </source>
        <translation> und </translation>
    </message>
    <message>
        <source> and %1 more</source>
        <translation> und %1 weitere</translation>
    </message>
    <message>
        <source>The file names values this version does not know, so they were not applied: %1.</source>
        <translation>Die Datei nennt Werte, die diese Version nicht kennt, daher wurden sie nicht angewendet: %1.</translation>
    </message>
    <message>
        <source>The file holds values of the wrong type, so they were not applied: %1.</source>
        <translation>Die Datei enthält Werte des falschen Typs, daher wurden sie nicht angewendet: %1.</translation>
    </message>
</context>
<context>
    <name>UpdateChecker</name>
    <message>
        <source>GitHub answered with HTTP status %1: %2</source>
        <translation>GitHub hat mit dem HTTP-Status %1 geantwortet: %2</translation>
    </message>
    <message>
        <source>GitHub answered with HTTP status %1.</source>
        <translation>GitHub hat mit dem HTTP-Status %1 geantwortet.</translation>
    </message>
    <message>
        <source>The release information is not valid JSON.</source>
        <translation>Die Release-Informationen sind kein gültiges JSON.</translation>
    </message>
    <message>
        <source>The release information does not name a version.</source>
        <translation>Die Release-Informationen nennen keine Version.</translation>
    </message>
    <message>
        <source>The release version %1 is not a valid version number.</source>
        <translation>Die Release-Version %1 ist keine gültige Versionsnummer.</translation>
    </message>
    <message>
        <source>The installed version %1 is not a valid version number.</source>
        <translation>Die installierte Version %1 ist keine gültige Versionsnummer.</translation>
    </message>
</context>
</TS>
