<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="de">
<context>
    <name>AboutDialog</name>
    <message>
        <location filename="../src/aboutdialog.ui" line="32"/>
        <source>About</source>
        <translation>Über</translation>
    </message>
    <message>
        <location filename="../src/aboutdialog.ui" line="45"/>
        <source>Version information</source>
        <translation>Versionsinformationen</translation>
    </message>
    <message>
        <location filename="../src/aboutdialog.ui" line="90"/>
        <location filename="../src/aboutdialog.ui" line="152"/>
        <source>[unknown]</source>
        <translation>[unbekannt]</translation>
    </message>
    <message>
        <location filename="../src/aboutdialog.ui" line="196"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../src/aboutdialog.cpp" line="48"/>
        <source>Error: %1</source>
        <translation>Fehler: %1</translation>
    </message>
</context>
<context>
    <name>HashcatInfoParser</name>
    <message>
        <location filename="../src/hashcatinfoparser.cpp" line="31"/>
        <source>Invalid JSON returned from hashcat.</source>
        <translation>hashcat hat ungültiges JSON zurückgegeben.</translation>
    </message>
    <message>
        <location filename="../src/hashcatinfoparser.cpp" line="53"/>
        <source>hashcat did not report any hash types.</source>
        <translation>hashcat hat keine Hash-Typen gemeldet.</translation>
    </message>
</context>
<context>
    <name>HelperUtils</name>
    <message>
        <location filename="../src/helperutils.cpp" line="85"/>
        <source>The hashcat path is not configured.</source>
        <translation>Der Pfad zu hashcat ist nicht konfiguriert.</translation>
    </message>
    <message>
        <location filename="../src/helperutils.cpp" line="99"/>
        <source>Failed to start hashcat
</source>
        <translation>hashcat konnte nicht gestartet werden
</translation>
    </message>
    <message>
        <location filename="../src/helperutils.cpp" line="108"/>
        <source>hashcat timed out
</source>
        <translation>hashcat hat das Zeitlimit erreicht
