<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN" sourcelanguage="en">
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
        <source>Ask GitHub whether a newer release is available.</source>
        <translation>询问 GitHub 是否有更新版本。</translation>
    </message>
    <message>
        <source>Copy the application, toolkit, system and hashcat versions to the clipboard.</source>
        <translation>将应用程序、工具包、系统和 hashcat 的版本复制到剪贴板。</translation>
    </message>
    <message>
        <source>Check for updates</source>
        <translation>检查更新</translation>
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
        <source>The configured hashcat executable is not found.</source>
        <translation>未找到已配置的 hashcat 可执行文件。</translation>
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
    <name>AttackModeView</name>
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
        <source>PCFG</source>
        <translation>PCFG</translation>
    </message>
    <message>
        <source>Table</source>
        <translation>表</translation>
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
        <source>This mode needs at least one wordlist.</source>
        <translation>此模式需要至少一个字典。</translation>
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
        <source>This mode needs a wordlist and a mask.</source>
        <translation>此模式需要一个字典和一个掩码。</translation>
    </message>
    <message>
        <source>This mode needs a feed. Arguments depend on the feed: random and stdin need none, wordlist needs a path.</source>
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
</context>
<context>
    <name>BackendInfoDialog</name>
    <message>
        <source>Backend information</source>
        <translation>后端信息</translation>
    </message>
    <message>
        <source>Query hashcat again for backend information.</source>
        <translation>再次向 hashcat 查询后端信息。</translation>
    </message>
    <message>
        <source>&amp;Refresh</source>
        <translation>刷新(&amp;R)</translation>
    </message>
    <message>
        <source>Stop the running query.</source>
        <translation>停止正在运行的查询。</translation>
    </message>
    <message>
        <source>C&amp;ancel</source>
        <translation>取消(&amp;A)</translation>
    </message>
    <message>
        <source>Copy the backend information to the clipboard.</source>
        <translation>将后端信息复制到剪贴板。</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>复制(&amp;C)</translation>
    </message>
    <message>
        <source>Close the dialog.</source>
        <translation>关闭对话框。</translation>
    </message>
    <message>
        <source>Cl&amp;ose</source>
        <translation>关闭(&amp;O)</translation>
    </message>
    <message>
        <source>No hashcat executable is configured. Set one in Settings.</source>
        <translation>未配置 hashcat 可执行文件。请在设置中指定。</translation>
    </message>
    <message>
        <source>The configured hashcat executable is not found:
%1</source>
        <translation>未找到已配置的 hashcat 可执行文件：
%1</translation>
    </message>
    <message>
        <source>Reading backend information…</source>
        <translation>正在读取后端信息…</translation>
    </message>
    <message>
        <source>Backend query cancelled.</source>
        <translation>后端查询已取消。</translation>
    </message>
    <message>
        <source>hashcat timed out while reading backend information.</source>
        <translation>hashcat 读取后端信息时超时。</translation>
    </message>
    <message>
        <source>hashcat could not read backend information.</source>
        <translation>hashcat 无法读取后端信息。</translation>
    </message>
    <message>
        <source>hashcat could not read backend information:
%1</source>
        <translation>hashcat 无法读取后端信息：
%1</translation>
    </message>
    <message>
        <source>Backend information reported by hashcat.</source>
        <translation>hashcat 报告的后端信息。</translation>
    </message>
</context>
<context>
    <name>ExampleSession</name>
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
    <name>HashcatRunner</name>
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
        <source>hashcat failed to run
%1</source>
        <translation>hashcat 运行失败
%1</translation>
    </message>
</context>
<context>
    <name>IdentifyDialog</name>
    <message>
        <source>Identify hash</source>
        <translation>识别哈希</translation>
    </message>
    <message>
        <source>&amp;Hash or hash file:</source>
        <translation>哈希或哈希文件(&amp;H)：</translation>
    </message>
    <message>
        <source>Paste a hash or select a hash file to identify.</source>
        <translation>粘贴哈希或选择哈希文件以进行识别。</translation>
    </message>
    <message>
        <source>Paste a hash or select a hash file</source>
        <translation>粘贴哈希或选择哈希文件</translation>
    </message>
    <message>
        <source>Choose a hash file.</source>
        <translation>选择一个哈希文件。</translation>
    </message>
    <message>
        <source>&amp;Browse…</source>
        <translation>浏览(&amp;B)…</translation>
    </message>
    <message>
        <source>Identify the hash type of the entered hash.</source>
        <translation>识别所输入哈希的哈希类型。</translation>
    </message>
    <message>
        <source>&amp;Identify</source>
        <translation>识别(&amp;I)</translation>
    </message>
    <message>
        <source>Paste a hash or choose a hash file, then select Identify.</source>
        <translation>粘贴哈希或选择哈希文件，然后选择“识别”。</translation>
    </message>
    <message>
        <source>Identification progress</source>
        <translation>识别进度</translation>
    </message>
    <message>
        <source>Elapsed time</source>
        <translation>已用时间</translation>
    </message>
    <message>
        <source>Matching hash types; select one and apply it to the attack.</source>
        <translation>匹配的哈希类型；选择一个并应用到攻击。</translation>
    </message>
    <message>
        <source>Mode</source>
        <translation>模式</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <source>Category</source>
        <translation>类别</translation>
    </message>
    <message>
        <source>Use the selected hash type in the attack.</source>
        <translation>在攻击中使用所选的哈希类型。</translation>
    </message>
    <message>
        <source>Apply mode to attack</source>
        <translation>将模式应用到攻击</translation>
    </message>
    <message>
        <source>Stop the running identification.</source>
        <translation>停止正在运行的识别。</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>取消(&amp;C)</translation>
    </message>
    <message>
        <source>Close the dialog.</source>
        <translation>关闭对话框。</translation>
    </message>
    <message>
        <source>Cl&amp;ose</source>
        <translation>关闭(&amp;O)</translation>
    </message>
    <message>
        <source>Elapsed: %1 s</source>
        <translation>已用时间：%1 秒</translation>
    </message>
    <message>
        <source>Multiple lines pasted. Use Browse to select a hash file, or paste one hash.</source>
        <translation>粘贴了多行内容。请使用“浏览”选择哈希文件，或只粘贴一个哈希。</translation>
    </message>
    <message>
        <source>Input changed. Identify it again.</source>
        <translation>输入已更改。请重新识别。</translation>
    </message>
    <message>
        <source>Select a hash file</source>
        <translation>选择哈希文件</translation>
    </message>
    <message>
        <source>Enter a hash or select a hash file first.</source>
        <translation>请先输入哈希或选择哈希文件。</translation>
    </message>
    <message>
        <source>No hashcat executable is configured. Set one in Settings.</source>
        <translation>未配置 hashcat 可执行文件。请在设置中指定。</translation>
    </message>
    <message>
        <source>The configured hashcat executable is not found:
%1</source>
        <translation>未找到已配置的 hashcat 可执行文件：
%1</translation>
    </message>
    <message>
        <source>Identifying hash…</source>
        <translation>正在识别哈希…</translation>
    </message>
    <message>
        <source>Identification cancelled.</source>
        <translation>识别已取消。</translation>
    </message>
    <message>
        <source>hashcat timed out while identifying the hash.</source>
        <translation>hashcat 识别哈希时超时。</translation>
    </message>
    <message>
        <source>No hash modes match this structure.</source>
        <translation>没有哈希模式与此结构匹配。</translation>
    </message>
    <message>
        <source>1 hash mode matches this structure. Select it and apply the mode and hash to the attack.</source>
        <translation>有 1 种哈希模式与此结构匹配。请选择它，并将模式和哈希应用到攻击。</translation>
    </message>
    <message>
        <source>1 hash mode matches this structure. Select it and apply the mode to the attack.</source>
        <translation>有 1 种哈希模式与此结构匹配。请选择它，并将模式应用到攻击。</translation>
    </message>
    <message>
        <source>%1 hash modes match this structure. Select one and apply the mode and hash to the attack.</source>
        <translation>有 %1 种哈希模式与此结构匹配。请选择一种，并将模式和哈希应用到攻击。</translation>
    </message>
    <message>
        <source>%1 hash modes match this structure. Select one and apply the mode to the attack.</source>
        <translation>有 %1 种哈希模式与此结构匹配。请选择一种，并将模式应用到攻击。</translation>
    </message>
    <message>
        <source>Cancelling…</source>
        <translation>正在取消…</translation>
    </message>
    <message>
        <source>Apply mode and hash to attack</source>
        <translation>将模式和哈希应用到攻击</translation>
    </message>
    <message>
        <source>Another hashcat instance is using this session. Close it and try again.</source>
        <translation>另一个 hashcat 实例正在使用此会话。请关闭它后重试。</translation>
    </message>
    <message>
        <source>hashcat could not load any hashes from the input.</source>
        <translation>hashcat 无法从输入中加载任何哈希。</translation>
    </message>
    <message>
        <source>The input contains characters hashcat cannot encode.</source>
        <translation>输入包含 hashcat 无法编码的字符。</translation>
    </message>
    <message>
        <source>hashcat reported:
%1</source>
        <translation>hashcat 报告：
