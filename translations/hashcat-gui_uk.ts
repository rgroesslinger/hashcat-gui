<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="uk" sourcelanguage="en">
<context>
    <name>AboutDialog</name>
    <message>
        <source>About</source>
        <translation>Про програму</translation>
    </message>
    <message>
        <source>Graphical user interface for hashcat, the password recovery utility</source>
        <translation>Графічний інтерфейс для hashcat — утиліти відновлення паролів</translation>
    </message>
    <message>
        <source>Version information</source>
        <translation>Відомості про версію</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Невідомо</translation>
    </message>
    <message>
        <source>Updates</source>
        <translation>Оновлення</translation>
    </message>
    <message>
        <source>Checking for updates…</source>
        <translation>Перевірка оновлень…</translation>
    </message>
    <message>
        <source>System information</source>
        <translation>Відомості про систему</translation>
    </message>
    <message>
        <source>Operating system</source>
        <translation>Операційна система</translation>
    </message>
    <message>
        <source>Links</source>
        <translation>Посилання</translation>
    </message>
    <message>
        <source>License</source>
        <translation>Ліцензія</translation>
    </message>
    <message>
        <source>Copy the application, toolkit, system and hashcat versions to the clipboard</source>
        <translation>Скопіювати до буфера обміну версії застосунку, бібліотеки, системи та hashcat</translation>
    </message>
    <message>
        <source>Copy details</source>
        <translation>Копіювати відомості</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Copied to the clipboard</source>
        <translation>Скопійовано до буфера обміну</translation>
    </message>
    <message>
        <source>Project page</source>
        <translation>Сторінка проєкту</translation>
    </message>
    <message>
        <source>Report an issue</source>
        <translation>Повідомити про проблему</translation>
    </message>
    <message>
        <source>hashcat</source>
        <translation>hashcat</translation>
    </message>
    <message>
        <source>hashcat FAQ</source>
        <translation>ЧаП hashcat</translation>
    </message>
    <message>
        <source>hashcat-gui is licensed under the %1.</source>
        <translation>hashcat-gui поширюється за ліцензією %1.</translation>
    </message>
    <message>
        <source>GNU GPL v3.0 or later</source>
        <translation>GNU GPL v3.0 або новішою</translation>
    </message>
    <message>
        <source>Not configured</source>
        <translation>Не налаштовано</translation>
    </message>
    <message>
        <source>Error: %1</source>
        <translation>Помилка: %1</translation>
    </message>
    <message>
        <source>A newer version is available: %1.</source>
        <translation>Доступна новіша версія: %1.</translation>
    </message>
    <message>
        <source>Release page</source>
        <translation>Сторінка випуску</translation>
    </message>
    <message>
        <source>No updates available.</source>
        <translation>Оновлень немає.</translation>
    </message>
    <message>
        <source>Could not check for updates.</source>
        <translation>Не вдалося перевірити наявність оновлень.</translation>
    </message>
    <message>
        <source>Could not check for updates: %1</source>
        <translation>Не вдалося перевірити наявність оновлень: %1</translation>
    </message>
</context>
<context>
    <name>HashcatInfoParser</name>
    <message>
        <source>Invalid JSON returned from hashcat.</source>
        <translation>hashcat повернув недійсний JSON.</translation>
    </message>
    <message>
        <source>hashcat did not report any hash types.</source>
        <translation>hashcat не повідомив про жоден тип гешів.</translation>
    </message>
</context>
<context>
    <name>HelperUtils</name>
    <message>
        <source>The hashcat path is not configured.</source>
        <translation>Шлях до hashcat не налаштовано.</translation>
    </message>
    <message>
        <source>Failed to start hashcat
%1</source>
        <translation>Не вдалося запустити hashcat
%1</translation>
    </message>
    <message>
        <source>hashcat timed out
%1</source>
        <translation>Вичерпано час очікування hashcat
