<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN">
<context>
    <name>AboutDialog</name>
    <message>
        <source>About</source>
        <translation>关于</translation>
    </message>
    <message>
        <source>Graphical user interface for hashcat, the password recovery utility</source>
        <translation>密码恢复工具 hashcat 的图形用户界面</translation>
    </message>
    <message>
        <source>Version information</source>
        <translation>版本信息</translation>
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
        <translation>正在检查更新…</translation>
    </message>
    <message>
        <source>System information</source>
        <translation>系统信息</translation>
    </message>
    <message>
        <source>Operating system</source>
        <translation>操作系统</translation>
    </message>
    <message>
        <source>Links</source>
        <translation>链接</translation>
    </message>
    <message>
        <source>License</source>
        <translation>许可证</translation>
    </message>
    <message>
        <source>Copy the application, toolkit, system and hashcat versions to the clipboard</source>
        <translation>将应用程序、工具包、系统和 hashcat 的版本复制到剪贴板</translation>
    </message>
    <message>
        <source>Copy details</source>
        <translation>复制详细信息</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>确定</translation>
    </message>
    <message>
        <source>Copied to the clipboard</source>
        <translation>已复制到剪贴板</translation>
    </message>
    <message>
        <source>Project page</source>
        <translation>项目主页</translation>
    </message>
    <message>
        <source>Report an issue</source>
        <translation>报告问题</translation>
    </message>
    <message>
        <source>hashcat</source>
        <translation>hashcat</translation>
    </message>
    <message>
        <source>hashcat FAQ</source>
        <translation>hashcat 常见问题</translation>
    </message>
    <message>
        <source>hashcat-gui is licensed under the %1.</source>
        <translation>hashcat-gui 采用 %1 授权。</translation>
    </message>
    <message>
        <source>GNU GPL v3.0 or later</source>
        <translation>GNU GPL v3.0 或更高版本</translation>
    </message>
    <message>
        <source>Not configured</source>
        <translation>未配置</translation>
    </message>
    <message>
        <source>Error: %1</source>
        <translation>错误：%1</translation>
    </message>
    <message>
        <source>A newer version is available: %1.</source>
        <translation>有新版本可用：%1。</translation>
    </message>
    <message>
        <source>Release page</source>
        <translation>发布页面</translation>
    </message>
    <message>
        <source>No updates available.</source>
        <translation>没有可用的更新。</translation>
    </message>
    <message>
        <source>Could not check for updates.</source>
        <translation>无法检查更新。</translation>
    </message>
    <message>
        <source>Could not check for updates: %1</source>
        <translation>无法检查更新：%1</translation>
    </message>
</context>
<context>
    <name>HashcatInfoParser</name>
    <message>
        <source>Invalid JSON returned from hashcat.</source>
        <translation>hashcat 返回了无效的 JSON。</translation>
    </message>
    <message>
        <source>hashcat did not report any hash types.</source>
        <translation>hashcat 未报告任何哈希类型。</translation>
    </message>
</context>
<context>
    <name>HelperUtils</name>
    <message>
        <source>The hashcat path is not configured.</source>
        <translation>尚未配置 hashcat 的路径。</translation>
    </message>
    <message>
        <source>Failed to start hashcat
%1</source>
        <translation>无法启动 hashcat
%1</translation>
    </message>
    <message>
        <source>hashcat timed out
%1</source>
        <translation>hashcat 超时
%1</translation>
    </message>
    <message>
        <source>No supported terminal was found on this system. hashcat-gui needs one to show the hashcat output.</source>
        <translation>此系统上未找到受支持的终端。hashcat-gui 需要终端来显示 hashcat 的输出。</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <source>Hash file or hash:</source>
        <translation>哈希文件或哈希：</translation>
    </message>
    <message>
        <source>Salt is hex</source>
        <translation>盐值为十六进制</translation>
    </message>
    <message>
        <source>Remove hash from hash list once it is cracked</source>
        <translation>破解后从哈希列表中移除该哈希</translation>
    </message>
    <message>
        <source>Move item up</source>
        <translation>上移</translation>
    </message>
    <message>
        <source>Move item down</source>
        <translation>下移</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>移除</translation>
    </message>
    <message>
        <source>Wordlist is hex</source>
        <translation>字典为十六进制</translation>
    </message>
    <message>
        <source>Encoding from:</source>
        <translation>源编码：</translation>
    </message>
    <message>
        <source>Encoding to:</source>
        <translation>目标编码：</translation>
    </message>
    <message>
        <source>Wordlist rules</source>
        <translation>字典规则</translation>
    </message>
    <message>
        <source>Left rule:</source>
        <translation>左侧规则：</translation>
    </message>
    <message>
        <source>Right rule:</source>
        <translation>右侧规则：</translation>
    </message>
    <message>
        <source>Charset is hex</source>
        <translation>字符集为十六进制</translation>
    </message>
    <message>
        <source>Increment mask length</source>
        <translation>递增掩码长度</translation>
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
        <source>Pin hashcat to these CPU devices, separated by commas. Leave empty to let hashcat decide.</source>
        <translation>将 hashcat 绑定到这些 CPU 设备，以逗号分隔。留空则由 hashcat 决定。</translation>
    </message>
    <message>
        <source>Devices to use, separated by commas. Leave empty for all devices.</source>
        <translation>要使用的设备，以逗号分隔。留空则使用所有设备。</translation>
    </message>
    <message>
        <source>Abort above</source>
        <translation>超过以下温度中止</translation>
    </message>
    <message>
        <source>Abort the session when an OpenCL device hotspot exceeds this many degrees Celsius. 0 disables the thermal limit.</source>
        <translation>当 OpenCL 设备热点温度超过此摄氏度数时中止会话。0 表示禁用温度限制。</translation>
    </message>
    <message>
        <source>Outfile</source>
        <translation>输出文件</translation>
    </message>
    <message>
        <source>Built-in variables:

&lt;hash&gt; = current hash filename or hash
&lt;unixtime&gt; = current Unix timestamp</source>
        <translation>内置变量：

