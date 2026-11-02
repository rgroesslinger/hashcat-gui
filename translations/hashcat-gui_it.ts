<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="it" sourcelanguage="en">
<context>
    <name>AboutDialog</name>
    <message>
        <source>About</source>
        <translation>Informazioni</translation>
    </message>
    <message>
        <source>Graphical user interface for hashcat, the password recovery utility</source>
        <translation>Interfaccia grafica per hashcat, l’utilità di recupero delle password</translation>
    </message>
    <message>
        <source>Version information</source>
        <translation>Informazioni sulla versione</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Sconosciuto</translation>
    </message>
    <message>
        <source>Updates</source>
        <translation>Aggiornamenti</translation>
    </message>
    <message>
        <source>Checking for updates…</source>
        <translation>Controllo degli aggiornamenti…</translation>
    </message>
    <message>
        <source>System information</source>
        <translation>Informazioni di sistema</translation>
    </message>
    <message>
        <source>Operating system</source>
        <translation>Sistema operativo</translation>
    </message>
    <message>
        <source>Links</source>
        <translation>Collegamenti</translation>
    </message>
    <message>
        <source>License</source>
        <translation>Licenza</translation>
    </message>
    <message>
        <source>Copy the application, toolkit, system and hashcat versions to the clipboard</source>
        <translation>Copia negli appunti le versioni di applicazione, toolkit, sistema e hashcat</translation>
    </message>
    <message>
        <source>Copy details</source>
        <translation>Copia dettagli</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Copied to the clipboard</source>
        <translation>Copiato negli appunti</translation>
    </message>
    <message>
        <source>Project page</source>
        <translation>Pagina del progetto</translation>
    </message>
    <message>
        <source>Report an issue</source>
        <translation>Segnala un problema</translation>
    </message>
    <message>
        <source>hashcat</source>
        <translation>hashcat</translation>
    </message>
    <message>
        <source>hashcat FAQ</source>
        <translation>FAQ di hashcat</translation>
    </message>
    <message>
        <source>hashcat-gui is licensed under the %1.</source>
        <translation>hashcat-gui è distribuito con licenza %1.</translation>
    </message>
    <message>
        <source>GNU GPL v3.0 or later</source>
        <translation>GNU GPL v3.0 o successiva</translation>
    </message>
    <message>
        <source>Not configured</source>
        <translation>Non configurato</translation>
    </message>
    <message>
        <source>Error: %1</source>
        <translation>Errore: %1</translation>
    </message>
    <message>
        <source>A newer version is available: %1.</source>
        <translation>È disponibile una versione più recente: %1.</translation>
    </message>
    <message>
        <source>Release page</source>
        <translation>Pagina della release</translation>
    </message>
    <message>
        <source>No updates available.</source>
        <translation>Nessun aggiornamento disponibile.</translation>
    </message>
    <message>
        <source>Could not check for updates.</source>
        <translation>Impossibile controllare gli aggiornamenti.</translation>
    </message>
    <message>
        <source>Could not check for updates: %1</source>
        <translation>Impossibile controllare gli aggiornamenti: %1</translation>
    </message>
</context>
<context>
    <name>HashcatInfoParser</name>
    <message>
        <source>Invalid JSON returned from hashcat.</source>
        <translation>hashcat ha restituito un JSON non valido.</translation>
    </message>
    <message>
        <source>hashcat did not report any hash types.</source>
        <translation>hashcat non ha riportato alcun tipo di hash.</translation>
    </message>
</context>
<context>
    <name>HelperUtils</name>
    <message>
        <source>The hashcat path is not configured.</source>
        <translation>Il percorso di hashcat non è configurato.</translation>
    </message>
    <message>
        <source>Failed to start hashcat
%1</source>
        <translation>Impossibile avviare hashcat
%1</translation>
    </message>
    <message>
        <source>hashcat timed out
%1</source>
        <translation>Timeout di hashcat
