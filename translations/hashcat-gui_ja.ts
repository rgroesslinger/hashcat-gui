<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ja" sourcelanguage="en">
<context>
    <name>AboutDialog</name>
    <message>
        <source>About</source>
        <translation>hashcat-gui について</translation>
    </message>
    <message>
        <source>Graphical user interface for hashcat, the password recovery utility</source>
        <translation>パスワード回復ユーティリティ hashcat のグラフィカルユーザーインターフェース</translation>
    </message>
    <message>
        <source>Version information</source>
        <translation>バージョン情報</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>不明</translation>
    </message>
    <message>
        <source>Updates</source>
        <translation>アップデート</translation>
    </message>
    <message>
        <source>Checking for updates…</source>
        <translation>アップデートを確認しています…</translation>
    </message>
    <message>
        <source>System information</source>
        <translation>システム情報</translation>
    </message>
    <message>
        <source>Operating system</source>
        <translation>オペレーティングシステム</translation>
    </message>
    <message>
        <source>Links</source>
        <translation>リンク</translation>
    </message>
    <message>
        <source>License</source>
        <translation>ライセンス</translation>
    </message>
    <message>
        <source>Copy the application, toolkit, system and hashcat versions to the clipboard</source>
        <translation>アプリケーション、ツールキット、システム、hashcat のバージョンをクリップボードにコピーします</translation>
    </message>
    <message>
        <source>Copy details</source>
        <translation>詳細をコピー</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Copied to the clipboard</source>
        <translation>クリップボードにコピーしました</translation>
    </message>
    <message>
        <source>Project page</source>
        <translation>プロジェクトページ</translation>
    </message>
    <message>
        <source>Report an issue</source>
        <translation>問題を報告</translation>
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
        <translation>hashcat-gui は %1 のもとでライセンスされています。</translation>
    </message>
    <message>
        <source>GNU GPL v3.0 or later</source>
        <translation>GNU GPL v3.0 以降</translation>
    </message>
    <message>
        <source>Not configured</source>
        <translation>未設定</translation>
    </message>
    <message>
        <source>Error: %1</source>
        <translation>エラー: %1</translation>
    </message>
    <message>
        <source>A newer version is available: %1.</source>
        <translation>新しいバージョンが利用可能です: %1。</translation>
    </message>
    <message>
        <source>Release page</source>
        <translation>リリースページ</translation>
    </message>
    <message>
        <source>No updates available.</source>
        <translation>利用可能なアップデートはありません。</translation>
    </message>
    <message>
        <source>Could not check for updates.</source>
        <translation>アップデートを確認できませんでした。</translation>
    </message>
    <message>
        <source>Could not check for updates: %1</source>
        <translation>アップデートを確認できませんでした: %1</translation>
    </message>
</context>
<context>
    <name>HashcatInfoParser</name>
    <message>
        <source>Invalid JSON returned from hashcat.</source>
        <translation>hashcat が無効な JSON を返しました。</translation>
    </message>
    <message>
        <source>hashcat did not report any hash types.</source>
        <translation>hashcat はハッシュタイプを報告しませんでした。</translation>
    </message>
</context>
<context>
    <name>HelperUtils</name>
    <message>
        <source>The hashcat path is not configured.</source>
        <translation>hashcat のパスが設定されていません。</translation>
    </message>
    <message>
        <source>Failed to start hashcat
%1</source>
        <translation>hashcat を起動できませんでした
%1</translation>
    </message>
    <message>
        <source>hashcat timed out
%1</source>
        <translation>hashcat がタイムアウトしました
