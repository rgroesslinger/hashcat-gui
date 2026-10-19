<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="it">
<context>
    <name>AboutDialog</name>
    <message>
        <location filename="../src/aboutdialog.ui" line="14"/>
        <source>About</source>
        <translation>Informazioni</translation>
    </message>
    <message>
        <location filename="../src/aboutdialog.ui" line="26"/>
        <source>Version information</source>
        <translation>Informazioni sulla versione</translation>
    </message>
    <message>
        <location filename="../src/aboutdialog.ui" line="77"/>
        <location filename="../src/aboutdialog.ui" line="129"/>
        <source>[unknown]</source>
        <translation>[sconosciuto]</translation>
    </message>
    <message>
        <location filename="../src/aboutdialog.ui" line="160"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <location filename="../src/aboutdialog.cpp" line="47"/>
        <source>Error: %1</source>
        <translation>Errore: %1</translation>
    </message>
</context>
<context>
    <name>HashcatInfoParser</name>
    <message>
        <location filename="../src/hashcatinfoparser.cpp" line="31"/>
        <source>Invalid JSON returned from hashcat.</source>
        <translation>hashcat ha restituito un JSON non valido.</translation>
    </message>
    <message>
        <location filename="../src/hashcatinfoparser.cpp" line="53"/>
        <source>hashcat did not report any hash types.</source>
        <translation>hashcat non ha riportato alcun tipo di hash.</translation>
    </message>
</context>
<context>
    <name>HelperUtils</name>
    <message>
        <location filename="../src/helperutils.cpp" line="81"/>
        <source>The hashcat path is not configured.</source>
        <translation>Il percorso di hashcat non è configurato.</translation>
    </message>
    <message>
        <location filename="../src/helperutils.cpp" line="95"/>
        <source>Failed to start hashcat
%1</source>
        <translation>Impossibile avviare hashcat
%1</translation>
    </message>
    <message>
        <location filename="../src/helperutils.cpp" line="104"/>
        <source>hashcat timed out
%1</source>
        <translation>Timeout di hashcat