%1</translation>
    </message>
    <message>
        <source>No supported terminal was found on this system. hashcat-gui needs one to show the hashcat output.</source>
        <translation>Non è stato trovato alcun terminale supportato su questo sistema. hashcat-gui ne ha bisogno per mostrare l’output di hashcat.</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <source>Hash file or hash:</source>
        <translation>File di hash o hash:</translation>
    </message>
    <message>
        <source>The hash, a file of hashes, or a .hc22000/.hccapx WPA capture.</source>
        <translation>L’hash, un file di hash o una cattura WPA .hc22000/.hccapx.</translation>
    </message>
    <message>
        <source>Choose a file of hashes.</source>
        <translation>Scegli un file di hash.</translation>
    </message>
    <message>
        <source>Salt is hex</source>
        <translation>Salt esadecimale</translation>
    </message>
    <message>
        <source>Rewrite the hash file without cracked hashes, including potfile-known ones; the potfile is untouched. Refused in attack mode 9.</source>
        <translation>Riscrivi il file di hash senza gli hash decifrati, inclusi quelli già noti al potfile; il potfile non viene modificato. Rifiutato nella modalità di attacco 9.</translation>
    </message>
    <message>
        <source>Remove hash from hash list once it is cracked</source>
        <translation>Rimuovi l’hash dall’elenco una volta decifrato</translation>
    </message>
    <message>
        <source>Ignore the username before the mode’s separator (usually “:”), so accounts sharing a hash are cracked once.</source>
        <translation>Ignora il nome utente prima del separatore della modalità (di solito “:”), così gli account che condividono un hash vengono decifrati una sola volta.</translation>
    </message>
    <message>
        <source>Hash recipe, e.g. “md5(md5(salt) . pass)”. Wrap it in the shell’s outer quotes and put any inner strings in the other kind of quotes; PowerShell needs a backtick before $.</source>
        <translation>Ricetta dell’hash, ad es. “md5(md5(salt) . pass)”. Racchiudila tra le virgolette esterne della shell e le stringhe interne tra quelle dell’altro tipo; PowerShell richiede un backtick prima di $.</translation>
    </message>
    <message>
        <source>Move item up</source>
        <translation>Sposta in alto</translation>
    </message>
    <message>
        <source>Move selected wordlist up</source>
        <translation>Sposta in alto la wordlist selezionata</translation>
    </message>
    <message>
        <source>Move item down</source>
        <translation>Sposta in basso</translation>
    </message>
    <message>
        <source>Move selected wordlist down</source>
        <translation>Sposta in basso la wordlist selezionata</translation>
    </message>
    <message>
        <source>Add one or more wordlist files. Each checked row is passed to hashcat.</source>
        <translation>Aggiungi uno o più file di wordlist. Ogni riga selezionata viene passata a hashcat.</translation>
    </message>
    <message>
        <source>Remove the selected wordlists.</source>
        <translation>Rimuovi le wordlist selezionate.</translation>
    </message>
    <message>
        <source>Remove wordlists</source>
        <translation>Rimuovi wordlist</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Rimuovi</translation>
    </message>
    <message>
        <source>Wordlist is hex</source>
        <translation>Wordlist esadecimale</translation>
    </message>
    <message>
        <source>Encoding from:</source>
        <translation>Codifica da:</translation>
    </message>
    <message>
        <source>Encoding to:</source>
        <translation>Codifica a:</translation>
    </message>
    <message>
        <source>Space-separated key=value settings for attack mode 9, e.g. phases=words,rules or rules.rulefile=…. Only used without a wordlist.</source>
        <translation>Impostazioni key=value separate da spazi per la modalità di attacco 9, ad es. phases=words,rules oppure rules.rulefile=…. Usate solo senza wordlist.</translation>
    </message>
    <message>
        <source>A ruleset file, a folder with Grammar/grammar.txt, or a bundled name. Unchecked rows are skipped; none runs default-passwords.</source>
        <translation>Un file di set di regole, una cartella con Grammar/grammar.txt o un nome incluso. Le righe non selezionate vengono saltate; se nessuna è selezionata, viene eseguito default-passwords.</translation>
    </message>
    <message>
        <source>Add one or more PCFG ruleset files.</source>
        <translation>Aggiungi uno o più file di set di regole PCFG.</translation>
    </message>
    <message>
        <source>Add PCFG rulesets</source>
        <translation>Aggiungi set di regole PCFG</translation>
    </message>
    <message>
        <source>Remove the selected rulesets.</source>
        <translation>Rimuovi i set di regole selezionati.</translation>
    </message>
    <message>
        <source>Remove rulesets</source>
        <translation>Rimuovi set di regole</translation>
    </message>
    <message>
        <source>Move selected ruleset up</source>
        <translation>Sposta in alto il set di regole selezionato</translation>
    </message>
    <message>
        <source>Move selected ruleset down</source>
        <translation>Sposta in basso il set di regole selezionato</translation>
    </message>
    <message>
        <source>One plain number per ruleset, colon-separated, e.g. 2:1; the count must match the number of rulesets.</source>
        <translation>Un numero semplice per ogni set di regole, separati da due punti, ad es. 2:1; la quantità deve corrispondere al numero di set di regole.</translation>
    </message>
    <message>
        <source>Maximum candidate length the kernel builds, in 4-byte words: 0 (auto), 16 or 32.</source>
        <translation>Lunghezza massima dei candidati che il kernel costruisce, in parole da 4 byte: 0 (auto), 16 o 32.</translation>
    </message>
    <message>
        <source>1 steps to the next base word where the order allows instead of unranking it; the candidates are identical, only the fast path is off.</source>
        <translation>1 passa alla parola di base successiva, dove l’ordine lo consente, invece di calcolarla con l’unranking; i candidati sono identici, viene disattivato solo il percorso rapido.</translation>
    </message>
    <message>
        <source>Only enumerate candidates this mask admits; excludes omen=1, and hintaccount unless hint words come from hintwords or hintfile.</source>
        <translation>Enumera solo i candidati che questa maschera ammette; esclude omen=1 e hintaccount, a meno che le parole di suggerimento non provengano da hintwords o hintfile.</translation>
    </message>
    <message>
        <source>Space-separated key=value settings: omen, cache, lookup, pwmin, pwmax, hintaccount, hintwords, hintfile, hintrank, hintrepeat. Rulesets go first.</source>
        <translation>Impostazioni key=value separate da spazi: omen, cache, lookup, pwmin, pwmax, hintaccount, hintwords, hintfile, hintrank, hintrepeat. I set di regole vanno per primi.</translation>
    </message>
    <message>
        <source>Add one or more table files.</source>
        <translation>Aggiungi uno o più file di tabella.</translation>
    </message>
    <message>
        <source>Remove the selected tables.</source>
        <translation>Rimuovi le tabelle selezionate.</translation>
    </message>
    <message>
        <source>Remove tables</source>
        <translation>Rimuovi tabelle</translation>
    </message>
    <message>
        <source>1 (default) keeps a token unchanged as a choice, 0 drops it so the table only yields variations. identity=0 excludes single=1.</source>
        <translation>1 (predefinito) mantiene un token invariato come scelta, 0 lo scarta in modo che la tabella produca solo variazioni. identity=0 esclude single=1.</translation>
    </message>
    <message>
        <source>1 reads the wordlist as patterns: no word is returned unchanged and unmatched words are dropped. 0 (default) keeps them.</source>
        <translation>1 legge la wordlist come pattern: nessuna parola viene restituita invariata e le parole senza corrispondenza vengono scartate. 0 (predefinito) le mantiene.</translation>
    </message>
    <message>
        <source>single=1: one substitution per candidate instead of the cross product; fewer candidates, more host work on fast hashes.</source>
        <translation>single=1: una sostituzione per candidato invece del prodotto cartesiano; meno candidati, più lavoro per l’host con hash veloci.</translation>
    </message>
    <message>
        <source>Keep only the first N replacements of each source word; 0 means no limit. Applies to single=1 too.</source>
        <translation>Mantieni solo le prime N sostituzioni di ogni parola sorgente; 0 significa nessun limite. Vale anche per single=1.</translation>
    </message>
    <message>
        <source>How much of a word’s cross product the host enumerates before giving up; 0 means no limit.</source>
        <translation>Quanto del prodotto cartesiano di una parola l’host enumera prima di rinunciare; 0 significa nessun limite.</translation>
    </message>
    <message>
        <source>Key=value settings separated by spaces; everything after the first one is a setting, so tables come first.</source>
        <translation>Impostazioni key=value separate da spazi; tutto ciò che segue la prima è un’impostazione, quindi le tabelle vengono per prime.</translation>
    </message>
    <message>
        <source>Feed name or path. A slash means a path; otherwise hashcat tries feed_&lt;name&gt;, rust_&lt;name&gt;, then &lt;name&gt; in its feeds directory.</source>
        <translation>Nome o percorso del feed. Una barra indica un percorso; altrimenti hashcat prova feed_&lt;name&gt;, rust_&lt;name&gt; e poi &lt;name&gt; nella propria directory dei feed.</translation>
    </message>
    <message>
        <source>Browse for a feed plugin instead of naming it.</source>
        <translation>Cerca un plugin di feed invece di indicarne il nome.</translation>
    </message>
    <message>
        <source>Browse for a feed plugin</source>
        <translation>Cerca un plugin di feed</translation>
    </message>
    <message>
        <source>Argument passed to the feed; meaning and count depend on the feed (the wordlist feed takes wordlist paths).</source>
        <translation>Argomento passato al feed; significato e numero dipendono dal feed (il feed wordlist accetta percorsi di wordlist).</translation>
    </message>
    <message>
        <source>Add an argument for the feed, in order.</source>
        <translation>Aggiungi un argomento per il feed, in ordine.</translation>
    </message>
    <message>
        <source>Remove the selected feed arguments.</source>
        <translation>Rimuovi gli argomenti del feed selezionati.</translation>
    </message>
    <message>
        <source>Remove feed arguments</source>
        <translation>Rimuovi argomenti del feed</translation>
    </message>
    <message>
        <source>Move selected feed argument up</source>
        <translation>Sposta in alto l’argomento del feed selezionato</translation>
    </message>
    <message>
        <source>Move selected feed argument down</source>
        <translation>Sposta in basso l’argomento del feed selezionato</translation>
    </message>
    <message>
        <source>Read rules from the files below instead of generating them.</source>
        <translation>Leggi le regole dai file qui sotto invece di generarle.</translation>
    </message>
    <message>
        <source>Rules file 1:</source>
        <translation>File di regole 1:</translation>
    </message>
    <message>
        <source>Choose the rules file for slot 1.</source>
        <translation>Scegli il file di regole per lo slot 1.</translation>
    </message>
    <message>
        <source>Open rules file 1</source>
        <translation>Apri file di regole 1</translation>
    </message>
    <message>
        <source>Rules file 2:</source>
        <translation>File di regole 2:</translation>
    </message>
    <message>
        <source>Choose the rules file for slot 2.</source>
        <translation>Scegli il file di regole per lo slot 2.</translation>
    </message>
    <message>
        <source>Open rules file 2</source>
        <translation>Apri file di regole 2</translation>
    </message>
    <message>
        <source>Rules file 3:</source>
        <translation>File di regole 3:</translation>
    </message>
    <message>
        <source>Choose the rules file for slot 3.</source>
        <translation>Scegli il file di regole per lo slot 3.</translation>
    </message>
    <message>
        <source>Open rules file 3</source>
        <translation>Apri file di regole 3</translation>
    </message>
    <message>
        <source>Number of random rules hashcat generates (-g). Only used with Generate rules; no rules file is read.</source>
        <translation>Numero di regole casuali che hashcat genera (-g). Usato solo con “Genera regole”; non viene letto alcun file di regole.</translation>
    </message>
    <message>
        <source>Treat every -r file as one list: rule counts add instead of multiplying.</source>
        <translation>Tratta ogni file -r come un’unica lista: i conteggi delle regole si sommano invece di moltiplicarsi.</translation>
    </message>
    <message>
        <source>Wordlist rules</source>
        <translation>Regole della wordlist</translation>
    </message>
    <message>
        <source>Left rule:</source>
        <translation>Regola sinistra:</translation>
    </message>
    <message>
        <source>Right rule:</source>
        <translation>Regola destra:</translation>
    </message>
    <message>
        <source>Interpret the mask and custom charsets as hexadecimal bytes.</source>
        <translation>Interpreta la maschera e i set di caratteri personalizzati come byte esadecimali.</translation>
    </message>
    <message>
        <source>Charset is hex</source>
        <translation>Set di caratteri esadecimale</translation>
    </message>
    <message>
        <source>Ctrl+Shift+I</source>
        <translation>Ctrl+Shift+I</translation>
    </message>
    <message>
        <source>Increment mask length</source>
        <translation>Incrementa la lunghezza della maschera</translation>
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
        <translation>Usa i kernel ottimizzati di hashcat (-O); limitano la lunghezza massima della password.</translation>
    </message>
    <message>
        <source>Pin hashcat to these CPU devices, separated by commas. Leave empty to let hashcat decide.</source>
        <translation>Vincola hashcat a questi dispositivi CPU, separati da virgole. Lascia vuoto per lasciar decidere a hashcat.</translation>
    </message>
    <message>
        <source>Devices to use, separated by commas. Leave empty for all devices.</source>
        <translation>Dispositivi da usare, separati da virgole. Lascia vuoto per usare tutti i dispositivi.</translation>
    </message>
    <message>
        <source>Abort above</source>
        <translation>Interrompi sopra</translation>
    </message>
    <message>
        <source>Outfile</source>
        <translation>File di output</translation>
    </message>
    <message>
        <source>Save…</source>
        <translation>Salva…</translation>
    </message>
    <message>
        <source>Write the outfile as JSON. The outfile format is ignored.</source>
        <translation>Scrivi il file di output come JSON. Il formato del file di output viene ignorato.</translation>
    </message>
    <message>
        <source>Write JSON</source>
        <translation>Scrivi JSON</translation>
    </message>
    <message>
        <source>Do not write recovered hashes to the potfile.</source>
        <translation>Non scrivere gli hash recuperati nel potfile.</translation>
    </message>
    <message>
        <source>Load Example Session</source>
        <translation>Carica sessione di esempio</translation>
    </message>
    <message>
        <source>Reset Session</source>
        <translation>Reimposta sessione</translation>
    </message>
    <message>
        <source>Mode:</source>
        <translation>Modalità:</translation>
    </message>
    <message>
        <source>Attack</source>
        <translation>Attacco</translation>
    </message>
    <message>
        <source>Open…</source>
        <translation>Apri…</translation>
    </message>
    <message>
        <source>Assume the salt is given in hexadecimal.</source>
        <translation>Presuppone che il salt sia fornito in esadecimale.</translation>
    </message>
    <message>
        <source>Attack mode. Each choice reveals its own options below.</source>
        <translation>Modalità di attacco. Ogni scelta mostra le proprie opzioni qui sotto.</translation>
    </message>
    <message>
        <source>Hash type:</source>
        <translation>Tipo di hash:</translation>
    </message>
    <message>
        <source>Hash recipe:</source>
        <translation>Ricetta dell’hash:</translation>
    </message>
    <message>
        <source>Add files…</source>
        <translation>Aggiungi file…</translation>
    </message>
    <message>
        <source>hashcat reads every file directly inside the folder as one wordlist.</source>
        <translation>hashcat legge ogni file presente direttamente nella cartella come un’unica wordlist.</translation>
    </message>
    <message>
        <source>Add folder…</source>
        <translation>Aggiungi cartella…</translation>
    </message>
    <message>
        <source>Only checked rows are used. A folder is one wordlist: hashcat reads the files directly inside it.</source>
        <translation>Vengono usate solo le righe selezionate. Una cartella è una wordlist: hashcat legge i file presenti direttamente al suo interno.</translation>
    </message>
    <message>
        <source>Assume words in the wordlist are given in hexadecimal.</source>
        <translation>Presuppone che le parole della wordlist siano fornite in esadecimale.</translation>
    </message>
    <message>
        <source>Settings (no wordlist):</source>
        <translation>Impostazioni (senza wordlist):</translation>
    </message>
    <message>
        <source>PCFG</source>
        <translation>PCFG</translation>
    </message>
    <message>
        <source>A ruleset folder needs a Grammar/grammar.txt.</source>
        <translation>Una cartella di set di regole richiede un Grammar/grammar.txt.</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Scale:</translation>
    </message>
    <message>
        <source>Quantization steps per bit (1–64)</source>
        <translation>Passi di quantizzazione per bit (1–64)</translation>
    </message>
    <message>
        <source>Costmax:</source>
        <translation>Costmax:</translation>
    </message>
    <message>
        <source>Highest cost level to enumerate (1–64)</source>
        <translation>Livello di costo massimo da enumerare (1–64)</translation>
    </message>
    <message>
        <source>Weights:</source>
        <translation>Weights:</translation>
    </message>
    <message>
        <source>Threads:</source>
        <translation>Thread:</translation>
    </message>
    <message>
        <source>Cores that generate base words (0–16); 0 runs on the calling thread.</source>
        <translation>Core che generano le parole di base (0–16); 0 viene eseguito nel thread chiamante.</translation>
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
        <translation>Bit che un ciclo interno può coprire (0–27)</translation>
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
        <translation>1.0–64.0; il separatore decimale è un punto, non il separatore delle impostazioni locali.</translation>
    </message>
    <message>
        <source>Walk:</source>
        <translation>Walk:</translation>
    </message>
    <message>
        <source>Mask:</source>
        <translation>Maschera:</translation>
    </message>
    <message>
        <source>Extra settings (key=value):</source>
        <translation>Impostazioni aggiuntive (key=value):</translation>
    </message>
    <message>
        <source>Table</source>
        <translation>Tabella</translation>
    </message>
    <message>
        <source>Every checked row after the wordlist is a table.</source>
        <translation>Ogni riga selezionata dopo la wordlist è una tabella.</translation>
    </message>
    <message>
        <source>Add tables…</source>
        <translation>Aggiungi tabelle…</translation>
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
        <translation>Feed generico</translation>
    </message>
    <message>
        <source>Feed:</source>
        <translation>Feed:</translation>
    </message>
    <message>
        <source>wordlist, stdin, random, association, pcfg, table, hybrid, mask, or a feed path</source>
        <translation>wordlist, stdin, random, association, pcfg, table, hybrid, mask oppure il percorso di un feed</translation>
    </message>
    <message>
        <source>Browse…</source>
        <translation>Sfoglia…</translation>
    </message>
    <message>
        <source>Argument:</source>
        <translation>Argomento:</translation>
    </message>
    <message>
        <source>Add</source>
        <translation>Aggiungi</translation>
    </message>
    <message>
        <source>Arguments the feed reads, in order; only checked rows are used.</source>
        <translation>Argomenti letti dal feed, in ordine; vengono usate solo le righe selezionate.</translation>
    </message>
    <message>
        <source>Rules</source>
        <translation>Regole</translation>
    </message>
    <message>
        <source>Use rules files:</source>
        <translation>Usa file di regole:</translation>
    </message>
    <message>
        <source>Enable rules file slot 1 for this run.</source>
        <translation>Abilita lo slot 1 del file di regole per questa esecuzione.</translation>
    </message>
    <message>
        <source>Rules file for slot 1; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>File di regole per lo slot 1; applicato a ogni parola. Lascia vuoto per non usare lo slot.</translation>
    </message>
    <message>
        <source>Enable rules file slot 2 for this run.</source>
        <translation>Abilita lo slot 2 del file di regole per questa esecuzione.</translation>
    </message>
    <message>
        <source>Rules file for slot 2; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>File di regole per lo slot 2; applicato a ogni parola. Lascia vuoto per non usare lo slot.</translation>
    </message>
    <message>
        <source>Enable rules file slot 3 for this run.</source>
        <translation>Abilita lo slot 3 del file di regole per questa esecuzione.</translation>
    </message>
    <message>
        <source>Rules file for slot 3; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>File di regole per lo slot 3; applicato a ogni parola. Lascia vuoto per non usare lo slot.</translation>
    </message>
    <message>
        <source>Generate the rules automatically instead of reading them from a file.</source>
        <translation>Genera le regole automaticamente invece di leggerle da un file.</translation>
    </message>
    <message>
        <source>Generate rules:</source>
        <translation>Genera regole:</translation>
    </message>
    <message>
        <source>Join -r files into one list</source>
        <translation>Unisci i file -r in un’unica lista</translation>
    </message>
    <message>
        <source>Single rule applied to each word from the left wordlist.</source>
        <translation>Singola regola applicata a ogni parola della wordlist di sinistra.</translation>
    </message>
    <message>
        <source>Single rule applied to each word from the right wordlist.</source>
        <translation>Singola regola applicata a ogni parola della wordlist di destra.</translation>
    </message>
    <message>
        <source>Mask</source>
        <translation>Maschera</translation>
    </message>
    <message>
        <source>Built-in charsets:</source>
        <translation>Set di caratteri integrati:</translation>
    </message>
    <message>
        <source>Custom charsets</source>
        <translation>Set di caratteri personalizzati</translation>
    </message>
    <message>
        <source>Charset 1:</source>
        <translation>Set di caratteri 1:</translation>
    </message>
    <message>
        <source>Charset 2:</source>
        <translation>Set di caratteri 2:</translation>
    </message>
    <message>
        <source>Charset 3:</source>
        <translation>Set di caratteri 3:</translation>
    </message>
    <message>
        <source>Charset 4:</source>
        <translation>Set di caratteri 4:</translation>
    </message>
    <message>
        <source>Session and limits</source>
        <translation>Sessione e limiti</translation>
    </message>
    <message>
        <source>Session name:</source>
        <translation>Nome della sessione:</translation>
    </message>
    <message>
        <source>Name hashcat uses for its restore file and status output.</source>
        <translation>Nome che hashcat usa per il suo file di ripristino e per l’output di stato.</translation>
    </message>
    <message>
        <source>Runtime limit:</source>
        <translation>Limite di esecuzione:</translation>
    </message>
    <message>
        <source>unlimited</source>
        <translation>illimitato</translation>
    </message>
    <message>
        <source>Skip:</source>
        <translation>Salta:</translation>
    </message>
    <message>
        <source>Limit:</source>
        <translation>Limite:</translation>
    </message>
    <message>
        <source>Keep updating the run status (speed, progress, ETA) instead of printing one final line.</source>
        <translation>Continua ad aggiornare lo stato dell’esecuzione (velocità, avanzamento, tempo stimato) invece di stampare una sola riga finale.</translation>
    </message>
    <message>
        <source>Update status automatically</source>
        <translation>Aggiorna lo stato automaticamente</translation>
    </message>
    <message>
        <source>Seconds between status updates; only used when automatic status is enabled.</source>
        <translation>Secondi tra un aggiornamento dello stato e l’altro; usato solo quando lo stato automatico è abilitato.</translation>
    </message>
    <message>
        <source>Ignore warnings and start hashcat anyway. Use with care.</source>
        <translation>Ignora gli avvisi e avvia comunque hashcat. Usare con cautela.</translation>
    </message>
    <message>
        <source>Force (ignore warnings)</source>
        <translation>Forza (ignora gli avvisi)</translation>
    </message>
    <message>
        <source>Performance</source>
        <translation>Prestazioni</translation>
    </message>
    <message>
        <source>Cache path:</source>
        <translation>Percorso della cache:</translation>
    </message>
    <message>
        <source>Directory hashcat keeps its cache in. Must exist.</source>
        <translation>Directory in cui hashcat conserva la propria cache. Deve esistere.</translation>
    </message>
    <message>
        <source>Choose the cache directory.</source>
        <translation>Scegli la directory della cache.</translation>
    </message>
    <message>
        <source>Choose cache directory</source>
        <translation>Scegli directory della cache</translation>
    </message>
    <message>
        <source>Resources</source>
        <translation>Risorse</translation>
    </message>
    <message>
        <source>CPU affinity:</source>
        <translation>Affinità CPU:</translation>
    </message>
    <message>
        <source>Backend devices:</source>
        <translation>Dispositivi backend:</translation>
    </message>
    <message>
        <source>Device types:</source>
        <translation>Tipi di dispositivo:</translation>
    </message>
    <message>
        <source>Abort when any backend device exceeds this temperature in °C; 0 disables it.</source>
        <translation>Interrompi quando un qualsiasi dispositivo backend supera questa temperatura in °C; 0 la disattiva.</translation>
    </message>
    <message>
        <source>Write recovered hashes to the file below, in the selected formats.</source>
        <translation>Scrivi gli hash recuperati nel file qui sotto, nei formati selezionati.</translation>
    </message>
    <message>
        <source>Write recovered hashes to file:</source>
        <translation>Scrivi gli hash recuperati nel file:</translation>
    </message>
    <message>
        <source>The file the recovered hashes are written to. Format variables:

&lt;hash&gt; = current hash filename or hash
&lt;unixtime&gt; = current Unix timestamp</source>
        <translation>Il file in cui vengono scritti gli hash recuperati. Variabili di formato:

&lt;hash&gt; = nome del file di hash corrente o hash
&lt;unixtime&gt; = timestamp Unix corrente</translation>
    </message>
    <message>
        <source>Choose the outfile path.</source>
        <translation>Scegli il percorso del file di output.</translation>
    </message>
    <message>
        <source>Choose outfile</source>
        <translation>Scegli file di output</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Formato:</translation>
    </message>
    <message>
        <source>Do not write the potfile</source>
        <translation>Non scrivere il potfile</translation>
    </message>
    <message>
        <source>Run hashcat in a terminal with these options.</source>
        <translation>Esegui hashcat in un terminale con queste opzioni.</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>Avvia</translation>
    </message>
    <message>
        <source>Copy command to clipboard</source>
        <translation>Copia il comando negli appunti</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Generale</translation>
    </message>
    <message>
        <source>Ignore username in hash file</source>
        <translation>Ignora il nome utente nel file di hash</translation>
    </message>
    <message>
        <source>Wordlists</source>
        <translation>Wordlist</translation>
    </message>
    <message>
        <source>Increment the mask length from the minimum to the maximum.</source>
        <translation>Incrementa la lunghezza della maschera dal minimo al massimo.</translation>
    </message>
    <message>
        <source>Minimum mask length; 0 uses hashcat’s default.</source>
        <translation>Lunghezza minima della maschera; 0 usa il valore predefinito di hashcat.</translation>
    </message>
    <message>
        <source>Maximum mask length; 0 uses hashcat’s default.</source>
        <translation>Lunghezza massima della maschera; 0 usa il valore predefinito di hashcat.</translation>
    </message>
    <message>
        <source>Abort the session after this many seconds; 0 runs until hashcat finishes.</source>
        <translation>Interrompi la sessione dopo questo numero di secondi; 0 la esegue finché hashcat non termina.</translation>
    </message>
    <message>
        <source>&#xa0;s</source>
        <translation>&#xa0;s</translation>
    </message>
    <message>
        <source>Skip this many candidates from the start.</source>
        <translation>Salta questo numero di candidati dall’inizio.</translation>
    </message>
    <message>
        <source>Only test this many candidates after the skipped ones.</source>
        <translation>Testa solo questo numero di candidati dopo quelli saltati.</translation>
    </message>
    <message>
        <source>Enable optimized kernels (limits password length)</source>
        <translation>Abilita i kernel ottimizzati (limita la lunghezza della password)</translation>
    </message>
    <message>
        <source>Print only the attack’s speed, then quit; no cracking.</source>
        <translation>Stampa solo la velocità dell’attacco, poi esci; nessuna decifratura.</translation>
    </message>
    <message>
        <source>Return expected speed of the attack, then quit</source>
        <translation>Mostra la velocità prevista dell’attacco, poi esci</translation>
    </message>
    <message>
        <source>&#xa0;°C</source>
        <translation>&#xa0;°C</translation>
    </message>
    <message>
        <source>Copy command</source>
        <translation>Copia comando</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>&amp;Aiuto</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Strumenti</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;File</translation>
    </message>
    <message>
        <source>Ctrl+Q</source>
        <translation>Ctrl+Q</translation>
    </message>
    <message>
        <source>Settings…</source>
        <translation>Impostazioni…</translation>
    </message>
    <message>
        <source>Ctrl+,</source>
        <translation>Ctrl+,</translation>
    </message>
    <message>
        <source>Import Settings…</source>
        <translation>Importa impostazioni…</translation>
    </message>
    <message>
        <source>Ctrl+Shift+O</source>
        <translation>Ctrl+Shift+O</translation>
    </message>
    <message>
        <source>Export Settings…</source>
        <translation>Esporta impostazioni…</translation>
    </message>
    <message>
        <source>Ctrl+Shift+S</source>
        <translation>Ctrl+Shift+S</translation>
    </message>
    <message>
        <source>Export Session…</source>
        <translation>Esporta sessione…</translation>
    </message>
    <message>
        <source>Ctrl+Shift+E</source>
        <translation>Ctrl+Shift+E</translation>
    </message>
    <message>
        <source>Import Session…</source>
        <translation>Importa sessione…</translation>
    </message>
    <message>
        <source>About</source>
        <translation>Informazioni</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation>Esci</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>Informazioni su Qt</translation>
    </message>
    <message>
        <source>JSON Files (*.json)</source>
        <translation>File JSON (*.json)</translation>
    </message>
    <message>
        <source>The session file could not be read, so the session was not restored. The file was left in place.

