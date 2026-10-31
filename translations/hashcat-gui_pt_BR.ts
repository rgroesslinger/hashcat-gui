<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="pt_BR" sourcelanguage="en">
<context>
    <name>AboutDialog</name>
    <message>
        <source>About</source>
        <translation>Sobre</translation>
    </message>
    <message>
        <source>Graphical user interface for hashcat, the password recovery utility</source>
        <translation>Interface gráfica para o hashcat, o utilitário de recuperação de senhas</translation>
    </message>
    <message>
        <source>Version information</source>
        <translation>Informações da versão</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Desconhecido</translation>
    </message>
    <message>
        <source>Updates</source>
        <translation>Atualizações</translation>
    </message>
    <message>
        <source>Checking for updates…</source>
        <translation>Verificando atualizações…</translation>
    </message>
    <message>
        <source>System information</source>
        <translation>Informações do sistema</translation>
    </message>
    <message>
        <source>Operating system</source>
        <translation>Sistema operacional</translation>
    </message>
    <message>
        <source>Links</source>
        <translation>Links</translation>
    </message>
    <message>
        <source>License</source>
        <translation>Licença</translation>
    </message>
    <message>
        <source>Copy the application, toolkit, system and hashcat versions to the clipboard</source>
        <translation>Copiar para a área de transferência as versões do aplicativo, do toolkit, do sistema e do hashcat</translation>
    </message>
    <message>
        <source>Copy details</source>
        <translation>Copiar detalhes</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Copied to the clipboard</source>
        <translation>Copiado para a área de transferência</translation>
    </message>
    <message>
        <source>Project page</source>
        <translation>Página do projeto</translation>
    </message>
    <message>
        <source>Report an issue</source>
        <translation>Relatar um problema</translation>
    </message>
    <message>
        <source>hashcat</source>
        <translation>hashcat</translation>
    </message>
    <message>
        <source>hashcat FAQ</source>
        <translation>FAQ do hashcat</translation>
    </message>
    <message>
        <source>hashcat-gui is licensed under the %1.</source>
        <translation>O hashcat-gui é licenciado sob a %1.</translation>
    </message>
    <message>
        <source>GNU GPL v3.0 or later</source>
        <translation>GNU GPL v3.0 ou posterior</translation>
    </message>
    <message>
        <source>Not configured</source>
        <translation>Não configurado</translation>
    </message>
    <message>
        <source>Error: %1</source>
        <translation>Erro: %1</translation>
    </message>
    <message>
        <source>A newer version is available: %1.</source>
        <translation>Uma versão mais recente está disponível: %1.</translation>
    </message>
    <message>
        <source>Release page</source>
        <translation>Página da versão</translation>
    </message>
    <message>
        <source>No updates available.</source>
        <translation>Nenhuma atualização disponível.</translation>
    </message>
    <message>
        <source>Could not check for updates.</source>
        <translation>Não foi possível verificar se há atualizações.</translation>
    </message>
    <message>
        <source>Could not check for updates: %1</source>
        <translation>Não foi possível verificar se há atualizações: %1</translation>
    </message>
</context>
<context>
    <name>HashcatInfoParser</name>
    <message>
        <source>Invalid JSON returned from hashcat.</source>
        <translation>O hashcat retornou um JSON inválido.</translation>
    </message>
    <message>
        <source>hashcat did not report any hash types.</source>
        <translation>O hashcat não informou nenhum tipo de hash.</translation>
    </message>
</context>
<context>
    <name>HelperUtils</name>
    <message>
        <source>The hashcat path is not configured.</source>
        <translation>O caminho do hashcat não está configurado.</translation>
    </message>
    <message>
        <source>Failed to start hashcat
%1</source>
        <translation>Falha ao iniciar o hashcat
%1</translation>
    </message>
    <message>
        <source>hashcat timed out
%1</source>
        <translation>O hashcat excedeu o tempo limite