%1</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../src/mainwindow.ui" line="72"/>
        <source>Hash file or hash:</source>
        <translation>File di hash o hash:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="89"/>
        <location filename="../src/mainwindow.ui" line="348"/>
        <location filename="../src/mainwindow.ui" line="391"/>
        <location filename="../src/mainwindow.ui" line="434"/>
        <location filename="../src/mainwindow.ui" line="857"/>
        <source>Open...</source>
        <translation>Apri...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="100"/>
        <source>Remove hash from hash list once it is cracked</source>
        <translation>Rimuovi l’hash dall’elenco una volta decifrato</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="107"/>
        <source>Ignore username in hashfile</source>
        <translation>Ignora il nome utente nel file di hash</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="116"/>
        <source>Word lists</source>
        <translation>Wordlist</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="129"/>
        <source>Move item up</source>
        <translation>Sposta in alto</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="148"/>
        <source>Move item down</source>
        <translation>Sposta in basso</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="172"/>
        <source>Add files...</source>
        <translation>Aggiungi file...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="188"/>
        <source>Remove</source>
        <translation>Rimuovi</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="230"/>
        <source>Mode:</source>
        <translation>Modalità:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="269"/>
        <source>Hash type:</source>
        <translation>Tipo di hash:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="282"/>
        <source>Configure path to hashcat binary in Settings to get available hash types</source>
        <translation>Configura il percorso dell’eseguibile di hashcat nelle Impostazioni per ottenere i tipi di hash disponibili</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="295"/>
        <source>Rules</source>
        <translation>Regole</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="301"/>
        <source>Use rules files:</source>
        <translation>Usa file di regole:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="316"/>
        <location filename="../src/mainwindow.ui" line="359"/>
        <location filename="../src/mainwindow.ui" line="402"/>
        <source>Rules file:</source>
        <translation>File di regole:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="445"/>
        <source>Generate rules:</source>
        <translation>Genera regole:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="515"/>
        <source>Assume charset is given in hex</source>
        <translation>Considera il set di caratteri come esadecimale</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="522"/>
        <source>Assume salt is given in hex</source>
        <translation>Considera il salt come esadecimale</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="529"/>
        <source>Mask</source>
        <translation>Maschera</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="538"/>
        <location filename="../src/mainwindow.ui" line="590"/>
        <location filename="../src/mainwindow.ui" line="632"/>
        <location filename="../src/mainwindow.ui" line="674"/>
        <location filename="../src/mainwindow.ui" line="713"/>
        <source>Built-in charsets:

  ?l = abcdefghijklmnopqrstuvwxyz [a-z]
  ?u = ABCDEFGHIJKLMNOPQRSTUVWXYZ [A-Z]
  ?d = 0123456789 [0-9]
  ?h = 0123456789abcdef [0-9a-f]
  ?H = 0123456789ABCDEF [0-9A-F]
  ?s =  !&quot;#$%&amp;&apos;()*+,-./:;&lt;=&gt;?@[]^_`{|}~
  ?a = ?l?u?d?s
  ?b = 0x00 - 0xff</source>
        <translation>Set di caratteri integrati:

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
        <location filename="../src/mainwindow.ui" line="563"/>
        <source>Custom charsets</source>
        <translation>Set di caratteri personalizzati</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="571"/>
        <source>Charset 1:</source>
        <translation>Set di caratteri 1:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="613"/>
        <source>Charset 2:</source>
        <translation>Set di caratteri 2:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="655"/>
        <source>Charset 3:</source>
        <translation>Set di caratteri 3:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="697"/>
        <source>Charset 4:</source>
        <translation>Set di caratteri 4:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="740"/>
        <source>Resources</source>
        <translation>Risorse</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="750"/>
        <source>CPU affinity:</source>
        <translation>Affinità CPU:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="757"/>
        <source>Backend devices:</source>
        <translation>Dispositivi backend:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="764"/>
        <source>Segment size:</source>
        <translation>Dimensione del segmento:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="775"/>
        <source>locks to CPU devices, separate with comma</source>
        <translation>vincola ai dispositivi CPU, separare con virgole</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="782"/>
        <source>devices to use, separate with comma</source>
        <translation>dispositivi da usare, separare con virgole</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="798"/>
        <source> MB</source>
        <translation> MB</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="821"/>
        <source>Output</source>
        <translation>Output</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="829"/>
        <source>Write recovered hashes to file:</source>
        <translation>Scrivi gli hash recuperati nel file:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="841"/>
        <source>Built-in variables:

  &lt;hash&gt; = current hash filename or hash
  &lt;unixtime&gt; = current unix timestamp</source>
        <translation>Variabili integrate:

  &lt;hash&gt; = nome del file di hash corrente o hash
  &lt;unixtime&gt; = timestamp Unix corrente</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="871"/>
        <source>Format:</source>
        <translation>Formato:</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="881"/>
        <source>Outfile format to use, separated with commas.
default: 1,2

  1 = hash[:salt]
  2 = plain
  3 = hex_plain
  4 = crack_pos
  5 = timestamp absolute
  6 = timestamp relative</source>
        <translation>Formato del file di output da usare, separato da virgole.
predefinito: 1,2

  1 = hash[:salt]
  2 = plain
  3 = hex_plain
  4 = crack_pos
  5 = timestamp absolute
  6 = timestamp relative</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="923"/>
        <source>Start</source>
        <translation>Avvia</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="930"/>
        <source>Copy command to clipboard</source>
        <translation>Copia il comando negli appunti</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="973"/>
        <source>Advanced</source>
        <translation>Avanzate</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="997"/>
        <source>General</source>
        <translation>Generale</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1009"/>
        <source>Enable optimized kernels (limits password length)</source>
        <translation>Abilita i kernel ottimizzati (limita la lunghezza della password)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1016"/>
        <source>Return expected speed of the attack, then quit</source>
        <translation>Mostra la velocità prevista dell’attacco, poi esci</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1025"/>
        <source>Override workload profile</source>
        <translation>Sovrascrivi il profilo di carico di lavoro</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1035"/>
        <source> # | Performance | Runtime | Power Consumption | Desktop Impact
 ===+=============+=========+===================+=========
  1 | Low               |   2 ms    | Low               | Minimal
  2 | Default         |  12 ms   | Economic     | Noticeable
  3 | High              |  96 ms   | High              | Unresponsive
  4 | Nightmare   | 480 ms  | Insane          | Headless