%1</translation>
    </message>
    <message>
        <source>hashcat’s output could not be parsed. Check the hashcat version or report a bug.</source>
        <translation>无法解析 hashcat 的输出。请检查 hashcat 版本或报告错误。</translation>
    </message>
</context>
<context>
    <name>LaunchMessage</name>
    <message>
        <source>Windows executable names include their extension, for example hashcat.exe.</source>
        <translation>Windows 可执行文件名包含其扩展名，例如 hashcat.exe。</translation>
    </message>
    <message>
        <source>hash file</source>
        <translation>哈希文件</translation>
    </message>
    <message>
        <source>outfile</source>
        <translation>输出文件</translation>
    </message>
    <message>
        <source>potfile</source>
        <translation>potfile</translation>
    </message>
    <message>
        <source>wordlist</source>
        <translation>字典</translation>
    </message>
    <message>
        <source>rules file</source>
        <translation>规则文件</translation>
    </message>
    <message>
        <source>table</source>
        <translation>表</translation>
    </message>
    <message>
        <source>cache folder</source>
        <translation>缓存文件夹</translation>
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
        <source>The configured hashcat path is not an executable file:
%1
Navigate to %2 → %3 to change it.</source>
        <translation>已配置的 hashcat 路径不是可执行文件：
%1
请前往 %2 → %3 进行修改。</translation>
    </message>
    <message>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to select the terminal used for launching.</source>
        <translation>请前往 &lt;b&gt;%1 → %2&lt;/b&gt; 选择用于启动的终端。</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not available. Available terminals: %2
Navigate to %3 → %4 to change it.</source>
        <translation>已配置的终端“%1”不可用。可用的终端：%2
请前往 %3 → %4 进行修改。</translation>
    </message>
    <message>
        <source>This mode needs at least one wordlist.</source>
        <translation>此模式需要至少一个字典。</translation>
    </message>
    <message>
        <source>This mode needs exactly %1 wordlists (left, right).
Currently selected: %2.</source>
        <translation>此模式需要恰好 %1 个字典（左、右）。
当前已选择：%2 个。</translation>
    </message>
    <message>
        <source>Maximum number of wordlists for this mode: %1.
Currently selected: %2.</source>
        <translation>此模式的字典数量上限为 %1。
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
        <source>This mode needs a mask.</source>
        <translation>此模式需要一个掩码。</translation>
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
        <source>A custom charset is used only inside a mask, and this run named no mask. Add a mask= setting that references ?1–?4, for example mask=?1?1?1?1, or clear the custom charset.</source>
        <translation>自定义字符集只能在掩码中使用，而本次运行未指定掩码。请添加引用 ?1–?4 的 mask= 设置（例如 mask=?1?1?1?1），或清除自定义字符集。</translation>
    </message>
    <message>
        <source>The PCFG feed reads at most %1 rulesets.
Currently selected: %2.</source>
        <translation>PCFG feed 最多读取 %1 个规则集。
当前已选择：%2 个。</translation>
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
        <source>Enter an outfile format: one or more comma-separated numbers from 1 to 6.</source>
        <translation>请输入输出文件格式：一个或多个 1 到 6 的数字，以逗号分隔。</translation>
    </message>
    <message>
        <source>The outfile format must be one or more comma-separated numbers from 1 to 6:
%1</source>
        <translation>输出文件格式必须是一个或多个 1 到 6 的数字，以逗号分隔：
%1</translation>
    </message>
    <message>
        <source>The cache path must be an existing directory:
%1</source>
        <translation>缓存路径必须是已存在的目录：
%1</translation>
    </message>
    <message>
        <source>The potfile path must be a writable file, or a new file in an existing writable folder:
%1</source>
        <translation>potfile 路径必须是可写文件，或已存在可写文件夹中的新文件：
%1</translation>
    </message>
    <message>
        <source>The skip or limit value must be a whole number from 0 to %1:
%2</source>
        <translation>跳过或限制值必须是 0 到 %1 的整数：
%2</translation>
    </message>
    <message>
        <source>The device types must be 1 (CPU) or 2 (GPU), separated by commas:
%1</source>
        <translation>设备类型必须是 1（CPU）或 2（GPU），以逗号分隔：
%1</translation>
    </message>
    <message>
        <source>The CPU affinity must be a comma-separated list of CPU numbers:
%1</source>
        <translation>CPU 亲和性必须是以逗号分隔的 CPU 编号列表：
%1</translation>
    </message>
    <message>
        <source>The backend devices must be a comma-separated list of device numbers starting at 1:
%1</source>
        <translation>后端设备必须是以逗号分隔、从 1 开始的设备编号列表：
%1</translation>
    </message>
    <message>
        <source>The session name must not contain a path separator:
%1</source>
        <translation>会话名称不能包含路径分隔符：
%1</translation>
    </message>
    <message>
        <source>A ruleset folder needs Grammar/grammar.txt; choose a rules file instead:
%1</source>
        <translation>规则集文件夹需要 Grammar/grammar.txt；请改为选择规则文件：
%1</translation>
    </message>
    <message>
        <source>A table has to be a file; hashcat does not read a folder:
%1</source>
        <translation>表必须是文件；hashcat 不读取文件夹：
%1</translation>
    </message>
    <message>
        <source>The %1 and the %2 use the same path:
%3
hashcat writes one or both of them, so a run could change the file. Choose different paths.</source>
        <translation>%1 和 %2 使用了相同的路径：
%3
hashcat 会写入其中之一或两者，因此运行可能会改变文件。请选择不同的路径。</translation>
    </message>
    <message>
        <source>The increment cannot be combined with the speed-only option.</source>
        <translation>递增不能与仅测速选项同时使用。</translation>
    </message>
    <message>
        <source>Removing cracked hashes is not possible in the association attack mode.</source>
        <translation>在“关联”攻击模式下无法移除已破解的哈希。</translation>
    </message>
    <message>
        <source>Keep guessing cannot be combined with removing cracked hashes from the hash file. Clear either option.</source>
        <translation>持续猜测不能与从哈希文件中移除已破解的哈希同时使用。请清除其中一个选项。</translation>
    </message>
    <message>
        <source>This value contains a control character that cannot be passed to hashcat:
%1
Remove the character and try again.</source>
        <translation>此值包含无法传递给 hashcat 的控制字符：