&lt;hash&gt; = 当前哈希文件名或哈希
&lt;unixtime&gt; = 当前 Unix 时间戳</translation>
    </message>
    <message>
        <source>Save…</source>
        <translation>保存…</translation>
    </message>
    <message>
        <source>Write the outfile as JSON. The outfile format is ignored.</source>
        <translation>将输出文件写为 JSON。输出文件格式将被忽略。</translation>
    </message>
    <message>
        <source>Write JSON</source>
        <translation>写入 JSON</translation>
    </message>
    <message>
        <source>Do not write recovered hashes to the potfile.</source>
        <translation>不将已恢复的哈希写入 potfile。</translation>
    </message>
    <message>
        <source>Load Example Session</source>
        <translation>加载示例会话</translation>
    </message>
    <message>
        <source>Reset Session</source>
        <translation>重置会话</translation>
    </message>
    <message>
        <source>Mode:</source>
        <translation>模式：</translation>
    </message>
    <message>
        <source>Attack</source>
        <translation>攻击</translation>
    </message>
    <message>
        <source>The hash to crack, a path to a file of hashes, or a .hccapx WPA capture. Paste a single hash here, or point it at a file.</source>
        <translation>要破解的哈希、哈希文件的路径，或 .hccapx WPA 抓包文件。可在此粘贴单个哈希，或指定一个文件。</translation>
    </message>
    <message>
        <source>Open…</source>
        <translation>打开…</translation>
    </message>
    <message>
        <source>Assume the salt is given in hexadecimal.</source>
        <translation>假定盐值以十六进制给出。</translation>
    </message>
    <message>
        <source>Remove already-cracked hashes from the potfile so a later run does not report them again. Refused in attack mode 9.</source>
        <translation>从 potfile 中移除已破解的哈希，使之后的运行不再重复报告。在攻击模式 9 中会被拒绝。</translation>
    </message>
    <message>
        <source>Ignore the @username part of hashes (in hash modes that carry one) so the same word can satisfy several accounts.</source>
        <translation>忽略哈希中的 @username 部分（适用于包含该部分的哈希模式），使同一个单词可以匹配多个账户。</translation>
    </message>
    <message>
        <source>Attack mode. Each choice reveals its own options below.</source>
        <translation>攻击模式。每个选项会在下方显示各自的选项。</translation>
    </message>
    <message>
        <source>Hash type:</source>
        <translation>哈希类型：</translation>
    </message>
    <message>
        <source>Hash recipe:</source>
        <translation>哈希配方：</translation>
    </message>
    <message>
        <source>Unix: &apos;md5(md5(salt) . pass)&apos;
               Windows: &quot;md5(&apos;abc&apos; . pass)&quot;</source>
        <translation>Unix: &apos;md5(md5(salt) . pass)&apos;
               Windows: &quot;md5(&apos;abc&apos; . pass)&quot;</translation>
    </message>
    <message>
        <source>Add files…</source>
        <translation>添加文件…</translation>
    </message>
    <message>
        <source>hashcat reads every file directly inside the folder as one wordlist.</source>
        <translation>hashcat 会将文件夹中直接包含的每个文件作为一个字典读取。</translation>
    </message>
    <message>
        <source>Add folder…</source>
        <translation>添加文件夹…</translation>
    </message>
    <message>
        <source>Only checked rows are used. A folder is one wordlist: hashcat reads the files directly inside it.</source>
        <translation>仅使用已勾选的行。一个文件夹视为一个字典：hashcat 会读取其中直接包含的文件。</translation>
    </message>
    <message>
        <source>Assume words in the wordlist are given in hexadecimal.</source>
        <translation>假定字典中的单词以十六进制给出。</translation>
    </message>
    <message>
        <source>Settings (no wordlist):</source>
        <translation>设置（无字典）：</translation>
    </message>
    <message>
        <source>Positional settings, separated by spaces. Only used when no wordlist is selected: with one, hashcat pairs the two files by line. “phases” picks which phases run, cheapest first; the default “words,rules,pcfg” ends only when stopped. Phase-prefixed settings: “rules.rulefile”, “rules.rulemax”, “words.hints”, “pcfg.hintaccount”, …</source>
        <translation>位置设置，以空格分隔。仅在未选择字典时使用：选择字典后，hashcat 会逐行配对两个文件。“phases”选择要运行的阶段，从开销最小的开始；默认的“words,rules,pcfg”只有在被停止时才会结束。带阶段前缀的设置：“rules.rulefile”、“rules.rulemax”、“words.hints”、“pcfg.hintaccount”、…</translation>
    </message>
    <message>
        <source>PCFG</source>
        <translation>PCFG</translation>
    </message>
    <message>
        <source>A row is a ruleset file, a folder with a Grammar/grammar.txt, or a bundled name. Only checked rows are used; with none checked, hashcat’s bundled default-passwords runs. hints is also bundled.</source>
        <translation>一行可以是规则集文件、包含 Grammar/grammar.txt 的文件夹，或内置名称。仅使用已勾选的行；若没有勾选任何行，则运行 hashcat 内置的 default-passwords。hints 也是内置的。</translation>
    </message>
    <message>
        <source>A ruleset folder needs a Grammar/grammar.txt.</source>
        <translation>规则集文件夹需要包含 Grammar/grammar.txt。</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Scale:</translation>
    </message>
    <message>
        <source>Quantization steps per bit (1–64)</source>
        <translation>每比特的量化步数 (1–64)</translation>
    </message>
    <message>
        <source>Costmax:</source>
        <translation>Costmax:</translation>
    </message>
    <message>
        <source>Highest cost level to enumerate (1–64)</source>
        <translation>要枚举的最高开销级别 (1–64)</translation>
    </message>
    <message>
        <source>Weights:</source>
        <translation>Weights:</translation>
    </message>
    <message>
        <source>One value per ruleset, plain numbers separated by colons, e.g. 2:1 for two rulesets. The count must match the number of rulesets.</source>
        <translation>每个规则集一个值，为以冒号分隔的纯数字，例如两个规则集写作 2:1。值的数量必须与规则集的数量一致。</translation>
    </message>
    <message>
        <source>Threads:</source>
        <translation>线程：</translation>
    </message>
    <message>
        <source>Cores that generate base words (0–16); 0 runs on the calling thread</source>
        <translation>生成基础单词的核心数 (0–16)；0 表示在调用线程中运行</translation>
    </message>
    <message>
        <source>auto</source>
        <translation>自动</translation>
    </message>
    <message>
        <source>Kbits:</source>
        <translation>Kbits:</translation>
    </message>
    <message>
        <source>Bits one inner loop may span (0–27)</source>
        <translation>一个内层循环可跨越的位数 (0–27)</translation>
    </message>
    <message>
        <source>Maxword:</source>
        <translation>Maxword:</translation>
    </message>
    <message>
        <source>Candidates the kernel builds per base word: 0, 16 or 32 (a multiple of 16)</source>
        <translation>内核为每个基础单词构建的候选数：0、16 或 32（16 的倍数）</translation>
    </message>
    <message>
        <source>Maxgain:</source>
        <translation>Maxgain:</translation>
    </message>
    <message>
        <source>1.0–64.0; decimal point is a dot, not the locale separator</source>
        <translation>1.0–64.0；小数点为英文句点，而非区域设置中的分隔符</translation>
    </message>
    <message>
        <source>Walk:</source>
        <translation>Walk:</translation>
    </message>
    <message>
        <source>1 steps the last token instead of unraking it</source>
        <translation>1 表示对最后一个 token 逐步前进，而不是对其执行“unraking”</translation>
    </message>
    <message>
        <source>Mask:</source>
        <translation>掩码：</translation>
    </message>
    <message>
        <source>Hold the grammar to the candidates this mask admits; cannot be combined with omen=1, or with hintaccount while the hint words come from the accounts (give hintwords or hintfile instead)</source>
        <translation>将语法限制在此掩码允许的候选范围内；不能与 omen=1 同时使用，且在提示词来自账户时也不能与 hintaccount 同时使用（请改用 hintwords 或 hintfile）</translation>
    </message>
    <message>
        <source>Extra settings (key=value):</source>
        <translation>额外设置 (key=value)：</translation>
    </message>
    <message>
        <source>Additional key=value settings, separated by spaces: omen, cache, lookup, pwmin, pwmax, hintaccount, hintwords, hintfile, hintrank, hintrepeat. Everything after the first key=value is a setting, so rulesets must come first.</source>
        <translation>以空格分隔的额外 key=value 设置：omen、cache、lookup、pwmin、pwmax、hintaccount、hintwords、hintfile、hintrank、hintrepeat。第一个 key=value 之后的所有内容都视为设置，因此规则集必须放在前面。</translation>
    </message>
    <message>
        <source>Table</source>
        <translation>表</translation>
    </message>
    <message>
        <source>Every checked row after the wordlist is a table</source>
        <translation>字典之后每个已勾选的行都是一个表</translation>
    </message>
    <message>
        <source>Add tables…</source>
        <translation>添加表…</translation>
    </message>
    <message>
        <source>Identity:</source>
        <translation>Identity:</translation>
    </message>
    <message>
        <source>Whether leaving a token unchanged is one of its choices: 1 (default) keeps it, 0 drops it so the table only produces variations. Cannot be combined with single=1.</source>
        <translation>是否将保持 token 不变作为其选择之一：1（默认）保留，0 丢弃，使表只产生变体。不能与 single=1 同时使用。</translation>
    </message>
    <message>
        <source>Template:</source>
        <translation>Template:</translation>
    </message>
    <message>
        <source>1 reads the wordlist as a set of patterns: no word is ever returned unchanged, and words matching no rule are dropped. 0 (default) keeps unchanged words.</source>
        <translation>1 将字典读作一组模式：任何单词都不会原样返回，且不匹配任何规则的单词会被丢弃。0（默认）保留未更改的单词。</translation>
    </message>
    <message>
        <source>Single:</source>
        <translation>Single:</translation>
    </message>
    <message>
        <source>single=1 makes each candidate carry exactly one substitution instead of the cross product of every matched position, usually a different, cheaper attack.</source>
        <translation>single=1 使每个候选恰好带有一处替换，而不是所有匹配位置的笛卡尔积；通常是一种不同且开销更小的攻击。</translation>
    </message>
    <message>
        <source>Cap:</source>
        <translation>Cap:</translation>
    </message>
    <message>
        <source>Maximum replacements one source may carry in a word; 0 means no limit. Only limits the host-side cross product, not a single=1 run.</source>
        <translation>一个来源在单词中最多可带有的替换数；0 表示不限制。仅限制主机端的笛卡尔积，不限制 single=1 的运行。</translation>
    </message>
    <message>
        <source>Maxperm:</source>
        <translation>Maxperm:</translation>
    </message>
    <message>
        <source>How many of one word’s cross product the host enumerates before giving up; 0 means no limit. Each device cell is never cut.</source>
        <translation>主机在放弃之前枚举一个单词笛卡尔积中的多少项；0 表示不限制。设备单元从不会被截断。</translation>
    </message>
    <message>
        <source>Additional key=value settings, separated by spaces. Everything after the first key=value is a setting, so the tables must come first.</source>
        <translation>以空格分隔的额外 key=value 设置。第一个 key=value 之后的所有内容都视为设置，因此表必须放在前面。</translation>
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
        <translation>候选生成器。包含斜杠的内容会被视为 feed 路径；否则 hashcat 会在其 feeds 目录下查找 feed_&lt;name&gt;。下方的参数取决于具体的 feed。</translation>
    </message>
    <message>
        <source>wordlist, stdin, random, association, pcfg, table, hybrid, mask, or a feed path</source>
        <translation>wordlist、stdin、random、association、pcfg、table、hybrid、mask，或 feed 路径</translation>
    </message>
    <message>
        <source>Browse…</source>
        <translation>浏览…</translation>
    </message>
    <message>
        <source>Argument:</source>
        <translation>参数：</translation>
    </message>
    <message>
        <source>Argument passed to the feed. Its meaning and count depend on the feed (the wordlist feed takes wordlist paths); extra tokens go straight to the feed.</source>
        <translation>传递给 feed 的参数。其含义和数量取决于 feed（wordlist feed 接受字典路径）；多余的 token 会直接交给 feed。</translation>
    </message>
    <message>
        <source>Add</source>
        <translation>添加</translation>
    </message>
    <message>
        <source>Arguments the feed reads, in order; only checked rows are used</source>
        <translation>feed 按顺序读取的参数；仅使用已勾选的行</translation>
    </message>
    <message>
        <source>Rules</source>
        <translation>规则</translation>
    </message>
    <message>
        <source>Use rules files:</source>
        <translation>使用规则文件：</translation>
    </message>
    <message>
        <source>Enable rules file slot 1 for this run.</source>
        <translation>为本次运行启用规则文件槽位 1。</translation>
    </message>
    <message>
        <source>Rules file:</source>
        <translation>规则文件：</translation>
    </message>
    <message>
        <source>Rule file for slot 1; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>槽位 1 的规则文件；应用于每个单词。留空则不使用该槽位。</translation>
    </message>
    <message>
        <source>Enable rules file slot 2 for this run.</source>
        <translation>为本次运行启用规则文件槽位 2。</translation>
    </message>
    <message>
        <source>Rule file for slot 2; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>槽位 2 的规则文件；应用于每个单词。留空则不使用该槽位。</translation>
    </message>
    <message>
        <source>Enable rules file slot 3 for this run.</source>
        <translation>为本次运行启用规则文件槽位 3。</translation>
    </message>
    <message>
        <source>Rule file for slot 3; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>槽位 3 的规则文件；应用于每个单词。留空则不使用该槽位。</translation>
    </message>
    <message>
        <source>Generate the rules automatically instead of reading them from a file.</source>
        <translation>自动生成规则，而不是从文件中读取。</translation>
    </message>
    <message>
        <source>Generate rules:</source>
        <translation>生成规则：</translation>
    </message>
    <message>
        <source>How many rules to generate for the finding-rule attack. No rules file is read, hashcat builds them itself; use an existing rule file with the slots above instead.</source>
        <translation>为规则查找攻击生成多少条规则。不会读取规则文件，hashcat 会自行构建规则；若要使用现有规则文件，请改用上方的槽位。</translation>
    </message>
    <message>
        <source>Pass every rules file to hashcat in one run, instead of one run per file.</source>
        <translation>将所有规则文件在一次运行中传给 hashcat，而不是每个文件运行一次。</translation>
    </message>
    <message>
        <source>Join -r files into one list</source>
        <translation>将 -r 文件合并为一个列表</translation>
    </message>
    <message>
        <source>Single rule applied to each word from the left wordlist.</source>
        <translation>应用于左侧字典中每个单词的单条规则。</translation>
    </message>
    <message>
        <source>Single rule applied to each word from the right wordlist.</source>
        <translation>应用于右侧字典中每个单词的单条规则。</translation>
    </message>
    <message>
        <source>Mask</source>
        <translation>掩码</translation>
    </message>
    <message>
        <source>Built-in charsets:</source>
        <translation>内置字符集：</translation>
    </message>
    <message>
        <source>Custom charsets</source>
        <translation>自定义字符集</translation>
    </message>
    <message>
        <source>Charset 1:</source>
        <translation>字符集 1：</translation>
    </message>
    <message>
        <source>Charset 2:</source>
        <translation>字符集 2：</translation>
    </message>
    <message>
        <source>Charset 3:</source>
        <translation>字符集 3：</translation>
    </message>
    <message>
        <source>Charset 4:</source>
        <translation>字符集 4：</translation>
    </message>
    <message>
        <source>Interpret the custom charsets as hexadecimal bytes. Applies to custom charset 1 through 4.</source>
        <translation>将自定义字符集解释为十六进制字节。适用于自定义字符集 1 至 4。</translation>
    </message>
    <message>
        <source>Session and limits</source>
        <translation>会话和限制</translation>
    </message>
    <message>
        <source>Session name:</source>
        <translation>会话名称：</translation>
    </message>
    <message>
        <source>Name hashcat uses for its restore file and status output.</source>
        <translation>hashcat 用于其恢复文件和状态输出的名称。</translation>
    </message>
    <message>
        <source>Runtime limit:</source>
        <translation>运行时间限制：</translation>
    </message>
    <message>
        <source>unlimited</source>
        <translation>无限制</translation>
    </message>
    <message>
        <source>Skip:</source>
        <translation>跳过：</translation>
    </message>
    <message>
        <source>Limit:</source>
        <translation>限制：</translation>
    </message>
    <message>
        <source>Keep updating the run status (speed, progress, ETA) instead of printing one final line.</source>
        <translation>持续更新运行状态（速度、进度、预计剩余时间），而不是只打印最后一行。</translation>
    </message>
    <message>
        <source>Update status automatically</source>
        <translation>自动更新状态</translation>
    </message>
    <message>
        <source>Ignore warnings and start hashcat anyway. Use with care.</source>
        <translation>忽略警告并强制启动 hashcat。请谨慎使用。</translation>
    </message>
    <message>
        <source>Force (ignore warnings)</source>
        <translation>强制（忽略警告）</translation>
    </message>
    <message>
        <source>Performance</source>
        <translation>性能</translation>
    </message>
    <message>
        <source>Print only the attack’s speed and quit, without cracking. Useful for benchmarking and comparing settings.</source>
        <translation>只输出攻击速度然后退出，不进行破解。适用于基准测试和比较设置。</translation>
    </message>
    <message>
        <source>Cache path:</source>
        <translation>缓存路径：</translation>
    </message>
    <message>
        <source>Directory hashcat keeps its cache in. Must exist.</source>
        <translation>hashcat 保存其缓存的目录。必须存在。</translation>
    </message>
    <message>
        <source>Resources</source>
        <translation>资源</translation>
    </message>
    <message>
        <source>CPU affinity:</source>
        <translation>CPU 亲和性：</translation>
    </message>
    <message>
        <source>Backend devices:</source>
        <translation>后端设备：</translation>
    </message>
    <message>
        <source>Device types:</source>
        <translation>设备类型：</translation>
    </message>
    <message>
        <source>Write recovered hashes to file:</source>
        <translation>将已恢复的哈希写入文件：</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>格式：</translation>
    </message>
    <message>
        <source>Do not write the potfile</source>
        <translation>不写入 potfile</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>开始</translation>
    </message>
    <message>
        <source>Copy command to clipboard</source>
        <translation>将命令复制到剪贴板</translation>
    </message>
    <message>
        <source>General</source>
        <translation>常规</translation>
    </message>
    <message>
        <source>Ignore username in hash file</source>
        <translation>忽略哈希文件中的用户名</translation>
    </message>
    <message>
        <source>Wordlists</source>
        <translation>字典</translation>
    </message>
    <message>
        <source>Increment the mask length from the minimum to the maximum.</source>
        <translation>将掩码长度从最小值递增到最大值。</translation>
    </message>
    <message>
        <source>Minimum mask length; 0 uses hashcat’s default.</source>
        <translation>掩码的最小长度；0 表示使用 hashcat 的默认值。</translation>
    </message>
    <message>
        <source>Maximum mask length; 0 uses hashcat’s default.</source>
        <translation>掩码的最大长度；0 表示使用 hashcat 的默认值。</translation>
    </message>
    <message>
        <source>Abort the session after this many seconds; 0 runs until hashcat finishes.</source>
        <translation>在此秒数后中止会话；0 表示一直运行到 hashcat 结束。</translation>
    </message>
    <message>
        <source>&#xa0;s</source>
        <translation>&#xa0;秒</translation>
    </message>
    <message>
        <source>Skip this many candidates from the start.</source>
        <translation>从开头跳过这么多个候选。</translation>
    </message>
    <message>
        <source>Only test this many candidates after the skipped ones.</source>
        <translation>在跳过的候选之后，仅测试这么多个候选。</translation>
    </message>
    <message>
        <source>Seconds between status updates.</source>
        <translation>状态更新之间的间隔秒数。</translation>
    </message>
    <message>
        <source>Enable optimized kernels (limits password length)</source>
        <translation>启用优化内核（会限制密码长度）</translation>
    </message>
    <message>
        <source>Return expected speed of the attack, then quit</source>
        <translation>输出攻击的预期速度，然后退出</translation>
    </message>
    <message>
        <source>Abort when a device reaches this temperature.</source>
        <translation>当设备达到此温度时中止。</translation>
    </message>
    <message>
        <source>&#xa0;°C</source>
        <translation>&#xa0;°C</translation>
    </message>
    <message>
        <source>Settings…</source>
        <translation>设置…</translation>
    </message>
    <message>
        <source>Import Settings…</source>
        <translation>导入设置…</translation>
    </message>
    <message>
        <source>Export Settings…</source>
        <translation>导出设置…</translation>
    </message>
    <message>
        <source>Export Session…</source>
        <translation>导出会话…</translation>
    </message>
    <message>
        <source>Import Session…</source>
        <translation>导入会话…</translation>
    </message>
    <message>
        <source>Help</source>
        <translation>帮助</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>工具</translation>
    </message>
    <message>
        <source>File</source>
        <translation>文件</translation>
    </message>
    <message>
        <source>About</source>
        <translation>关于</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation>退出</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>关于 Qt</translation>
    </message>
    <message>
        <source>JSON Files (*.json)</source>
        <translation>JSON 文件 (*.json)</translation>
    </message>
    <message>
        <source>The session file could not be read, so the session was not restored. The file was left in place.