</source>
        <translation> # | Prestazioni | Durata | Consumo energetico | Impatto sul desktop
 ===+=============+========+====================+====================
  1 | Basso       |   2 ms | Basso              | Minimo
  2 | Predefinito |  12 ms | Economico          | Percettibile
  3 | Alto        |  96 ms | Alto               | Non reattivo
  4 | Incubo      | 480 ms | Folle              | Senza desktop

</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1121"/>
        <source>Help</source>
        <translation>Aiuto</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1128"/>
        <source>Tools</source>
        <translation>Strumenti</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1134"/>
        <source>File</source>
        <translation>File</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1148"/>
        <source>About</source>
        <translation>Informazioni</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1153"/>
        <source>Reset fields</source>
        <translation>Reimposta i campi</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1158"/>
        <source>Quit</source>
        <translation>Esci</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1163"/>
        <source>About Qt</source>
        <translation>Informazioni su Qt</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1168"/>
        <source>Settings</source>
        <translation>Impostazioni</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1173"/>
        <source>Export Profile</source>
        <translation>Esporta profilo</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.ui" line="1178"/>
        <source>Import Profile</source>
        <translation>Importa profilo</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="172"/>
        <source>Save Profile</source>
        <translation>Salva profilo</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="173"/>
        <location filename="../src/mainwindow.cpp" line="191"/>
        <source>JSON Files (*.json)</source>
        <translation>File JSON (*.json)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="180"/>
        <source>Saved</source>
        <translation>Salvato</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="180"/>
        <source>Profile saved to %1.</source>
        <translation>Profilo salvato in %1.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="182"/>
        <location filename="../src/mainwindow.cpp" line="332"/>
        <source>Save failed</source>
        <translation>Salvataggio non riuscito</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="190"/>
        <source>Load Profile</source>
        <translation>Carica profilo</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="205"/>
        <source>Loaded</source>
        <translation>Caricato</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="205"/>
        <source>Profile loaded from %1.</source>
        <translation>Profilo caricato da %1.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="207"/>
        <location filename="../src/mainwindow.cpp" line="318"/>
        <source>Load failed</source>
        <translation>Caricamento non riuscito</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="344"/>
        <source>Straight</source>
        <translation>Dizionario</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="345"/>
        <source>Combination</source>
        <translation>Combinazione</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="346"/>
        <source>Brute-force</source>
        <translation>Forza bruta</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="347"/>
        <source>Hybrid Wordlist + Mask</source>
        <translation>Ibrido: wordlist + maschera</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="348"/>
        <source>Hybrid Mask + Wordlist</source>
        <translation>Ibrido: maschera + wordlist</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="349"/>
        <source>Association</source>
        <translation>Associazione</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="361"/>
        <source>Updating...</source>
        <translation>Aggiornamento in corso...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="376"/>
        <source>Failed to obtain supported hash types.
Error: %1</source>
        <translation>Impossibile ottenere i tipi di hash supportati.
Errore: %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="408"/>
        <source>hashcat error</source>
        <translation>Errore di hashcat</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="442"/>
        <source>Open Hash File</source>
        <translation>Apri file di hash</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="450"/>
        <source>Save Output File</source>
        <translation>Salva file di output</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="467"/>
        <source>Add Wordlists</source>
        <translation>Aggiungi wordlist</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="553"/>
        <location filename="../src/mainwindow.cpp" line="561"/>
        <location filename="../src/mainwindow.cpp" line="569"/>
        <source>Open Rules File</source>
        <translation>Apri file di regole</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="666"/>
        <source>Launch failed</source>
        <translation>Avvio non riuscito</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="667"/>
        <source>Could not start %1: %2</source>
        <translation>Impossibile avviare %1: %2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="684"/>
        <source>Please choose a hash file.</source>
        <translation>Scegliere un file di hash.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="691"/>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to configure the path to the hashcat executable.</source>
        <translation>Vai su &lt;b&gt;%1 → %2&lt;/b&gt; per configurare il percorso dell’eseguibile di hashcat.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="701"/>
        <source>The configured hashcat executable does not exist:
%1
Navigate to %2 → %3 to change it.</source>
        <translation>L’eseguibile di hashcat configurato non esiste:
%1
Vai su %2 → %3 per modificarlo.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="709"/>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to select the terminal used for launching.</source>
        <translation>Vai su &lt;b&gt;%1 → %2&lt;/b&gt; per selezionare il terminale usato per l’avvio.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="714"/>
        <source>No supported terminal was found on this system. hashcat-gui needs one to show the hashcat output.</source>
        <translation>Nessun terminale supportato trovato su questo sistema. hashcat-gui ne richiede uno per mostrare l’output di hashcat.</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="720"/>
        <source>The configured terminal &quot;%1&quot; is not available. Available terminals: %2
Navigate to %3 → %4 to change it.</source>
        <translation>Il terminale configurato &quot;%1&quot; non è disponibile. Terminali disponibili: %2
Vai su %3 → %4 per modificarlo.</translation>
    </message>
</context>
<context>
    <name>SettingsDialog</name>
    <message>
        <location filename="../src/settingsdialog.ui" line="14"/>
        <source>Settings</source>
        <translation>Impostazioni</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.ui" line="38"/>
        <source>Hashcat executable</source>
        <translation>Eseguibile di hashcat</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.ui" line="50"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Download from &lt;a href=&quot;https://hashcat.net&quot;&gt;&lt;span style=&quot; text-decoration: underline; color:#27bf73;&quot;&gt;https://hashcat.net&lt;/span&gt;&lt;/a&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Scarica da &lt;a href=&quot;https://hashcat.net&quot;&gt;&lt;span style=&quot; text-decoration: underline; color:#27bf73;&quot;&gt;https://hashcat.net&lt;/span&gt;&lt;/a&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.ui" line="75"/>
        <source>Select</source>
        <translation>Seleziona</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.ui" line="96"/>
        <source>Terminal</source>
        <translation>Terminale</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.ui" line="136"/>
        <source>General</source>
        <translation>Generale</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.ui" line="142"/>
        <source>Use short parameters</source>
        <translation>Usa parametri brevi</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.ui" line="158"/>
        <source>Language</source>
        <translation>Lingua</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.ui" line="192"/>
        <source>Save</source>
        <translation>Salva</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.ui" line="199"/>
        <source>Cancel</source>
        <translation>Annulla</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="52"/>
        <source>System default</source>
        <translation>Predefinito di sistema</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="78"/>
        <source>Select the hashcat executable</source>
        <translation>Seleziona l’eseguibile di hashcat</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="80"/>
        <source>Executable Files (*.exe *.bin);;All Files (*)</source>
        <translation>File eseguibili (*.exe *.bin);;Tutti i file (*)</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="86"/>
        <source>Invalid file</source>
        <translation>File non valido</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="87"/>
        <source>The selected file is not an executable.</source>
        <translation>Il file selezionato non è un eseguibile.</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="110"/>
        <source>Restart required</source>
        <translation>Riavvio necessario</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="111"/>
        <source>Restart hashcat-gui to apply the new language.</source>
        <translation>Riavvia hashcat-gui per applicare la nuova lingua.</translation>
    </message>
</context>
<context>
    <name>WidgetStateSerializer</name>
    <message>
        <location filename="../src/widgetstateserializer.cpp" line="174"/>
        <source>Could not open %1 for writing: %2</source>
        <translation>Impossibile aprire %1 in scrittura: %2</translation>
    </message>
    <message>
        <location filename="../src/widgetstateserializer.cpp" line="180"/>
        <source>Could not write to %1: %2</source>
        <translation>Impossibile scrivere in %1: %2</translation>
    </message>
    <message>
        <location filename="../src/widgetstateserializer.cpp" line="199"/>
        <source>Could not open %1 for reading: %2</source>
        <translation>Impossibile aprire %1 in lettura: %2</translation>
    </message>
    <message>
        <location filename="../src/widgetstateserializer.cpp" line="207"/>
        <source>The file is not a valid JSON file: %1</source>
        <translation>Il file non è un file JSON valido: %1</translation>
    </message>
    <message>
        <location filename="../src/widgetstateserializer.cpp" line="213"/>
        <source>The file does not contain a profile for &quot;%1&quot;.</source>
        <translation>Il file non contiene un profilo per &quot;%1&quot;.</translation>
    </message>
</context>
</TS>
