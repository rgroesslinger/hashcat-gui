<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_TW" sourcelanguage="en">
<context>
    <name>AboutDialog</name>
    <message>
        <source>About</source>
        <translation>關於</translation>
    </message>
    <message>
        <source>Graphical user interface for hashcat, the password recovery utility</source>
        <translation>密碼復原工具 hashcat 的圖形使用者介面</translation>
    </message>
    <message>
        <source>Version information</source>
        <translation>版本資訊</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>未知</translation>
    </message>
    <message>
        <source>Updates</source>
        <translation>更新</translation>
    </message>
    <message>
        <source>Checking for updates…</source>
        <translation>正在檢查更新…</translation>
    </message>
    <message>
        <source>System information</source>
        <translation>系統資訊</translation>
    </message>
    <message>
        <source>Operating system</source>
        <translation>作業系統</translation>
    </message>
    <message>
        <source>Links</source>
        <translation>連結</translation>
    </message>
    <message>
        <source>License</source>
        <translation>授權</translation>
    </message>
    <message>
        <source>Copy the application, toolkit, system and hashcat versions to the clipboard</source>
        <translation>將應用程式、工具套件、系統及 hashcat 的版本複製到剪貼簿</translation>
    </message>
    <message>
        <source>Copy details</source>
        <translation>複製詳細資料</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>確定</translation>
    </message>
    <message>
        <source>Copied to the clipboard</source>
        <translation>已複製到剪貼簿</translation>
    </message>
    <message>
        <source>Project page</source>
        <translation>專案頁面</translation>
    </message>
    <message>
        <source>Report an issue</source>
        <translation>回報問題</translation>
    </message>
    <message>
        <source>hashcat</source>
        <translation>hashcat</translation>
    </message>
    <message>
        <source>hashcat FAQ</source>
        <translation>hashcat 常見問題</translation>
    </message>
    <message>
        <source>hashcat-gui is licensed under the %1.</source>
        <translation>hashcat-gui 採用 %1 授權。</translation>
    </message>
    <message>
        <source>GNU GPL v3.0 or later</source>
        <translation>GNU GPL v3.0 或更新版本</translation>
    </message>
    <message>
        <source>Not configured</source>
        <translation>未設定</translation>
    </message>
    <message>
        <source>Error: %1</source>
        <translation>錯誤：%1</translation>
    </message>
    <message>
        <source>A newer version is available: %1.</source>
        <translation>有較新的版本可用：%1。</translation>
    </message>
    <message>
        <source>Release page</source>
        <translation>發行頁面</translation>
    </message>
    <message>
        <source>No updates available.</source>
        <translation>沒有可用的更新。</translation>
    </message>
    <message>
        <source>Could not check for updates.</source>
        <translation>無法檢查更新。</translation>
    </message>
    <message>
        <source>Could not check for updates: %1</source>
        <translation>無法檢查更新：%1</translation>
    </message>
</context>
<context>
    <name>HashcatInfoParser</name>
    <message>
        <source>Invalid JSON returned from hashcat.</source>
        <translation>hashcat 傳回了無效的 JSON。</translation>
    </message>
    <message>
        <source>hashcat did not report any hash types.</source>
        <translation>hashcat 未回報任何雜湊類型。</translation>
    </message>
</context>
<context>
    <name>HelperUtils</name>
    <message>
        <source>The hashcat path is not configured.</source>
        <translation>尚未設定 hashcat 的路徑。</translation>
    </message>
    <message>
        <source>Failed to start hashcat
%1</source>
        <translation>無法啟動 hashcat
%1</translation>
    </message>
    <message>
        <source>hashcat timed out
%1</source>
        <translation>hashcat 逾時
