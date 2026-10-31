<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="pl" sourcelanguage="en">
<context>
    <name>AboutDialog</name>
    <message>
        <source>About</source>
        <translation>O programie</translation>
    </message>
    <message>
        <source>Graphical user interface for hashcat, the password recovery utility</source>
        <translation>Graficzny interfejs użytkownika dla hashcat, narzędzia do odzyskiwania haseł</translation>
    </message>
    <message>
        <source>Version information</source>
        <translation>Informacje o wersji</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Nieznane</translation>
    </message>
    <message>
        <source>Updates</source>
        <translation>Aktualizacje</translation>
    </message>
    <message>
        <source>Checking for updates…</source>
        <translation>Sprawdzanie aktualizacji…</translation>
    </message>
    <message>
        <source>System information</source>
        <translation>Informacje o systemie</translation>
    </message>
    <message>
        <source>Operating system</source>
        <translation>System operacyjny</translation>
    </message>
    <message>
        <source>Links</source>
        <translation>Odnośniki</translation>
    </message>
    <message>
        <source>License</source>
        <translation>Licencja</translation>
    </message>
    <message>
        <source>Copy the application, toolkit, system and hashcat versions to the clipboard</source>
        <translation>Skopiuj do schowka wersje aplikacji, biblioteki Qt, systemu i hashcat</translation>
    </message>
    <message>
        <source>Copy details</source>
        <translation>Kopiuj szczegóły</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Copied to the clipboard</source>
        <translation>Skopiowano do schowka</translation>
    </message>
    <message>
        <source>Project page</source>
        <translation>Strona projektu</translation>
    </message>
    <message>
        <source>Report an issue</source>
        <translation>Zgłoś problem</translation>
    </message>
    <message>
        <source>hashcat</source>
        <translation>hashcat</translation>
    </message>
    <message>
        <source>hashcat FAQ</source>
        <translation>FAQ hashcat</translation>
    </message>
    <message>
        <source>hashcat-gui is licensed under the %1.</source>
        <translation>hashcat-gui jest udostępniany na licencji %1.</translation>
    </message>
    <message>
        <source>GNU GPL v3.0 or later</source>
        <translation>GNU GPL v3.0 lub nowszej</translation>
    </message>
    <message>
        <source>Not configured</source>
        <translation>Nie skonfigurowano</translation>
    </message>
    <message>
        <source>Error: %1</source>
        <translation>Błąd: %1</translation>
    </message>
    <message>
        <source>A newer version is available: %1.</source>
        <translation>Dostępna jest nowsza wersja: %1.</translation>
    </message>
    <message>
        <source>Release page</source>
        <translation>Strona wydania</translation>
    </message>
    <message>
        <source>No updates available.</source>
        <translation>Brak dostępnych aktualizacji.</translation>
    </message>
    <message>
        <source>Could not check for updates.</source>
        <translation>Nie udało się sprawdzić aktualizacji.</translation>
    </message>
    <message>
        <source>Could not check for updates: %1</source>
        <translation>Nie udało się sprawdzić aktualizacji: %1</translation>
    </message>
</context>
<context>
    <name>HashcatInfoParser</name>
    <message>
        <source>Invalid JSON returned from hashcat.</source>
        <translation>hashcat zwrócił nieprawidłowy JSON.</translation>
    </message>
    <message>
        <source>hashcat did not report any hash types.</source>
        <translation>hashcat nie zgłosił żadnych typów hashy.</translation>
    </message>
</context>
<context>
    <name>HelperUtils</name>
    <message>
        <source>The hashcat path is not configured.</source>
        <translation>Ścieżka do hashcat nie jest skonfigurowana.</translation>
    </message>
    <message>
        <source>Failed to start hashcat
%1</source>
        <translation>Nie udało się uruchomić hashcat
%1</translation>
    </message>
    <message>
        <source>hashcat timed out
%1</source>
        <translation>Upłynął limit czasu hashcat