%1</translation>
    </message>
    <message>
        <source>No supported terminal was found on this system. hashcat-gui needs one to show the hashcat output.</source>
        <translation>Nenhum terminal compatível foi encontrado neste sistema. O hashcat-gui precisa de um para mostrar a saída do hashcat.</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <source>Attack</source>
        <translation>Ataque</translation>
    </message>
    <message>
        <source>Hash file or hash:</source>
        <translation>Arquivo de hashes ou hash:</translation>
    </message>
    <message>
        <source>The hash to crack, a path to a file of hashes, or a .hccapx WPA capture. Paste a single hash here, or point it at a file.</source>
        <translation>O hash a quebrar, o caminho de um arquivo de hashes ou uma captura WPA .hccapx. Cole aqui um único hash ou aponte para um arquivo.</translation>
    </message>
    <message>
        <source>Salt is hex</source>
        <translation>O salt é hexadecimal</translation>
    </message>
    <message>
        <source>Remove already-cracked hashes from the potfile so a later run does not report them again. Refused in attack mode 9.</source>
        <translation>Remover do potfile os hashes já quebrados para que uma execução posterior não os informe de novo. Recusado no modo de ataque 9.</translation>
    </message>
    <message>
        <source>Remove hash from hash list once it is cracked</source>
        <translation>Remover o hash da lista de hashes depois de quebrado</translation>
    </message>
    <message>
        <source>Ignore the @username part of hashes (in hash modes that carry one) so the same word can satisfy several accounts.</source>
        <translation>Ignorar a parte @username dos hashes (nos modos de hash que a incluem) para que a mesma palavra possa servir a várias contas.</translation>
    </message>
    <message>
        <source>Ignore username in hash file</source>
        <translation>Ignorar o nome de usuário no arquivo de hashes</translation>
    </message>
    <message>
        <source>Mode:</source>
        <translation>Modo:</translation>
    </message>
    <message>
        <source>Attack mode. Each choice reveals its own options below.</source>
        <translation>Modo de ataque. Cada opção mostra suas próprias opções abaixo.</translation>
    </message>
    <message>
        <source>Hash type:</source>
        <translation>Tipo de hash:</translation>
    </message>
    <message>
        <source>Hash recipe:</source>
        <translation>Receita do hash:</translation>
    </message>
    <message>
        <source>Unix: &apos;md5(md5(salt) . pass)&apos;
               Windows: &quot;md5(&apos;abc&apos; . pass)&quot;</source>
        <translation>Unix: &apos;md5(md5(salt) . pass)&apos;
               Windows: &quot;md5(&apos;abc&apos; . pass)&quot;</translation>
    </message>
    <message>
        <source>Wordlists</source>
        <translation>Listas de palavras</translation>
    </message>
    <message>
        <source>Move item up</source>
        <translation>Mover item para cima</translation>
    </message>
    <message>
        <source>Move item down</source>
        <translation>Mover item para baixo</translation>
    </message>
    <message>
        <source>hashcat reads every file directly inside the folder as one wordlist.</source>
        <translation>O hashcat lê cada arquivo diretamente dentro da pasta como uma única lista de palavras.</translation>
    </message>
    <message>
        <source>Add folder…</source>
        <translation>Adicionar pasta…</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Remover</translation>
    </message>
    <message>
        <source>Only checked rows are used. A folder is one wordlist: hashcat reads the files directly inside it.</source>
        <translation>Somente as linhas marcadas são usadas. Uma pasta é uma lista de palavras: o hashcat lê os arquivos diretamente dentro dela.</translation>
    </message>
    <message>
        <source>Wordlist is hex</source>
        <translation>A lista de palavras é hexadecimal</translation>
    </message>
    <message>
        <source>Encoding from:</source>
        <translation>Codificação de origem:</translation>
    </message>
    <message>
        <source>Encoding to:</source>
        <translation>Codificação de destino:</translation>
    </message>
    <message>
        <source>Settings (no wordlist):</source>
        <translation>Configurações (sem lista de palavras):</translation>
    </message>
    <message>
        <source>Positional settings, separated by spaces. Only used when no wordlist is selected: with one, hashcat pairs the two files by line. “phases” picks which phases run, cheapest first; the default “words,rules,pcfg” ends only when stopped. Phase-prefixed settings: “rules.rulefile”, “rules.rulemax”, “words.hints”, “pcfg.hintaccount”, …</source>
        <translation>Configurações posicionais, separadas por espaços. Usadas somente quando nenhuma lista de palavras está selecionada: com uma lista, o hashcat emparelha os dois arquivos linha a linha. “phases” escolhe quais fases são executadas, da mais barata para a mais cara; o padrão “words,rules,pcfg” só termina quando é interrompido. Configurações com prefixo de fase: “rules.rulefile”, “rules.rulemax”, “words.hints”, “pcfg.hintaccount”, …</translation>
    </message>
    <message>
        <source>PCFG</source>
        <translation>PCFG</translation>
    </message>
    <message>
        <source>A row is a ruleset file, a folder with a Grammar/grammar.txt, or a bundled name. Only checked rows are used; with none checked, hashcat’s bundled default-passwords runs. hints is also bundled.</source>
        <translation>Uma linha é um arquivo de conjunto de regras, uma pasta com um Grammar/grammar.txt ou um nome incluído. Somente as linhas marcadas são usadas; se nenhuma estiver marcada, o default-passwords incluído no hashcat é executado. hints também é incluído.</translation>
    </message>
    <message>
        <source>A ruleset folder needs a Grammar/grammar.txt.</source>
        <translation>Uma pasta de conjunto de regras precisa de um Grammar/grammar.txt.</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Scale:</translation>
    </message>
    <message>
        <source>Quantization steps per bit (1–64)</source>
        <translation>Passos de quantização por bit (1–64)</translation>
    </message>
    <message>
        <source>Costmax:</source>
        <translation>Costmax:</translation>
    </message>
    <message>
        <source>Highest cost level to enumerate (1–64)</source>
        <translation>Nível de custo mais alto a enumerar (1–64)</translation>
    </message>
    <message>
        <source>Weights:</source>
        <translation>Weights:</translation>
    </message>
    <message>
        <source>One value per ruleset, plain numbers separated by colons, e.g. 2:1 for two rulesets. The count must match the number of rulesets.</source>
        <translation>Um valor por conjunto de regras, números simples separados por dois-pontos, p. ex., 2:1 para dois conjuntos de regras. A quantidade deve corresponder ao número de conjuntos de regras.</translation>
    </message>
    <message>
        <source>Threads:</source>
        <translation>Threads:</translation>
    </message>
    <message>
        <source>Cores that generate base words (0–16); 0 runs on the calling thread</source>
        <translation>Núcleos que geram palavras-base (0–16); 0 executa na thread chamadora</translation>
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
        <translation>Bits que um loop interno pode abranger (0–27)</translation>
    </message>
    <message>
        <source>Maxword:</source>
        <translation>Maxword:</translation>
    </message>
    <message>
        <source>Candidates the kernel builds per base word: 0, 16 or 32 (a multiple of 16)</source>
        <translation>Candidatos que o kernel constrói por palavra-base: 0, 16 ou 32 (múltiplo de 16)</translation>
    </message>
    <message>
        <source>Maxgain:</source>
        <translation>Maxgain:</translation>
    </message>
    <message>
        <source>1.0–64.0; decimal point is a dot, not the locale separator</source>
        <translation>1.0–64.0; o separador decimal é um ponto, não o separador da localidade</translation>
    </message>
    <message>
        <source>Walk:</source>
        <translation>Walk:</translation>
    </message>
    <message>
        <source>1 steps the last token instead of unraking it</source>
        <translation>1 avança o último token passo a passo em vez de aplicar “unraking” a ele</translation>
    </message>
    <message>
        <source>Mask:</source>
        <translation>Máscara:</translation>
    </message>
    <message>
        <source>Hold the grammar to the candidates this mask admits; cannot be combined with omen=1, or with hintaccount while the hint words come from the accounts (give hintwords or hintfile instead)</source>
        <translation>Limitar a gramática aos candidatos que esta máscara admite; não pode ser combinado com omen=1, nem com hintaccount enquanto as palavras de dica vierem das contas (informe hintwords ou hintfile em vez disso)</translation>
    </message>
    <message>
        <source>Extra settings (key=value):</source>
        <translation>Configurações extras (key=value):</translation>
    </message>
    <message>
        <source>Additional key=value settings, separated by spaces: omen, cache, lookup, pwmin, pwmax, hintaccount, hintwords, hintfile, hintrank, hintrepeat. Everything after the first key=value is a setting, so rulesets must come first.</source>
        <translation>Configurações key=value adicionais, separadas por espaços: omen, cache, lookup, pwmin, pwmax, hintaccount, hintwords, hintfile, hintrank, hintrepeat. Tudo depois do primeiro key=value é uma configuração, portanto os conjuntos de regras devem vir primeiro.</translation>
    </message>
    <message>
        <source>Table</source>
        <translation>Tabela</translation>
    </message>
    <message>
        <source>Every checked row after the wordlist is a table</source>
        <translation>Toda linha marcada após a lista de palavras é uma tabela</translation>
    </message>
    <message>
        <source>Add tables…</source>
        <translation>Adicionar tabelas…</translation>
    </message>
    <message>
        <source>Identity:</source>
        <translation>Identity:</translation>
    </message>
    <message>
        <source>Whether leaving a token unchanged is one of its choices: 1 (default) keeps it, 0 drops it so the table only produces variations. Cannot be combined with single=1.</source>
        <translation>Se deixar um token inalterado é uma de suas opções: 1 (padrão) o mantém, 0 o descarta para que a tabela produza apenas variações. Não pode ser combinado com single=1.</translation>
    </message>
    <message>
        <source>Template:</source>
        <translation>Template:</translation>
    </message>
    <message>
        <source>1 reads the wordlist as a set of patterns: no word is ever returned unchanged, and words matching no rule are dropped. 0 (default) keeps unchanged words.</source>
        <translation>1 lê a lista de palavras como um conjunto de padrões: nenhuma palavra é retornada inalterada, e palavras que não correspondem a nenhuma regra são descartadas. 0 (padrão) mantém as palavras inalteradas.</translation>
    </message>
    <message>
        <source>Single:</source>
        <translation>Single:</translation>
    </message>
    <message>
        <source>single=1 makes each candidate carry exactly one substitution instead of the cross product of every matched position, usually a different, cheaper attack.</source>
        <translation>single=1 faz cada candidato carregar exatamente uma substituição, em vez do produto cartesiano de todas as posições correspondentes; geralmente é um ataque diferente e mais barato.</translation>
    </message>
    <message>
        <source>Cap:</source>
        <translation>Cap:</translation>
    </message>
    <message>
        <source>Maximum replacements one source may carry in a word; 0 means no limit. Only limits the host-side cross product, not a single=1 run.</source>
        <translation>Máximo de substituições que uma fonte pode carregar em uma palavra; 0 significa sem limite. Limita apenas o produto cartesiano no lado do host, não uma execução com single=1.</translation>
    </message>
    <message>
        <source>Maxperm:</source>
        <translation>Maxperm:</translation>
    </message>
    <message>
        <source>How many of one word’s cross product the host enumerates before giving up; 0 means no limit. Each device cell is never cut.</source>
        <translation>Quantos elementos do produto cartesiano de uma palavra o host enumera antes de desistir; 0 significa sem limite. Nenhuma célula do dispositivo é cortada.</translation>
    </message>
    <message>
        <source>Additional key=value settings, separated by spaces. Everything after the first key=value is a setting, so the tables must come first.</source>
        <translation>Configurações key=value adicionais, separadas por espaços. Tudo depois do primeiro key=value é uma configuração, portanto as tabelas devem vir primeiro.</translation>
    </message>
    <message>
        <source>Generic feed</source>
        <translation>Feed genérico</translation>
    </message>
    <message>
        <source>Feed:</source>
        <translation>Feed:</translation>
    </message>
    <message>
        <source>Candidate generator. Anything containing a slash is treated as a feed path; otherwise hashcat looks for feed_&lt;name&gt; under its feeds directory. Arguments below are feed-specific.</source>
        <translation>Gerador de candidatos. Qualquer coisa que contenha uma barra é tratada como caminho de feed; caso contrário, o hashcat procura feed_&lt;name&gt; em seu diretório de feeds. Os argumentos abaixo são específicos de cada feed.</translation>
    </message>
    <message>
        <source>wordlist, stdin, random, association, pcfg, table, hybrid, mask, or a feed path</source>
        <translation>wordlist, stdin, random, association, pcfg, table, hybrid, mask ou um caminho de feed</translation>
    </message>
    <message>
        <source>Browse…</source>
        <translation>Procurar…</translation>
    </message>
    <message>
        <source>Argument:</source>
        <translation>Argumento:</translation>
    </message>
    <message>
        <source>Argument passed to the feed. Its meaning and count depend on the feed (the wordlist feed takes wordlist paths); extra tokens go straight to the feed.</source>
        <translation>Argumento passado ao feed. Seu significado e quantidade dependem do feed (o feed wordlist recebe caminhos de listas de palavras); tokens extras vão direto para o feed.</translation>
    </message>
    <message>
        <source>Add</source>
        <translation>Adicionar</translation>
    </message>
    <message>
        <source>Arguments the feed reads, in order; only checked rows are used</source>
        <translation>Argumentos que o feed lê, em ordem; somente as linhas marcadas são usadas</translation>
    </message>
    <message>
        <source>Rules</source>
        <translation>Regras</translation>
    </message>
    <message>
        <source>Use rules files:</source>
        <translation>Usar arquivos de regras:</translation>
    </message>
    <message>
        <source>Enable rules file slot 1 for this run.</source>
        <translation>Ativar o slot 1 de arquivo de regras nesta execução.</translation>
    </message>
    <message>
        <source>Rules file:</source>
        <translation>Arquivo de regras:</translation>
    </message>
    <message>
        <source>Rule file for slot 1; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>Arquivo de regras do slot 1; aplicado a cada palavra. Deixe em branco para não usar o slot.</translation>
    </message>
    <message>
        <source>Enable rules file slot 2 for this run.</source>
        <translation>Ativar o slot 2 de arquivo de regras nesta execução.</translation>
    </message>
    <message>
        <source>Rule file for slot 2; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>Arquivo de regras do slot 2; aplicado a cada palavra. Deixe em branco para não usar o slot.</translation>
    </message>
    <message>
        <source>Enable rules file slot 3 for this run.</source>
        <translation>Ativar o slot 3 de arquivo de regras nesta execução.</translation>
    </message>
    <message>
        <source>Rule file for slot 3; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>Arquivo de regras do slot 3; aplicado a cada palavra. Deixe em branco para não usar o slot.</translation>
    </message>
    <message>
        <source>Generate the rules automatically instead of reading them from a file.</source>
        <translation>Gerar as regras automaticamente em vez de lê-las de um arquivo.</translation>
    </message>
    <message>
        <source>Generate rules:</source>
        <translation>Gerar regras:</translation>
    </message>
    <message>
        <source>How many rules to generate for the finding-rule attack. No rules file is read, hashcat builds them itself; use an existing rule file with the slots above instead.</source>
        <translation>Quantas regras gerar para o ataque de busca de regras. Nenhum arquivo de regras é lido, o hashcat as constrói sozinho; use um arquivo de regras existente com os slots acima.</translation>
    </message>
    <message>
        <source>Pass every rules file to hashcat in one run, instead of one run per file.</source>
        <translation>Passar todos os arquivos de regras ao hashcat em uma única execução, em vez de uma execução por arquivo.</translation>
    </message>
    <message>
        <source>Join -r files into one list</source>
        <translation>Unir os arquivos -r em uma única lista</translation>
    </message>
    <message>
        <source>Wordlist rules</source>
        <translation>Regras da lista de palavras</translation>
    </message>
    <message>
        <source>Left rule:</source>
        <translation>Regra esquerda:</translation>
    </message>
    <message>
        <source>Right rule:</source>
        <translation>Regra direita:</translation>
    </message>
    <message>
        <source>Interpret the custom charsets as hexadecimal bytes. Applies to custom charset 1 through 4.</source>
        <translation>Interpretar os conjuntos de caracteres personalizados como bytes hexadecimais. Aplica-se aos conjuntos de caracteres personalizados de 1 a 4.</translation>
    </message>
    <message>
        <source>Charset is hex</source>
        <translation>O conjunto de caracteres é hexadecimal</translation>
    </message>
    <message>
        <source>Mask</source>
        <translation>Máscara</translation>
    </message>
    <message>
        <source>Increment mask length</source>
        <translation>Incrementar o comprimento da máscara</translation>
    </message>
    <message>
        <source>Min:</source>
        <translation>Mín.:</translation>
    </message>
    <message>
        <source>Max:</source>
        <translation>Máx.:</translation>
    </message>
    <message>
        <source>Custom charsets</source>
        <translation>Conjuntos de caracteres personalizados</translation>
    </message>
    <message>
        <source>Charset 1:</source>
        <translation>Conjunto de caracteres 1:</translation>
    </message>
    <message>
        <source>Charset 2:</source>
        <translation>Conjunto de caracteres 2:</translation>
    </message>
    <message>
        <source>Charset 3:</source>
        <translation>Conjunto de caracteres 3:</translation>
    </message>
    <message>
        <source>Charset 4:</source>
        <translation>Conjunto de caracteres 4:</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Geral</translation>
    </message>
    <message>
        <source>Session and limits</source>
        <translation>Sessão e limites</translation>
    </message>
    <message>
        <source>Session name:</source>
        <translation>Nome da sessão:</translation>
    </message>
    <message>
        <source>Name hashcat uses for its restore file and status output.</source>
        <translation>Nome que o hashcat usa para seu arquivo de restauração e para a saída de status.</translation>
    </message>
    <message>
        <source>Runtime limit:</source>
        <translation>Limite de execução:</translation>
    </message>
    <message>
        <source>unlimited</source>
        <translation>ilimitado</translation>
    </message>
    <message>
        <source>Skip:</source>
        <translation>Ignorar:</translation>
    </message>
    <message>
        <source>Limit:</source>
        <translation>Limite:</translation>
    </message>
    <message>
        <source>Update status automatically</source>
        <translation>Atualizar o status automaticamente</translation>
    </message>
    <message>
        <source>Open…</source>
        <translation>Abrir…</translation>
    </message>
    <message>
        <source>Assume the salt is given in hexadecimal.</source>
        <translation>Presumir que o salt é informado em hexadecimal.</translation>
    </message>
    <message>
        <source>Add files…</source>
        <translation>Adicionar arquivos…</translation>
    </message>
    <message>
        <source>Assume words in the wordlist are given in hexadecimal.</source>
        <translation>Presumir que as palavras da lista de palavras são informadas em hexadecimal.</translation>
    </message>
    <message>
        <source>Single rule applied to each word from the left wordlist.</source>
        <translation>Regra única aplicada a cada palavra da lista de palavras da esquerda.</translation>
    </message>
    <message>
        <source>Single rule applied to each word from the right wordlist.</source>
        <translation>Regra única aplicada a cada palavra da lista de palavras da direita.</translation>
    </message>
    <message>
        <source>Increment the mask length from the minimum to the maximum.</source>
        <translation>Incrementar o comprimento da máscara do mínimo ao máximo.</translation>
    </message>
    <message>
        <source>Minimum mask length; 0 uses hashcat’s default.</source>
        <translation>Comprimento mínimo da máscara; 0 usa o padrão do hashcat.</translation>
    </message>
    <message>
        <source>Maximum mask length; 0 uses hashcat’s default.</source>
        <translation>Comprimento máximo da máscara; 0 usa o padrão do hashcat.</translation>
    </message>
    <message>
        <source>Abort the session after this many seconds; 0 runs until hashcat finishes.</source>
        <translation>Abortar a sessão após este número de segundos; 0 executa até o hashcat terminar.</translation>
    </message>
    <message>
        <source>&#xa0;s</source>
        <translation>&#xa0;s</translation>
    </message>
    <message>
        <source>Skip this many candidates from the start.</source>
        <translation>Pular este número de candidatos desde o início.</translation>
    </message>
    <message>
        <source>Only test this many candidates after the skipped ones.</source>
        <translation>Testar apenas este número de candidatos depois dos pulados.</translation>
    </message>
    <message>
        <source>Keep updating the run status (speed, progress, ETA) instead of printing one final line.</source>
        <translation>Continuar atualizando o status da execução (velocidade, progresso, tempo estimado) em vez de imprimir uma única linha final.</translation>
    </message>
    <message>
        <source>Seconds between status updates.</source>
        <translation>Segundos entre as atualizações de status.</translation>
    </message>
    <message>
        <source>Ignore warnings and start hashcat anyway. Use with care.</source>
        <translation>Ignorar os avisos e iniciar o hashcat mesmo assim. Use com cuidado.</translation>
    </message>
    <message>
        <source>Force (ignore warnings)</source>
        <translation>Forçar (ignorar avisos)</translation>
    </message>
    <message>
        <source>Performance</source>
        <translation>Desempenho</translation>
    </message>
    <message>
        <source>Enable optimized kernels (limits password length)</source>
        <translation>Ativar kernels otimizados (limita o comprimento da senha)</translation>
    </message>
    <message>
        <source>Print only the attack’s speed and quit, without cracking. Useful for benchmarking and comparing settings.</source>
        <translation>Mostrar apenas a velocidade do ataque e sair, sem quebrar. Útil para benchmarks e para comparar configurações.</translation>
    </message>
    <message>
        <source>Return expected speed of the attack, then quit</source>
        <translation>Retornar a velocidade esperada do ataque e sair</translation>
    </message>
    <message>
        <source>Resources</source>
        <translation>Recursos</translation>
    </message>
    <message>
        <source>CPU affinity:</source>
        <translation>Afinidade de CPU:</translation>
    </message>
    <message>
        <source>Backend devices:</source>
        <translation>Dispositivos de backend:</translation>
    </message>
    <message>
        <source>Device types:</source>
        <translation>Tipos de dispositivo:</translation>
    </message>
    <message>
        <source>Pin hashcat to these CPU devices, separated by commas. Leave empty to let hashcat decide.</source>
        <translation>Fixar o hashcat a estes dispositivos de CPU, separados por vírgulas. Deixe em branco para que o hashcat decida.</translation>
    </message>
    <message>
        <source>Devices to use, separated by commas. Leave empty for all devices.</source>
        <translation>Dispositivos a usar, separados por vírgulas. Deixe em branco para usar todos os dispositivos.</translation>
    </message>
    <message>
        <source>Abort when a device reaches this temperature.</source>
        <translation>Abortar quando um dispositivo atingir esta temperatura.</translation>
    </message>
    <message>
        <source>&#xa0;°C</source>
        <translation>&#xa0;°C</translation>
    </message>
    <message>
        <source>Built-in variables:

&lt;hash&gt; = current hash filename or hash
&lt;unixtime&gt; = current Unix timestamp</source>
        <translation>Variáveis integradas:

&lt;hash&gt; = nome do arquivo de hashes atual ou hash
&lt;unixtime&gt; = carimbo de data/hora Unix atual</translation>
    </message>
    <message>
        <source>Save…</source>
        <translation>Salvar…</translation>
    </message>
    <message>
        <source>Do not write recovered hashes to the potfile.</source>
        <translation>Não gravar os hashes recuperados no potfile.</translation>
    </message>
    <message>
        <source>Settings…</source>
        <translation>Configurações…</translation>
    </message>
    <message>
        <source>Import Settings…</source>
        <translation>Importar configurações…</translation>
    </message>
    <message>
        <source>Export Settings…</source>
        <translation>Exportar configurações…</translation>
    </message>
    <message>
        <source>Export Session…</source>
        <translation>Exportar sessão…</translation>
    </message>
    <message>
        <source>Import Session…</source>
        <translation>Importar sessão…</translation>
    </message>
    <message>
        <source>Abort above</source>
        <translation>Abortar acima de</translation>
    </message>
    <message>
        <source>Cache path:</source>
        <translation>Caminho do cache:</translation>
    </message>
    <message>
        <source>Directory hashcat keeps its cache in. Must exist.</source>
        <translation>Diretório em que o hashcat mantém seu cache. Deve existir.</translation>
    </message>
    <message>
        <source>Abort the session when an OpenCL device hotspot exceeds this many degrees Celsius. 0 disables the thermal limit.</source>
        <translation>Abortar a sessão quando o ponto quente de um dispositivo OpenCL exceder este número de graus Celsius. 0 desativa o limite térmico.</translation>
    </message>
    <message>
        <source>Outfile</source>
        <translation>Arquivo de saída</translation>
    </message>
    <message>
        <source>Write recovered hashes to file:</source>
        <translation>Gravar os hashes recuperados no arquivo:</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Formato:</translation>
    </message>
    <message>
        <source>Write the outfile as JSON. The outfile format is ignored.</source>
        <translation>Gravar o arquivo de saída como JSON. O formato do arquivo de saída é ignorado.</translation>
    </message>
    <message>
        <source>Write JSON</source>
        <translation>Gravar JSON</translation>
    </message>
    <message>
        <source>Do not write the potfile</source>
        <translation>Não gravar o potfile</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>Iniciar</translation>
    </message>
    <message>
        <source>Copy command to clipboard</source>
        <translation>Copiar comando para a área de transferência</translation>
    </message>
    <message>
        <source>Help</source>
        <translation>Ajuda</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Ferramentas</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Arquivo</translation>
    </message>
    <message>
        <source>About</source>
        <translation>Sobre</translation>
    </message>
    <message>
        <source>Load Example Session</source>
        <translation>Carregar sessão de exemplo</translation>
    </message>
    <message>
        <source>Reset Session</source>
        <translation>Redefinir sessão</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation>Sair</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>Sobre o Qt</translation>
    </message>
    <message>
        <source>Built-in charsets:</source>
        <translation>Conjuntos de caracteres integrados:</translation>
    </message>
    <message>
        <source>Outfile format to use, separated by commas.