%1</translation>
    </message>
    <message>
        <source>No supported terminal was found on this system. hashcat-gui needs one to show the hashcat output.</source>
        <translation>此系統上找不到支援的終端機。hashcat-gui 需要終端機來顯示 hashcat 的輸出。</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <source>Attack</source>
        <translation>攻擊</translation>
    </message>
    <message>
        <source>Hash file or hash:</source>
        <translation>雜湊檔案或雜湊：</translation>
    </message>
    <message>
        <source>The hash to crack, a path to a file of hashes, or a .hccapx WPA capture. Paste a single hash here, or point it at a file.</source>
        <translation>要破解的雜湊、雜湊檔案的路徑，或 .hccapx WPA 擷取檔。可在此貼上單一雜湊，或指定一個檔案。</translation>
    </message>
    <message>
        <source>Salt is hex</source>
        <translation>鹽值為十六進位</translation>
    </message>
    <message>
        <source>Remove already-cracked hashes from the potfile so a later run does not report them again. Refused in attack mode 9.</source>
        <translation>從 potfile 中移除已破解的雜湊，使之後的執行不再重複回報。在攻擊模式 9 中會被拒絕。</translation>
    </message>
    <message>
        <source>Remove hash from hash list once it is cracked</source>
        <translation>破解後從雜湊清單中移除該雜湊</translation>
    </message>
    <message>
        <source>Ignore the @username part of hashes (in hash modes that carry one) so the same word can satisfy several accounts.</source>
        <translation>忽略雜湊中的 @username 部分（適用於包含該部分的雜湊模式），使同一個單字可以符合多個帳號。</translation>
    </message>
    <message>
        <source>Ignore username in hash file</source>
        <translation>忽略雜湊檔案中的使用者名稱</translation>
    </message>
    <message>
        <source>Mode:</source>
        <translation>模式：</translation>
    </message>
    <message>
        <source>Attack mode. Each choice reveals its own options below.</source>
        <translation>攻擊模式。每個選項會在下方顯示各自的選項。</translation>
    </message>
    <message>
        <source>Hash type:</source>
        <translation>雜湊類型：</translation>
    </message>
    <message>
        <source>Hash recipe:</source>
        <translation>雜湊配方：</translation>
    </message>
    <message>
        <source>Unix: &apos;md5(md5(salt) . pass)&apos;
               Windows: &quot;md5(&apos;abc&apos; . pass)&quot;</source>
        <translation>Unix: &apos;md5(md5(salt) . pass)&apos;
               Windows: &quot;md5(&apos;abc&apos; . pass)&quot;</translation>
    </message>
    <message>
        <source>Wordlists</source>
        <translation>字典</translation>
    </message>
    <message>
        <source>Move item up</source>
        <translation>上移項目</translation>
    </message>
    <message>
        <source>Move item down</source>
        <translation>下移項目</translation>
    </message>
    <message>
        <source>hashcat reads every file directly inside the folder as one wordlist.</source>
        <translation>hashcat 會將資料夾中直接包含的每個檔案作為一個字典讀取。</translation>
    </message>
    <message>
        <source>Add folder…</source>
        <translation>新增資料夾…</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>移除</translation>
    </message>
    <message>
        <source>Only checked rows are used. A folder is one wordlist: hashcat reads the files directly inside it.</source>
        <translation>僅使用已勾選的列。一個資料夾視為一個字典：hashcat 會讀取其中直接包含的檔案。</translation>
    </message>
    <message>
        <source>Wordlist is hex</source>
        <translation>字典為十六進位</translation>
    </message>
    <message>
        <source>Encoding from:</source>
        <translation>來源編碼：</translation>
    </message>
    <message>
        <source>Encoding to:</source>
        <translation>目標編碼：</translation>
    </message>
    <message>
        <source>Settings (no wordlist):</source>
        <translation>設定（無字典）：</translation>
    </message>
    <message>
        <source>Positional settings, separated by spaces. Only used when no wordlist is selected: with one, hashcat pairs the two files by line. “phases” picks which phases run, cheapest first; the default “words,rules,pcfg” ends only when stopped. Phase-prefixed settings: “rules.rulefile”, “rules.rulemax”, “words.hints”, “pcfg.hintaccount”, …</source>
        <translation>位置設定，以空格分隔。僅在未選擇字典時使用：選擇字典後，hashcat 會逐行配對兩個檔案。「phases」選擇要執行的階段，從成本最低的開始；預設的「words,rules,pcfg」只有在被停止時才會結束。帶階段前綴的設定：「rules.rulefile」、「rules.rulemax」、「words.hints」、「pcfg.hintaccount」、…</translation>
    </message>
    <message>
        <source>PCFG</source>
        <translation>PCFG</translation>
    </message>
    <message>
        <source>A row is a ruleset file, a folder with a Grammar/grammar.txt, or a bundled name. Only checked rows are used; with none checked, hashcat’s bundled default-passwords runs. hints is also bundled.</source>
        <translation>一列可以是規則集檔案、包含 Grammar/grammar.txt 的資料夾，或內建名稱。僅使用已勾選的列；若沒有勾選任何列，則執行 hashcat 內建的 default-passwords。hints 也是內建的。</translation>
    </message>
    <message>
        <source>A ruleset folder needs a Grammar/grammar.txt.</source>
        <translation>規則集資料夾需要包含 Grammar/grammar.txt。</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Scale:</translation>
    </message>
    <message>
        <source>Quantization steps per bit (1–64)</source>
        <translation>每位元的量化步數 (1–64)</translation>
    </message>
    <message>
        <source>Costmax:</source>
        <translation>Costmax:</translation>
    </message>
    <message>
        <source>Highest cost level to enumerate (1–64)</source>
        <translation>要列舉的最高成本等級 (1–64)</translation>
    </message>
    <message>
        <source>Weights:</source>
        <translation>Weights:</translation>
    </message>
    <message>
        <source>One value per ruleset, plain numbers separated by colons, e.g. 2:1 for two rulesets. The count must match the number of rulesets.</source>
        <translation>每個規則集一個值，為以冒號分隔的純數字，例如兩個規則集寫作 2:1。值的數量必須與規則集的數量一致。</translation>
    </message>
    <message>
        <source>Threads:</source>
        <translation>執行緒：</translation>
    </message>
    <message>
        <source>Cores that generate base words (0–16); 0 runs on the calling thread</source>
        <translation>產生基礎單字的核心數 (0–16)；0 表示在呼叫端執行緒中執行</translation>
    </message>
    <message>
        <source>auto</source>
        <translation>自動</translation>
    </message>
    <message>
        <source>Kbits:</source>
        <translation>Kbits:</translation>
    </message>
    <message>
        <source>Bits one inner loop may span (0–27)</source>
        <translation>一個內層迴圈可跨越的位元數 (0–27)</translation>
    </message>
    <message>
        <source>Maxword:</source>
        <translation>Maxword:</translation>
    </message>
    <message>
        <source>Candidates the kernel builds per base word: 0, 16 or 32 (a multiple of 16)</source>
        <translation>核心為每個基礎單字建立的候選數：0、16 或 32（16 的倍數）</translation>
    </message>
    <message>
        <source>Maxgain:</source>
        <translation>Maxgain:</translation>
    </message>
    <message>
        <source>1.0–64.0; decimal point is a dot, not the locale separator</source>
        <translation>1.0–64.0；小數點為英文句點，而非地區設定中的分隔符號</translation>
    </message>
    <message>
        <source>Walk:</source>
        <translation>Walk:</translation>
    </message>
    <message>
        <source>1 steps the last token instead of unraking it</source>
        <translation>1 表示對最後一個 token 逐步前進，而不是對其執行「unraking」</translation>
    </message>
    <message>
        <source>Mask:</source>
        <translation>遮罩：</translation>
    </message>
    <message>
        <source>Hold the grammar to the candidates this mask admits; cannot be combined with omen=1, or with hintaccount while the hint words come from the accounts (give hintwords or hintfile instead)</source>
        <translation>將語法限制在此遮罩允許的候選範圍內；不能與 omen=1 同時使用，且在提示字來自帳號時也不能與 hintaccount 同時使用（請改用 hintwords 或 hintfile）</translation>
    </message>
    <message>
        <source>Extra settings (key=value):</source>
        <translation>額外設定 (key=value)：</translation>
    </message>
    <message>
        <source>Additional key=value settings, separated by spaces: omen, cache, lookup, pwmin, pwmax, hintaccount, hintwords, hintfile, hintrank, hintrepeat. Everything after the first key=value is a setting, so rulesets must come first.</source>
        <translation>以空格分隔的額外 key=value 設定：omen、cache、lookup、pwmin、pwmax、hintaccount、hintwords、hintfile、hintrank、hintrepeat。第一個 key=value 之後的所有內容都視為設定，因此規則集必須放在前面。</translation>
    </message>
    <message>
        <source>Table</source>
        <translation>表格</translation>
    </message>
    <message>
        <source>Every checked row after the wordlist is a table</source>
        <translation>字典之後每個已勾選的列都是一個表格</translation>
    </message>
    <message>
        <source>Add tables…</source>
        <translation>新增表格…</translation>
    </message>
    <message>
        <source>Identity:</source>
        <translation>Identity:</translation>
    </message>
    <message>
        <source>Whether leaving a token unchanged is one of its choices: 1 (default) keeps it, 0 drops it so the table only produces variations. Cannot be combined with single=1.</source>
        <translation>是否將保持 token 不變作為其選擇之一：1（預設）保留，0 捨棄，使表格只產生變體。不能與 single=1 同時使用。</translation>
    </message>
    <message>
        <source>Template:</source>
        <translation>Template:</translation>
    </message>
    <message>
        <source>1 reads the wordlist as a set of patterns: no word is ever returned unchanged, and words matching no rule are dropped. 0 (default) keeps unchanged words.</source>
        <translation>1 將字典讀作一組模式：任何單字都不會原樣傳回，且不符合任何規則的單字會被捨棄。0（預設）保留未更改的單字。</translation>
    </message>
    <message>
        <source>Single:</source>
        <translation>Single:</translation>
    </message>
    <message>
        <source>single=1 makes each candidate carry exactly one substitution instead of the cross product of every matched position, usually a different, cheaper attack.</source>
        <translation>single=1 使每個候選恰好帶有一處取代，而不是所有符合位置的笛卡兒積；通常是一種不同且成本較低的攻擊。</translation>
    </message>
    <message>
        <source>Cap:</source>
        <translation>Cap:</translation>
    </message>
    <message>
        <source>Maximum replacements one source may carry in a word; 0 means no limit. Only limits the host-side cross product, not a single=1 run.</source>
        <translation>一個來源在單字中最多可帶有的取代數；0 表示不限制。僅限制主機端的笛卡兒積，不限制 single=1 的執行。</translation>
    </message>
    <message>
        <source>Maxperm:</source>
        <translation>Maxperm:</translation>
    </message>
    <message>
        <source>How many of one word’s cross product the host enumerates before giving up; 0 means no limit. Each device cell is never cut.</source>
        <translation>主機在放棄之前列舉一個單字笛卡兒積中的多少項；0 表示不限制。裝置單元從不會被截斷。</translation>
    </message>
    <message>
        <source>Additional key=value settings, separated by spaces. Everything after the first key=value is a setting, so the tables must come first.</source>
        <translation>以空格分隔的額外 key=value 設定。第一個 key=value 之後的所有內容都視為設定，因此表格必須放在前面。</translation>
    </message>
    <message>
        <source>Generic feed</source>
        <translation>通用 Feed</translation>
    </message>
    <message>
        <source>Feed:</source>
        <translation>Feed:</translation>
    </message>
    <message>
        <source>Candidate generator. Anything containing a slash is treated as a feed path; otherwise hashcat looks for feed_&lt;name&gt; under its feeds directory. Arguments below are feed-specific.</source>
        <translation>候選產生器。包含斜線的內容會被視為 feed 路徑；否則 hashcat 會在其 feeds 目錄下尋找 feed_&lt;name&gt;。下方的參數取決於具體的 feed。</translation>
    </message>
    <message>
        <source>wordlist, stdin, random, association, pcfg, table, hybrid, mask, or a feed path</source>
        <translation>wordlist、stdin、random、association、pcfg、table、hybrid、mask，或 feed 路徑</translation>
    </message>
    <message>
        <source>Browse…</source>
        <translation>瀏覽…</translation>
    </message>
    <message>
        <source>Argument:</source>
        <translation>參數：</translation>
    </message>
    <message>
        <source>Argument passed to the feed. Its meaning and count depend on the feed (the wordlist feed takes wordlist paths); extra tokens go straight to the feed.</source>
        <translation>傳遞給 feed 的參數。其意義與數量取決於 feed（wordlist feed 接受字典路徑）；多餘的 token 會直接交給 feed。</translation>
    </message>
    <message>
        <source>Add</source>
        <translation>新增</translation>
    </message>
    <message>
        <source>Arguments the feed reads, in order; only checked rows are used</source>
        <translation>feed 依序讀取的參數；僅使用已勾選的列</translation>
    </message>
    <message>
        <source>Rules</source>
        <translation>規則</translation>
    </message>
    <message>
        <source>Use rules files:</source>
        <translation>使用規則檔案：</translation>
    </message>
    <message>
        <source>Enable rules file slot 1 for this run.</source>
        <translation>為本次執行啟用規則檔案插槽 1。</translation>
    </message>
    <message>
        <source>Rules file:</source>
        <translation>規則檔案：</translation>
    </message>
    <message>
        <source>Rule file for slot 1; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>插槽 1 的規則檔案；套用於每個單字。留空則不使用該插槽。</translation>
    </message>
    <message>
        <source>Enable rules file slot 2 for this run.</source>
        <translation>為本次執行啟用規則檔案插槽 2。</translation>
    </message>
    <message>
        <source>Rule file for slot 2; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>插槽 2 的規則檔案；套用於每個單字。留空則不使用該插槽。</translation>
    </message>
    <message>
        <source>Enable rules file slot 3 for this run.</source>
        <translation>為本次執行啟用規則檔案插槽 3。</translation>
    </message>
    <message>
        <source>Rule file for slot 3; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>插槽 3 的規則檔案；套用於每個單字。留空則不使用該插槽。</translation>
    </message>
    <message>
        <source>Generate the rules automatically instead of reading them from a file.</source>
        <translation>自動產生規則，而不是從檔案中讀取。</translation>
    </message>
    <message>
        <source>Generate rules:</source>
        <translation>產生規則：</translation>
    </message>
    <message>
        <source>How many rules to generate for the finding-rule attack. No rules file is read, hashcat builds them itself; use an existing rule file with the slots above instead.</source>
        <translation>為規則尋找攻擊產生多少條規則。不會讀取規則檔案，hashcat 會自行建立規則；若要使用現有規則檔案，請改用上方的插槽。</translation>
    </message>
    <message>
        <source>Pass every rules file to hashcat in one run, instead of one run per file.</source>
        <translation>將所有規則檔案在一次執行中傳給 hashcat，而不是每個檔案執行一次。</translation>
    </message>
    <message>
        <source>Join -r files into one list</source>
        <translation>將 -r 檔案合併為一個清單</translation>
    </message>
    <message>
        <source>Wordlist rules</source>
        <translation>字典規則</translation>
    </message>
    <message>
        <source>Left rule:</source>
        <translation>左側規則：</translation>
    </message>
    <message>
        <source>Right rule:</source>
        <translation>右側規則：</translation>
    </message>
    <message>
        <source>Interpret the custom charsets as hexadecimal bytes. Applies to custom charset 1 through 4.</source>
        <translation>將自訂字元集解讀為十六進位位元組。適用於自訂字元集 1 至 4。</translation>
    </message>
    <message>
        <source>Charset is hex</source>
        <translation>字元集為十六進位</translation>
    </message>
    <message>
        <source>Mask</source>
        <translation>遮罩</translation>
    </message>
    <message>
        <source>Increment mask length</source>
        <translation>遞增遮罩長度</translation>
    </message>
    <message>
        <source>Min:</source>
        <translation>最小：</translation>
    </message>
    <message>
        <source>Max:</source>
        <translation>最大：</translation>
    </message>
    <message>
        <source>Custom charsets</source>
        <translation>自訂字元集</translation>
    </message>
    <message>
        <source>Charset 1:</source>
        <translation>字元集 1：</translation>
    </message>
    <message>
        <source>Charset 2:</source>
        <translation>字元集 2：</translation>
    </message>
    <message>
        <source>Charset 3:</source>
        <translation>字元集 3：</translation>
    </message>
    <message>
        <source>Charset 4:</source>
        <translation>字元集 4：</translation>
    </message>
    <message>
        <source>General</source>
        <translation>一般</translation>
    </message>
    <message>
        <source>Session and limits</source>
        <translation>工作階段與限制</translation>
    </message>
    <message>
        <source>Session name:</source>
        <translation>工作階段名稱：</translation>
    </message>
    <message>
        <source>Name hashcat uses for its restore file and status output.</source>
        <translation>hashcat 用於其還原檔案與狀態輸出的名稱。</translation>
    </message>
    <message>
        <source>Runtime limit:</source>
        <translation>執行時間限制：</translation>
    </message>
    <message>
        <source>unlimited</source>
        <translation>無限制</translation>
    </message>
    <message>
        <source>Skip:</source>
        <translation>跳過：</translation>
    </message>
    <message>
        <source>Limit:</source>
        <translation>限制：</translation>
    </message>
    <message>
        <source>Update status automatically</source>
        <translation>自動更新狀態</translation>
    </message>
    <message>
        <source>Open…</source>
        <translation>開啟…</translation>
    </message>
    <message>
        <source>Assume the salt is given in hexadecimal.</source>
        <translation>假定鹽值以十六進位給定。</translation>
    </message>
    <message>
        <source>Add files…</source>
        <translation>新增檔案…</translation>
    </message>
    <message>
        <source>Assume words in the wordlist are given in hexadecimal.</source>
        <translation>假定字典中的單字以十六進位給定。</translation>
    </message>
    <message>
        <source>Single rule applied to each word from the left wordlist.</source>
        <translation>套用於左側字典中每個單字的單一規則。</translation>
    </message>
    <message>
        <source>Single rule applied to each word from the right wordlist.</source>
        <translation>套用於右側字典中每個單字的單一規則。</translation>
    </message>
    <message>
        <source>Increment the mask length from the minimum to the maximum.</source>
        <translation>將遮罩長度從最小值遞增到最大值。</translation>
    </message>
    <message>
        <source>Minimum mask length; 0 uses hashcat’s default.</source>
        <translation>遮罩的最小長度；0 表示使用 hashcat 的預設值。</translation>
    </message>
    <message>
        <source>Maximum mask length; 0 uses hashcat’s default.</source>
        <translation>遮罩的最大長度；0 表示使用 hashcat 的預設值。</translation>
    </message>
    <message>
        <source>Abort the session after this many seconds; 0 runs until hashcat finishes.</source>
        <translation>在此秒數後中止工作階段；0 表示一直執行到 hashcat 結束。</translation>
    </message>
    <message>
        <source>&#xa0;s</source>
        <translation>&#xa0;秒</translation>
    </message>
    <message>
        <source>Skip this many candidates from the start.</source>
        <translation>從開頭跳過這麼多個候選。</translation>
    </message>
    <message>
        <source>Only test this many candidates after the skipped ones.</source>
        <translation>在跳過的候選之後，僅測試這麼多個候選。</translation>
    </message>
    <message>
        <source>Keep updating the run status (speed, progress, ETA) instead of printing one final line.</source>
        <translation>持續更新執行狀態（速度、進度、預計剩餘時間），而不是只列印最後一行。</translation>
    </message>
    <message>
        <source>Seconds between status updates.</source>
        <translation>狀態更新之間的間隔秒數。</translation>
    </message>
    <message>
        <source>Ignore warnings and start hashcat anyway. Use with care.</source>
        <translation>忽略警告並強制啟動 hashcat。請謹慎使用。</translation>
    </message>
    <message>
        <source>Force (ignore warnings)</source>
        <translation>強制（忽略警告）</translation>
    </message>
    <message>
        <source>Performance</source>
        <translation>效能</translation>
    </message>
    <message>
        <source>Enable optimized kernels (limits password length)</source>
        <translation>啟用最佳化核心（會限制密碼長度）</translation>
    </message>
    <message>
        <source>Print only the attack’s speed and quit, without cracking. Useful for benchmarking and comparing settings.</source>
        <translation>只輸出攻擊速度然後結束，不進行破解。適用於效能基準測試和比較設定。</translation>
    </message>
    <message>
        <source>Return expected speed of the attack, then quit</source>
        <translation>輸出攻擊的預期速度，然後結束</translation>
    </message>
    <message>
        <source>Resources</source>
        <translation>資源</translation>
    </message>
    <message>
        <source>CPU affinity:</source>
        <translation>CPU 親和性：</translation>
    </message>
    <message>
        <source>Backend devices:</source>
        <translation>後端裝置：</translation>
    </message>
    <message>
        <source>Device types:</source>
        <translation>裝置類型：</translation>
    </message>
    <message>
        <source>Pin hashcat to these CPU devices, separated by commas. Leave empty to let hashcat decide.</source>
        <translation>將 hashcat 綁定到這些 CPU 裝置，以逗號分隔。留空則由 hashcat 決定。</translation>
    </message>
    <message>
        <source>Devices to use, separated by commas. Leave empty for all devices.</source>
        <translation>要使用的裝置，以逗號分隔。留空則使用所有裝置。</translation>
    </message>
    <message>
        <source>Abort when a device reaches this temperature.</source>
        <translation>當裝置達到此溫度時中止。</translation>
    </message>
    <message>
        <source>&#xa0;°C</source>
        <translation>&#xa0;°C</translation>
    </message>
    <message>
        <source>Built-in variables:

&lt;hash&gt; = current hash filename or hash
&lt;unixtime&gt; = current Unix timestamp</source>
        <translation>內建變數：

&lt;hash&gt; = 目前的雜湊檔案名稱或雜湊
&lt;unixtime&gt; = 目前的 Unix 時間戳記</translation>
    </message>
    <message>
        <source>Save…</source>
        <translation>儲存…</translation>
    </message>
    <message>
        <source>Do not write recovered hashes to the potfile.</source>
        <translation>不將已復原的雜湊寫入 potfile。</translation>
    </message>
    <message>
        <source>Settings…</source>
        <translation>設定…</translation>
    </message>
    <message>
        <source>Import Settings…</source>
        <translation>匯入設定…</translation>
    </message>
    <message>
        <source>Export Settings…</source>
        <translation>匯出設定…</translation>
    </message>
    <message>
        <source>Export Session…</source>
        <translation>匯出工作階段…</translation>
    </message>
    <message>
        <source>Import Session…</source>
        <translation>匯入工作階段…</translation>
    </message>
    <message>
        <source>Abort above</source>
        <translation>超過以下溫度中止</translation>
    </message>
    <message>
        <source>Cache path:</source>
        <translation>快取路徑：</translation>
    </message>
    <message>
        <source>Directory hashcat keeps its cache in. Must exist.</source>
        <translation>hashcat 儲存其快取的目錄。必須存在。</translation>
    </message>
    <message>
        <source>Abort the session when an OpenCL device hotspot exceeds this many degrees Celsius. 0 disables the thermal limit.</source>
        <translation>當 OpenCL 裝置熱點溫度超過此攝氏度數時中止工作階段。0 表示停用溫度限制。</translation>
    </message>
    <message>
        <source>Outfile</source>
        <translation>輸出檔案</translation>
    </message>
    <message>
        <source>Write recovered hashes to file:</source>
        <translation>將已復原的雜湊寫入檔案：</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>格式：</translation>
    </message>
    <message>
        <source>Write the outfile as JSON. The outfile format is ignored.</source>
        <translation>將輸出檔案寫為 JSON。輸出檔案格式將被忽略。</translation>
    </message>
    <message>
        <source>Write JSON</source>
        <translation>寫入 JSON</translation>
    </message>
    <message>
        <source>Do not write the potfile</source>
        <translation>不寫入 potfile</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>開始</translation>
    </message>
    <message>
        <source>Copy command to clipboard</source>
        <translation>將命令複製到剪貼簿</translation>
    </message>
    <message>
        <source>Help</source>
        <translation>說明</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>工具</translation>
    </message>
    <message>
        <source>File</source>
        <translation>檔案</translation>
    </message>
    <message>
        <source>About</source>
        <translation>關於</translation>
    </message>
    <message>
        <source>Load Example Session</source>
        <translation>載入範例工作階段</translation>
    </message>
    <message>
        <source>Reset Session</source>
        <translation>重設工作階段</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation>結束</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>關於 Qt</translation>
    </message>
    <message>
        <source>Built-in charsets:</source>
        <translation>內建字元集：</translation>
    </message>
    <message>
        <source>Outfile format to use, separated by commas.