%1</source>
        <translation>Impossibile leggere il file di sessione, quindi la sessione non è stata ripristinata. Il file è stato lasciato così com’è.

%1</translation>
    </message>
    <message>
        <source>Save failed</source>
        <translation>Salvataggio non riuscito</translation>
    </message>
    <message>
        <source>Outfile format to use, separated by commas.
Default: 1,2</source>
        <translation>Formato del file di output da usare, separato da virgole.
Predefinito: 1,2</translation>
    </message>
    <message>
        <source>Common encodings:</source>
        <translation>Codifiche comuni:</translation>
    </message>
    <message>
        <source>Force the internal wordlist encoding from this code. Leave empty to let hashcat decide.</source>
        <translation>Forza la codifica interna della wordlist da questo codice. Lascia vuoto per lasciar decidere a hashcat.</translation>
    </message>
    <message>
        <source>Force the internal wordlist encoding to this code. Leave empty to let hashcat decide.</source>
        <translation>Forza la codifica interna della wordlist a questo codice. Lascia vuoto per lasciar decidere a hashcat.</translation>
    </message>
    <message>
        <source>Exported</source>
        <translation>Esportato</translation>
    </message>
    <message>
        <source>Export failed</source>
        <translation>Esportazione non riuscita</translation>
    </message>
    <message>
        <source>Import failed</source>
        <translation>Importazione non riuscita</translation>
    </message>
    <message>
        <source>Imported</source>
        <translation>Importato</translation>
    </message>
    <message>
        <source>The file was made by an older version of hashcat-gui (format version %1; this version reads format version %2).</source>
        <translation>Il file è stato creato da una versione precedente di hashcat-gui (versione del formato %1; questa versione legge la versione del formato %2).</translation>
    </message>
    <message>
        <source>The file holds sections this import does not use: %1.</source>
        <translation>Il file contiene sezioni che questa importazione non usa: %1.</translation>
    </message>
    <message>
        <source>The file is in format version %1, but this version reads format version %2, so it was not applied. Update hashcat-gui to read it.</source>
        <translation>Il file è nella versione del formato %1, ma questa versione legge la versione del formato %2, quindi non è stato applicato. Aggiorna hashcat-gui per leggerlo.</translation>
    </message>
    <message>
        <source>The session file is in format version %1, but this version reads format version %2, so the session was not restored. The file was left as it is, so the session of this run is not stored either. Update hashcat-gui to read it.</source>
        <translation>Il file di sessione è nella versione del formato %1, ma questa versione legge la versione del formato %2, quindi la sessione non è stata ripristinata. Il file è stato lasciato così com’è, quindi nemmeno la sessione di questa esecuzione viene salvata. Aggiorna hashcat-gui per leggerlo.</translation>
    </message>
    <message>
        <source>Settings exported to %1.</source>
        <translation>Impostazioni esportate in %1.</translation>
    </message>
    <message>
        <source>OpenCL device types to use, separated by commas.</source>
        <translation>Tipi di dispositivo OpenCL da usare, separati da virgole.</translation>
    </message>
    <message>
        <source>Export session</source>
        <translation>Esporta sessione</translation>
    </message>
    <message>
        <source>Session exported to “%1”.</source>
        <translation>Sessione esportata in “%1”.</translation>
    </message>
    <message>
        <source>Import session</source>
        <translation>Importa sessione</translation>
    </message>
    <message>
        <source>Session imported from “%1”.</source>
        <translation>Sessione importata da “%1”.</translation>
    </message>
    <message>
        <source>Session imported from “%1”.