Default: 1,2</source>
        <translation>Formato do arquivo de saída a usar, separado por vírgulas.
Padrão: 1,2</translation>
    </message>
    <message>
        <source>Common encodings:</source>
        <translation>Codificações comuns:</translation>
    </message>
    <message>
        <source>Force the internal wordlist encoding from this code. Leave empty to let hashcat decide.</source>
        <translation>Forçar a codificação interna da lista de palavras a partir deste código. Deixe em branco para que o hashcat decida.</translation>
    </message>
    <message>
        <source>Force the internal wordlist encoding to this code. Leave empty to let hashcat decide.</source>
        <translation>Forçar a codificação interna da lista de palavras para este código. Deixe em branco para que o hashcat decida.</translation>
    </message>
    <message>
        <source>JSON Files (*.json)</source>
        <translation>Arquivos JSON (*.json)</translation>
    </message>
    <message>
        <source>Exported</source>
        <translation>Exportado</translation>
    </message>
    <message>
        <source>Export failed</source>
        <translation>Falha ao exportar</translation>
    </message>
    <message>
        <source>Import failed</source>
        <translation>Falha ao importar</translation>
    </message>
    <message>
        <source>Imported</source>
        <translation>Importado</translation>
    </message>
    <message>
        <source>The file was made by an older version of hashcat-gui (format version %1; this version reads format version %2).</source>
        <translation>O arquivo foi criado por uma versão mais antiga do hashcat-gui (versão de formato %1; esta versão lê a versão de formato %2).</translation>
    </message>
    <message>
        <source>The file holds sections this import does not use: %1.</source>
        <translation>O arquivo contém seções que esta importação não usa: %1.</translation>
    </message>
    <message>
        <source>The file is in format version %1, but this version reads format version %2, so it was not applied. Update hashcat-gui to read it.</source>
        <translation>O arquivo está na versão de formato %1, mas esta versão lê a versão de formato %2, portanto ele não foi aplicado. Atualize o hashcat-gui para lê-lo.</translation>
    </message>
    <message>
        <source>The session file is in format version %1, but this version reads format version %2, so the session was not restored. The file was left as it is, so the session of this run is not stored either. Update hashcat-gui to read it.</source>
        <translation>O arquivo de sessão está na versão de formato %1, mas esta versão lê a versão de formato %2, portanto a sessão não foi restaurada. O arquivo foi mantido como estava, então a sessão desta execução também não é armazenada. Atualize o hashcat-gui para lê-lo.</translation>
    </message>
    <message>
        <source>Settings exported to %1.</source>
        <translation>Configurações exportadas para %1.</translation>
    </message>
    <message>
        <source>OpenCL device types to use, separated by commas.</source>
        <translation>Tipos de dispositivo OpenCL a usar, separados por vírgulas.</translation>
    </message>
    <message>
        <source>Export session</source>
        <translation>Exportar sessão</translation>
    </message>
    <message>
        <source>Session exported to “%1”.</source>
        <translation>Sessão exportada para “%1”.</translation>
    </message>
    <message>
        <source>Import session</source>
        <translation>Importar sessão</translation>
    </message>
    <message>
        <source>Session imported from “%1”.</source>
        <translation>Sessão importada de “%1”.</translation>
    </message>
    <message>
        <source>Session imported from “%1”.