%1</source>
        <translation>无法读取会话文件，因此未恢复会话。文件已保持原样。

%1</translation>
    </message>
    <message>
        <source>Save failed</source>
        <translation>保存失败</translation>
    </message>
    <message>
        <source>Outfile format to use, separated by commas.
Default: 1,2</source>
        <translation>要使用的输出文件格式，以逗号分隔。
默认值：1,2</translation>
    </message>
    <message>
        <source>Common encodings:</source>
        <translation>常用编码：</translation>
    </message>
    <message>
        <source>Force the internal wordlist encoding from this code. Leave empty to let hashcat decide.</source>
        <translation>强制内部字典编码的源编码为此代码。留空则由 hashcat 决定。</translation>
    </message>
    <message>
        <source>Force the internal wordlist encoding to this code. Leave empty to let hashcat decide.</source>
        <translation>强制内部字典编码的目标编码为此代码。留空则由 hashcat 决定。</translation>
    </message>
    <message>
        <source>Exported</source>
        <translation>已导出</translation>
    </message>
    <message>
        <source>Export failed</source>
        <translation>导出失败</translation>
    </message>
    <message>
        <source>Import failed</source>
        <translation>导入失败</translation>
    </message>
    <message>
        <source>Imported</source>
        <translation>已导入</translation>
    </message>
    <message>
        <source>The file was made by an older version of hashcat-gui (format version %1; this version reads format version %2).</source>
        <translation>该文件由旧版本的 hashcat-gui 创建（格式版本 %1；此版本读取格式版本 %2）。</translation>
    </message>
    <message>
        <source>The file holds sections this import does not use: %1.</source>
        <translation>该文件包含此导入未使用的部分：%1。</translation>
    </message>
    <message>
        <source>The file is in format version %1, but this version reads format version %2, so it was not applied. Update hashcat-gui to read it.</source>
        <translation>该文件的格式版本为 %1，但此版本读取的是格式版本 %2，因此未应用。请更新 hashcat-gui 以读取该文件。</translation>
    </message>
    <message>
        <source>The session file is in format version %1, but this version reads format version %2, so the session was not restored. The file was left as it is, so the session of this run is not stored either. Update hashcat-gui to read it.</source>
        <translation>会话文件的格式版本为 %1，但此版本读取的是格式版本 %2，因此未恢复会话。文件已保持原样，因此本次运行的会话也不会被保存。请更新 hashcat-gui 以读取该文件。</translation>
    </message>
    <message>
        <source>Settings exported to %1.</source>
        <translation>设置已导出到 %1。</translation>
    </message>
    <message>
        <source>OpenCL device types to use, separated by commas.</source>
        <translation>要使用的 OpenCL 设备类型，以逗号分隔。</translation>
    </message>
    <message>
        <source>Export session</source>
        <translation>导出会话</translation>
    </message>
    <message>
        <source>Session exported to “%1”.</source>
        <translation>会话已导出到“%1”。</translation>
    </message>
    <message>
        <source>Import session</source>
        <translation>导入会话</translation>
    </message>
    <message>
        <source>Session imported from “%1”.</source>
        <translation>已从“%1”导入会话。</translation>
    </message>
    <message>
        <source>Session imported from “%1”.