%1</translation>
    </message>
    <message>
        <source>No supported terminal was found on this system. hashcat-gui needs one to show the hashcat output.</source>
        <translation>サポートされているターミナルがこのシステムで見つかりませんでした。hashcat の出力を表示するには、hashcat-gui にターミナルが必要です。</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <source>Hash file or hash:</source>
        <translation>ハッシュファイルまたはハッシュ:</translation>
    </message>
    <message>
        <source>The hash, a file of hashes, or a .hc22000/.hccapx WPA capture.</source>
        <translation>ハッシュ、ハッシュのファイル、または .hc22000/.hccapx 形式の WPA キャプチャ。</translation>
    </message>
    <message>
        <source>Choose a file of hashes.</source>
        <translation>ハッシュのファイルを選択します。</translation>
    </message>
    <message>
        <source>Salt is hex</source>
        <translation>ソルトは 16 進数</translation>
    </message>
    <message>
        <source>Rewrite the hash file without cracked hashes, including potfile-known ones; the potfile is untouched. Refused in attack mode 9.</source>
        <translation>解読済みのハッシュ (potfile に記録済みのものを含む) を除いてハッシュファイルを書き直します。potfile は変更されません。攻撃モード 9 では拒否されます。</translation>
    </message>
    <message>
        <source>Remove hash from hash list once it is cracked</source>
        <translation>解読したハッシュをハッシュリストから削除する</translation>
    </message>
    <message>
        <source>Ignore the username before the mode’s separator (usually “:”), so accounts sharing a hash are cracked once.</source>
        <translation>モードの区切り文字 (通常は「:」) より前のユーザー名を無視し、同じハッシュを共有するアカウントを 1 回だけ解読します。</translation>
    </message>
    <message>
        <source>Hash recipe, e.g. “md5(md5(salt) . pass)”. Wrap it in the shell’s outer quotes and put any inner strings in the other kind of quotes; PowerShell needs a backtick before $.</source>
        <translation>ハッシュレシピ (例: 「md5(md5(salt) . pass)」)。シェルの外側の引用符でレシピ全体を囲み、内側の文字列にはもう一方の引用符を使います。PowerShell では $ の前にバッククォートが必要です。</translation>
    </message>
    <message>
        <source>Move item up</source>
        <translation>項目を上へ移動</translation>
    </message>
    <message>
        <source>Move selected wordlist up</source>
        <translation>選択したワードリストを上へ移動</translation>
    </message>
    <message>
        <source>Move item down</source>
        <translation>項目を下へ移動</translation>
    </message>
    <message>
        <source>Move selected wordlist down</source>
        <translation>選択したワードリストを下へ移動</translation>
    </message>
    <message>
        <source>Add one or more wordlist files. Each checked row is passed to hashcat.</source>
        <translation>ワードリストのファイルを 1 つ以上追加します。チェックされた各行が hashcat に渡されます。</translation>
    </message>
    <message>
        <source>Remove the selected wordlists.</source>
        <translation>選択したワードリストを削除します。</translation>
    </message>
    <message>
        <source>Remove wordlists</source>
        <translation>ワードリストを削除</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>削除</translation>
    </message>
    <message>
        <source>Wordlist is hex</source>
        <translation>ワードリストは 16 進数</translation>
    </message>
    <message>
        <source>Encoding from:</source>
        <translation>エンコード元:</translation>
    </message>
    <message>
        <source>Encoding to:</source>
        <translation>エンコード先:</translation>
    </message>
    <message>
        <source>Space-separated key=value settings for attack mode 9, e.g. phases=words,rules or rules.rulefile=…. Only used without a wordlist.</source>
        <translation>攻撃モード 9 用の、スペース区切りの key=value 設定 (例: phases=words,rules や rules.rulefile=…)。ワードリストを使用しない場合のみ有効です。</translation>
    </message>
    <message>
        <source>A ruleset file, a folder with Grammar/grammar.txt, or a bundled name. Unchecked rows are skipped; none runs default-passwords.</source>
        <translation>ルールセットのファイル、Grammar/grammar.txt を含むフォルダー、または同梱の名前。チェックされていない行はスキップされ、1 つもチェックされていない場合は default-passwords が実行されます。</translation>
    </message>
    <message>
        <source>Add one or more PCFG ruleset files.</source>
        <translation>PCFG ルールセットのファイルを 1 つ以上追加します。</translation>
    </message>
    <message>
        <source>Add PCFG rulesets</source>
        <translation>PCFG ルールセットを追加</translation>
    </message>
    <message>
        <source>Remove the selected rulesets.</source>
        <translation>選択したルールセットを削除します。</translation>
    </message>
    <message>
        <source>Remove rulesets</source>
        <translation>ルールセットを削除</translation>
    </message>
    <message>
        <source>Move selected ruleset up</source>
        <translation>選択したルールセットを上へ移動</translation>
    </message>
    <message>
        <source>Move selected ruleset down</source>
        <translation>選択したルールセットを下へ移動</translation>
    </message>
    <message>
        <source>One plain number per ruleset, colon-separated, e.g. 2:1; the count must match the number of rulesets.</source>
        <translation>ルールセットごとに 1 つの数値をコロンで区切って指定します (例: 2:1)。数値の個数はルールセットの数と一致する必要があります。</translation>
    </message>
    <message>
        <source>Maximum candidate length the kernel builds, in 4-byte words: 0 (auto), 16 or 32.</source>
        <translation>カーネルが構築する候補の最大長 (4 バイトワード単位): 0 (自動)、16、または 32。</translation>
    </message>
    <message>
        <source>1 steps to the next base word where the order allows instead of unranking it; the candidates are identical, only the fast path is off.</source>
        <translation>1 にすると、順序が許す場合はアンランキングせずに次の基本ワードへ進みます。候補は同一で、高速パスのみ無効になります。</translation>
    </message>
    <message>
        <source>Only enumerate candidates this mask admits; excludes omen=1, and hintaccount unless hint words come from hintwords or hintfile.</source>
        <translation>このマスクが許容する候補のみを列挙します。omen=1 は除外され、ヒントワードが hintwords または hintfile から取得される場合を除き hintaccount も除外されます。</translation>
    </message>
    <message>
        <source>Space-separated key=value settings: omen, cache, lookup, pwmin, pwmax, hintaccount, hintwords, hintfile, hintrank, hintrepeat. Rulesets go first.</source>
        <translation>スペース区切りの key=value 設定: omen、cache、lookup、pwmin、pwmax、hintaccount、hintwords、hintfile、hintrank、hintrepeat。ルールセットは先頭に指定します。</translation>
    </message>
    <message>
        <source>Add one or more table files.</source>
        <translation>テーブルのファイルを 1 つ以上追加します。</translation>
    </message>
    <message>
        <source>Remove the selected tables.</source>
        <translation>選択したテーブルを削除します。</translation>
    </message>
    <message>
        <source>Remove tables</source>
        <translation>テーブルを削除</translation>
    </message>
    <message>
        <source>1 (default) keeps a token unchanged as a choice, 0 drops it so the table only yields variations. identity=0 excludes single=1.</source>
        <translation>1 (既定) ではトークンを選択肢としてそのまま残し、0 では除外してテーブルがバリエーションのみを生成します。identity=0 は single=1 を除外します。</translation>
    </message>
    <message>
        <source>1 reads the wordlist as patterns: no word is returned unchanged and unmatched words are dropped. 0 (default) keeps them.</source>
        <translation>1 にするとワードリストをパターンとして読み込みます。変更されていない単語は返されず、一致しない単語は破棄されます。0 (既定) ではそれらが保持されます。</translation>
    </message>
    <message>
        <source>single=1: one substitution per candidate instead of the cross product; fewer candidates, more host work on fast hashes.</source>
        <translation>single=1: 直積ではなく候補ごとに 1 回の置換を行います。候補は少なくなりますが、高速なハッシュではホストの処理が増えます。</translation>
    </message>
    <message>
        <source>Keep only the first N replacements of each source word; 0 means no limit. Applies to single=1 too.</source>
        <translation>各元単語の最初の N 個の置換のみを保持します。0 は無制限を意味します。single=1 にも適用されます。</translation>
    </message>
    <message>
        <source>How much of a word’s cross product the host enumerates before giving up; 0 means no limit.</source>
        <translation>ホストが断念するまでに列挙する、単語の直積の量。0 は無制限を意味します。</translation>
    </message>
    <message>
        <source>Key=value settings separated by spaces; everything after the first one is a setting, so tables come first.</source>
        <translation>スペース区切りの key=value 設定。最初の 1 つ以降はすべて設定として扱われるため、テーブルを先に指定します。</translation>
    </message>
    <message>
        <source>Feed name or path. A slash means a path; otherwise hashcat tries feed_&lt;name&gt;, rust_&lt;name&gt;, then &lt;name&gt; in its feeds directory.</source>
        <translation>フィードの名前またはパス。スラッシュはパスを意味します。それ以外の場合、hashcat はフィードディレクトリで feed_&lt;name&gt;、rust_&lt;name&gt;、&lt;name&gt; の順に試します。</translation>
    </message>
    <message>
        <source>Browse for a feed plugin instead of naming it.</source>
        <translation>名前を指定する代わりに、フィードプラグインを参照して選択します。</translation>
    </message>
    <message>
        <source>Browse for a feed plugin</source>
        <translation>フィードプラグインを参照</translation>
    </message>
    <message>
        <source>Argument passed to the feed; meaning and count depend on the feed (the wordlist feed takes wordlist paths).</source>
        <translation>フィードに渡す引数。意味と数はフィードによって異なります (wordlist フィードはワードリストのパスを受け取ります)。</translation>
    </message>
    <message>
        <source>Add an argument for the feed, in order.</source>
        <translation>フィードの引数を順番に追加します。</translation>
    </message>
    <message>
        <source>Remove the selected feed arguments.</source>
        <translation>選択したフィード引数を削除します。</translation>
    </message>
    <message>
        <source>Remove feed arguments</source>
        <translation>フィード引数を削除</translation>
    </message>
    <message>
        <source>Move selected feed argument up</source>
        <translation>選択したフィード引数を上へ移動</translation>
    </message>
    <message>
        <source>Move selected feed argument down</source>
        <translation>選択したフィード引数を下へ移動</translation>
    </message>
    <message>
        <source>Read rules from the files below instead of generating them.</source>
        <translation>ルールを生成する代わりに、下のファイルから読み込みます。</translation>
    </message>
    <message>
        <source>Rules file 1:</source>
        <translation>ルールファイル 1:</translation>
    </message>
    <message>
        <source>Choose the rules file for slot 1.</source>
        <translation>スロット 1 のルールファイルを選択します。</translation>
    </message>
    <message>
        <source>Open rules file 1</source>
        <translation>ルールファイル 1 を開く</translation>
    </message>
    <message>
        <source>Rules file 2:</source>
        <translation>ルールファイル 2:</translation>
    </message>
    <message>
        <source>Choose the rules file for slot 2.</source>
        <translation>スロット 2 のルールファイルを選択します。</translation>
    </message>
    <message>
        <source>Open rules file 2</source>
        <translation>ルールファイル 2 を開く</translation>
    </message>
    <message>
        <source>Rules file 3:</source>
        <translation>ルールファイル 3:</translation>
    </message>
    <message>
        <source>Choose the rules file for slot 3.</source>
        <translation>スロット 3 のルールファイルを選択します。</translation>
    </message>
    <message>
        <source>Open rules file 3</source>
        <translation>ルールファイル 3 を開く</translation>
    </message>
    <message>
        <source>Number of random rules hashcat generates (-g). Only used with Generate rules; no rules file is read.</source>
        <translation>hashcat が生成するランダムルールの数 (-g)。「ルールを生成」の場合のみ使用され、ルールファイルは読み込まれません。</translation>
    </message>
    <message>
        <source>Treat every -r file as one list: rule counts add instead of multiplying.</source>
        <translation>すべての -r ファイルを 1 つのリストとして扱います。ルール数は乗算ではなく加算されます。</translation>
    </message>
    <message>
        <source>Wordlist rules</source>
        <translation>ワードリストのルール</translation>
    </message>
    <message>
        <source>Left rule:</source>
        <translation>左ルール:</translation>
    </message>
    <message>
        <source>Right rule:</source>
        <translation>右ルール:</translation>
    </message>
    <message>
        <source>Interpret the mask and custom charsets as hexadecimal bytes.</source>
        <translation>マスクとカスタム文字セットを 16 進バイトとして解釈します。</translation>
    </message>
    <message>
        <source>Charset is hex</source>
        <translation>文字セットは 16 進数</translation>
    </message>
    <message>
        <source>Ctrl+Shift+I</source>
        <translation>Ctrl+Shift+I</translation>
    </message>
    <message>
        <source>Increment mask length</source>
        <translation>マスク長のインクリメント</translation>
    </message>
    <message>
        <source>Min:</source>
        <translation>最小:</translation>
    </message>
    <message>
        <source>Max:</source>
        <translation>最大:</translation>
    </message>
    <message>
        <source>Use hashcat’s optimized kernels (-O); they cap the maximum password length.</source>
        <translation>hashcat の最適化カーネルを使用します (-O)。パスワードの最大長が制限されます。</translation>
    </message>
    <message>
        <source>Pin hashcat to these CPU devices, separated by commas. Leave empty to let hashcat decide.</source>
        <translation>hashcat をこれらの CPU デバイスに固定します (カンマ区切り)。空欄の場合は hashcat が決定します。</translation>
    </message>
    <message>
        <source>Devices to use, separated by commas. Leave empty for all devices.</source>
        <translation>使用するデバイス (カンマ区切り)。空欄の場合はすべてのデバイスを使用します。</translation>
    </message>
    <message>
        <source>Abort above</source>
        <translation>中止する温度</translation>
    </message>
    <message>
        <source>Outfile</source>
        <translation>出力ファイル</translation>
    </message>
    <message>
        <source>Save…</source>
        <translation>保存…</translation>
    </message>
    <message>
        <source>Write the outfile as JSON. The outfile format is ignored.</source>
        <translation>出力ファイルを JSON で書き出します。出力ファイルの形式は無視されます。</translation>
    </message>
    <message>
        <source>Write JSON</source>
        <translation>JSON を書き出す</translation>
    </message>
    <message>
        <source>Do not write recovered hashes to the potfile.</source>
        <translation>復元したハッシュを potfile に書き込みません。</translation>
    </message>
    <message>
        <source>Load Example Session</source>
        <translation>サンプルセッションを読み込む</translation>
    </message>
    <message>
        <source>Reset Session</source>
        <translation>セッションをリセット</translation>
    </message>
    <message>
        <source>Mode:</source>
        <translation>モード:</translation>
    </message>
    <message>
        <source>Attack</source>
        <translation>攻撃</translation>
    </message>
    <message>
        <source>Open…</source>
        <translation>開く…</translation>
    </message>
    <message>
        <source>Assume the salt is given in hexadecimal.</source>
        <translation>ソルトが 16 進数で指定されているものとして扱います。</translation>
    </message>
    <message>
        <source>Attack mode. Each choice reveals its own options below.</source>
        <translation>攻撃モード。選択に応じて、下に固有のオプションが表示されます。</translation>
    </message>
    <message>
        <source>Hash type:</source>
        <translation>ハッシュタイプ:</translation>
    </message>
    <message>
        <source>Hash recipe:</source>
        <translation>ハッシュレシピ:</translation>
    </message>
    <message>
        <source>Add files…</source>
        <translation>ファイルを追加…</translation>
    </message>
    <message>
        <source>hashcat reads every file directly inside the folder as one wordlist.</source>
        <translation>hashcat は、フォルダー直下のすべてのファイルを 1 つのワードリストとして読み込みます。</translation>
    </message>
    <message>
        <source>Add folder…</source>
        <translation>フォルダーを追加…</translation>
    </message>
    <message>
        <source>Only checked rows are used. A folder is one wordlist: hashcat reads the files directly inside it.</source>
        <translation>チェックされた行のみ使用されます。フォルダーは 1 つのワードリストとして扱われ、hashcat はその直下のファイルを読み込みます。</translation>
    </message>
    <message>
        <source>Assume words in the wordlist are given in hexadecimal.</source>
        <translation>ワードリスト内の単語が 16 進数で指定されているものとして扱います。</translation>
    </message>
    <message>
        <source>Settings (no wordlist):</source>
        <translation>設定 (ワードリストなし):</translation>
    </message>
    <message>
        <source>PCFG</source>
        <translation>PCFG</translation>
    </message>
    <message>
        <source>A ruleset folder needs a Grammar/grammar.txt.</source>
        <translation>ルールセットフォルダーには Grammar/grammar.txt が必要です。</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Scale:</translation>
    </message>
    <message>
        <source>Quantization steps per bit (1–64)</source>
        <translation>ビットあたりの量子化ステップ数 (1–64)</translation>
    </message>
    <message>
        <source>Costmax:</source>
        <translation>Costmax:</translation>
    </message>
    <message>
        <source>Highest cost level to enumerate (1–64)</source>
        <translation>列挙するコストレベルの上限 (1–64)</translation>
    </message>
    <message>
        <source>Weights:</source>
        <translation>Weights:</translation>
    </message>
    <message>
        <source>Threads:</source>
        <translation>スレッド:</translation>
    </message>
    <message>
        <source>Cores that generate base words (0–16); 0 runs on the calling thread.</source>
        <translation>基本ワードを生成するコア数 (0–16)。0 の場合は呼び出し元のスレッドで実行します。</translation>
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
        <translation>内部ループがまたがることのできるビット数 (0–27)</translation>
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
        <translation>1.0–64.0。小数点はロケールの区切り文字ではなく、ドットです。</translation>
    </message>
    <message>
        <source>Walk:</source>
        <translation>Walk:</translation>
    </message>
    <message>
        <source>Mask:</source>
        <translation>マスク:</translation>
    </message>
    <message>
        <source>Extra settings (key=value):</source>
        <translation>追加の設定 (key=value):</translation>
    </message>
    <message>
        <source>Table</source>
        <translation>テーブル</translation>
    </message>
    <message>
        <source>Every checked row after the wordlist is a table.</source>
        <translation>ワードリストの後にあるチェックされた行はすべてテーブルです。</translation>
    </message>
    <message>
        <source>Add tables…</source>
        <translation>テーブルを追加…</translation>
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
        <translation>汎用フィード</translation>
    </message>
    <message>
        <source>Feed:</source>
        <translation>Feed:</translation>
    </message>
    <message>
        <source>wordlist, stdin, random, association, pcfg, table, hybrid, mask, or a feed path</source>
        <translation>wordlist、stdin、random、association、pcfg、table、hybrid、mask、またはフィードのパス</translation>
    </message>
    <message>
        <source>Browse…</source>
        <translation>参照…</translation>
    </message>
    <message>
        <source>Argument:</source>
        <translation>引数:</translation>
    </message>
    <message>
        <source>Add</source>
        <translation>追加</translation>
    </message>
    <message>
        <source>Arguments the feed reads, in order; only checked rows are used.</source>
        <translation>フィードが順に読み込む引数。チェックされた行のみ使用されます。</translation>
    </message>
    <message>
        <source>Rules</source>
        <translation>ルール</translation>
    </message>
    <message>
        <source>Use rules files:</source>
        <translation>ルールファイルを使用:</translation>
    </message>
    <message>
        <source>Enable rules file slot 1 for this run.</source>
        <translation>今回の実行でルールファイルのスロット 1 を有効にします。</translation>
    </message>
    <message>
        <source>Rules file for slot 1; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>スロット 1 のルールファイル。各単語に適用されます。スロットを使用しない場合は空欄のままにします。</translation>
    </message>
    <message>
        <source>Enable rules file slot 2 for this run.</source>
        <translation>今回の実行でルールファイルのスロット 2 を有効にします。</translation>
    </message>
    <message>
        <source>Rules file for slot 2; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>スロット 2 のルールファイル。各単語に適用されます。スロットを使用しない場合は空欄のままにします。</translation>
    </message>
    <message>
        <source>Enable rules file slot 3 for this run.</source>
        <translation>今回の実行でルールファイルのスロット 3 を有効にします。</translation>
    </message>
    <message>
        <source>Rules file for slot 3; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>スロット 3 のルールファイル。各単語に適用されます。スロットを使用しない場合は空欄のままにします。</translation>
    </message>
    <message>
        <source>Generate the rules automatically instead of reading them from a file.</source>
        <translation>ルールをファイルから読み込む代わりに、自動的に生成します。</translation>
    </message>
    <message>
        <source>Generate rules:</source>
        <translation>ルールを生成:</translation>
    </message>
    <message>
        <source>Join -r files into one list</source>
        <translation>-r ファイルを 1 つのリストに結合する</translation>
    </message>
    <message>
        <source>Single rule applied to each word from the left wordlist.</source>
        <translation>左側のワードリストの各単語に適用される単一のルール。</translation>
    </message>
    <message>
        <source>Single rule applied to each word from the right wordlist.</source>
        <translation>右側のワードリストの各単語に適用される単一のルール。</translation>
    </message>
    <message>
        <source>Mask</source>
        <translation>マスク</translation>
    </message>
    <message>
        <source>Built-in charsets:</source>
        <translation>組み込み文字セット:</translation>
    </message>
    <message>
        <source>Custom charsets</source>
        <translation>カスタム文字セット</translation>
    </message>
    <message>
        <source>Charset 1:</source>
        <translation>文字セット 1:</translation>
    </message>
    <message>
        <source>Charset 2:</source>
        <translation>文字セット 2:</translation>
    </message>
    <message>
        <source>Charset 3:</source>
        <translation>文字セット 3:</translation>
    </message>
    <message>
        <source>Charset 4:</source>
        <translation>文字セット 4:</translation>
    </message>
    <message>
        <source>Session and limits</source>
        <translation>セッションと制限</translation>
    </message>
    <message>
        <source>Session name:</source>
        <translation>セッション名:</translation>
    </message>
    <message>
        <source>Name hashcat uses for its restore file and status output.</source>
        <translation>hashcat が復元ファイルとステータス出力に使用する名前です。</translation>
    </message>
    <message>
        <source>Runtime limit:</source>
        <translation>実行時間の上限:</translation>
    </message>
    <message>
        <source>unlimited</source>
        <translation>無制限</translation>
    </message>
    <message>
        <source>Skip:</source>
        <translation>スキップ:</translation>
    </message>
    <message>
        <source>Limit:</source>
        <translation>上限:</translation>
    </message>
    <message>
        <source>Keep updating the run status (speed, progress, ETA) instead of printing one final line.</source>
        <translation>最終行を 1 回だけ出力する代わりに、実行ステータス (速度、進捗、ETA) を更新し続けます。</translation>
    </message>
    <message>
        <source>Update status automatically</source>
        <translation>ステータスを自動的に更新する</translation>
    </message>
    <message>
        <source>Seconds between status updates; only used when automatic status is enabled.</source>
        <translation>ステータス更新の間隔 (秒)。自動ステータスが有効な場合のみ使用されます。</translation>
    </message>
    <message>
        <source>Ignore warnings and start hashcat anyway. Use with care.</source>
        <translation>警告を無視して hashcat を開始します。注意して使用してください。</translation>
    </message>
    <message>
        <source>Force (ignore warnings)</source>
        <translation>強制 (警告を無視)</translation>
    </message>
    <message>
        <source>Performance</source>
        <translation>パフォーマンス</translation>
    </message>
    <message>
        <source>Cache path:</source>
        <translation>キャッシュパス:</translation>
    </message>
    <message>
        <source>Directory hashcat keeps its cache in. Must exist.</source>
        <translation>hashcat がキャッシュを保存するディレクトリ。存在している必要があります。</translation>
    </message>
    <message>
        <source>Choose the cache directory.</source>
        <translation>キャッシュディレクトリを選択します。</translation>
    </message>
    <message>
        <source>Choose cache directory</source>
        <translation>キャッシュディレクトリを選択</translation>
    </message>
    <message>
        <source>Resources</source>
        <translation>リソース</translation>
    </message>
    <message>
        <source>CPU affinity:</source>
        <translation>CPU アフィニティ:</translation>
    </message>
    <message>
        <source>Backend devices:</source>
        <translation>バックエンドデバイス:</translation>
    </message>
    <message>
        <source>Device types:</source>
        <translation>デバイスタイプ:</translation>
    </message>
    <message>
        <source>Abort when any backend device exceeds this temperature in °C; 0 disables it.</source>
        <translation>いずれかのバックエンドデバイスがこの温度 (°C) を超えたら中止します。0 で無効になります。</translation>
    </message>
    <message>
        <source>Write recovered hashes to the file below, in the selected formats.</source>
        <translation>復元したハッシュを、選択した形式で下のファイルに書き込みます。</translation>
    </message>
    <message>
        <source>Write recovered hashes to file:</source>
        <translation>復元したハッシュをファイルに書き込む:</translation>
    </message>
    <message>
        <source>The file the recovered hashes are written to. Format variables:

&lt;hash&gt; = current hash filename or hash
&lt;unixtime&gt; = current Unix timestamp</source>
        <translation>復元したハッシュの書き込み先ファイル。書式変数:

&lt;hash&gt; = 現在のハッシュファイル名またはハッシュ
&lt;unixtime&gt; = 現在の Unix タイムスタンプ</translation>
    </message>
    <message>
        <source>Choose the outfile path.</source>
        <translation>出力ファイルのパスを選択します。</translation>
    </message>
    <message>
        <source>Choose outfile</source>
        <translation>出力ファイルを選択</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>形式:</translation>
    </message>
    <message>
        <source>Do not write the potfile</source>
        <translation>potfile に書き込まない</translation>
    </message>
    <message>
        <source>Run hashcat in a terminal with these options.</source>
        <translation>これらのオプションで hashcat をターミナルで実行します。</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>開始</translation>
    </message>
    <message>
        <source>Copy command to clipboard</source>
        <translation>コマンドをクリップボードにコピー</translation>
    </message>
    <message>
        <source>General</source>
        <translation>全般</translation>
    </message>
    <message>
        <source>Ignore username in hash file</source>
        <translation>ハッシュファイル内のユーザー名を無視する</translation>
    </message>
    <message>
        <source>Wordlists</source>
        <translation>ワードリスト</translation>
    </message>
    <message>
        <source>Increment the mask length from the minimum to the maximum.</source>
        <translation>マスク長を最小値から最大値まで増やします。</translation>
    </message>
    <message>
        <source>Minimum mask length; 0 uses hashcat’s default.</source>
        <translation>マスクの最小長。0 の場合は hashcat の既定値を使用します。</translation>
    </message>
    <message>
        <source>Maximum mask length; 0 uses hashcat’s default.</source>
        <translation>マスクの最大長。0 の場合は hashcat の既定値を使用します。</translation>
    </message>
    <message>
        <source>Abort the session after this many seconds; 0 runs until hashcat finishes.</source>
        <translation>この秒数が経過するとセッションを中止します。0 の場合は hashcat が終了するまで実行します。</translation>
    </message>
    <message>
        <source>&#xa0;s</source>
        <translation>&#xa0;秒</translation>
    </message>
    <message>
        <source>Skip this many candidates from the start.</source>
        <translation>先頭からこの数の候補をスキップします。</translation>
    </message>
    <message>
        <source>Only test this many candidates after the skipped ones.</source>
        <translation>スキップした候補の後で、この数の候補だけをテストします。</translation>
    </message>
    <message>
        <source>Enable optimized kernels (limits password length)</source>
        <translation>最適化カーネルを有効にする (パスワード長が制限されます)</translation>
    </message>
    <message>
        <source>Print only the attack’s speed, then quit; no cracking.</source>
        <translation>攻撃の速度のみを表示して終了します。解読は行いません。</translation>
    </message>
    <message>
        <source>Return expected speed of the attack, then quit</source>
        <translation>攻撃の予想速度を表示して終了する</translation>
    </message>
    <message>
        <source>&#xa0;°C</source>
        <translation>&#xa0;°C</translation>
    </message>
    <message>
        <source>Copy command</source>
        <translation>コマンドをコピー</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>ヘルプ(&amp;H)</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>ツール(&amp;T)</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>ファイル(&amp;F)</translation>
    </message>
    <message>
        <source>Ctrl+Q</source>
        <translation>Ctrl+Q</translation>
    </message>
    <message>
        <source>Settings…</source>
        <translation>設定…</translation>
    </message>
    <message>
        <source>Ctrl+,</source>
        <translation>Ctrl+,</translation>
    </message>
    <message>
        <source>Import Settings…</source>
        <translation>設定のインポート…</translation>
    </message>
    <message>
        <source>Ctrl+Shift+O</source>
        <translation>Ctrl+Shift+O</translation>
    </message>
    <message>
        <source>Export Settings…</source>
        <translation>設定のエクスポート…</translation>
    </message>
    <message>
        <source>Ctrl+Shift+S</source>
        <translation>Ctrl+Shift+S</translation>
    </message>
    <message>
        <source>Export Session…</source>
        <translation>セッションのエクスポート…</translation>
    </message>
    <message>
        <source>Ctrl+Shift+E</source>
        <translation>Ctrl+Shift+E</translation>
    </message>
    <message>
        <source>Import Session…</source>
        <translation>セッションのインポート…</translation>
    </message>
    <message>
        <source>About</source>
        <translation>hashcat-gui について</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation>終了</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>Qt について</translation>
    </message>
    <message>
        <source>JSON Files (*.json)</source>
        <translation>JSON ファイル (*.json)</translation>
    </message>
    <message>
        <source>The session file could not be read, so the session was not restored. The file was left in place.