%1
请删除该字符后重试。</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not available.</source>
        <translation>已配置的终端“%1”不可用。</translation>
    </message>
    <message>
        <source>The command line is too long for a PowerShell terminal. The encoded script must fit into 32,767 characters, which leaves room for about 12,000 ASCII characters of hashcat arguments. Shorten the argument lists or their values and try again.</source>
        <translation>命令行对于 PowerShell 终端来说过长。编码后的脚本必须能放入 32,767 个字符，这为 hashcat 参数留出约 12,000 个 ASCII 字符的空间。请缩短参数列表或其值后重试。</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <source>The hash, a file of hashes, or a .hc22000/.hccapx WPA capture.</source>
        <translation>哈希、哈希文件，或 .hc22000/.hccapx 格式的 WPA 抓包文件。</translation>
    </message>
    <message>
        <source>Choose a file of hashes.</source>
        <translation>选择一个哈希文件。</translation>
    </message>
    <message>
        <source>Salt is hex</source>
        <translation>盐值为十六进制</translation>
    </message>
    <message>
        <source>Rewrite the hash file without cracked hashes, including potfile-known ones; the potfile is untouched. Refused in attack mode 9.</source>
        <translation>重写哈希文件，去除已破解的哈希（包括 potfile 中已有的）；potfile 不会被修改。在攻击模式 9 下会被拒绝。</translation>
    </message>
    <message>
        <source>Remove hash from hash list once it is cracked</source>
        <translation>破解后从哈希列表中移除该哈希</translation>
    </message>
    <message>
        <source>Ignore the username before the mode’s separator (usually “:”), so accounts sharing a hash are cracked once.</source>
        <translation>忽略模式分隔符（通常为“:”）之前的用户名，使共用同一哈希的账户只被破解一次。</translation>
    </message>
    <message>
        <source>Hash recipe, e.g. “md5(md5(salt) . pass)”. Wrap it in the shell’s outer quotes and put any inner strings in the other kind of quotes; PowerShell needs a backtick before $.</source>
        <translation>哈希配方，例如“md5(md5(salt) . pass)”。请用 Shell 的外层引号包住整个配方，内部字符串则使用另一种引号；PowerShell 需要在 $ 前加反引号。</translation>
    </message>
    <message>
        <source>Move selected wordlist up</source>
        <translation>上移所选字典</translation>
    </message>
    <message>
        <source>Move selected wordlist down</source>
        <translation>下移所选字典</translation>
    </message>
    <message>
        <source>Add one or more wordlist files. Each checked row is passed to hashcat.</source>
        <translation>添加一个或多个字典文件。每个已勾选的行都会传递给 hashcat。</translation>
    </message>
    <message>
        <source>Remove the selected wordlists.</source>
        <translation>移除所选字典。</translation>
    </message>
    <message>
        <source>Remove wordlists</source>
        <translation>移除字典</translation>
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
        <source>A ruleset file, a folder with Grammar/grammar.txt, or a bundled name. Unchecked rows are skipped; none runs default-passwords.</source>
        <translation>规则集文件、包含 Grammar/grammar.txt 的文件夹，或内置名称。未勾选的行会被跳过；若一行都未勾选，则运行 default-passwords。</translation>
    </message>
    <message>
        <source>Add one or more PCFG ruleset files.</source>
        <translation>添加一个或多个 PCFG 规则集文件。</translation>
    </message>
    <message>
        <source>Add PCFG rulesets</source>
        <translation>添加 PCFG 规则集</translation>
    </message>
    <message>
        <source>Remove the selected rulesets.</source>
        <translation>移除所选规则集。</translation>
    </message>
    <message>
        <source>Remove rulesets</source>
        <translation>移除规则集</translation>
    </message>
    <message>
        <source>Move selected ruleset up.</source>
        <translation>上移所选规则集。</translation>
    </message>
    <message>
        <source>Move selected ruleset up</source>
        <translation>上移所选规则集</translation>
    </message>
    <message>
        <source>Move selected ruleset down.</source>
        <translation>下移所选规则集。</translation>
    </message>
    <message>
        <source>Move selected ruleset down</source>
        <translation>下移所选规则集</translation>
    </message>
    <message>
        <source>Sca&amp;le:</source>
        <translation>缩放(&amp;L)：</translation>
    </message>
    <message>
        <source>Quantization steps per bit (1–64).</source>
        <translation>每位的量化步数（1–64）。</translation>
    </message>
    <message>
        <source>&amp;Costmax:</source>
        <translation>成本上限(&amp;C)：</translation>
    </message>
    <message>
        <source>Highest cost level to enumerate (1–64).</source>
        <translation>要枚举的最高层级成本（1–64）。</translation>
    </message>
    <message>
        <source>&amp;Weights:</source>
        <translation>权重(&amp;W)：</translation>
    </message>
    <message>
        <source>&amp;Kbits:</source>
        <translation>Kbits(&amp;K)：</translation>
    </message>
    <message>
        <source>Bits one inner loop may span (0–27).</source>
        <translation>一次内层循环可能跨越的位数（0–27）。</translation>
    </message>
    <message>
        <source>Ma&amp;xword:</source>
        <translation>Maxword(&amp;X)：</translation>
    </message>
    <message>
        <source>Maximum candidate length the kernel builds, in 4-byte words: 0 (auto), 16 or 32.</source>
        <translation>内核构建的候选最大长度，以 4 字节字为单位：0（自动）、16 或 32。</translation>
    </message>
    <message>
        <source>Max&amp;gain:</source>
        <translation>Maxgain(&amp;G)：</translation>
    </message>
    <message>
        <source>Space-separated key=value settings: omen, cache, lookup, pwmin, pwmax, hintaccount, hintwords, hintfile, hintrank, hintrepeat. Rulesets go first.</source>
        <translation>以空格分隔的 key=value 设置：omen、cache、lookup、pwmin、pwmax、hintaccount、hintwords、hintfile、hintrank、hintrepeat。规则集放在最前面。</translation>
    </message>
    <message>
        <source>Tables</source>
        <translation>表</translation>
    </message>
    <message>
        <source>Add one or more table files.</source>
        <translation>添加一个或多个表文件。</translation>
    </message>
    <message>
        <source>Add table</source>
        <translation>添加表</translation>
    </message>
    <message>
        <source>Remove the selected tables.</source>
        <translation>移除所选表。</translation>
    </message>
    <message>
        <source>Remove tables</source>
        <translation>移除表</translation>
    </message>
    <message>
        <source>Move selected table up.</source>
        <translation>上移所选表。</translation>
    </message>
    <message>
        <source>Move selected table down.</source>
        <translation>下移所选表。</translation>
    </message>
    <message>
        <source>1 (default) keeps a token unchanged as a choice, 0 drops it so the table only yields variations. identity=0 excludes single=1.</source>
        <translation>设为 1（默认）时，token 会作为一个选项原样保留；设为 0 时丢弃它，表只产生变体。identity=0 会排除 single=1。</translation>
    </message>
    <message>
        <source>1 reads the wordlist as patterns: no word is returned unchanged and unmatched words are dropped. 0 (default) keeps them.</source>
        <translation>设为 1 时，将字典作为模式读取：不会原样返回任何单词，未匹配的单词会被丢弃。设为 0（默认）时则保留它们。</translation>
    </message>
    <message>
        <source>single=1: one substitution per candidate instead of the cross product; fewer candidates, more host work on fast hashes.</source>
        <translation>single=1：每个候选只做一次替换，而不是取笛卡尔积；候选更少，但在快速哈希上主机的工作量更大。</translation>
    </message>
    <message>
        <source>Keep only the first N replacements of each source word; 0 means no limit. Applies to single=1 too.</source>
        <translation>每个源单词只保留前 N 个替换；0 表示不限制。也适用于 single=1。</translation>
    </message>
    <message>
        <source>How much of a word’s cross product the host enumerates before giving up; 0 means no limit.</source>
        <translation>主机在放弃之前枚举一个单词的笛卡尔积的多少；0 表示不限制。</translation>
    </message>
    <message>
        <source>Key=value settings separated by spaces; everything after the first one is a setting, so tables come first.</source>
        <translation>以空格分隔的 key=value 设置；第一个之后的所有内容都是设置，因此表要放在最前面。</translation>
    </message>
    <message>
        <source>Feed name or path. A slash means a path; otherwise hashcat tries feed_&lt;name&gt;, rust_&lt;name&gt;, then &lt;name&gt; in its feeds directory.</source>
        <translation>feed 名称或路径。包含斜杠表示路径；否则 hashcat 会在其 feed 目录中依次尝试 feed_&lt;name&gt;、rust_&lt;name&gt; 和 &lt;name&gt;。</translation>
    </message>
    <message>
        <source>Browse for a feed plugin instead of naming it.</source>
        <translation>浏览选择 feed 插件，而不是输入其名称。</translation>
    </message>
    <message>
        <source>Browse for a feed plugin</source>
        <translation>浏览 feed 插件</translation>
    </message>
    <message>
        <source>Argument passed to the feed; meaning and count depend on the feed (the wordlist feed takes wordlist paths).</source>
        <translation>传递给 feed 的参数；含义和数量取决于 feed（wordlist feed 接受字典路径）。</translation>
    </message>
    <message>
        <source>Add an argument for the feed, in order.</source>
        <translation>为 feed 按顺序添加一个参数。</translation>
    </message>
    <message>
        <source>Add feed argument</source>
        <translation>添加 feed 参数</translation>
    </message>
    <message>
        <source>Feed arguments</source>
        <translation>feed 参数</translation>
    </message>
    <message>
        <source>Remove the selected feed arguments.</source>
        <translation>移除所选 feed 参数。</translation>
    </message>
    <message>
        <source>Remove feed arguments</source>
        <translation>移除 feed 参数</translation>
    </message>
    <message>
        <source>Move selected feed argument up.</source>
        <translation>上移所选 feed 参数。</translation>
    </message>
    <message>
        <source>Move selected feed argument up</source>
        <translation>上移所选 feed 参数</translation>
    </message>
    <message>
        <source>Move selected feed argument down.</source>
        <translation>下移所选 feed 参数。</translation>
    </message>
    <message>
        <source>Move selected feed argument down</source>
        <translation>下移所选 feed 参数</translation>
    </message>
    <message>
        <source>Read rules from the files below instead of generating them.</source>
        <translation>从下方的文件中读取规则，而不是生成规则。</translation>
    </message>
    <message>
        <source>Rules file 1:</source>
        <translation>规则文件 1：</translation>
    </message>
    <message>
        <source>Choose the rules file for slot 1.</source>
        <translation>选择槽位 1 的规则文件。</translation>
    </message>
    <message>
        <source>Open rules file 1</source>
        <translation>打开规则文件 1</translation>
    </message>
    <message>
        <source>Rules file 2:</source>
        <translation>规则文件 2：</translation>
    </message>
    <message>
        <source>Choose the rules file for slot 2.</source>
        <translation>选择槽位 2 的规则文件。</translation>
    </message>
    <message>
        <source>Open rules file 2</source>
        <translation>打开规则文件 2</translation>
    </message>
    <message>
        <source>Rules file 3:</source>
        <translation>规则文件 3：</translation>
    </message>
    <message>
        <source>Choose the rules file for slot 3.</source>
        <translation>选择槽位 3 的规则文件。</translation>
    </message>
    <message>
        <source>Open rules file 3</source>
        <translation>打开规则文件 3</translation>
    </message>
    <message>
        <source>Number of random rules hashcat generates (-g). Only used with Generate rules; no rules file is read.</source>
        <translation>hashcat 生成的随机规则数量（-g）。仅在使用“生成规则”时有效；不会读取规则文件。</translation>
    </message>
    <message>
        <source>Treat every -r file as one list: rule counts add instead of multiplying.</source>
        <translation>将每个 -r 文件视为一个列表：规则数量相加而不是相乘。</translation>
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
        <source>Minimum mask length; 0 omits the flag and uses hashcat’s default (1).</source>
        <translation>掩码最小长度；0 表示省略该选项并使用 hashcat 的默认值（1）。</translation>
    </message>
    <message>
        <source>Maximum mask length; 0 omits the flag and uses hashcat’s default (the mask length).</source>
        <translation>掩码最大长度；0 表示省略该选项并使用 hashcat 的默认值（掩码长度）。</translation>
    </message>
    <message>
        <source>Grow the mask from the right end instead of the left (hashcat -ii).</source>
        <translation>从右端而非左端增长掩码（hashcat -ii）。</translation>
    </message>
    <message>
        <source>Right-to-left</source>
        <translation>从右到左</translation>
    </message>
    <message>
        <source>Use ?1 in the mask to insert this charset.</source>
        <translation>在掩码中使用 ?1 以插入此字符集。</translation>
    </message>
    <message>
        <source>Use ?2 in the mask to insert this charset.</source>
        <translation>在掩码中使用 ?2 以插入此字符集。</translation>
    </message>
    <message>
        <source>Use ?3 in the mask to insert this charset.</source>
        <translation>在掩码中使用 ?3 以插入此字符集。</translation>
    </message>
    <message>
        <source>Use ?4 in the mask to insert this charset.</source>
        <translation>在掩码中使用 ?4 以插入此字符集。</translation>
    </message>
    <message>
        <source>Interpret the mask and custom charsets as hexadecimal bytes.</source>
        <translation>将掩码和自定义字符集解释为十六进制字节。</translation>
    </message>
    <message>
        <source>Charset is hex</source>
        <translation>字符集为十六进制</translation>
    </message>
    <message>
        <source>&amp;Session name:</source>
        <translation>会话名称(&amp;S)：</translation>
    </message>
    <message>
        <source>R&amp;untime limit:</source>
        <translation>运行时限(&amp;U)：</translation>
    </message>
    <message>
        <source>&amp;Device types:</source>
        <translation>设备类型(&amp;D)：</translation>
    </message>
    <message>
        <source>Potfile:</source>
        <translation>potfile：</translation>
    </message>
    <message>
        <source>Read and write recovered hashes in this potfile instead of the default one.</source>
        <translation>在此 potfile（而非默认 potfile）中读写已恢复的哈希。</translation>
    </message>
    <message>
        <source>Choose the potfile path.</source>
        <translation>选择 potfile 路径。</translation>
    </message>
    <message>
        <source>Choose potfile</source>
        <translation>选择 potfile</translation>
    </message>
    <message>
        <source>Copy command to clipboard.</source>
        <translation>将命令复制到剪贴板。</translation>
    </message>
    <message>
        <source>Command preview</source>
        <translation>命令预览</translation>
    </message>
    <message>
        <source>Ctrl+Shift+I</source>
        <translation>Ctrl+Shift+I</translation>
    </message>
    <message>
        <source>Copy the unencoded PowerShell script that starts hashcat.</source>
        <translation>复制用于启动 hashcat 的未编码 PowerShell 脚本。</translation>
    </message>
    <message>
        <source>&amp;Identify Hash…</source>
        <translation>识别哈希(&amp;I)…</translation>
    </message>
    <message>
        <source>Ask hashcat which hash modes match a hash.</source>
        <translation>询问 hashcat 哪些哈希模式与哈希匹配。</translation>
    </message>
    <message>
        <source>&amp;Backend Information…</source>
        <translation>后端信息(&amp;B)…</translation>
    </message>
    <message>
        <source>Show the devices and backends hashcat detected.</source>
        <translation>显示 hashcat 检测到的设备和后端。</translation>
    </message>
    <message>
        <source>Check Against &amp;Potfile…</source>
        <translation>对照 potfile 检查(&amp;P)…</translation>
    </message>
    <message>
        <source>Check whether the hashes in the hash field are already in the potfile.</source>
        <translation>检查哈希字段中的哈希是否已存在于 potfile 中。</translation>
    </message>
    <message>
        <source>Potfile I&amp;nspector…</source>
        <translation>potfile 检查器(&amp;N)…</translation>
    </message>
    <message>
        <source>Browse an explicit potfile without hash-type verification.</source>
        <translation>浏览指定的 potfile，无需验证哈希类型。</translation>
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
        <source>Use hashcat’s optimized kernels (-O); they cap the maximum password length.</source>
        <translation>使用 hashcat 的优化内核（-O）；它们会限制密码的最大长度。</translation>
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
        <source>Outfile</source>
        <translation>输出文件</translation>
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
        <source>Attack</source>
        <translation>攻击</translation>
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
        <source>Attack mode. Each choice reveals its own options below.</source>
        <translation>攻击模式。每个选项会在下方显示各自的选项。</translation>
    </message>
    <message>
        <source>Add files…</source>
        <translation>添加文件…</translation>
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
        <source>Space-separated key=value settings for attack mode 9, e.g. phases=words,rules or rules.rulefile=…. Only used without a wordlist. The default phases=words,rules,pcfg never ends (the pcfg phase is unbounded); set phases=words,rules for a bounded run.</source>
        <translation>攻击模式 9 的以空格分隔的 key=value 设置，例如 phases=words,rules 或 rules.rulefile=…。仅在没有字典时使用。默认值 phases=words,rules,pcfg 永远不会结束（pcfg 阶段没有上限）；请设置 phases=words,rules 以获得有限的运行。</translation>
    </message>
    <message>
        <source>PCFG</source>
        <translation>PCFG</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>名称：</translation>
    </message>
    <message>
        <source>A bundled or installed ruleset name, such as default-passwords or hints. A slash means a path; hashcat searches its profile and shared pcfg folders first.</source>
        <translation>内置或已安装的规则集名称，例如 default-passwords 或 hints。包含斜杠表示路径；hashcat 会先在其配置文件夹和共享的 pcfg 文件夹中搜索。</translation>
    </message>
    <message>
        <source>default-passwords, hints, or a ruleset path</source>
        <translation>default-passwords、hints，或规则集路径</translation>
    </message>
    <message>
        <source>Add a bundled or installed ruleset by name.</source>
        <translation>按名称添加内置或已安装的规则集。</translation>
    </message>
    <message>
        <source>Add ruleset name</source>
        <translation>添加规则集名称</translation>
    </message>
    <message>
        <source>A ruleset folder needs a Grammar/grammar.txt.</source>
        <translation>规则集文件夹需要包含 Grammar/grammar.txt。</translation>
    </message>
    <message>
        <source>One positive number per ruleset, colon-separated, e.g. 2:1; the count must match the number of rulesets.</source>
        <translation>每个规则集一个正数，以冒号分隔，例如 2:1；数量必须与规则集的数量一致。</translation>
    </message>
    <message>
        <source>Threads:</source>
        <translation>线程：</translation>
    </message>
    <message>
        <source>Cores that generate base words (0–16); 0 runs on the calling thread.</source>
        <translation>生成基础单词的核心数（0–16）；0 表示在调用线程中运行。</translation>
    </message>
    <message>
        <source>auto</source>
        <translation>自动</translation>
    </message>
    <message>
        <source>1.0–64.0; decimal point is a dot, not the locale separator.</source>
        <translation>1.0–64.0；小数点为英文句点，而非区域设置中的分隔符。</translation>
    </message>
    <message>
        <source>Walk:</source>
        <translation>Walk:</translation>
    </message>
    <message>
        <source>1 (default) steps to the next base word where the ordering allows (the fast path); 0 unranks every base word. The candidates are identical either way.</source>
        <translation>设为 1（默认）时，在顺序允许的情况下直接步进到下一个基础单词（快速路径）；设为 0 时对每个基础单词重新进行反排名（unranking）。两种方式产生的候选完全相同。</translation>
    </message>
    <message>
        <source>Mask:</source>
        <translation>掩码：</translation>
    </message>
    <message>
        <source>A mask selects the candidates the grammar may spell. ?1–?4 insert a custom charset. Cannot combine with omen=1, or hintaccount without hintwords/hintfile.</source>
        <translation>掩码用于选择语法可以生成的候选。?1–?4 插入自定义字符集。不能与 omen=1 组合使用，也不能在没有 hintwords/hintfile 的情况下与 hintaccount 组合使用。</translation>
    </message>
    <message>
        <source>Extra settings (key=value):</source>
        <translation>额外设置（key=value）：</translation>
    </message>
    <message>
        <source>Table</source>
        <translation>表</translation>
    </message>
    <message>
        <source>Every checked row after the wordlist is a table.</source>
        <translation>字典之后每个已勾选的行都是一个表。</translation>
    </message>
    <message>
        <source>Add tables…</source>
        <translation>添加表…</translation>
    </message>
    <message>
        <source>Move selected table up</source>
        <translation>上移所选表</translation>
    </message>
    <message>
        <source>Move selected table down</source>
        <translation>下移所选表</translation>
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
        <translation>通用 Feed</translation>
    </message>
    <message>
        <source>Feed:</source>
        <translation>Feed：</translation>
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
        <source>Copy as PowerShell Script</source>
        <translation>复制为 PowerShell 脚本</translation>
    </message>
    <message>
        <source>Add</source>
        <translation>添加</translation>
    </message>
    <message>
        <source>Arguments the feed reads, in order; only checked rows are used.</source>
        <translation>feed 按顺序读取的参数；仅使用已勾选的行。</translation>
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
        <source>Rules file for slot 1; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>槽位 1 的规则文件；应用于每个单词。留空则不使用该槽位。</translation>
    </message>
    <message>
        <source>Enable rules file slot 2 for this run.</source>
        <translation>为本次运行启用规则文件槽位 2。</translation>
    </message>
    <message>
        <source>Rules file for slot 2; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>槽位 2 的规则文件；应用于每个单词。留空则不使用该槽位。</translation>
    </message>
    <message>
        <source>Enable rules file slot 3 for this run.</source>
        <translation>为本次运行启用规则文件槽位 3。</translation>
    </message>
    <message>
        <source>Rules file for slot 3; applied to each word. Leave empty to leave the slot unused.</source>
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
        <source>Session and limits</source>
        <translation>会话和限制</translation>
    </message>
    <message>
        <source>Name hashcat uses for its restore file and status output.</source>
        <translation>hashcat 用于其恢复文件和状态输出的名称。</translation>
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
        <source>Seconds between status updates; only used when automatic status is enabled.</source>
        <translation>状态更新之间的间隔秒数；仅在启用自动状态时使用。</translation>
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
        <source>Cache path:</source>
        <translation>缓存路径：</translation>
    </message>
    <message>
        <source>Directory hashcat keeps its cache in. Must exist.</source>
        <translation>hashcat 保存其缓存的目录。必须存在。</translation>
    </message>
    <message>
        <source>Choose the cache directory.</source>
        <translation>选择缓存目录。</translation>
    </message>
    <message>
        <source>Choose cache directory</source>
        <translation>选择缓存目录</translation>
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
        <source>Abort when any backend device exceeds this temperature in °C; 0 disables it.</source>
        <translation>当任一后端设备超过此温度（°C）时中止；0 表示禁用。</translation>
    </message>
    <message>
        <source>Write recovered hashes to the file below, in the selected formats.</source>
        <translation>将已恢复的哈希按所选格式写入下方的文件。</translation>
    </message>
    <message>
        <source>Write recovered hashes to file:</source>
        <translation>将已恢复的哈希写入文件：</translation>
    </message>
    <message>
        <source>The file the recovered hashes are written to. Format variables:

&lt;hash&gt; = current hash filename or hash
&lt;unixtime&gt; = current Unix timestamp</source>
        <translation>写入已恢复哈希的文件。格式变量：

&lt;hash&gt; = 当前哈希文件名或哈希
&lt;unixtime&gt; = 当前 Unix 时间戳</translation>
    </message>
    <message>
        <source>Choose the outfile path.</source>
        <translation>选择输出文件路径。</translation>
    </message>
    <message>
        <source>Choose outfile</source>
        <translation>选择输出文件</translation>
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
        <source>Run hashcat in a terminal with these options.</source>
        <translation>在终端中使用这些选项运行 hashcat。</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>开始</translation>
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
        <source>H&amp;ash file or hash:</source>
        <translation>哈希文件或哈希(&amp;A)：</translation>
    </message>
    <message>
        <source>Keep attacking a hash after it is cracked, to find further plaintexts such as collisions. Cannot be combined with removing cracked hashes.</source>
        <translation>哈希被破解后仍继续攻击，以找到更多明文（如碰撞）。不能与移除已破解的哈希同时使用。</translation>
    </message>
    <message>
        <source>Keep guessing</source>
        <translation>持续猜测</translation>
    </message>
    <message>
        <source>&amp;Mode:</source>
        <translation>模式(&amp;M)：</translation>
    </message>
    <message>
        <source>Hash t&amp;ype:</source>
        <translation>哈希类型(&amp;Y)：</translation>
    </message>
    <message>
        <source>Hash &amp;recipe:</source>
        <translation>哈希配方(&amp;R)：</translation>
    </message>
    <message>
        <source>Wordlists</source>
        <translation>字典</translation>
    </message>
    <message>
        <source>Move selected wordlist up.</source>
        <translation>上移所选字典。</translation>
    </message>
    <message>
        <source>Move selected wordlist down.</source>
        <translation>下移所选字典。</translation>
    </message>
    <message>
        <source>Add a folder; hashcat reads every file directly inside it as one wordlist.</source>
        <translation>添加文件夹；hashcat 会将其直接包含的每个文件视为一个字典。</translation>
    </message>
    <message>
        <source>&amp;Encoding from:</source>
        <translation>源编码(&amp;E)：</translation>
    </message>
    <message>
        <source>Enc&amp;oding to:</source>
        <translation>目标编码(&amp;O)：</translation>
    </message>
    <message>
        <source>PCFG rulesets</source>
        <translation>PCFG 规则集</translation>
    </message>
    <message>
        <source>Increment the mask length from the minimum to the maximum.</source>
        <translation>将掩码长度从最小值递增到最大值。</translation>
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
        <source>Enable optimized kernels (limits password length)</source>
        <translation>启用优化内核（会限制密码长度）</translation>
    </message>
    <message>
        <source>Print only the attack’s speed, then quit; no cracking.</source>
        <translation>仅输出攻击速度，然后退出；不进行破解。</translation>
    </message>
    <message>
        <source>Return expected speed of the attack, then quit</source>
        <translation>输出攻击的预期速度，然后退出</translation>
    </message>
    <message>
        <source>&#xa0;°C</source>
        <translation>&#xa0;°C</translation>
    </message>
    <message>
        <source>Copy command</source>
        <translation>复制命令</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>帮助(&amp;H)</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>工具(&amp;T)</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>文件(&amp;F)</translation>
    </message>
    <message>
        <source>Ctrl+Q</source>
        <translation>Ctrl+Q</translation>
    </message>
    <message>
        <source>Settings…</source>
        <translation>设置…</translation>
    </message>
    <message>
        <source>Ctrl+,</source>
        <translation>Ctrl+,</translation>
    </message>
    <message>
        <source>Import Settings…</source>
        <translation>导入设置…</translation>
    </message>
    <message>
        <source>Ctrl+Shift+O</source>
        <translation>Ctrl+Shift+O</translation>
    </message>
    <message>
        <source>Export Settings…</source>
        <translation>导出设置…</translation>
    </message>
    <message>
        <source>Ctrl+Shift+S</source>
        <translation>Ctrl+Shift+S</translation>
    </message>
    <message>
        <source>Export Session…</source>
        <translation>导出会话…</translation>
    </message>
    <message>
        <source>Ctrl+Shift+E</source>
        <translation>Ctrl+Shift+E</translation>
    </message>
    <message>
        <source>Import Session…</source>
        <translation>导入会话…</translation>
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
        <source>Settings exported to “%1”.</source>
        <translation>设置已导出到“%1”。</translation>
    </message>
    <message>
        <source>OpenCL device types to use, separated by commas.</source>
        <translation>要使用的 OpenCL 设备类型，以逗号分隔。</translation>
    </message>
    <message>
        <source>Hash type changed</source>
        <translation>哈希类型已更改</translation>
    </message>
    <message>
        <source>The hash types could not be refreshed; showing the previous list.</source>
        <translation>无法刷新哈希类型；显示的是之前的列表。</translation>
    </message>
    <message>
        <source>The hash algorithm (-m). Auto-detect omits -m and lets hashcat identify the type. Type to search the list.</source>
        <translation>哈希算法（-m）。“自动检测”会省略 -m，由 hashcat 识别类型。输入文字可搜索列表。</translation>
    </message>
    <message>
        <source>Use ?1–?4 in a mask to insert a custom charset. A mask is used by Brute-force, the hybrid modes and PCFG’s mask= setting. In PCFG a custom charset needs mask=; a charset the mask does not reference is ignored.</source>
        <translation>在掩码中使用 ?1–?4 可插入自定义字符集。暴力破解、混合模式以及 PCFG 的 mask= 设置会使用掩码。在 PCFG 中，自定义字符集需要 mask=；掩码未引用的字符集会被忽略。</translation>
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
        <source>The session names the hash type %1, which this hashcat version does not support.</source>
        <translation>会话指定了哈希类型 %1，此版本的 hashcat 不支持该类型。</translation>
    </message>
    <message>
        <source>Password candidate mask. ?1–?4 insert a custom charset, ?w inserts the wordlist word and ?q a second wordlist word in the Hybrid (mask + ?w/?q) mode, and ?? is a literal ?.</source>
        <translation>密码候选掩码。?1–?4 插入自定义字符集，?w 插入字典中的单词，?q 在“混合（掩码 + ?w/?q）”模式下插入第二个字典中的单词，?? 表示字面的 ?。</translation>
    </message>
    <message>
        <source>Load the %1 example? The current session is lost.