Default: 1,2</source>
        <translation>要使用的輸出檔案格式，以逗號分隔。
預設值：1,2</translation>
    </message>
    <message>
        <source>Common encodings:</source>
        <translation>常用編碼：</translation>
    </message>
    <message>
        <source>Force the internal wordlist encoding from this code. Leave empty to let hashcat decide.</source>
        <translation>強制內部字典編碼的來源編碼為此代碼。留空則由 hashcat 決定。</translation>
    </message>
    <message>
        <source>Force the internal wordlist encoding to this code. Leave empty to let hashcat decide.</source>
        <translation>強制內部字典編碼的目標編碼為此代碼。留空則由 hashcat 決定。</translation>
    </message>
    <message>
        <source>JSON Files (*.json)</source>
        <translation>JSON 檔案 (*.json)</translation>
    </message>
    <message>
        <source>Exported</source>
        <translation>已匯出</translation>
    </message>
    <message>
        <source>Export failed</source>
        <translation>匯出失敗</translation>
    </message>
    <message>
        <source>Import failed</source>
        <translation>匯入失敗</translation>
    </message>
    <message>
        <source>Imported</source>
        <translation>已匯入</translation>
    </message>
    <message>
        <source>The file was made by an older version of hashcat-gui (format version %1; this version reads format version %2).</source>
        <translation>該檔案由舊版 hashcat-gui 建立（格式版本 %1；此版本讀取格式版本 %2）。</translation>
    </message>
    <message>
        <source>The file holds sections this import does not use: %1.</source>
        <translation>該檔案包含此匯入未使用的區段：%1。</translation>
    </message>
    <message>
        <source>The file is in format version %1, but this version reads format version %2, so it was not applied. Update hashcat-gui to read it.</source>
        <translation>該檔案的格式版本為 %1，但此版本讀取的是格式版本 %2，因此未套用。請更新 hashcat-gui 以讀取該檔案。</translation>
    </message>
    <message>
        <source>The session file is in format version %1, but this version reads format version %2, so the session was not restored. The file was left as it is, so the session of this run is not stored either. Update hashcat-gui to read it.</source>
        <translation>工作階段檔案的格式版本為 %1，但此版本讀取的是格式版本 %2，因此未還原工作階段。檔案已保持原樣，因此本次執行的工作階段也不會被儲存。請更新 hashcat-gui 以讀取該檔案。</translation>
    </message>
    <message>
        <source>Settings exported to %1.</source>
        <translation>設定已匯出到 %1。</translation>
    </message>
    <message>
        <source>OpenCL device types to use, separated by commas.</source>
        <translation>要使用的 OpenCL 裝置類型，以逗號分隔。</translation>
    </message>
    <message>
        <source>Export session</source>
        <translation>匯出工作階段</translation>
    </message>
    <message>
        <source>Session exported to “%1”.</source>
        <translation>工作階段已匯出到「%1」。</translation>
    </message>
    <message>
        <source>Import session</source>
        <translation>匯入工作階段</translation>
    </message>
    <message>
        <source>Session imported from “%1”.</source>
        <translation>已從「%1」匯入工作階段。</translation>
    </message>
    <message>
        <source>Session imported from “%1”.