%1</translation>
    </message>
    <message>
        <source>No supported terminal was found on this system. hashcat-gui needs one to show the hashcat output.</source>
        <translation>У цій системі не знайдено підтримуваного термінала. hashcat-gui потрібен термінал, щоб показувати вивід hashcat.</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <source>Attack</source>
        <translation>Атака</translation>
    </message>
    <message>
        <source>Hash file or hash:</source>
        <translation>Файл гешів або геш:</translation>
    </message>
    <message>
        <source>The hash to crack, a path to a file of hashes, or a .hccapx WPA capture. Paste a single hash here, or point it at a file.</source>
        <translation>Геш для злому, шлях до файлу з гешами або дамп WPA у форматі .hccapx. Вставте сюди один геш або вкажіть файл.</translation>
    </message>
    <message>
        <source>Salt is hex</source>
        <translation>Сіль у hex</translation>
    </message>
    <message>
        <source>Remove already-cracked hashes from the potfile so a later run does not report them again. Refused in attack mode 9.</source>
        <translation>Вилучати вже зламані геші з potfile, щоб наступний запуск не повідомляв про них знову. Відхиляється в режимі атаки 9.</translation>
    </message>
    <message>
        <source>Remove hash from hash list once it is cracked</source>
        <translation>Вилучати геш зі списку гешів після злому</translation>
    </message>
    <message>
        <source>Ignore the @username part of hashes (in hash modes that carry one) so the same word can satisfy several accounts.</source>
        <translation>Ігнорувати частину @username у гешах (у режимах гешів, що її містять), щоб те саме слово могло підійти до кількох облікових записів.</translation>
    </message>
    <message>
        <source>Ignore username in hash file</source>
        <translation>Ігнорувати ім’я користувача у файлі гешів</translation>
    </message>
    <message>
        <source>Mode:</source>
        <translation>Режим:</translation>
    </message>
    <message>
        <source>Attack mode. Each choice reveals its own options below.</source>
        <translation>Режим атаки. Кожен варіант показує нижче власні параметри.</translation>
    </message>
    <message>
        <source>Hash type:</source>
        <translation>Тип геша:</translation>
    </message>
    <message>
        <source>Hash recipe:</source>
        <translation>Рецепт геша:</translation>
    </message>
    <message>
        <source>Unix: &apos;md5(md5(salt) . pass)&apos;
               Windows: &quot;md5(&apos;abc&apos; . pass)&quot;</source>
        <translation>Unix: &apos;md5(md5(salt) . pass)&apos;
               Windows: &quot;md5(&apos;abc&apos; . pass)&quot;</translation>
    </message>
    <message>
        <source>Wordlists</source>
        <translation>Словники</translation>
    </message>
    <message>
        <source>Move item up</source>
        <translation>Перемістити елемент угору</translation>
    </message>
    <message>
        <source>Move item down</source>
        <translation>Перемістити елемент униз</translation>
    </message>
    <message>
        <source>hashcat reads every file directly inside the folder as one wordlist.</source>
        <translation>hashcat читає кожен файл, що лежить безпосередньо в теці, як один словник.</translation>
    </message>
    <message>
        <source>Add folder…</source>
        <translation>Додати теку…</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Вилучити</translation>
    </message>
    <message>
        <source>Only checked rows are used. A folder is one wordlist: hashcat reads the files directly inside it.</source>
        <translation>Використовуються лише позначені рядки. Тека — це один словник: hashcat читає файли, що лежать безпосередньо в ній.</translation>
    </message>
    <message>
        <source>Wordlist is hex</source>
        <translation>Словник у hex</translation>
    </message>
    <message>
        <source>Encoding from:</source>
        <translation>Кодування з:</translation>
    </message>
    <message>
        <source>Encoding to:</source>
        <translation>Кодування в:</translation>
    </message>
    <message>
        <source>Settings (no wordlist):</source>
        <translation>Налаштування (без словника):</translation>
    </message>
    <message>
        <source>Positional settings, separated by spaces. Only used when no wordlist is selected: with one, hashcat pairs the two files by line. “phases” picks which phases run, cheapest first; the default “words,rules,pcfg” ends only when stopped. Phase-prefixed settings: “rules.rulefile”, “rules.rulemax”, “words.hints”, “pcfg.hintaccount”, …</source>
        <translation>Позиційні налаштування через пробіл. Використовуються, лише коли словник не вибрано: за вибраного словника hashcat зіставляє два файли порядково. «phases» визначає, які фази виконуються, починаючи з найдешевшої; типове значення «words,rules,pcfg» завершується лише після зупинки. Налаштування з префіксом фази: «rules.rulefile», «rules.rulemax», «words.hints», «pcfg.hintaccount», …</translation>
    </message>
    <message>
        <source>PCFG</source>
        <translation>PCFG</translation>
    </message>
    <message>
        <source>A row is a ruleset file, a folder with a Grammar/grammar.txt, or a bundled name. Only checked rows are used; with none checked, hashcat’s bundled default-passwords runs. hints is also bundled.</source>
        <translation>Рядок — це файл набору правил, тека з Grammar/grammar.txt або вбудована назва. Використовуються лише позначені рядки; якщо жодного не позначено, запускається вбудований у hashcat default-passwords. hints також вбудований.</translation>
    </message>
    <message>
        <source>A ruleset folder needs a Grammar/grammar.txt.</source>
        <translation>Теці набору правил потрібен файл Grammar/grammar.txt.</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Scale:</translation>
    </message>
    <message>
        <source>Quantization steps per bit (1–64)</source>
        <translation>Кроків квантування на біт (1–64)</translation>
    </message>
    <message>
        <source>Costmax:</source>
        <translation>Costmax:</translation>
    </message>
    <message>
        <source>Highest cost level to enumerate (1–64)</source>
        <translation>Найвищий рівень вартості для перебору (1–64)</translation>
    </message>
    <message>
        <source>Weights:</source>
        <translation>Weights:</translation>
    </message>
    <message>
        <source>One value per ruleset, plain numbers separated by colons, e.g. 2:1 for two rulesets. The count must match the number of rulesets.</source>
        <translation>Одне значення на набір правил, прості числа через двокрапку, наприклад 2:1 для двох наборів правил. Кількість значень має збігатися з кількістю наборів правил.</translation>
    </message>
    <message>
        <source>Threads:</source>
        <translation>Потоки:</translation>
    </message>
    <message>
        <source>Cores that generate base words (0–16); 0 runs on the calling thread</source>
        <translation>Ядра, що генерують базові слова (0–16); 0 виконує роботу у викликаючому потоці</translation>
    </message>
    <message>
        <source>auto</source>
        <translation>авто</translation>
    </message>
    <message>
        <source>Kbits:</source>
        <translation>Kbits:</translation>
    </message>
    <message>
        <source>Bits one inner loop may span (0–27)</source>
        <translation>Кількість бітів, яку може охопити один внутрішній цикл (0–27)</translation>
    </message>
    <message>
        <source>Maxword:</source>
        <translation>Maxword:</translation>
    </message>
    <message>
        <source>Candidates the kernel builds per base word: 0, 16 or 32 (a multiple of 16)</source>
        <translation>Кандидати, які ядро будує на одне базове слово: 0, 16 або 32 (кратно 16)</translation>
    </message>
    <message>
        <source>Maxgain:</source>
        <translation>Maxgain:</translation>
    </message>
    <message>
        <source>1.0–64.0; decimal point is a dot, not the locale separator</source>
        <translation>1.0–64.0; десятковим роздільником є крапка, а не роздільник із регіональних налаштувань</translation>
    </message>
    <message>
        <source>Walk:</source>
        <translation>Walk:</translation>
    </message>
    <message>
        <source>1 steps the last token instead of unraking it</source>
        <translation>1 просуває останній токен покроково, замість того щоб застосовувати до нього «unraking»</translation>
    </message>
    <message>
        <source>Mask:</source>
        <translation>Маска:</translation>
    </message>
    <message>
        <source>Hold the grammar to the candidates this mask admits; cannot be combined with omen=1, or with hintaccount while the hint words come from the accounts (give hintwords or hintfile instead)</source>
        <translation>Обмежити граматику кандидатами, які допускає ця маска; не можна поєднувати з omen=1, а також із hintaccount, поки слова-підказки беруться з облікових записів (натомість укажіть hintwords або hintfile)</translation>
    </message>
    <message>
        <source>Extra settings (key=value):</source>
        <translation>Додаткові налаштування (key=value):</translation>
    </message>
    <message>
        <source>Additional key=value settings, separated by spaces: omen, cache, lookup, pwmin, pwmax, hintaccount, hintwords, hintfile, hintrank, hintrepeat. Everything after the first key=value is a setting, so rulesets must come first.</source>
        <translation>Додаткові налаштування key=value через пробіл: omen, cache, lookup, pwmin, pwmax, hintaccount, hintwords, hintfile, hintrank, hintrepeat. Усе після першого key=value вважається налаштуванням, тому набори правил мають іти першими.</translation>
    </message>
    <message>
        <source>Table</source>
        <translation>Таблиця</translation>
    </message>
    <message>
        <source>Every checked row after the wordlist is a table</source>
        <translation>Кожен позначений рядок після словника — це таблиця</translation>
    </message>
    <message>
        <source>Add tables…</source>
        <translation>Додати таблиці…</translation>
    </message>
    <message>
        <source>Identity:</source>
        <translation>Identity:</translation>
    </message>
    <message>
        <source>Whether leaving a token unchanged is one of its choices: 1 (default) keeps it, 0 drops it so the table only produces variations. Cannot be combined with single=1.</source>
        <translation>Чи входить залишення токена без змін до його варіантів: 1 (типово) залишає, 0 виключає, тож таблиця дає лише варіації. Не можна поєднувати з single=1.</translation>
    </message>
    <message>
        <source>Template:</source>
        <translation>Template:</translation>
    </message>
    <message>
        <source>1 reads the wordlist as a set of patterns: no word is ever returned unchanged, and words matching no rule are dropped. 0 (default) keeps unchanged words.</source>
        <translation>1 читає словник як набір шаблонів: жодне слово не повертається без змін, а слова, що не підходять до жодного правила, відкидаються. 0 (типово) зберігає слова без змін.</translation>
    </message>
    <message>
        <source>Single:</source>
        <translation>Single:</translation>
    </message>
    <message>
        <source>single=1 makes each candidate carry exactly one substitution instead of the cross product of every matched position, usually a different, cheaper attack.</source>
        <translation>single=1 робить так, що кожен кандидат містить рівно одну заміну замість декартового добутку всіх відповідних позицій; зазвичай це інша, дешевша атака.</translation>
    </message>
    <message>
        <source>Cap:</source>
        <translation>Cap:</translation>
    </message>
    <message>
        <source>Maximum replacements one source may carry in a word; 0 means no limit. Only limits the host-side cross product, not a single=1 run.</source>
        <translation>Максимальна кількість замін, яку одне джерело може внести в слово; 0 означає без обмежень. Обмежує лише декартів добуток на боці хоста, але не запуск із single=1.</translation>
    </message>
    <message>
        <source>Maxperm:</source>
        <translation>Maxperm:</translation>
    </message>
    <message>
        <source>How many of one word’s cross product the host enumerates before giving up; 0 means no limit. Each device cell is never cut.</source>
        <translation>Скільки елементів декартового добутку одного слова хост перелічує, перш ніж здатися; 0 означає без обмежень. Клітинка пристрою ніколи не обрізається.</translation>
    </message>
    <message>
        <source>Additional key=value settings, separated by spaces. Everything after the first key=value is a setting, so the tables must come first.</source>
        <translation>Додаткові налаштування key=value через пробіл. Усе після першого key=value вважається налаштуванням, тому таблиці мають іти першими.</translation>
    </message>
    <message>
        <source>Generic feed</source>
        <translation>Універсальний feed</translation>
    </message>
    <message>
        <source>Feed:</source>
        <translation>Feed:</translation>
    </message>
    <message>
        <source>Candidate generator. Anything containing a slash is treated as a feed path; otherwise hashcat looks for feed_&lt;name&gt; under its feeds directory. Arguments below are feed-specific.</source>
        <translation>Генератор кандидатів. Усе, що містить скісну риску, вважається шляхом до feed; інакше hashcat шукає feed_&lt;name&gt; у своєму каталозі feed. Наведені нижче аргументи залежать від feed.</translation>
    </message>
    <message>
        <source>wordlist, stdin, random, association, pcfg, table, hybrid, mask, or a feed path</source>
        <translation>wordlist, stdin, random, association, pcfg, table, hybrid, mask або шлях до feed</translation>
    </message>
    <message>
        <source>Browse…</source>
        <translation>Огляд…</translation>
    </message>
    <message>
        <source>Argument:</source>
        <translation>Аргумент:</translation>
    </message>
    <message>
        <source>Argument passed to the feed. Its meaning and count depend on the feed (the wordlist feed takes wordlist paths); extra tokens go straight to the feed.</source>
        <translation>Аргумент, що передається до feed. Його зміст і кількість залежать від feed (feed wordlist приймає шляхи до словників); зайві токени передаються просто до feed.</translation>
    </message>
    <message>
        <source>Add</source>
        <translation>Додати</translation>
    </message>
    <message>
        <source>Arguments the feed reads, in order; only checked rows are used</source>
        <translation>Аргументи, які читає feed, по порядку; використовуються лише позначені рядки</translation>
    </message>
    <message>
        <source>Rules</source>
        <translation>Правила</translation>
    </message>
    <message>
        <source>Use rules files:</source>
        <translation>Використовувати файли правил:</translation>
    </message>
    <message>
        <source>Enable rules file slot 1 for this run.</source>
        <translation>Увімкнути слот 1 файлу правил для цього запуску.</translation>
    </message>
    <message>
        <source>Rules file:</source>
        <translation>Файл правил:</translation>
    </message>
    <message>
        <source>Rule file for slot 1; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>Файл правил для слота 1; застосовується до кожного слова. Залиште порожнім, щоб не використовувати слот.</translation>
    </message>
    <message>
        <source>Enable rules file slot 2 for this run.</source>
        <translation>Увімкнути слот 2 файлу правил для цього запуску.</translation>
    </message>
    <message>
        <source>Rule file for slot 2; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>Файл правил для слота 2; застосовується до кожного слова. Залиште порожнім, щоб не використовувати слот.</translation>
    </message>
    <message>
        <source>Enable rules file slot 3 for this run.</source>
        <translation>Увімкнути слот 3 файлу правил для цього запуску.</translation>
    </message>
    <message>
        <source>Rule file for slot 3; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>Файл правил для слота 3; застосовується до кожного слова. Залиште порожнім, щоб не використовувати слот.</translation>
    </message>
    <message>
        <source>Generate the rules automatically instead of reading them from a file.</source>
        <translation>Генерувати правила автоматично, а не читати їх із файлу.</translation>
    </message>
    <message>
        <source>Generate rules:</source>
        <translation>Генерувати правила:</translation>
    </message>
    <message>
        <source>How many rules to generate for the finding-rule attack. No rules file is read, hashcat builds them itself; use an existing rule file with the slots above instead.</source>
        <translation>Скільки правил згенерувати для атаки з пошуком правил. Файл правил не читається, hashcat створює їх сам; щоб використати наявний файл правил, скористайтеся слотами вище.</translation>
    </message>
    <message>
        <source>Pass every rules file to hashcat in one run, instead of one run per file.</source>
        <translation>Передати всі файли правил до hashcat за один запуск, а не по одному запуску на файл.</translation>
    </message>
    <message>
        <source>Join -r files into one list</source>
        <translation>Об’єднати файли -r в один список</translation>
    </message>
    <message>
        <source>Wordlist rules</source>
        <translation>Правила словника</translation>
    </message>
    <message>
        <source>Left rule:</source>
        <translation>Ліве правило:</translation>
    </message>
    <message>
        <source>Right rule:</source>
        <translation>Праве правило:</translation>
    </message>
    <message>
        <source>Interpret the custom charsets as hexadecimal bytes. Applies to custom charset 1 through 4.</source>
        <translation>Інтерпретувати власні набори символів як шістнадцяткові байти. Стосується власних наборів символів з 1 по 4.</translation>
    </message>
    <message>
        <source>Charset is hex</source>
        <translation>Набір символів у hex</translation>
    </message>
    <message>
        <source>Mask</source>
        <translation>Маска</translation>
    </message>
    <message>
        <source>Increment mask length</source>
        <translation>Збільшувати довжину маски</translation>
    </message>
    <message>
        <source>Min:</source>
        <translation>Мін.:</translation>
    </message>
    <message>
        <source>Max:</source>
        <translation>Макс.:</translation>
    </message>
    <message>
        <source>Custom charsets</source>
        <translation>Власні набори символів</translation>
    </message>
    <message>
        <source>Charset 1:</source>
        <translation>Набір символів 1:</translation>
    </message>
    <message>
        <source>Charset 2:</source>
        <translation>Набір символів 2:</translation>
    </message>
    <message>
        <source>Charset 3:</source>
        <translation>Набір символів 3:</translation>
    </message>
    <message>
        <source>Charset 4:</source>
        <translation>Набір символів 4:</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Загальні</translation>
    </message>
    <message>
        <source>Session and limits</source>
        <translation>Сеанс і обмеження</translation>
    </message>
    <message>
        <source>Session name:</source>
        <translation>Назва сеансу:</translation>
    </message>
    <message>
        <source>Name hashcat uses for its restore file and status output.</source>
        <translation>Назва, яку hashcat використовує для файлу відновлення та виведення стану.</translation>
    </message>
    <message>
        <source>Runtime limit:</source>
        <translation>Обмеження часу роботи:</translation>
    </message>
    <message>
        <source>unlimited</source>
        <translation>без обмежень</translation>
    </message>
    <message>
        <source>Skip:</source>
        <translation>Пропустити:</translation>
    </message>
    <message>
        <source>Limit:</source>
        <translation>Ліміт:</translation>
    </message>
    <message>
        <source>Update status automatically</source>
        <translation>Оновлювати стан автоматично</translation>
    </message>
    <message>
        <source>Open…</source>
        <translation>Відкрити…</translation>
    </message>
    <message>
        <source>Assume the salt is given in hexadecimal.</source>
        <translation>Вважати, що сіль задано в шістнадцятковому вигляді.</translation>
    </message>
    <message>
        <source>Add files…</source>
        <translation>Додати файли…</translation>
    </message>
    <message>
        <source>Assume words in the wordlist are given in hexadecimal.</source>
        <translation>Вважати, що слова у словнику задано в шістнадцятковому вигляді.</translation>
    </message>
    <message>
        <source>Single rule applied to each word from the left wordlist.</source>
        <translation>Одне правило, що застосовується до кожного слова лівого словника.</translation>
    </message>
    <message>
        <source>Single rule applied to each word from the right wordlist.</source>
        <translation>Одне правило, що застосовується до кожного слова правого словника.</translation>
    </message>
    <message>
        <source>Increment the mask length from the minimum to the maximum.</source>
        <translation>Збільшувати довжину маски від мінімальної до максимальної.</translation>
    </message>
    <message>
        <source>Minimum mask length; 0 uses hashcat’s default.</source>
        <translation>Мінімальна довжина маски; 0 — типове значення hashcat.</translation>
    </message>
    <message>
        <source>Maximum mask length; 0 uses hashcat’s default.</source>
        <translation>Максимальна довжина маски; 0 — типове значення hashcat.</translation>
    </message>
    <message>
        <source>Abort the session after this many seconds; 0 runs until hashcat finishes.</source>
        <translation>Перервати сеанс через указану кількість секунд; 0 — працювати, доки hashcat не завершить роботу.</translation>
    </message>
    <message>
        <source>&#xa0;s</source>
        <translation>&#xa0;с</translation>
    </message>
    <message>
        <source>Skip this many candidates from the start.</source>
        <translation>Пропустити вказану кількість кандидатів із початку.</translation>
    </message>
    <message>
        <source>Only test this many candidates after the skipped ones.</source>
        <translation>Перевірити лише вказану кількість кандидатів після пропущених.</translation>
    </message>
    <message>
        <source>Keep updating the run status (speed, progress, ETA) instead of printing one final line.</source>
        <translation>Постійно оновлювати стан запуску (швидкість, перебіг, розрахунковий час) замість виведення одного підсумкового рядка.</translation>
    </message>
    <message>
        <source>Seconds between status updates.</source>
        <translation>Секунд між оновленнями стану.</translation>
    </message>
    <message>
        <source>Ignore warnings and start hashcat anyway. Use with care.</source>
        <translation>Ігнорувати попередження й усе одно запустити hashcat. Користуйтеся обережно.</translation>
    </message>
    <message>
        <source>Force (ignore warnings)</source>
        <translation>Примусово (ігнорувати попередження)</translation>
    </message>
    <message>
        <source>Performance</source>
        <translation>Продуктивність</translation>
    </message>
    <message>
        <source>Enable optimized kernels (limits password length)</source>
        <translation>Увімкнути оптимізовані ядра (обмежує довжину пароля)</translation>
    </message>
    <message>
        <source>Print only the attack’s speed and quit, without cracking. Useful for benchmarking and comparing settings.</source>
        <translation>Вивести лише швидкість атаки й вийти, не ламаючи. Зручно для вимірювання продуктивності та порівняння налаштувань.</translation>
    </message>
    <message>
        <source>Return expected speed of the attack, then quit</source>
        <translation>Показати очікувану швидкість атаки й вийти</translation>
    </message>
    <message>
        <source>Resources</source>
        <translation>Ресурси</translation>
    </message>
    <message>
        <source>CPU affinity:</source>
        <translation>Прив’язка до ЦП:</translation>
    </message>
    <message>
        <source>Backend devices:</source>
        <translation>Пристрої бекенда:</translation>
    </message>
    <message>
        <source>Device types:</source>
        <translation>Типи пристроїв:</translation>
    </message>
    <message>
        <source>Pin hashcat to these CPU devices, separated by commas. Leave empty to let hashcat decide.</source>
        <translation>Прив’язати hashcat до цих пристроїв ЦП, через кому. Залиште порожнім, щоб hashcat вирішив сам.</translation>
    </message>
    <message>
        <source>Devices to use, separated by commas. Leave empty for all devices.</source>
        <translation>Пристрої, що використовуються, через кому. Залиште порожнім, щоб використовувати всі пристрої.</translation>
    </message>
    <message>
        <source>Abort when a device reaches this temperature.</source>
        <translation>Перервати, коли пристрій досягне цієї температури.</translation>
    </message>
    <message>
        <source>&#xa0;°C</source>
        <translation>&#xa0;°C</translation>
    </message>
    <message>
        <source>Built-in variables:

&lt;hash&gt; = current hash filename or hash
&lt;unixtime&gt; = current Unix timestamp</source>
        <translation>Вбудовані змінні:

&lt;hash&gt; = ім’я поточного файлу гешів або геш
&lt;unixtime&gt; = поточна мітка часу Unix</translation>
    </message>
    <message>
        <source>Save…</source>
        <translation>Зберегти…</translation>
    </message>
    <message>
        <source>Do not write recovered hashes to the potfile.</source>
        <translation>Не записувати відновлені геші в potfile.</translation>
    </message>
    <message>
        <source>Settings…</source>
        <translation>Налаштування…</translation>
    </message>
    <message>
        <source>Import Settings…</source>
        <translation>Імпорт налаштувань…</translation>
    </message>
    <message>
        <source>Export Settings…</source>
        <translation>Експорт налаштувань…</translation>
    </message>
    <message>
        <source>Export Session…</source>
        <translation>Експорт сеансу…</translation>
    </message>
    <message>
        <source>Import Session…</source>
        <translation>Імпорт сеансу…</translation>
    </message>
    <message>
        <source>Abort above</source>
        <translation>Перервати вище</translation>
    </message>
    <message>
        <source>Cache path:</source>
        <translation>Шлях до кешу:</translation>
    </message>
    <message>
        <source>Directory hashcat keeps its cache in. Must exist.</source>
        <translation>Каталог, у якому hashcat зберігає свій кеш. Має існувати.</translation>
    </message>
    <message>
        <source>Abort the session when an OpenCL device hotspot exceeds this many degrees Celsius. 0 disables the thermal limit.</source>
        <translation>Перервати сеанс, коли гаряча точка пристрою OpenCL перевищить указану кількість градусів Цельсія. 0 вимикає обмеження за температурою.</translation>
    </message>
    <message>
        <source>Outfile</source>
        <translation>Вихідний файл</translation>
    </message>
    <message>
        <source>Write recovered hashes to file:</source>
        <translation>Записувати відновлені геші у файл:</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Формат:</translation>
    </message>
    <message>
        <source>Write the outfile as JSON. The outfile format is ignored.</source>
        <translation>Записувати вихідний файл у форматі JSON. Формат вихідного файлу ігнорується.</translation>
    </message>
    <message>
        <source>Write JSON</source>
        <translation>Записувати JSON</translation>
    </message>
    <message>
        <source>Do not write the potfile</source>
        <translation>Не записувати potfile</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>Пуск</translation>
    </message>
    <message>
        <source>Copy command to clipboard</source>
        <translation>Копіювати команду до буфера обміну</translation>
    </message>
    <message>
        <source>Help</source>
        <translation>Довідка</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Інструменти</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Файл</translation>
    </message>
    <message>
        <source>About</source>
        <translation>Про програму</translation>
    </message>
    <message>
        <source>Load Example Session</source>
        <translation>Завантажити приклад сеансу</translation>
    </message>
    <message>
        <source>Reset Session</source>
        <translation>Скинути сеанс</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation>Вийти</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>Про Qt</translation>
    </message>
    <message>
        <source>Built-in charsets:</source>
        <translation>Вбудовані набори символів:</translation>
    </message>
    <message>
        <source>Outfile format to use, separated by commas.