The example cracks a test hash; hashcat should recover the password %2.</source>
        <translation>要加载 %1 示例吗？当前会话将丢失。

该示例会破解一个测试哈希；hashcat 应能恢复出密码 %2。</translation>
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
        <source>

This can be caused by Windows Defender or other antivirus software. See the &lt;a href=&quot;%1&quot;&gt;FAQ entry&lt;/a&gt; for a remedy.</source>
        <translation>

这可能是由 Windows Defender 或其他杀毒软件引起的。解决方法请参阅&lt;a href=&quot;%1&quot;&gt;常见问题条目&lt;/a&gt;。</translation>
    </message>
    <message>
        <source>Enter a hash or select a hash file first.</source>
        <translation>请先输入哈希或选择哈希文件。</translation>
    </message>
    <message>
        <source>No hashcat executable is configured. Set one in Settings.</source>
        <translation>未配置 hashcat 可执行文件。请在设置中指定。</translation>
    </message>
    <message>
        <source>Check Against Potfile</source>
        <translation>对照 potfile 检查</translation>
    </message>
    <message>
        <source>Checking potfile…</source>
        <translation>正在检查 potfile…</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
    <message>
        <source>Check cancelled.</source>
        <translation>检查已取消。</translation>
    </message>
    <message>
        <source>hashcat could not query the potfile.</source>
        <translation>hashcat 无法查询 potfile。</translation>
    </message>
    <message>
        <source>Identify hash</source>
        <translation>识别哈希</translation>
    </message>
    <message>
        <source>The hash type list is unavailable or still loading. Nothing was changed. Check the hashcat path in Settings or try again once the list is available.</source>
        <translation>哈希类型列表不可用或仍在加载。未做任何更改。请在设置中检查 hashcat 路径，或等列表可用后重试。</translation>
    </message>
    <message>
        <source>Hash mode %1 is not in this hashcat’s hash-type list. Nothing was changed.</source>
        <translation>哈希模式 %1 不在此 hashcat 的哈希类型列表中。未做任何更改。</translation>
    </message>
    <message>
        <source>The session file could not be read, so the session was not restored.