%2</source>
        <translation>已从“%1”导入会话。

%2</translation>
    </message>
    <message>
        <source>Export settings</source>
        <translation>导出设置</translation>
    </message>
    <message>
        <source>Import settings</source>
        <translation>导入设置</translation>
    </message>
    <message>
        <source>Settings loaded from “%1”.</source>
        <translation>已从“%1”加载设置。</translation>
    </message>
    <message>
        <source>Review them and save to apply them.</source>
        <translation>请检查设置并保存以应用。</translation>
    </message>
    <message>
        <source>Load example session</source>
        <translation>加载示例会话</translation>
    </message>
    <message>
        <source>Reset session</source>
        <translation>重置会话</translation>
    </message>
    <message>
        <source>Reset every field to its default? The current session is lost.</source>
        <translation>要将所有字段重置为默认值吗？当前会话将丢失。</translation>
    </message>
    <message>
        <source>Session warning</source>
        <translation>会话警告</translation>
    </message>
    <message>
        <source>Updating…</source>
        <translation>正在更新…</translation>
    </message>
    <message>
        <source>Failed to load hash types</source>
        <translation>无法加载哈希类型</translation>
    </message>
    <message>
        <source>This mode needs a wordlist and a mask.</source>
        <translation>此模式需要一个字典和一个掩码。</translation>
    </message>
    <message>
        <source>This mode needs at least one wordlist.</source>
        <translation>此模式需要至少一个字典。</translation>
    </message>
    <message>
        <source>Open hash file</source>
        <translation>打开哈希文件</translation>
    </message>
    <message>
        <source>Save output file</source>
        <translation>保存输出文件</translation>
    </message>
    <message>
        <source>Add wordlists</source>
        <translation>添加字典</translation>
    </message>
    <message>
        <source>Open rules file</source>
        <translation>打开规则文件</translation>
    </message>
    <message>
        <source>Could not start “%1”: %2</source>
        <translation>无法启动“%1”：%2</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not available. Available terminals: %2