%2</source>
        <translation>Sessão importada de “%1”.

%2</translation>
    </message>
    <message>
        <source>Export settings</source>
        <translation>Exportar configurações</translation>
    </message>
    <message>
        <source>Import settings</source>
        <translation>Importar configurações</translation>
    </message>
    <message>
        <source>Settings loaded from “%1”.</source>
        <translation>Configurações carregadas de “%1”.</translation>
    </message>
    <message>
        <source>Review them and save to apply them.</source>
        <translation>Revise-as e salve para aplicá-las.</translation>
    </message>
    <message>
        <source>Load example session</source>
        <translation>Carregar sessão de exemplo</translation>
    </message>
    <message>
        <source>Reset session</source>
        <translation>Redefinir sessão</translation>
    </message>
    <message>
        <source>Reset every field to its default? The current session is lost.</source>
        <translation>Redefinir todos os campos para o padrão? A sessão atual será perdida.</translation>
    </message>
    <message>
        <source>Session warning</source>
        <translation>Aviso de sessão</translation>
    </message>
    <message>
        <source>The session file could not be read, so the session was not restored. The file was left in place.

%1</source>
        <translation>Não foi possível ler o arquivo de sessão, portanto a sessão não foi restaurada. O arquivo foi mantido como estava.

%1</translation>
    </message>
    <message>
        <source>The session names the hash type %1, which this hashcat version does not support.</source>
        <translation>A sessão indica o tipo de hash %1, que esta versão do hashcat não suporta.</translation>
    </message>
    <message>
        <source>Save failed</source>
        <translation>Falha ao salvar</translation>
    </message>
    <message>
        <source>Straight</source>
        <translation>Dicionário</translation>
    </message>
    <message>
        <source>Combination</source>
        <translation>Combinação</translation>
    </message>
    <message>
        <source>Brute-force</source>
        <translation>Força bruta</translation>
    </message>
    <message>
        <source>Hybrid Wordlist + Mask</source>
        <translation>Híbrido: lista de palavras + máscara</translation>
    </message>
    <message>
        <source>Hybrid Mask + Wordlist</source>
        <translation>Híbrido: máscara + lista de palavras</translation>
    </message>
    <message>
        <source>Generic (feed)</source>
        <translation>Genérico (feed)</translation>
    </message>
    <message>
        <source>Association</source>
        <translation>Associação</translation>
    </message>
    <message>
        <source>Hybrid (mask + ?w/?q)</source>
        <translation>Híbrido (máscara + ?w/?q)</translation>
    </message>
    <message>
        <source>Updating…</source>
        <translation>Atualizando…</translation>
    </message>
    <message>
        <source>Failed to load hash types</source>
        <translation>Falha ao carregar os tipos de hash</translation>
    </message>
    <message>
        <source>This mode needs exactly 2 wordlists (left, right).</source>
        <translation>Este modo precisa de exatamente 2 listas de palavras (esquerda, direita).</translation>
    </message>
    <message>
        <source>This mode runs hashcat’s bundled default-passwords ruleset; name one or more to use your own.</source>
        <translation>Este modo executa o conjunto de regras default-passwords incluído no hashcat; informe um ou mais para usar os seus.</translation>
    </message>
    <message>
        <source>This mode needs one wordlist and at least one table.</source>
        <translation>Este modo precisa de uma lista de palavras e de pelo menos uma tabela.</translation>
    </message>
    <message>
        <source>This mode needs a feed. Arguments depend on the feed — random and stdin need none, wordlist needs a path.</source>
        <translation>Este modo precisa de um feed. Os argumentos dependem do feed: random e stdin não precisam de nenhum, wordlist precisa de um caminho.</translation>
    </message>
    <message>
        <source>This mode needs at most one wordlist. Without one the words come from the hash file and the settings line can bound the run; with one the two files are paired by line.</source>
        <translation>Este modo precisa de no máximo uma lista de palavras. Sem ela, as palavras vêm do arquivo de hashes e a linha de configurações pode limitar a execução; com uma, os dois arquivos são emparelhados linha a linha.</translation>
    </message>
    <message>
        <source>This mode needs a mask with exactly one ?w; a ?q after ?w names a second wordlist.</source>
        <translation>Este modo precisa de uma máscara com exatamente um ?w; um ?q depois de ?w indica uma segunda lista de palavras.</translation>
    </message>
    <message>
        <source>Open hash file</source>
        <translation>Abrir arquivo de hashes</translation>
    </message>
    <message>
        <source>Save output file</source>
        <translation>Salvar arquivo de saída</translation>
    </message>
    <message>
        <source>Open cache directory</source>
        <translation>Abrir diretório de cache</translation>
    </message>
    <message>
        <source>Add wordlists</source>
        <translation>Adicionar listas de palavras</translation>
    </message>
    <message>
        <source>Add wordlist folder</source>
        <translation>Adicionar pasta de listas de palavras</translation>
    </message>
    <message>
        <source>Add rulesets</source>
        <translation>Adicionar conjuntos de regras</translation>
    </message>
    <message>
        <source>Add ruleset folder</source>
        <translation>Adicionar pasta de conjunto de regras</translation>
    </message>
    <message>
        <source>Add tables</source>
        <translation>Adicionar tabelas</translation>
    </message>
    <message>
        <source>Open feed file</source>
        <translation>Abrir arquivo de feed</translation>
    </message>
    <message>
        <source>This folder has no wordlists. hashcat reads the regular files directly inside it, not subfolders.</source>
        <translation>Esta pasta não tem listas de palavras. O hashcat lê os arquivos comuns diretamente dentro dela, não as subpastas.</translation>
    </message>
    <message>
        <source>This ruleset folder has no Grammar/grammar.txt:
%1</source>
        <translation>Esta pasta de conjunto de regras não tem Grammar/grammar.txt:
%1</translation>
    </message>
    <message>
        <source>This ruleset does not exist:
%1</source>
        <translation>Este conjunto de regras não existe:
%1</translation>
    </message>
    <message>
        <source>A table must be a file; hashcat does not read a folder:
%1</source>
        <translation>Uma tabela deve ser um arquivo; o hashcat não lê uma pasta:
%1</translation>
    </message>
    <message>
        <source>This table does not exist:
%1</source>
        <translation>Esta tabela não existe:
%1</translation>
    </message>
    <message>
        <source>Open rules file</source>
        <translation>Abrir arquivo de regras</translation>
    </message>
    <message>
        <source>Could not start “%1”: %2</source>
        <translation>Não foi possível iniciar “%1”: %2</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not available. Available terminals: %2
Navigate to %3 → %4 to change it.</source>
        <translation>O terminal configurado “%1” não está disponível. Terminais disponíveis: %2
Acesse %3 → %4 para alterá-lo.</translation>
    </message>
    <message>
        <source>This mode needs exactly %1 wordlists (left, right).
Currently selected: %2.</source>
        <translation>Este modo precisa de exatamente %1 listas de palavras (esquerda, direita).
Selecionadas no momento: %2.</translation>
    </message>
    <message>
        <source>Failed to obtain supported hash types.
Error: %1</source>
        <translation>Falha ao obter os tipos de hash suportados.
Erro: %1</translation>
    </message>
    <message>
        <source>The hash types could not be loaded. Check the hashcat path in Settings.</source>
        <translation>Não foi possível carregar os tipos de hash. Verifique o caminho do hashcat nas configurações.</translation>
    </message>
    <message>
        <source>Set the hashcat path under %1 → %2 to load the available hash types.</source>
        <translation>Defina o caminho do hashcat em %1 → %2 para carregar os tipos de hash disponíveis.</translation>
    </message>
    <message>
        <source>This mode needs a wordlist and a mask.</source>
        <translation>Este modo precisa de uma lista de palavras e de uma máscara.</translation>
    </message>
    <message>
        <source>This mode needs a mask.</source>
        <translation>Este modo precisa de uma máscara.</translation>
    </message>
    <message>
        <source>Load the %1 example? The current session is lost.

The example cracks a test hash; hashcat should recover the password %2.</source>
        <translation>Carregar o exemplo %1? A sessão atual será perdida.

O exemplo quebra um hash de teste; o hashcat deve recuperar a senha %2.</translation>
    </message>
    <message>
        <source>Could not create the example directory “%1”.</source>
        <translation>Não foi possível criar o diretório de exemplo “%1”.</translation>
    </message>
    <message>
        <source>Could not read the bundled file “%1”: %2</source>
        <translation>Não foi possível ler o arquivo incluído “%1”: %2</translation>
    </message>
    <message>
        <source>Could not write “%1”: %2</source>
        <translation>Não foi possível gravar “%1”: %2</translation>
    </message>
    <message>
        <source>This mode needs at least one wordlist.</source>
        <translation>Este modo precisa de pelo menos uma lista de palavras.</translation>
    </message>
    <message>
        <source>This wordlist does not exist:
%1</source>
        <translation>Esta lista de palavras não existe:
%1</translation>
    </message>
    <message>
        <source>Copied to the clipboard</source>
        <translation>Copiado para a área de transferência</translation>
    </message>
    <message>
        <source>Launch failed</source>
        <translation>Falha ao iniciar</translation>
    </message>
    <message>
        <source>Please choose a hash file.</source>
        <translation>Escolha um arquivo de hashes.</translation>
    </message>
    <message>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to configure the path to the hashcat executable.</source>
        <translation>Acesse &lt;b&gt;%1 → %2&lt;/b&gt; para configurar o caminho do executável do hashcat.</translation>
    </message>
    <message>
        <source>The configured hashcat executable does not exist:
%1
Navigate to %2 → %3 to change it.</source>
        <translation>O executável do hashcat configurado não existe:
%1
Acesse %2 → %3 para alterá-lo.</translation>
    </message>
    <message>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to select the terminal used for launching.</source>
        <translation>Acesse &lt;b&gt;%1 → %2&lt;/b&gt; para selecionar o terminal usado na inicialização.</translation>
    </message>
    <message>
        <source>This mode takes at most %1 wordlists.
Currently selected: %2.</source>
        <translation>Este modo aceita no máximo %1 listas de palavras.
Selecionadas no momento: %2.</translation>
    </message>
    <message>
        <source>hashcat pairs the two files by line here; this GUI offers only one wordlist for that role.</source>
        <translation>Aqui o hashcat emparelha os dois arquivos linha a linha; esta interface oferece apenas uma lista de palavras para esse papel.</translation>
    </message>
    <message>
        <source>This wordlist has to be a file: hashcat reads it as the multiplied wordlist (the ?q side), and a folder cannot be used there:
%1</source>
        <translation>Esta lista de palavras deve ser um arquivo: o hashcat a lê como a lista de palavras multiplicada (o lado ?q), e uma pasta não pode ser usada ali:
%1</translation>
    </message>
    <message>
        <source>This mode needs at least one table.</source>
        <translation>Este modo precisa de pelo menos uma tabela.</translation>
    </message>
    <message>
        <source>This mode needs a feed: a bundled name (wordlist, stdin, random, …) or the path of a feed file.</source>
        <translation>Este modo precisa de um feed: um nome incluído (wordlist, stdin, random, …) ou o caminho de um arquivo de feed.</translation>
    </message>
    <message>
        <source>This mode pairs the wordlists with the mask; the mask needs a ?w for the word.</source>
        <translation>Este modo emparelha as listas de palavras com a máscara; a máscara precisa de um ?w para a palavra.</translation>
    </message>
    <message>
        <source>The mask contains more than one ?w.</source>
        <translation>A máscara contém mais de um ?w.</translation>
    </message>
    <message>
        <source>The mask contains a ?q before the first ?w.</source>
        <translation>A máscara contém um ?q antes do primeiro ?w.</translation>
    </message>
    <message>
        <source>The mask contains more than one ?q.</source>
        <translation>A máscara contém mais de um ?q.</translation>
    </message>
    <message>
        <source>The mask names a second word with ?q, but only one wordlist is selected.</source>
        <translation>A máscara indica uma segunda palavra com ?q, mas apenas uma lista de palavras está selecionada.</translation>
    </message>
    <message>
        <source>Two wordlists are selected, but the mask has no ?q for the second one.</source>
        <translation>Duas listas de palavras estão selecionadas, mas a máscara não tem ?q para a segunda.</translation>
    </message>
    <message>
        <source>The increment is not offered in this hybrid mode.</source>
        <translation>O incremento não é oferecido neste modo híbrido.</translation>
    </message>
    <message>
        <source>The ?w and ?q markers are only offered in the hybrid mode (mask + wordlists).</source>
        <translation>Os marcadores ?w e ?q só são oferecidos no modo híbrido (máscara + listas de palavras).</translation>
    </message>
    <message>
        <source>The setting “%1” is not valid for this mode.</source>
        <translation>A configuração “%1” não é válida para este modo.</translation>
    </message>
    <message>
        <source>The token “%1” is not a setting; settings are written as key=value.</source>
        <translation>O token “%1” não é uma configuração; as configurações são escritas como key=value.</translation>
    </message>
    <message>
        <source>The settings line applies only while the words come from the hash file. Clear either the settings line or the wordlist list.</source>
        <translation>A linha de configurações só se aplica enquanto as palavras vêm do arquivo de hashes. Limpe a linha de configurações ou a lista de listas de palavras.</translation>
    </message>
    <message>
        <source>The hash type 4000 describes its hashes as a recipe, for example
md5(md5(salt) . pass). Enter it in the hash recipe field.</source>
        <translation>O tipo de hash 4000 descreve seus hashes como uma receita, por exemplo
md5(md5(salt) . pass). Digite-a no campo de receita do hash.</translation>
    </message>
    <message>
        <source>The minimum increment length is larger than the maximum.</source>
        <translation>O comprimento mínimo do incremento é maior que o máximo.</translation>
    </message>
    <message>
        <source>The cache path must be an existing directory:
%1</source>
        <translation>O caminho do cache deve ser um diretório existente:
%1</translation>
    </message>
    <message>
        <source>The increment cannot be combined with the speed-only option.</source>
        <translation>O incremento não pode ser combinado com a opção somente de velocidade.</translation>
    </message>
    <message>
        <source>Removing cracked hashes is not possible in the association attack mode.</source>
        <translation>Remover hashes quebrados não é possível no modo de ataque de associação.</translation>
    </message>