%1</source>
        <translation>セッションファイルを読み込めなかったため、セッションは復元されませんでした。ファイルはそのまま残されています。

%1</translation>
    </message>
    <message>
        <source>Save failed</source>
        <translation>保存に失敗しました</translation>
    </message>
    <message>
        <source>Outfile format to use, separated by commas.
Default: 1,2</source>
        <translation>使用する出力ファイル形式 (カンマ区切り)。
既定値: 1,2</translation>
    </message>
    <message>
        <source>Common encodings:</source>
        <translation>一般的なエンコード:</translation>
    </message>
    <message>
        <source>Force the internal wordlist encoding from this code. Leave empty to let hashcat decide.</source>
        <translation>ワードリストの内部エンコードの変換元を、このコードに強制します。空欄の場合は hashcat が決定します。</translation>
    </message>
    <message>
        <source>Force the internal wordlist encoding to this code. Leave empty to let hashcat decide.</source>
        <translation>ワードリストの内部エンコードの変換先を、このコードに強制します。空欄の場合は hashcat が決定します。</translation>
    </message>
    <message>
        <source>Exported</source>
        <translation>エクスポート完了</translation>
    </message>
    <message>
        <source>Export failed</source>
        <translation>エクスポートに失敗しました</translation>
    </message>
    <message>
        <source>Import failed</source>
        <translation>インポートに失敗しました</translation>
    </message>
    <message>
        <source>Imported</source>
        <translation>インポート完了</translation>
    </message>
    <message>
        <source>The file was made by an older version of hashcat-gui (format version %1; this version reads format version %2).</source>
        <translation>このファイルは古いバージョンの hashcat-gui で作成されました (フォーマットバージョン %1。このバージョンはフォーマットバージョン %2 を読み込みます)。</translation>
    </message>
    <message>
        <source>The file holds sections this import does not use: %1.</source>
        <translation>このファイルには、このインポートで使用しないセクションが含まれています: %1。</translation>
    </message>
    <message>
        <source>The file is in format version %1, but this version reads format version %2, so it was not applied. Update hashcat-gui to read it.</source>
        <translation>このファイルはフォーマットバージョン %1 ですが、このバージョンはフォーマットバージョン %2 を読み込むため、適用されませんでした。読み込むには hashcat-gui を更新してください。</translation>
    </message>
    <message>
        <source>The session file is in format version %1, but this version reads format version %2, so the session was not restored. The file was left as it is, so the session of this run is not stored either. Update hashcat-gui to read it.</source>
        <translation>セッションファイルはフォーマットバージョン %1 ですが、このバージョンはフォーマットバージョン %2 を読み込むため、セッションは復元されませんでした。ファイルはそのまま残されているため、今回の実行のセッションも保存されません。読み込むには hashcat-gui を更新してください。</translation>
    </message>
    <message>
        <source>Settings exported to %1.</source>
        <translation>設定を %1 にエクスポートしました。</translation>
    </message>
    <message>
        <source>OpenCL device types to use, separated by commas.</source>
        <translation>使用する OpenCL デバイスタイプ (カンマ区切り)。</translation>
    </message>
    <message>
        <source>Export session</source>
        <translation>セッションのエクスポート</translation>
    </message>
    <message>
        <source>Session exported to “%1”.</source>
        <translation>セッションを「%1」にエクスポートしました。</translation>
    </message>
    <message>
        <source>Import session</source>
        <translation>セッションのインポート</translation>
    </message>
    <message>
        <source>Session imported from “%1”.</source>
        <translation>セッションを「%1」からインポートしました。</translation>
    </message>
    <message>
        <source>Session imported from “%1”.

