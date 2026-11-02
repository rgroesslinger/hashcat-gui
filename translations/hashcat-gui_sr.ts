<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="sr" sourcelanguage="en">
<context>
    <name>AboutDialog</name>
    <message>
        <source>About</source>
        <translation>O programu</translation>
    </message>
    <message>
        <source>Graphical user interface for hashcat, the password recovery utility</source>
        <translation>Grafički korisnički interfejs za hashcat, alatku za oporavak lozinki</translation>
    </message>
    <message>
        <source>Version information</source>
        <translation>Informacije o verziji</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Nepoznato</translation>
    </message>
    <message>
        <source>Updates</source>
        <translation>Ažuriranja</translation>
    </message>
    <message>
        <source>Checking for updates…</source>
        <translation>Provera ažuriranja…</translation>
    </message>
    <message>
        <source>System information</source>
        <translation>Informacije o sistemu</translation>
    </message>
    <message>
        <source>Operating system</source>
        <translation>Operativni sistem</translation>
    </message>
    <message>
        <source>Links</source>
        <translation>Veze</translation>
    </message>
    <message>
        <source>License</source>
        <translation>Licenca</translation>
    </message>
    <message>
        <source>Copy the application, toolkit, system and hashcat versions to the clipboard</source>
        <translation>Kopiraj verzije aplikacije, alata, sistema i hashcat-a u privremenu memoriju</translation>
    </message>
    <message>
        <source>Copy details</source>
        <translation>Kopiraj detalje</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Copied to the clipboard</source>
        <translation>Kopirano u privremenu memoriju</translation>
    </message>
    <message>
        <source>Project page</source>
        <translation>Stranica projekta</translation>
    </message>
    <message>
        <source>Report an issue</source>
        <translation>Prijavi problem</translation>
    </message>
    <message>
        <source>hashcat</source>
        <translation>hashcat</translation>
    </message>
    <message>
        <source>hashcat FAQ</source>
        <translation>hashcat FAQ</translation>
    </message>
    <message>
        <source>hashcat-gui is licensed under the %1.</source>
        <translation>Licenca programa hashcat-gui: %1.</translation>
    </message>
    <message>
        <source>GNU GPL v3.0 or later</source>
        <translation>GNU GPL v3.0 ili novija</translation>
    </message>
    <message>
        <source>Not configured</source>
        <translation>Nije konfigurisano</translation>
    </message>
    <message>
        <source>Error: %1</source>
        <translation>Greška: %1</translation>
    </message>
    <message>
        <source>A newer version is available: %1.</source>
        <translation>Dostupna je novija verzija: %1.</translation>
    </message>
    <message>
        <source>Release page</source>
        <translation>Stranica izdanja</translation>
    </message>
    <message>
        <source>No updates available.</source>
        <translation>Nema dostupnih ažuriranja.</translation>
    </message>
    <message>
        <source>Could not check for updates.</source>
        <translation>Nije bilo moguće proveriti ažuriranja.</translation>
    </message>
    <message>
        <source>Could not check for updates: %1</source>
        <translation>Nije bilo moguće proveriti ažuriranja: %1</translation>
    </message>
</context>
<context>
    <name>HashcatInfoParser</name>
    <message>
        <source>Invalid JSON returned from hashcat.</source>
        <translation>hashcat je vratio nevažeći JSON.</translation>
    </message>
    <message>
        <source>hashcat did not report any hash types.</source>
        <translation>hashcat nije prijavio nijednu vrstu hash-a.</translation>
    </message>
</context>
<context>
    <name>HelperUtils</name>
    <message>
        <source>The hashcat path is not configured.</source>
        <translation>Putanja do hashcat-a nije konfigurisana.</translation>
    </message>
    <message>
        <source>Failed to start hashcat
%1</source>
        <translation>Pokretanje hashcat-a nije uspelo
%1</translation>
    </message>
    <message>
        <source>hashcat timed out
%1</source>
        <translation>hashcat je dostigao vremenski limit