Navigate to %3 → %4 to change it.</source>
        <translation>已配置的终端“%1”不可用。可用的终端：%2
请前往 %3 → %4 进行修改。</translation>
    </message>
    <message>
        <source>This mode needs exactly %1 wordlists (left, right).
Currently selected: %2.</source>
        <translation>此模式需要恰好 %1 个字典（左、右）。
当前已选择：%2 个。</translation>
    </message>
    <message>
        <source>The session names the hash type %1, which this hashcat version does not support.</source>
        <translation>会话指定了哈希类型 %1，此版本的 hashcat 不支持该类型。</translation>
    </message>
    <message>
        <source>Load the %1 example? The current session is lost.

The example cracks a test hash; hashcat should recover the password %2.</source>
        <translation>要加载 %1 示例吗？当前会话将丢失。

该示例会破解一个测试哈希；hashcat 应能恢复出密码 %2。</translation>
    </message>
    <message>
        <source>Could not create the example directory “%1”.</source>
        <translation>无法创建示例目录“%1”。</translation>
    </message>
    <message>
        <source>Could not read the bundled file “%1”: %2</source>
        <translation>无法读取内置文件“%1”：%2</translation>
    </message>
    <message>
        <source>Could not write “%1”: %2</source>
        <translation>无法写入“%1”：%2</translation>
    </message>
    <message>
        <source>Straight</source>
        <translation>字典</translation>
    </message>
    <message>
        <source>Combination</source>
        <translation>组合</translation>
    </message>
    <message>
        <source>Brute-force</source>
        <translation>暴力破解</translation>
    </message>
    <message>
        <source>Hybrid Wordlist + Mask</source>
        <translation>混合：字典 + 掩码</translation>
    </message>
    <message>
        <source>Hybrid Mask + Wordlist</source>
        <translation>混合：掩码 + 字典</translation>
    </message>
    <message>
        <source>Generic (feed)</source>
        <translation>通用（feed）</translation>
    </message>
    <message>
        <source>Association</source>
        <translation>关联</translation>
    </message>
    <message>
        <source>Hybrid (mask + ?w/?q)</source>
        <translation>混合（掩码 + ?w/?q）</translation>
    </message>
    <message>
        <source>Failed to obtain supported hash types.