%1</translation>
    </message>
    <message>
        <source>No supported terminal was found on this system. hashcat-gui needs one to show the hashcat output.</source>
        <translation>W tym systemie nie znaleziono obsługiwanego terminala. hashcat-gui potrzebuje go do wyświetlania danych wyjściowych hashcat.</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <source>Attack</source>
        <translation>Atak</translation>
    </message>
    <message>
        <source>Hash file or hash:</source>
        <translation>Plik z hashami lub hash:</translation>
    </message>
    <message>
        <source>The hash to crack, a path to a file of hashes, or a .hccapx WPA capture. Paste a single hash here, or point it at a file.</source>
        <translation>Hash do złamania, ścieżka do pliku z hashami lub zrzut WPA .hccapx. Wklej tutaj pojedynczy hash albo wskaż plik.</translation>
    </message>
    <message>
        <source>Salt is hex</source>
        <translation>Sól jest szesnastkowa</translation>
    </message>
    <message>
        <source>Remove already-cracked hashes from the potfile so a later run does not report them again. Refused in attack mode 9.</source>
        <translation>Usuń już złamane hashe z pliku potfile, aby kolejne uruchomienie nie zgłaszało ich ponownie. Odrzucane w trybie ataku 9.</translation>
    </message>
    <message>
        <source>Remove hash from hash list once it is cracked</source>
        <translation>Usuń hash z listy hashy po jego złamaniu</translation>
    </message>
    <message>
        <source>Ignore the @username part of hashes (in hash modes that carry one) so the same word can satisfy several accounts.</source>
        <translation>Ignoruj część @username w hashach (w trybach hashy, które ją zawierają), aby to samo słowo mogło pasować do kilku kont.</translation>
    </message>
    <message>
        <source>Ignore username in hash file</source>
        <translation>Ignoruj nazwę użytkownika w pliku z hashami</translation>
    </message>
    <message>
        <source>Mode:</source>
        <translation>Tryb:</translation>
    </message>
    <message>
        <source>Attack mode. Each choice reveals its own options below.</source>
        <translation>Tryb ataku. Każdy wybór pokazuje poniżej własne opcje.</translation>
    </message>
    <message>
        <source>Hash type:</source>
        <translation>Typ hasha:</translation>
    </message>
    <message>
        <source>Hash recipe:</source>
        <translation>Przepis na hash:</translation>
    </message>
    <message>
        <source>Unix: &apos;md5(md5(salt) . pass)&apos;
               Windows: &quot;md5(&apos;abc&apos; . pass)&quot;</source>
        <translation>Unix: &apos;md5(md5(salt) . pass)&apos;
               Windows: &quot;md5(&apos;abc&apos; . pass)&quot;</translation>
    </message>
    <message>
        <source>Wordlists</source>
        <translation>Listy słów</translation>
    </message>
    <message>
        <source>Move item up</source>
        <translation>Przesuń element w górę</translation>
    </message>
    <message>
        <source>Move item down</source>
        <translation>Przesuń element w dół</translation>
    </message>
    <message>
        <source>hashcat reads every file directly inside the folder as one wordlist.</source>
        <translation>hashcat odczytuje każdy plik znajdujący się bezpośrednio w folderze jako jedną listę słów.</translation>
    </message>
    <message>
        <source>Add folder…</source>
        <translation>Dodaj folder…</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Usuń</translation>
    </message>
    <message>
        <source>Only checked rows are used. A folder is one wordlist: hashcat reads the files directly inside it.</source>
        <translation>Używane są tylko zaznaczone wiersze. Folder to jedna lista słów: hashcat odczytuje pliki znajdujące się bezpośrednio w nim.</translation>
    </message>
    <message>
        <source>Wordlist is hex</source>
        <translation>Lista słów jest szesnastkowa</translation>
    </message>
    <message>
        <source>Encoding from:</source>
        <translation>Kodowanie źródłowe:</translation>
    </message>
    <message>
        <source>Encoding to:</source>
        <translation>Kodowanie docelowe:</translation>
    </message>
    <message>
        <source>Settings (no wordlist):</source>
        <translation>Ustawienia (bez listy słów):</translation>
    </message>
    <message>
        <source>Positional settings, separated by spaces. Only used when no wordlist is selected: with one, hashcat pairs the two files by line. “phases” picks which phases run, cheapest first; the default “words,rules,pcfg” ends only when stopped. Phase-prefixed settings: “rules.rulefile”, “rules.rulemax”, “words.hints”, “pcfg.hintaccount”, …</source>
        <translation>Ustawienia pozycyjne, oddzielone spacjami. Używane tylko wtedy, gdy nie wybrano listy słów: przy wybranej liście hashcat łączy oba pliki wiersz po wierszu. „phases” wybiera, które fazy są uruchamiane, od najtańszej; domyślne „words,rules,pcfg” kończy się dopiero po zatrzymaniu. Ustawienia z prefiksem fazy: „rules.rulefile”, „rules.rulemax”, „words.hints”, „pcfg.hintaccount”, …</translation>
    </message>
    <message>
        <source>PCFG</source>
        <translation>PCFG</translation>
    </message>
    <message>
        <source>A row is a ruleset file, a folder with a Grammar/grammar.txt, or a bundled name. Only checked rows are used; with none checked, hashcat’s bundled default-passwords runs. hints is also bundled.</source>
        <translation>Wiersz to plik zestawu reguł, folder zawierający Grammar/grammar.txt albo nazwa dołączona do programu. Używane są tylko zaznaczone wiersze; gdy żaden nie jest zaznaczony, uruchamiany jest dołączony do hashcat zestaw default-passwords. hints również jest dołączony.</translation>
    </message>
    <message>
        <source>A ruleset folder needs a Grammar/grammar.txt.</source>
        <translation>Folder zestawu reguł wymaga pliku Grammar/grammar.txt.</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Scale:</translation>
    </message>
    <message>
        <source>Quantization steps per bit (1–64)</source>
        <translation>Kroki kwantyzacji na bit (1–64)</translation>
    </message>
    <message>
        <source>Costmax:</source>
        <translation>Costmax:</translation>
    </message>
    <message>
        <source>Highest cost level to enumerate (1–64)</source>
        <translation>Najwyższy poziom kosztu do wyliczenia (1–64)</translation>
    </message>
    <message>
        <source>Weights:</source>
        <translation>Weights:</translation>
    </message>
    <message>
        <source>One value per ruleset, plain numbers separated by colons, e.g. 2:1 for two rulesets. The count must match the number of rulesets.</source>
        <translation>Jedna wartość na zestaw reguł, zwykłe liczby oddzielone dwukropkami, np. 2:1 dla dwóch zestawów reguł. Liczba wartości musi odpowiadać liczbie zestawów reguł.</translation>
    </message>
    <message>
        <source>Threads:</source>
        <translation>Wątki:</translation>
    </message>
    <message>
        <source>Cores that generate base words (0–16); 0 runs on the calling thread</source>
        <translation>Rdzenie generujące słowa bazowe (0–16); 0 działa w wątku wywołującym</translation>
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
        <translation>Liczba bitów, jaką może obejmować jedna pętla wewnętrzna (0–27)</translation>
    </message>
    <message>
        <source>Maxword:</source>
        <translation>Maxword:</translation>
    </message>
    <message>
        <source>Candidates the kernel builds per base word: 0, 16 or 32 (a multiple of 16)</source>
        <translation>Kandydaci budowani przez jądro na jedno słowo bazowe: 0, 16 lub 32 (wielokrotność 16)</translation>
    </message>
    <message>
        <source>Maxgain:</source>
        <translation>Maxgain:</translation>
    </message>
    <message>
        <source>1.0–64.0; decimal point is a dot, not the locale separator</source>
        <translation>1.0–64.0; separatorem dziesiętnym jest kropka, a nie separator z ustawień regionalnych</translation>
    </message>
    <message>
        <source>Walk:</source>
        <translation>Walk:</translation>
    </message>
    <message>
        <source>1 steps the last token instead of unraking it</source>
        <translation>1 przesuwa ostatni token krok po kroku zamiast stosować do niego „unraking”</translation>
    </message>
    <message>
        <source>Mask:</source>
        <translation>Maska:</translation>
    </message>
    <message>
        <source>Hold the grammar to the candidates this mask admits; cannot be combined with omen=1, or with hintaccount while the hint words come from the accounts (give hintwords or hintfile instead)</source>
        <translation>Ogranicz gramatykę do kandydatów dopuszczanych przez tę maskę; nie można łączyć z omen=1 ani z hintaccount, dopóki słowa podpowiedzi pochodzą z kont (zamiast tego podaj hintwords lub hintfile)</translation>
    </message>
    <message>
        <source>Extra settings (key=value):</source>
        <translation>Dodatkowe ustawienia (key=value):</translation>
    </message>
    <message>
        <source>Additional key=value settings, separated by spaces: omen, cache, lookup, pwmin, pwmax, hintaccount, hintwords, hintfile, hintrank, hintrepeat. Everything after the first key=value is a setting, so rulesets must come first.</source>
        <translation>Dodatkowe ustawienia key=value, oddzielone spacjami: omen, cache, lookup, pwmin, pwmax, hintaccount, hintwords, hintfile, hintrank, hintrepeat. Wszystko po pierwszym key=value jest ustawieniem, więc zestawy reguł muszą występować jako pierwsze.</translation>
    </message>
    <message>
        <source>Table</source>
        <translation>Tabela</translation>
    </message>
    <message>
        <source>Every checked row after the wordlist is a table</source>
        <translation>Każdy zaznaczony wiersz po liście słów jest tabelą</translation>
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
        <source>Whether leaving a token unchanged is one of its choices: 1 (default) keeps it, 0 drops it so the table only produces variations. Cannot be combined with single=1.</source>
        <translation>Czy pozostawienie tokenu bez zmian jest jednym z jego wariantów: 1 (domyślnie) zachowuje go, 0 odrzuca go, więc tabela tworzy tylko odmiany. Nie można łączyć z single=1.</translation>
    </message>
    <message>
        <source>Template:</source>
        <translation>Template:</translation>
    </message>
    <message>
        <source>1 reads the wordlist as a set of patterns: no word is ever returned unchanged, and words matching no rule are dropped. 0 (default) keeps unchanged words.</source>
        <translation>1 odczytuje listę słów jako zbiór wzorców: żadne słowo nie jest zwracane bez zmian, a słowa niepasujące do żadnej reguły są odrzucane. 0 (domyślnie) zachowuje słowa bez zmian.</translation>
    </message>
    <message>
        <source>Single:</source>
        <translation>Single:</translation>
    </message>
    <message>
        <source>single=1 makes each candidate carry exactly one substitution instead of the cross product of every matched position, usually a different, cheaper attack.</source>
        <translation>single=1 sprawia, że każdy kandydat zawiera dokładnie jedną podmianę zamiast iloczynu kartezjańskiego wszystkich dopasowanych pozycji, co zwykle jest innym, tańszym atakiem.</translation>
    </message>
    <message>
        <source>Cap:</source>
        <translation>Cap:</translation>
    </message>
    <message>
        <source>Maximum replacements one source may carry in a word; 0 means no limit. Only limits the host-side cross product, not a single=1 run.</source>
        <translation>Maksymalna liczba podmian, jaką jedno źródło może wprowadzić w słowie; 0 oznacza brak limitu. Ogranicza tylko iloczyn kartezjański po stronie hosta, a nie przebieg z single=1.</translation>
    </message>
    <message>
        <source>Maxperm:</source>
        <translation>Maxperm:</translation>
    </message>
    <message>
        <source>How many of one word’s cross product the host enumerates before giving up; 0 means no limit. Each device cell is never cut.</source>
        <translation>Ile elementów iloczynu kartezjańskiego jednego słowa host wylicza, zanim się podda; 0 oznacza brak limitu. Żadna komórka urządzenia nie jest nigdy ucinana.</translation>
    </message>
    <message>
        <source>Additional key=value settings, separated by spaces. Everything after the first key=value is a setting, so the tables must come first.</source>
        <translation>Dodatkowe ustawienia key=value, oddzielone spacjami. Wszystko po pierwszym key=value jest ustawieniem, więc tabele muszą występować jako pierwsze.</translation>
    </message>
    <message>
        <source>Generic feed</source>
        <translation>Generyczny feed</translation>
    </message>
    <message>
        <source>Feed:</source>
        <translation>Feed:</translation>
    </message>
    <message>
        <source>Candidate generator. Anything containing a slash is treated as a feed path; otherwise hashcat looks for feed_&lt;name&gt; under its feeds directory. Arguments below are feed-specific.</source>
        <translation>Generator kandydatów. Wszystko, co zawiera ukośnik, jest traktowane jako ścieżka feedu; w przeciwnym razie hashcat szuka feed_&lt;name&gt; w swoim katalogu feedów. Poniższe argumenty zależą od feedu.</translation>
    </message>
    <message>
        <source>wordlist, stdin, random, association, pcfg, table, hybrid, mask, or a feed path</source>
        <translation>wordlist, stdin, random, association, pcfg, table, hybrid, mask lub ścieżka feedu</translation>
    </message>
    <message>
        <source>Browse…</source>
        <translation>Przeglądaj…</translation>
    </message>
    <message>
        <source>Argument:</source>
        <translation>Argument:</translation>
    </message>
    <message>
        <source>Argument passed to the feed. Its meaning and count depend on the feed (the wordlist feed takes wordlist paths); extra tokens go straight to the feed.</source>
        <translation>Argument przekazywany do feedu. Jego znaczenie i liczba zależą od feedu (feed wordlist przyjmuje ścieżki list słów); dodatkowe tokeny trafiają bezpośrednio do feedu.</translation>
    </message>
    <message>
        <source>Add</source>
        <translation>Dodaj</translation>
    </message>
    <message>
        <source>Arguments the feed reads, in order; only checked rows are used</source>
        <translation>Argumenty odczytywane przez feed, w kolejności; używane są tylko zaznaczone wiersze</translation>
    </message>
    <message>
        <source>Rules</source>
        <translation>Reguły</translation>
    </message>
    <message>
        <source>Use rules files:</source>
        <translation>Użyj plików reguł:</translation>
    </message>
    <message>
        <source>Enable rules file slot 1 for this run.</source>
        <translation>Włącz slot 1 pliku reguł dla tego uruchomienia.</translation>
    </message>
    <message>
        <source>Rules file:</source>
        <translation>Plik reguł:</translation>
    </message>
    <message>
        <source>Rule file for slot 1; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>Plik reguł dla slotu 1; stosowany do każdego słowa. Pozostaw puste, aby nie używać slotu.</translation>
    </message>
    <message>
        <source>Enable rules file slot 2 for this run.</source>
        <translation>Włącz slot 2 pliku reguł dla tego uruchomienia.</translation>
    </message>
    <message>
        <source>Rule file for slot 2; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>Plik reguł dla slotu 2; stosowany do każdego słowa. Pozostaw puste, aby nie używać slotu.</translation>
    </message>
    <message>
        <source>Enable rules file slot 3 for this run.</source>
        <translation>Włącz slot 3 pliku reguł dla tego uruchomienia.</translation>
    </message>
    <message>
        <source>Rule file for slot 3; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>Plik reguł dla slotu 3; stosowany do każdego słowa. Pozostaw puste, aby nie używać slotu.</translation>
    </message>
    <message>
        <source>Generate the rules automatically instead of reading them from a file.</source>
        <translation>Generuj reguły automatycznie zamiast wczytywać je z pliku.</translation>
    </message>
    <message>
        <source>Generate rules:</source>
        <translation>Generuj reguły:</translation>
    </message>
    <message>
        <source>How many rules to generate for the finding-rule attack. No rules file is read, hashcat builds them itself; use an existing rule file with the slots above instead.</source>
        <translation>Ile reguł wygenerować na potrzeby ataku wyszukującego reguły. Żaden plik reguł nie jest wczytywany, hashcat tworzy je sam; zamiast tego użyj istniejącego pliku reguł w powyższych slotach.</translation>
    </message>
    <message>
        <source>Pass every rules file to hashcat in one run, instead of one run per file.</source>
        <translation>Przekaż wszystkie pliki reguł do hashcat w jednym uruchomieniu zamiast jednego uruchomienia na plik.</translation>
    </message>
    <message>
        <source>Join -r files into one list</source>
        <translation>Połącz pliki -r w jedną listę</translation>
    </message>
    <message>
        <source>Wordlist rules</source>
        <translation>Reguły listy słów</translation>
    </message>
    <message>
        <source>Left rule:</source>
        <translation>Reguła lewa:</translation>
    </message>
    <message>
        <source>Right rule:</source>
        <translation>Reguła prawa:</translation>
    </message>
    <message>
        <source>Interpret the custom charsets as hexadecimal bytes. Applies to custom charset 1 through 4.</source>
        <translation>Interpretuj własne zestawy znaków jako bajty szesnastkowe. Dotyczy własnych zestawów znaków od 1 do 4.</translation>
    </message>
    <message>
        <source>Charset is hex</source>
        <translation>Zestaw znaków jest szesnastkowy</translation>
    </message>
    <message>
        <source>Mask</source>
        <translation>Maska</translation>
    </message>
    <message>
        <source>Increment mask length</source>
        <translation>Zwiększaj długość maski</translation>
    </message>
    <message>
        <source>Min:</source>
        <translation>Min.:</translation>
    </message>
    <message>
        <source>Max:</source>
        <translation>Maks.:</translation>
    </message>
    <message>
        <source>Custom charsets</source>
        <translation>Własne zestawy znaków</translation>
    </message>
    <message>
        <source>Charset 1:</source>
        <translation>Zestaw znaków 1:</translation>
    </message>
    <message>
        <source>Charset 2:</source>
        <translation>Zestaw znaków 2:</translation>
    </message>
    <message>
        <source>Charset 3:</source>
        <translation>Zestaw znaków 3:</translation>
    </message>
    <message>
        <source>Charset 4:</source>
        <translation>Zestaw znaków 4:</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Ogólne</translation>
    </message>
    <message>
        <source>Session and limits</source>
        <translation>Sesja i limity</translation>
    </message>
    <message>
        <source>Session name:</source>
        <translation>Nazwa sesji:</translation>
    </message>
    <message>
        <source>Name hashcat uses for its restore file and status output.</source>
        <translation>Nazwa używana przez hashcat dla pliku przywracania i danych wyjściowych statusu.</translation>
    </message>
    <message>
        <source>Runtime limit:</source>
        <translation>Limit czasu działania:</translation>
    </message>
    <message>
        <source>unlimited</source>
        <translation>bez limitu</translation>
    </message>
    <message>
        <source>Skip:</source>
        <translation>Pomiń:</translation>
    </message>
    <message>
        <source>Limit:</source>
        <translation>Limit:</translation>
    </message>
    <message>
        <source>Update status automatically</source>
        <translation>Aktualizuj status automatycznie</translation>
    </message>
    <message>
        <source>Open…</source>
        <translation>Otwórz…</translation>
    </message>
    <message>
        <source>Assume the salt is given in hexadecimal.</source>
        <translation>Zakładaj, że sól podano szesnastkowo.</translation>
    </message>
    <message>
        <source>Add files…</source>
        <translation>Dodaj pliki…</translation>
    </message>
    <message>
        <source>Assume words in the wordlist are given in hexadecimal.</source>
        <translation>Zakładaj, że słowa na liście słów podano szesnastkowo.</translation>
    </message>
    <message>
        <source>Single rule applied to each word from the left wordlist.</source>
        <translation>Pojedyncza reguła stosowana do każdego słowa z lewej listy słów.</translation>
    </message>
    <message>
        <source>Single rule applied to each word from the right wordlist.</source>
        <translation>Pojedyncza reguła stosowana do każdego słowa z prawej listy słów.</translation>
    </message>
    <message>
        <source>Increment the mask length from the minimum to the maximum.</source>
        <translation>Zwiększaj długość maski od minimum do maksimum.</translation>
    </message>
    <message>
        <source>Minimum mask length; 0 uses hashcat’s default.</source>
        <translation>Minimalna długość maski; 0 używa wartości domyślnej hashcat.</translation>
    </message>
    <message>
        <source>Maximum mask length; 0 uses hashcat’s default.</source>
        <translation>Maksymalna długość maski; 0 używa wartości domyślnej hashcat.</translation>
    </message>
    <message>
        <source>Abort the session after this many seconds; 0 runs until hashcat finishes.</source>
        <translation>Przerwij sesję po tylu sekundach; 0 działa, aż hashcat zakończy pracę.</translation>
    </message>
    <message>
        <source>&#xa0;s</source>
        <translation>&#xa0;s</translation>
    </message>
    <message>
        <source>Skip this many candidates from the start.</source>
        <translation>Pomiń tylu kandydatów od początku.</translation>
    </message>
    <message>
        <source>Only test this many candidates after the skipped ones.</source>
        <translation>Testuj tylko tylu kandydatów po pominiętych.</translation>
    </message>
    <message>
        <source>Keep updating the run status (speed, progress, ETA) instead of printing one final line.</source>
        <translation>Na bieżąco aktualizuj stan przebiegu (szybkość, postęp, szacowany czas) zamiast wypisywać jedną końcową linię.</translation>
    </message>
    <message>
        <source>Seconds between status updates.</source>
        <translation>Sekundy między aktualizacjami statusu.</translation>
    </message>
    <message>
        <source>Ignore warnings and start hashcat anyway. Use with care.</source>
        <translation>Zignoruj ostrzeżenia i mimo to uruchom hashcat. Używaj ostrożnie.</translation>
    </message>
    <message>
        <source>Force (ignore warnings)</source>
        <translation>Wymuś (ignoruj ostrzeżenia)</translation>
    </message>
    <message>
        <source>Performance</source>
        <translation>Wydajność</translation>
    </message>
    <message>
        <source>Enable optimized kernels (limits password length)</source>
        <translation>Włącz zoptymalizowane jądra (ogranicza długość hasła)</translation>
    </message>
    <message>
        <source>Print only the attack’s speed and quit, without cracking. Useful for benchmarking and comparing settings.</source>
        <translation>Wypisz tylko szybkość ataku i zakończ, bez łamania. Przydatne do testów wydajności i porównywania ustawień.</translation>
    </message>
    <message>
        <source>Return expected speed of the attack, then quit</source>
        <translation>Zwróć oczekiwaną szybkość ataku, a następnie zakończ</translation>
    </message>
    <message>
        <source>Resources</source>
        <translation>Zasoby</translation>
    </message>
    <message>
        <source>CPU affinity:</source>
        <translation>Powinowactwo CPU:</translation>
    </message>
    <message>
        <source>Backend devices:</source>
        <translation>Urządzenia backendu:</translation>
    </message>
    <message>
        <source>Device types:</source>
        <translation>Typy urządzeń:</translation>
    </message>
    <message>
        <source>Pin hashcat to these CPU devices, separated by commas. Leave empty to let hashcat decide.</source>
        <translation>Przypnij hashcat do tych urządzeń CPU, oddzielonych przecinkami. Pozostaw puste, aby hashcat zdecydował.</translation>
    </message>
    <message>
        <source>Devices to use, separated by commas. Leave empty for all devices.</source>
        <translation>Urządzenia do użycia, oddzielone przecinkami. Pozostaw puste, aby użyć wszystkich urządzeń.</translation>
    </message>
    <message>
        <source>Abort when a device reaches this temperature.</source>
        <translation>Przerwij, gdy urządzenie osiągnie tę temperaturę.</translation>
    </message>
    <message>
        <source>&#xa0;°C</source>
        <translation>&#xa0;°C</translation>
    </message>
    <message>
        <source>Built-in variables:

&lt;hash&gt; = current hash filename or hash
&lt;unixtime&gt; = current Unix timestamp</source>
        <translation>Wbudowane zmienne:

&lt;hash&gt; = nazwa bieżącego pliku z hashami lub hash
&lt;unixtime&gt; = bieżący znacznik czasu Unix</translation>
    </message>
    <message>
        <source>Save…</source>
        <translation>Zapisz…</translation>
    </message>
    <message>
        <source>Do not write recovered hashes to the potfile.</source>
        <translation>Nie zapisuj odzyskanych hashy do pliku potfile.</translation>
    </message>
    <message>
        <source>Settings…</source>
        <translation>Ustawienia…</translation>
    </message>
    <message>
        <source>Import Settings…</source>
        <translation>Importuj ustawienia…</translation>
    </message>
    <message>
        <source>Export Settings…</source>
        <translation>Eksportuj ustawienia…</translation>
    </message>
    <message>
        <source>Export Session…</source>
        <translation>Eksportuj sesję…</translation>
    </message>
    <message>
        <source>Import Session…</source>
        <translation>Importuj sesję…</translation>
    </message>
    <message>
        <source>Abort above</source>
        <translation>Przerwij powyżej</translation>
    </message>
    <message>
        <source>Cache path:</source>
        <translation>Ścieżka pamięci podręcznej:</translation>
    </message>
    <message>
        <source>Directory hashcat keeps its cache in. Must exist.</source>
        <translation>Katalog, w którym hashcat przechowuje swoją pamięć podręczną. Musi istnieć.</translation>
    </message>
    <message>
        <source>Abort the session when an OpenCL device hotspot exceeds this many degrees Celsius. 0 disables the thermal limit.</source>
        <translation>Przerwij sesję, gdy punkt krytyczny urządzenia OpenCL przekroczy tę liczbę stopni Celsjusza. 0 wyłącza limit temperatury.</translation>
    </message>
    <message>
        <source>Outfile</source>
        <translation>Plik wyjściowy</translation>
    </message>
    <message>
        <source>Write recovered hashes to file:</source>
        <translation>Zapisz odzyskane hashe do pliku:</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Format:</translation>
    </message>
    <message>
        <source>Write the outfile as JSON. The outfile format is ignored.</source>
        <translation>Zapisz plik wyjściowy jako JSON. Format pliku wyjściowego jest ignorowany.</translation>
    </message>
    <message>
        <source>Write JSON</source>
        <translation>Zapisz JSON</translation>
    </message>
    <message>
        <source>Do not write the potfile</source>
        <translation>Nie zapisuj pliku potfile</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>Start</translation>
    </message>
    <message>
        <source>Copy command to clipboard</source>
        <translation>Kopiuj polecenie do schowka</translation>
    </message>
    <message>
        <source>Help</source>
        <translation>Pomoc</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Narzędzia</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Plik</translation>
    </message>
    <message>
        <source>About</source>
        <translation>O programie</translation>
    </message>
    <message>
        <source>Load Example Session</source>
        <translation>Wczytaj przykładową sesję</translation>
    </message>
    <message>
        <source>Reset Session</source>
        <translation>Zresetuj sesję</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation>Zakończ</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>O Qt</translation>
    </message>
    <message>
        <source>Built-in charsets:</source>
        <translation>Wbudowane zestawy znaków:</translation>
    </message>
    <message>
        <source>Outfile format to use, separated by commas.