%2</source>
        <translation>已從「%1」匯入工作階段。

%2</translation>
    </message>
    <message>
        <source>Export settings</source>
        <translation>匯出設定</translation>
    </message>
    <message>
        <source>Import settings</source>
        <translation>匯入設定</translation>
    </message>
    <message>
        <source>Settings loaded from “%1”.</source>
        <translation>已從「%1」載入設定。</translation>
    </message>
    <message>
        <source>Review them and save to apply them.</source>
        <translation>請檢查設定並儲存以套用。</translation>
    </message>
    <message>
        <source>Load example session</source>
        <translation>載入範例工作階段</translation>
    </message>
    <message>
        <source>Reset session</source>
        <translation>重設工作階段</translation>
    </message>
    <message>
        <source>Reset every field to its default? The current session is lost.</source>
        <translation>要將所有欄位重設為預設值嗎？目前的工作階段將會遺失。</translation>
    </message>
    <message>
        <source>Session warning</source>
        <translation>工作階段警告</translation>
    </message>
    <message>
        <source>The session file could not be read, so the session was not restored. The file was left in place.

%1</source>
        <translation>無法讀取工作階段檔案，因此未還原工作階段。檔案已保持原樣。

%1</translation>
    </message>
    <message>
        <source>The session names the hash type %1, which this hashcat version does not support.</source>
        <translation>工作階段指定了雜湊類型 %1，此版本的 hashcat 不支援該類型。</translation>
    </message>
    <message>
        <source>Save failed</source>
        <translation>儲存失敗</translation>
    </message>
    <message>
        <source>Straight</source>
        <translation>字典</translation>
    </message>
    <message>
        <source>Combination</source>
        <translation>組合</translation>
    </message>
    <message>
        <source>Brute-force</source>
        <translation>暴力破解</translation>
    </message>
    <message>
        <source>Hybrid Wordlist + Mask</source>
        <translation>混合：字典 + 遮罩</translation>
    </message>
    <message>
        <source>Hybrid Mask + Wordlist</source>
        <translation>混合：遮罩 + 字典</translation>
    </message>
    <message>
        <source>Generic (feed)</source>
        <translation>通用（feed）</translation>
    </message>
    <message>
        <source>Association</source>
        <translation>關聯</translation>
    </message>
    <message>
        <source>Hybrid (mask + ?w/?q)</source>
        <translation>混合（遮罩 + ?w/?q）</translation>
    </message>
    <message>
        <source>Updating…</source>
        <translation>正在更新…</translation>
    </message>
    <message>
        <source>Failed to load hash types</source>
        <translation>無法載入雜湊類型</translation>
    </message>
    <message>
        <source>This mode needs exactly 2 wordlists (left, right).</source>
        <translation>此模式需要恰好 2 個字典（左、右）。</translation>
    </message>
    <message>
        <source>This mode runs hashcat’s bundled default-passwords ruleset; name one or more to use your own.</source>
        <translation>此模式會執行 hashcat 內建的 default-passwords 規則集；請指定一個或多個規則集以使用您自己的。</translation>
    </message>
    <message>
        <source>This mode needs one wordlist and at least one table.</source>
        <translation>此模式需要一個字典和至少一個表格。</translation>
    </message>
    <message>
        <source>This mode needs a feed. Arguments depend on the feed — random and stdin need none, wordlist needs a path.</source>
        <translation>此模式需要一個 feed。參數取決於 feed：random 和 stdin 不需要參數，wordlist 需要一個路徑。</translation>
    </message>
    <message>
        <source>This mode needs at most one wordlist. Without one the words come from the hash file and the settings line can bound the run; with one the two files are paired by line.</source>
        <translation>此模式最多需要一個字典。沒有字典時，單字來自雜湊檔案，設定列可以限定執行範圍；有字典時，兩個檔案將逐行配對。</translation>
    </message>
    <message>
        <source>This mode needs a mask with exactly one ?w; a ?q after ?w names a second wordlist.</source>
        <translation>此模式需要恰好包含一個 ?w 的遮罩；?w 之後的 ?q 表示第二個字典。</translation>
    </message>
    <message>
        <source>Open hash file</source>
        <translation>開啟雜湊檔案</translation>
    </message>
    <message>
        <source>Save output file</source>
        <translation>儲存輸出檔案</translation>
    </message>
    <message>
        <source>Open cache directory</source>
        <translation>開啟快取目錄</translation>
    </message>
    <message>
        <source>Add wordlists</source>
        <translation>新增字典</translation>
    </message>
    <message>
        <source>Add wordlist folder</source>
        <translation>新增字典資料夾</translation>
    </message>
    <message>
        <source>Add rulesets</source>
        <translation>新增規則集</translation>
    </message>
    <message>
        <source>Add ruleset folder</source>
        <translation>新增規則集資料夾</translation>
    </message>
    <message>
        <source>Add tables</source>
        <translation>新增表格</translation>
    </message>
    <message>
        <source>Open feed file</source>
        <translation>開啟 feed 檔案</translation>
    </message>
    <message>
        <source>This folder has no wordlists. hashcat reads the regular files directly inside it, not subfolders.</source>
        <translation>此資料夾中沒有字典。hashcat 讀取其中直接包含的一般檔案，不讀取子資料夾。</translation>
    </message>
    <message>
        <source>This ruleset folder has no Grammar/grammar.txt:
%1</source>
        <translation>此規則集資料夾中沒有 Grammar/grammar.txt：
%1</translation>
    </message>
    <message>
        <source>This ruleset does not exist:
%1</source>
        <translation>此規則集不存在：
%1</translation>
    </message>
    <message>
        <source>A table must be a file; hashcat does not read a folder:
%1</source>
        <translation>表格必須是檔案；hashcat 不讀取資料夾：
%1</translation>
    </message>
    <message>
        <source>This table does not exist:
%1</source>
        <translation>此表格不存在：
%1</translation>
    </message>
    <message>
        <source>Open rules file</source>
        <translation>開啟規則檔案</translation>
    </message>
    <message>
        <source>Could not start “%1”: %2</source>
        <translation>無法啟動「%1」：%2</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not available. Available terminals: %2
Navigate to %3 → %4 to change it.</source>
        <translation>已設定的終端機「%1」無法使用。可用的終端機：%2
請前往 %3 → %4 進行修改。</translation>
    </message>
    <message>
        <source>This mode needs exactly %1 wordlists (left, right).
Currently selected: %2.</source>
        <translation>此模式需要恰好 %1 個字典（左、右）。
目前已選擇：%2 個。</translation>
    </message>
    <message>
        <source>Failed to obtain supported hash types.
Error: %1</source>
        <translation>無法取得支援的雜湊類型。
錯誤：%1</translation>
    </message>
    <message>
        <source>The hash types could not be loaded. Check the hashcat path in Settings.</source>
        <translation>無法載入雜湊類型。請在設定中檢查 hashcat 路徑。</translation>
    </message>
    <message>
        <source>Set the hashcat path under %1 → %2 to load the available hash types.</source>
        <translation>請在 %1 → %2 中設定 hashcat 路徑，以載入可用的雜湊類型。</translation>
    </message>
    <message>
        <source>This mode needs a wordlist and a mask.</source>
        <translation>此模式需要一個字典和一個遮罩。</translation>
    </message>
    <message>
        <source>This mode needs a mask.</source>
        <translation>此模式需要一個遮罩。</translation>
    </message>
    <message>
        <source>Load the %1 example? The current session is lost.

The example cracks a test hash; hashcat should recover the password %2.</source>
        <translation>要載入 %1 範例嗎？目前的工作階段將會遺失。

該範例會破解一個測試雜湊；hashcat 應能復原出密碼 %2。</translation>
    </message>
    <message>
        <source>Could not create the example directory “%1”.</source>
        <translation>無法建立範例目錄「%1」。</translation>
    </message>
    <message>
        <source>Could not read the bundled file “%1”: %2</source>
        <translation>無法讀取內建檔案「%1」：%2</translation>
    </message>
    <message>
        <source>Could not write “%1”: %2</source>
        <translation>無法寫入「%1」：%2</translation>
    </message>
    <message>
        <source>This mode needs at least one wordlist.</source>
        <translation>此模式需要至少一個字典。</translation>
    </message>
    <message>
        <source>This wordlist does not exist:
%1</source>
        <translation>此字典不存在：
%1</translation>
    </message>
    <message>
        <source>Copied to the clipboard</source>
        <translation>已複製到剪貼簿</translation>
    </message>
    <message>
        <source>Launch failed</source>
        <translation>啟動失敗</translation>
    </message>
    <message>
        <source>Please choose a hash file.</source>
        <translation>請選擇一個雜湊檔案。</translation>
    </message>
    <message>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to configure the path to the hashcat executable.</source>
        <translation>請前往 &lt;b&gt;%1 → %2&lt;/b&gt; 設定 hashcat 可執行檔的路徑。</translation>
    </message>
    <message>
        <source>The configured hashcat executable does not exist:
%1
Navigate to %2 → %3 to change it.</source>
        <translation>設定的 hashcat 可執行檔不存在：
%1
請前往 %2 → %3 進行修改。</translation>
    </message>
    <message>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to select the terminal used for launching.</source>
        <translation>請前往 &lt;b&gt;%1 → %2&lt;/b&gt; 選擇用於啟動的終端機。</translation>
    </message>
    <message>
        <source>This mode takes at most %1 wordlists.
Currently selected: %2.</source>
        <translation>此模式最多接受 %1 個字典。
目前已選擇：%2 個。</translation>
    </message>
    <message>
        <source>hashcat pairs the two files by line here; this GUI offers only one wordlist for that role.</source>
        <translation>在此處 hashcat 會逐行配對兩個檔案；此介面只為該角色提供一個字典。</translation>
    </message>
    <message>
        <source>This wordlist has to be a file: hashcat reads it as the multiplied wordlist (the ?q side), and a folder cannot be used there:
%1</source>
        <translation>此字典必須是檔案：hashcat 將其作為被乘的字典（?q 一側）讀取，此處不能使用資料夾：
%1</translation>
    </message>
    <message>
        <source>This mode needs at least one table.</source>
        <translation>此模式需要至少一個表格。</translation>
    </message>
    <message>
        <source>This mode needs a feed: a bundled name (wordlist, stdin, random, …) or the path of a feed file.</source>
        <translation>此模式需要一個 feed：內建名稱（wordlist、stdin、random、…）或 feed 檔案的路徑。</translation>
    </message>
    <message>
        <source>This mode pairs the wordlists with the mask; the mask needs a ?w for the word.</source>
        <translation>此模式將字典與遮罩配對；遮罩需要一個 ?w 來代表單字。</translation>
    </message>
    <message>
        <source>The mask contains more than one ?w.</source>
        <translation>遮罩包含多個 ?w。</translation>
    </message>
    <message>
        <source>The mask contains a ?q before the first ?w.</source>
        <translation>遮罩在第一個 ?w 之前包含 ?q。</translation>
    </message>
    <message>
        <source>The mask contains more than one ?q.</source>
        <translation>遮罩包含多個 ?q。</translation>
    </message>
    <message>
        <source>The mask names a second word with ?q, but only one wordlist is selected.</source>
        <translation>遮罩用 ?q 指定了第二個單字，但只選擇了一個字典。</translation>
    </message>
    <message>
        <source>Two wordlists are selected, but the mask has no ?q for the second one.</source>
        <translation>已選擇兩個字典，但遮罩中沒有用於第二個字典的 ?q。</translation>
    </message>
    <message>
        <source>The increment is not offered in this hybrid mode.</source>
        <translation>此混合模式不提供遞增功能。</translation>
    </message>
    <message>
        <source>The ?w and ?q markers are only offered in the hybrid mode (mask + wordlists).</source>
        <translation>?w 和 ?q 標記僅在混合模式（遮罩 + 字典）中提供。</translation>
    </message>
    <message>
        <source>The setting “%1” is not valid for this mode.</source>
        <translation>設定「%1」對此模式無效。</translation>
    </message>
    <message>
        <source>The token “%1” is not a setting; settings are written as key=value.</source>
        <translation>token「%1」不是設定；設定應寫成 key=value 的形式。</translation>
    </message>
    <message>
        <source>The settings line applies only while the words come from the hash file. Clear either the settings line or the wordlist list.</source>
        <translation>設定列僅在單字來自雜湊檔案時才適用。請清除設定列或字典清單之一。</translation>
    </message>
    <message>
        <source>The hash type 4000 describes its hashes as a recipe, for example
md5(md5(salt) . pass). Enter it in the hash recipe field.</source>
        <translation>雜湊類型 4000 以配方的形式描述其雜湊，例如
md5(md5(salt) . pass)。請在雜湊配方欄位中輸入。</translation>
    </message>
    <message>
        <source>The minimum increment length is larger than the maximum.</source>
        <translation>遞增的最小長度大於最大長度。</translation>
    </message>
    <message>
        <source>The cache path must be an existing directory:
%1</source>
        <translation>快取路徑必須是已存在的目錄：
%1</translation>
    </message>
    <message>
        <source>The increment cannot be combined with the speed-only option.</source>
        <translation>遞增不能與僅測速選項同時使用。</translation>
    </message>
    <message>
        <source>Removing cracked hashes is not possible in the association attack mode.</source>
        <translation>在「關聯」攻擊模式下無法移除已破解的雜湊。</translation>
    </message>