Default: 1,2</source>
        <translation>Формати вихідного файлу, через кому.
Типово: 1,2</translation>
    </message>
    <message>
        <source>Common encodings:</source>
        <translation>Поширені кодування:</translation>
    </message>
    <message>
        <source>Force the internal wordlist encoding from this code. Leave empty to let hashcat decide.</source>
        <translation>Примусово задати внутрішнє кодування словника, з якого виконується перетворення. Залиште порожнім, щоб hashcat вирішив сам.</translation>
    </message>
    <message>
        <source>Force the internal wordlist encoding to this code. Leave empty to let hashcat decide.</source>
        <translation>Примусово задати внутрішнє кодування словника, в яке виконується перетворення. Залиште порожнім, щоб hashcat вирішив сам.</translation>
    </message>
    <message>
        <source>JSON Files (*.json)</source>
        <translation>Файли JSON (*.json)</translation>
    </message>
    <message>
        <source>Exported</source>
        <translation>Експортовано</translation>
    </message>
    <message>
        <source>Export failed</source>
        <translation>Не вдалося експортувати</translation>
    </message>
    <message>
        <source>Import failed</source>
        <translation>Не вдалося імпортувати</translation>
    </message>
    <message>
        <source>Imported</source>
        <translation>Імпортовано</translation>
    </message>
    <message>
        <source>The file was made by an older version of hashcat-gui (format version %1; this version reads format version %2).</source>
        <translation>Файл створено старішою версією hashcat-gui (версія формату %1; ця версія читає формат версії %2).</translation>
    </message>
    <message>
        <source>The file holds sections this import does not use: %1.</source>
        <translation>Файл містить розділи, які цей імпорт не використовує: %1.</translation>
    </message>
    <message>
        <source>The file is in format version %1, but this version reads format version %2, so it was not applied. Update hashcat-gui to read it.</source>
        <translation>Файл має версію формату %1, але ця версія читає формат версії %2, тому його не було застосовано. Оновіть hashcat-gui, щоб прочитати його.</translation>
    </message>
    <message>
        <source>The session file is in format version %1, but this version reads format version %2, so the session was not restored. The file was left as it is, so the session of this run is not stored either. Update hashcat-gui to read it.</source>
        <translation>Файл сеансу має версію формату %1, але ця версія читає формат версії %2, тому сеанс не було відновлено. Файл залишено без змін, тому сеанс цього запуску теж не зберігається. Оновіть hashcat-gui, щоб прочитати його.</translation>
    </message>
    <message>
        <source>Settings exported to %1.</source>
        <translation>Налаштування експортовано до %1.</translation>
    </message>
    <message>
        <source>OpenCL device types to use, separated by commas.</source>
        <translation>Типи пристроїв OpenCL для використання, через кому.</translation>
    </message>
    <message>
        <source>Export session</source>
        <translation>Експорт сеансу</translation>
    </message>
    <message>
        <source>Session exported to “%1”.</source>
        <translation>Сеанс експортовано до «%1».</translation>
    </message>
    <message>
        <source>Import session</source>
        <translation>Імпорт сеансу</translation>
    </message>
    <message>
        <source>Session imported from “%1”.</source>
        <translation>Сеанс імпортовано з «%1».</translation>
    </message>
    <message>
        <source>Session imported from “%1”.