Default: 1,2</source>
        <translation>Format pliku wyjściowego, oddzielony przecinkami.
Domyślnie: 1,2</translation>
    </message>
    <message>
        <source>Common encodings:</source>
        <translation>Popularne kodowania:</translation>
    </message>
    <message>
        <source>Force the internal wordlist encoding from this code. Leave empty to let hashcat decide.</source>
        <translation>Wymuś wewnętrzne kodowanie listy słów z tego kodu. Pozostaw puste, aby hashcat zdecydował.</translation>
    </message>
    <message>
        <source>Force the internal wordlist encoding to this code. Leave empty to let hashcat decide.</source>
        <translation>Wymuś wewnętrzne kodowanie listy słów na ten kod. Pozostaw puste, aby hashcat zdecydował.</translation>
    </message>
    <message>
        <source>JSON Files (*.json)</source>
        <translation>Pliki JSON (*.json)</translation>
    </message>
    <message>
        <source>Exported</source>
        <translation>Wyeksportowano</translation>
    </message>
    <message>
        <source>Export failed</source>
        <translation>Eksport nie powiódł się</translation>
    </message>
    <message>
        <source>Import failed</source>
        <translation>Import nie powiódł się</translation>
    </message>
    <message>
        <source>Imported</source>
        <translation>Zaimportowano</translation>
    </message>
    <message>
        <source>The file was made by an older version of hashcat-gui (format version %1; this version reads format version %2).</source>
        <translation>Plik został utworzony przez starszą wersję hashcat-gui (wersja formatu %1; ta wersja odczytuje wersję formatu %2).</translation>
    </message>
    <message>
        <source>The file holds sections this import does not use: %1.</source>
        <translation>Plik zawiera sekcje, których ten import nie używa: %1.</translation>
    </message>
    <message>
        <source>The file is in format version %1, but this version reads format version %2, so it was not applied. Update hashcat-gui to read it.</source>
        <translation>Plik ma wersję formatu %1, ale ta wersja odczytuje wersję formatu %2, więc nie został zastosowany. Zaktualizuj hashcat-gui, aby go odczytać.</translation>
    </message>
    <message>
        <source>The session file is in format version %1, but this version reads format version %2, so the session was not restored. The file was left as it is, so the session of this run is not stored either. Update hashcat-gui to read it.</source>
        <translation>Plik sesji ma wersję formatu %1, ale ta wersja odczytuje wersję formatu %2, więc sesja nie została przywrócona. Plik pozostawiono bez zmian, więc sesja z tego uruchomienia również nie zostanie zapisana. Zaktualizuj hashcat-gui, aby go odczytać.</translation>
    </message>
    <message>
        <source>Settings exported to %1.</source>
        <translation>Ustawienia wyeksportowano do %1.</translation>
    </message>
    <message>
        <source>OpenCL device types to use, separated by commas.</source>
        <translation>Typy urządzeń OpenCL do użycia, oddzielone przecinkami.</translation>
    </message>
    <message>
        <source>Export session</source>
        <translation>Eksport sesji</translation>
    </message>
    <message>
        <source>Session exported to “%1”.</source>
        <translation>Wyeksportowano sesję do „%1”.</translation>
    </message>
    <message>
        <source>Import session</source>
        <translation>Import sesji</translation>
    </message>
    <message>
        <source>Session imported from “%1”.</source>
        <translation>Zaimportowano sesję z „%1”.</translation>
    </message>
    <message>
        <source>Session imported from “%1”.

