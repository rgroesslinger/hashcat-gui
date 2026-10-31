<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ru" sourcelanguage="en">
<context>
    <name>AboutDialog</name>
    <message>
        <source>About</source>
        <translation>О программе</translation>
    </message>
    <message>
        <source>Graphical user interface for hashcat, the password recovery utility</source>
        <translation>Графический интерфейс для hashcat — утилиты восстановления паролей</translation>
    </message>
    <message>
        <source>Version information</source>
        <translation>Информация о версии</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Неизвестно</translation>
    </message>
    <message>
        <source>Updates</source>
        <translation>Обновления</translation>
    </message>
    <message>
        <source>Checking for updates…</source>
        <translation>Проверка обновлений…</translation>
    </message>
    <message>
        <source>System information</source>
        <translation>Информация о системе</translation>
    </message>
    <message>
        <source>Operating system</source>
        <translation>Операционная система</translation>
    </message>
    <message>
        <source>Links</source>
        <translation>Ссылки</translation>
    </message>
    <message>
        <source>License</source>
        <translation>Лицензия</translation>
    </message>
    <message>
        <source>Copy the application, toolkit, system and hashcat versions to the clipboard</source>
        <translation>Скопировать в буфер обмена версии приложения, библиотеки, системы и hashcat</translation>
    </message>
    <message>
        <source>Copy details</source>
        <translation>Копировать сведения</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Copied to the clipboard</source>
        <translation>Скопировано в буфер обмена</translation>
    </message>
    <message>
        <source>Project page</source>
        <translation>Страница проекта</translation>
    </message>
    <message>
        <source>Report an issue</source>
        <translation>Сообщить о проблеме</translation>
    </message>
    <message>
        <source>hashcat</source>
        <translation>hashcat</translation>
    </message>
    <message>
        <source>hashcat FAQ</source>
        <translation>FAQ по hashcat</translation>
    </message>
    <message>
        <source>hashcat-gui is licensed under the %1.</source>
        <translation>hashcat-gui распространяется под лицензией %1.</translation>
    </message>
    <message>
        <source>GNU GPL v3.0 or later</source>
        <translation>GNU GPL v3.0 или более поздней</translation>
    </message>
    <message>
        <source>Not configured</source>
        <translation>Не настроено</translation>
    </message>
    <message>
        <source>Error: %1</source>
        <translation>Ошибка: %1</translation>
    </message>
    <message>
        <source>A newer version is available: %1.</source>
        <translation>Доступна более новая версия: %1.</translation>
    </message>
    <message>
        <source>Release page</source>
        <translation>Страница релиза</translation>
    </message>
    <message>
        <source>No updates available.</source>
        <translation>Обновлений нет.</translation>
    </message>
    <message>
        <source>Could not check for updates.</source>
        <translation>Не удалось проверить наличие обновлений.</translation>
    </message>
    <message>
        <source>Could not check for updates: %1</source>
        <translation>Не удалось проверить наличие обновлений: %1</translation>
    </message>
</context>
<context>
    <name>HashcatInfoParser</name>
    <message>
        <source>Invalid JSON returned from hashcat.</source>
        <translation>hashcat вернул недопустимый JSON.</translation>
    </message>
    <message>
        <source>hashcat did not report any hash types.</source>
        <translation>hashcat не сообщил ни об одном типе хешей.</translation>
    </message>
</context>
<context>
    <name>HelperUtils</name>
    <message>
        <source>The hashcat path is not configured.</source>
        <translation>Путь к hashcat не настроен.</translation>
    </message>
    <message>
        <source>Failed to start hashcat
%1</source>
        <translation>Не удалось запустить hashcat
%1</translation>
    </message>
    <message>
        <source>hashcat timed out
%1</source>
        <translation>Истекло время ожидания hashcat
