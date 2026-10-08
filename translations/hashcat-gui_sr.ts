<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="sr">
<context>
    <name>AboutDialog</name>
    <message>
        <location filename="../src/aboutdialog.ui" line="32"/>
        <source>About</source>
        <translation>O programu</translation>
    </message>
    <message>
        <location filename="../src/aboutdialog.ui" line="45"/>
        <source>Version information</source>
        <translation>Informacije o verziji</translation>
    </message>
    <message>
        <location filename="../src/aboutdialog.ui" line="90"/>
        <location filename="../src/aboutdialog.ui" line="152"/>
        <source>[unknown]</source>
        <translation>[nepoznato]</translation>
    </message>
    <message>
        <location filename="../src/aboutdialog.ui" line="196"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../src/aboutdialog.cpp" line="48"/>
        <source>Error: %1</source>
        <translation>Greška: %1</translation>
    </message>
</context>
<context>
    <name>HashcatInfoParser</name>
    <message>
        <location filename="../src/hashcatinfoparser.cpp" line="31"/>
        <source>Invalid JSON returned from hashcat.</source>
        <translation>hashcat je vratio nevažeći JSON.</translation>
    </message>
    <message>
        <location filename="../src/hashcatinfoparser.cpp" line="53"/>
        <source>hashcat did not report any hash types.</source>
        <translation>hashcat nije prijavio nijednu vrstu hash-a.</translation>
    </message>
</context>
<context>
    <name>HelperUtils</name>
    <message>
        <location filename="../src/helperutils.cpp" line="85"/>
        <source>The hashcat path is not configured.</source>
        <translation>Putanja do hashcat-a nije konfigurisana.</translation>
    </message>
    <message>
        <location filename="../src/helperutils.cpp" line="99"/>
        <source>Failed to start hashcat
%1</source>
        <translation>Pokretanje hashcat-a nije uspelo
%1</translation>
    </message>
    <message>
        <location filename="../src/helperutils.cpp" line="108"/>
        <source>hashcat timed out
%1</source>
        <translation>hashcat je dostigao vremenski limit