%2</source>
        <translation>Zaimportowano sesję z „%1”.

%2</translation>
    </message>
    <message>
        <source>Export settings</source>
        <translation>Eksport ustawień</translation>
    </message>
    <message>
        <source>Import settings</source>
        <translation>Import ustawień</translation>
    </message>
    <message>
        <source>Settings loaded from “%1”.</source>
        <translation>Wczytano ustawienia z „%1”.</translation>
    </message>
    <message>
        <source>Review them and save to apply them.</source>
        <translation>Sprawdź je i zapisz, aby je zastosować.</translation>
    </message>
    <message>
        <source>Load example session</source>
        <translation>Wczytaj przykładową sesję</translation>
    </message>
    <message>
        <source>Reset session</source>
        <translation>Zresetuj sesję</translation>
    </message>
    <message>
        <source>Reset every field to its default? The current session is lost.</source>
        <translation>Przywrócić domyślną wartość każdego pola? Bieżąca sesja zostanie utracona.</translation>
    </message>
    <message>
        <source>Session warning</source>
        <translation>Ostrzeżenie o sesji</translation>
    </message>
    <message>
        <source>The session file could not be read, so the session was not restored. The file was left in place.

%1</source>
        <translation>Nie można odczytać pliku sesji, więc sesja nie została przywrócona. Plik pozostawiono bez zmian.