%1</translation>
    </message>
    <message>
        <source>No supported terminal was found on this system. hashcat-gui needs one to show the hashcat output.</source>
        <translation>В этой системе не найден поддерживаемый терминал. hashcat-gui нужен терминал, чтобы показывать вывод hashcat.</translation>
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
        <translation>Файл хешей или хеш:</translation>
    </message>
    <message>
        <source>The hash to crack, a path to a file of hashes, or a .hccapx WPA capture. Paste a single hash here, or point it at a file.</source>
        <translation>Хеш для взлома, путь к файлу с хешами или дамп WPA в формате .hccapx. Вставьте сюда один хеш или укажите файл.</translation>
    </message>
    <message>
        <source>Salt is hex</source>
        <translation>Соль в hex</translation>
    </message>
    <message>
        <source>Remove already-cracked hashes from the potfile so a later run does not report them again. Refused in attack mode 9.</source>
        <translation>Удалять уже взломанные хеши из potfile, чтобы последующий запуск не сообщал о них снова. Отклоняется в режиме атаки 9.</translation>
    </message>
    <message>
        <source>Remove hash from hash list once it is cracked</source>
        <translation>Удалять хеш из списка хешей после взлома</translation>
    </message>
    <message>
        <source>Ignore the @username part of hashes (in hash modes that carry one) so the same word can satisfy several accounts.</source>
        <translation>Игнорировать часть @username в хешах (в режимах хешей, где она есть), чтобы одно и то же слово подходило сразу к нескольким учётным записям.</translation>
    </message>
    <message>
        <source>Ignore username in hash file</source>
        <translation>Игнорировать имя пользователя в файле хешей</translation>
    </message>
    <message>
        <source>Mode:</source>
        <translation>Режим:</translation>
    </message>
    <message>
        <source>Attack mode. Each choice reveals its own options below.</source>
        <translation>Режим атаки. Каждый вариант показывает ниже собственные параметры.</translation>
    </message>
    <message>
        <source>Hash type:</source>
        <translation>Тип хеша:</translation>
    </message>
    <message>
        <source>Hash recipe:</source>
        <translation>Рецепт хеша:</translation>
    </message>
    <message>
        <source>Unix: &apos;md5(md5(salt) . pass)&apos;
               Windows: &quot;md5(&apos;abc&apos; . pass)&quot;</source>
        <translation>Unix: &apos;md5(md5(salt) . pass)&apos;
               Windows: &quot;md5(&apos;abc&apos; . pass)&quot;</translation>
    </message>
    <message>
        <source>Wordlists</source>
        <translation>Словари</translation>
    </message>
    <message>
        <source>Move item up</source>
        <translation>Переместить элемент вверх</translation>
    </message>
    <message>
        <source>Move item down</source>
        <translation>Переместить элемент вниз</translation>
    </message>
    <message>
        <source>hashcat reads every file directly inside the folder as one wordlist.</source>
        <translation>hashcat читает каждый файл, лежащий непосредственно в папке, как один словарь.</translation>
    </message>
    <message>
        <source>Add folder…</source>
        <translation>Добавить папку…</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Удалить</translation>
    </message>
    <message>
        <source>Only checked rows are used. A folder is one wordlist: hashcat reads the files directly inside it.</source>
        <translation>Используются только отмеченные строки. Папка — это один словарь: hashcat читает файлы, лежащие непосредственно в ней.</translation>
    </message>
    <message>
        <source>Wordlist is hex</source>
        <translation>Словарь в hex</translation>
    </message>
    <message>
        <source>Encoding from:</source>
        <translation>Кодировка из:</translation>
    </message>
    <message>
        <source>Encoding to:</source>
        <translation>Кодировка в:</translation>
    </message>
    <message>
        <source>Settings (no wordlist):</source>
        <translation>Настройки (без словаря):</translation>
    </message>
    <message>
        <source>Positional settings, separated by spaces. Only used when no wordlist is selected: with one, hashcat pairs the two files by line. “phases” picks which phases run, cheapest first; the default “words,rules,pcfg” ends only when stopped. Phase-prefixed settings: “rules.rulefile”, “rules.rulemax”, “words.hints”, “pcfg.hintaccount”, …</source>
        <translation>Позиционные настройки через пробел. Используются, только когда словарь не выбран: при выбранном словаре hashcat сопоставляет два файла построчно. «phases» выбирает, какие фазы выполняются, начиная с самой дешёвой; значение по умолчанию «words,rules,pcfg» завершается только при остановке. Настройки с префиксом фазы: «rules.rulefile», «rules.rulemax», «words.hints», «pcfg.hintaccount», …</translation>
    </message>
    <message>
        <source>PCFG</source>
        <translation>PCFG</translation>
    </message>
    <message>
        <source>A row is a ruleset file, a folder with a Grammar/grammar.txt, or a bundled name. Only checked rows are used; with none checked, hashcat’s bundled default-passwords runs. hints is also bundled.</source>
        <translation>Строка — это файл набора правил, папка с Grammar/grammar.txt или встроенное имя. Используются только отмеченные строки; если не отмечена ни одна, запускается встроенный в hashcat default-passwords. hints тоже встроен.</translation>
    </message>
    <message>
        <source>A ruleset folder needs a Grammar/grammar.txt.</source>
        <translation>Папке набора правил нужен файл Grammar/grammar.txt.</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Scale:</translation>
    </message>
    <message>
        <source>Quantization steps per bit (1–64)</source>
        <translation>Шагов квантования на бит (1–64)</translation>
    </message>
    <message>
        <source>Costmax:</source>
        <translation>Costmax:</translation>
    </message>
    <message>
        <source>Highest cost level to enumerate (1–64)</source>
        <translation>Наибольший уровень стоимости для перебора (1–64)</translation>
    </message>
    <message>
        <source>Weights:</source>
        <translation>Weights:</translation>
    </message>
    <message>
        <source>One value per ruleset, plain numbers separated by colons, e.g. 2:1 for two rulesets. The count must match the number of rulesets.</source>
        <translation>Одно значение на набор правил, простые числа через двоеточие, например 2:1 для двух наборов правил. Количество значений должно совпадать с количеством наборов правил.</translation>
    </message>
    <message>
        <source>Threads:</source>
        <translation>Потоки:</translation>
    </message>
    <message>
        <source>Cores that generate base words (0–16); 0 runs on the calling thread</source>
        <translation>Ядра, генерирующие базовые слова (0–16); 0 выполняет работу в вызывающем потоке</translation>
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
        <translation>Число бит, которое может охватывать один внутренний цикл (0–27)</translation>
    </message>
    <message>
        <source>Maxword:</source>
        <translation>Maxword:</translation>
    </message>
    <message>
        <source>Candidates the kernel builds per base word: 0, 16 or 32 (a multiple of 16)</source>
        <translation>Кандидаты, которые ядро строит на одно базовое слово: 0, 16 или 32 (кратно 16)</translation>
    </message>
    <message>
        <source>Maxgain:</source>
        <translation>Maxgain:</translation>
    </message>
    <message>
        <source>1.0–64.0; decimal point is a dot, not the locale separator</source>
        <translation>1.0–64.0; десятичным разделителем служит точка, а не разделитель из региональных настроек</translation>
    </message>
    <message>
        <source>Walk:</source>
        <translation>Walk:</translation>
    </message>
    <message>
        <source>1 steps the last token instead of unraking it</source>
        <translation>1 продвигает последний токен пошагово, вместо того чтобы применять к нему «unraking»</translation>
    </message>
    <message>
        <source>Mask:</source>
        <translation>Маска:</translation>
    </message>
    <message>
        <source>Hold the grammar to the candidates this mask admits; cannot be combined with omen=1, or with hintaccount while the hint words come from the accounts (give hintwords or hintfile instead)</source>
        <translation>Ограничить грамматику кандидатами, которые допускает эта маска; нельзя сочетать с omen=1, а также с hintaccount, пока слова-подсказки берутся из учётных записей (вместо этого укажите hintwords или hintfile)</translation>
    </message>
    <message>
        <source>Extra settings (key=value):</source>
        <translation>Дополнительные настройки (key=value):</translation>
    </message>
    <message>
        <source>Additional key=value settings, separated by spaces: omen, cache, lookup, pwmin, pwmax, hintaccount, hintwords, hintfile, hintrank, hintrepeat. Everything after the first key=value is a setting, so rulesets must come first.</source>
        <translation>Дополнительные настройки key=value через пробел: omen, cache, lookup, pwmin, pwmax, hintaccount, hintwords, hintfile, hintrank, hintrepeat. Всё после первого key=value считается настройкой, поэтому наборы правил должны идти первыми.</translation>
    </message>
    <message>
        <source>Table</source>
        <translation>Таблица</translation>
    </message>
    <message>
        <source>Every checked row after the wordlist is a table</source>
        <translation>Каждая отмеченная строка после словаря — это таблица</translation>
    </message>
    <message>
        <source>Add tables…</source>
        <translation>Добавить таблицы…</translation>
    </message>
    <message>
        <source>Identity:</source>
        <translation>Identity:</translation>
    </message>
    <message>
        <source>Whether leaving a token unchanged is one of its choices: 1 (default) keeps it, 0 drops it so the table only produces variations. Cannot be combined with single=1.</source>
        <translation>Входит ли в число вариантов токена оставление его без изменений: 1 (по умолчанию) оставляет, 0 исключает, так что таблица даёт только вариации. Нельзя сочетать с single=1.</translation>
    </message>
    <message>
        <source>Template:</source>
        <translation>Template:</translation>
    </message>
    <message>
        <source>1 reads the wordlist as a set of patterns: no word is ever returned unchanged, and words matching no rule are dropped. 0 (default) keeps unchanged words.</source>
        <translation>1 читает словарь как набор шаблонов: ни одно слово не возвращается без изменений, а слова, не подходящие ни под одно правило, отбрасываются. 0 (по умолчанию) сохраняет слова без изменений.</translation>
    </message>
    <message>
        <source>Single:</source>
        <translation>Single:</translation>
    </message>
    <message>
        <source>single=1 makes each candidate carry exactly one substitution instead of the cross product of every matched position, usually a different, cheaper attack.</source>
        <translation>single=1 делает так, что каждый кандидат содержит ровно одну замену вместо декартова произведения всех подходящих позиций; обычно это другая, более дешёвая атака.</translation>
    </message>
    <message>
        <source>Cap:</source>
        <translation>Cap:</translation>
    </message>
    <message>
        <source>Maximum replacements one source may carry in a word; 0 means no limit. Only limits the host-side cross product, not a single=1 run.</source>
        <translation>Максимальное число замен, которое один источник может внести в слово; 0 означает без ограничений. Ограничивает только декартово произведение на стороне хоста, но не запуск с single=1.</translation>
    </message>
    <message>
        <source>Maxperm:</source>
        <translation>Maxperm:</translation>
    </message>
    <message>
        <source>How many of one word’s cross product the host enumerates before giving up; 0 means no limit. Each device cell is never cut.</source>
        <translation>Сколько элементов декартова произведения одного слова хост перечисляет, прежде чем сдаться; 0 означает без ограничений. Ячейка устройства никогда не обрезается.</translation>
    </message>
    <message>
        <source>Additional key=value settings, separated by spaces. Everything after the first key=value is a setting, so the tables must come first.</source>
        <translation>Дополнительные настройки key=value через пробел. Всё после первого key=value считается настройкой, поэтому таблицы должны идти первыми.</translation>
    </message>
    <message>
        <source>Generic feed</source>
        <translation>Универсальный feed</translation>
    </message>
    <message>
        <source>Feed:</source>
        <translation>Feed:</translation>
    </message>
    <message>
        <source>Candidate generator. Anything containing a slash is treated as a feed path; otherwise hashcat looks for feed_&lt;name&gt; under its feeds directory. Arguments below are feed-specific.</source>
        <translation>Генератор кандидатов. Всё, что содержит косую черту, считается путём к feed; иначе hashcat ищет feed_&lt;name&gt; в своём каталоге feed. Приведённые ниже аргументы зависят от feed.</translation>
    </message>
    <message>
        <source>wordlist, stdin, random, association, pcfg, table, hybrid, mask, or a feed path</source>
        <translation>wordlist, stdin, random, association, pcfg, table, hybrid, mask или путь к feed</translation>
    </message>
    <message>
        <source>Browse…</source>
        <translation>Обзор…</translation>
    </message>
    <message>
        <source>Argument:</source>
        <translation>Аргумент:</translation>
    </message>
    <message>
        <source>Argument passed to the feed. Its meaning and count depend on the feed (the wordlist feed takes wordlist paths); extra tokens go straight to the feed.</source>
        <translation>Аргумент, передаваемый в feed. Его смысл и количество зависят от feed (feed wordlist принимает пути к словарям); лишние токены передаются прямо в feed.</translation>
    </message>
    <message>
        <source>Add</source>
        <translation>Добавить</translation>
    </message>
    <message>
        <source>Arguments the feed reads, in order; only checked rows are used</source>
        <translation>Аргументы, которые читает feed, по порядку; используются только отмеченные строки</translation>
    </message>
    <message>
        <source>Rules</source>
        <translation>Правила</translation>
    </message>
    <message>
        <source>Use rules files:</source>
        <translation>Использовать файлы правил:</translation>
    </message>
    <message>
        <source>Enable rules file slot 1 for this run.</source>
        <translation>Включить слот 1 файла правил для этого запуска.</translation>
    </message>
    <message>
        <source>Rules file:</source>
        <translation>Файл правил:</translation>
    </message>
    <message>
        <source>Rule file for slot 1; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>Файл правил для слота 1; применяется к каждому слову. Оставьте пустым, чтобы не использовать слот.</translation>
    </message>
    <message>
        <source>Enable rules file slot 2 for this run.</source>
        <translation>Включить слот 2 файла правил для этого запуска.</translation>
    </message>
    <message>
        <source>Rule file for slot 2; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>Файл правил для слота 2; применяется к каждому слову. Оставьте пустым, чтобы не использовать слот.</translation>
    </message>
    <message>
        <source>Enable rules file slot 3 for this run.</source>
        <translation>Включить слот 3 файла правил для этого запуска.</translation>
    </message>
    <message>
        <source>Rule file for slot 3; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>Файл правил для слота 3; применяется к каждому слову. Оставьте пустым, чтобы не использовать слот.</translation>
    </message>
    <message>
        <source>Generate the rules automatically instead of reading them from a file.</source>
        <translation>Генерировать правила автоматически, а не читать их из файла.</translation>
    </message>
    <message>
        <source>Generate rules:</source>
        <translation>Генерировать правила:</translation>
    </message>
    <message>
        <source>How many rules to generate for the finding-rule attack. No rules file is read, hashcat builds them itself; use an existing rule file with the slots above instead.</source>
        <translation>Сколько правил сгенерировать для атаки с поиском правил. Файл правил не читается, hashcat создаёт их сам; чтобы использовать готовый файл правил, воспользуйтесь слотами выше.</translation>
    </message>
    <message>
        <source>Pass every rules file to hashcat in one run, instead of one run per file.</source>
        <translation>Передать все файлы правил в hashcat за один запуск, а не по одному запуску на файл.</translation>
    </message>
    <message>
        <source>Join -r files into one list</source>
        <translation>Объединить файлы -r в один список</translation>
    </message>
    <message>
        <source>Wordlist rules</source>
        <translation>Правила словаря</translation>
    </message>
    <message>
        <source>Left rule:</source>
        <translation>Левое правило:</translation>
    </message>
    <message>
        <source>Right rule:</source>
        <translation>Правое правило:</translation>
    </message>
    <message>
        <source>Interpret the custom charsets as hexadecimal bytes. Applies to custom charset 1 through 4.</source>
        <translation>Интерпретировать пользовательские наборы символов как шестнадцатеричные байты. Относится к пользовательским наборам символов с 1 по 4.</translation>
    </message>
    <message>
        <source>Charset is hex</source>
        <translation>Набор символов в hex</translation>
    </message>
    <message>
        <source>Mask</source>
        <translation>Маска</translation>
    </message>
    <message>
        <source>Increment mask length</source>
        <translation>Увеличивать длину маски</translation>
    </message>
    <message>
        <source>Min:</source>
        <translation>Мин.:</translation>
    </message>
    <message>
        <source>Max:</source>
        <translation>Макс.:</translation>
    </message>
    <message>
        <source>Custom charsets</source>
        <translation>Пользовательские наборы символов</translation>
    </message>
    <message>
        <source>Charset 1:</source>
        <translation>Набор символов 1:</translation>
    </message>
    <message>
        <source>Charset 2:</source>
        <translation>Набор символов 2:</translation>
    </message>
    <message>
        <source>Charset 3:</source>
        <translation>Набор символов 3:</translation>
    </message>
    <message>
        <source>Charset 4:</source>
        <translation>Набор символов 4:</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Общие</translation>
    </message>
    <message>
        <source>Session and limits</source>
        <translation>Сессия и ограничения</translation>
    </message>
    <message>
        <source>Session name:</source>
        <translation>Имя сессии:</translation>
    </message>
    <message>
        <source>Name hashcat uses for its restore file and status output.</source>
        <translation>Имя, которое hashcat использует для файла восстановления и вывода статуса.</translation>
    </message>
    <message>
        <source>Runtime limit:</source>
        <translation>Ограничение времени работы:</translation>
    </message>
    <message>
        <source>unlimited</source>
        <translation>без ограничения</translation>
    </message>
    <message>
        <source>Skip:</source>
        <translation>Пропустить:</translation>
    </message>
    <message>
        <source>Limit:</source>
        <translation>Лимит:</translation>
    </message>
    <message>
        <source>Update status automatically</source>
        <translation>Обновлять статус автоматически</translation>
    </message>
    <message>
        <source>Open…</source>
        <translation>Открыть…</translation>
    </message>
    <message>
        <source>Assume the salt is given in hexadecimal.</source>
        <translation>Считать, что соль задана в шестнадцатеричном виде.</translation>
    </message>
    <message>
        <source>Add files…</source>
        <translation>Добавить файлы…</translation>
    </message>
    <message>
        <source>Assume words in the wordlist are given in hexadecimal.</source>
        <translation>Считать, что слова в словаре заданы в шестнадцатеричном виде.</translation>
    </message>
    <message>
        <source>Single rule applied to each word from the left wordlist.</source>
        <translation>Одно правило, применяемое к каждому слову левого словаря.</translation>
    </message>
    <message>
        <source>Single rule applied to each word from the right wordlist.</source>
        <translation>Одно правило, применяемое к каждому слову правого словаря.</translation>
    </message>
    <message>
        <source>Increment the mask length from the minimum to the maximum.</source>
        <translation>Увеличивать длину маски от минимальной до максимальной.</translation>
    </message>
    <message>
        <source>Minimum mask length; 0 uses hashcat’s default.</source>
        <translation>Минимальная длина маски; 0 — значение hashcat по умолчанию.</translation>
    </message>
    <message>
        <source>Maximum mask length; 0 uses hashcat’s default.</source>
        <translation>Максимальная длина маски; 0 — значение hashcat по умолчанию.</translation>
    </message>
    <message>
        <source>Abort the session after this many seconds; 0 runs until hashcat finishes.</source>
        <translation>Прервать сессию через указанное число секунд; 0 — работать, пока hashcat не завершится.</translation>
    </message>
    <message>
        <source>&#xa0;s</source>
        <translation>&#xa0;с</translation>
    </message>
    <message>
        <source>Skip this many candidates from the start.</source>
        <translation>Пропустить указанное число кандидатов с начала.</translation>
    </message>
    <message>
        <source>Only test this many candidates after the skipped ones.</source>
        <translation>Проверить только указанное число кандидатов после пропущенных.</translation>
    </message>
    <message>
        <source>Keep updating the run status (speed, progress, ETA) instead of printing one final line.</source>
        <translation>Постоянно обновлять состояние запуска (скорость, прогресс, расчётное время) вместо вывода одной итоговой строки.</translation>
    </message>
    <message>
        <source>Seconds between status updates.</source>
        <translation>Секунд между обновлениями состояния.</translation>
    </message>
    <message>
        <source>Ignore warnings and start hashcat anyway. Use with care.</source>
        <translation>Игнорировать предупреждения и всё равно запустить hashcat. Используйте с осторожностью.</translation>
    </message>
    <message>
        <source>Force (ignore warnings)</source>
        <translation>Принудительно (игнорировать предупреждения)</translation>
    </message>
    <message>
        <source>Performance</source>
        <translation>Производительность</translation>
    </message>
    <message>
        <source>Enable optimized kernels (limits password length)</source>
        <translation>Включить оптимизированные ядра (ограничивает длину пароля)</translation>
    </message>
    <message>
        <source>Print only the attack’s speed and quit, without cracking. Useful for benchmarking and comparing settings.</source>
        <translation>Вывести только скорость атаки и выйти, не взламывая. Удобно для замеров производительности и сравнения настроек.</translation>
    </message>
    <message>
        <source>Return expected speed of the attack, then quit</source>
        <translation>Показать ожидаемую скорость атаки и выйти</translation>
    </message>
    <message>
        <source>Resources</source>
        <translation>Ресурсы</translation>
    </message>
    <message>
        <source>CPU affinity:</source>
        <translation>Привязка к ЦП:</translation>
    </message>
    <message>
        <source>Backend devices:</source>
        <translation>Устройства бэкенда:</translation>
    </message>
    <message>
        <source>Device types:</source>
        <translation>Типы устройств:</translation>
    </message>
    <message>
        <source>Pin hashcat to these CPU devices, separated by commas. Leave empty to let hashcat decide.</source>
        <translation>Привязать hashcat к этим устройствам ЦП, через запятую. Оставьте пустым, чтобы hashcat решил сам.</translation>
    </message>
    <message>
        <source>Devices to use, separated by commas. Leave empty for all devices.</source>
        <translation>Используемые устройства, через запятую. Оставьте пустым, чтобы использовать все устройства.</translation>
    </message>
    <message>
        <source>Abort when a device reaches this temperature.</source>
        <translation>Прервать, когда устройство достигнет этой температуры.</translation>
    </message>
    <message>
        <source>&#xa0;°C</source>
        <translation>&#xa0;°C</translation>
    </message>
    <message>
        <source>Built-in variables:

&lt;hash&gt; = current hash filename or hash
&lt;unixtime&gt; = current Unix timestamp</source>
        <translation>Встроенные переменные:

&lt;hash&gt; = имя текущего файла хешей или хеш
&lt;unixtime&gt; = текущая метка времени Unix</translation>
    </message>
    <message>
        <source>Save…</source>
        <translation>Сохранить…</translation>
    </message>
    <message>
        <source>Do not write recovered hashes to the potfile.</source>
        <translation>Не записывать восстановленные хеши в potfile.</translation>
    </message>
    <message>
        <source>Settings…</source>
        <translation>Настройки…</translation>
    </message>
    <message>
        <source>Import Settings…</source>
        <translation>Импорт настроек…</translation>
    </message>
    <message>
        <source>Export Settings…</source>
        <translation>Экспорт настроек…</translation>
    </message>
    <message>
        <source>Export Session…</source>
        <translation>Экспорт сессии…</translation>
    </message>
    <message>
        <source>Import Session…</source>
        <translation>Импорт сессии…</translation>
    </message>
    <message>
        <source>Abort above</source>
        <translation>Прервать выше</translation>
    </message>
    <message>
        <source>Cache path:</source>
        <translation>Путь к кешу:</translation>
    </message>
    <message>
        <source>Directory hashcat keeps its cache in. Must exist.</source>
        <translation>Каталог, в котором hashcat хранит свой кеш. Должен существовать.</translation>
    </message>
    <message>
        <source>Abort the session when an OpenCL device hotspot exceeds this many degrees Celsius. 0 disables the thermal limit.</source>
        <translation>Прервать сессию, когда горячая точка устройства OpenCL превысит указанное число градусов Цельсия. 0 отключает ограничение по температуре.</translation>
    </message>
    <message>
        <source>Outfile</source>
        <translation>Выходной файл</translation>
    </message>
    <message>
        <source>Write recovered hashes to file:</source>
        <translation>Записывать восстановленные хеши в файл:</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Формат:</translation>
    </message>
    <message>
        <source>Write the outfile as JSON. The outfile format is ignored.</source>
        <translation>Записывать выходной файл в формате JSON. Формат выходного файла игнорируется.</translation>
    </message>
    <message>
        <source>Write JSON</source>
        <translation>Записывать JSON</translation>
    </message>
    <message>
        <source>Do not write the potfile</source>
        <translation>Не записывать potfile</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>Пуск</translation>
    </message>
    <message>
        <source>Copy command to clipboard</source>
        <translation>Копировать команду в буфер обмена</translation>
    </message>
    <message>
        <source>Help</source>
        <translation>Справка</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Инструменты</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Файл</translation>
    </message>
    <message>
        <source>About</source>
        <translation>О программе</translation>
    </message>
    <message>
        <source>Load Example Session</source>
        <translation>Загрузить пример сессии</translation>
    </message>
    <message>
        <source>Reset Session</source>
        <translation>Сбросить сессию</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation>Выход</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>О Qt</translation>
    </message>
    <message>
        <source>Built-in charsets:</source>
        <translation>Встроенные наборы символов:</translation>
    </message>
    <message>
        <source>Outfile format to use, separated by commas.