%1</translation>
    </message>
    <message>
        <source>No supported terminal was found on this system. hashcat-gui needs one to show the hashcat output.</source>
        <translation>Na ovom sistemu nije pronađen podržani terminal. hashcat-gui ga zahteva da bi prikazao izlaz programa hashcat.</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <source>Hash file or hash:</source>
        <translation>Datoteka sa hash-ovima ili hash:</translation>
    </message>
    <message>
        <source>The hash, a file of hashes, or a .hc22000/.hccapx WPA capture.</source>
        <translation>Hash, datoteka sa hash-ovima ili WPA snimak .hc22000/.hccapx.</translation>
    </message>
    <message>
        <source>Choose a file of hashes.</source>
        <translation>Izaberi datoteku sa hash-ovima.</translation>
    </message>
    <message>
        <source>Salt is hex</source>
        <translation>Salt je heksadecimalan</translation>
    </message>
    <message>
        <source>Rewrite the hash file without cracked hashes, including potfile-known ones; the potfile is untouched. Refused in attack mode 9.</source>
        <translation>Ponovo upiši datoteku sa hash-ovima bez provaljenih hash-ova, uključujući one koje potfile već poznaje; sam potfile ostaje neizmenjen. Odbija se u režimu napada 9.</translation>
    </message>
    <message>
        <source>Remove hash from hash list once it is cracked</source>
        <translation>Ukloni hash sa liste hash-ova kada bude provaljen</translation>
    </message>
    <message>
        <source>Ignore the username before the mode’s separator (usually “:”), so accounts sharing a hash are cracked once.</source>
        <translation>Zanemari korisničko ime pre separatora režima (obično „:“), tako da se nalozi sa istim hash-om provaljuju samo jednom.</translation>
    </message>
    <message>
        <source>Hash recipe, e.g. “md5(md5(salt) . pass)”. Wrap it in the shell’s outer quotes and put any inner strings in the other kind of quotes; PowerShell needs a backtick before $.</source>
        <translation>Recept za hash, npr. „md5(md5(salt) . pass)“. Stavite ga u spoljne navodnike shell-a, a unutrašnje niske u navodnike druge vrste; PowerShell zahteva obrnuti apostrof pre $.</translation>
    </message>
    <message>
        <source>Move item up</source>
        <translation>Pomeri stavku gore</translation>
    </message>
    <message>
        <source>Move selected wordlist up</source>
        <translation>Pomeri izabranu listu reči gore</translation>
    </message>
    <message>
        <source>Move item down</source>
        <translation>Pomeri stavku dole</translation>
    </message>
    <message>
        <source>Move selected wordlist down</source>
        <translation>Pomeri izabranu listu reči dole</translation>
    </message>
    <message>
        <source>Add one or more wordlist files. Each checked row is passed to hashcat.</source>
        <translation>Dodaj jednu ili više datoteka sa listama reči. Svaki označeni red se prosleđuje programu hashcat.</translation>
    </message>
    <message>
        <source>Remove the selected wordlists.</source>
        <translation>Ukloni izabrane liste reči.</translation>
    </message>
    <message>
        <source>Remove wordlists</source>
        <translation>Ukloni liste reči</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Ukloni</translation>
    </message>
    <message>
        <source>Wordlist is hex</source>
        <translation>Lista reči je heksadecimalna</translation>
    </message>
    <message>
        <source>Encoding from:</source>
        <translation>Kodiranje iz:</translation>
    </message>
    <message>
        <source>Encoding to:</source>
        <translation>Kodiranje u:</translation>
    </message>
    <message>
        <source>Space-separated key=value settings for attack mode 9, e.g. phases=words,rules or rules.rulefile=…. Only used without a wordlist.</source>
        <translation>Podešavanja key=value razdvojena razmacima za režim napada 9, npr. phases=words,rules ili rules.rulefile=…. Koriste se samo bez liste reči.</translation>
    </message>
    <message>
        <source>A ruleset file, a folder with Grammar/grammar.txt, or a bundled name. Unchecked rows are skipped; none runs default-passwords.</source>
        <translation>Datoteka skupa pravila, fascikla sa Grammar/grammar.txt ili ugrađeno ime. Neoznačeni redovi se preskaču; ako nijedan nije označen, pokreće se default-passwords.</translation>
    </message>
    <message>
        <source>Add one or more PCFG ruleset files.</source>
        <translation>Dodaj jednu ili više datoteka PCFG skupova pravila.</translation>
    </message>
    <message>
        <source>Add PCFG rulesets</source>
        <translation>Dodaj PCFG skupove pravila</translation>
    </message>
    <message>
        <source>Remove the selected rulesets.</source>
        <translation>Ukloni izabrane skupove pravila.</translation>
    </message>
    <message>
        <source>Remove rulesets</source>
        <translation>Ukloni skupove pravila</translation>
    </message>
    <message>
        <source>Move selected ruleset up</source>
        <translation>Pomeri izabrani skup pravila gore</translation>
    </message>
    <message>
        <source>Move selected ruleset down</source>
        <translation>Pomeri izabrani skup pravila dole</translation>
    </message>
    <message>
        <source>One plain number per ruleset, colon-separated, e.g. 2:1; the count must match the number of rulesets.</source>
        <translation>Po jedan običan broj za svaki skup pravila, razdvojeni dvotačkom, npr. 2:1; broj vrednosti mora da odgovara broju skupova pravila.</translation>
    </message>
    <message>
        <source>Maximum candidate length the kernel builds, in 4-byte words: 0 (auto), 16 or 32.</source>
        <translation>Maksimalna dužina kandidata koju kernel gradi, u 4-bajtnim rečima: 0 (auto), 16 ili 32.</translation>
    </message>
    <message>
        <source>1 steps to the next base word where the order allows instead of unranking it; the candidates are identical, only the fast path is off.</source>
        <translation>1 prelazi na sledeću osnovnu reč gde redosled to dozvoljava, umesto da je izračuna pomoću unranking-a; kandidati su identični, isključuje se samo brza putanja.</translation>
    </message>
    <message>
        <source>Only enumerate candidates this mask admits; excludes omen=1, and hintaccount unless hint words come from hintwords or hintfile.</source>
        <translation>Nabroj samo kandidate koje ova maska dopušta; isključuje omen=1 i hintaccount osim ako reči nagoveštaja ne potiču iz hintwords ili hintfile.</translation>
    </message>
    <message>
        <source>Space-separated key=value settings: omen, cache, lookup, pwmin, pwmax, hintaccount, hintwords, hintfile, hintrank, hintrepeat. Rulesets go first.</source>
        <translation>Podešavanja key=value razdvojena razmacima: omen, cache, lookup, pwmin, pwmax, hintaccount, hintwords, hintfile, hintrank, hintrepeat. Skupovi pravila idu prvi.</translation>
    </message>
    <message>
        <source>Add one or more table files.</source>
        <translation>Dodaj jednu ili više datoteka tabela.</translation>
    </message>
    <message>
        <source>Remove the selected tables.</source>
        <translation>Ukloni izabrane tabele.</translation>
    </message>
    <message>
        <source>Remove tables</source>
        <translation>Ukloni tabele</translation>
    </message>
    <message>
        <source>1 (default) keeps a token unchanged as a choice, 0 drops it so the table only yields variations. identity=0 excludes single=1.</source>
        <translation>1 (podrazumevano) zadržava token nepromenjen kao jedan izbor, 0 ga odbacuje pa tabela daje samo varijacije. identity=0 isključuje single=1.</translation>
    </message>
    <message>
        <source>1 reads the wordlist as patterns: no word is returned unchanged and unmatched words are dropped. 0 (default) keeps them.</source>
        <translation>1 čita listu reči kao obrasce: nijedna reč se ne vraća nepromenjena, a reči bez podudaranja se odbacuju. 0 (podrazumevano) ih zadržava.</translation>
    </message>
    <message>
        <source>single=1: one substitution per candidate instead of the cross product; fewer candidates, more host work on fast hashes.</source>
        <translation>single=1: jedna zamena po kandidatu umesto Dekartovog proizvoda; manje kandidata, više posla za host kod brzih hash-ova.</translation>
    </message>
    <message>
        <source>Keep only the first N replacements of each source word; 0 means no limit. Applies to single=1 too.</source>
        <translation>Zadrži samo prvih N zamena svake izvorne reči; 0 znači bez ograničenja. Važi i za single=1.</translation>
    </message>
    <message>
        <source>How much of a word’s cross product the host enumerates before giving up; 0 means no limit.</source>
        <translation>Koliki deo Dekartovog proizvoda jedne reči host nabraja pre nego što odustane; 0 znači bez ograničenja.</translation>
    </message>
    <message>
        <source>Key=value settings separated by spaces; everything after the first one is a setting, so tables come first.</source>
        <translation>Podešavanja key=value razdvojena razmacima; sve posle prvog je podešavanje, pa tabele idu prve.</translation>
    </message>
    <message>
        <source>Feed name or path. A slash means a path; otherwise hashcat tries feed_&lt;name&gt;, rust_&lt;name&gt;, then &lt;name&gt; in its feeds directory.</source>
        <translation>Ime ili putanja feed-a. Kosa crta znači putanju; u suprotnom hashcat pokušava feed_&lt;name&gt;, rust_&lt;name&gt;, pa &lt;name&gt; u svom direktorijumu feed-ova.</translation>
    </message>
    <message>
        <source>Browse for a feed plugin instead of naming it.</source>
        <translation>Potraži feed plugin umesto navođenja njegovog imena.</translation>
    </message>
    <message>
        <source>Browse for a feed plugin</source>
        <translation>Potraži feed plugin</translation>
    </message>
    <message>
        <source>Argument passed to the feed; meaning and count depend on the feed (the wordlist feed takes wordlist paths).</source>
        <translation>Argument koji se prosleđuje feed-u; značenje i broj zavise od feed-a (feed wordlist prima putanje lista reči).</translation>
    </message>
    <message>
        <source>Add an argument for the feed, in order.</source>
        <translation>Dodaj argument za feed, redom.</translation>
    </message>
    <message>
        <source>Remove the selected feed arguments.</source>
        <translation>Ukloni izabrane argumente feed-a.</translation>
    </message>
    <message>
        <source>Remove feed arguments</source>
        <translation>Ukloni argumente feed-a</translation>
    </message>
    <message>
        <source>Move selected feed argument up</source>
        <translation>Pomeri izabrani argument feed-a gore</translation>
    </message>
    <message>
        <source>Move selected feed argument down</source>
        <translation>Pomeri izabrani argument feed-a dole</translation>
    </message>
    <message>
        <source>Read rules from the files below instead of generating them.</source>
        <translation>Čitaj pravila iz datoteka ispod umesto da ih generišeš.</translation>
    </message>
    <message>
        <source>Rules file 1:</source>
        <translation>Datoteka sa pravilima 1:</translation>
    </message>
    <message>
        <source>Choose the rules file for slot 1.</source>
        <translation>Izaberi datoteku sa pravilima za slot 1.</translation>
    </message>
    <message>
        <source>Open rules file 1</source>
        <translation>Otvori datoteku sa pravilima 1</translation>
    </message>
    <message>
        <source>Rules file 2:</source>
        <translation>Datoteka sa pravilima 2:</translation>
    </message>
    <message>
        <source>Choose the rules file for slot 2.</source>
        <translation>Izaberi datoteku sa pravilima za slot 2.</translation>
    </message>
    <message>
        <source>Open rules file 2</source>
        <translation>Otvori datoteku sa pravilima 2</translation>
    </message>
    <message>
        <source>Rules file 3:</source>
        <translation>Datoteka sa pravilima 3:</translation>
    </message>
    <message>
        <source>Choose the rules file for slot 3.</source>
        <translation>Izaberi datoteku sa pravilima za slot 3.</translation>
    </message>
    <message>
        <source>Open rules file 3</source>
        <translation>Otvori datoteku sa pravilima 3</translation>
    </message>
    <message>
        <source>Number of random rules hashcat generates (-g). Only used with Generate rules; no rules file is read.</source>
        <translation>Broj nasumičnih pravila koje hashcat generiše (-g). Koristi se samo uz „Generiši pravila“; datoteka sa pravilima se ne čita.</translation>
    </message>
    <message>
        <source>Treat every -r file as one list: rule counts add instead of multiplying.</source>
        <translation>Tretiraj svaku -r datoteku kao jednu listu: brojevi pravila se sabiraju umesto da se množe.</translation>
    </message>
    <message>
        <source>Wordlist rules</source>
        <translation>Pravila liste reči</translation>
    </message>
    <message>
        <source>Left rule:</source>
        <translation>Levo pravilo:</translation>
    </message>
    <message>
        <source>Right rule:</source>
        <translation>Desno pravilo:</translation>
    </message>
    <message>
        <source>Interpret the mask and custom charsets as hexadecimal bytes.</source>
        <translation>Tumači masku i prilagođene skupove znakova kao heksadecimalne bajtove.</translation>
    </message>
    <message>
        <source>Charset is hex</source>
        <translation>Skup znakova je heksadecimalan</translation>
    </message>
    <message>
        <source>Ctrl+Shift+I</source>
        <translation>Ctrl+Shift+I</translation>
    </message>
    <message>
        <source>Increment mask length</source>
        <translation>Povećavaj dužinu maske</translation>
    </message>
    <message>
        <source>Min:</source>
        <translation>Min:</translation>
    </message>
    <message>
        <source>Max:</source>
        <translation>Maks:</translation>
    </message>
    <message>
        <source>Use hashcat’s optimized kernels (-O); they cap the maximum password length.</source>
        <translation>Koristi hashcat-ove optimizovane kernele (-O); oni ograničavaju maksimalnu dužinu lozinke.</translation>
    </message>
    <message>
        <source>Pin hashcat to these CPU devices, separated by commas. Leave empty to let hashcat decide.</source>
        <translation>Veži hashcat za ove CPU uređaje, odvojene zarezima. Ostavite prazno da hashcat odluči.</translation>
    </message>
    <message>
        <source>Devices to use, separated by commas. Leave empty for all devices.</source>
        <translation>Uređaji koji se koriste, odvojeni zarezima. Ostavite prazno za sve uređaje.</translation>
    </message>
    <message>
        <source>Abort above</source>
        <translation>Prekini iznad</translation>
    </message>
    <message>
        <source>Outfile</source>
        <translation>Izlazna datoteka</translation>
    </message>
    <message>
        <source>Save…</source>
        <translation>Sačuvaj…</translation>
    </message>
    <message>
        <source>Write the outfile as JSON. The outfile format is ignored.</source>
        <translation>Upiši izlaznu datoteku kao JSON. Format izlazne datoteke se zanemaruje.</translation>
    </message>
    <message>
        <source>Write JSON</source>
        <translation>Upiši JSON</translation>
    </message>
    <message>
        <source>Do not write recovered hashes to the potfile.</source>
        <translation>Ne upisuj pronađene hash-ove u potfile.</translation>
    </message>
    <message>
        <source>Load Example Session</source>
        <translation>Učitaj primer sesije</translation>
    </message>
    <message>
        <source>Reset Session</source>
        <translation>Resetuj sesiju</translation>
    </message>
    <message>
        <source>Mode:</source>
        <translation>Režim:</translation>
    </message>
    <message>
        <source>Attack</source>
        <translation>Napad</translation>
    </message>
    <message>
        <source>Open…</source>
        <translation>Otvori…</translation>
    </message>
    <message>
        <source>Assume the salt is given in hexadecimal.</source>
        <translation>Pretpostavi da je salt zadat heksadecimalno.</translation>
    </message>
    <message>
        <source>Attack mode. Each choice reveals its own options below.</source>
        <translation>Režim napada. Svaki izbor prikazuje sopstvene opcije ispod.</translation>
    </message>
    <message>
        <source>Hash type:</source>
        <translation>Vrsta hash-a:</translation>
    </message>
    <message>
        <source>Hash recipe:</source>
        <translation>Recept za hash:</translation>
    </message>
    <message>
        <source>Add files…</source>
        <translation>Dodaj datoteke…</translation>
    </message>
    <message>
        <source>hashcat reads every file directly inside the folder as one wordlist.</source>
        <translation>hashcat čita svaku datoteku koja se nalazi direktno u fascikli kao jednu listu reči.</translation>
    </message>
    <message>
        <source>Add folder…</source>
        <translation>Dodaj fasciklu…</translation>
    </message>
    <message>
        <source>Only checked rows are used. A folder is one wordlist: hashcat reads the files directly inside it.</source>
        <translation>Koriste se samo označeni redovi. Fascikla je jedna lista reči: hashcat čita datoteke koje se nalaze direktno u njoj.</translation>
    </message>
    <message>
        <source>Assume words in the wordlist are given in hexadecimal.</source>
        <translation>Pretpostavi da su reči u listi reči zadate heksadecimalno.</translation>
    </message>
    <message>
        <source>Settings (no wordlist):</source>
        <translation>Podešavanja (bez liste reči):</translation>
    </message>
    <message>
        <source>PCFG</source>
        <translation>PCFG</translation>
    </message>
    <message>
        <source>A ruleset folder needs a Grammar/grammar.txt.</source>
        <translation>Fascikla skupa pravila zahteva Grammar/grammar.txt.</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Scale:</translation>
    </message>
    <message>
        <source>Quantization steps per bit (1–64)</source>
        <translation>Koraka kvantizacije po bitu (1–64)</translation>
    </message>
    <message>
        <source>Costmax:</source>
        <translation>Costmax:</translation>
    </message>
    <message>
        <source>Highest cost level to enumerate (1–64)</source>
        <translation>Najviši nivo troška koji se nabraja (1–64)</translation>
    </message>
    <message>
        <source>Weights:</source>
        <translation>Weights:</translation>
    </message>
    <message>
        <source>Threads:</source>
        <translation>Niti:</translation>
    </message>
    <message>
        <source>Cores that generate base words (0–16); 0 runs on the calling thread.</source>
        <translation>Jezgra koja generišu osnovne reči (0–16); 0 radi u niti koja poziva.</translation>
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
        <translation>Broj bitova koje jedna unutrašnja petlja može da obuhvati (0–27)</translation>
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
        <translation>1.0–64.0; decimalni separator je tačka, a ne separator iz regionalnih podešavanja.</translation>
    </message>
    <message>
        <source>Walk:</source>
        <translation>Walk:</translation>
    </message>
    <message>
        <source>Mask:</source>
        <translation>Maska:</translation>
    </message>
    <message>
        <source>Extra settings (key=value):</source>
        <translation>Dodatna podešavanja (key=value):</translation>
    </message>
    <message>
        <source>Table</source>
        <translation>Tabela</translation>
    </message>
    <message>
        <source>Every checked row after the wordlist is a table.</source>
        <translation>Svaki označeni red posle liste reči je tabela.</translation>
    </message>
    <message>
        <source>Add tables…</source>
        <translation>Dodaj tabele…</translation>
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
        <translation>Generički feed</translation>
    </message>
    <message>
        <source>Feed:</source>
        <translation>Feed:</translation>
    </message>
    <message>
        <source>wordlist, stdin, random, association, pcfg, table, hybrid, mask, or a feed path</source>
        <translation>wordlist, stdin, random, association, pcfg, table, hybrid, mask ili putanja feed-a</translation>
    </message>
    <message>
        <source>Browse…</source>
        <translation>Potraži…</translation>
    </message>
    <message>
        <source>Argument:</source>
        <translation>Argument:</translation>
    </message>
    <message>
        <source>Add</source>
        <translation>Dodaj</translation>
    </message>
    <message>
        <source>Arguments the feed reads, in order; only checked rows are used.</source>
        <translation>Argumenti koje feed čita, redom; koriste se samo označeni redovi.</translation>
    </message>
    <message>
        <source>Rules</source>
        <translation>Pravila</translation>
    </message>
    <message>
        <source>Use rules files:</source>
        <translation>Koristi datoteke sa pravilima:</translation>
    </message>
    <message>
        <source>Enable rules file slot 1 for this run.</source>
        <translation>Uključi slot 1 datoteke sa pravilima za ovo pokretanje.</translation>
    </message>
    <message>
        <source>Rules file for slot 1; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>Datoteka sa pravilima za slot 1; primenjuje se na svaku reč. Ostavite prazno da se slot ne koristi.</translation>
    </message>
    <message>
        <source>Enable rules file slot 2 for this run.</source>
        <translation>Uključi slot 2 datoteke sa pravilima za ovo pokretanje.</translation>
    </message>
    <message>
        <source>Rules file for slot 2; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>Datoteka sa pravilima za slot 2; primenjuje se na svaku reč. Ostavite prazno da se slot ne koristi.</translation>
    </message>
    <message>
        <source>Enable rules file slot 3 for this run.</source>
        <translation>Uključi slot 3 datoteke sa pravilima za ovo pokretanje.</translation>
    </message>
    <message>
        <source>Rules file for slot 3; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>Datoteka sa pravilima za slot 3; primenjuje se na svaku reč. Ostavite prazno da se slot ne koristi.</translation>
    </message>
    <message>
        <source>Generate the rules automatically instead of reading them from a file.</source>
        <translation>Generiši pravila automatski umesto da se čitaju iz datoteke.</translation>
    </message>
    <message>
        <source>Generate rules:</source>
        <translation>Generiši pravila:</translation>
    </message>
    <message>
        <source>Join -r files into one list</source>
        <translation>Spoji -r datoteke u jednu listu</translation>
    </message>
    <message>
        <source>Single rule applied to each word from the left wordlist.</source>
        <translation>Jedno pravilo koje se primenjuje na svaku reč iz leve liste reči.</translation>
    </message>
    <message>
        <source>Single rule applied to each word from the right wordlist.</source>
        <translation>Jedno pravilo koje se primenjuje na svaku reč iz desne liste reči.</translation>
    </message>
    <message>
        <source>Mask</source>
        <translation>Maska</translation>
    </message>
    <message>
        <source>Built-in charsets:</source>
        <translation>Ugrađeni skupovi znakova:</translation>
    </message>
    <message>
        <source>Custom charsets</source>
        <translation>Prilagođeni skupovi znakova</translation>
    </message>
    <message>
        <source>Charset 1:</source>
        <translation>Skup znakova 1:</translation>
    </message>
    <message>
        <source>Charset 2:</source>
        <translation>Skup znakova 2:</translation>
    </message>
    <message>
        <source>Charset 3:</source>
        <translation>Skup znakova 3:</translation>
    </message>
    <message>
        <source>Charset 4:</source>
        <translation>Skup znakova 4:</translation>
    </message>
    <message>
        <source>Session and limits</source>
        <translation>Sesija i ograničenja</translation>
    </message>
    <message>
        <source>Session name:</source>
        <translation>Naziv sesije:</translation>
    </message>
    <message>
        <source>Name hashcat uses for its restore file and status output.</source>
        <translation>Naziv koji hashcat koristi za svoju datoteku za nastavak i izlaz statusa.</translation>
    </message>
    <message>
        <source>Runtime limit:</source>
        <translation>Ograničenje trajanja:</translation>
    </message>
    <message>
        <source>unlimited</source>
        <translation>neograničeno</translation>
    </message>
    <message>
        <source>Skip:</source>
        <translation>Preskoči:</translation>
    </message>
    <message>
        <source>Limit:</source>
        <translation>Ograničenje:</translation>
    </message>
    <message>
        <source>Keep updating the run status (speed, progress, ETA) instead of printing one final line.</source>
        <translation>Neprekidno ažuriraj status pokretanja (brzina, napredak, procenjeno vreme) umesto ispisivanja jedne završne linije.</translation>
    </message>
    <message>
        <source>Update status automatically</source>
        <translation>Automatski ažuriraj status</translation>
    </message>
    <message>
        <source>Seconds between status updates; only used when automatic status is enabled.</source>
        <translation>Sekunde između ažuriranja statusa; koristi se samo kada je automatski status omogućen.</translation>
    </message>
    <message>
        <source>Ignore warnings and start hashcat anyway. Use with care.</source>
        <translation>Zanemari upozorenja i ipak pokreni hashcat. Koristite oprezno.</translation>
    </message>
    <message>
        <source>Force (ignore warnings)</source>
        <translation>Prisili (zanemari upozorenja)</translation>
    </message>
    <message>
        <source>Performance</source>
        <translation>Performanse</translation>
    </message>
    <message>
        <source>Cache path:</source>
        <translation>Putanja keša:</translation>
    </message>
    <message>
        <source>Directory hashcat keeps its cache in. Must exist.</source>
        <translation>Direktorijum u kome hashcat čuva svoj keš. Mora da postoji.</translation>
    </message>
    <message>
        <source>Choose the cache directory.</source>
        <translation>Izaberi direktorijum keša.</translation>
    </message>
    <message>
        <source>Choose cache directory</source>
        <translation>Izaberi direktorijum keša</translation>
    </message>
    <message>
        <source>Resources</source>
        <translation>Resursi</translation>
    </message>
    <message>
        <source>CPU affinity:</source>
        <translation>CPU afinitet:</translation>
    </message>
    <message>
        <source>Backend devices:</source>
        <translation>Backend uređaji:</translation>
    </message>
    <message>
        <source>Device types:</source>
        <translation>Vrste uređaja:</translation>
    </message>
    <message>
        <source>Abort when any backend device exceeds this temperature in °C; 0 disables it.</source>
        <translation>Prekini kada bilo koji backend uređaj pređe ovu temperaturu u °C; 0 isključuje ograničenje.</translation>
    </message>
    <message>
        <source>Write recovered hashes to the file below, in the selected formats.</source>
        <translation>Upiši pronađene hash-ove u datoteku ispod, u izabranim formatima.</translation>
    </message>
    <message>
        <source>Write recovered hashes to file:</source>
        <translation>Upiši pronađene hash-ove u datoteku:</translation>
    </message>
    <message>
        <source>The file the recovered hashes are written to. Format variables:

&lt;hash&gt; = current hash filename or hash
&lt;unixtime&gt; = current Unix timestamp</source>
        <translation>Datoteka u koju se upisuju pronađeni hash-ovi. Promenljive formata:

&lt;hash&gt; = ime trenutne datoteke sa hash-ovima ili hash
&lt;unixtime&gt; = trenutna Unix vremenska oznaka</translation>
    </message>
    <message>
        <source>Choose the outfile path.</source>
        <translation>Izaberi putanju izlazne datoteke.</translation>
    </message>
    <message>
        <source>Choose outfile</source>
        <translation>Izaberi izlaznu datoteku</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Format:</translation>
    </message>
    <message>
        <source>Do not write the potfile</source>
        <translation>Ne upisuj potfile</translation>
    </message>
    <message>
        <source>Run hashcat in a terminal with these options.</source>
        <translation>Pokreni hashcat u terminalu sa ovim opcijama.</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>Pokreni</translation>
    </message>
    <message>
        <source>Copy command to clipboard</source>
        <translation>Kopiraj naredbu u privremenu memoriju</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Opšte</translation>
    </message>
    <message>
        <source>Ignore username in hash file</source>
        <translation>Zanemari korisničko ime u datoteci sa hash-ovima</translation>
    </message>
    <message>
        <source>Wordlists</source>
        <translation>Liste reči</translation>
    </message>
    <message>
        <source>Increment the mask length from the minimum to the maximum.</source>
        <translation>Povećavaj dužinu maske od minimuma do maksimuma.</translation>
    </message>
    <message>
        <source>Minimum mask length; 0 uses hashcat’s default.</source>
        <translation>Minimalna dužina maske; 0 koristi podrazumevanu vrednost programa hashcat.</translation>
    </message>
    <message>
        <source>Maximum mask length; 0 uses hashcat’s default.</source>
        <translation>Maksimalna dužina maske; 0 koristi podrazumevanu vrednost programa hashcat.</translation>
    </message>
    <message>
        <source>Abort the session after this many seconds; 0 runs until hashcat finishes.</source>
        <translation>Prekini sesiju posle ovoliko sekundi; 0 radi dok hashcat ne završi.</translation>
    </message>
    <message>
        <source>&#xa0;s</source>
        <translation>&#xa0;s</translation>
    </message>
    <message>
        <source>Skip this many candidates from the start.</source>
        <translation>Preskoči ovoliko kandidata od početka.</translation>
    </message>
    <message>
        <source>Only test this many candidates after the skipped ones.</source>
        <translation>Testiraj samo ovoliko kandidata posle preskočenih.</translation>
    </message>
    <message>
        <source>Enable optimized kernels (limits password length)</source>
        <translation>Omogući optimizovane kernele (ograničava dužinu lozinke)</translation>
    </message>
    <message>
        <source>Print only the attack’s speed, then quit; no cracking.</source>
        <translation>Ispiši samo brzinu napada, a zatim izađi; bez provaljivanja.</translation>
    </message>
    <message>
        <source>Return expected speed of the attack, then quit</source>
        <translation>Ispiši očekivanu brzinu napada i izađi</translation>
    </message>
    <message>
        <source>&#xa0;°C</source>
        <translation>&#xa0;°C</translation>
    </message>
    <message>
        <source>Copy command</source>
        <translation>Kopiraj naredbu</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>&amp;Pomoć</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Alati</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Datoteka</translation>
    </message>
    <message>
        <source>Ctrl+Q</source>
        <translation>Ctrl+Q</translation>
    </message>
    <message>
        <source>Settings…</source>
        <translation>Podešavanja…</translation>
    </message>
    <message>
        <source>Ctrl+,</source>
        <translation>Ctrl+,</translation>
    </message>
    <message>
        <source>Import Settings…</source>
        <translation>Uvezi podešavanja…</translation>
    </message>
    <message>
        <source>Ctrl+Shift+O</source>
        <translation>Ctrl+Shift+O</translation>
    </message>
    <message>
        <source>Export Settings…</source>
        <translation>Izvezi podešavanja…</translation>
    </message>
    <message>
        <source>Ctrl+Shift+S</source>
        <translation>Ctrl+Shift+S</translation>
    </message>
    <message>
        <source>Export Session…</source>
        <translation>Izvezi sesiju…</translation>
    </message>
    <message>
        <source>Ctrl+Shift+E</source>
        <translation>Ctrl+Shift+E</translation>
    </message>
    <message>
        <source>Import Session…</source>
        <translation>Uvezi sesiju…</translation>
    </message>
    <message>
        <source>About</source>
        <translation>O programu</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation>Izađi</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>O Qt-u</translation>
    </message>
    <message>
        <source>JSON Files (*.json)</source>
        <translation>JSON datoteke (*.json)</translation>
    </message>
    <message>
        <source>The session file could not be read, so the session was not restored. The file was left in place.