%2</source>
        <translation>Sessione importata da “%1”.

%2</translation>
    </message>
    <message>
        <source>Export settings</source>
        <translation>Esporta impostazioni</translation>
    </message>
    <message>
        <source>Import settings</source>
        <translation>Importa impostazioni</translation>
    </message>
    <message>
        <source>Settings loaded from “%1”.</source>
        <translation>Impostazioni caricate da “%1”.</translation>
    </message>
    <message>
        <source>Review them and save to apply them.</source>
        <translation>Controllale e salva per applicarle.</translation>
    </message>
    <message>
        <source>Load example session</source>
        <translation>Carica sessione di esempio</translation>
    </message>
    <message>
        <source>Reset session</source>
        <translation>Reimposta sessione</translation>
    </message>
    <message>
        <source>Reset every field to its default? The current session is lost.</source>
        <translation>Reimpostare ogni campo al valore predefinito? La sessione corrente andrà persa.</translation>
    </message>
    <message>
        <source>Session warning</source>
        <translation>Avviso di sessione</translation>
    </message>
    <message>
        <source>Updating…</source>
        <translation>Aggiornamento…</translation>
    </message>
    <message>
        <source>Failed to load hash types</source>
        <translation>Caricamento dei tipi di hash non riuscito</translation>
    </message>
    <message>
        <source>This mode needs a wordlist and a mask.</source>
        <translation>Questa modalità richiede una wordlist e una maschera.</translation>
    </message>
    <message>
        <source>This mode needs at least one wordlist.</source>
        <translation>Questa modalità richiede almeno una wordlist.</translation>
    </message>
    <message>
        <source>Open hash file</source>
        <translation>Apri file di hash</translation>
    </message>
    <message>
        <source>Save output file</source>
        <translation>Salva file di output</translation>
    </message>
    <message>
        <source>Add wordlists</source>
        <translation>Aggiungi wordlist</translation>
    </message>
    <message>
        <source>Open rules file</source>
        <translation>Apri file di regole</translation>
    </message>
    <message>
        <source>Could not start “%1”: %2</source>
        <translation>Impossibile avviare “%1”: %2</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not available. Available terminals: %2