</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../src/mainwindow.ui" line="76"/>
        <source>Hash file or hash:</source>
        <translation>Hash-Datei oder Hash:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="93"/>
        <location filename="../src/mainwindow.ui" line="352"/>
        <location filename="../src/mainwindow.ui" line="395"/>
        <location filename="../src/mainwindow.ui" line="438"/>
        <location filename="../src/mainwindow.ui" line="861"/>
        <source>Open...</source>
        <translation>Öffnen...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="104"/>
        <source>Remove hash from hash list once it is cracked</source>
        <translation>Hash nach dem Knacken aus der Liste entfernen</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="111"/>
        <source>Ignore username in hashfile</source>
        <translation>Benutzernamen in der Hash-Datei ignorieren</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="120"/>
        <source>Word lists</source>
        <translation>Wortlisten</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="133"/>
        <source>Move item up</source>
        <translation>Eintrag nach oben verschieben</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="152"/>
        <source>Move item down</source>
        <translation>Eintrag nach unten verschieben</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="176"/>
        <source>Add files...</source>
        <translation>Dateien hinzufügen...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="192"/>
        <source>Remove</source>
        <translation>Entfernen</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="234"/>
        <source>Mode:</source>
        <translation>Modus:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="273"/>
        <source>Hash type:</source>
        <translation>Hash-Typ:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="286"/>
        <source>Configure path to hashcat binary in Settings to get available hash types</source>
        <translation>Pfad zum hashcat-Programm in den Einstellungen festlegen, um verfügbare Hash-Typen zu erhalten</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="299"/>
        <source>Rules</source>
        <translation>Regeln</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="305"/>
        <source>Use rules files:</source>
        <translation>Regeldateien verwenden:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="320"/>
        <location filename="../src/mainwindow.ui" line="363"/>
        <location filename="../src/mainwindow.ui" line="406"/>
        <source>Rules file:</source>
        <translation>Regeldatei:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="449"/>
        <source>Generate rules:</source>
        <translation>Regeln erzeugen:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="519"/>
        <source>Assume charset is given in hex</source>
        <translation>Zeichensatz als Hexadezimal annehmen</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="526"/>
        <source>Assume salt is given in hex</source>
        <translation>Salt als Hexadezimal annehmen</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="533"/>
        <source>Mask</source>
        <translation>Maske</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="542"/>
        <location filename="../src/mainwindow.ui" line="594"/>
        <location filename="../src/mainwindow.ui" line="636"/>
        <location filename="../src/mainwindow.ui" line="678"/>
        <location filename="../src/mainwindow.ui" line="717"/>
        <source>Built-in charsets:

  ?l = abcdefghijklmnopqrstuvwxyz [a-z]
  ?u = ABCDEFGHIJKLMNOPQRSTUVWXYZ [A-Z]
  ?d = 0123456789 [0-9]
  ?h = 0123456789abcdef [0-9a-f]
  ?H = 0123456789ABCDEF [0-9A-F]
  ?s =  !&quot;#$%&amp;&apos;()*+,-./:;&lt;=&gt;?@[]^_`{|}~
  ?a = ?l?u?d?s
  ?b = 0x00 - 0xff</source>
        <translation>Eingebaute Zeichensätze:

  ?l = abcdefghijklmnopqrstuvwxyz [a-z]
  ?u = ABCDEFGHIJKLMNOPQRSTUVWXYZ [A-Z]
  ?d = 0123456789 [0-9]
  ?h = 0123456789abcdef [0-9a-f]
  ?H = 0123456789ABCDEF [0-9A-F]
  ?s =  !&quot;#$%&amp;&apos;()*+,-./:;&lt;=&gt;?@[]^_`{|}~
  ?a = ?l?u?d?s
  ?b = 0x00 - 0xff</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="567"/>
        <source>Custom charsets</source>
        <translation>Eigene Zeichensätze</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="575"/>
        <source>Charset 1:</source>
        <translation>Zeichensatz 1:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="617"/>
        <source>Charset 2:</source>
        <translation>Zeichensatz 2:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="659"/>
        <source>Charset 3:</source>
        <translation>Zeichensatz 3:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="701"/>
        <source>Charset 4:</source>
        <translation>Zeichensatz 4:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="744"/>
        <source>Resources</source>
        <translation>Ressourcen</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="754"/>
        <source>CPU affinity:</source>
        <translation>CPU-Affinität:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="761"/>
        <source>Backend devices:</source>
        <translation>Backend-Geräte:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="768"/>
        <source>Segment size:</source>
        <translation>Segmentgröße:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="779"/>
        <source>locks to CPU devices, seperate with comma</source>
        <translation>bindet an CPU-Geräte, mit Komma trennen</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="786"/>
        <source>devices to use, seperate with comma</source>
        <translation>zu verwendende Geräte, mit Komma trennen</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="802"/>
        <source> MB</source>
        <translation> MB</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="825"/>
        <source>Output</source>
        <translation>Ausgabe</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="833"/>
        <source>Write recovered hashes to file:</source>
        <translation>Gefundene Hashes in Datei schreiben:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="845"/>
        <source>Built-in variables:

  &lt;hash&gt; = current hash filename or hash
  &lt;unixtime&gt; = current unix timestamp</source>
        <translation>Eingebaute Variablen:

  &lt;hash&gt; = aktueller Hash-Dateiname oder Hash
  &lt;unixtime&gt; = aktueller Unix-Zeitstempel</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="875"/>
        <source>Format:</source>
        <translation>Format:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="885"/>
        <source>Outfile format to use, separated with commas.
default: 1,2

  1 = hash[:salt]
  2 = plain
  3 = hex_plain
  4 = crack_pos
  5 = timestamp absolute
  6 = timestamp relative</source>
        <translation>Zu verwendendes Ausgabeformat, mit Komma getrennt.
Standard: 1,2

  1 = hash[:salt]
  2 = plain
  3 = hex_plain
  4 = crack_pos
  5 = timestamp absolute
  6 = timestamp relative</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="927"/>
        <source>Start</source>
        <translation>Start</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="934"/>
        <source>Copy command to clipboard</source>
        <translation>Befehl in die Zwischenablage kopieren</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="977"/>
        <source>Advanced</source>
        <translation>Erweitert</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1001"/>
        <source>General</source>
        <translation>Allgemein</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1013"/>
        <source>Enable optimized kernels (limits password length)</source>
        <translation>Optimierte Kerne verwenden (begrenzt die Passwortlänge)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1020"/>
        <source>Return expected speed of the attack, then quit</source>
        <translation>Erwartete Angriffsgeschwindigkeit ausgeben und beenden</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1029"/>
        <source>Override workload profile</source>
        <translation>Workload-Profil überschreiben</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1039"/>
        <source> # | Performance | Runtime | Power Consumption | Desktop Impact
 ===+=============+=========+===================+=========
  1 | Low               |   2 ms    | Low               | Minimal
  2 | Default         |  12 ms   | Economic     | Noticeable
  3 | High              |  96 ms   | High              | Unresponsive
  4 | Nightmare   | 480 ms  | Insane          | Headless