%1</source>
        <translation>Datoteka sesije nije mogla da se pročita, pa sesija nije vraćena. Datoteka je ostavljena kakva jeste.

%1</translation>
    </message>
    <message>
        <source>Save failed</source>
        <translation>Čuvanje nije uspelo</translation>
    </message>
    <message>
        <source>Outfile format to use, separated by commas.
Default: 1,2</source>
        <translation>Format izlazne datoteke koji se koristi, odvojen zarezima.
Podrazumevano: 1,2</translation>
    </message>
    <message>
        <source>Common encodings:</source>
        <translation>Uobičajena kodiranja:</translation>
    </message>
    <message>
        <source>Force the internal wordlist encoding from this code. Leave empty to let hashcat decide.</source>
        <translation>Prisili interno kodiranje liste reči iz ovog koda. Ostavite prazno da hashcat odluči.</translation>
    </message>
    <message>
        <source>Force the internal wordlist encoding to this code. Leave empty to let hashcat decide.</source>
        <translation>Prisili interno kodiranje liste reči u ovaj kod. Ostavite prazno da hashcat odluči.</translation>
    </message>
    <message>
        <source>Exported</source>
        <translation>Izvezeno</translation>
    </message>
    <message>
        <source>Export failed</source>
        <translation>Izvoz nije uspeo</translation>
    </message>
    <message>
        <source>Import failed</source>
        <translation>Uvoz nije uspeo</translation>
    </message>
    <message>
        <source>Imported</source>
        <translation>Uvezeno</translation>
    </message>
    <message>
        <source>The file was made by an older version of hashcat-gui (format version %1; this version reads format version %2).</source>
        <translation>Datoteku je napravila starija verzija programa hashcat-gui (verzija formata %1; ova verzija čita verziju formata %2).</translation>
    </message>
    <message>
        <source>The file holds sections this import does not use: %1.</source>
        <translation>Datoteka sadrži odeljke koje ovaj uvoz ne koristi: %1.</translation>
    </message>
    <message>
        <source>The file is in format version %1, but this version reads format version %2, so it was not applied. Update hashcat-gui to read it.</source>
        <translation>Datoteka je u verziji formata %1, ali ova verzija čita verziju formata %2, pa nije primenjena. Ažurirajte hashcat-gui da biste je pročitali.</translation>
    </message>
    <message>
        <source>The session file is in format version %1, but this version reads format version %2, so the session was not restored. The file was left as it is, so the session of this run is not stored either. Update hashcat-gui to read it.</source>
        <translation>Datoteka sesije je u verziji formata %1, ali ova verzija čita verziju formata %2, pa sesija nije vraćena. Datoteka je ostavljena kakva jeste, pa se ni sesija ovog pokretanja ne čuva. Ažurirajte hashcat-gui da biste je pročitali.</translation>
    </message>
    <message>
        <source>Settings exported to %1.</source>
        <translation>Podešavanja su izvezena u %1.</translation>
    </message>
    <message>
        <source>OpenCL device types to use, separated by commas.</source>
        <translation>Vrste OpenCL uređaja koje treba koristiti, razdvojene zarezima.</translation>
    </message>
    <message>
        <source>Export session</source>
        <translation>Izvoz sesije</translation>
    </message>
    <message>
        <source>Session exported to “%1”.</source>
        <translation>Sesija je izvezena u „%1“.</translation>
    </message>
    <message>
        <source>Import session</source>
        <translation>Uvoz sesije</translation>
    </message>
    <message>
        <source>Session imported from “%1”.</source>
        <translation>Sesija je uvezena iz „%1“.</translation>
    </message>
    <message>
        <source>Session imported from “%1”.