Default: 1,2</source>
        <translation>Используемые форматы выходного файла, через запятую.
По умолчанию: 1,2</translation>
    </message>
    <message>
        <source>Common encodings:</source>
        <translation>Распространённые кодировки:</translation>
    </message>
    <message>
        <source>Force the internal wordlist encoding from this code. Leave empty to let hashcat decide.</source>
        <translation>Принудительно задать внутреннюю кодировку словаря, из которой выполняется преобразование. Оставьте пустым, чтобы hashcat решил сам.</translation>
    </message>
    <message>
        <source>Force the internal wordlist encoding to this code. Leave empty to let hashcat decide.</source>
        <translation>Принудительно задать внутреннюю кодировку словаря, в которую выполняется преобразование. Оставьте пустым, чтобы hashcat решил сам.</translation>
    </message>
    <message>
        <source>JSON Files (*.json)</source>
        <translation>Файлы JSON (*.json)</translation>
    </message>
    <message>
        <source>Exported</source>
        <translation>Экспортировано</translation>
    </message>
    <message>
        <source>Export failed</source>
        <translation>Не удалось экспортировать</translation>
    </message>
    <message>
        <source>Import failed</source>
        <translation>Не удалось импортировать</translation>
    </message>
    <message>
        <source>Imported</source>
        <translation>Импортировано</translation>
    </message>
    <message>
        <source>The file was made by an older version of hashcat-gui (format version %1; this version reads format version %2).</source>
        <translation>Файл создан более старой версией hashcat-gui (версия формата %1; эта версия читает формат версии %2).</translation>
    </message>
    <message>
        <source>The file holds sections this import does not use: %1.</source>
        <translation>Файл содержит разделы, которые этот импорт не использует: %1.</translation>
    </message>
    <message>
        <source>The file is in format version %1, but this version reads format version %2, so it was not applied. Update hashcat-gui to read it.</source>
        <translation>Файл имеет версию формата %1, но эта версия читает формат версии %2, поэтому он не был применён. Обновите hashcat-gui, чтобы прочитать его.</translation>
    </message>
    <message>
        <source>The session file is in format version %1, but this version reads format version %2, so the session was not restored. The file was left as it is, so the session of this run is not stored either. Update hashcat-gui to read it.</source>
        <translation>Файл сессии имеет версию формата %1, но эта версия читает формат версии %2, поэтому сессия не была восстановлена. Файл оставлен без изменений, поэтому сессия этого запуска тоже не сохраняется. Обновите hashcat-gui, чтобы прочитать его.</translation>
    </message>
    <message>
        <source>Settings exported to %1.</source>
        <translation>Настройки экспортированы в %1.</translation>
    </message>
    <message>
        <source>OpenCL device types to use, separated by commas.</source>
        <translation>Типы устройств OpenCL для использования, через запятую.</translation>
    </message>
    <message>
        <source>Export session</source>
        <translation>Экспорт сессии</translation>
    </message>
    <message>
        <source>Session exported to “%1”.</source>
        <translation>Сессия экспортирована в «%1».</translation>
    </message>
    <message>
        <source>Import session</source>
        <translation>Импорт сессии</translation>
    </message>
    <message>
        <source>Session imported from “%1”.</source>
        <translation>Сессия импортирована из «%1».</translation>
    </message>
    <message>
        <source>Session imported from “%1”.