%2</source>
        <translation>Сеанс імпортовано з «%1».

%2</translation>
    </message>
    <message>
        <source>Export settings</source>
        <translation>Експорт налаштувань</translation>
    </message>
    <message>
        <source>Import settings</source>
        <translation>Імпорт налаштувань</translation>
    </message>
    <message>
        <source>Settings loaded from “%1”.</source>
        <translation>Налаштування завантажено з «%1».</translation>
    </message>
    <message>
        <source>Review them and save to apply them.</source>
        <translation>Перевірте їх і збережіть, щоб застосувати.</translation>
    </message>
    <message>
        <source>Load example session</source>
        <translation>Завантажити приклад сеансу</translation>
    </message>
    <message>
        <source>Reset session</source>
        <translation>Скинути сеанс</translation>
    </message>
    <message>
        <source>Reset every field to its default? The current session is lost.</source>
        <translation>Скинути всі поля до типових значень? Поточний сеанс буде втрачено.</translation>
    </message>
    <message>
        <source>Session warning</source>
        <translation>Попередження щодо сеансу</translation>
    </message>
    <message>
        <source>The session file could not be read, so the session was not restored. The file was left in place.

%1</source>
        <translation>Не вдалося прочитати файл сеансу, тому сеанс не було відновлено. Файл залишено без змін.