%1</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../src/mainwindow.ui" line="76"/>
        <source>Hash file or hash:</source>
        <translation>Datoteka sa hashom ili hash:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="93"/>
        <location filename="../src/mainwindow.ui" line="352"/>
        <location filename="../src/mainwindow.ui" line="395"/>
        <location filename="../src/mainwindow.ui" line="438"/>
        <location filename="../src/mainwindow.ui" line="861"/>
        <source>Open...</source>
        <translation>Otvori...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="104"/>
        <source>Remove hash from hash list once it is cracked</source>
        <translation>Ukloni hash sa liste hash-ova kada bude provaljen</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="111"/>
        <source>Ignore username in hashfile</source>
        <translation>Zanemari korisničko ime u datoteci sa hashom</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="120"/>
        <source>Word lists</source>
        <translation>Liste reči</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="133"/>
        <source>Move item up</source>
        <translation>Pomeri stavku gore</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="152"/>
        <source>Move item down</source>
        <translation>Pomeri stavku dole</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="176"/>
        <source>Add files...</source>
        <translation>Dodaj datoteke...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="192"/>
        <source>Remove</source>
        <translation>Ukloni</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="234"/>
        <source>Mode:</source>
        <translation>Režim:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="273"/>
        <source>Hash type:</source>
        <translation>Vrsta hash-a:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="286"/>
        <source>Configure path to hashcat binary in Settings to get available hash types</source>
        <translation>Podesite putanju do hashcat programa u Podešavanjima da biste dobili dostupne vrste hash-ova</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="299"/>
        <source>Rules</source>
        <translation>Pravila</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="305"/>
        <source>Use rules files:</source>
        <translation>Koristi datoteke sa pravilima:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="320"/>
        <location filename="../src/mainwindow.ui" line="363"/>
        <location filename="../src/mainwindow.ui" line="406"/>
        <source>Rules file:</source>
        <translation>Datoteka sa pravilima:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="449"/>
        <source>Generate rules:</source>
        <translation>Generiši pravila:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="519"/>
        <source>Assume charset is given in hex</source>
        <translation>Pretpostavi da je skup znakata zadan heksadecimalno</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="526"/>
        <source>Assume salt is given in hex</source>
        <translation>Pretpostavi da je salt zadan heksadecimalno</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="533"/>
        <source>Mask</source>
        <translation>Maska</translation>
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
        <translation>Ugrađeni skupovi znakata:

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
        <translation>Prilagođeni skupovi znakata</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="575"/>
        <source>Charset 1:</source>
        <translation>Skup znakata 1:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="617"/>
        <source>Charset 2:</source>
        <translation>Skup znakata 2:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="659"/>
        <source>Charset 3:</source>
        <translation>Skup znakata 3:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="701"/>
        <source>Charset 4:</source>
        <translation>Skup znakata 4:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="744"/>
        <source>Resources</source>
        <translation>Resursi</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="754"/>
        <source>CPU affinity:</source>
        <translation>CPU afinitet:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="761"/>
        <source>Backend devices:</source>
        <translation>Backend uređaji:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="768"/>
        <source>Segment size:</source>
        <translation>Veličina segmenta:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="779"/>
        <source>locks to CPU devices, seperate with comma</source>
        <translation>vezuje za CPU uređaje, odvojene zarezom</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="786"/>
        <source>devices to use, seperate with comma</source>
        <translation>uređaji za korišćenje, odvojeni zarezom</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="802"/>
        <source> MB</source>
        <translation> MB</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="825"/>
        <source>Output</source>
        <translation>Izlaz</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="833"/>
        <source>Write recovered hashes to file:</source>
        <translation>Upiši pronađene hasheve u datoteku:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="845"/>
        <source>Built-in variables:

  &lt;hash&gt; = current hash filename or hash
  &lt;unixtime&gt; = current unix timestamp</source>
        <translation>Ugrađene promenljive:

  &lt;hash&gt; = trenutni naziv datoteke sa hashom ili hash
  &lt;unixtime&gt; = trenutni unix vremenski pečat</translation>
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
        <translation>Format izlazne datoteke za korišćenje, odvojeni zarezom.
Podrazumevano: 1,2

  1 = hash[:salt]
  2 = plain
  3 = hex_plain
  4 = crack_pos
  5 = apsolutni vremenski pečat
  6 = relativni vremenski pečat</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="927"/>
        <source>Start</source>
        <translation>Pokreni</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="934"/>
        <source>Copy command to clipboard</source>
        <translation>Kopiraj naredbu u privremenu memoriju</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="977"/>
        <source>Advanced</source>
        <translation>Napredno</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1001"/>
        <source>General</source>
        <translation>Opšte</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1013"/>
        <source>Enable optimized kernels (limits password length)</source>
        <translation>Omogući optimizovane kernele (ograničava dužinu lozinke)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1020"/>
        <source>Return expected speed of the attack, then quit</source>
        <translation>Ispiši očekivanu brzinu napada i izađi</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1029"/>
        <source>Override workload profile</source>
        <translation>Zameni profil opterećenja</translation>
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
        <translation> # | Performansa   |Trajanje | Potrošnja energije| Uticaj na desktop
 ===+=============+=========+===================+=========
  1 | Nisko         |    2 ms | Nisko             | Minimalno
  2 | Podrazumevano |   12 ms | Ekonomično        | Primetno
  3 | Visoko        |   96 ms | Visoko            | Ne reaguje
  4 | Noćna mora    |  480 ms | Ludo              | Bez desktopa