%2</source>
        <translation>Сессия импортирована из «%1».

%2</translation>
    </message>
    <message>
        <source>Export settings</source>
        <translation>Экспорт настроек</translation>
    </message>
    <message>
        <source>Import settings</source>
        <translation>Импорт настроек</translation>
    </message>
    <message>
        <source>Settings loaded from “%1”.</source>
        <translation>Настройки загружены из «%1».</translation>
    </message>
    <message>
        <source>Review them and save to apply them.</source>
        <translation>Проверьте их и сохраните, чтобы применить.</translation>
    </message>
    <message>
        <source>Load example session</source>
        <translation>Загрузить пример сессии</translation>
    </message>
    <message>
        <source>Reset session</source>
        <translation>Сбросить сессию</translation>
    </message>
    <message>
        <source>Reset every field to its default? The current session is lost.</source>
        <translation>Сбросить все поля к значениям по умолчанию? Текущая сессия будет потеряна.</translation>
    </message>
    <message>
        <source>Session warning</source>
        <translation>Предупреждение о сессии</translation>
    </message>
    <message>
        <source>The session file could not be read, so the session was not restored. The file was left in place.

%1</source>
        <translation>Не удалось прочитать файл сессии, поэтому сессия не была восстановлена. Файл оставлен без изменений.