%1</translation>
    </message>
    <message>
        <source>The session names the hash type %1, which this hashcat version does not support.</source>
        <translation>Sesja wskazuje typ hasha %1, którego ta wersja hashcat nie obsługuje.</translation>
    </message>
    <message>
        <source>Save failed</source>
        <translation>Zapis nie powiódł się</translation>
    </message>
    <message>
        <source>Straight</source>
        <translation>Słownikowy</translation>
    </message>
    <message>
        <source>Combination</source>
        <translation>Kombinacja</translation>
    </message>
    <message>
        <source>Brute-force</source>
        <translation>Brute-force</translation>
    </message>
    <message>
        <source>Hybrid Wordlist + Mask</source>
        <translation>Hybrydowy: lista słów + maska</translation>
    </message>
    <message>
        <source>Hybrid Mask + Wordlist</source>
        <translation>Hybrydowy: maska + lista słów</translation>
    </message>
    <message>
        <source>Generic (feed)</source>
        <translation>Generyczny (feed)</translation>
    </message>
    <message>
        <source>Association</source>
        <translation>Asocjacja</translation>
    </message>
    <message>
        <source>Hybrid (mask + ?w/?q)</source>
        <translation>Hybrydowy (maska + ?w/?q)</translation>
    </message>
    <message>
        <source>Updating…</source>
        <translation>Aktualizowanie…</translation>
    </message>
    <message>
        <source>Failed to load hash types</source>
        <translation>Nie udało się wczytać typów hashy</translation>
    </message>
    <message>
        <source>This mode needs exactly 2 wordlists (left, right).</source>
        <translation>Ten tryb wymaga dokładnie 2 list słów (lewej, prawej).</translation>
    </message>
    <message>
        <source>This mode runs hashcat’s bundled default-passwords ruleset; name one or more to use your own.</source>
        <translation>Ten tryb uruchamia dołączony do hashcat zestaw reguł default-passwords; podaj jeden lub więcej, aby użyć własnych.</translation>
    </message>
    <message>
        <source>This mode needs one wordlist and at least one table.</source>
        <translation>Ten tryb wymaga jednej listy słów i co najmniej jednej tabeli.</translation>
    </message>
    <message>
        <source>This mode needs a feed. Arguments depend on the feed — random and stdin need none, wordlist needs a path.</source>
        <translation>Ten tryb wymaga feedu. Argumenty zależą od feedu: random i stdin żadnych nie potrzebują, wordlist potrzebuje ścieżki.</translation>
    </message>
    <message>
        <source>This mode needs at most one wordlist. Without one the words come from the hash file and the settings line can bound the run; with one the two files are paired by line.</source>
        <translation>Ten tryb wymaga co najwyżej jednej listy słów. Bez niej słowa pochodzą z pliku z hashami, a wiersz ustawień może ograniczyć przebieg; z listą oba pliki są łączone wiersz po wierszu.</translation>
    </message>
    <message>
        <source>This mode needs a mask with exactly one ?w; a ?q after ?w names a second wordlist.</source>
        <translation>Ten tryb wymaga maski z dokładnie jednym ?w; ?q po ?w wskazuje drugą listę słów.</translation>
    </message>
    <message>
        <source>Open hash file</source>
        <translation>Otwórz plik z hashami</translation>
    </message>
    <message>
        <source>Save output file</source>
        <translation>Zapisz plik wyjściowy</translation>
    </message>
    <message>
        <source>Open cache directory</source>
        <translation>Otwórz katalog pamięci podręcznej</translation>
    </message>
    <message>
        <source>Add wordlists</source>
        <translation>Dodaj listy słów</translation>
    </message>
    <message>
        <source>Add wordlist folder</source>
        <translation>Dodaj folder z listami słów</translation>
    </message>
    <message>
        <source>Add rulesets</source>
        <translation>Dodaj zestawy reguł</translation>
    </message>
    <message>
        <source>Add ruleset folder</source>
        <translation>Dodaj folder zestawu reguł</translation>
    </message>
    <message>
        <source>Add tables</source>
        <translation>Dodaj tabele</translation>
    </message>
    <message>
        <source>Open feed file</source>
        <translation>Otwórz plik feedu</translation>
    </message>
    <message>
        <source>This folder has no wordlists. hashcat reads the regular files directly inside it, not subfolders.</source>
        <translation>Ten folder nie zawiera list słów. hashcat odczytuje zwykłe pliki znajdujące się bezpośrednio w nim, a nie podfoldery.</translation>
    </message>
    <message>
        <source>This ruleset folder has no Grammar/grammar.txt:
%1</source>
        <translation>Ten folder zestawu reguł nie zawiera pliku Grammar/grammar.txt:
%1</translation>
    </message>
    <message>
        <source>This ruleset does not exist:
%1</source>
        <translation>Ten zestaw reguł nie istnieje:
%1</translation>
    </message>
    <message>
        <source>A table must be a file; hashcat does not read a folder:
%1</source>
        <translation>Tabela musi być plikiem; hashcat nie odczytuje folderu:
%1</translation>
    </message>
    <message>
        <source>This table does not exist:
%1</source>
        <translation>Ta tabela nie istnieje:
%1</translation>
    </message>
    <message>
        <source>Open rules file</source>
        <translation>Otwórz plik reguł</translation>
    </message>
    <message>
        <source>Could not start “%1”: %2</source>
        <translation>Nie udało się uruchomić „%1”: %2</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not available. Available terminals: %2
Navigate to %3 → %4 to change it.</source>
        <translation>Skonfigurowany terminal „%1” jest niedostępny. Dostępne terminale: %2
Przejdź do %3 → %4, aby to zmienić.</translation>
    </message>
    <message>
        <source>This mode needs exactly %1 wordlists (left, right).
Currently selected: %2.</source>
        <translation>Ten tryb wymaga dokładnie %1 list słów (lewej, prawej).
Aktualnie wybrano: %2.</translation>
    </message>
    <message>
        <source>Failed to obtain supported hash types.
Error: %1</source>
        <translation>Nie udało się pobrać obsługiwanych typów hashy.
Błąd: %1</translation>
    </message>
    <message>
        <source>The hash types could not be loaded. Check the hashcat path in Settings.</source>
        <translation>Nie udało się wczytać typów hashy. Sprawdź ścieżkę do hashcat w ustawieniach.</translation>
    </message>
    <message>
        <source>Set the hashcat path under %1 → %2 to load the available hash types.</source>
        <translation>Ustaw ścieżkę do hashcat w %1 → %2, aby wczytać dostępne typy hashy.</translation>
    </message>
    <message>
        <source>This mode needs a wordlist and a mask.</source>
        <translation>Ten tryb wymaga listy słów i maski.</translation>
    </message>
    <message>
        <source>This mode needs a mask.</source>
        <translation>Ten tryb wymaga maski.</translation>
    </message>
    <message>
        <source>Load the %1 example? The current session is lost.

The example cracks a test hash; hashcat should recover the password %2.</source>
        <translation>Wczytać przykład %1? Bieżąca sesja zostanie utracona.

Przykład łamie testowy hash; hashcat powinien odzyskać hasło %2.</translation>
    </message>
    <message>
        <source>Could not create the example directory “%1”.</source>
        <translation>Nie udało się utworzyć katalogu przykładu „%1”.</translation>
    </message>
    <message>
        <source>Could not read the bundled file “%1”: %2</source>
        <translation>Nie udało się odczytać dołączonego pliku „%1”: %2</translation>
    </message>
    <message>
        <source>Could not write “%1”: %2</source>
        <translation>Nie udało się zapisać „%1”: %2</translation>
    </message>
    <message>
        <source>This mode needs at least one wordlist.</source>
        <translation>Ten tryb wymaga co najmniej jednej listy słów.</translation>
    </message>
    <message>
        <source>This wordlist does not exist:
%1</source>
        <translation>Ta lista słów nie istnieje:
%1</translation>
    </message>
    <message>
        <source>Copied to the clipboard</source>
        <translation>Skopiowano do schowka</translation>
    </message>
    <message>
        <source>Launch failed</source>
        <translation>Uruchomienie nie powiodło się</translation>
    </message>
    <message>
        <source>Please choose a hash file.</source>
        <translation>Wybierz plik z hashami.</translation>
    </message>
    <message>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to configure the path to the hashcat executable.</source>
        <translation>Przejdź do &lt;b&gt;%1 → %2&lt;/b&gt;, aby skonfigurować ścieżkę do pliku wykonywalnego hashcat.</translation>
    </message>
    <message>
        <source>The configured hashcat executable does not exist:
%1
Navigate to %2 → %3 to change it.</source>
        <translation>Skonfigurowany plik wykonywalny hashcat nie istnieje:
%1
Przejdź do %2 → %3, aby to zmienić.</translation>
    </message>
    <message>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to select the terminal used for launching.</source>
        <translation>Przejdź do &lt;b&gt;%1 → %2&lt;/b&gt;, aby wybrać terminal używany do uruchamiania.</translation>
    </message>
    <message>
        <source>This mode takes at most %1 wordlists.
Currently selected: %2.</source>
        <translation>Ten tryb przyjmuje co najwyżej %1 list słów.
Aktualnie wybrano: %2.</translation>
    </message>
    <message>
        <source>hashcat pairs the two files by line here; this GUI offers only one wordlist for that role.</source>
        <translation>hashcat łączy tu oba pliki wiersz po wierszu; ten interfejs oferuje dla tej roli tylko jedną listę słów.</translation>
    </message>
    <message>
        <source>This wordlist has to be a file: hashcat reads it as the multiplied wordlist (the ?q side), and a folder cannot be used there:
%1</source>
        <translation>Ta lista słów musi być plikiem: hashcat odczytuje ją jako mnożoną listę słów (strona ?q), a folder nie może być tam użyty:
%1</translation>
    </message>
    <message>
        <source>This mode needs at least one table.</source>
        <translation>Ten tryb wymaga co najmniej jednej tabeli.</translation>
    </message>
    <message>
        <source>This mode needs a feed: a bundled name (wordlist, stdin, random, …) or the path of a feed file.</source>
        <translation>Ten tryb wymaga feedu: dołączonej nazwy (wordlist, stdin, random, …) lub ścieżki pliku feedu.</translation>
    </message>
    <message>
        <source>This mode pairs the wordlists with the mask; the mask needs a ?w for the word.</source>
        <translation>Ten tryb łączy listy słów z maską; maska wymaga ?w dla słowa.</translation>
    </message>
    <message>
        <source>The mask contains more than one ?w.</source>
        <translation>Maska zawiera więcej niż jedno ?w.</translation>
    </message>
    <message>
        <source>The mask contains a ?q before the first ?w.</source>
        <translation>Maska zawiera ?q przed pierwszym ?w.</translation>
    </message>
    <message>
        <source>The mask contains more than one ?q.</source>
        <translation>Maska zawiera więcej niż jedno ?q.</translation>
    </message>
    <message>
        <source>The mask names a second word with ?q, but only one wordlist is selected.</source>
        <translation>Maska wskazuje drugie słowo za pomocą ?q, ale wybrano tylko jedną listę słów.</translation>
    </message>
    <message>
        <source>Two wordlists are selected, but the mask has no ?q for the second one.</source>
        <translation>Wybrano dwie listy słów, ale maska nie ma ?q dla drugiej.</translation>
    </message>
    <message>
        <source>The increment is not offered in this hybrid mode.</source>
        <translation>Przyrost nie jest dostępny w tym trybie hybrydowym.</translation>
    </message>
    <message>
        <source>The ?w and ?q markers are only offered in the hybrid mode (mask + wordlists).</source>
        <translation>Znaczniki ?w i ?q są dostępne tylko w trybie hybrydowym (maska + listy słów).</translation>
    </message>
    <message>
        <source>The setting “%1” is not valid for this mode.</source>
        <translation>Ustawienie „%1” nie jest prawidłowe dla tego trybu.</translation>
    </message>
    <message>
        <source>The token “%1” is not a setting; settings are written as key=value.</source>
        <translation>Token „%1” nie jest ustawieniem; ustawienia zapisuje się jako key=value.</translation>
    </message>
    <message>
        <source>The settings line applies only while the words come from the hash file. Clear either the settings line or the wordlist list.</source>
        <translation>Wiersz ustawień obowiązuje tylko wtedy, gdy słowa pochodzą z pliku z hashami. Wyczyść wiersz ustawień albo listę list słów.</translation>
    </message>
    <message>
        <source>The hash type 4000 describes its hashes as a recipe, for example
md5(md5(salt) . pass). Enter it in the hash recipe field.</source>
        <translation>Typ hasha 4000 opisuje swoje hashe jako przepis, na przykład
md5(md5(salt) . pass). Wpisz go w polu przepisu na hash.</translation>
    </message>
    <message>
        <source>The minimum increment length is larger than the maximum.</source>
        <translation>Minimalna długość przyrostu jest większa od maksymalnej.</translation>
    </message>
    <message>
        <source>The cache path must be an existing directory:
%1</source>
        <translation>Ścieżka pamięci podręcznej musi wskazywać istniejący katalog:
%1</translation>
    </message>
    <message>
        <source>The increment cannot be combined with the speed-only option.</source>
        <translation>Przyrostu nie można łączyć z opcją samej szybkości.</translation>
    </message>
    <message>
        <source>Removing cracked hashes is not possible in the association attack mode.</source>
        <translation>Usuwanie złamanych hashy nie jest możliwe w trybie ataku asocjacyjnego.</translation>
    </message>