%1</translation>
    </message>
    <message>
        <source>The session names the hash type %1, which this hashcat version does not support.</source>
        <translation>Сеанс указує тип геша %1, який ця версія hashcat не підтримує.</translation>
    </message>
    <message>
        <source>Save failed</source>
        <translation>Не вдалося зберегти</translation>
    </message>
    <message>
        <source>Straight</source>
        <translation>Словникова</translation>
    </message>
    <message>
        <source>Combination</source>
        <translation>Комбінаторна</translation>
    </message>
    <message>
        <source>Brute-force</source>
        <translation>Повний перебір</translation>
    </message>
    <message>
        <source>Hybrid Wordlist + Mask</source>
        <translation>Гібридна: словник + маска</translation>
    </message>
    <message>
        <source>Hybrid Mask + Wordlist</source>
        <translation>Гібридна: маска + словник</translation>
    </message>
    <message>
        <source>Generic (feed)</source>
        <translation>Універсальна (feed)</translation>
    </message>
    <message>
        <source>Association</source>
        <translation>Асоціативна</translation>
    </message>
    <message>
        <source>Hybrid (mask + ?w/?q)</source>
        <translation>Гібридна (маска + ?w/?q)</translation>
    </message>
    <message>
        <source>Updating…</source>
        <translation>Оновлення…</translation>
    </message>
    <message>
        <source>Failed to load hash types</source>
        <translation>Не вдалося завантажити типи гешів</translation>
    </message>
    <message>
        <source>This mode needs exactly 2 wordlists (left, right).</source>
        <translation>Цьому режиму потрібно рівно 2 словники (лівий, правий).</translation>
    </message>
    <message>
        <source>This mode runs hashcat’s bundled default-passwords ruleset; name one or more to use your own.</source>
        <translation>Цей режим запускає вбудований у hashcat набір правил default-passwords; укажіть один або кілька, щоб використати власні.</translation>
    </message>
    <message>
        <source>This mode needs one wordlist and at least one table.</source>
        <translation>Цьому режиму потрібні один словник і щонайменше одна таблиця.</translation>
    </message>
    <message>
        <source>This mode needs a feed. Arguments depend on the feed — random and stdin need none, wordlist needs a path.</source>
        <translation>Цьому режиму потрібен feed. Аргументи залежать від feed: random і stdin не потребують жодного, wordlist потребує шлях.</translation>
    </message>
    <message>
        <source>This mode needs at most one wordlist. Without one the words come from the hash file and the settings line can bound the run; with one the two files are paired by line.</source>
        <translation>Цьому режиму потрібно щонайбільше один словник. Без словника слова беруться з файлу гешів, а рядок налаштувань може обмежити запуск; зі словником два файли зіставляються порядково.</translation>
    </message>
    <message>
        <source>This mode needs a mask with exactly one ?w; a ?q after ?w names a second wordlist.</source>
        <translation>Цьому режиму потрібна маска рівно з одним ?w; ?q після ?w указує другий словник.</translation>
    </message>
    <message>
        <source>Open hash file</source>
        <translation>Відкрити файл гешів</translation>
    </message>
    <message>
        <source>Save output file</source>
        <translation>Зберегти вихідний файл</translation>
    </message>
    <message>
        <source>Open cache directory</source>
        <translation>Відкрити каталог кешу</translation>
    </message>
    <message>
        <source>Add wordlists</source>
        <translation>Додати словники</translation>
    </message>
    <message>
        <source>Add wordlist folder</source>
        <translation>Додати теку зі словниками</translation>
    </message>
    <message>
        <source>Add rulesets</source>
        <translation>Додати набори правил</translation>
    </message>
    <message>
        <source>Add ruleset folder</source>
        <translation>Додати теку набору правил</translation>
    </message>
    <message>
        <source>Add tables</source>
        <translation>Додати таблиці</translation>
    </message>
    <message>
        <source>Open feed file</source>
        <translation>Відкрити файл feed</translation>
    </message>
    <message>
        <source>This folder has no wordlists. hashcat reads the regular files directly inside it, not subfolders.</source>
        <translation>У цій теці немає словників. hashcat читає звичайні файли, що лежать безпосередньо в ній, але не підтеки.</translation>
    </message>
    <message>
        <source>This ruleset folder has no Grammar/grammar.txt:
%1</source>
        <translation>У цій теці набору правил немає Grammar/grammar.txt:
%1</translation>
    </message>
    <message>
        <source>This ruleset does not exist:
%1</source>
        <translation>Цей набір правил не існує:
%1</translation>
    </message>
    <message>
        <source>A table must be a file; hashcat does not read a folder:
%1</source>
        <translation>Таблиця має бути файлом; hashcat не читає теку:
%1</translation>
    </message>
    <message>
        <source>This table does not exist:
%1</source>
        <translation>Ця таблиця не існує:
%1</translation>
    </message>
    <message>
        <source>Open rules file</source>
        <translation>Відкрити файл правил</translation>
    </message>
    <message>
        <source>Could not start “%1”: %2</source>
        <translation>Не вдалося запустити «%1»: %2</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not available. Available terminals: %2
Navigate to %3 → %4 to change it.</source>
        <translation>Налаштований термінал «%1» недоступний. Доступні термінали: %2
Перейдіть до %3 → %4, щоб змінити його.</translation>
    </message>
    <message>
        <source>This mode needs exactly %1 wordlists (left, right).
Currently selected: %2.</source>
        <translation>Цьому режиму потрібно рівно %1 словників (лівий, правий).
Наразі вибрано: %2.</translation>
    </message>
    <message>
        <source>Failed to obtain supported hash types.
Error: %1</source>
        <translation>Не вдалося отримати підтримувані типи гешів.
Помилка: %1</translation>
    </message>
    <message>
        <source>The hash types could not be loaded. Check the hashcat path in Settings.</source>
        <translation>Не вдалося завантажити типи гешів. Перевірте шлях до hashcat у налаштуваннях.</translation>
    </message>
    <message>
        <source>Set the hashcat path under %1 → %2 to load the available hash types.</source>
        <translation>Укажіть шлях до hashcat у %1 → %2, щоб завантажити доступні типи гешів.</translation>
    </message>
    <message>
        <source>This mode needs a wordlist and a mask.</source>
        <translation>Цьому режиму потрібні словник і маска.</translation>
    </message>
    <message>
        <source>This mode needs a mask.</source>
        <translation>Цьому режиму потрібна маска.</translation>
    </message>
    <message>
        <source>Load the %1 example? The current session is lost.

The example cracks a test hash; hashcat should recover the password %2.</source>
        <translation>Завантажити приклад %1? Поточний сеанс буде втрачено.

Приклад ламає тестовий геш; hashcat має відновити пароль %2.</translation>
    </message>
    <message>
        <source>Could not create the example directory “%1”.</source>
        <translation>Не вдалося створити каталог прикладу «%1».</translation>
    </message>
    <message>
        <source>Could not read the bundled file “%1”: %2</source>
        <translation>Не вдалося прочитати вбудований файл «%1»: %2</translation>
    </message>
    <message>
        <source>Could not write “%1”: %2</source>
        <translation>Не вдалося записати «%1»: %2</translation>
    </message>
    <message>
        <source>This mode needs at least one wordlist.</source>
        <translation>Цьому режиму потрібен принаймні один словник.</translation>
    </message>
    <message>
        <source>This wordlist does not exist:
%1</source>
        <translation>Цього словника не існує:
%1</translation>
    </message>
    <message>
        <source>Copied to the clipboard</source>
        <translation>Скопійовано до буфера обміну</translation>
    </message>
    <message>
        <source>Launch failed</source>
        <translation>Не вдалося запустити</translation>
    </message>
    <message>
        <source>Please choose a hash file.</source>
        <translation>Виберіть файл гешів.</translation>
    </message>
    <message>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to configure the path to the hashcat executable.</source>
        <translation>Перейдіть до &lt;b&gt;%1 → %2&lt;/b&gt;, щоб налаштувати шлях до виконуваного файлу hashcat.</translation>
    </message>
    <message>
        <source>The configured hashcat executable does not exist:
%1
Navigate to %2 → %3 to change it.</source>
        <translation>Налаштованого виконуваного файлу hashcat не існує:
%1
Перейдіть до %2 → %3, щоб змінити його.</translation>
    </message>
    <message>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to select the terminal used for launching.</source>
        <translation>Перейдіть до &lt;b&gt;%1 → %2&lt;/b&gt;, щоб вибрати термінал, який використовується для запуску.</translation>
    </message>
    <message>
        <source>This mode takes at most %1 wordlists.
Currently selected: %2.</source>
        <translation>Цей режим приймає щонайбільше %1 словників.
Наразі вибрано: %2.</translation>
    </message>
    <message>
        <source>hashcat pairs the two files by line here; this GUI offers only one wordlist for that role.</source>
        <translation>Тут hashcat зіставляє два файли порядково; цей інтерфейс пропонує для такої ролі лише один словник.</translation>
    </message>
    <message>
        <source>This wordlist has to be a file: hashcat reads it as the multiplied wordlist (the ?q side), and a folder cannot be used there:
%1</source>
        <translation>Цей словник має бути файлом: hashcat читає його як множений словник (сторона ?q), і теку там використати не можна:
%1</translation>
    </message>
    <message>
        <source>This mode needs at least one table.</source>
        <translation>Цьому режиму потрібна щонайменше одна таблиця.</translation>
    </message>
    <message>
        <source>This mode needs a feed: a bundled name (wordlist, stdin, random, …) or the path of a feed file.</source>
        <translation>Цьому режиму потрібен feed: вбудована назва (wordlist, stdin, random, …) або шлях до файлу feed.</translation>
    </message>
    <message>
        <source>This mode pairs the wordlists with the mask; the mask needs a ?w for the word.</source>
        <translation>Цей режим зіставляє словники з маскою; масці потрібен ?w для слова.</translation>
    </message>
    <message>
        <source>The mask contains more than one ?w.</source>
        <translation>Маска містить більше ніж один ?w.</translation>
    </message>
    <message>
        <source>The mask contains a ?q before the first ?w.</source>
        <translation>Маска містить ?q перед першим ?w.</translation>
    </message>
    <message>
        <source>The mask contains more than one ?q.</source>
        <translation>Маска містить більше ніж один ?q.</translation>
    </message>
    <message>
        <source>The mask names a second word with ?q, but only one wordlist is selected.</source>
        <translation>Маска вказує друге слово через ?q, але вибрано лише один словник.</translation>
    </message>
    <message>
        <source>Two wordlists are selected, but the mask has no ?q for the second one.</source>
        <translation>Вибрано два словники, але в масці немає ?q для другого.</translation>
    </message>
    <message>
        <source>The increment is not offered in this hybrid mode.</source>
        <translation>Приріст недоступний у цьому гібридному режимі.</translation>
    </message>
    <message>
        <source>The ?w and ?q markers are only offered in the hybrid mode (mask + wordlists).</source>
        <translation>Маркери ?w і ?q доступні лише в гібридному режимі (маска + словники).</translation>
    </message>
    <message>
        <source>The setting “%1” is not valid for this mode.</source>
        <translation>Налаштування «%1» недійсне для цього режиму.</translation>
    </message>
    <message>
        <source>The token “%1” is not a setting; settings are written as key=value.</source>
        <translation>Токен «%1» не є налаштуванням; налаштування записуються у вигляді key=value.</translation>
    </message>
    <message>
        <source>The settings line applies only while the words come from the hash file. Clear either the settings line or the wordlist list.</source>
        <translation>Рядок налаштувань діє, лише поки слова беруться з файлу гешів. Очистьте або рядок налаштувань, або список словників.</translation>
    </message>
    <message>
        <source>The hash type 4000 describes its hashes as a recipe, for example
md5(md5(salt) . pass). Enter it in the hash recipe field.</source>
        <translation>Тип геша 4000 описує свої геші у вигляді рецепта, наприклад
md5(md5(salt) . pass). Введіть його в поле рецепта геша.</translation>
    </message>
    <message>
        <source>The minimum increment length is larger than the maximum.</source>
        <translation>Мінімальна довжина приросту більша за максимальну.</translation>
    </message>
    <message>
        <source>The cache path must be an existing directory:
%1</source>
        <translation>Шлях до кешу має вказувати на наявний каталог:
%1</translation>
    </message>
    <message>
        <source>The increment cannot be combined with the speed-only option.</source>
        <translation>Приріст не можна поєднувати з параметром «лише швидкість».</translation>
    </message>
    <message>
        <source>Removing cracked hashes is not possible in the association attack mode.</source>
        <translation>Вилучення зламаних гешів неможливе в асоціативному режимі атаки.</translation>
    </message>