%1</source>
        <translation>无法读取会话文件，因此未恢复会话。

%1</translation>
    </message>
    <message>
        <source>Move the file aside to start with a fresh session? It is renamed, never deleted.</source>
        <translation>要将该文件移开以启动全新会话吗？文件只会被重命名，不会被删除。</translation>
    </message>
    <message>
        <source>Move File Aside</source>
        <translation>移开文件</translation>
    </message>
    <message>
        <source>Keep File</source>
        <translation>保留文件</translation>
    </message>
    <message>
        <source>The session file could not be moved to:
%1</source>
        <translation>无法将会话文件移动到：
%1</translation>
    </message>
    <message>
        <source>The session file was moved to:
%1</source>
        <translation>会话文件已移动到：
%1</translation>
    </message>
    <message>
        <source>The session names the hash type %1, which this hashcat version does not support. Auto-detect was selected.</source>
        <translation>会话指定了哈希类型 %1，此版本的 hashcat 不支持该类型。已选择“自动检测”。</translation>
    </message>
    <message>
        <source>The selected hash type %1 is not offered by this hashcat version. Auto-detect was selected.</source>
        <translation>此版本的 hashcat 未提供所选的哈希类型 %1。已选择“自动检测”。</translation>
    </message>
    <message>
        <source>Set the hashcat path under %1 → %2 to load the available hash types.</source>
        <translation>请在 %1 → %2 中设置 hashcat 路径，以加载可用的哈希类型。</translation>
    </message>
    <message>
        <source>The configured hashcat executable is not found: %1</source>
        <translation>未找到已配置的 hashcat 可执行文件：%1</translation>
    </message>
    <message>
        <source>Auto-detect</source>
        <translation>自动检测</translation>
    </message>
    <message>
        <source>Open cache directory</source>
        <translation>打开缓存目录</translation>
    </message>
    <message>
        <source>Wait until the hash type list has loaded.</source>
        <translation>请等待哈希类型列表加载完成。</translation>
    </message>
    <message>
        <source>The hash type list is still loading. Wait until it has loaded and try again.</source>
        <translation>哈希类型列表仍在加载。请等待加载完成后重试。</translation>
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
        <source>Copied to the clipboard</source>
        <translation>已复制到剪贴板</translation>
    </message>
    <message>
        <source>Select the hashcat executable under %1 → %2 first.</source>
        <translation>请先在 %1 → %2 中选择 hashcat 可执行文件。</translation>
    </message>
    <message>
        <source>The configured hashcat executable is not found:
%1</source>
        <translation>未找到已配置的 hashcat 可执行文件：
%1</translation>
    </message>
    <message>
        <source>PowerShell script copied to the clipboard</source>
        <translation>PowerShell 脚本已复制到剪贴板</translation>
    </message>
    <message>
        <source>Launch failed</source>
        <translation>启动失败</translation>
    </message>
</context>
<context>
    <name>PathListController</name>
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
</context>
<context>
    <name>PotfileCheck</name>
    <message>
        <source>The hashcat path is not configured.</source>
        <translation>尚未配置 hashcat 的路径。</translation>
    </message>
    <message>
        <source>Enter a hash or select a hash file first.</source>
        <translation>请先输入哈希或选择哈希文件。</translation>
    </message>
    <message>
        <source>Choose a hash type. Potfile checks need an explicit hash type; Auto-detect is best-effort and can miscount. Use Tools → Identify Hash if you do not know the type.</source>
        <translation>请选择哈希类型。potfile 检查需要明确的哈希类型；“自动检测”只是尽力而为，可能计数不准。如果不知道类型，请使用“工具 → 识别哈希”。</translation>
    </message>
    <message>
        <source>The remaining hashes are binary data; the count is unknown.</source>
        <translation>剩余的哈希为二进制数据；数量未知。</translation>
    </message>
    <message>
        <source>Choose a hash type.</source>
        <translation>请选择哈希类型。</translation>
    </message>
    <message>
        <source>hashcat could not load any hashes from the input.</source>
        <translation>hashcat 无法从输入中加载任何哈希。</translation>
    </message>
    <message>
        <source>hashcat could not parse the input for the selected hash type. Check the hash and the hash type.</source>
        <translation>hashcat 无法按所选哈希类型解析输入。请检查哈希和哈希类型。</translation>
    </message>
    <message>
        <source>hashcat could not query the potfile.</source>
        <translation>hashcat 无法查询 potfile。</translation>
    </message>
</context>
<context>
    <name>PotfileExportUi</name>
    <message>
        <source>CSV (*.csv)</source>
        <translation>CSV (*.csv)</translation>
    </message>
    <message>
        <source>TSV (*.tsv *.txt)</source>
        <translation>TSV (*.tsv *.txt)</translation>
    </message>
    <message>
        <source>JSON (*.json)</source>
        <translation>JSON (*.json)</translation>
    </message>
    <message>
        <source>Export potfile results</source>
        <translation>导出 potfile 结果</translation>
    </message>
    <message>
        <source>Export failed</source>
        <translation>导出失败</translation>
    </message>
    <message>
        <source>Choose a path other than the potfile.</source>
        <translation>请选择 potfile 以外的路径。</translation>
    </message>
    <message>
        <source>The destination is named hashcat.potfile. If this is hashcat’s live potfile, exporting overwrites it.

Export anyway?</source>
        <translation>目标文件名为 hashcat.potfile。如果这是 hashcat 正在使用的 potfile，导出会覆盖它。

仍要导出吗？</translation>
    </message>
    <message>
        <source>The file already exists:
%1

Overwrite it?</source>
        <translation>文件已存在：
%1

要覆盖吗？</translation>
    </message>
    <message>
        <source>1 entry</source>
        <translation>1 条记录</translation>
    </message>
    <message>
        <source>%1 entries</source>
        <translation>%1 条记录</translation>
    </message>
    <message>
        <source>Export %1 as %2 to:
%3

The file will contain plaintext passwords. Store it locally and protect it. Spreadsheet applications may evaluate cells beginning with =, +, -, or @ as formulas; open the file as text or disable formula evaluation.</source>
        <translation>将 %1 以 %2 格式导出到：
%3

该文件将包含明文密码。请将其存储在本地并加以保护。电子表格程序可能会将以 =、+、- 或 @ 开头的单元格当作公式；请以文本形式打开该文件，或禁用公式计算。</translation>
    </message>
    <message>
        <source>

Only %1 of %2 entries are included.</source>
        <translation>

仅包含 %1 条（共 %2 条）。</translation>
    </message>
    <message>
        <source>Export plaintexts</source>
        <translation>导出明文</translation>
    </message>
    <message>
        <source>Export</source>
        <translation>导出</translation>
    </message>
    <message>
        <source>Export complete</source>
        <translation>导出完成</translation>
    </message>
    <message>
        <source>Exported %1 to:
%2</source>
        <translation>已将 %1 导出到：
%2</translation>
    </message>
</context>
<context>
    <name>PotfileExporter</name>
    <message>
        <source>Could not open the export file: %1</source>
        <translation>无法打开导出文件：%1</translation>
    </message>
    <message>
        <source>Could not write the export file: %1</source>
        <translation>无法写入导出文件：%1</translation>
    </message>
    <message>
        <source>Could not save the export file: %1</source>
        <translation>无法保存导出文件：%1</translation>
    </message>