</context>
<context>
    <name>SettingsDialog</name>
    <message>
        <source>Settings</source>
        <translation>Configurações</translation>
    </message>
    <message>
        <source>Not configured</source>
        <translation>Não configurado</translation>
    </message>
    <message>
        <source>hashcat</source>
        <translation>hashcat</translation>
    </message>
    <message>
        <source>&amp;hashcat executable</source>
        <translation>&amp;Executável do hashcat</translation>
    </message>
    <message>
        <source>Select…</source>
        <translation>Selecionar…</translation>
    </message>
    <message>
        <source>&amp;Terminal</source>
        <translation>&amp;Terminal</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Geral</translation>
    </message>
    <message>
        <source>&amp;Language</source>
        <translation>&amp;Idioma</translation>
    </message>
    <message>
        <source>&amp;Color scheme</source>
        <translation>Esque&amp;ma de cores</translation>
    </message>
    <message>
        <source>Not all color schemes are available on all platforms.</source>
        <translation>Nem todos os esquemas de cores estão disponíveis em todas as plataformas.</translation>
    </message>
    <message>
        <source>&amp;Use short parameters</source>
        <translation>&amp;Usar parâmetros curtos</translation>
    </message>
    <message>
        <source>Session and files</source>
        <translation>Sessão e arquivos</translation>
    </message>
    <message>
        <source>&amp;Restore the last session at startup</source>
        <translation>&amp;Restaurar a última sessão ao iniciar</translation>
    </message>
    <message>
        <source>&amp;Session file</source>
        <translation>Arquivo de &amp;sessão</translation>
    </message>
    <message>
        <source>The file the last session is stored in</source>
        <translation>O arquivo em que a última sessão é armazenada</translation>
    </message>
    <message>
        <source>Open folder</source>
        <translation>Abrir pasta</translation>
    </message>
    <message>
        <source>Open the folder the session is stored in</source>
        <translation>Abrir a pasta em que a sessão é armazenada</translation>
    </message>
    <message>
        <source>Settings &amp;file</source>
        <translation>Arquivo de c&amp;onfigurações</translation>
    </message>
    <message>
        <source>The file the settings are stored in</source>
        <translation>O arquivo em que as configurações são armazenadas</translation>
    </message>
    <message>
        <source>Open the folder the settings are stored in</source>
        <translation>Abrir a pasta em que as configurações são armazenadas</translation>
    </message>
    <message>
        <source>Download from %1</source>
        <translation>Baixar de %1</translation>
    </message>
    <message>
        <source>Use the short option names (-m) instead of the long ones (--hash-type) in the command.</source>
        <translation>Usar os nomes curtos das opções (-m) em vez dos longos (--hash-type) no comando.</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Salvar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Select a terminal</source>
        <translation>Selecionar um terminal</translation>
    </message>
    <message>
        <source>System default</source>
        <translation>Padrão do sistema</translation>
    </message>
    <message>
        <source>Light</source>
        <translation>Claro</translation>
    </message>
    <message>
        <source>Dark</source>
        <translation>Escuro</translation>
    </message>
    <message>
        <source>The hashcat path “%1” was not found on this computer.</source>
        <translation>O caminho do hashcat “%1” não foi encontrado neste computador.</translation>
    </message>
    <message>
        <source>The terminal “%1” is not installed and was not applied.</source>
        <translation>O terminal “%1” não está instalado e não foi aplicado.</translation>
    </message>
    <message>
        <source>The color scheme “%1” is not offered and was not applied.</source>
        <translation>O esquema de cores “%1” não é oferecido e não foi aplicado.</translation>
    </message>
    <message>
        <source>The language “%1” is not offered and was not applied.</source>
        <translation>O idioma “%1” não é oferecido e não foi aplicado.</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not installed.</source>
        <translation>O terminal configurado “%1” não está instalado.</translation>
    </message>
    <message>
        <source>Executable Files (*.exe *.bin hashcat);;All Files (*)</source>
        <translation>Arquivos executáveis (*.exe *.bin hashcat);;Todos os arquivos (*)</translation>
    </message>
    <message>
        <source>No hashcat executable is configured; hashcat cannot be started.</source>
        <translation>Nenhum executável do hashcat está configurado; não é possível iniciar o hashcat.</translation>
    </message>
    <message>
        <source>The configured hashcat executable was not found on this computer.</source>
        <translation>O executável do hashcat configurado não foi encontrado neste computador.</translation>
    </message>
    <message>
        <source>Select the terminal hashcat is started in.</source>
        <translation>Selecione o terminal em que o hashcat é iniciado.</translation>
    </message>
    <message>
        <source>Select the hashcat executable</source>
        <translation>Selecionar o executável do hashcat</translation>
    </message>
    <message>
        <source>Invalid file</source>
        <translation>Arquivo inválido</translation>
    </message>
    <message>
        <source>The selected file is not an executable.</source>
        <translation>O arquivo selecionado não é um executável.</translation>
    </message>
    <message>
        <source>Restart required</source>
        <translation>Reinicialização necessária</translation>
    </message>
    <message>
        <source>Restart hashcat-gui to apply the new language.</source>
        <translation>Reinicie o hashcat-gui para aplicar o novo idioma.</translation>
    </message>
</context>
<context>
    <name>StateFile</name>
    <message>
        <source>The file is not valid JSON: %1</source>
        <translation>O arquivo não é um JSON válido: %1</translation>
    </message>
    <message>
        <source>The file does not contain a JSON object.</source>
        <translation>O arquivo não contém um objeto JSON.</translation>
    </message>
    <message>
        <source>The file does not contain any sections.</source>
        <translation>O arquivo não contém nenhuma seção.</translation>
    </message>
    <message>
        <source>Could not open “%1” for writing: %2</source>
        <translation>Não foi possível abrir “%1” para gravação: %2</translation>
    </message>
    <message>
        <source>Could not write to “%1”: %2</source>
        <translation>Não foi possível gravar em “%1”: %2</translation>
    </message>
    <message>
        <source>Could not open “%1” for reading: %2</source>
        <translation>Não foi possível abrir “%1” para leitura: %2</translation>
    </message>
    <message>
        <source>The file does not contain a “%1” section.</source>
        <translation>O arquivo não contém uma seção “%1”.</translation>
    </message>
    <message>
        <source>The file does not contain a “%1” section; it holds %2.</source>
        <translation>O arquivo não contém uma seção “%1”; ele contém %2.</translation>
    </message>
    <message>
        <source>The “%1” section does not contain any values.</source>
        <translation>A seção “%1” não contém nenhum valor.</translation>
    </message>
    <message>
        <source> and </source>
        <translation> e </translation>
    </message>
    <message>
        <source> and %1 more</source>
        <translation> e mais %1</translation>
    </message>
    <message>
        <source>The file names values this version does not know, so they were not applied: %1.</source>
        <translation>O arquivo cita valores que esta versão não conhece, portanto eles não foram aplicados: %1.</translation>
    </message>
    <message>
        <source>The file holds values of the wrong type, so they were not applied: %1.</source>
        <translation>O arquivo contém valores de tipo incorreto, portanto eles não foram aplicados: %1.</translation>
    </message>
</context>
<context>
    <name>UpdateChecker</name>
    <message>
        <source>GitHub answered with HTTP status %1: %2</source>
        <translation>O GitHub respondeu com o status HTTP %1: %2</translation>
    </message>
    <message>
        <source>GitHub answered with HTTP status %1.</source>
        <translation>O GitHub respondeu com o status HTTP %1.</translation>
    </message>
    <message>
        <source>The release information is not valid JSON.</source>
        <translation>As informações da versão não são um JSON válido.</translation>
    </message>
    <message>
        <source>The release information does not name a version.</source>
        <translation>As informações da versão não indicam nenhuma versão.</translation>
    </message>
    <message>
        <source>The release version %1 is not a valid version number.</source>
        <translation>A versão de lançamento %1 não é um número de versão válido.</translation>
    </message>
    <message>
        <source>The installed version %1 is not a valid version number.</source>
        <translation>A versão instalada %1 não é um número de versão válido.</translation>
    </message>
</context>
</TS>