%2</source>
        <translation>Sesija je uvezena iz „%1“.

%2</translation>
    </message>
    <message>
        <source>Export settings</source>
        <translation>Izvoz podešavanja</translation>
    </message>
    <message>
        <source>Import settings</source>
        <translation>Uvoz podešavanja</translation>
    </message>
    <message>
        <source>Settings loaded from “%1”.</source>
        <translation>Podešavanja su učitana iz „%1“.</translation>
    </message>
    <message>
        <source>Review them and save to apply them.</source>
        <translation>Pregledajte ih i sačuvajte da biste ih primenili.</translation>
    </message>
    <message>
        <source>Load example session</source>
        <translation>Učitaj primer sesije</translation>
    </message>
    <message>
        <source>Reset session</source>
        <translation>Resetuj sesiju</translation>
    </message>
    <message>
        <source>Reset every field to its default? The current session is lost.</source>
        <translation>Vratiti sva polja na podrazumevane vrednosti? Trenutna sesija će biti izgubljena.</translation>
    </message>
    <message>
        <source>Session warning</source>
        <translation>Upozorenje o sesiji</translation>
    </message>
    <message>
        <source>Updating…</source>
        <translation>Ažuriranje…</translation>
    </message>
    <message>
        <source>Failed to load hash types</source>
        <translation>Vrste hash-ova nisu mogle da se učitaju</translation>
    </message>
    <message>
        <source>This mode needs a wordlist and a mask.</source>
        <translation>Ovaj režim zahteva listu reči i masku.</translation>
    </message>
    <message>
        <source>This mode needs at least one wordlist.</source>
        <translation>Ovaj režim zahteva najmanje jednu listu reči.</translation>
    </message>
    <message>
        <source>Open hash file</source>
        <translation>Otvori datoteku sa hash-ovima</translation>
    </message>
    <message>
        <source>Save output file</source>
        <translation>Sačuvaj izlaznu datoteku</translation>
    </message>
    <message>
        <source>Add wordlists</source>
        <translation>Dodaj liste reči</translation>
    </message>
    <message>
        <source>Open rules file</source>
        <translation>Otvori datoteku sa pravilima</translation>
    </message>
    <message>
        <source>Could not start “%1”: %2</source>
        <translation>„%1“ nije moglo da se pokrene: %2</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not available. Available terminals: %2