</context>
<context>
    <name>PotfileInspectorDialog</name>
    <message>
        <source>Potfile inspector</source>
        <translation>potfile 检查器</translation>
    </message>
    <message>
        <source>Parsed directly; duplicates are exact repeated lines. Separator: ‘:’.</source>
        <translation>直接解析；重复项是完全相同的行。分隔符：“:”。</translation>
    </message>
    <message>
        <source>&amp;Potfile:</source>
        <translation>potfile(&amp;P)：</translation>
    </message>
    <message>
        <source>&amp;Browse…</source>
        <translation>浏览(&amp;B)…</translation>
    </message>
    <message>
        <source>Read the selected potfile again.</source>
        <translation>再次读取所选 potfile。</translation>
    </message>
    <message>
        <source>&amp;Reload</source>
        <translation>重新加载(&amp;R)</translation>
    </message>
    <message>
        <source>Stop reading the potfile.</source>
        <translation>停止读取 potfile。</translation>
    </message>
    <message>
        <source>C&amp;ancel</source>
        <translation>取消(&amp;A)</translation>
    </message>
    <message>
        <source>The prefilled path is a heuristic; hashcat may use another path.</source>
        <translation>预填的路径只是推测；hashcat 可能使用其他路径。</translation>
    </message>
    <message>
        <source>Potfile reading progress</source>
        <translation>potfile 读取进度</translation>
    </message>
    <message>
        <source>&amp;Statistics…</source>
        <translation>统计信息(&amp;S)…</translation>
    </message>
    <message>
        <source>Show aggregate password statistics for the visible entries.</source>
        <translation>显示可见记录的汇总密码统计信息。</translation>
    </message>
    <message>
        <source>&amp;Export…</source>
        <translation>导出(&amp;E)…</translation>
    </message>
    <message>
        <source>Export the visible entries as CSV, TSV or JSON.</source>
        <translation>将可见记录导出为 CSV、TSV 或 JSON。</translation>
    </message>
    <message>
        <source>Close the dialog.</source>
        <translation>关闭对话框。</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>关闭(&amp;C)</translation>
    </message>
    <message>
        <source>Dup</source>
        <translation>重复</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>行</translation>
    </message>
    <message>
        <source>This potfile has no entries.</source>
        <translation>此 potfile 没有记录。</translation>
    </message>
    <message>
        <source>Choose a potfile.</source>
        <translation>请选择一个 potfile。</translation>
    </message>
    <message>
        <source>Select a potfile</source>
        <translation>选择 potfile</translation>
    </message>
    <message>
        <source>Cancelling…</source>
        <translation>正在取消…</translation>
    </message>
    <message>
        <source>Potfile: %1</source>
        <translation>potfile：%1</translation>
    </message>
    <message>
        <source>Potfile not found: %1</source>
        <translation>找不到 potfile：%1</translation>
    </message>
    <message>
        <source>Reading potfile…</source>
        <translation>正在读取 potfile…</translation>
    </message>
    <message>
        <source>Reading cancelled.</source>
        <translation>读取已取消。</translation>
    </message>
    <message>
        <source>dup</source>
        <translation>重复</translation>
    </message>
    <message>
        <source>1 entry</source>
        <translation>1 条记录</translation>
    </message>
    <message>
        <source>%1 entries</source>
        <translation>%1 条记录</translation>
    </message>
    <message>
        <source>1 duplicate line</source>
        <translation>1 行重复</translation>
    </message>
    <message>
        <source>1 duplicate line (at least)</source>
        <translation>1 行重复（至少）</translation>
    </message>
    <message>
        <source>%1 duplicate lines</source>
        <translation>%1 行重复</translation>
    </message>
    <message>
        <source>%1 duplicate lines (at least)</source>
        <translation>%1 行重复（至少）</translation>
    </message>
    <message>
        <source>duplicate count incomplete</source>
        <translation>重复计数不完整</translation>
    </message>
    <message>
        <source>1 malformed line</source>
        <translation>1 行格式错误</translation>
    </message>
    <message>
        <source>%1 malformed lines</source>
        <translation>%1 行格式错误</translation>
    </message>
    <message>
        <source>1 encrypted entry</source>
        <translation>1 条加密记录</translation>
    </message>
    <message>
        <source>%1 encrypted entries</source>
        <translation>%1 条加密记录</translation>
    </message>
    <message>
        <source>1 entry without a password</source>
        <translation>1 条无密码的记录</translation>
    </message>
    <message>
        <source>%1 entries without a password</source>
        <translation>%1 条无密码的记录</translation>
    </message>
    <message>
        <source>1 incomplete line skipped</source>
        <translation>已跳过 1 行不完整内容</translation>
    </message>
    <message>
        <source>%1 incomplete lines skipped</source>
        <translation>已跳过 %1 行不完整内容</translation>
    </message>
    <message>
        <source> · showing first %1 of %2</source>
        <translation> · 仅显示前 %1 条（共 %2 条）</translation>
    </message>
    <message>
        <source> · %1 hidden by filter</source>
        <translation> · %1 条被筛选隐藏</translation>
    </message>
    <message>
        <source>Reveal plaintexts to export the real values.</source>
        <translation>显示明文以导出真实值。</translation>
    </message>
</context>
<context>
    <name>PotfileParser</name>
    <message>
        <source>Could not open the potfile: %1</source>
        <translation>无法打开 potfile：%1</translation>
    </message>
    <message>
        <source>Could not read the potfile: %1</source>
        <translation>无法读取 potfile：%1</translation>
    </message>
</context>
<context>
    <name>PotfileResultsDialog</name>
    <message>
        <source>Cracked hashes</source>
        <translation>已破解的哈希</translation>
    </message>
    <message>
        <source>&amp;Statistics…</source>
        <translation>统计信息(&amp;S)…</translation>
    </message>
    <message>
        <source>Show aggregate password statistics for the visible results.</source>
        <translation>显示可见结果的汇总密码统计信息。</translation>
    </message>
    <message>
        <source>&amp;Export…</source>
        <translation>导出(&amp;E)…</translation>
    </message>
    <message>
        <source>Export the visible results as CSV, TSV or JSON.</source>
        <translation>将可见结果导出为 CSV、TSV 或 JSON。</translation>
    </message>
    <message>
        <source>Close the dialog.</source>
        <translation>关闭对话框。</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>关闭(&amp;C)</translation>
    </message>
    <message>
        <source>Username</source>
        <translation>用户名</translation>
    </message>
    <message>
        <source>Filter hash or username (plaintext after Reveal)</source>
        <translation>筛选哈希或用户名（显示明文后）</translation>
    </message>
    <message>
        <source>No cracked hashes for this input.</source>
        <translation>此输入没有已破解的哈希。</translation>
    </message>
    <message>
        <source>Cracked hashes — mode %1</source>
        <translation>已破解的哈希 — 模式 %1</translation>
    </message>
    <message>
        <source>Potfile: hashcat’s default potfile. hashcat matches potfile entries by hash; the selected hash type narrows the search.</source>
        <translation>potfile：hashcat 的默认 potfile。hashcat 按哈希匹配 potfile 条目；所选哈希类型会缩小搜索范围。</translation>
    </message>
    <message>
        <source>Potfile: %1. hashcat matches potfile entries by hash; the selected hash type narrows the search.</source>
        <translation>potfile：%1。hashcat 按哈希匹配 potfile 条目；所选哈希类型会缩小搜索范围。</translation>
    </message>
    <message>
        <source>%1 of %2 cracked</source>
        <translation>已破解 %1 条（共 %2 条）</translation>
    </message>
    <message>
        <source>%1 cracked (remaining unknown)</source>
        <translation>已破解 %1 个（剩余数量未知）</translation>
    </message>
    <message>
        <source>Check cancelled.</source>
        <translation>检查已取消。</translation>
    </message>
    <message>
        <source>hashcat could not query the potfile.</source>
        <translation>hashcat 无法查询 potfile。</translation>
    </message>
    <message>
        <source> · showing first %1 of %2</source>
        <translation> · 仅显示前 %1 条（共 %2 条）</translation>
    </message>
    <message>
        <source>1 unreadable result line skipped.</source>
        <translation>已跳过 1 行无法读取的结果。</translation>
    </message>
    <message>
        <source>%1 unreadable result lines skipped.</source>
        <translation>已跳过 %1 行无法读取的结果。</translation>
    </message>
    <message>
        <source> · %1 encrypted</source>
        <translation> · %1 个加密</translation>
    </message>
    <message>
        <source> · includes a binary hash file</source>
        <translation> · 包含一个二进制哈希文件</translation>
    </message>
    <message>
        <source> · %1 hidden by filter</source>
        <translation> · %1 条被筛选隐藏</translation>
    </message>
    <message>
        <source>Reveal plaintexts to export the real values.</source>
        <translation>显示明文以导出真实值。</translation>
    </message>