%2</source>
        <translation>セッションを「%1」からインポートしました。

%2</translation>
    </message>
    <message>
        <source>Export settings</source>
        <translation>設定のエクスポート</translation>
    </message>
    <message>
        <source>Import settings</source>
        <translation>設定のインポート</translation>
    </message>
    <message>
        <source>Settings loaded from “%1”.</source>
        <translation>設定を「%1」から読み込みました。</translation>
    </message>
    <message>
        <source>Review them and save to apply them.</source>
        <translation>内容を確認し、保存して適用してください。</translation>
    </message>
    <message>
        <source>Load example session</source>
        <translation>サンプルセッションを読み込む</translation>
    </message>
    <message>
        <source>Reset session</source>
        <translation>セッションをリセット</translation>
    </message>
    <message>
        <source>Reset every field to its default? The current session is lost.</source>
        <translation>すべてのフィールドを既定値にリセットしますか? 現在のセッションは失われます。</translation>
    </message>
    <message>
        <source>Session warning</source>
        <translation>セッションの警告</translation>
    </message>
    <message>
        <source>Updating…</source>
        <translation>更新中…</translation>
    </message>
    <message>
        <source>Failed to load hash types</source>
        <translation>ハッシュタイプの読み込みに失敗しました</translation>
    </message>
    <message>
        <source>This mode needs a wordlist and a mask.</source>
        <translation>このモードにはワードリストとマスクが必要です。</translation>
    </message>
    <message>
        <source>This mode needs at least one wordlist.</source>
        <translation>このモードには少なくとも 1 つのワードリストが必要です。</translation>
    </message>
    <message>
        <source>Open hash file</source>
        <translation>ハッシュファイルを開く</translation>
    </message>
    <message>
        <source>Save output file</source>
        <translation>出力ファイルを保存</translation>
    </message>
    <message>
        <source>Add wordlists</source>
        <translation>ワードリストを追加</translation>
    </message>
    <message>
        <source>Open rules file</source>
        <translation>ルールファイルを開く</translation>
    </message>
    <message>
        <source>Could not start “%1”: %2</source>
        <translation>「%1」を起動できませんでした: %2</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not available. Available terminals: %2