Navigate to %3 → %4 to change it.</source>
        <translation>Konfigurisani terminal „%1“ nije dostupan. Dostupni terminali: %2
Idite na %3 → %4 da biste ga promenili.</translation>
    </message>
    <message>
        <source>This mode needs exactly %1 wordlists (left, right).
Currently selected: %2.</source>
        <translation>Ovaj režim zahteva liste reči u tačnom broju %1 (leva, desna).
Trenutno izabrano: %2.</translation>
    </message>
    <message>
        <source>The session names the hash type %1, which this hashcat version does not support.</source>
        <translation>Sesija navodi vrstu hash-a %1, koju ova verzija hashcat-a ne podržava.</translation>
    </message>
    <message>
        <source>Password candidate mask. ?1–?4 insert a custom charset, ?w inserts the wordlist word and ?q a second wordlist word in the Hybrid (mask + ?w/?q) mode, and ?? is a literal ?.</source>
        <translation>Maska kandidata za lozinku. ?1–?4 umeću prilagođeni skup znakova, ?w umeće reč iz liste reči, a ?q drugu reč iz liste reči u režimu Hibrid (maska + ?w/?q), dok ?? označava bukvalno ?.</translation>
    </message>
    <message>
        <source>Load the %1 example? The current session is lost.

The example cracks a test hash; hashcat should recover the password %2.</source>
        <translation>Učitati primer %1? Trenutna sesija će biti izgubljena.

Primer provaljuje probni hash; hashcat bi trebalo da pronađe lozinku %2.</translation>
    </message>
    <message>
        <source>Could not create the example directory “%1”.</source>
        <translation>Direktorijum primera „%1“ nije mogao da se napravi.</translation>
    </message>
    <message>
        <source>Could not read the bundled file “%1”: %2</source>
        <translation>Ugrađena datoteka „%1“ nije mogla da se pročita: %2</translation>
    </message>
    <message>
        <source>Could not write “%1”: %2</source>
        <translation>„%1“ nije moglo da se upiše: %2</translation>
    </message>
    <message>
        <source>Straight</source>
        <translation>Rečnik</translation>
    </message>
    <message>
        <source>Combination</source>
        <translation>Kombinacija</translation>
    </message>
    <message>
        <source>Brute-force</source>
        <translation>Brute-force</translation>
    </message>
    <message>
        <source>Hybrid Wordlist + Mask</source>
        <translation>Hibrid: lista reči + maska</translation>
    </message>
    <message>
        <source>Hybrid Mask + Wordlist</source>
        <translation>Hibrid: maska + lista reči</translation>
    </message>
    <message>
        <source>Generic (feed)</source>
        <translation>Generički (feed)</translation>
    </message>
    <message>
        <source>Association</source>
        <translation>Asocijacija</translation>
    </message>
    <message>
        <source>Hybrid (mask + ?w/?q)</source>
        <translation>Hibrid (maska + ?w/?q)</translation>
    </message>
    <message>
        <source>Failed to obtain supported hash types.
Error: %1</source>
        <translation>Preuzimanje podržanih vrsta hash-ova nije uspelo.
Greška: %1</translation>
    </message>
    <message>
        <source>The hash types could not be loaded. Check the hashcat path in Settings.</source>
        <translation>Vrste hash-ova nisu mogle da se učitaju. Proverite putanju do hashcat-a u podešavanjima.</translation>
    </message>
    <message>
        <source>

This can be caused by Windows Defender or other antivirus software. See the &lt;a href=&quot;%1&quot;&gt;FAQ entry&lt;/a&gt; for a remedy.</source>
        <translation>

Uzrok može biti Windows Defender ili drugi antivirusni softver. Rešenje potražite u &lt;a href=&quot;%1&quot;&gt;stavci FAQ-a&lt;/a&gt;.</translation>
    </message>
    <message>
        <source>Set the hashcat path under %1 → %2 to load the available hash types.</source>
        <translation>Podesite putanju do hashcat-a u %1 → %2 da biste učitali dostupne vrste hash-ova.</translation>
    </message>
    <message>
        <source>This mode needs exactly 2 wordlists (left, right).</source>
        <translation>Ovaj režim zahteva tačno 2 liste reči (leva, desna).</translation>
    </message>
    <message>
        <source>This mode needs a mask.</source>
        <translation>Ovaj režim zahteva masku.</translation>
    </message>
    <message>
        <source>This mode runs hashcat’s bundled default-passwords ruleset; name one or more to use your own.</source>
        <translation>Ovaj režim pokreće ugrađeni skup pravila default-passwords programa hashcat; navedite jedan ili više da biste koristili sopstvene.</translation>
    </message>
    <message>
        <source>This mode needs one wordlist and at least one table.</source>
        <translation>Ovaj režim zahteva jednu listu reči i najmanje jednu tabelu.</translation>
    </message>
    <message>
        <source>This mode needs a feed. Arguments depend on the feed: random and stdin need none, wordlist needs a path.</source>
        <translation>Ovaj režim zahteva feed. Argumenti zavise od feed-a: random i stdin ne traže nijedan, wordlist traži putanju.</translation>
    </message>
    <message>
        <source>This mode needs at most one wordlist. Without one the words come from the hash file and the settings line can bound the run; with one the two files are paired by line.</source>
        <translation>Ovaj režim zahteva najviše jednu listu reči. Bez nje reči potiču iz datoteke sa hash-ovima, a red podešavanja može da ograniči pokretanje; sa listom reči dve datoteke se uparuju red po red.</translation>
    </message>
    <message>
        <source>This mode needs a mask with exactly one ?w; a ?q after ?w names a second wordlist.</source>
        <translation>Ovaj režim zahteva masku sa tačno jednim ?w; ?q posle ?w navodi drugu listu reči.</translation>
    </message>
    <message>
        <source>Open cache directory</source>
        <translation>Otvori direktorijum keša</translation>
    </message>
    <message>
        <source>Add wordlist folder</source>
        <translation>Dodaj fasciklu sa listama reči</translation>
    </message>
    <message>
        <source>Add rulesets</source>
        <translation>Dodaj skupove pravila</translation>
    </message>
    <message>
        <source>Add ruleset folder</source>
        <translation>Dodaj fasciklu skupa pravila</translation>
    </message>
    <message>
        <source>Add tables</source>
        <translation>Dodaj tabele</translation>
    </message>
    <message>
        <source>Open feed file</source>
        <translation>Otvori datoteku feed-a</translation>
    </message>
    <message>
        <source>This folder has no wordlists. hashcat reads the regular files directly inside it, not subfolders.</source>
        <translation>Ova fascikla nema liste reči. hashcat čita obične datoteke koje se nalaze direktno u njoj, ne i potfascikle.</translation>
    </message>
    <message>
        <source>This wordlist does not exist:
%1</source>
        <translation>Ova lista reči ne postoji:
%1</translation>
    </message>
    <message>
        <source>This ruleset folder has no Grammar/grammar.txt:
%1</source>
        <translation>Ova fascikla skupa pravila nema Grammar/grammar.txt:
%1</translation>
    </message>
    <message>
        <source>This ruleset does not exist:
%1</source>
        <translation>Ovaj skup pravila ne postoji:
%1</translation>
    </message>
    <message>
        <source>A table must be a file; hashcat does not read a folder:
%1</source>
        <translation>Tabela mora biti datoteka; hashcat ne čita fasciklu:
%1</translation>
    </message>
    <message>
        <source>This table does not exist:
%1</source>
        <translation>Ova tabela ne postoji:
%1</translation>
    </message>
    <message>
        <source>Copied to the clipboard</source>
        <translation>Kopirano u privremenu memoriju</translation>
    </message>
    <message>
        <source>Launch failed</source>
        <translation>Pokretanje nije uspelo</translation>
    </message>
    <message>
        <source>Please choose a hash file.</source>
        <translation>Izaberite datoteku sa hash-ovima.</translation>
    </message>
    <message>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to configure the path to the hashcat executable.</source>
        <translation>Idite na &lt;b&gt;%1 → %2&lt;/b&gt; da podesite putanju do hashcat programa.</translation>
    </message>
    <message>
        <source>The configured hashcat executable does not exist:
%1
Navigate to %2 → %3 to change it.</source>
        <translation>Konfigurisani hashcat program ne postoji:
%1
Idite na %2 → %3 da to promenite.</translation>
    </message>
    <message>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to select the terminal used for launching.</source>
        <translation>Idite na &lt;b&gt;%1 → %2&lt;/b&gt; da izaberete terminal koji se koristi za pokretanje.</translation>
    </message>
    <message>
        <source>Maximum number of wordlists for this mode: %1.
Currently selected: %2.</source>
        <translation>Najveći broj lista reči za ovaj režim: %1.
Trenutno izabrano: %2.</translation>
    </message>
    <message>
        <source>hashcat pairs the two files by line here; this GUI offers only one wordlist for that role.</source>
        <translation>Ovde hashcat uparuje dve datoteke red po red; ovaj interfejs nudi samo jednu listu reči za tu ulogu.</translation>
    </message>
    <message>
        <source>This wordlist has to be a file: hashcat reads it as the multiplied wordlist (the ?q side), and a folder cannot be used there:
%1</source>
        <translation>Ova lista reči mora biti datoteka: hashcat je čita kao umnoženu listu reči (strana ?q), a fascikla se tamo ne može koristiti:
%1</translation>
    </message>
    <message>
        <source>This mode needs at least one table.</source>
        <translation>Ovaj režim zahteva najmanje jednu tabelu.</translation>
    </message>
    <message>
        <source>This mode needs a feed: a bundled name (wordlist, stdin, random, …) or the path of a feed file.</source>
        <translation>Ovaj režim zahteva feed: ugrađeno ime (wordlist, stdin, random, …) ili putanju datoteke feed-a.</translation>
    </message>
    <message>
        <source>This mode pairs the wordlists with the mask; the mask needs a ?w for the word.</source>
        <translation>Ovaj režim uparuje liste reči sa maskom; maska zahteva ?w za reč.</translation>
    </message>
    <message>
        <source>The mask contains more than one ?w.</source>
        <translation>Maska sadrži više od jednog ?w.</translation>
    </message>
    <message>
        <source>The mask contains a ?q before the first ?w.</source>
        <translation>Maska sadrži ?q pre prvog ?w.</translation>
    </message>
    <message>
        <source>The mask contains more than one ?q.</source>
        <translation>Maska sadrži više od jednog ?q.</translation>
    </message>
    <message>
        <source>The mask names a second word with ?q, but only one wordlist is selected.</source>
        <translation>Maska navodi drugu reč pomoću ?q, ali je izabrana samo jedna lista reči.</translation>
    </message>
    <message>
        <source>Two wordlists are selected, but the mask has no ?q for the second one.</source>
        <translation>Izabrane su dve liste reči, ali maska nema ?q za drugu.</translation>
    </message>
    <message>
        <source>The increment is not offered in this hybrid mode.</source>
        <translation>Povećavanje se ne nudi u ovom hibridnom režimu.</translation>
    </message>
    <message>
        <source>The ?w and ?q markers are only offered in the hybrid mode (mask + wordlists).</source>
        <translation>Oznake ?w i ?q nude se samo u hibridnom režimu (maska + liste reči).</translation>
    </message>
    <message>
        <source>The setting “%1” is not valid for this mode.</source>
        <translation>Postavka „%1“ nije ispravna za ovaj režim.</translation>
    </message>
    <message>
        <source>The token “%1” is not a setting; settings are written as key=value.</source>
        <translation>Token „%1“ nije postavka; postavke se pišu kao key=value.</translation>
    </message>
    <message>
        <source>The settings line applies only while the words come from the hash file. Clear either the settings line or the wordlist list.</source>
        <translation>Red podešavanja važi samo dok reči potiču iz datoteke sa hash-ovima. Obrišite ili red podešavanja ili listu lista reči.</translation>
    </message>
    <message>
        <source>The hash type 4000 describes its hashes as a recipe, for example
md5(md5(salt) . pass). Enter it in the hash recipe field.</source>
        <translation>Vrsta hash-a 4000 opisuje svoje hash-ove kao recept, na primer
md5(md5(salt) . pass). Unesite ga u polje recepta za hash.</translation>
    </message>
    <message>
        <source>The minimum increment length is larger than the maximum.</source>
        <translation>Minimalna dužina povećavanja veća je od maksimalne.</translation>
    </message>
    <message>
        <source>The cache path must be an existing directory:
%1</source>
        <translation>Putanja keša mora biti postojeći direktorijum:
%1</translation>
    </message>
    <message>
        <source>The increment cannot be combined with the speed-only option.</source>
        <translation>Povećavanje se ne može kombinovati sa opcijom samo za brzinu.</translation>
    </message>
    <message>
        <source>Removing cracked hashes is not possible in the association attack mode.</source>
        <translation>Uklanjanje provaljenih hash-ova nije moguće u režimu napada asocijacije.</translation>
    </message>