Error: %1</source>
        <translation>无法获取支持的哈希类型。
错误：%1</translation>
    </message>
    <message>
        <source>The hash types could not be loaded. Check the hashcat path in Settings.</source>
        <translation>无法加载哈希类型。请在设置中检查 hashcat 路径。</translation>
    </message>
    <message>
        <source>Set the hashcat path under %1 → %2 to load the available hash types.</source>
        <translation>请在 %1 → %2 中设置 hashcat 路径，以加载可用的哈希类型。</translation>
    </message>
    <message>
        <source>This mode needs exactly 2 wordlists (left, right).</source>
        <translation>此模式需要恰好 2 个字典（左、右）。</translation>
    </message>
    <message>
        <source>This mode needs a mask.</source>
        <translation>此模式需要一个掩码。</translation>
    </message>
    <message>
        <source>This mode runs hashcat’s bundled default-passwords ruleset; name one or more to use your own.</source>
        <translation>此模式会运行 hashcat 内置的 default-passwords 规则集；请指定一个或多个规则集以使用您自己的。</translation>
    </message>
    <message>
        <source>This mode needs one wordlist and at least one table.</source>
        <translation>此模式需要一个字典和至少一个表。</translation>
    </message>
    <message>
        <source>This mode needs a feed. Arguments depend on the feed — random and stdin need none, wordlist needs a path.</source>
        <translation>此模式需要一个 feed。参数取决于 feed：random 和 stdin 不需要参数，wordlist 需要一个路径。</translation>
    </message>
    <message>
        <source>This mode needs at most one wordlist. Without one the words come from the hash file and the settings line can bound the run; with one the two files are paired by line.</source>
        <translation>此模式最多需要一个字典。没有字典时，单词来自哈希文件，设置行可以限定运行范围；有字典时，两个文件将逐行配对。</translation>
    </message>
    <message>
        <source>This mode needs a mask with exactly one ?w; a ?q after ?w names a second wordlist.</source>
        <translation>此模式需要恰好包含一个 ?w 的掩码；?w 之后的 ?q 表示第二个字典。</translation>
    </message>
    <message>
        <source>Open cache directory</source>
        <translation>打开缓存目录</translation>
    </message>
    <message>
        <source>Add wordlist folder</source>
        <translation>添加字典文件夹</translation>
    </message>
    <message>
        <source>Add rulesets</source>
        <translation>添加规则集</translation>
    </message>
    <message>
        <source>Add ruleset folder</source>
        <translation>添加规则集文件夹</translation>
    </message>
    <message>
        <source>Add tables</source>
        <translation>添加表</translation>
    </message>
    <message>
        <source>Open feed file</source>
        <translation>打开 feed 文件</translation>
    </message>
    <message>
        <source>This folder has no wordlists. hashcat reads the regular files directly inside it, not subfolders.</source>
        <translation>此文件夹中没有字典。hashcat 读取其中直接包含的普通文件，不读取子文件夹。</translation>
    </message>
    <message>
        <source>This wordlist does not exist:
%1</source>
        <translation>此字典不存在：
%1</translation>
    </message>
    <message>
        <source>This ruleset folder has no Grammar/grammar.txt:
%1</source>
        <translation>此规则集文件夹中没有 Grammar/grammar.txt：
%1</translation>
    </message>
    <message>
        <source>This ruleset does not exist:
%1</source>
        <translation>此规则集不存在：
%1</translation>
    </message>
    <message>
        <source>A table must be a file; hashcat does not read a folder:
%1</source>
        <translation>表必须是文件；hashcat 不读取文件夹：