</context>
<context>
    <name>SettingsDialog</name>
    <message>
        <source>Settings</source>
        <translation>Налаштування</translation>
    </message>
    <message>
        <source>Not configured</source>
        <translation>Не налаштовано</translation>
    </message>
    <message>
        <source>hashcat</source>
        <translation>hashcat</translation>
    </message>
    <message>
        <source>&amp;hashcat executable</source>
        <translation>&amp;Виконуваний файл hashcat</translation>
    </message>
    <message>
        <source>Select…</source>
        <translation>Вибрати…</translation>
    </message>
    <message>
        <source>&amp;Terminal</source>
        <translation>&amp;Термінал</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Загальні</translation>
    </message>
    <message>
        <source>&amp;Language</source>
        <translation>&amp;Мова</translation>
    </message>
    <message>
        <source>&amp;Color scheme</source>
        <translation>&amp;Кольорова схема</translation>
    </message>
    <message>
        <source>Not all color schemes are available on all platforms.</source>
        <translation>Не всі кольорові схеми доступні на всіх платформах.</translation>
    </message>
    <message>
        <source>&amp;Use short parameters</source>
        <translation>Використовувати ко&amp;роткі параметри</translation>
    </message>
    <message>
        <source>Session and files</source>
        <translation>Сеанс і файли</translation>
    </message>
    <message>
        <source>&amp;Restore the last session at startup</source>
        <translation>Відновлювати &amp;останній сеанс під час запуску</translation>
    </message>
    <message>
        <source>&amp;Session file</source>
        <translation>Файл &amp;сеансу</translation>
    </message>
    <message>
        <source>The file the last session is stored in</source>
        <translation>Файл, у якому зберігається останній сеанс</translation>
    </message>
    <message>
        <source>Open folder</source>
        <translation>Відкрити теку</translation>
    </message>
    <message>
        <source>Open the folder the session is stored in</source>
        <translation>Відкрити теку, у якій зберігається сеанс</translation>
    </message>
    <message>
        <source>Settings &amp;file</source>
        <translation>Файл &amp;налаштувань</translation>
    </message>
    <message>
        <source>The file the settings are stored in</source>
        <translation>Файл, у якому зберігаються налаштування</translation>
    </message>
    <message>
        <source>Open the folder the settings are stored in</source>
        <translation>Відкрити теку, у якій зберігаються налаштування</translation>
    </message>
    <message>
        <source>Download from %1</source>
        <translation>Завантажити з %1</translation>
    </message>
    <message>
        <source>Use the short option names (-m) instead of the long ones (--hash-type) in the command.</source>
        <translation>Використовувати в команді короткі назви параметрів (-m) замість довгих (--hash-type).</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Зберегти</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Скасувати</translation>
    </message>
    <message>
        <source>Select a terminal</source>
        <translation>Вибрати термінал</translation>
    </message>
    <message>
        <source>System default</source>
        <translation>Системна</translation>
    </message>
    <message>
        <source>Light</source>
        <translation>Світла</translation>
    </message>
    <message>
        <source>Dark</source>
        <translation>Темна</translation>
    </message>
    <message>
        <source>The hashcat path “%1” was not found on this computer.</source>
        <translation>Шлях до hashcat «%1» не знайдено на цьому комп’ютері.</translation>
    </message>
    <message>
        <source>The terminal “%1” is not installed and was not applied.</source>
        <translation>Термінал «%1» не встановлено, і його не було застосовано.</translation>
    </message>
    <message>
        <source>The color scheme “%1” is not offered and was not applied.</source>
        <translation>Кольорову схему «%1» не запропоновано, і її не було застосовано.</translation>
    </message>
    <message>
        <source>The language “%1” is not offered and was not applied.</source>
        <translation>Мову «%1» не запропоновано, і її не було застосовано.</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not installed.</source>
        <translation>Налаштований термінал «%1» не встановлено.</translation>
    </message>
    <message>
        <source>Executable Files (*.exe *.bin hashcat);;All Files (*)</source>
        <translation>Виконувані файли (*.exe *.bin hashcat);;Усі файли (*)</translation>
    </message>
    <message>
        <source>No hashcat executable is configured; hashcat cannot be started.</source>
        <translation>Виконуваний файл hashcat не налаштовано; запустити hashcat неможливо.</translation>
    </message>
    <message>
        <source>The configured hashcat executable was not found on this computer.</source>
        <translation>Налаштований виконуваний файл hashcat не знайдено на цьому комп’ютері.</translation>
    </message>
    <message>
        <source>Select the terminal hashcat is started in.</source>
        <translation>Виберіть термінал, у якому запускається hashcat.</translation>
    </message>
    <message>
        <source>Select the hashcat executable</source>
        <translation>Виберіть виконуваний файл hashcat</translation>
    </message>
    <message>
        <source>Invalid file</source>
        <translation>Недійсний файл</translation>
    </message>
    <message>
        <source>The selected file is not an executable.</source>
        <translation>Вибраний файл не є виконуваним.</translation>
    </message>
    <message>
        <source>Restart required</source>
        <translation>Потрібен перезапуск</translation>
    </message>
    <message>
        <source>Restart hashcat-gui to apply the new language.</source>
        <translation>Перезапустіть hashcat-gui, щоб застосувати нову мову.</translation>
    </message>
</context>
<context>
    <name>StateFile</name>
    <message>
        <source>The file is not valid JSON: %1</source>
        <translation>Файл не є дійсним JSON: %1</translation>
    </message>
    <message>
        <source>The file does not contain a JSON object.</source>
        <translation>Файл не містить об’єкта JSON.</translation>
    </message>
    <message>
        <source>The file does not contain any sections.</source>
        <translation>Файл не містить жодного розділу.</translation>
    </message>
    <message>
        <source>Could not open “%1” for writing: %2</source>
        <translation>Не вдалося відкрити «%1» для запису: %2</translation>
    </message>
    <message>
        <source>Could not write to “%1”: %2</source>
        <translation>Не вдалося записати в «%1»: %2</translation>
    </message>
    <message>
        <source>Could not open “%1” for reading: %2</source>
        <translation>Не вдалося відкрити «%1» для читання: %2</translation>
    </message>
    <message>
        <source>The file does not contain a “%1” section.</source>
        <translation>У файлі немає розділу «%1».</translation>
    </message>
    <message>
        <source>The file does not contain a “%1” section; it holds %2.</source>
        <translation>У файлі немає розділу «%1»; він містить %2.</translation>
    </message>
    <message>
        <source>The “%1” section does not contain any values.</source>
        <translation>Розділ «%1» не містить жодних значень.</translation>
    </message>
    <message>
        <source> and </source>
        <translation> і </translation>
    </message>
    <message>
        <source> and %1 more</source>
        <translation> і ще %1</translation>
    </message>
    <message>
        <source>The file names values this version does not know, so they were not applied: %1.</source>
        <translation>Файл містить значення, невідомі цій версії, тому їх не було застосовано: %1.</translation>
    </message>
    <message>
        <source>The file holds values of the wrong type, so they were not applied: %1.</source>
        <translation>Файл містить значення неправильного типу, тому їх не було застосовано: %1.</translation>
    </message>
</context>
<context>
    <name>UpdateChecker</name>
    <message>
        <source>GitHub answered with HTTP status %1: %2</source>
        <translation>GitHub відповів зі статусом HTTP %1: %2</translation>
    </message>
    <message>
        <source>GitHub answered with HTTP status %1.</source>
        <translation>GitHub відповів зі статусом HTTP %1.</translation>
    </message>
    <message>
        <source>The release information is not valid JSON.</source>
        <translation>Відомості про випуск не є дійсним JSON.</translation>
    </message>
    <message>
        <source>The release information does not name a version.</source>
        <translation>Відомості про випуск не вказують версію.</translation>
    </message>
    <message>
        <source>The release version %1 is not a valid version number.</source>
        <translation>Версія випуску %1 не є дійсним номером версії.</translation>
    </message>
    <message>
        <source>The installed version %1 is not a valid version number.</source>
        <translation>Встановлена версія %1 не є дійсним номером версії.</translation>
    </message>
</context>
</TS>