</context>
<context>
    <name>SettingsDialog</name>
    <message>
        <source>Settings</source>
        <translation>Podešavanja</translation>
    </message>
    <message>
        <source>hashcat executable</source>
        <translation>hashcat program</translation>
    </message>
    <message>
        <source>Not configured</source>
        <translation>Nije konfigurisano</translation>
    </message>
    <message>
        <source>hashcat</source>
        <translation>hashcat</translation>
    </message>
    <message>
        <source>Select…</source>
        <translation>Izaberi…</translation>
    </message>
    <message>
        <source>&amp;Terminal</source>
        <translation>&amp;Terminal</translation>
    </message>
    <message>
        <source>&amp;Language</source>
        <translation>&amp;Jezik</translation>
    </message>
    <message>
        <source>&amp;Color scheme</source>
        <translation>Še&amp;ma boja</translation>
    </message>
    <message>
        <source>Not all color schemes are available on all platforms.</source>
        <translation>Nisu sve šeme boja dostupne na svim platformama.</translation>
    </message>
    <message>
        <source>&amp;Parameters</source>
        <translation>&amp;Parametri</translation>
    </message>
    <message>
        <source>&amp;Use short parameters</source>
        <translation>Koristi &amp;kratke parametre</translation>
    </message>
    <message>
        <source>Session and files</source>
        <translation>Sesija i datoteke</translation>
    </message>
    <message>
        <source>&amp;Restore the last session at startup</source>
        <translation>&amp;Vrati poslednju sesiju pri pokretanju</translation>
    </message>
    <message>
        <source>&amp;Session file</source>
        <translation>Datoteka &amp;sesije</translation>
    </message>
    <message>
        <source>The file the last session is stored in</source>
        <translation>Datoteka u kojoj se čuva poslednja sesija</translation>
    </message>
    <message>
        <source>Open folder</source>
        <translation>Otvori fasciklu</translation>
    </message>
    <message>
        <source>Open the folder the session is stored in</source>
        <translation>Otvori fasciklu u kojoj se čuva sesija</translation>
    </message>
    <message>
        <source>Settings &amp;file</source>
        <translation>Datoteka po&amp;dešavanja</translation>
    </message>
    <message>
        <source>The file the settings are stored in</source>
        <translation>Datoteka u kojoj se čuvaju podešavanja</translation>
    </message>
    <message>
        <source>Open the folder the settings are stored in</source>
        <translation>Otvori fasciklu u kojoj se čuvaju podešavanja</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Opšte</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Sačuvaj</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Otkaži</translation>
    </message>
    <message>
        <source>Download from %1</source>
        <translation>Preuzmi sa %1</translation>
    </message>
    <message>
        <source>Use the short option names (-m) instead of the long ones (--hash-type) in the command.</source>
        <translation>Koristi kratke nazive opcija (-m) umesto dugih (--hash-type) u naredbi.</translation>
    </message>
    <message>
        <source>Select a terminal</source>
        <translation>Izaberite terminal</translation>
    </message>
    <message>
        <source>System default</source>
        <translation>Sistemski podrazumevano</translation>
    </message>
    <message>
        <source>Light</source>
        <translation>Svetla</translation>
    </message>
    <message>
        <source>Dark</source>
        <translation>Tamna</translation>
    </message>
    <message>
        <source>The hashcat path “%1” was not found on this computer.</source>
        <translation>Putanja do programa hashcat „%1“ nije pronađena na ovom računaru.</translation>
    </message>
    <message>
        <source>The terminal “%1” is not installed and was not applied.</source>
        <translation>Terminal „%1“ nije instaliran i nije primenjen.</translation>
    </message>
    <message>
        <source>The color scheme “%1” is not offered and was not applied.</source>
        <translation>Šema boja „%1“ se ne nudi i nije primenjena.</translation>
    </message>
    <message>
        <source>The language “%1” is not offered and was not applied.</source>
        <translation>Jezik „%1“ se ne nudi i nije primenjen.</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not installed.</source>
        <translation>Konfigurisani terminal „%1“ nije instaliran.</translation>
    </message>
    <message>
        <source>Executable Files (*.exe *.bin hashcat);;All Files (*)</source>
        <translation>Izvršne datoteke (*.exe *.bin hashcat);;Sve datoteke (*)</translation>
    </message>
    <message>
        <source>No hashcat executable is configured; hashcat cannot be started.</source>
        <translation>Nijedan hashcat program nije konfigurisan; hashcat ne može da se pokrene.</translation>
    </message>
    <message>
        <source>The configured hashcat executable was not found on this computer.</source>
        <translation>Konfigurisani hashcat program nije pronađen na ovom računaru.</translation>
    </message>
    <message>
        <source>Select the terminal hashcat is started in.</source>
        <translation>Izaberite terminal u kojem se pokreće hashcat.</translation>
    </message>
    <message>
        <source>Select the hashcat executable</source>
        <translation>Izaberite hashcat program</translation>
    </message>
    <message>
        <source>Invalid file</source>
        <translation>Nevažeća datoteka</translation>
    </message>
    <message>
        <source>The selected file is not an executable.</source>
        <translation>Izabrana datoteka nije izvršna datoteka.</translation>
    </message>
    <message>
        <source>Restart required</source>
        <translation>Potreban restart</translation>
    </message>
    <message>
        <source>Restart hashcat-gui to apply the new language.</source>
        <translation>Restartujte hashcat-gui da biste primenili novi jezik.</translation>
    </message>
</context>
<context>
    <name>StateFile</name>
    <message>
        <source>The file is not valid JSON: %1</source>
        <translation>Datoteka nije ispravan JSON: %1</translation>
    </message>
    <message>
        <source>The file does not contain a JSON object.</source>
        <translation>Datoteka ne sadrži JSON objekat.</translation>
    </message>
    <message>
        <source>The file does not contain any sections.</source>
        <translation>Datoteka ne sadrži nijedan odeljak.</translation>
    </message>
    <message>
        <source>Could not open “%1” for writing: %2</source>
        <translation>„%1“ nije moglo da se otvori za upisivanje: %2</translation>
    </message>
    <message>
        <source>Could not write to “%1”: %2</source>
        <translation>U „%1“ nije moglo da se upiše: %2</translation>
    </message>
    <message>
        <source>Could not open “%1” for reading: %2</source>
        <translation>„%1“ nije moglo da se otvori za čitanje: %2</translation>
    </message>
    <message>
        <source>The file does not contain a “%1” section.</source>
        <translation>Datoteka ne sadrži odeljak „%1“.</translation>
    </message>
    <message>
        <source>The file does not contain a “%1” section; it holds %2.</source>
        <translation>Datoteka ne sadrži odeljak „%1“; sadrži %2.</translation>
    </message>
    <message>
        <source>The “%1” section does not contain any values.</source>
        <translation>Odeljak „%1“ ne sadrži nijednu vrednost.</translation>
    </message>
    <message>
        <source> and </source>
        <translation> i </translation>
    </message>
    <message>
        <source> and %1 more</source>
        <translation> i još %1</translation>
    </message>
    <message>
        <source>The file names values this version does not know, so they were not applied: %1.</source>
        <translation>Datoteka navodi vrednosti koje ova verzija ne poznaje, pa nisu primenjene: %1.</translation>
    </message>
    <message>
        <source>The file holds values of the wrong type, so they were not applied: %1.</source>
        <translation>Datoteka sadrži vrednosti pogrešnog tipa, pa nisu primenjene: %1.</translation>
    </message>
</context>
<context>
    <name>UpdateChecker</name>
    <message>
        <source>GitHub answered with HTTP status %1: %2</source>
        <translation>GitHub je odgovorio HTTP statusom %1: %2</translation>
    </message>
    <message>
        <source>GitHub answered with HTTP status %1.</source>
        <translation>GitHub je odgovorio HTTP statusom %1.</translation>
    </message>
    <message>
        <source>The release information is not valid JSON.</source>
        <translation>Informacije o izdanju nisu ispravan JSON.</translation>
    </message>
    <message>
        <source>The release information does not name a version.</source>
        <translation>Informacije o izdanju ne navode nijednu verziju.</translation>
    </message>
    <message>
        <source>The release version %1 is not a valid version number.</source>
        <translation>Verzija izdanja %1 nije ispravan broj verzije.</translation>
    </message>
    <message>
        <source>The installed version %1 is not a valid version number.</source>
        <translation>Instalirana verzija %1 nije ispravan broj verzije.</translation>
    </message>
</context>
</TS>