%1</translation>
    </message>
    <message>
        <source>This table does not exist:
%1</source>
        <translation>此表不存在：
%1</translation>
    </message>
    <message>
        <source>Copied to the clipboard</source>
        <translation>已复制到剪贴板</translation>
    </message>
    <message>
        <source>Launch failed</source>
        <translation>启动失败</translation>
    </message>
    <message>
        <source>Please choose a hash file.</source>
        <translation>请选择一个哈希文件。</translation>
    </message>
    <message>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to configure the path to the hashcat executable.</source>
        <translation>请前往 &lt;b&gt;%1 → %2&lt;/b&gt; 配置 hashcat 可执行文件的路径。</translation>
    </message>
    <message>
        <source>The configured hashcat executable does not exist:
%1
Navigate to %2 → %3 to change it.</source>
        <translation>配置的 hashcat 可执行文件不存在：
%1
请前往 %2 → %3 进行修改。</translation>
    </message>
    <message>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to select the terminal used for launching.</source>
        <translation>请前往 &lt;b&gt;%1 → %2&lt;/b&gt; 选择用于启动的终端。</translation>
    </message>
    <message>
        <source>This mode takes at most %1 wordlists.
Currently selected: %2.</source>
        <translation>此模式最多接受 %1 个字典。
当前已选择：%2 个。</translation>
    </message>
    <message>
        <source>hashcat pairs the two files by line here; this GUI offers only one wordlist for that role.</source>
        <translation>在此处 hashcat 会逐行配对两个文件；此界面只为该角色提供一个字典。</translation>
    </message>
    <message>
        <source>This wordlist has to be a file: hashcat reads it as the multiplied wordlist (the ?q side), and a folder cannot be used there:
%1</source>
        <translation>此字典必须是文件：hashcat 将其作为被乘的字典（?q 一侧）读取，此处不能使用文件夹：
%1</translation>
    </message>
    <message>
        <source>This mode needs at least one table.</source>
        <translation>此模式需要至少一个表。</translation>
    </message>
    <message>
        <source>This mode needs a feed: a bundled name (wordlist, stdin, random, …) or the path of a feed file.</source>
        <translation>此模式需要一个 feed：内置名称（wordlist、stdin、random、…）或 feed 文件的路径。</translation>
    </message>
    <message>
        <source>This mode pairs the wordlists with the mask; the mask needs a ?w for the word.</source>
        <translation>此模式将字典与掩码配对；掩码需要一个 ?w 来代表单词。</translation>
    </message>
    <message>
        <source>The mask contains more than one ?w.</source>
        <translation>掩码包含多个 ?w。</translation>
    </message>
    <message>
        <source>The mask contains a ?q before the first ?w.</source>
        <translation>掩码在第一个 ?w 之前包含 ?q。</translation>
    </message>
    <message>
        <source>The mask contains more than one ?q.</source>
        <translation>掩码包含多个 ?q。</translation>
    </message>
    <message>
        <source>The mask names a second word with ?q, but only one wordlist is selected.</source>
        <translation>掩码用 ?q 指定了第二个单词，但只选择了一个字典。</translation>
    </message>
    <message>
        <source>Two wordlists are selected, but the mask has no ?q for the second one.</source>
        <translation>已选择两个字典，但掩码中没有用于第二个字典的 ?q。</translation>
    </message>
    <message>
        <source>The increment is not offered in this hybrid mode.</source>
        <translation>此混合模式不提供递增功能。</translation>
    </message>
    <message>
        <source>The ?w and ?q markers are only offered in the hybrid mode (mask + wordlists).</source>
        <translation>?w 和 ?q 标记仅在混合模式（掩码 + 字典）中提供。</translation>
    </message>
    <message>
        <source>The setting “%1” is not valid for this mode.</source>
        <translation>设置“%1”对此模式无效。</translation>
    </message>
    <message>
        <source>The token “%1” is not a setting; settings are written as key=value.</source>
        <translation>token“%1”不是设置；设置应写成 key=value 的形式。</translation>
    </message>
    <message>
        <source>The settings line applies only while the words come from the hash file. Clear either the settings line or the wordlist list.</source>
        <translation>设置行仅在单词来自哈希文件时才适用。请清除设置行或字典列表之一。</translation>
    </message>
    <message>
        <source>The hash type 4000 describes its hashes as a recipe, for example
md5(md5(salt) . pass). Enter it in the hash recipe field.</source>
        <translation>哈希类型 4000 以配方的形式描述其哈希，例如
md5(md5(salt) . pass)。请在哈希配方字段中输入。</translation>
    </message>
    <message>
        <source>The minimum increment length is larger than the maximum.</source>
        <translation>递增的最小长度大于最大长度。</translation>
    </message>
    <message>
        <source>The cache path must be an existing directory:
%1</source>
        <translation>缓存路径必须是已存在的目录：
%1</translation>
    </message>
    <message>
        <source>The increment cannot be combined with the speed-only option.</source>
        <translation>递增不能与仅测速选项同时使用。</translation>
    </message>
    <message>
        <source>Removing cracked hashes is not possible in the association attack mode.</source>
        <translation>在“关联”攻击模式下无法移除已破解的哈希。</translation>
    </message>