Navigate to %3 → %4 to change it.</source>
        <translation>設定されたターミナル「%1」は利用できません。利用可能なターミナル: %2
変更するには %3 → %4 を開いてください。</translation>
    </message>
    <message>
        <source>This mode needs exactly %1 wordlists (left, right).
Currently selected: %2.</source>
        <translation>このモードにはちょうど %1 個のワードリスト (左、右) が必要です。
現在の選択: %2。</translation>
    </message>
    <message>
        <source>The session names the hash type %1, which this hashcat version does not support.</source>
        <translation>セッションがハッシュタイプ %1 を指定していますが、この hashcat のバージョンはサポートしていません。</translation>
    </message>
    <message>
        <source>Password candidate mask. ?1–?4 insert a custom charset, ?w inserts the wordlist word and ?q a second wordlist word in the Hybrid (mask + ?w/?q) mode, and ?? is a literal ?.</source>
        <translation>パスワード候補のマスク。?1–?4 はカスタム文字セットを挿入し、?w はワードリストの単語を、?q は「ハイブリッド (マスク + ?w/?q)」モードで 2 つ目のワードリストの単語を挿入します。?? はリテラルの ? です。</translation>
    </message>
    <message>
        <source>Load the %1 example? The current session is lost.

The example cracks a test hash; hashcat should recover the password %2.</source>
        <translation>%1 のサンプルを読み込みますか? 現在のセッションは失われます。

このサンプルはテスト用ハッシュを解読します。hashcat はパスワード %2 を復元するはずです。</translation>
    </message>
    <message>
        <source>Could not create the example directory “%1”.</source>
        <translation>サンプルディレクトリ「%1」を作成できませんでした。</translation>
    </message>
    <message>
        <source>Could not read the bundled file “%1”: %2</source>
        <translation>同梱ファイル「%1」を読み込めませんでした: %2</translation>
    </message>
    <message>
        <source>Could not write “%1”: %2</source>
        <translation>「%1」を書き込めませんでした: %2</translation>
    </message>
    <message>
        <source>Straight</source>
        <translation>辞書</translation>
    </message>
    <message>
        <source>Combination</source>
        <translation>組み合わせ</translation>
    </message>
    <message>
        <source>Brute-force</source>
        <translation>ブルートフォース</translation>
    </message>
    <message>
        <source>Hybrid Wordlist + Mask</source>
        <translation>ハイブリッド: ワードリスト + マスク</translation>
    </message>
    <message>
        <source>Hybrid Mask + Wordlist</source>
        <translation>ハイブリッド: マスク + ワードリスト</translation>
    </message>
    <message>
        <source>Generic (feed)</source>
        <translation>汎用 (フィード)</translation>
    </message>
    <message>
        <source>Association</source>
        <translation>関連付け</translation>
    </message>
    <message>
        <source>Hybrid (mask + ?w/?q)</source>
        <translation>ハイブリッド (マスク + ?w/?q)</translation>
    </message>
    <message>
        <source>Failed to obtain supported hash types.
Error: %1</source>
        <translation>サポートされているハッシュタイプを取得できませんでした。
エラー: %1</translation>
    </message>
    <message>
        <source>The hash types could not be loaded. Check the hashcat path in Settings.</source>
        <translation>ハッシュタイプを読み込めませんでした。設定で hashcat のパスを確認してください。</translation>
    </message>
    <message>
        <source>

This can be caused by Windows Defender or other antivirus software. See the &lt;a href=&quot;%1&quot;&gt;FAQ entry&lt;/a&gt; for a remedy.</source>
        <translation>

これは Windows Defender などのウイルス対策ソフトが原因の場合があります。対処方法は&lt;a href=&quot;%1&quot;&gt;FAQ の項目&lt;/a&gt;を参照してください。</translation>
    </message>
    <message>
        <source>Set the hashcat path under %1 → %2 to load the available hash types.</source>
        <translation>利用可能なハッシュタイプを読み込むには、%1 → %2 で hashcat のパスを設定してください。</translation>
    </message>
    <message>
        <source>This mode needs exactly 2 wordlists (left, right).</source>
        <translation>このモードにはちょうど 2 個のワードリスト (左、右) が必要です。</translation>
    </message>
    <message>
        <source>This mode needs a mask.</source>
        <translation>このモードにはマスクが必要です。</translation>
    </message>
    <message>
        <source>This mode runs hashcat’s bundled default-passwords ruleset; name one or more to use your own.</source>
        <translation>このモードは hashcat に同梱のルールセット default-passwords を実行します。独自のものを使用するには、1 つ以上指定してください。</translation>
    </message>
    <message>
        <source>This mode needs one wordlist and at least one table.</source>
        <translation>このモードにはワードリスト 1 つと、少なくとも 1 つのテーブルが必要です。</translation>
    </message>
    <message>
        <source>This mode needs a feed. Arguments depend on the feed: random and stdin need none, wordlist needs a path.</source>
        <translation>このモードにはフィードが必要です。引数はフィードによって異なります。random と stdin には不要で、wordlist にはパスが必要です。</translation>
    </message>
    <message>
        <source>This mode needs at most one wordlist. Without one the words come from the hash file and the settings line can bound the run; with one the two files are paired by line.</source>
        <translation>このモードで使用できるワードリストは最大 1 つです。ワードリストがない場合、単語はハッシュファイルから取得され、設定行で実行を制限できます。ワードリストがある場合は、2 つのファイルが行ごとに対応付けられます。</translation>
    </message>
    <message>
        <source>This mode needs a mask with exactly one ?w; a ?q after ?w names a second wordlist.</source>
        <translation>このモードには、?w をちょうど 1 つ含むマスクが必要です。?w の後に ?q を置くと 2 つ目のワードリストを指定できます。</translation>
    </message>
    <message>
        <source>Open cache directory</source>
        <translation>キャッシュディレクトリを開く</translation>
    </message>
    <message>
        <source>Add wordlist folder</source>
        <translation>ワードリストフォルダーを追加</translation>
    </message>
    <message>
        <source>Add rulesets</source>
        <translation>ルールセットを追加</translation>
    </message>
    <message>
        <source>Add ruleset folder</source>
        <translation>ルールセットフォルダーを追加</translation>
    </message>
    <message>
        <source>Add tables</source>
        <translation>テーブルを追加</translation>
    </message>
    <message>
        <source>Open feed file</source>
        <translation>フィードファイルを開く</translation>
    </message>
    <message>
        <source>This folder has no wordlists. hashcat reads the regular files directly inside it, not subfolders.</source>
        <translation>このフォルダーにはワードリストがありません。hashcat は、その直下の通常のファイルを読み込み、サブフォルダーは読み込みません。</translation>
    </message>
    <message>
        <source>This wordlist does not exist:
%1</source>
        <translation>このワードリストは存在しません:
%1</translation>
    </message>
    <message>
        <source>This ruleset folder has no Grammar/grammar.txt:
%1</source>
        <translation>このルールセットフォルダーには Grammar/grammar.txt がありません:
%1</translation>
    </message>
    <message>
        <source>This ruleset does not exist:
%1</source>
        <translation>このルールセットは存在しません:
%1</translation>
    </message>
    <message>
        <source>A table must be a file; hashcat does not read a folder:
%1</source>
        <translation>テーブルはファイルである必要があります。hashcat はフォルダーを読み込みません:
%1</translation>
    </message>
    <message>
        <source>This table does not exist:
%1</source>
        <translation>このテーブルは存在しません:
%1</translation>
    </message>
    <message>
        <source>Copied to the clipboard</source>
        <translation>クリップボードにコピーしました</translation>
    </message>
    <message>
        <source>Launch failed</source>
        <translation>起動に失敗しました</translation>
    </message>
    <message>
        <source>Please choose a hash file.</source>
        <translation>ハッシュファイルを選択してください。</translation>
    </message>
    <message>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to configure the path to the hashcat executable.</source>
        <translation>&lt;b&gt;%1 → %2&lt;/b&gt; を開いて、hashcat 実行ファイルのパスを設定してください。</translation>
    </message>
    <message>
        <source>The configured hashcat executable does not exist:
%1
Navigate to %2 → %3 to change it.</source>
        <translation>設定された hashcat 実行ファイルが存在しません:
%1
変更するには %2 → %3 を開いてください。</translation>
    </message>
    <message>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to select the terminal used for launching.</source>
        <translation>&lt;b&gt;%1 → %2&lt;/b&gt; を開いて、起動に使用するターミナルを選択してください。</translation>
    </message>
    <message>
        <source>Maximum number of wordlists for this mode: %1.
Currently selected: %2.</source>
        <translation>このモードで使用できるワードリストの最大数は %1 個です。
現在の選択: %2。</translation>
    </message>
    <message>
        <source>hashcat pairs the two files by line here; this GUI offers only one wordlist for that role.</source>
        <translation>ここでは hashcat が 2 つのファイルを行ごとに対応付けます。この GUI では、その役割に使用できるワードリストは 1 つだけです。</translation>
    </message>
    <message>
        <source>This wordlist has to be a file: hashcat reads it as the multiplied wordlist (the ?q side), and a folder cannot be used there:
%1</source>
        <translation>このワードリストはファイルである必要があります。hashcat はこれを乗算されるワードリスト (?q 側) として読み込むため、フォルダーは使用できません:
%1</translation>
    </message>
    <message>
        <source>This mode needs at least one table.</source>
        <translation>このモードには少なくとも 1 つのテーブルが必要です。</translation>
    </message>
    <message>
        <source>This mode needs a feed: a bundled name (wordlist, stdin, random, …) or the path of a feed file.</source>
        <translation>このモードにはフィードが必要です。同梱の名前 (wordlist、stdin、random、…) またはフィードファイルのパスを指定してください。</translation>
    </message>
    <message>
        <source>This mode pairs the wordlists with the mask; the mask needs a ?w for the word.</source>
        <translation>このモードはワードリストをマスクと対応付けます。マスクには単語用の ?w が必要です。</translation>
    </message>
    <message>
        <source>The mask contains more than one ?w.</source>
        <translation>マスクに ?w が複数含まれています。</translation>
    </message>
    <message>
        <source>The mask contains a ?q before the first ?w.</source>
        <translation>マスクの最初の ?w より前に ?q が含まれています。</translation>
    </message>
    <message>
        <source>The mask contains more than one ?q.</source>
        <translation>マスクに ?q が複数含まれています。</translation>
    </message>
    <message>
        <source>The mask names a second word with ?q, but only one wordlist is selected.</source>
        <translation>マスクが ?q で 2 つ目の単語を指定していますが、選択されているワードリストは 1 つだけです。</translation>
    </message>
    <message>
        <source>Two wordlists are selected, but the mask has no ?q for the second one.</source>
        <translation>2 つのワードリストが選択されていますが、マスクに 2 つ目用の ?q がありません。</translation>
    </message>
    <message>
        <source>The increment is not offered in this hybrid mode.</source>
        <translation>このハイブリッドモードではインクリメントは使用できません。</translation>
    </message>
    <message>
        <source>The ?w and ?q markers are only offered in the hybrid mode (mask + wordlists).</source>
        <translation>?w と ?q のマーカーは、ハイブリッドモード (マスク + ワードリスト) でのみ使用できます。</translation>
    </message>
    <message>
        <source>The setting “%1” is not valid for this mode.</source>
        <translation>設定「%1」はこのモードでは無効です。</translation>
    </message>
    <message>
        <source>The token “%1” is not a setting; settings are written as key=value.</source>
        <translation>トークン「%1」は設定ではありません。設定は key=value の形式で記述します。</translation>
    </message>
    <message>
        <source>The settings line applies only while the words come from the hash file. Clear either the settings line or the wordlist list.</source>
        <translation>設定行は、単語がハッシュファイルから取得される場合にのみ適用されます。設定行かワードリスト一覧のどちらかをクリアしてください。</translation>
    </message>
    <message>
        <source>The hash type 4000 describes its hashes as a recipe, for example
md5(md5(salt) . pass). Enter it in the hash recipe field.</source>
        <translation>ハッシュタイプ 4000 は、ハッシュをレシピとして記述します。例:
md5(md5(salt) . pass)。これをハッシュレシピ欄に入力してください。</translation>
    </message>
    <message>
        <source>The minimum increment length is larger than the maximum.</source>
        <translation>インクリメントの最小長が最大長より大きくなっています。</translation>
    </message>
    <message>
        <source>The cache path must be an existing directory:
%1</source>
        <translation>キャッシュパスは既存のディレクトリである必要があります:
%1</translation>
    </message>
    <message>
        <source>The increment cannot be combined with the speed-only option.</source>
        <translation>インクリメントは速度のみのオプションと併用できません。</translation>
    </message>
    <message>
        <source>Removing cracked hashes is not possible in the association attack mode.</source>
        <translation>「関連付け」攻撃モードでは、解読済みのハッシュを削除できません。</translation>
    </message>