</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1125"/>
        <source>Help</source>
        <translation>Pomoć</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1132"/>
        <source>Tools</source>
        <translation>Alati</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1138"/>
        <source>File</source>
        <translation>Datoteka</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1152"/>
        <source>About</source>
        <translation>O programu</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1157"/>
        <source>Reset fields</source>
        <translation>Poništi polja</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1162"/>
        <source>Quit</source>
        <translation>Izađi</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1167"/>
        <source>About Qt</source>
        <translation>O Qt-u</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1172"/>
        <source>Settings</source>
        <translation>Podešavanja</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1177"/>
        <source>Export Profile</source>
        <translation>Izvezi profil</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1182"/>
        <source>Import Profile</source>
        <translation>Uvezi profil</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="113"/>
        <source>Save Profile</source>
        <translation>Sačuvaj profil</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="115"/>
        <location filename="../src/mainwindow.cpp" line="135"/>
        <source>JSON Files (*.json)</source>
        <translation>JSON datoteke (*.json)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="122"/>
        <source>Saved</source>
        <translation>Sačuvano</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="122"/>
        <source>Profile saved to %1.</source>
        <translation>Profil je sačuvan u %1.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="124"/>
        <location filename="../src/mainwindow.cpp" line="256"/>
        <source>Save failed</source>
        <translation>Čuvanje nije uspelo</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="133"/>
        <source>Load Profile</source>
        <translation>Učitaj profil</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="142"/>
        <source>Loaded</source>
        <translation>Učitano</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="142"/>
        <source>Profile loaded from %1.</source>
        <translation>Profil je učitan iz %1.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="144"/>
        <location filename="../src/mainwindow.cpp" line="243"/>
        <source>Load failed</source>
        <translation>Učitanje nije uspelo</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="268"/>
        <source>Straight</source>
        <translation>Rečnik</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="269"/>
        <source>Combination</source>
        <translation>Kombinacija</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="270"/>
        <source>Brute-force</source>
        <translation>Brute-Force</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="271"/>
        <source>Hybrid Wordlist + Mask</source>
        <translation>Hibrid: lista reči + maska</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="272"/>
        <source>Hybrid Mask + Wordlist</source>
        <translation>Hibrid: maska + lista reči</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="273"/>
        <source>Association</source>
        <translation>Asocijacija</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="285"/>
        <source>Updating...</source>
        <translation>Ažuriranje...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="300"/>
        <source>Failed to obtain supported hash types.
Error: %1</source>
        <translation>Preuzimanje podržanih vrsta hash-ova nije uspelo.
Greška: %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="322"/>
        <source>hashcat error</source>
        <translation>hashcat greška</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="354"/>
        <source>Open Hash File</source>
        <translation>Otvori datoteku sa hashom</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="362"/>
        <source>Save Output File</source>
        <translation>Sačuvaj izlaznu datoteku</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="379"/>
        <source>Add Wordlists</source>
        <translation>Dodaj liste reči</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="461"/>
        <location filename="../src/mainwindow.cpp" line="469"/>
        <location filename="../src/mainwindow.cpp" line="477"/>
        <source>Open Rules File</source>
        <translation>Otvori datoteku sa pravilima</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="576"/>
        <source>Launch failed</source>
        <translation>Pokretanje nije uspelo</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="577"/>
        <source>Could not start %1: %2</source>
        <translation>Nije bilo moguće pokrenuti %1: %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="593"/>
        <source>Please choose a hash file.</source>
        <translation>Izaberite datoteku sa hashom.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="599"/>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to configure the path to the hashcat executable.</source>
        <translation>Idite na &lt;b&gt;%1 → %2&lt;/b&gt; da podesite putanju do hashcat programa.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="609"/>
        <source>The configured hashcat executable does not exist:
%1
Navigate to %2 → %3 to change it.</source>
        <translation>Konfigurisani hashcat program ne postoji:
%1
Idite na %2 → %3 da to promenite.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="617"/>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to select the terminal used for launching.</source>
        <translation>Idite na &lt;b&gt;%1 → %2&lt;/b&gt; da izaberete terminal koji se koristi za pokretanje.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="622"/>
        <source>No supported terminal was found on this system. hashcat-gui needs one to show the hashcat output.</source>
        <translation>Na ovom sistemu nije pronađen nijedan podržani terminal. hashcat-gui mu je potreban da bi prikazao izlaz hashcat-a.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="628"/>
        <source>The configured terminal &quot;%1&quot; is not available. Available terminals: %2