</context>
<context>
    <name>SettingsDialog</name>
    <message>
        <source>Settings</source>
        <translation>设置</translation>
    </message>
    <message>
        <source>Not configured</source>
        <translation>未配置</translation>
    </message>
    <message>
        <source>hashcat</source>
        <translation>hashcat</translation>
    </message>
    <message>
        <source>&amp;hashcat executable</source>
        <translation>hashcat 可执行文件(&amp;H)</translation>
    </message>
    <message>
        <source>Select…</source>
        <translation>选择…</translation>
    </message>
    <message>
        <source>&amp;Terminal</source>
        <translation>终端(&amp;T)</translation>
    </message>
    <message>
        <source>&amp;Language</source>
        <translation>语言(&amp;L)</translation>
    </message>
    <message>
        <source>&amp;Color scheme</source>
        <translation>配色方案(&amp;C)</translation>
    </message>
    <message>
        <source>Not all color schemes are available on all platforms.</source>
        <translation>并非所有配色方案在所有平台上都可用。</translation>
    </message>
    <message>
        <source>&amp;Use short parameters</source>
        <translation>使用短参数(&amp;U)</translation>
    </message>
    <message>
        <source>Session and files</source>
        <translation>会话和文件</translation>
    </message>
    <message>
        <source>&amp;Restore the last session at startup</source>
        <translation>启动时恢复上次的会话(&amp;R)</translation>
    </message>
    <message>
        <source>&amp;Session file</source>
        <translation>会话文件(&amp;S)</translation>
    </message>
    <message>
        <source>The file the last session is stored in</source>
        <translation>保存上次会话的文件</translation>
    </message>
    <message>
        <source>Open folder</source>
        <translation>打开文件夹</translation>
    </message>
    <message>
        <source>Open the folder the session is stored in</source>
        <translation>打开保存会话的文件夹</translation>
    </message>
    <message>
        <source>Settings &amp;file</source>
        <translation>设置文件(&amp;F)</translation>
    </message>
    <message>
        <source>The file the settings are stored in</source>
        <translation>保存设置的文件</translation>
    </message>
    <message>
        <source>Open the folder the settings are stored in</source>
        <translation>打开保存设置的文件夹</translation>
    </message>
    <message>
        <source>General</source>
        <translation>常规</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>保存</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <source>Download from %1</source>
        <translation>从 %1 下载</translation>
    </message>
    <message>
        <source>Use the short option names (-m) instead of the long ones (--hash-type) in the command.</source>
        <translation>在命令中使用短选项名（-m）代替长选项名（--hash-type）。</translation>
    </message>
    <message>
        <source>Select a terminal</source>
        <translation>选择终端</translation>
    </message>
    <message>
        <source>System default</source>
        <translation>系统默认</translation>
    </message>
    <message>
        <source>Light</source>
        <translation>浅色</translation>
    </message>
    <message>
        <source>Dark</source>
        <translation>深色</translation>
    </message>
    <message>
        <source>The hashcat path “%1” was not found on this computer.</source>
        <translation>在此计算机上未找到 hashcat 路径“%1”。</translation>
    </message>
    <message>
        <source>The terminal “%1” is not installed and was not applied.</source>
        <translation>终端“%1”未安装，未被应用。</translation>
    </message>
    <message>
        <source>The color scheme “%1” is not offered and was not applied.</source>
        <translation>未提供配色方案“%1”，未被应用。</translation>
    </message>
    <message>
        <source>The language “%1” is not offered and was not applied.</source>
        <translation>未提供语言“%1”，未被应用。</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not installed.</source>
        <translation>已配置的终端“%1”未安装。</translation>
    </message>
    <message>
        <source>Executable Files (*.exe *.bin hashcat);;All Files (*)</source>
        <translation>可执行文件 (*.exe *.bin hashcat);;所有文件 (*)</translation>
    </message>
    <message>
        <source>No hashcat executable is configured; hashcat cannot be started.</source>
        <translation>未配置 hashcat 可执行文件，无法启动 hashcat。</translation>
    </message>
    <message>
        <source>The configured hashcat executable was not found on this computer.</source>
        <translation>在此计算机上未找到已配置的 hashcat 可执行文件。</translation>
    </message>
    <message>
        <source>Select the terminal hashcat is started in.</source>
        <translation>选择用于启动 hashcat 的终端。</translation>
    </message>
    <message>
        <source>Select the hashcat executable</source>
        <translation>选择 hashcat 可执行文件</translation>
    </message>
    <message>
        <source>Invalid file</source>
        <translation>无效文件</translation>
    </message>
    <message>
        <source>The selected file is not an executable.</source>
        <translation>所选文件不是可执行文件。</translation>
    </message>
    <message>
        <source>Restart required</source>
        <translation>需要重启</translation>
    </message>
    <message>
        <source>Restart hashcat-gui to apply the new language.</source>
        <translation>请重启 hashcat-gui 以应用新语言。</translation>
    </message>
</context>
<context>
    <name>StateFile</name>
    <message>
        <source>The file is not valid JSON: %1</source>
        <translation>该文件不是有效的 JSON：%1</translation>
    </message>
    <message>
        <source>The file does not contain a JSON object.</source>
        <translation>该文件不包含 JSON 对象。</translation>
    </message>
    <message>
        <source>The file does not contain any sections.</source>
        <translation>该文件不包含任何部分。</translation>
    </message>
    <message>
        <source>Could not open “%1” for writing: %2</source>
        <translation>无法打开“%1”进行写入：%2</translation>
    </message>
    <message>
        <source>Could not write to “%1”: %2</source>
        <translation>无法写入“%1”：%2</translation>
    </message>
    <message>
        <source>Could not open “%1” for reading: %2</source>
        <translation>无法打开“%1”进行读取：%2</translation>
    </message>
    <message>
        <source>The file does not contain a “%1” section.</source>
        <translation>文件中不包含“%1”部分。</translation>
    </message>
    <message>
        <source>The file does not contain a “%1” section; it holds %2.</source>
        <translation>文件中不包含“%1”部分；其中包含 %2。</translation>
    </message>
    <message>
        <source>The “%1” section does not contain any values.</source>
        <translation>“%1”部分不包含任何值。</translation>
    </message>
    <message>
        <source> and </source>
        <translation> 和 </translation>
    </message>
    <message>
        <source> and %1 more</source>
        <translation> 以及另外 %1 项</translation>
    </message>
    <message>
        <source>The file names values this version does not know, so they were not applied: %1.</source>
        <translation>该文件包含此版本无法识别的值，因此未应用：%1。</translation>
    </message>
    <message>
        <source>The file holds values of the wrong type, so they were not applied: %1.</source>
        <translation>该文件包含类型错误的值，因此未应用：%1。</translation>
    </message>
</context>
<context>
    <name>UpdateChecker</name>
    <message>
        <source>GitHub answered with HTTP status %1: %2</source>
        <translation>GitHub 返回了 HTTP 状态 %1：%2</translation>
    </message>
    <message>
        <source>GitHub answered with HTTP status %1.</source>
        <translation>GitHub 返回了 HTTP 状态 %1。</translation>
    </message>
    <message>
        <source>The release information is not valid JSON.</source>
        <translation>发布信息不是有效的 JSON。</translation>
    </message>
    <message>
        <source>The release information does not name a version.</source>
        <translation>发布信息中未指明版本。</translation>
    </message>
    <message>
        <source>The release version %1 is not a valid version number.</source>
        <translation>发布版本 %1 不是有效的版本号。</translation>
    </message>
    <message>
        <source>The installed version %1 is not a valid version number.</source>
        <translation>已安装的版本 %1 不是有效的版本号。</translation>
    </message>
</context>
</TS>