</context>
<context>
    <name>PotfileStatisticsDialog</name>
    <message>
        <source>Password statistics</source>
        <translation>密码统计信息</translation>
    </message>
    <message>
        <source>Password length distribution</source>
        <translation>密码长度分布</translation>
    </message>
    <message>
        <source>Length</source>
        <translation>长度</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation>密码</translation>
    </message>
    <message>
        <source>Close the dialog.</source>
        <translation>关闭对话框。</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>关闭(&amp;C)</translation>
    </message>
    <message>
        <source>1 entry</source>
        <translation>1 条记录</translation>
    </message>
    <message>
        <source>%1 entries</source>
        <translation>%1 条记录</translation>
    </message>
    <message>
        <source>1 password analyzed</source>
        <translation>已分析 1 个密码</translation>
    </message>
    <message>
        <source>%1 passwords analyzed</source>
        <translation>已分析 %1 个密码</translation>
    </message>
    <message>
        <source>1 duplicate line</source>
        <translation>1 行重复</translation>
    </message>
    <message>
        <source>%1 duplicate lines</source>
        <translation>%1 行重复</translation>
    </message>
    <message>
        <source>duplicate lines: unknown</source>
        <translation>重复行：未知</translation>
    </message>
    <message>
        <source>%1 · %2</source>
        <translation>%1 · %2</translation>
    </message>
    <message>
        <source>%1 empty · %2 encrypted · %3 · %4 unique plaintexts</source>
        <translation>%1 个空密码 · %2 个加密 · %3 · %4 个唯一明文</translation>
    </message>
    <message>
        <source>Length: —</source>
        <translation>长度：—</translation>
    </message>
    <message>
        <source>Length: min %1 · max %2 · average %3</source>
        <translation>长度：最小 %1 · 最大 %2 · 平均 %3</translation>
    </message>
    <message>
        <source>Based on the visible %1 of %2 entries.</source>
        <translation>基于可见记录中的 %1 条（共 %2 条）。</translation>
    </message>
    <message>
        <source>No plaintext passwords to analyze.</source>
        <translation>没有可分析的明文密码。</translation>
    </message>
    <message>
        <source>Passwords containing:
lowercase: %1 (%2%)
uppercase: %3 (%4%)
digits: %5 (%6%)
symbols: %7 (%8%)
whitespace: %9 (%10%)
non-ASCII: %11 (%12%)</source>
        <translation>密码中包含：
小写字母：%1 (%2%)
大写字母：%3 (%4%)
数字：%5 (%6%)
符号：%7 (%8%)
空白：%9 (%10%)
非 ASCII：%11 (%12%)</translation>
    </message>
    <message>
        <source>Calculating statistics…</source>
        <translation>正在计算统计信息…</translation>
    </message>
</context>
<context>
    <name>PotfileTableView</name>
    <message>
        <source>Filter hash (plaintext after Reveal)</source>
        <translation>筛选哈希（显示明文后）</translation>
    </message>
    <message>
        <source>Plaintext is searched only after Reveal plaintexts is enabled.</source>
        <translation>只有在启用“显示明文”后才会搜索明文。</translation>
    </message>
    <message>
        <source>Copy selected</source>
        <translation>复制所选内容</translation>
    </message>
    <message>
        <source>Reveal plaintexts to copy the real values.</source>
        <translation>显示明文以复制真实值。</translation>
    </message>
    <message>
        <source>Copy all</source>
        <translation>复制全部</translation>
    </message>
    <message>
        <source>Reveal plaintexts</source>
        <translation>显示明文</translation>
    </message>
    <message>
        <source>Show or mask plaintext values.</source>
        <translation>显示或屏蔽明文值。</translation>
    </message>
    <message>
        <source>Hash</source>
        <translation>哈希</translation>
    </message>
    <message>
        <source>Plaintext</source>
        <translation>明文</translation>
    </message>
    <message>
        <source>Copied 1 row</source>
        <translation>已复制 1 行</translation>
    </message>
    <message>
        <source>Copied %1 rows</source>
        <translation>已复制 %1 行</translation>
    </message>
    <message>
        <source>Copy selected rows</source>
        <translation>复制所选行</translation>
    </message>
    <message>
        <source>Copies tab-separated rows. Encrypted rows copy the stored $HCENC$ token.</source>
        <translation>复制以制表符分隔的行。加密行复制存储的 $HCENC$ 标记。</translation>
    </message>
    <message>
        <source>Hide plaintexts</source>
        <translation>隐藏明文</translation>
    </message>
    <message>
        <source>Binary hash file; hashcat reports the file name.</source>
        <translation>二进制哈希文件；hashcat 会报告文件名。</translation>
    </message>
    <message>
        <source>No rows match the filter.</source>
        <translation>没有符合筛选条件的行。</translation>
    </message>
    <message>
        <source>(encrypted)</source>
        <translation>（已加密）</translation>
    </message>
    <message>
        <source>(empty)</source>
        <translation>（空）</translation>
    </message>
    <message>
        <source>(file) %1</source>
        <translation>（文件）%1</translation>
    </message>
</context>
<context>
    <name>SessionTransfer</name>
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
</context>
<context>
    <name>SettingsDialog</name>
    <message>
        <source>Settings</source>
        <translation>设置</translation>
    </message>
    <message>
        <source>hashcat executable</source>
        <translation>hashcat 可执行文件</translation>
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
        <source>Path to the hashcat executable, or a bare name such as hashcat to look up on PATH.</source>
        <translation>hashcat 可执行文件的路径，或在 PATH 中查找的简单名称（如 hashcat）。</translation>
    </message>
    <message>
        <source>Browse for the hashcat executable.</source>
        <translation>浏览选择 hashcat 可执行文件。</translation>
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
        <source>Interface language. Takes effect after a restart.</source>
        <translation>界面语言。重启后生效。</translation>
    </message>
    <message>
        <source>&amp;Color scheme</source>
        <translation>配色方案(&amp;C)</translation>
    </message>
    <message>
        <source>The file the last session is stored in.</source>
        <translation>上次会话保存到的文件。</translation>
    </message>
    <message>
        <source>Open the folder the session is stored in.</source>
        <translation>打开保存会话的文件夹。</translation>
    </message>
    <message>
        <source>The file the settings are stored in.</source>
        <translation>设置保存到的文件。</translation>
    </message>
    <message>
        <source>Open the folder the settings are stored in.</source>
        <translation>打开保存设置的文件夹。</translation>
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
        <source>Sho&amp;w the exact command in the terminal</source>
        <translation>在终端中显示确切命令(&amp;W)</translation>
    </message>
    <message>
        <source>Print the exact command hashcat receives in the launch terminal, so a mangled value is visible.</source>
        <translation>在启动终端中打印 hashcat 收到的确切命令，以便发现被破坏的值。</translation>
    </message>
    <message>
        <source>Session and files</source>
        <translation>会话和文件</translation>
    </message>
    <message>
        <source>Restore the session saved when the application last closed.</source>
        <translation>恢复应用程序上次关闭时保存的会话。</translation>
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
        <source>Open folder</source>
        <translation>打开文件夹</translation>
    </message>
    <message>
        <source>Settings &amp;file</source>
        <translation>设置文件(&amp;F)</translation>
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
        <source>The terminal hashcat is started in.</source>
        <translation>启动 hashcat 所用的终端。</translation>
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
        <source>Settings saved</source>
        <translation>设置已保存</translation>
    </message>
    <message>
        <source>The old settings file was moved to:
%1</source>
        <translation>旧的设置文件已移动到：
%1</translation>
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
        <source>Settings not saved</source>
        <translation>设置未保存</translation>
    </message>
    <message>
        <source>Move the file aside and save these values? It is renamed, never deleted.</source>
        <translation>要将该文件移开并保存这些值吗？文件只会被重命名，不会被删除。</translation>
    </message>
    <message>
        <source>Move File Aside and Save</source>
        <translation>移开文件并保存</translation>
    </message>
    <message>
        <source>Keep File</source>
        <translation>保留文件</translation>
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
    <name>SettingsManager</name>
    <message>
        <source>The settings could not be written to “%1”. Check the file permissions.</source>
        <translation>无法将设置写入“%1”。请检查文件权限。</translation>
    </message>
    <message>
        <source>The settings file “%1” is not in a readable format.</source>
        <translation>设置文件“%1”的格式无法读取。</translation>
    </message>
    <message>
        <source>The settings file could not be moved to “%1”.</source>
        <translation>无法将设置文件移动到“%1”。</translation>
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
        <source>The section names a version this build cannot understand.</source>
        <translation>该部分指明了此构建无法理解的版本。</translation>
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
        <source>The “%1” section names a version that is not a number.</source>
        <translation>“%1”部分指定的版本不是数字。</translation>
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
    <name>TerminalCatalog</name>
    <message>
        <source>No supported terminal was found. ptyxis is installed, but it changes “$” values before hashcat sees them, so it is not offered. Install another terminal; see the FAQ.</source>
        <translation>未找到受支持的终端。ptyxis 已安装，但它会在 hashcat 看到之前修改“$”值，因此不予提供。请安装其他终端；参见常见问题。</translation>
    </message>
    <message>
        <source>No supported terminal was found on this system. hashcat-gui needs one to show the hashcat output.</source>
        <translation>此系统上未找到受支持的终端。hashcat-gui 需要终端来显示 hashcat 的输出。</translation>
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