</source>
        <translation> # | Leistung | Laufzeit | Stromverbrauch | Einfluss auf den Desktop
 ===+=============+=========+===================+=========
  1 | Niedrig            |   2 ms   | Niedrig           | Minimal
  2 | Standard        |  12 ms   | Sparsam       | Spürbar
  3 | Hoch               |  96 ms   | Hoch                | Reagiert nicht
  4 | Alptraum     | 480 ms  | Wahnsinn     | Ohne Desktop

</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1125"/>
        <source>Help</source>
        <translation>Hilfe</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1132"/>
        <source>Tools</source>
        <translation>Werkzeuge</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1138"/>
        <source>File</source>
        <translation>Datei</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1152"/>
        <source>About</source>
        <translation>Über</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1157"/>
        <source>Reset fields</source>
        <translation>Felder zurücksetzen</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1162"/>
        <source>Quit</source>
        <translation>Beenden</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1167"/>
        <source>About Qt</source>
        <translation>Über Qt</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1172"/>
        <source>Settings</source>
        <translation>Einstellungen</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1177"/>
        <source>Export Profile</source>
        <translation>Profil exportieren</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1182"/>
        <source>Import Profile</source>
        <translation>Profil importieren</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="113"/>
        <source>Save Profile</source>
        <translation>Profil speichern</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="115"/>
        <location filename="../src/mainwindow.cpp" line="135"/>
        <source>JSON Files (*.json)</source>
        <translation>JSON-Dateien (*.json)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="122"/>
        <source>Saved</source>
        <translation>Gespeichert</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="122"/>
        <source>Profile saved to %1.</source>
        <translation>Profil wurde in %1 gespeichert.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="124"/>
        <location filename="../src/mainwindow.cpp" line="256"/>
        <source>Save failed</source>
        <translation>Speichern fehlgeschlagen</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="133"/>
        <source>Load Profile</source>
        <translation>Profil laden</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="142"/>
        <source>Loaded</source>
        <translation>Geladen</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="142"/>
        <source>Profile loaded from %1.</source>
        <translation>Profil wurde aus %1 geladen.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="144"/>
        <location filename="../src/mainwindow.cpp" line="243"/>
        <source>Load failed</source>
        <translation>Laden fehlgeschlagen</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="268"/>
        <source>Straight</source>
        <translation>Wörterbuch</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="269"/>
        <source>Combination</source>
        <translation>Kombination</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="270"/>
        <source>Brute-force</source>
        <translation>Brute-Force</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="271"/>
        <source>Hybrid Wordlist + Mask</source>
        <translation>Hybrid: Wortliste + Maske</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="272"/>
        <source>Hybrid Mask + Wordlist</source>
        <translation>Hybrid: Maske + Wortliste</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="273"/>
        <source>Association</source>
        <translation>Assoziation</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="285"/>
        <source>Updating...</source>
        <translation>Wird aktualisiert...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="300"/>
        <source>Failed to obtain supported hash types.
Error: %1</source>
        <translation>Die unterstützten Hash-Typen konnten nicht ermittelt werden.
Fehler: %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="322"/>
        <source>hashcat error</source>
        <translation>hashcat-Fehler</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="354"/>
        <source>Open Hash File</source>
        <translation>Hash-Datei öffnen</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="362"/>
        <source>Save Output File</source>
        <translation>Ausgabedatei speichern</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="379"/>
        <source>Add Wordlists</source>
        <translation>Wortlisten hinzufügen</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="461"/>
        <location filename="../src/mainwindow.cpp" line="469"/>
        <location filename="../src/mainwindow.cpp" line="477"/>
        <source>Open Rules File</source>
        <translation>Regeldatei öffnen</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="576"/>
        <source>Launch failed</source>
        <translation>Start fehlgeschlagen</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="577"/>
        <source>Could not start %1: %2</source>
        <translation>%1 konnte nicht gestartet werden: %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="593"/>
        <source>Please choose a hash file.</source>
        <translation>Bitte wählen Sie eine Hash-Datei aus.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="599"/>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to configure the path to the hashcat executable.</source>
        <translation>Gehen Sie zu &lt;b&gt;%1 → %2&lt;/b&gt;, um den Pfad zum hashcat-Programm festzulegen.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="607"/>
        <source>The configured hashcat executable does not exist:
%1
Navigate to %2 → %3 to change it.</source>
        <translation>Das konfigurierte hashcat-Programm existiert nicht:
%1
Gehen Sie zu %2 → %3, um es zu ändern.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="615"/>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to select the terminal used for launching.</source>
        <translation>Gehen Sie zu &lt;b&gt;%1 → %2&lt;/b&gt;, um das zum Starten verwendete Terminal auszuwählen.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="620"/>
        <source>No supported terminal was found on this system. hashcat-gui needs one to show the hashcat output.</source>
        <translation>Auf diesem System wurde kein unterstütztes Terminal gefunden. hashcat-gui benötigt eines, um die Ausgabe von hashcat anzuzeigen.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="625"/>
        <source>The configured terminal &quot;%1&quot; is not available. Available terminals: %2
Navigate to %3 → %4 to change it.</source>
        <translation>Das konfigurierte Terminal &quot;%1&quot; ist nicht verfügbar. Verfügbare Terminals: %2
Gehen Sie zu %3 → %4, um es zu ändern.</translation>
    </message>
</context>
<context>
    <name>SettingsDialog</name>
    <message>
        <location filename="../src/settingsdialog.ui" line="32"/>
        <source>Settings</source>
        <translation>Einstellungen</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.ui" line="57"/>
        <source>Hashcat executable</source>
        <translation>hashcat-Programm</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.ui" line="69"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Download from &lt;a href=&quot;https://hashcat.net&quot;&gt;&lt;span style=&quot; text-decoration: underline; color:#27bf73;&quot;&gt;https://hashcat.net&lt;/span&gt;&lt;/a&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Download von &lt;a href=&quot;https://hashcat.net&quot;&gt;&lt;span style=&quot; text-decoration: underline; color:#27bf73;&quot;&gt;https://hashcat.net&lt;/span&gt;&lt;/a&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.ui" line="88"/>
        <source>Select</source>
        <translation>Auswählen</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.ui" line="109"/>
        <source>Terminal</source>
        <translation>Terminal</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.ui" line="149"/>
        <source>General</source>
        <translation>Allgemein</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.ui" line="155"/>
        <source>Use short parameters</source>
        <translation>Kurze Parameter verwenden</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.ui" line="180"/>
        <source>Save</source>
        <translation>Speichern</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.ui" line="187"/>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="51"/>
        <source>Select the hashcat executable</source>
        <translation>hashcat-Programm auswählen</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="53"/>
        <source>Executable Files (*.exe *.bin);;All Files (*)</source>
        <translation>Programmdateien (*.exe *.bin);;Alle Dateien (*)</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="59"/>
        <source>Invalid file</source>
        <translation>Ungültige Datei</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="60"/>
        <source>The selected file is not an executable.</source>
        <translation>Die ausgewählte Datei ist kein Programm.</translation>
    </message>
</context>
<context>
    <name>WidgetStateSerializer</name>
    <message>
        <location filename="../src/widgetstateserializer.cpp" line="167"/>
        <source>Could not open %1 for writing: %2</source>
        <translation>%1 konnte zum Schreiben nicht geöffnet werden: %2</translation>
    </message>
    <message>
        <location filename="../src/widgetstateserializer.cpp" line="173"/>
        <source>Could not write to %1: %2</source>
        <translation>In %1 konnte nicht geschrieben werden: %2</translation>
    </message>
    <message>
        <location filename="../src/widgetstateserializer.cpp" line="191"/>
        <source>Could not open %1 for reading: %2</source>
        <translation>%1 konnte zum Lesen nicht geöffnet werden: %2</translation>
    </message>
    <message>
        <location filename="../src/widgetstateserializer.cpp" line="199"/>
        <source>The file is not a valid JSON file: %1</source>
        <translation>Die Datei ist keine gültige JSON-Datei: %1</translation>
    </message>
    <message>
        <location filename="../src/widgetstateserializer.cpp" line="205"/>
        <source>The file does not contain a profile for &quot;%1&quot;.</source>
        <translation>Die Datei enthält kein Profil für &quot;%1&quot;.</translation>
    </message>
</context>
</TS>