Navigate to %3 → %4 to change it.</source>
        <translation>Konfigurisani terminal &quot;%1&quot; nije dostupan. Dostupni terminali: %2
Idite na %3 → %4 da to promenite.</translation>
    </message>
</context>
<context>
    <name>SettingsDialog</name>
    <message>
        <location filename="../src/settingsdialog.ui" line="32"/>
        <source>Settings</source>
        <translation>Podešavanja</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.ui" line="57"/>
        <source>Hashcat executable</source>
        <translation>hashcat program</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.ui" line="69"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Download from &lt;a href=&quot;https://hashcat.net&quot;&gt;&lt;span style=&quot; text-decoration: underline; color:#27bf73;&quot;&gt;https://hashcat.net&lt;/span&gt;&lt;/a&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Preuzmi sa &lt;a href=&quot;https://hashcat.net&quot;&gt;&lt;span style=&quot; text-decoration: underline; color:#27bf73;&quot;&gt;https://hashcat.net&lt;/span&gt;&lt;/a&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.ui" line="88"/>
        <source>Select</source>
        <translation>Izaberi</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.ui" line="109"/>
        <source>Terminal</source>
        <translation>Terminal</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.ui" line="149"/>
        <source>General</source>
        <translation>Opšte</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.ui" line="155"/>
        <source>Use short parameters</source>
        <translation>Koristi kratke parametre</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.ui" line="171"/>
        <source>Language</source>
        <translation>Jezik</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.ui" line="205"/>
        <source>Save</source>
        <translation>Sačuvaj</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.ui" line="212"/>
        <source>Cancel</source>
        <translation>Otkaži</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="51"/>
        <source>System default</source>
        <translation>Sistemski podrazumevano</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="79"/>
        <source>Select the hashcat executable</source>
        <translation>Izaberite hashcat program</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="81"/>
        <source>Executable Files (*.exe *.bin);;All Files (*)</source>
        <translation>Izvršne datoteke (*.exe *.bin);;Sve datoteke (*)</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="87"/>
        <source>Invalid file</source>
        <translation>Nevažeća datoteka</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="88"/>
        <source>The selected file is not an executable.</source>
        <translation>Izabrana datoteka nije izvršna datoteka.</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="110"/>
        <source>Restart required</source>
        <translation>Potreban restart</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="111"/>
        <source>Restart hashcat-gui to apply the new language.</source>
        <translation>Restartujte hashcat-gui da biste primenili novi jezik.</translation>
    </message>
</context>
<context>
    <name>WidgetStateSerializer</name>
    <message>
        <location filename="../src/widgetstateserializer.cpp" line="167"/>
        <source>Could not open %1 for writing: %2</source>
        <translation>Nije bilo moguće otvoriti %1 za upis: %2</translation>
    </message>
    <message>
        <location filename="../src/widgetstateserializer.cpp" line="173"/>
        <source>Could not write to %1: %2</source>
        <translation>Nije bilo moguće upisati u %1: %2</translation>
    </message>
    <message>
        <location filename="../src/widgetstateserializer.cpp" line="191"/>
        <source>Could not open %1 for reading: %2</source>
        <translation>Nije bilo moguće otvoriti %1 za čitanje: %2</translation>
    </message>
    <message>
        <location filename="../src/widgetstateserializer.cpp" line="199"/>
        <source>The file is not a valid JSON file: %1</source>
        <translation>Datoteka nije važeći JSON fajl: %1</translation>
    </message>
    <message>
        <location filename="../src/widgetstateserializer.cpp" line="205"/>
        <source>The file does not contain a profile for &quot;%1&quot;.</source>
        <translation>Datoteka ne sadrži profil za &quot;%1&quot;.</translation>
    </message>
</context>
</TS>