</context>
<context>
    <name>SettingsDialog</name>
    <message>
        <source>Settings</source>
        <translation>設定</translation>
    </message>
    <message>
        <source>Not configured</source>
        <translation>未設定</translation>
    </message>
    <message>
        <source>hashcat</source>
        <translation>hashcat</translation>
    </message>
    <message>
        <source>&amp;hashcat executable</source>
        <translation>hashcat 可執行檔(&amp;H)</translation>
    </message>
    <message>
        <source>Select…</source>
        <translation>選取…</translation>
    </message>
    <message>
        <source>&amp;Terminal</source>
        <translation>終端機(&amp;T)</translation>
    </message>
    <message>
        <source>General</source>
        <translation>一般</translation>
    </message>
    <message>
        <source>&amp;Language</source>
        <translation>語言(&amp;L)</translation>
    </message>
    <message>
        <source>&amp;Color scheme</source>
        <translation>色彩配置(&amp;C)</translation>
    </message>
    <message>
        <source>Not all color schemes are available on all platforms.</source>
        <translation>並非所有色彩配置在所有平台上都可用。</translation>
    </message>
    <message>
        <source>&amp;Use short parameters</source>
        <translation>使用短參數(&amp;U)</translation>
    </message>
    <message>
        <source>Session and files</source>
        <translation>工作階段與檔案</translation>
    </message>
    <message>
        <source>&amp;Restore the last session at startup</source>
        <translation>啟動時還原上次的工作階段(&amp;R)</translation>
    </message>
    <message>
        <source>&amp;Session file</source>
        <translation>工作階段檔案(&amp;S)</translation>
    </message>
    <message>
        <source>The file the last session is stored in</source>
        <translation>儲存上次工作階段的檔案</translation>
    </message>
    <message>
        <source>Open folder</source>
        <translation>開啟資料夾</translation>
    </message>
    <message>
        <source>Open the folder the session is stored in</source>
        <translation>開啟儲存工作階段的資料夾</translation>
    </message>
    <message>
        <source>Settings &amp;file</source>
        <translation>設定檔案(&amp;F)</translation>
    </message>
    <message>
        <source>The file the settings are stored in</source>
        <translation>儲存設定的檔案</translation>
    </message>
    <message>
        <source>Open the folder the settings are stored in</source>
        <translation>開啟儲存設定的資料夾</translation>
    </message>
    <message>
        <source>Download from %1</source>
        <translation>從 %1 下載</translation>
    </message>
    <message>
        <source>Use the short option names (-m) instead of the long ones (--hash-type) in the command.</source>
        <translation>在命令中使用短選項名稱（-m）代替長選項名稱（--hash-type）。</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>儲存</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <source>Select a terminal</source>
        <translation>選擇終端機</translation>
    </message>
    <message>
        <source>System default</source>
        <translation>系統預設</translation>
    </message>
    <message>
        <source>Light</source>
        <translation>淺色</translation>
    </message>
    <message>
        <source>Dark</source>
        <translation>深色</translation>
    </message>
    <message>
        <source>The hashcat path “%1” was not found on this computer.</source>
        <translation>在此電腦上找不到 hashcat 路徑「%1」。</translation>
    </message>
    <message>
        <source>The terminal “%1” is not installed and was not applied.</source>
        <translation>終端機「%1」未安裝，未被套用。</translation>
    </message>
    <message>
        <source>The color scheme “%1” is not offered and was not applied.</source>
        <translation>未提供色彩配置「%1」，未被套用。</translation>
    </message>
    <message>
        <source>The language “%1” is not offered and was not applied.</source>
        <translation>未提供語言「%1」，未被套用。</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not installed.</source>
        <translation>已設定的終端機「%1」未安裝。</translation>
    </message>
    <message>
        <source>Executable Files (*.exe *.bin hashcat);;All Files (*)</source>
        <translation>可執行檔 (*.exe *.bin hashcat);;所有檔案 (*)</translation>
    </message>
    <message>
        <source>No hashcat executable is configured; hashcat cannot be started.</source>
        <translation>尚未設定 hashcat 可執行檔，無法啟動 hashcat。</translation>
    </message>
    <message>
        <source>The configured hashcat executable was not found on this computer.</source>
        <translation>在此電腦上找不到已設定的 hashcat 可執行檔。</translation>
    </message>
    <message>
        <source>Select the terminal hashcat is started in.</source>
        <translation>選擇用於啟動 hashcat 的終端機。</translation>
    </message>
    <message>
        <source>Select the hashcat executable</source>
        <translation>選擇 hashcat 可執行檔</translation>
    </message>
    <message>
        <source>Invalid file</source>
        <translation>無效的檔案</translation>
    </message>
    <message>
        <source>The selected file is not an executable.</source>
        <translation>所選檔案不是可執行檔。</translation>
    </message>
    <message>
        <source>Restart required</source>
        <translation>需要重新啟動</translation>
    </message>
    <message>
        <source>Restart hashcat-gui to apply the new language.</source>
        <translation>請重新啟動 hashcat-gui 以套用新語言。</translation>
    </message>
</context>
<context>
    <name>StateFile</name>
    <message>
        <source>The file is not valid JSON: %1</source>
        <translation>該檔案不是有效的 JSON：%1</translation>
    </message>
    <message>
        <source>The file does not contain a JSON object.</source>
        <translation>該檔案不包含 JSON 物件。</translation>
    </message>
    <message>
        <source>The file does not contain any sections.</source>
        <translation>該檔案不包含任何區段。</translation>
    </message>
    <message>
        <source>Could not open “%1” for writing: %2</source>
        <translation>無法開啟「%1」進行寫入：%2</translation>
    </message>
    <message>
        <source>Could not write to “%1”: %2</source>
        <translation>無法寫入「%1」：%2</translation>
    </message>
    <message>
        <source>Could not open “%1” for reading: %2</source>
        <translation>無法開啟「%1」進行讀取：%2</translation>
    </message>
    <message>
        <source>The file does not contain a “%1” section.</source>
        <translation>檔案中不包含「%1」區段。</translation>
    </message>
    <message>
        <source>The file does not contain a “%1” section; it holds %2.</source>
        <translation>檔案中不包含「%1」區段；其中包含 %2。</translation>
    </message>
    <message>
        <source>The “%1” section does not contain any values.</source>
        <translation>「%1」區段不包含任何值。</translation>
    </message>
    <message>
        <source> and </source>
        <translation> 和 </translation>
    </message>
    <message>
        <source> and %1 more</source>
        <translation> 以及另外 %1 項</translation>
    </message>
    <message>
        <source>The file names values this version does not know, so they were not applied: %1.</source>
        <translation>該檔案包含此版本無法辨識的值，因此未套用：%1。</translation>
    </message>
    <message>
        <source>The file holds values of the wrong type, so they were not applied: %1.</source>
        <translation>該檔案包含類型錯誤的值，因此未套用：%1。</translation>
    </message>
</context>
<context>
    <name>UpdateChecker</name>
    <message>
        <source>GitHub answered with HTTP status %1: %2</source>
        <translation>GitHub 傳回了 HTTP 狀態 %1：%2</translation>
    </message>
    <message>
        <source>GitHub answered with HTTP status %1.</source>
        <translation>GitHub 傳回了 HTTP 狀態 %1。</translation>
    </message>
    <message>
        <source>The release information is not valid JSON.</source>
        <translation>發行資訊不是有效的 JSON。</translation>
    </message>
    <message>
        <source>The release information does not name a version.</source>
        <translation>發行資訊中未指明版本。</translation>
    </message>
    <message>
        <source>The release version %1 is not a valid version number.</source>
        <translation>發行版本 %1 不是有效的版本號碼。</translation>
    </message>
    <message>
        <source>The installed version %1 is not a valid version number.</source>
        <translation>已安裝的版本 %1 不是有效的版本號碼。</translation>
    </message>
</context>
</TS>