</context>
<context>
    <name>SettingsDialog</name>
    <message>
        <source>Settings</source>
        <translation>設定</translation>
    </message>
    <message>
        <source>hashcat executable</source>
        <translation>hashcat 実行ファイル</translation>
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
        <source>Select…</source>
        <translation>選択…</translation>
    </message>
    <message>
        <source>&amp;Terminal</source>
        <translation>ターミナル(&amp;T)</translation>
    </message>
    <message>
        <source>&amp;Language</source>
        <translation>言語(&amp;L)</translation>
    </message>
    <message>
        <source>&amp;Color scheme</source>
        <translation>カラースキーム(&amp;C)</translation>
    </message>
    <message>
        <source>Not all color schemes are available on all platforms.</source>
        <translation>すべてのカラースキームがすべてのプラットフォームで利用できるわけではありません。</translation>
    </message>
    <message>
        <source>&amp;Parameters</source>
        <translation>パラメーター(&amp;P)</translation>
    </message>
    <message>
        <source>&amp;Use short parameters</source>
        <translation>短いパラメーターを使用する(&amp;U)</translation>
    </message>
    <message>
        <source>Session and files</source>
        <translation>セッションとファイル</translation>
    </message>
    <message>
        <source>&amp;Restore the last session at startup</source>
        <translation>起動時に前回のセッションを復元する(&amp;R)</translation>
    </message>
    <message>
        <source>&amp;Session file</source>
        <translation>セッションファイル(&amp;S)</translation>
    </message>
    <message>
        <source>The file the last session is stored in</source>
        <translation>前回のセッションの保存先ファイル</translation>
    </message>
    <message>
        <source>Open folder</source>
        <translation>フォルダーを開く</translation>
    </message>
    <message>
        <source>Open the folder the session is stored in</source>
        <translation>セッションの保存先フォルダーを開きます</translation>
    </message>
    <message>
        <source>Settings &amp;file</source>
        <translation>設定ファイル(&amp;F)</translation>
    </message>
    <message>
        <source>The file the settings are stored in</source>
        <translation>設定の保存先ファイル</translation>
    </message>
    <message>
        <source>Open the folder the settings are stored in</source>
        <translation>設定の保存先フォルダーを開きます</translation>
    </message>
    <message>
        <source>General</source>
        <translation>全般</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>保存</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>キャンセル</translation>
    </message>
    <message>
        <source>Download from %1</source>
        <translation>ダウンロード元: %1</translation>
    </message>
    <message>
        <source>Use the short option names (-m) instead of the long ones (--hash-type) in the command.</source>
        <translation>コマンドで長いオプション名 (--hash-type) の代わりに短いオプション名 (-m) を使用します。</translation>
    </message>
    <message>
        <source>Select a terminal</source>
        <translation>ターミナルを選択</translation>
    </message>
    <message>
        <source>System default</source>
        <translation>システムの既定</translation>
    </message>
    <message>
        <source>Light</source>
        <translation>ライト</translation>
    </message>
    <message>
        <source>Dark</source>
        <translation>ダーク</translation>
    </message>
    <message>
        <source>The hashcat path “%1” was not found on this computer.</source>
        <translation>hashcat のパス「%1」はこのコンピューターで見つかりませんでした。</translation>
    </message>
    <message>
        <source>The terminal “%1” is not installed and was not applied.</source>
        <translation>ターミナル「%1」はインストールされていないため、適用されませんでした。</translation>
    </message>
    <message>
        <source>The color scheme “%1” is not offered and was not applied.</source>
        <translation>カラースキーム「%1」は提供されていないため、適用されませんでした。</translation>
    </message>
    <message>
        <source>The language “%1” is not offered and was not applied.</source>
        <translation>言語「%1」は提供されていないため、適用されませんでした。</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not installed.</source>
        <translation>設定されたターミナル「%1」はインストールされていません。</translation>
    </message>
    <message>
        <source>Executable Files (*.exe *.bin hashcat);;All Files (*)</source>
        <translation>実行ファイル (*.exe *.bin hashcat);;すべてのファイル (*)</translation>
    </message>
    <message>
        <source>No hashcat executable is configured; hashcat cannot be started.</source>
        <translation>hashcat 実行ファイルが設定されていないため、hashcat を起動できません。</translation>
    </message>
    <message>
        <source>The configured hashcat executable was not found on this computer.</source>
        <translation>設定された hashcat 実行ファイルはこのコンピューターで見つかりませんでした。</translation>
    </message>
    <message>
        <source>Select the terminal hashcat is started in.</source>
        <translation>hashcat を起動するターミナルを選択してください。</translation>
    </message>
    <message>
        <source>Select the hashcat executable</source>
        <translation>hashcat 実行ファイルを選択</translation>
    </message>
    <message>
        <source>Invalid file</source>
        <translation>無効なファイル</translation>
    </message>
    <message>
        <source>The selected file is not an executable.</source>
        <translation>選択したファイルは実行ファイルではありません。</translation>
    </message>
    <message>
        <source>Restart required</source>
        <translation>再起動が必要です</translation>
    </message>
    <message>
        <source>Restart hashcat-gui to apply the new language.</source>
        <translation>新しい言語を適用するには hashcat-gui を再起動してください。</translation>
    </message>
</context>
<context>
    <name>StateFile</name>
    <message>
        <source>The file is not valid JSON: %1</source>
        <translation>ファイルは有効な JSON ではありません: %1</translation>
    </message>
    <message>
        <source>The file does not contain a JSON object.</source>
        <translation>ファイルに JSON オブジェクトが含まれていません。</translation>
    </message>
    <message>
        <source>The file does not contain any sections.</source>
        <translation>ファイルにセクションが含まれていません。</translation>
    </message>
    <message>
        <source>Could not open “%1” for writing: %2</source>
        <translation>「%1」を書き込み用に開けませんでした: %2</translation>
    </message>
    <message>
        <source>Could not write to “%1”: %2</source>
        <translation>「%1」に書き込めませんでした: %2</translation>
    </message>
    <message>
        <source>Could not open “%1” for reading: %2</source>
        <translation>「%1」を読み込み用に開けませんでした: %2</translation>
    </message>
    <message>
        <source>The file does not contain a “%1” section.</source>
        <translation>ファイルに「%1」セクションが含まれていません。</translation>
    </message>
    <message>
        <source>The file does not contain a “%1” section; it holds %2.</source>
        <translation>ファイルに「%1」セクションが含まれていません。含まれているのは %2 です。</translation>
    </message>
    <message>
        <source>The “%1” section does not contain any values.</source>
        <translation>「%1」セクションに値が含まれていません。</translation>
    </message>
    <message>
        <source> and </source>
        <translation> と </translation>
    </message>
    <message>
        <source> and %1 more</source>
        <translation> ほか %1 件</translation>
    </message>
    <message>
        <source>The file names values this version does not know, so they were not applied: %1.</source>
        <translation>このバージョンが認識しない値がファイルに含まれているため、適用されませんでした: %1。</translation>
    </message>
    <message>
        <source>The file holds values of the wrong type, so they were not applied: %1.</source>
        <translation>ファイルに型が正しくない値が含まれているため、適用されませんでした: %1。</translation>
    </message>
</context>
<context>
    <name>UpdateChecker</name>
    <message>
        <source>GitHub answered with HTTP status %1: %2</source>
        <translation>GitHub が HTTP ステータス %1 で応答しました: %2</translation>
    </message>
    <message>
        <source>GitHub answered with HTTP status %1.</source>
        <translation>GitHub が HTTP ステータス %1 で応答しました。</translation>
    </message>
    <message>
        <source>The release information is not valid JSON.</source>
        <translation>リリース情報が有効な JSON ではありません。</translation>
    </message>
    <message>
        <source>The release information does not name a version.</source>
        <translation>リリース情報にバージョンが含まれていません。</translation>
    </message>
    <message>
        <source>The release version %1 is not a valid version number.</source>
        <translation>リリースバージョン %1 は有効なバージョン番号ではありません。</translation>
    </message>
    <message>
        <source>The installed version %1 is not a valid version number.</source>
        <translation>インストールされているバージョン %1 は有効なバージョン番号ではありません。</translation>
    </message>
</context>
</TS>