%1</translation>
    </message>
    <message>
        <source>The session names the hash type %1, which this hashcat version does not support.</source>
        <translation>Сессия указывает тип хеша %1, который эта версия hashcat не поддерживает.</translation>
    </message>
    <message>
        <source>Save failed</source>
        <translation>Не удалось сохранить</translation>
    </message>
    <message>
        <source>Straight</source>
        <translation>Словарная</translation>
    </message>
    <message>
        <source>Combination</source>
        <translation>Комбинаторная</translation>
    </message>
    <message>
        <source>Brute-force</source>
        <translation>Полный перебор</translation>
    </message>
    <message>
        <source>Hybrid Wordlist + Mask</source>
        <translation>Гибридная: словарь + маска</translation>
    </message>
    <message>
        <source>Hybrid Mask + Wordlist</source>
        <translation>Гибридная: маска + словарь</translation>
    </message>
    <message>
        <source>Generic (feed)</source>
        <translation>Универсальная (feed)</translation>
    </message>
    <message>
        <source>Association</source>
        <translation>Ассоциативная</translation>
    </message>
    <message>
        <source>Hybrid (mask + ?w/?q)</source>
        <translation>Гибридная (маска + ?w/?q)</translation>
    </message>
    <message>
        <source>Updating…</source>
        <translation>Обновление…</translation>
    </message>
    <message>
        <source>Failed to load hash types</source>
        <translation>Не удалось загрузить типы хешей</translation>
    </message>
    <message>
        <source>This mode needs exactly 2 wordlists (left, right).</source>
        <translation>Этому режиму нужно ровно 2 словаря (левый, правый).</translation>
    </message>
    <message>
        <source>This mode runs hashcat’s bundled default-passwords ruleset; name one or more to use your own.</source>
        <translation>Этот режим запускает встроенный в hashcat набор правил default-passwords; укажите один или несколько, чтобы использовать свои.</translation>
    </message>
    <message>
        <source>This mode needs one wordlist and at least one table.</source>
        <translation>Этому режиму нужны один словарь и хотя бы одна таблица.</translation>
    </message>
    <message>
        <source>This mode needs a feed. Arguments depend on the feed — random and stdin need none, wordlist needs a path.</source>
        <translation>Этому режиму нужен feed. Аргументы зависят от feed: random и stdin не требуют их, wordlist требует путь.</translation>
    </message>
    <message>
        <source>This mode needs at most one wordlist. Without one the words come from the hash file and the settings line can bound the run; with one the two files are paired by line.</source>
        <translation>Этому режиму нужно не более одного словаря. Без словаря слова берутся из файла хешей, а строка настроек может ограничить запуск; со словарём два файла сопоставляются построчно.</translation>
    </message>
    <message>
        <source>This mode needs a mask with exactly one ?w; a ?q after ?w names a second wordlist.</source>
        <translation>Этому режиму нужна маска ровно с одним ?w; ?q после ?w задаёт второй словарь.</translation>
    </message>
    <message>
        <source>Open hash file</source>
        <translation>Открыть файл хешей</translation>
    </message>
    <message>
        <source>Save output file</source>
        <translation>Сохранить выходной файл</translation>
    </message>
    <message>
        <source>Open cache directory</source>
        <translation>Открыть каталог кеша</translation>
    </message>
    <message>
        <source>Add wordlists</source>
        <translation>Добавить словари</translation>
    </message>
    <message>
        <source>Add wordlist folder</source>
        <translation>Добавить папку со словарями</translation>
    </message>
    <message>
        <source>Add rulesets</source>
        <translation>Добавить наборы правил</translation>
    </message>
    <message>
        <source>Add ruleset folder</source>
        <translation>Добавить папку набора правил</translation>
    </message>
    <message>
        <source>Add tables</source>
        <translation>Добавить таблицы</translation>
    </message>
    <message>
        <source>Open feed file</source>
        <translation>Открыть файл feed</translation>
    </message>
    <message>
        <source>This folder has no wordlists. hashcat reads the regular files directly inside it, not subfolders.</source>
        <translation>В этой папке нет словарей. hashcat читает обычные файлы, лежащие непосредственно в ней, но не подпапки.</translation>
    </message>
    <message>
        <source>This ruleset folder has no Grammar/grammar.txt:
%1</source>
        <translation>В этой папке набора правил нет Grammar/grammar.txt:
%1</translation>
    </message>
    <message>
        <source>This ruleset does not exist:
%1</source>
        <translation>Этот набор правил не существует:
%1</translation>
    </message>
    <message>
        <source>A table must be a file; hashcat does not read a folder:
%1</source>
        <translation>Таблица должна быть файлом; hashcat не читает папку:
%1</translation>
    </message>
    <message>
        <source>This table does not exist:
%1</source>
        <translation>Эта таблица не существует:
%1</translation>
    </message>
    <message>
        <source>Open rules file</source>
        <translation>Открыть файл правил</translation>
    </message>
    <message>
        <source>Could not start “%1”: %2</source>
        <translation>Не удалось запустить «%1»: %2</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not available. Available terminals: %2
Navigate to %3 → %4 to change it.</source>
        <translation>Настроенный терминал «%1» недоступен. Доступные терминалы: %2
Перейдите в %3 → %4, чтобы изменить его.</translation>
    </message>
    <message>
        <source>This mode needs exactly %1 wordlists (left, right).
Currently selected: %2.</source>
        <translation>Этому режиму нужно ровно %1 словарей (левый, правый).
Сейчас выбрано: %2.</translation>
    </message>
    <message>
        <source>Failed to obtain supported hash types.
Error: %1</source>
        <translation>Не удалось получить поддерживаемые типы хешей.
Ошибка: %1</translation>
    </message>
    <message>
        <source>The hash types could not be loaded. Check the hashcat path in Settings.</source>
        <translation>Не удалось загрузить типы хешей. Проверьте путь к hashcat в настройках.</translation>
    </message>
    <message>
        <source>Set the hashcat path under %1 → %2 to load the available hash types.</source>
        <translation>Укажите путь к hashcat в %1 → %2, чтобы загрузить доступные типы хешей.</translation>
    </message>
    <message>
        <source>This mode needs a wordlist and a mask.</source>
        <translation>Этому режиму нужны словарь и маска.</translation>
    </message>
    <message>
        <source>This mode needs a mask.</source>
        <translation>Этому режиму нужна маска.</translation>
    </message>
    <message>
        <source>Load the %1 example? The current session is lost.

The example cracks a test hash; hashcat should recover the password %2.</source>
        <translation>Загрузить пример %1? Текущая сессия будет потеряна.

Пример взламывает тестовый хеш; hashcat должен восстановить пароль %2.</translation>
    </message>
    <message>
        <source>Could not create the example directory “%1”.</source>
        <translation>Не удалось создать каталог примера «%1».</translation>
    </message>
    <message>
        <source>Could not read the bundled file “%1”: %2</source>
        <translation>Не удалось прочитать встроенный файл «%1»: %2</translation>
    </message>
    <message>
        <source>Could not write “%1”: %2</source>
        <translation>Не удалось записать «%1»: %2</translation>
    </message>
    <message>
        <source>This mode needs at least one wordlist.</source>
        <translation>Этому режиму нужен хотя бы один словарь.</translation>
    </message>
    <message>
        <source>This wordlist does not exist:
%1</source>
        <translation>Этот словарь не существует:
%1</translation>
    </message>
    <message>
        <source>Copied to the clipboard</source>
        <translation>Скопировано в буфер обмена</translation>
    </message>
    <message>
        <source>Launch failed</source>
        <translation>Не удалось запустить</translation>
    </message>
    <message>
        <source>Please choose a hash file.</source>
        <translation>Выберите файл хешей.</translation>
    </message>
    <message>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to configure the path to the hashcat executable.</source>
        <translation>Перейдите в &lt;b&gt;%1 → %2&lt;/b&gt;, чтобы настроить путь к исполняемому файлу hashcat.</translation>
    </message>
    <message>
        <source>The configured hashcat executable does not exist:
%1
Navigate to %2 → %3 to change it.</source>
        <translation>Настроенный исполняемый файл hashcat не существует:
%1
Перейдите в %2 → %3, чтобы изменить его.</translation>
    </message>
    <message>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to select the terminal used for launching.</source>
        <translation>Перейдите в &lt;b&gt;%1 → %2&lt;/b&gt;, чтобы выбрать терминал, используемый для запуска.</translation>
    </message>
    <message>
        <source>This mode takes at most %1 wordlists.
Currently selected: %2.</source>
        <translation>Этот режим принимает не более %1 словарей.
Сейчас выбрано: %2.</translation>
    </message>
    <message>
        <source>hashcat pairs the two files by line here; this GUI offers only one wordlist for that role.</source>
        <translation>Здесь hashcat сопоставляет два файла построчно; этот интерфейс предлагает для такой роли только один словарь.</translation>
    </message>
    <message>
        <source>This wordlist has to be a file: hashcat reads it as the multiplied wordlist (the ?q side), and a folder cannot be used there:
%1</source>
        <translation>Этот словарь должен быть файлом: hashcat читает его как умножаемый словарь (сторона ?q), и папку там использовать нельзя:
%1</translation>
    </message>
    <message>
        <source>This mode needs at least one table.</source>
        <translation>Этому режиму нужна хотя бы одна таблица.</translation>
    </message>
    <message>
        <source>This mode needs a feed: a bundled name (wordlist, stdin, random, …) or the path of a feed file.</source>
        <translation>Этому режиму нужен feed: встроенное имя (wordlist, stdin, random, …) или путь к файлу feed.</translation>
    </message>
    <message>
        <source>This mode pairs the wordlists with the mask; the mask needs a ?w for the word.</source>
        <translation>Этот режим сопоставляет словари с маской; маске нужен ?w для слова.</translation>
    </message>
    <message>
        <source>The mask contains more than one ?w.</source>
        <translation>Маска содержит более одного ?w.</translation>
    </message>
    <message>
        <source>The mask contains a ?q before the first ?w.</source>
        <translation>Маска содержит ?q перед первым ?w.</translation>
    </message>
    <message>
        <source>The mask contains more than one ?q.</source>
        <translation>Маска содержит более одного ?q.</translation>
    </message>
    <message>
        <source>The mask names a second word with ?q, but only one wordlist is selected.</source>
        <translation>Маска задаёт второе слово через ?q, но выбран только один словарь.</translation>
    </message>
    <message>
        <source>Two wordlists are selected, but the mask has no ?q for the second one.</source>
        <translation>Выбрано два словаря, но в маске нет ?q для второго.</translation>
    </message>
    <message>
        <source>The increment is not offered in this hybrid mode.</source>
        <translation>Инкремент недоступен в этом гибридном режиме.</translation>
    </message>
    <message>
        <source>The ?w and ?q markers are only offered in the hybrid mode (mask + wordlists).</source>
        <translation>Маркеры ?w и ?q доступны только в гибридном режиме (маска + словари).</translation>
    </message>
    <message>
        <source>The setting “%1” is not valid for this mode.</source>
        <translation>Настройка «%1» недопустима для этого режима.</translation>
    </message>
    <message>
        <source>The token “%1” is not a setting; settings are written as key=value.</source>
        <translation>Токен «%1» не является настройкой; настройки записываются в виде key=value.</translation>
    </message>
    <message>
        <source>The settings line applies only while the words come from the hash file. Clear either the settings line or the wordlist list.</source>
        <translation>Строка настроек действует, только пока слова берутся из файла хешей. Очистите либо строку настроек, либо список словарей.</translation>
    </message>
    <message>
        <source>The hash type 4000 describes its hashes as a recipe, for example
md5(md5(salt) . pass). Enter it in the hash recipe field.</source>
        <translation>Тип хеша 4000 описывает свои хеши в виде рецепта, например
md5(md5(salt) . pass). Введите его в поле рецепта хеша.</translation>
    </message>
    <message>
        <source>The minimum increment length is larger than the maximum.</source>
        <translation>Минимальная длина инкремента больше максимальной.</translation>
    </message>
    <message>
        <source>The cache path must be an existing directory:
%1</source>
        <translation>Путь к кешу должен указывать на существующий каталог:
%1</translation>
    </message>
    <message>
        <source>The increment cannot be combined with the speed-only option.</source>
        <translation>Инкремент нельзя сочетать с параметром «только скорость».</translation>
    </message>
    <message>
        <source>Removing cracked hashes is not possible in the association attack mode.</source>
        <translation>Удаление взломанных хешей невозможно в ассоциативном режиме атаки.</translation>
    </message>