Navigate to %3 → %4 to change it.</source>
        <translation>Il terminale configurato “%1” non è disponibile. Terminali disponibili: %2
Vai su %3 → %4 per cambiarlo.</translation>
    </message>
    <message>
        <source>This mode needs exactly %1 wordlists (left, right).
Currently selected: %2.</source>
        <translation>Questa modalità richiede esattamente %1 wordlist (sinistra, destra).
Attualmente selezionate: %2.</translation>
    </message>
    <message>
        <source>The session names the hash type %1, which this hashcat version does not support.</source>
        <translation>La sessione indica il tipo di hash %1, che questa versione di hashcat non supporta.</translation>
    </message>
    <message>
        <source>Password candidate mask. ?1–?4 insert a custom charset, ?w inserts the wordlist word and ?q a second wordlist word in the Hybrid (mask + ?w/?q) mode, and ?? is a literal ?.</source>
        <translation>Maschera dei candidati password. ?1–?4 inseriscono un set di caratteri personalizzato, ?w inserisce la parola della wordlist e ?q una seconda parola della wordlist nella modalità Ibrido (maschera + ?w/?q), e ?? è un ? letterale.</translation>
    </message>
    <message>
        <source>Load the %1 example? The current session is lost.

The example cracks a test hash; hashcat should recover the password %2.</source>
        <translation>Caricare l’esempio %1? La sessione corrente andrà persa.

L’esempio decifra un hash di prova; hashcat dovrebbe recuperare la password %2.</translation>
    </message>
    <message>
        <source>Could not create the example directory “%1”.</source>
        <translation>Impossibile creare la directory di esempio “%1”.</translation>
    </message>
    <message>
        <source>Could not read the bundled file “%1”: %2</source>
        <translation>Impossibile leggere il file incluso “%1”: %2</translation>
    </message>
    <message>
        <source>Could not write “%1”: %2</source>
        <translation>Impossibile scrivere “%1”: %2</translation>
    </message>
    <message>
        <source>Straight</source>
        <translation>Dizionario</translation>
    </message>
    <message>
        <source>Combination</source>
        <translation>Combinazione</translation>
    </message>
    <message>
        <source>Brute-force</source>
        <translation>Forza bruta</translation>
    </message>
    <message>
        <source>Hybrid Wordlist + Mask</source>
        <translation>Ibrido: wordlist + maschera</translation>
    </message>
    <message>
        <source>Hybrid Mask + Wordlist</source>
        <translation>Ibrido: maschera + wordlist</translation>
    </message>
    <message>
        <source>Generic (feed)</source>
        <translation>Generico (feed)</translation>
    </message>
    <message>
        <source>Association</source>
        <translation>Associazione</translation>
    </message>
    <message>
        <source>Hybrid (mask + ?w/?q)</source>
        <translation>Ibrido (maschera + ?w/?q)</translation>
    </message>
    <message>
        <source>Failed to obtain supported hash types.
Error: %1</source>
        <translation>Impossibile ottenere i tipi di hash supportati.
Errore: %1</translation>
    </message>
    <message>
        <source>The hash types could not be loaded. Check the hashcat path in Settings.</source>
        <translation>Impossibile caricare i tipi di hash. Controlla il percorso di hashcat nelle Impostazioni.</translation>
    </message>
    <message>
        <source>

This can be caused by Windows Defender or other antivirus software. See the &lt;a href=&quot;%1&quot;&gt;FAQ entry&lt;/a&gt; for a remedy.</source>
        <translation>

Può essere causato da Windows Defender o da altro software antivirus. Consulta la &lt;a href=&quot;%1&quot;&gt;voce delle FAQ&lt;/a&gt; per trovare un rimedio.</translation>
    </message>
    <message>
        <source>Set the hashcat path under %1 → %2 to load the available hash types.</source>
        <translation>Imposta il percorso di hashcat in %1 → %2 per caricare i tipi di hash disponibili.</translation>
    </message>
    <message>
        <source>This mode needs exactly 2 wordlists (left, right).</source>
        <translation>Questa modalità richiede esattamente 2 wordlist (sinistra, destra).</translation>
    </message>
    <message>
        <source>This mode needs a mask.</source>
        <translation>Questa modalità richiede una maschera.</translation>
    </message>
    <message>
        <source>This mode runs hashcat’s bundled default-passwords ruleset; name one or more to use your own.</source>
        <translation>Questa modalità esegue il set di regole default-passwords incluso in hashcat; indicane uno o più per usare i tuoi.</translation>
    </message>
    <message>
        <source>This mode needs one wordlist and at least one table.</source>
        <translation>Questa modalità richiede una wordlist e almeno una tabella.</translation>
    </message>
    <message>
        <source>This mode needs a feed. Arguments depend on the feed: random and stdin need none, wordlist needs a path.</source>
        <translation>Questa modalità richiede un feed. Gli argomenti dipendono dal feed: random e stdin non ne richiedono, wordlist richiede un percorso.</translation>
    </message>
    <message>
        <source>This mode needs at most one wordlist. Without one the words come from the hash file and the settings line can bound the run; with one the two files are paired by line.</source>
        <translation>Questa modalità richiede al massimo una wordlist. Senza wordlist, le parole provengono dal file di hash e la riga delle impostazioni può delimitare l’esecuzione; con una wordlist, i due file vengono abbinati riga per riga.</translation>
    </message>
    <message>
        <source>This mode needs a mask with exactly one ?w; a ?q after ?w names a second wordlist.</source>
        <translation>Questa modalità richiede una maschera con esattamente un ?w; un ?q dopo ?w indica una seconda wordlist.</translation>
    </message>
    <message>
        <source>Open cache directory</source>
        <translation>Apri directory della cache</translation>
    </message>
    <message>
        <source>Add wordlist folder</source>
        <translation>Aggiungi cartella di wordlist</translation>
    </message>
    <message>
        <source>Add rulesets</source>
        <translation>Aggiungi set di regole</translation>
    </message>
    <message>
        <source>Add ruleset folder</source>
        <translation>Aggiungi cartella di set di regole</translation>
    </message>
    <message>
        <source>Add tables</source>
        <translation>Aggiungi tabelle</translation>
    </message>
    <message>
        <source>Open feed file</source>
        <translation>Apri file di feed</translation>
    </message>
    <message>
        <source>This folder has no wordlists. hashcat reads the regular files directly inside it, not subfolders.</source>
        <translation>Questa cartella non contiene wordlist. hashcat legge i file normali presenti direttamente al suo interno, non le sottocartelle.</translation>
    </message>
    <message>
        <source>This wordlist does not exist:
%1</source>
        <translation>Questa wordlist non esiste:
%1</translation>
    </message>
    <message>
        <source>This ruleset folder has no Grammar/grammar.txt:
%1</source>
        <translation>Questa cartella di set di regole non contiene Grammar/grammar.txt:
%1</translation>
    </message>
    <message>
        <source>This ruleset does not exist:
%1</source>
        <translation>Questo set di regole non esiste:
%1</translation>
    </message>
    <message>
        <source>A table must be a file; hashcat does not read a folder:
%1</source>
        <translation>Una tabella deve essere un file; hashcat non legge una cartella:
%1</translation>
    </message>
    <message>
        <source>This table does not exist:
%1</source>
        <translation>Questa tabella non esiste:
%1</translation>
    </message>
    <message>
        <source>Copied to the clipboard</source>
        <translation>Copiato negli appunti</translation>
    </message>
    <message>
        <source>Launch failed</source>
        <translation>Avvio non riuscito</translation>
    </message>
    <message>
        <source>Please choose a hash file.</source>
        <translation>Scegli un file di hash.</translation>
    </message>
    <message>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to configure the path to the hashcat executable.</source>
        <translation>Vai su &lt;b&gt;%1 → %2&lt;/b&gt; per configurare il percorso dell’eseguibile di hashcat.</translation>
    </message>
    <message>
        <source>The configured hashcat executable does not exist:
%1
Navigate to %2 → %3 to change it.</source>
        <translation>L’eseguibile di hashcat configurato non esiste:
%1
Vai su %2 → %3 per modificarlo.</translation>
    </message>
    <message>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to select the terminal used for launching.</source>
        <translation>Vai su &lt;b&gt;%1 → %2&lt;/b&gt; per selezionare il terminale usato per l’avvio.</translation>
    </message>
    <message>
        <source>Maximum number of wordlists for this mode: %1.
Currently selected: %2.</source>
        <translation>Numero massimo di wordlist per questa modalità: %1.
Attualmente selezionate: %2.</translation>
    </message>
    <message>
        <source>hashcat pairs the two files by line here; this GUI offers only one wordlist for that role.</source>
        <translation>Qui hashcat abbina i due file riga per riga; questa interfaccia offre una sola wordlist per quel ruolo.</translation>
    </message>
    <message>
        <source>This wordlist has to be a file: hashcat reads it as the multiplied wordlist (the ?q side), and a folder cannot be used there:
%1</source>
        <translation>Questa wordlist deve essere un file: hashcat la legge come wordlist moltiplicata (il lato ?q) e lì non si può usare una cartella:
%1</translation>
    </message>
    <message>
        <source>This mode needs at least one table.</source>
        <translation>Questa modalità richiede almeno una tabella.</translation>
    </message>
    <message>
        <source>This mode needs a feed: a bundled name (wordlist, stdin, random, …) or the path of a feed file.</source>
        <translation>Questa modalità richiede un feed: un nome incluso (wordlist, stdin, random, …) oppure il percorso di un file di feed.</translation>
    </message>
    <message>
        <source>This mode pairs the wordlists with the mask; the mask needs a ?w for the word.</source>
        <translation>Questa modalità abbina le wordlist alla maschera; la maschera richiede un ?w per la parola.</translation>
    </message>
    <message>
        <source>The mask contains more than one ?w.</source>
        <translation>La maschera contiene più di un ?w.</translation>
    </message>
    <message>
        <source>The mask contains a ?q before the first ?w.</source>
        <translation>La maschera contiene un ?q prima del primo ?w.</translation>
    </message>
    <message>
        <source>The mask contains more than one ?q.</source>
        <translation>La maschera contiene più di un ?q.</translation>
    </message>
    <message>
        <source>The mask names a second word with ?q, but only one wordlist is selected.</source>
        <translation>La maschera indica una seconda parola con ?q, ma è selezionata una sola wordlist.</translation>
    </message>
    <message>
        <source>Two wordlists are selected, but the mask has no ?q for the second one.</source>
        <translation>Sono selezionate due wordlist, ma la maschera non ha un ?q per la seconda.</translation>
    </message>
    <message>
        <source>The increment is not offered in this hybrid mode.</source>
        <translation>L’incremento non è disponibile in questa modalità ibrida.</translation>
    </message>
    <message>
        <source>The ?w and ?q markers are only offered in the hybrid mode (mask + wordlists).</source>
        <translation>I marcatori ?w e ?q sono disponibili solo nella modalità ibrida (maschera + wordlist).</translation>
    </message>
    <message>
        <source>The setting “%1” is not valid for this mode.</source>
        <translation>L’impostazione “%1” non è valida per questa modalità.</translation>
    </message>
    <message>
        <source>The token “%1” is not a setting; settings are written as key=value.</source>
        <translation>Il token “%1” non è un’impostazione; le impostazioni si scrivono come key=value.</translation>
    </message>
    <message>
        <source>The settings line applies only while the words come from the hash file. Clear either the settings line or the wordlist list.</source>
        <translation>La riga delle impostazioni si applica solo finché le parole provengono dal file di hash. Svuota la riga delle impostazioni oppure l’elenco delle wordlist.</translation>
    </message>
    <message>
        <source>The hash type 4000 describes its hashes as a recipe, for example
md5(md5(salt) . pass). Enter it in the hash recipe field.</source>
        <translation>Il tipo di hash 4000 descrive i propri hash come una ricetta, ad esempio
md5(md5(salt) . pass). Inseriscila nel campo della ricetta dell’hash.</translation>
    </message>
    <message>
        <source>The minimum increment length is larger than the maximum.</source>
        <translation>La lunghezza minima dell’incremento è maggiore di quella massima.</translation>
    </message>
    <message>
        <source>The cache path must be an existing directory:
%1</source>
        <translation>Il percorso della cache deve essere una directory esistente:
%1</translation>
    </message>
    <message>
        <source>The increment cannot be combined with the speed-only option.</source>
        <translation>L’incremento non può essere combinato con l’opzione di sola velocità.</translation>
    </message>
    <message>
        <source>Removing cracked hashes is not possible in the association attack mode.</source>
        <translation>La rimozione degli hash decifrati non è possibile nella modalità di attacco Associazione.</translation>
    </message>