</context>
<context>
    <name>SettingsDialog</name>
    <message>
        <source>Settings</source>
        <translation>Ustawienia</translation>
    </message>
    <message>
        <source>Not configured</source>
        <translation>Nie skonfigurowano</translation>
    </message>
    <message>
        <source>hashcat</source>
        <translation>hashcat</translation>
    </message>
    <message>
        <source>&amp;hashcat executable</source>
        <translation>&amp;Plik wykonywalny hashcat</translation>
    </message>
    <message>
        <source>Select…</source>
        <translation>Wybierz…</translation>
    </message>
    <message>
        <source>&amp;Terminal</source>
        <translation>&amp;Terminal</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Ogólne</translation>
    </message>
    <message>
        <source>&amp;Language</source>
        <translation>&amp;Język</translation>
    </message>
    <message>
        <source>&amp;Color scheme</source>
        <translation>Schemat &amp;kolorów</translation>
    </message>
    <message>
        <source>Not all color schemes are available on all platforms.</source>
        <translation>Nie wszystkie schematy kolorów są dostępne na wszystkich platformach.</translation>
    </message>
    <message>
        <source>&amp;Use short parameters</source>
        <translation>&amp;Używaj krótkich parametrów</translation>
    </message>
    <message>
        <source>Session and files</source>
        <translation>Sesja i pliki</translation>
    </message>
    <message>
        <source>&amp;Restore the last session at startup</source>
        <translation>Przywróć &amp;ostatnią sesję przy uruchomieniu</translation>
    </message>
    <message>
        <source>&amp;Session file</source>
        <translation>Plik &amp;sesji</translation>
    </message>
    <message>
        <source>The file the last session is stored in</source>
        <translation>Plik, w którym zapisana jest ostatnia sesja</translation>
    </message>
    <message>
        <source>Open folder</source>
        <translation>Otwórz folder</translation>
    </message>
    <message>
        <source>Open the folder the session is stored in</source>
        <translation>Otwórz folder, w którym zapisana jest sesja</translation>
    </message>
    <message>
        <source>Settings &amp;file</source>
        <translation>Plik ustaw&amp;ień</translation>
    </message>
    <message>
        <source>The file the settings are stored in</source>
        <translation>Plik, w którym zapisane są ustawienia</translation>
    </message>
    <message>
        <source>Open the folder the settings are stored in</source>
        <translation>Otwórz folder, w którym zapisane są ustawienia</translation>
    </message>
    <message>
        <source>Download from %1</source>
        <translation>Pobierz z %1</translation>
    </message>
    <message>
        <source>Use the short option names (-m) instead of the long ones (--hash-type) in the command.</source>
        <translation>Używaj w poleceniu krótkich nazw opcji (-m) zamiast długich (--hash-type).</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Zapisz</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Anuluj</translation>
    </message>
    <message>
        <source>Select a terminal</source>
        <translation>Wybierz terminal</translation>
    </message>
    <message>
        <source>System default</source>
        <translation>Domyślny systemowy</translation>
    </message>
    <message>
        <source>Light</source>
        <translation>Jasny</translation>
    </message>
    <message>
        <source>Dark</source>
        <translation>Ciemny</translation>
    </message>
    <message>
        <source>The hashcat path “%1” was not found on this computer.</source>
        <translation>Nie znaleziono ścieżki hashcat „%1” na tym komputerze.</translation>
    </message>
    <message>
        <source>The terminal “%1” is not installed and was not applied.</source>
        <translation>Terminal „%1” nie jest zainstalowany i nie został zastosowany.</translation>
    </message>
    <message>
        <source>The color scheme “%1” is not offered and was not applied.</source>
        <translation>Schemat kolorów „%1” nie jest oferowany i nie został zastosowany.</translation>
    </message>
    <message>
        <source>The language “%1” is not offered and was not applied.</source>
        <translation>Język „%1” nie jest oferowany i nie został zastosowany.</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not installed.</source>
        <translation>Skonfigurowany terminal „%1” nie jest zainstalowany.</translation>
    </message>
    <message>
        <source>Executable Files (*.exe *.bin hashcat);;All Files (*)</source>
        <translation>Pliki wykonywalne (*.exe *.bin hashcat);;Wszystkie pliki (*)</translation>
    </message>
    <message>
        <source>No hashcat executable is configured; hashcat cannot be started.</source>
        <translation>Nie skonfigurowano pliku wykonywalnego hashcat; nie można uruchomić hashcat.</translation>
    </message>
    <message>
        <source>The configured hashcat executable was not found on this computer.</source>
        <translation>Skonfigurowany plik wykonywalny hashcat nie został znaleziony na tym komputerze.</translation>
    </message>
    <message>
        <source>Select the terminal hashcat is started in.</source>
        <translation>Wybierz terminal, w którym uruchamiany jest hashcat.</translation>
    </message>
    <message>
        <source>Select the hashcat executable</source>
        <translation>Wybierz plik wykonywalny hashcat</translation>
    </message>
    <message>
        <source>Invalid file</source>
        <translation>Nieprawidłowy plik</translation>
    </message>
    <message>
        <source>The selected file is not an executable.</source>
        <translation>Wybrany plik nie jest plikiem wykonywalnym.</translation>
    </message>
    <message>
        <source>Restart required</source>
        <translation>Wymagane ponowne uruchomienie</translation>
    </message>
    <message>
        <source>Restart hashcat-gui to apply the new language.</source>
        <translation>Uruchom ponownie hashcat-gui, aby zastosować nowy język.</translation>
    </message>
</context>
<context>
    <name>StateFile</name>
    <message>
        <source>The file is not valid JSON: %1</source>
        <translation>Plik nie jest prawidłowym JSON-em: %1</translation>
    </message>
    <message>
        <source>The file does not contain a JSON object.</source>
        <translation>Plik nie zawiera obiektu JSON.</translation>
    </message>
    <message>
        <source>The file does not contain any sections.</source>
        <translation>Plik nie zawiera żadnych sekcji.</translation>
    </message>
    <message>
        <source>Could not open “%1” for writing: %2</source>
        <translation>Nie udało się otworzyć „%1” do zapisu: %2</translation>
    </message>
    <message>
        <source>Could not write to “%1”: %2</source>
        <translation>Nie udało się zapisać do „%1”: %2</translation>
    </message>
    <message>
        <source>Could not open “%1” for reading: %2</source>
        <translation>Nie udało się otworzyć „%1” do odczytu: %2</translation>
    </message>
    <message>
        <source>The file does not contain a “%1” section.</source>
        <translation>Plik nie zawiera sekcji „%1”.</translation>
    </message>
    <message>
        <source>The file does not contain a “%1” section; it holds %2.</source>
        <translation>Plik nie zawiera sekcji „%1”; zawiera %2.</translation>
    </message>
    <message>
        <source>The “%1” section does not contain any values.</source>
        <translation>Sekcja „%1” nie zawiera żadnych wartości.</translation>
    </message>
    <message>
        <source> and </source>
        <translation> i </translation>
    </message>
    <message>
        <source> and %1 more</source>
        <translation> i jeszcze %1</translation>
    </message>
    <message>
        <source>The file names values this version does not know, so they were not applied: %1.</source>
        <translation>Plik zawiera wartości, których ta wersja nie zna, więc nie zostały zastosowane: %1.</translation>
    </message>
    <message>
        <source>The file holds values of the wrong type, so they were not applied: %1.</source>
        <translation>Plik zawiera wartości niewłaściwego typu, więc nie zostały zastosowane: %1.</translation>
    </message>
</context>
<context>
    <name>UpdateChecker</name>
    <message>
        <source>GitHub answered with HTTP status %1: %2</source>
        <translation>GitHub odpowiedział statusem HTTP %1: %2</translation>
    </message>
    <message>
        <source>GitHub answered with HTTP status %1.</source>
        <translation>GitHub odpowiedział statusem HTTP %1.</translation>
    </message>
    <message>
        <source>The release information is not valid JSON.</source>
        <translation>Informacje o wydaniu nie są prawidłowym JSON-em.</translation>
    </message>
    <message>
        <source>The release information does not name a version.</source>
        <translation>Informacje o wydaniu nie wskazują wersji.</translation>
    </message>
    <message>
        <source>The release version %1 is not a valid version number.</source>
        <translation>Wersja wydania %1 nie jest prawidłowym numerem wersji.</translation>
    </message>
    <message>
        <source>The installed version %1 is not a valid version number.</source>
        <translation>Zainstalowana wersja %1 nie jest prawidłowym numerem wersji.</translation>
    </message>
</context>
</TS>