</context>
<context>
    <name>SettingsDialog</name>
    <message>
        <source>Settings</source>
        <translation>Настройки</translation>
    </message>
    <message>
        <source>Not configured</source>
        <translation>Не настроено</translation>
    </message>
    <message>
        <source>hashcat</source>
        <translation>hashcat</translation>
    </message>
    <message>
        <source>&amp;hashcat executable</source>
        <translation>&amp;Исполняемый файл hashcat</translation>
    </message>
    <message>
        <source>Select…</source>
        <translation>Выбрать…</translation>
    </message>
    <message>
        <source>&amp;Terminal</source>
        <translation>&amp;Терминал</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Общие</translation>
    </message>
    <message>
        <source>&amp;Language</source>
        <translation>&amp;Язык</translation>
    </message>
    <message>
        <source>&amp;Color scheme</source>
        <translation>&amp;Цветовая схема</translation>
    </message>
    <message>
        <source>Not all color schemes are available on all platforms.</source>
        <translation>Не все цветовые схемы доступны на всех платформах.</translation>
    </message>
    <message>
        <source>&amp;Use short parameters</source>
        <translation>Использовать &amp;короткие параметры</translation>
    </message>
    <message>
        <source>Session and files</source>
        <translation>Сессия и файлы</translation>
    </message>
    <message>
        <source>&amp;Restore the last session at startup</source>
        <translation>&amp;Восстанавливать последнюю сессию при запуске</translation>
    </message>
    <message>
        <source>&amp;Session file</source>
        <translation>Файл &amp;сессии</translation>
    </message>
    <message>
        <source>The file the last session is stored in</source>
        <translation>Файл, в котором хранится последняя сессия</translation>
    </message>
    <message>
        <source>Open folder</source>
        <translation>Открыть папку</translation>
    </message>
    <message>
        <source>Open the folder the session is stored in</source>
        <translation>Открыть папку, в которой хранится сессия</translation>
    </message>
    <message>
        <source>Settings &amp;file</source>
        <translation>Файл &amp;настроек</translation>
    </message>
    <message>
        <source>The file the settings are stored in</source>
        <translation>Файл, в котором хранятся настройки</translation>
    </message>
    <message>
        <source>Open the folder the settings are stored in</source>
        <translation>Открыть папку, в которой хранятся настройки</translation>
    </message>
    <message>
        <source>Download from %1</source>
        <translation>Скачать с %1</translation>
    </message>
    <message>
        <source>Use the short option names (-m) instead of the long ones (--hash-type) in the command.</source>
        <translation>Использовать в команде короткие имена параметров (-m) вместо длинных (--hash-type).</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Сохранить</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Отмена</translation>
    </message>
    <message>
        <source>Select a terminal</source>
        <translation>Выбрать терминал</translation>
    </message>
    <message>
        <source>System default</source>
        <translation>Системная</translation>
    </message>
    <message>
        <source>Light</source>
        <translation>Светлая</translation>
    </message>
    <message>
        <source>Dark</source>
        <translation>Тёмная</translation>
    </message>
    <message>
        <source>The hashcat path “%1” was not found on this computer.</source>
        <translation>Путь к hashcat «%1» не найден на этом компьютере.</translation>
    </message>
    <message>
        <source>The terminal “%1” is not installed and was not applied.</source>
        <translation>Терминал «%1» не установлен и не был применён.</translation>
    </message>
    <message>
        <source>The color scheme “%1” is not offered and was not applied.</source>
        <translation>Цветовая схема «%1» недоступна и не была применена.</translation>
    </message>
    <message>
        <source>The language “%1” is not offered and was not applied.</source>
        <translation>Язык «%1» недоступен и не был применён.</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not installed.</source>
        <translation>Настроенный терминал «%1» не установлен.</translation>
    </message>
    <message>
        <source>Executable Files (*.exe *.bin hashcat);;All Files (*)</source>
        <translation>Исполняемые файлы (*.exe *.bin hashcat);;Все файлы (*)</translation>
    </message>
    <message>
        <source>No hashcat executable is configured; hashcat cannot be started.</source>
        <translation>Исполняемый файл hashcat не настроен; запустить hashcat невозможно.</translation>
    </message>
    <message>
        <source>The configured hashcat executable was not found on this computer.</source>
        <translation>Настроенный исполняемый файл hashcat не найден на этом компьютере.</translation>
    </message>
    <message>
        <source>Select the terminal hashcat is started in.</source>
        <translation>Выберите терминал, в котором запускается hashcat.</translation>
    </message>
    <message>
        <source>Select the hashcat executable</source>
        <translation>Выберите исполняемый файл hashcat</translation>
    </message>
    <message>
        <source>Invalid file</source>
        <translation>Недопустимый файл</translation>
    </message>
    <message>
        <source>The selected file is not an executable.</source>
        <translation>Выбранный файл не является исполняемым.</translation>
    </message>
    <message>
        <source>Restart required</source>
        <translation>Требуется перезапуск</translation>
    </message>
    <message>
        <source>Restart hashcat-gui to apply the new language.</source>
        <translation>Перезапустите hashcat-gui, чтобы применить новый язык.</translation>
    </message>
</context>
<context>
    <name>StateFile</name>
    <message>
        <source>The file is not valid JSON: %1</source>
        <translation>Файл не является допустимым JSON: %1</translation>
    </message>
    <message>
        <source>The file does not contain a JSON object.</source>
        <translation>Файл не содержит объекта JSON.</translation>
    </message>
    <message>
        <source>The file does not contain any sections.</source>
        <translation>Файл не содержит ни одного раздела.</translation>
    </message>
    <message>
        <source>Could not open “%1” for writing: %2</source>
        <translation>Не удалось открыть «%1» для записи: %2</translation>
    </message>
    <message>
        <source>Could not write to “%1”: %2</source>
        <translation>Не удалось записать в «%1»: %2</translation>
    </message>
    <message>
        <source>Could not open “%1” for reading: %2</source>
        <translation>Не удалось открыть «%1» для чтения: %2</translation>
    </message>
    <message>
        <source>The file does not contain a “%1” section.</source>
        <translation>В файле нет раздела «%1».</translation>
    </message>
    <message>
        <source>The file does not contain a “%1” section; it holds %2.</source>
        <translation>В файле нет раздела «%1»; он содержит %2.</translation>
    </message>
    <message>
        <source>The “%1” section does not contain any values.</source>
        <translation>Раздел «%1» не содержит значений.</translation>
    </message>
    <message>
        <source> and </source>
        <translation> и </translation>
    </message>
    <message>
        <source> and %1 more</source>
        <translation> и ещё %1</translation>
    </message>
    <message>
        <source>The file names values this version does not know, so they were not applied: %1.</source>
        <translation>Файл содержит значения, неизвестные этой версии, поэтому они не были применены: %1.</translation>
    </message>
    <message>
        <source>The file holds values of the wrong type, so they were not applied: %1.</source>
        <translation>Файл содержит значения неверного типа, поэтому они не были применены: %1.</translation>
    </message>
</context>
<context>
    <name>UpdateChecker</name>
    <message>
        <source>GitHub answered with HTTP status %1: %2</source>
        <translation>GitHub ответил со статусом HTTP %1: %2</translation>
    </message>
    <message>
        <source>GitHub answered with HTTP status %1.</source>
        <translation>GitHub ответил со статусом HTTP %1.</translation>
    </message>
    <message>
        <source>The release information is not valid JSON.</source>
        <translation>Информация о релизе не является допустимым JSON.</translation>
    </message>
    <message>
        <source>The release information does not name a version.</source>
        <translation>Информация о релизе не указывает версию.</translation>
    </message>
    <message>
        <source>The release version %1 is not a valid version number.</source>
        <translation>Версия релиза %1 не является допустимым номером версии.</translation>
    </message>
    <message>
        <source>The installed version %1 is not a valid version number.</source>
        <translation>Установленная версия %1 не является допустимым номером версии.</translation>
    </message>
</context>
</TS>