</context>
<context>
    <name>SettingsDialog</name>
    <message>
        <source>Settings</source>
        <translation>Impostazioni</translation>
    </message>
    <message>
        <source>hashcat executable</source>
        <translation>eseguibile di hashcat</translation>
    </message>
    <message>
        <source>Not configured</source>
        <translation>Non configurato</translation>
    </message>
    <message>
        <source>hashcat</source>
        <translation>hashcat</translation>
    </message>
    <message>
        <source>Select…</source>
        <translation>Seleziona…</translation>
    </message>
    <message>
        <source>&amp;Terminal</source>
        <translation>&amp;Terminale</translation>
    </message>
    <message>
        <source>&amp;Language</source>
        <translation>&amp;Lingua</translation>
    </message>
    <message>
        <source>&amp;Color scheme</source>
        <translation>Schema di &amp;colori</translation>
    </message>
    <message>
        <source>Not all color schemes are available on all platforms.</source>
        <translation>Non tutti gli schemi di colori sono disponibili su tutte le piattaforme.</translation>
    </message>
    <message>
        <source>&amp;Parameters</source>
        <translation>&amp;Parametri</translation>
    </message>
    <message>
        <source>&amp;Use short parameters</source>
        <translation>&amp;Usa parametri brevi</translation>
    </message>
    <message>
        <source>Session and files</source>
        <translation>Sessione e file</translation>
    </message>
    <message>
        <source>&amp;Restore the last session at startup</source>
        <translation>&amp;Ripristina l’ultima sessione all’avvio</translation>
    </message>
    <message>
        <source>&amp;Session file</source>
        <translation>File di &amp;sessione</translation>
    </message>
    <message>
        <source>The file the last session is stored in</source>
        <translation>Il file in cui è salvata l’ultima sessione</translation>
    </message>
    <message>
        <source>Open folder</source>
        <translation>Apri cartella</translation>
    </message>
    <message>
        <source>Open the folder the session is stored in</source>
        <translation>Apre la cartella in cui è salvata la sessione</translation>
    </message>
    <message>
        <source>Settings &amp;file</source>
        <translation>File delle &amp;impostazioni</translation>
    </message>
    <message>
        <source>The file the settings are stored in</source>
        <translation>Il file in cui sono salvate le impostazioni</translation>
    </message>
    <message>
        <source>Open the folder the settings are stored in</source>
        <translation>Apre la cartella in cui sono salvate le impostazioni</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Generale</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Salva</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annulla</translation>
    </message>
    <message>
        <source>Download from %1</source>
        <translation>Scarica da %1</translation>
    </message>
    <message>
        <source>Use the short option names (-m) instead of the long ones (--hash-type) in the command.</source>
        <translation>Usa i nomi brevi delle opzioni (-m) invece di quelli lunghi (--hash-type) nel comando.</translation>
    </message>
    <message>
        <source>Select a terminal</source>
        <translation>Seleziona un terminale</translation>
    </message>
    <message>
        <source>System default</source>
        <translation>Predefinito di sistema</translation>
    </message>
    <message>
        <source>Light</source>
        <translation>Chiaro</translation>
    </message>
    <message>
        <source>Dark</source>
        <translation>Scuro</translation>
    </message>
    <message>
        <source>The hashcat path “%1” was not found on this computer.</source>
        <translation>Il percorso di hashcat “%1” non è stato trovato su questo computer.</translation>
    </message>
    <message>
        <source>The terminal “%1” is not installed and was not applied.</source>
        <translation>Il terminale “%1” non è installato e non è stato applicato.</translation>
    </message>
    <message>
        <source>The color scheme “%1” is not offered and was not applied.</source>
        <translation>Lo schema di colori “%1” non è disponibile e non è stato applicato.</translation>
    </message>
    <message>
        <source>The language “%1” is not offered and was not applied.</source>
        <translation>La lingua “%1” non è disponibile e non è stata applicata.</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not installed.</source>
        <translation>Il terminale configurato “%1” non è installato.</translation>
    </message>
    <message>
        <source>Executable Files (*.exe *.bin hashcat);;All Files (*)</source>
        <translation>File eseguibili (*.exe *.bin hashcat);;Tutti i file (*)</translation>
    </message>
    <message>
        <source>No hashcat executable is configured; hashcat cannot be started.</source>
        <translation>Nessun eseguibile di hashcat è configurato; non è possibile avviare hashcat.</translation>
    </message>
    <message>
        <source>The configured hashcat executable was not found on this computer.</source>
        <translation>L’eseguibile di hashcat configurato non è stato trovato su questo computer.</translation>
    </message>
    <message>
        <source>Select the terminal hashcat is started in.</source>
        <translation>Seleziona il terminale in cui viene avviato hashcat.</translation>
    </message>
    <message>
        <source>Select the hashcat executable</source>
        <translation>Seleziona l’eseguibile di hashcat</translation>
    </message>
    <message>
        <source>Invalid file</source>
        <translation>File non valido</translation>
    </message>
    <message>
        <source>The selected file is not an executable.</source>
        <translation>Il file selezionato non è un eseguibile.</translation>
    </message>
    <message>
        <source>Restart required</source>
        <translation>Riavvio necessario</translation>
    </message>
    <message>
        <source>Restart hashcat-gui to apply the new language.</source>
        <translation>Riavvia hashcat-gui per applicare la nuova lingua.</translation>
    </message>
</context>
<context>
    <name>StateFile</name>
    <message>
        <source>The file is not valid JSON: %1</source>
        <translation>Il file non è un JSON valido: %1</translation>
    </message>
    <message>
        <source>The file does not contain a JSON object.</source>
        <translation>Il file non contiene un oggetto JSON.</translation>
    </message>
    <message>
        <source>The file does not contain any sections.</source>
        <translation>Il file non contiene alcuna sezione.</translation>
    </message>
    <message>
        <source>Could not open “%1” for writing: %2</source>
        <translation>Impossibile aprire “%1” in scrittura: %2</translation>
    </message>
    <message>
        <source>Could not write to “%1”: %2</source>
        <translation>Impossibile scrivere in “%1”: %2</translation>
    </message>
    <message>
        <source>Could not open “%1” for reading: %2</source>
        <translation>Impossibile aprire “%1” in lettura: %2</translation>
    </message>
    <message>
        <source>The file does not contain a “%1” section.</source>
        <translation>Il file non contiene una sezione “%1”.</translation>
    </message>
    <message>
        <source>The file does not contain a “%1” section; it holds %2.</source>
        <translation>Il file non contiene una sezione “%1”; contiene %2.</translation>
    </message>
    <message>
        <source>The “%1” section does not contain any values.</source>
        <translation>La sezione “%1” non contiene alcun valore.</translation>
    </message>
    <message>
        <source> and </source>
        <translation> e </translation>
    </message>
    <message>
        <source> and %1 more</source>
        <translation> e altri %1</translation>
    </message>
    <message>
        <source>The file names values this version does not know, so they were not applied: %1.</source>
        <translation>Il file cita valori che questa versione non conosce, quindi non sono stati applicati: %1.</translation>
    </message>
    <message>
        <source>The file holds values of the wrong type, so they were not applied: %1.</source>
        <translation>Il file contiene valori di tipo errato, quindi non sono stati applicati: %1.</translation>
    </message>
</context>
<context>
    <name>UpdateChecker</name>
    <message>
        <source>GitHub answered with HTTP status %1: %2</source>
        <translation>GitHub ha risposto con lo stato HTTP %1: %2</translation>
    </message>
    <message>
        <source>GitHub answered with HTTP status %1.</source>
        <translation>GitHub ha risposto con lo stato HTTP %1.</translation>
    </message>
    <message>
        <source>The release information is not valid JSON.</source>
        <translation>Le informazioni sulla release non sono un JSON valido.</translation>
    </message>
    <message>
        <source>The release information does not name a version.</source>
        <translation>Le informazioni sulla release non indicano alcuna versione.</translation>
    </message>
    <message>
        <source>The release version %1 is not a valid version number.</source>
        <translation>La versione della release %1 non è un numero di versione valido.</translation>
    </message>
    <message>
        <source>The installed version %1 is not a valid version number.</source>
        <translation>La versione installata %1 non è un numero di versione valido.</translation>
    </message>
</context>
</TS>
