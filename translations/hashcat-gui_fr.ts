<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="fr" sourcelanguage="en">
<context>
    <name>AboutDialog</name>
    <message>
        <source>About</source>
        <translation>À propos</translation>
    </message>
    <message>
        <source>Graphical user interface for hashcat, the password recovery utility</source>
        <translation>Interface graphique pour hashcat, l’utilitaire de récupération de mots de passe</translation>
    </message>
    <message>
        <source>Version information</source>
        <translation>Informations sur la version</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Inconnu</translation>
    </message>
    <message>
        <source>Updates</source>
        <translation>Mises à jour</translation>
    </message>
    <message>
        <source>Checking for updates…</source>
        <translation>Recherche de mises à jour…</translation>
    </message>
    <message>
        <source>System information</source>
        <translation>Informations système</translation>
    </message>
    <message>
        <source>Operating system</source>
        <translation>Système d’exploitation</translation>
    </message>
    <message>
        <source>Links</source>
        <translation>Liens</translation>
    </message>
    <message>
        <source>License</source>
        <translation>Licence</translation>
    </message>
    <message>
        <source>Copy the application, toolkit, system and hashcat versions to the clipboard</source>
        <translation>Copier dans le presse-papiers les versions de l’application, du toolkit, du système et de hashcat</translation>
    </message>
    <message>
        <source>Copy details</source>
        <translation>Copier les détails</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Copied to the clipboard</source>
        <translation>Copié dans le presse-papiers</translation>
    </message>
    <message>
        <source>Project page</source>
        <translation>Page du projet</translation>
    </message>
    <message>
        <source>Report an issue</source>
        <translation>Signaler un problème</translation>
    </message>
    <message>
        <source>hashcat</source>
        <translation>hashcat</translation>
    </message>
    <message>
        <source>hashcat FAQ</source>
        <translation>FAQ de hashcat</translation>
    </message>
    <message>
        <source>hashcat-gui is licensed under the %1.</source>
        <translation>hashcat-gui est distribué sous licence %1.</translation>
    </message>
    <message>
        <source>GNU GPL v3.0 or later</source>
        <translation>GNU GPL v3.0 ou ultérieure</translation>
    </message>
    <message>
        <source>Not configured</source>
        <translation>Non configuré</translation>
    </message>
    <message>
        <source>Error: %1</source>
        <translation>Erreur&#xa0;: %1</translation>
    </message>
    <message>
        <source>A newer version is available: %1.</source>
        <translation>Une version plus récente est disponible&#xa0;: %1.</translation>
    </message>
    <message>
        <source>Release page</source>
        <translation>Page de la version</translation>
    </message>
    <message>
        <source>No updates available.</source>
        <translation>Aucune mise à jour disponible.</translation>
    </message>
    <message>
        <source>Could not check for updates.</source>
        <translation>Impossible de rechercher les mises à jour.</translation>
    </message>
    <message>
        <source>Could not check for updates: %1</source>
        <translation>Impossible de rechercher les mises à jour&#xa0;: %1</translation>
    </message>
</context>
<context>
    <name>HashcatInfoParser</name>
    <message>
        <source>Invalid JSON returned from hashcat.</source>
        <translation>hashcat a renvoyé un JSON non valide.</translation>
    </message>
    <message>
        <source>hashcat did not report any hash types.</source>
        <translation>hashcat n’a signalé aucun type de hash.</translation>
    </message>
</context>
<context>
    <name>HelperUtils</name>
    <message>
        <source>The hashcat path is not configured.</source>
        <translation>Le chemin de hashcat n’est pas configuré.</translation>
    </message>
    <message>
        <source>Failed to start hashcat
%1</source>
        <translation>Impossible de démarrer hashcat
%1</translation>
    </message>
    <message>
        <source>hashcat timed out
%1</source>
        <translation>hashcat a dépassé le délai d’attente
%1</translation>
    </message>
    <message>
        <source>No supported terminal was found on this system. hashcat-gui needs one to show the hashcat output.</source>
        <translation>Aucun terminal pris en charge n’a été trouvé sur ce système. hashcat-gui en a besoin d’un pour afficher la sortie de hashcat.</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <source>Hash file or hash:</source>
        <translation>Fichier de hashes ou hash&#xa0;:</translation>
    </message>
    <message>
        <source>The hash, a file of hashes, or a .hc22000/.hccapx WPA capture.</source>
        <translation>Le hash, un fichier de hashes ou une capture WPA .hc22000/.hccapx.</translation>
    </message>
    <message>
        <source>Choose a file of hashes.</source>
        <translation>Choisir un fichier de hashes.</translation>
    </message>
    <message>
        <source>Salt is hex</source>
        <translation>Sel en hexadécimal</translation>
    </message>
    <message>
        <source>Rewrite the hash file without cracked hashes, including potfile-known ones; the potfile is untouched. Refused in attack mode 9.</source>
        <translation>Réécrire le fichier de hashes sans les hashes craqués, y compris ceux déjà connus du potfile&#xa0;; le potfile reste intact. Refusé en mode d’attaque 9.</translation>
    </message>
    <message>
        <source>Remove hash from hash list once it is cracked</source>
        <translation>Retirer le hash de la liste une fois craqué</translation>
    </message>
    <message>
        <source>Ignore the username before the mode’s separator (usually “:”), so accounts sharing a hash are cracked once.</source>
        <translation>Ignorer le nom d’utilisateur avant le séparateur du mode (généralement «&#xa0;:&#xa0;»), afin que les comptes partageant un hash ne soient craqués qu’une fois.</translation>
    </message>
    <message>
        <source>Hash recipe, e.g. “md5(md5(salt) . pass)”. Wrap it in the shell’s outer quotes and put any inner strings in the other kind of quotes; PowerShell needs a backtick before $.</source>
        <translation>Recette de hash, p. ex. «&#xa0;md5(md5(salt) . pass)&#xa0;». Entourez-la des guillemets extérieurs du shell et placez les chaînes internes entre l’autre type de guillemets&#xa0;; PowerShell exige un accent grave avant $.</translation>
    </message>
    <message>
        <source>Move item up</source>
        <translation>Monter l’élément</translation>
    </message>
    <message>
        <source>Move selected wordlist up</source>
        <translation>Monter la liste de mots sélectionnée</translation>
    </message>
    <message>
        <source>Move item down</source>
        <translation>Descendre l’élément</translation>
    </message>
    <message>
        <source>Move selected wordlist down</source>
        <translation>Descendre la liste de mots sélectionnée</translation>
    </message>
    <message>
        <source>Add one or more wordlist files. Each checked row is passed to hashcat.</source>
        <translation>Ajouter un ou plusieurs fichiers de listes de mots. Chaque ligne cochée est transmise à hashcat.</translation>
    </message>
    <message>
        <source>Remove the selected wordlists.</source>
        <translation>Retirer les listes de mots sélectionnées.</translation>
    </message>
    <message>
        <source>Remove wordlists</source>
        <translation>Retirer les listes de mots</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Retirer</translation>
    </message>
    <message>
        <source>Wordlist is hex</source>
        <translation>Liste de mots en hexadécimal</translation>
    </message>
    <message>
        <source>Encoding from:</source>
        <translation>Encodage source&#xa0;:</translation>
    </message>
    <message>
        <source>Encoding to:</source>
        <translation>Encodage cible&#xa0;:</translation>
    </message>
    <message>
        <source>Space-separated key=value settings for attack mode 9, e.g. phases=words,rules or rules.rulefile=…. Only used without a wordlist.</source>
        <translation>Paramètres key=value séparés par des espaces pour le mode d’attaque 9, p. ex. phases=words,rules ou rules.rulefile=…. Utilisés uniquement sans liste de mots.</translation>
    </message>
    <message>
        <source>A ruleset file, a folder with Grammar/grammar.txt, or a bundled name. Unchecked rows are skipped; none runs default-passwords.</source>
        <translation>Un fichier de jeu de règles, un dossier contenant Grammar/grammar.txt ou un nom fourni. Les lignes non cochées sont ignorées&#xa0;; si aucune n’est cochée, default-passwords est exécuté.</translation>
    </message>
    <message>
        <source>Add one or more PCFG ruleset files.</source>
        <translation>Ajouter un ou plusieurs fichiers de jeux de règles PCFG.</translation>
    </message>
    <message>
        <source>Add PCFG rulesets</source>
        <translation>Ajouter des jeux de règles PCFG</translation>
    </message>
    <message>
        <source>Remove the selected rulesets.</source>
        <translation>Retirer les jeux de règles sélectionnés.</translation>
    </message>
    <message>
        <source>Remove rulesets</source>
        <translation>Retirer les jeux de règles</translation>
    </message>
    <message>
        <source>Move selected ruleset up</source>
        <translation>Monter le jeu de règles sélectionné</translation>
    </message>
    <message>
        <source>Move selected ruleset down</source>
        <translation>Descendre le jeu de règles sélectionné</translation>
    </message>
    <message>
        <source>One plain number per ruleset, colon-separated, e.g. 2:1; the count must match the number of rulesets.</source>
        <translation>Un nombre simple par jeu de règles, séparés par des deux-points, p. ex. 2:1&#xa0;; leur nombre doit correspondre à celui des jeux de règles.</translation>
    </message>
    <message>
        <source>Maximum candidate length the kernel builds, in 4-byte words: 0 (auto), 16 or 32.</source>
        <translation>Longueur maximale des candidats que construit le kernel, en mots de 4 octets&#xa0;: 0 (auto), 16 ou 32.</translation>
    </message>
    <message>
        <source>1 steps to the next base word where the order allows instead of unranking it; the candidates are identical, only the fast path is off.</source>
        <translation>1 passe au mot de base suivant lorsque l’ordre le permet, au lieu de le calculer par unranking&#xa0;; les candidats sont identiques, seul le chemin rapide est désactivé.</translation>
    </message>
    <message>
        <source>Only enumerate candidates this mask admits; excludes omen=1, and hintaccount unless hint words come from hintwords or hintfile.</source>
        <translation>Énumérer uniquement les candidats que ce masque admet&#xa0;; exclut omen=1, ainsi que hintaccount sauf si les mots d’indice proviennent de hintwords ou hintfile.</translation>
    </message>
    <message>
        <source>Space-separated key=value settings: omen, cache, lookup, pwmin, pwmax, hintaccount, hintwords, hintfile, hintrank, hintrepeat. Rulesets go first.</source>
        <translation>Paramètres key=value séparés par des espaces&#xa0;: omen, cache, lookup, pwmin, pwmax, hintaccount, hintwords, hintfile, hintrank, hintrepeat. Les jeux de règles viennent en premier.</translation>
    </message>
    <message>
        <source>Add one or more table files.</source>
        <translation>Ajouter un ou plusieurs fichiers de tables.</translation>
    </message>
    <message>
        <source>Remove the selected tables.</source>
        <translation>Retirer les tables sélectionnées.</translation>
    </message>
    <message>
        <source>Remove tables</source>
        <translation>Retirer les tables</translation>
    </message>
    <message>
        <source>1 (default) keeps a token unchanged as a choice, 0 drops it so the table only yields variations. identity=0 excludes single=1.</source>
        <translation>1 (par défaut) conserve un token inchangé comme choix, 0 l’écarte pour que la table ne produise que des variantes. identity=0 exclut single=1.</translation>
    </message>
    <message>
        <source>1 reads the wordlist as patterns: no word is returned unchanged and unmatched words are dropped. 0 (default) keeps them.</source>
        <translation>1 lit la liste de mots comme des motifs&#xa0;: aucun mot n’est renvoyé tel quel et les mots sans correspondance sont écartés. 0 (par défaut) les conserve.</translation>
    </message>
    <message>
        <source>single=1: one substitution per candidate instead of the cross product; fewer candidates, more host work on fast hashes.</source>
        <translation>single=1&#xa0;: une substitution par candidat au lieu du produit cartésien&#xa0;; moins de candidats, plus de travail pour l’hôte avec les hashes rapides.</translation>
    </message>
    <message>
        <source>Keep only the first N replacements of each source word; 0 means no limit. Applies to single=1 too.</source>
        <translation>Ne conserver que les N premières substitutions de chaque mot source&#xa0;; 0 signifie aucune limite. S’applique aussi à single=1.</translation>
    </message>
    <message>
        <source>How much of a word’s cross product the host enumerates before giving up; 0 means no limit.</source>
        <translation>Part du produit cartésien d’un mot que l’hôte énumère avant d’abandonner&#xa0;; 0 signifie aucune limite.</translation>
    </message>
    <message>
        <source>Key=value settings separated by spaces; everything after the first one is a setting, so tables come first.</source>
        <translation>Paramètres key=value séparés par des espaces&#xa0;; tout ce qui suit le premier est un paramètre, les tables viennent donc en premier.</translation>
    </message>
    <message>
        <source>Feed name or path. A slash means a path; otherwise hashcat tries feed_&lt;name&gt;, rust_&lt;name&gt;, then &lt;name&gt; in its feeds directory.</source>
        <translation>Nom ou chemin du feed. Une barre oblique indique un chemin&#xa0;; sinon, hashcat essaie feed_&lt;name&gt;, rust_&lt;name&gt;, puis &lt;name&gt; dans son répertoire de feeds.</translation>
    </message>
    <message>
        <source>Browse for a feed plugin instead of naming it.</source>
        <translation>Parcourir pour trouver un plugin de feed au lieu d’en saisir le nom.</translation>
    </message>
    <message>
        <source>Browse for a feed plugin</source>
        <translation>Parcourir pour trouver un plugin de feed</translation>
    </message>
    <message>
        <source>Argument passed to the feed; meaning and count depend on the feed (the wordlist feed takes wordlist paths).</source>
        <translation>Argument transmis au feed&#xa0;; son sens et leur nombre dépendent du feed (le feed wordlist prend des chemins de listes de mots).</translation>
    </message>
    <message>
        <source>Add an argument for the feed, in order.</source>
        <translation>Ajouter un argument pour le feed, dans l’ordre.</translation>
    </message>
    <message>
        <source>Remove the selected feed arguments.</source>
        <translation>Retirer les arguments de feed sélectionnés.</translation>
    </message>
    <message>
        <source>Remove feed arguments</source>
        <translation>Retirer les arguments de feed</translation>
    </message>
    <message>
        <source>Move selected feed argument up</source>
        <translation>Monter l’argument de feed sélectionné</translation>
    </message>
    <message>
        <source>Move selected feed argument down</source>
        <translation>Descendre l’argument de feed sélectionné</translation>
    </message>
    <message>
        <source>Read rules from the files below instead of generating them.</source>
        <translation>Lire les règles dans les fichiers ci-dessous au lieu de les générer.</translation>
    </message>
    <message>
        <source>Rules file 1:</source>
        <translation>Fichier de règles 1&#xa0;:</translation>
    </message>
    <message>
        <source>Choose the rules file for slot 1.</source>
        <translation>Choisir le fichier de règles pour l’emplacement 1.</translation>
    </message>
    <message>
        <source>Open rules file 1</source>
        <translation>Ouvrir le fichier de règles 1</translation>
    </message>
    <message>
        <source>Rules file 2:</source>
        <translation>Fichier de règles 2&#xa0;:</translation>
    </message>
    <message>
        <source>Choose the rules file for slot 2.</source>
        <translation>Choisir le fichier de règles pour l’emplacement 2.</translation>
    </message>
    <message>
        <source>Open rules file 2</source>
        <translation>Ouvrir le fichier de règles 2</translation>
    </message>
    <message>
        <source>Rules file 3:</source>
        <translation>Fichier de règles 3&#xa0;:</translation>
    </message>
    <message>
        <source>Choose the rules file for slot 3.</source>
        <translation>Choisir le fichier de règles pour l’emplacement 3.</translation>
    </message>
    <message>
        <source>Open rules file 3</source>
        <translation>Ouvrir le fichier de règles 3</translation>
    </message>
    <message>
        <source>Number of random rules hashcat generates (-g). Only used with Generate rules; no rules file is read.</source>
        <translation>Nombre de règles aléatoires que hashcat génère (-g). Utilisé uniquement avec «&#xa0;Générer des règles&#xa0;»&#xa0;; aucun fichier de règles n’est lu.</translation>
    </message>
    <message>
        <source>Treat every -r file as one list: rule counts add instead of multiplying.</source>
        <translation>Traiter chaque fichier -r comme une seule liste&#xa0;: les nombres de règles s’additionnent au lieu de se multiplier.</translation>
    </message>
    <message>
        <source>Wordlist rules</source>
        <translation>Règles de la liste de mots</translation>
    </message>
    <message>
        <source>Left rule:</source>
        <translation>Règle de gauche&#xa0;:</translation>
    </message>
    <message>
        <source>Right rule:</source>
        <translation>Règle de droite&#xa0;:</translation>
    </message>
    <message>
        <source>Interpret the mask and custom charsets as hexadecimal bytes.</source>
        <translation>Interpréter le masque et les jeux de caractères personnalisés comme des octets hexadécimaux.</translation>
    </message>
    <message>
        <source>Charset is hex</source>
        <translation>Jeu de caractères en hexadécimal</translation>
    </message>
    <message>
        <source>Ctrl+Shift+I</source>
        <translation>Ctrl+Shift+I</translation>
    </message>
    <message>
        <source>Increment mask length</source>
        <translation>Incrémenter la longueur du masque</translation>
    </message>
    <message>
        <source>Min:</source>
        <translation>Min&#xa0;:</translation>
    </message>
    <message>
        <source>Max:</source>
        <translation>Max&#xa0;:</translation>
    </message>
    <message>
        <source>Use hashcat’s optimized kernels (-O); they cap the maximum password length.</source>
        <translation>Utiliser les kernels optimisés de hashcat (-O)&#xa0;; ils limitent la longueur maximale du mot de passe.</translation>
    </message>
    <message>
        <source>Pin hashcat to these CPU devices, separated by commas. Leave empty to let hashcat decide.</source>
        <translation>Lier hashcat à ces périphériques CPU, séparés par des virgules. Laisser vide pour que hashcat décide.</translation>
    </message>
    <message>
        <source>Devices to use, separated by commas. Leave empty for all devices.</source>
        <translation>Périphériques à utiliser, séparés par des virgules. Laisser vide pour tous les périphériques.</translation>
    </message>
    <message>
        <source>Abort above</source>
        <translation>Interrompre au-dessus de</translation>
    </message>
    <message>
        <source>Outfile</source>
        <translation>Fichier de sortie</translation>
    </message>
    <message>
        <source>Save…</source>
        <translation>Enregistrer…</translation>
    </message>
    <message>
        <source>Write the outfile as JSON. The outfile format is ignored.</source>
        <translation>Écrire le fichier de sortie au format JSON. Le format du fichier de sortie est ignoré.</translation>
    </message>
    <message>
        <source>Write JSON</source>
        <translation>Écrire en JSON</translation>
    </message>
    <message>
        <source>Do not write recovered hashes to the potfile.</source>
        <translation>Ne pas écrire les hashes retrouvés dans le potfile.</translation>
    </message>
    <message>
        <source>Load Example Session</source>
        <translation>Charger la session d’exemple</translation>
    </message>
    <message>
        <source>Reset Session</source>
        <translation>Réinitialiser la session</translation>
    </message>
    <message>
        <source>Mode:</source>
        <translation>Mode&#xa0;:</translation>
    </message>
    <message>
        <source>Attack</source>
        <translation>Attaque</translation>
    </message>
    <message>
        <source>Open…</source>
        <translation>Ouvrir…</translation>
    </message>
    <message>
        <source>Assume the salt is given in hexadecimal.</source>
        <translation>Considérer que le sel est donné en hexadécimal.</translation>
    </message>
    <message>
        <source>Attack mode. Each choice reveals its own options below.</source>
        <translation>Mode d’attaque. Chaque choix affiche ses propres options ci-dessous.</translation>
    </message>
    <message>
        <source>Hash type:</source>
        <translation>Type de hash&#xa0;:</translation>
    </message>
    <message>
        <source>Hash recipe:</source>
        <translation>Recette de hash&#xa0;:</translation>
    </message>
    <message>
        <source>Add files…</source>
        <translation>Ajouter des fichiers…</translation>
    </message>
    <message>
        <source>hashcat reads every file directly inside the folder as one wordlist.</source>
        <translation>hashcat lit chaque fichier situé directement dans le dossier comme une liste de mots.</translation>
    </message>
    <message>
        <source>Add folder…</source>
        <translation>Ajouter un dossier…</translation>
    </message>
    <message>
        <source>Only checked rows are used. A folder is one wordlist: hashcat reads the files directly inside it.</source>
        <translation>Seules les lignes cochées sont utilisées. Un dossier est une liste de mots&#xa0;: hashcat lit les fichiers situés directement dedans.</translation>
    </message>
    <message>
        <source>Assume words in the wordlist are given in hexadecimal.</source>
        <translation>Considérer que les mots de la liste de mots sont donnés en hexadécimal.</translation>
    </message>
    <message>
        <source>Settings (no wordlist):</source>
        <translation>Paramètres (sans liste de mots)&#xa0;:</translation>
    </message>
    <message>
        <source>PCFG</source>
        <translation>PCFG</translation>
    </message>
    <message>
        <source>A ruleset folder needs a Grammar/grammar.txt.</source>
        <translation>Un dossier de jeu de règles nécessite un Grammar/grammar.txt.</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Scale&#xa0;:</translation>
    </message>
    <message>
        <source>Quantization steps per bit (1–64)</source>
        <translation>Pas de quantification par bit (1–64)</translation>
    </message>
    <message>
        <source>Costmax:</source>
        <translation>Costmax&#xa0;:</translation>
    </message>
    <message>
        <source>Highest cost level to enumerate (1–64)</source>
        <translation>Niveau de coût le plus élevé à énumérer (1–64)</translation>
    </message>
    <message>
        <source>Weights:</source>
        <translation>Weights&#xa0;:</translation>
    </message>
    <message>
        <source>Threads:</source>
        <translation>Threads&#xa0;:</translation>
    </message>
    <message>
        <source>Cores that generate base words (0–16); 0 runs on the calling thread.</source>
        <translation>Cœurs qui génèrent les mots de base (0–16)&#xa0;; 0 s’exécute dans le thread appelant.</translation>
    </message>
    <message>
        <source>auto</source>
        <translation>auto</translation>
    </message>
    <message>
        <source>Kbits:</source>
        <translation>Kbits&#xa0;:</translation>
    </message>
    <message>
        <source>Bits one inner loop may span (0–27)</source>
        <translation>Bits qu’une boucle interne peut couvrir (0–27)</translation>
    </message>
    <message>
        <source>Maxword:</source>
        <translation>Maxword&#xa0;:</translation>
    </message>
    <message>
        <source>Maxgain:</source>
        <translation>Maxgain&#xa0;:</translation>
    </message>
    <message>
        <source>1.0–64.0; decimal point is a dot, not the locale separator.</source>
        <translation>1.0–64.0&#xa0;; le séparateur décimal est un point, et non le séparateur des paramètres régionaux.</translation>
    </message>
    <message>
        <source>Walk:</source>
        <translation>Walk&#xa0;:</translation>
    </message>
    <message>
        <source>Mask:</source>
        <translation>Masque&#xa0;:</translation>
    </message>
    <message>
        <source>Extra settings (key=value):</source>
        <translation>Paramètres supplémentaires (key=value)&#xa0;:</translation>
    </message>
    <message>
        <source>Table</source>
        <translation>Table</translation>
    </message>
    <message>
        <source>Every checked row after the wordlist is a table.</source>
        <translation>Chaque ligne cochée après la liste de mots est une table.</translation>
    </message>
    <message>
        <source>Add tables…</source>
        <translation>Ajouter des tables…</translation>
    </message>
    <message>
        <source>Identity:</source>
        <translation>Identity&#xa0;:</translation>
    </message>
    <message>
        <source>Template:</source>
        <translation>Template&#xa0;:</translation>
    </message>
    <message>
        <source>Single:</source>
        <translation>Single&#xa0;:</translation>
    </message>
    <message>
        <source>Cap:</source>
        <translation>Cap&#xa0;:</translation>
    </message>
    <message>
        <source>Maxperm:</source>
        <translation>Maxperm&#xa0;:</translation>
    </message>
    <message>
        <source>Generic feed</source>
        <translation>Feed générique</translation>
    </message>
    <message>
        <source>Feed:</source>
        <translation>Feed&#xa0;:</translation>
    </message>
    <message>
        <source>wordlist, stdin, random, association, pcfg, table, hybrid, mask, or a feed path</source>
        <translation>wordlist, stdin, random, association, pcfg, table, hybrid, mask ou un chemin de feed</translation>
    </message>
    <message>
        <source>Browse…</source>
        <translation>Parcourir…</translation>
    </message>
    <message>
        <source>Argument:</source>
        <translation>Argument&#xa0;:</translation>
    </message>
    <message>
        <source>Add</source>
        <translation>Ajouter</translation>
    </message>
    <message>
        <source>Arguments the feed reads, in order; only checked rows are used.</source>
        <translation>Arguments lus par le feed, dans l’ordre&#xa0;; seules les lignes cochées sont utilisées.</translation>
    </message>
    <message>
        <source>Rules</source>
        <translation>Règles</translation>
    </message>
    <message>
        <source>Use rules files:</source>
        <translation>Utiliser des fichiers de règles&#xa0;:</translation>
    </message>
    <message>
        <source>Enable rules file slot 1 for this run.</source>
        <translation>Activer l’emplacement 1 de fichier de règles pour cette exécution.</translation>
    </message>
    <message>
        <source>Rules file for slot 1; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>Fichier de règles pour l’emplacement 1&#xa0;; appliqué à chaque mot. Laissez vide pour ne pas utiliser l’emplacement.</translation>
    </message>
    <message>
        <source>Enable rules file slot 2 for this run.</source>
        <translation>Activer l’emplacement 2 de fichier de règles pour cette exécution.</translation>
    </message>
    <message>
        <source>Rules file for slot 2; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>Fichier de règles pour l’emplacement 2&#xa0;; appliqué à chaque mot. Laissez vide pour ne pas utiliser l’emplacement.</translation>
    </message>
    <message>
        <source>Enable rules file slot 3 for this run.</source>
        <translation>Activer l’emplacement 3 de fichier de règles pour cette exécution.</translation>
    </message>
    <message>
        <source>Rules file for slot 3; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>Fichier de règles pour l’emplacement 3&#xa0;; appliqué à chaque mot. Laissez vide pour ne pas utiliser l’emplacement.</translation>
    </message>
    <message>
        <source>Generate the rules automatically instead of reading them from a file.</source>
        <translation>Générer les règles automatiquement au lieu de les lire dans un fichier.</translation>
    </message>
    <message>
        <source>Generate rules:</source>
        <translation>Générer des règles&#xa0;:</translation>
    </message>
    <message>
        <source>Join -r files into one list</source>
        <translation>Réunir les fichiers -r en une seule liste</translation>
    </message>
    <message>
        <source>Single rule applied to each word from the left wordlist.</source>
        <translation>Règle unique appliquée à chaque mot de la liste de mots de gauche.</translation>
    </message>
    <message>
        <source>Single rule applied to each word from the right wordlist.</source>
        <translation>Règle unique appliquée à chaque mot de la liste de mots de droite.</translation>
    </message>
    <message>
        <source>Mask</source>
        <translation>Masque</translation>
    </message>
    <message>
        <source>Built-in charsets:</source>
        <translation>Jeux de caractères intégrés&#xa0;:</translation>
    </message>
    <message>
        <source>Custom charsets</source>
        <translation>Jeux de caractères personnalisés</translation>
    </message>
    <message>
        <source>Charset 1:</source>
        <translation>Jeu de caractères 1&#xa0;:</translation>
    </message>
    <message>
        <source>Charset 2:</source>
        <translation>Jeu de caractères 2&#xa0;:</translation>
    </message>
    <message>
        <source>Charset 3:</source>
        <translation>Jeu de caractères 3&#xa0;:</translation>
    </message>
    <message>
        <source>Charset 4:</source>
        <translation>Jeu de caractères 4&#xa0;:</translation>
    </message>
    <message>
        <source>Session and limits</source>
        <translation>Session et limites</translation>
    </message>
    <message>
        <source>Session name:</source>
        <translation>Nom de la session&#xa0;:</translation>
    </message>
    <message>
        <source>Name hashcat uses for its restore file and status output.</source>
        <translation>Nom que hashcat utilise pour son fichier de restauration et sa sortie d’état.</translation>
    </message>
    <message>
        <source>Runtime limit:</source>
        <translation>Durée maximale&#xa0;:</translation>
    </message>
    <message>
        <source>unlimited</source>
        <translation>illimitée</translation>
    </message>
    <message>
        <source>Skip:</source>
        <translation>Ignorer&#xa0;:</translation>
    </message>
    <message>
        <source>Limit:</source>
        <translation>Limite&#xa0;:</translation>
    </message>
    <message>
        <source>Keep updating the run status (speed, progress, ETA) instead of printing one final line.</source>
        <translation>Continuer à mettre à jour l’état de l’exécution (vitesse, progression, durée restante) au lieu d’afficher une seule ligne finale.</translation>
    </message>
    <message>
        <source>Update status automatically</source>
        <translation>Mettre à jour l’état automatiquement</translation>
    </message>
    <message>
        <source>Seconds between status updates; only used when automatic status is enabled.</source>
        <translation>Secondes entre les mises à jour de l’état&#xa0;; utilisé uniquement lorsque l’état automatique est activé.</translation>
    </message>
    <message>
        <source>Ignore warnings and start hashcat anyway. Use with care.</source>
        <translation>Ignorer les avertissements et lancer hashcat malgré tout. À utiliser avec précaution.</translation>
    </message>
    <message>
        <source>Force (ignore warnings)</source>
        <translation>Forcer (ignorer les avertissements)</translation>
    </message>
    <message>
        <source>Performance</source>
        <translation>Performances</translation>
    </message>
    <message>
        <source>Cache path:</source>
        <translation>Chemin du cache&#xa0;:</translation>
    </message>
    <message>
        <source>Directory hashcat keeps its cache in. Must exist.</source>
        <translation>Répertoire dans lequel hashcat conserve son cache. Doit exister.</translation>
    </message>
    <message>
        <source>Choose the cache directory.</source>
        <translation>Choisir le répertoire du cache.</translation>
    </message>
    <message>
        <source>Choose cache directory</source>
        <translation>Choisir le répertoire du cache</translation>
    </message>
    <message>
        <source>Resources</source>
        <translation>Ressources</translation>
    </message>
    <message>
        <source>CPU affinity:</source>
        <translation>Affinité CPU&#xa0;:</translation>
    </message>
    <message>
        <source>Backend devices:</source>
        <translation>Périphériques backend&#xa0;:</translation>
    </message>
    <message>
        <source>Device types:</source>
        <translation>Types de périphériques&#xa0;:</translation>
    </message>
    <message>
        <source>Abort when any backend device exceeds this temperature in °C; 0 disables it.</source>
        <translation>Interrompre lorsqu’un périphérique backend dépasse cette température en °C&#xa0;; 0 désactive cette limite.</translation>
    </message>
    <message>
        <source>Write recovered hashes to the file below, in the selected formats.</source>
        <translation>Écrire les hashes retrouvés dans le fichier ci-dessous, dans les formats sélectionnés.</translation>
    </message>
    <message>
        <source>Write recovered hashes to file:</source>
        <translation>Écrire les hashes retrouvés dans le fichier&#xa0;:</translation>
    </message>
    <message>
        <source>The file the recovered hashes are written to. Format variables:

&lt;hash&gt; = current hash filename or hash
&lt;unixtime&gt; = current Unix timestamp</source>
        <translation>Le fichier dans lequel les hashes retrouvés sont écrits. Variables de format&#xa0;:

&lt;hash&gt; = nom du fichier de hashes actuel ou hash
&lt;unixtime&gt; = horodatage Unix actuel</translation>
    </message>
    <message>
        <source>Choose the outfile path.</source>
        <translation>Choisir le chemin du fichier de sortie.</translation>
    </message>
    <message>
        <source>Choose outfile</source>
        <translation>Choisir le fichier de sortie</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Format&#xa0;:</translation>
    </message>
    <message>
        <source>Do not write the potfile</source>
        <translation>Ne pas écrire le potfile</translation>
    </message>
    <message>
        <source>Run hashcat in a terminal with these options.</source>
        <translation>Lancer hashcat dans un terminal avec ces options.</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>Démarrer</translation>
    </message>
    <message>
        <source>Copy command to clipboard</source>
        <translation>Copier la commande dans le presse-papiers</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Général</translation>
    </message>
    <message>
        <source>Ignore username in hash file</source>
        <translation>Ignorer le nom d’utilisateur dans le fichier de hashes</translation>
    </message>
    <message>
        <source>Wordlists</source>
        <translation>Listes de mots</translation>
    </message>
    <message>
        <source>Increment the mask length from the minimum to the maximum.</source>
        <translation>Incrémenter la longueur du masque du minimum au maximum.</translation>
    </message>
    <message>
        <source>Minimum mask length; 0 uses hashcat’s default.</source>
        <translation>Longueur minimale du masque&#xa0;; 0 utilise la valeur par défaut de hashcat.</translation>
    </message>
    <message>
        <source>Maximum mask length; 0 uses hashcat’s default.</source>
        <translation>Longueur maximale du masque&#xa0;; 0 utilise la valeur par défaut de hashcat.</translation>
    </message>
    <message>
        <source>Abort the session after this many seconds; 0 runs until hashcat finishes.</source>
        <translation>Interrompre la session après ce nombre de secondes&#xa0;; 0 s’exécute jusqu’à ce que hashcat termine.</translation>
    </message>
    <message>
        <source>&#xa0;s</source>
        <translation>&#xa0;s</translation>
    </message>
    <message>
        <source>Skip this many candidates from the start.</source>
        <translation>Ignorer ce nombre de candidats depuis le début.</translation>
    </message>
    <message>
        <source>Only test this many candidates after the skipped ones.</source>
        <translation>Tester uniquement ce nombre de candidats après ceux ignorés.</translation>
    </message>
    <message>
        <source>Enable optimized kernels (limits password length)</source>
        <translation>Activer les kernels optimisés (limite la longueur du mot de passe)</translation>
    </message>
    <message>
        <source>Print only the attack’s speed, then quit; no cracking.</source>
        <translation>Afficher uniquement la vitesse de l’attaque, puis quitter&#xa0;; aucun craquage.</translation>
    </message>
    <message>
        <source>Return expected speed of the attack, then quit</source>
        <translation>Afficher la vitesse attendue de l’attaque, puis quitter</translation>
    </message>
    <message>
        <source>&#xa0;°C</source>
        <translation>&#xa0;°C</translation>
    </message>
    <message>
        <source>Copy command</source>
        <translation>Copier la commande</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>&amp;Aide</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Outils</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fichier</translation>
    </message>
    <message>
        <source>Ctrl+Q</source>
        <translation>Ctrl+Q</translation>
    </message>
    <message>
        <source>Settings…</source>
        <translation>Paramètres…</translation>
    </message>
    <message>
        <source>Ctrl+,</source>
        <translation>Ctrl+,</translation>
    </message>
    <message>
        <source>Import Settings…</source>
        <translation>Importer les paramètres…</translation>
    </message>
    <message>
        <source>Ctrl+Shift+O</source>
        <translation>Ctrl+Shift+O</translation>
    </message>
    <message>
        <source>Export Settings…</source>
        <translation>Exporter les paramètres…</translation>
    </message>
    <message>
        <source>Ctrl+Shift+S</source>
        <translation>Ctrl+Shift+S</translation>
    </message>
    <message>
        <source>Export Session…</source>
        <translation>Exporter la session…</translation>
    </message>
    <message>
        <source>Ctrl+Shift+E</source>
        <translation>Ctrl+Shift+E</translation>
    </message>
    <message>
        <source>Import Session…</source>
        <translation>Importer la session…</translation>
    </message>
    <message>
        <source>About</source>
        <translation>À propos</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation>Quitter</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>À propos de Qt</translation>
    </message>
    <message>
        <source>JSON Files (*.json)</source>
        <translation>Fichiers JSON (*.json)</translation>
    </message>
    <message>
        <source>The session file could not be read, so the session was not restored. The file was left in place.

%1</source>
        <translation>Le fichier de session n’a pas pu être lu, la session n’a donc pas été restaurée. Le fichier a été laissé tel quel.

%1</translation>
    </message>
    <message>
        <source>Save failed</source>
        <translation>Échec de l’enregistrement</translation>
    </message>
    <message>
        <source>Outfile format to use, separated by commas.
Default: 1,2</source>
        <translation>Format du fichier de sortie à utiliser, séparé par des virgules.
Par défaut&#xa0;: 1,2</translation>
    </message>
    <message>
        <source>Common encodings:</source>
        <translation>Encodages courants&#xa0;:</translation>
    </message>
    <message>
        <source>Force the internal wordlist encoding from this code. Leave empty to let hashcat decide.</source>
        <translation>Forcer l’encodage interne de la liste de mots à partir de ce code. Laisser vide pour que hashcat décide.</translation>
    </message>
    <message>
        <source>Force the internal wordlist encoding to this code. Leave empty to let hashcat decide.</source>
        <translation>Forcer l’encodage interne de la liste de mots vers ce code. Laisser vide pour que hashcat décide.</translation>
    </message>
    <message>
        <source>Exported</source>
        <translation>Exportation terminée</translation>
    </message>
    <message>
        <source>Export failed</source>
        <translation>Échec de l’exportation</translation>
    </message>
    <message>
        <source>Import failed</source>
        <translation>Échec de l’importation</translation>
    </message>
    <message>
        <source>Imported</source>
        <translation>Importation terminée</translation>
    </message>
    <message>
        <source>The file was made by an older version of hashcat-gui (format version %1; this version reads format version %2).</source>
        <translation>Le fichier a été créé par une version plus ancienne de hashcat-gui (version de format %1&#xa0;; cette version lit la version de format %2).</translation>
    </message>
    <message>
        <source>The file holds sections this import does not use: %1.</source>
        <translation>Le fichier contient des sections que cette importation n’utilise pas&#xa0;: %1.</translation>
    </message>
    <message>
        <source>The file is in format version %1, but this version reads format version %2, so it was not applied. Update hashcat-gui to read it.</source>
        <translation>Le fichier est en version de format %1, mais cette version lit la version de format %2, il n’a donc pas été appliqué. Mettez hashcat-gui à jour pour le lire.</translation>
    </message>
    <message>
        <source>The session file is in format version %1, but this version reads format version %2, so the session was not restored. The file was left as it is, so the session of this run is not stored either. Update hashcat-gui to read it.</source>
        <translation>Le fichier de session est en version de format %1, mais cette version lit la version de format %2, la session n’a donc pas été restaurée. Le fichier a été laissé tel quel, la session de cette exécution n’est donc pas enregistrée non plus. Mettez hashcat-gui à jour pour le lire.</translation>
    </message>
    <message>
        <source>Settings exported to %1.</source>
        <translation>Paramètres exportés vers %1.</translation>
    </message>
    <message>
        <source>OpenCL device types to use, separated by commas.</source>
        <translation>Types de périphériques OpenCL à utiliser, séparés par des virgules.</translation>
    </message>
    <message>
        <source>Export session</source>
        <translation>Exporter la session</translation>
    </message>
    <message>
        <source>Session exported to “%1”.</source>
        <translation>Session exportée vers «&#xa0;%1&#xa0;».</translation>
    </message>
    <message>
        <source>Import session</source>
        <translation>Importer la session</translation>
    </message>
    <message>
        <source>Session imported from “%1”.</source>
        <translation>Session importée depuis «&#xa0;%1&#xa0;».</translation>
    </message>
    <message>
        <source>Session imported from “%1”.

%2</source>
        <translation>Session importée depuis «&#xa0;%1&#xa0;».

%2</translation>
    </message>
    <message>
        <source>Export settings</source>
        <translation>Exporter les paramètres</translation>
    </message>
    <message>
        <source>Import settings</source>
        <translation>Importer les paramètres</translation>
    </message>
    <message>
        <source>Settings loaded from “%1”.</source>
        <translation>Paramètres chargés depuis «&#xa0;%1&#xa0;».</translation>
    </message>
    <message>
        <source>Review them and save to apply them.</source>
        <translation>Vérifiez-les et enregistrez pour les appliquer.</translation>
    </message>
    <message>
        <source>Load example session</source>
        <translation>Charger la session d’exemple</translation>
    </message>
    <message>
        <source>Reset session</source>
        <translation>Réinitialiser la session</translation>
    </message>
    <message>
        <source>Reset every field to its default? The current session is lost.</source>
        <translation>Réinitialiser tous les champs à leur valeur par défaut&#xa0;? La session actuelle sera perdue.</translation>
    </message>
    <message>
        <source>Session warning</source>
        <translation>Avertissement de session</translation>
    </message>
    <message>
        <source>Updating…</source>
        <translation>Mise à jour…</translation>
    </message>
    <message>
        <source>Failed to load hash types</source>
        <translation>Échec du chargement des types de hash</translation>
    </message>
    <message>
        <source>This mode needs a wordlist and a mask.</source>
        <translation>Ce mode nécessite une liste de mots et un masque.</translation>
    </message>
    <message>
        <source>This mode needs at least one wordlist.</source>
        <translation>Ce mode nécessite au moins une liste de mots.</translation>
    </message>
    <message>
        <source>Open hash file</source>
        <translation>Ouvrir un fichier de hashes</translation>
    </message>
    <message>
        <source>Save output file</source>
        <translation>Enregistrer le fichier de sortie</translation>
    </message>
    <message>
        <source>Add wordlists</source>
        <translation>Ajouter des listes de mots</translation>
    </message>
    <message>
        <source>Open rules file</source>
        <translation>Ouvrir un fichier de règles</translation>
    </message>
    <message>
        <source>Could not start “%1”: %2</source>
        <translation>Impossible de démarrer «&#xa0;%1&#xa0;»&#xa0;: %2</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not available. Available terminals: %2
Navigate to %3 → %4 to change it.</source>
        <translation>Le terminal configuré «&#xa0;%1&#xa0;» n’est pas disponible. Terminaux disponibles&#xa0;: %2
Accédez à %3 → %4 pour le modifier.</translation>
    </message>
    <message>
        <source>This mode needs exactly %1 wordlists (left, right).
Currently selected: %2.</source>
        <translation>Ce mode nécessite exactement %1 listes de mots (gauche, droite).
Actuellement sélectionnées&#xa0;: %2.</translation>
    </message>
    <message>
        <source>The session names the hash type %1, which this hashcat version does not support.</source>
        <translation>La session désigne le type de hash %1, que cette version de hashcat ne prend pas en charge.</translation>
    </message>
    <message>
        <source>Password candidate mask. ?1–?4 insert a custom charset, ?w inserts the wordlist word and ?q a second wordlist word in the Hybrid (mask + ?w/?q) mode, and ?? is a literal ?.</source>
        <translation>Masque des candidats de mot de passe. ?1–?4 insèrent un jeu de caractères personnalisé, ?w insère le mot de la liste de mots et ?q un second mot de liste de mots dans le mode Hybride (masque + ?w/?q), et ?? est un ? littéral.</translation>
    </message>
    <message>
        <source>Load the %1 example? The current session is lost.

The example cracks a test hash; hashcat should recover the password %2.</source>
        <translation>Charger l’exemple %1&#xa0;? La session actuelle sera perdue.

L’exemple craque un hash de test&#xa0;; hashcat devrait retrouver le mot de passe %2.</translation>
    </message>
    <message>
        <source>Could not create the example directory “%1”.</source>
        <translation>Impossible de créer le répertoire d’exemple «&#xa0;%1&#xa0;».</translation>
    </message>
    <message>
        <source>Could not read the bundled file “%1”: %2</source>
        <translation>Impossible de lire le fichier fourni «&#xa0;%1&#xa0;»&#xa0;: %2</translation>
    </message>
    <message>
        <source>Could not write “%1”: %2</source>
        <translation>Impossible d’écrire «&#xa0;%1&#xa0;»&#xa0;: %2</translation>
    </message>
    <message>
        <source>Straight</source>
        <translation>Dictionnaire</translation>
    </message>
    <message>
        <source>Combination</source>
        <translation>Combinaison</translation>
    </message>
    <message>
        <source>Brute-force</source>
        <translation>Force brute</translation>
    </message>
    <message>
        <source>Hybrid Wordlist + Mask</source>
        <translation>Hybride&#xa0;: liste de mots + masque</translation>
    </message>
    <message>
        <source>Hybrid Mask + Wordlist</source>
        <translation>Hybride&#xa0;: masque + liste de mots</translation>
    </message>
    <message>
        <source>Generic (feed)</source>
        <translation>Générique (feed)</translation>
    </message>
    <message>
        <source>Association</source>
        <translation>Association</translation>
    </message>
    <message>
        <source>Hybrid (mask + ?w/?q)</source>
        <translation>Hybride (masque + ?w/?q)</translation>
    </message>
    <message>
        <source>Failed to obtain supported hash types.
Error: %1</source>
        <translation>Impossible d’obtenir les types de hash pris en charge.
Erreur&#xa0;: %1</translation>
    </message>
    <message>
        <source>The hash types could not be loaded. Check the hashcat path in Settings.</source>
        <translation>Les types de hash n’ont pas pu être chargés. Vérifiez le chemin de hashcat dans les paramètres.</translation>
    </message>
    <message>
        <source>

This can be caused by Windows Defender or other antivirus software. See the &lt;a href=&quot;%1&quot;&gt;FAQ entry&lt;/a&gt; for a remedy.</source>
        <translation>

Cela peut être causé par Windows Defender ou un autre logiciel antivirus. Consultez &lt;a href=&quot;%1&quot;&gt;l’entrée de la FAQ&lt;/a&gt; pour y remédier.</translation>
    </message>
    <message>
        <source>Set the hashcat path under %1 → %2 to load the available hash types.</source>
        <translation>Définissez le chemin de hashcat sous %1 → %2 pour charger les types de hash disponibles.</translation>
    </message>
    <message>
        <source>This mode needs exactly 2 wordlists (left, right).</source>
        <translation>Ce mode nécessite exactement 2 listes de mots (gauche, droite).</translation>
    </message>
    <message>
        <source>This mode needs a mask.</source>
        <translation>Ce mode nécessite un masque.</translation>
    </message>
    <message>
        <source>This mode runs hashcat’s bundled default-passwords ruleset; name one or more to use your own.</source>
        <translation>Ce mode exécute le jeu de règles default-passwords fourni avec hashcat&#xa0;; indiquez-en un ou plusieurs pour utiliser les vôtres.</translation>
    </message>
    <message>
        <source>This mode needs one wordlist and at least one table.</source>
        <translation>Ce mode nécessite une liste de mots et au moins une table.</translation>
    </message>
    <message>
        <source>This mode needs a feed. Arguments depend on the feed: random and stdin need none, wordlist needs a path.</source>
        <translation>Ce mode nécessite un feed. Les arguments dépendent du feed&#xa0;: random et stdin n’en demandent aucun, wordlist demande un chemin.</translation>
    </message>
    <message>
        <source>This mode needs at most one wordlist. Without one the words come from the hash file and the settings line can bound the run; with one the two files are paired by line.</source>
        <translation>Ce mode nécessite au plus une liste de mots. Sans liste, les mots proviennent du fichier de hashes et la ligne de paramètres peut borner l’exécution&#xa0;; avec une liste, les deux fichiers sont associés ligne par ligne.</translation>
    </message>
    <message>
        <source>This mode needs a mask with exactly one ?w; a ?q after ?w names a second wordlist.</source>
        <translation>Ce mode nécessite un masque contenant exactement un ?w&#xa0;; un ?q après ?w désigne une seconde liste de mots.</translation>
    </message>
    <message>
        <source>Open cache directory</source>
        <translation>Ouvrir le répertoire du cache</translation>
    </message>
    <message>
        <source>Add wordlist folder</source>
        <translation>Ajouter un dossier de listes de mots</translation>
    </message>
    <message>
        <source>Add rulesets</source>
        <translation>Ajouter des jeux de règles</translation>
    </message>
    <message>
        <source>Add ruleset folder</source>
        <translation>Ajouter un dossier de jeu de règles</translation>
    </message>
    <message>
        <source>Add tables</source>
        <translation>Ajouter des tables</translation>
    </message>
    <message>
        <source>Open feed file</source>
        <translation>Ouvrir un fichier de feed</translation>
    </message>
    <message>
        <source>This folder has no wordlists. hashcat reads the regular files directly inside it, not subfolders.</source>
        <translation>Ce dossier ne contient aucune liste de mots. hashcat lit les fichiers normaux situés directement dedans, pas les sous-dossiers.</translation>
    </message>
    <message>
        <source>This wordlist does not exist:
%1</source>
        <translation>Cette liste de mots n’existe pas&#xa0;:
%1</translation>
    </message>
    <message>
        <source>This ruleset folder has no Grammar/grammar.txt:
%1</source>
        <translation>Ce dossier de jeu de règles ne contient pas de Grammar/grammar.txt&#xa0;:
%1</translation>
    </message>
    <message>
        <source>This ruleset does not exist:
%1</source>
        <translation>Ce jeu de règles n’existe pas&#xa0;:
%1</translation>
    </message>
    <message>
        <source>A table must be a file; hashcat does not read a folder:
%1</source>
        <translation>Une table doit être un fichier&#xa0;; hashcat ne lit pas un dossier&#xa0;:
%1</translation>
    </message>
    <message>
        <source>This table does not exist:
%1</source>
        <translation>Cette table n’existe pas&#xa0;:
%1</translation>
    </message>
    <message>
        <source>Copied to the clipboard</source>
        <translation>Copié dans le presse-papiers</translation>
    </message>
    <message>
        <source>Launch failed</source>
        <translation>Échec du lancement</translation>
    </message>
    <message>
        <source>Please choose a hash file.</source>
        <translation>Veuillez choisir un fichier de hashes.</translation>
    </message>
    <message>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to configure the path to the hashcat executable.</source>
        <translation>Accédez à &lt;b&gt;%1 → %2&lt;/b&gt; pour configurer le chemin de l’exécutable hashcat.</translation>
    </message>
    <message>
        <source>The configured hashcat executable does not exist:
%1
Navigate to %2 → %3 to change it.</source>
        <translation>L’exécutable hashcat configuré n’existe pas&#xa0;:
%1
Accédez à %2 → %3 pour le modifier.</translation>
    </message>
    <message>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to select the terminal used for launching.</source>
        <translation>Accédez à &lt;b&gt;%1 → %2&lt;/b&gt; pour choisir le terminal utilisé pour le lancement.</translation>
    </message>
    <message>
        <source>Maximum number of wordlists for this mode: %1.
Currently selected: %2.</source>
        <translation>Nombre maximal de listes de mots pour ce mode&#xa0;: %1.
Actuellement sélectionnées&#xa0;: %2.</translation>
    </message>
    <message>
        <source>hashcat pairs the two files by line here; this GUI offers only one wordlist for that role.</source>
        <translation>Ici, hashcat associe les deux fichiers ligne par ligne&#xa0;; cette interface ne propose qu’une seule liste de mots pour ce rôle.</translation>
    </message>
    <message>
        <source>This wordlist has to be a file: hashcat reads it as the multiplied wordlist (the ?q side), and a folder cannot be used there:
%1</source>
        <translation>Cette liste de mots doit être un fichier&#xa0;: hashcat la lit comme la liste de mots multipliée (le côté ?q), et un dossier n’y est pas utilisable&#xa0;:
%1</translation>
    </message>
    <message>
        <source>This mode needs at least one table.</source>
        <translation>Ce mode nécessite au moins une table.</translation>
    </message>
    <message>
        <source>This mode needs a feed: a bundled name (wordlist, stdin, random, …) or the path of a feed file.</source>
        <translation>Ce mode nécessite un feed&#xa0;: un nom fourni (wordlist, stdin, random, …) ou le chemin d’un fichier de feed.</translation>
    </message>
    <message>
        <source>This mode pairs the wordlists with the mask; the mask needs a ?w for the word.</source>
        <translation>Ce mode associe les listes de mots au masque&#xa0;; le masque nécessite un ?w pour le mot.</translation>
    </message>
    <message>
        <source>The mask contains more than one ?w.</source>
        <translation>Le masque contient plus d’un ?w.</translation>
    </message>
    <message>
        <source>The mask contains a ?q before the first ?w.</source>
        <translation>Le masque contient un ?q avant le premier ?w.</translation>
    </message>
    <message>
        <source>The mask contains more than one ?q.</source>
        <translation>Le masque contient plus d’un ?q.</translation>
    </message>
    <message>
        <source>The mask names a second word with ?q, but only one wordlist is selected.</source>
        <translation>Le masque désigne un second mot avec ?q, mais une seule liste de mots est sélectionnée.</translation>
    </message>
    <message>
        <source>Two wordlists are selected, but the mask has no ?q for the second one.</source>
        <translation>Deux listes de mots sont sélectionnées, mais le masque n’a pas de ?q pour la seconde.</translation>
    </message>
    <message>
        <source>The increment is not offered in this hybrid mode.</source>
        <translation>L’incrément n’est pas proposé dans ce mode hybride.</translation>
    </message>
    <message>
        <source>The ?w and ?q markers are only offered in the hybrid mode (mask + wordlists).</source>
        <translation>Les marqueurs ?w et ?q ne sont proposés que dans le mode hybride (masque + listes de mots).</translation>
    </message>
    <message>
        <source>The setting “%1” is not valid for this mode.</source>
        <translation>Le paramètre «&#xa0;%1&#xa0;» n’est pas valide pour ce mode.</translation>
    </message>
    <message>
        <source>The token “%1” is not a setting; settings are written as key=value.</source>
        <translation>Le token «&#xa0;%1&#xa0;» n’est pas un paramètre&#xa0;; les paramètres s’écrivent sous la forme key=value.</translation>
    </message>
    <message>
        <source>The settings line applies only while the words come from the hash file. Clear either the settings line or the wordlist list.</source>
        <translation>La ligne de paramètres ne s’applique que tant que les mots proviennent du fichier de hashes. Videz soit la ligne de paramètres, soit la liste de listes de mots.</translation>
    </message>
    <message>
        <source>The hash type 4000 describes its hashes as a recipe, for example
md5(md5(salt) . pass). Enter it in the hash recipe field.</source>
        <translation>Le type de hash 4000 décrit ses hashes sous forme de recette, par exemple
md5(md5(salt) . pass). Saisissez-la dans le champ de recette de hash.</translation>
    </message>
    <message>
        <source>The minimum increment length is larger than the maximum.</source>
        <translation>La longueur minimale de l’incrément est supérieure à la longueur maximale.</translation>
    </message>
    <message>
        <source>The cache path must be an existing directory:
%1</source>
        <translation>Le chemin du cache doit être un répertoire existant&#xa0;:
%1</translation>
    </message>
    <message>
        <source>The increment cannot be combined with the speed-only option.</source>
        <translation>L’incrément ne peut pas être combiné avec l’option de vitesse seule.</translation>
    </message>
    <message>
        <source>Removing cracked hashes is not possible in the association attack mode.</source>
        <translation>Retirer les hashes craqués n’est pas possible dans le mode d’attaque par association.</translation>
    </message>
</context>
<context>
    <name>SettingsDialog</name>
    <message>
        <source>Settings</source>
        <translation>Paramètres</translation>
    </message>
    <message>
        <source>hashcat executable</source>
        <translation>exécutable hashcat</translation>
    </message>
    <message>
        <source>Not configured</source>
        <translation>Non configuré</translation>
    </message>
    <message>
        <source>hashcat</source>
        <translation>hashcat</translation>
    </message>
    <message>
        <source>Select…</source>
        <translation>Sélectionner…</translation>
    </message>
    <message>
        <source>&amp;Terminal</source>
        <translation>&amp;Terminal</translation>
    </message>
    <message>
        <source>&amp;Language</source>
        <translation>&amp;Langue</translation>
    </message>
    <message>
        <source>&amp;Color scheme</source>
        <translation>S&amp;chéma de couleurs</translation>
    </message>
    <message>
        <source>Not all color schemes are available on all platforms.</source>
        <translation>Tous les schémas de couleurs ne sont pas disponibles sur toutes les plateformes.</translation>
    </message>
    <message>
        <source>&amp;Parameters</source>
        <translation>P&amp;aramètres</translation>
    </message>
    <message>
        <source>&amp;Use short parameters</source>
        <translation>&amp;Utiliser les paramètres courts</translation>
    </message>
    <message>
        <source>Session and files</source>
        <translation>Session et fichiers</translation>
    </message>
    <message>
        <source>&amp;Restore the last session at startup</source>
        <translation>&amp;Restaurer la dernière session au démarrage</translation>
    </message>
    <message>
        <source>&amp;Session file</source>
        <translation>Fichier de &amp;session</translation>
    </message>
    <message>
        <source>The file the last session is stored in</source>
        <translation>Le fichier dans lequel la dernière session est enregistrée</translation>
    </message>
    <message>
        <source>Open folder</source>
        <translation>Ouvrir le dossier</translation>
    </message>
    <message>
        <source>Open the folder the session is stored in</source>
        <translation>Ouvrir le dossier dans lequel la session est enregistrée</translation>
    </message>
    <message>
        <source>Settings &amp;file</source>
        <translation>Fichier des &amp;paramètres</translation>
    </message>
    <message>
        <source>The file the settings are stored in</source>
        <translation>Le fichier dans lequel les paramètres sont enregistrés</translation>
    </message>
    <message>
        <source>Open the folder the settings are stored in</source>
        <translation>Ouvrir le dossier dans lequel les paramètres sont enregistrés</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Général</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Enregistrer</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <source>Download from %1</source>
        <translation>Télécharger depuis %1</translation>
    </message>
    <message>
        <source>Use the short option names (-m) instead of the long ones (--hash-type) in the command.</source>
        <translation>Utiliser les noms courts des options (-m) au lieu des noms longs (--hash-type) dans la commande.</translation>
    </message>
    <message>
        <source>Select a terminal</source>
        <translation>Sélectionner un terminal</translation>
    </message>
    <message>
        <source>System default</source>
        <translation>Par défaut du système</translation>
    </message>
    <message>
        <source>Light</source>
        <translation>Clair</translation>
    </message>
    <message>
        <source>Dark</source>
        <translation>Sombre</translation>
    </message>
    <message>
        <source>The hashcat path “%1” was not found on this computer.</source>
        <translation>Le chemin de hashcat «&#xa0;%1&#xa0;» est introuvable sur cet ordinateur.</translation>
    </message>
    <message>
        <source>The terminal “%1” is not installed and was not applied.</source>
        <translation>Le terminal «&#xa0;%1&#xa0;» n’est pas installé et n’a pas été appliqué.</translation>
    </message>
    <message>
        <source>The color scheme “%1” is not offered and was not applied.</source>
        <translation>Le schéma de couleurs «&#xa0;%1&#xa0;» n’est pas proposé et n’a pas été appliqué.</translation>
    </message>
    <message>
        <source>The language “%1” is not offered and was not applied.</source>
        <translation>La langue «&#xa0;%1&#xa0;» n’est pas proposée et n’a pas été appliquée.</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not installed.</source>
        <translation>Le terminal configuré «&#xa0;%1&#xa0;» n’est pas installé.</translation>
    </message>
    <message>
        <source>Executable Files (*.exe *.bin hashcat);;All Files (*)</source>
        <translation>Fichiers exécutables (*.exe *.bin hashcat);;Tous les fichiers (*)</translation>
    </message>
    <message>
        <source>No hashcat executable is configured; hashcat cannot be started.</source>
        <translation>Aucun exécutable hashcat n’est configuré&#xa0;; hashcat ne peut pas être lancé.</translation>
    </message>
    <message>
        <source>The configured hashcat executable was not found on this computer.</source>
        <translation>L’exécutable hashcat configuré est introuvable sur cet ordinateur.</translation>
    </message>
    <message>
        <source>Select the terminal hashcat is started in.</source>
        <translation>Sélectionnez le terminal dans lequel hashcat est lancé.</translation>
    </message>
    <message>
        <source>Select the hashcat executable</source>
        <translation>Sélectionner l’exécutable hashcat</translation>
    </message>
    <message>
        <source>Invalid file</source>
        <translation>Fichier non valide</translation>
    </message>
    <message>
        <source>The selected file is not an executable.</source>
        <translation>Le fichier sélectionné n’est pas un exécutable.</translation>
    </message>
    <message>
        <source>Restart required</source>
        <translation>Redémarrage requis</translation>
    </message>
    <message>
        <source>Restart hashcat-gui to apply the new language.</source>
        <translation>Redémarrez hashcat-gui pour appliquer la nouvelle langue.</translation>
    </message>
</context>
<context>
    <name>StateFile</name>
    <message>
        <source>The file is not valid JSON: %1</source>
        <translation>Le fichier n’est pas un JSON valide&#xa0;: %1</translation>
    </message>
    <message>
        <source>The file does not contain a JSON object.</source>
        <translation>Le fichier ne contient pas d’objet JSON.</translation>
    </message>
    <message>
        <source>The file does not contain any sections.</source>
        <translation>Le fichier ne contient aucune section.</translation>
    </message>
    <message>
        <source>Could not open “%1” for writing: %2</source>
        <translation>Impossible d’ouvrir «&#xa0;%1&#xa0;» en écriture&#xa0;: %2</translation>
    </message>
    <message>
        <source>Could not write to “%1”: %2</source>
        <translation>Impossible d’écrire dans «&#xa0;%1&#xa0;»&#xa0;: %2</translation>
    </message>
    <message>
        <source>Could not open “%1” for reading: %2</source>
        <translation>Impossible d’ouvrir «&#xa0;%1&#xa0;» en lecture&#xa0;: %2</translation>
    </message>
    <message>
        <source>The file does not contain a “%1” section.</source>
        <translation>Le fichier ne contient pas de section «&#xa0;%1&#xa0;».</translation>
    </message>
    <message>
        <source>The file does not contain a “%1” section; it holds %2.</source>
        <translation>Le fichier ne contient pas de section «&#xa0;%1&#xa0;»&#xa0;; il contient %2.</translation>
    </message>
    <message>
        <source>The “%1” section does not contain any values.</source>
        <translation>La section «&#xa0;%1&#xa0;» ne contient aucune valeur.</translation>
    </message>
    <message>
        <source> and </source>
        <translation> et </translation>
    </message>
    <message>
        <source> and %1 more</source>
        <translation> et %1 de plus</translation>
    </message>
    <message>
        <source>The file names values this version does not know, so they were not applied: %1.</source>
        <translation>Le fichier mentionne des valeurs que cette version ne connaît pas, elles n’ont donc pas été appliquées&#xa0;: %1.</translation>
    </message>
    <message>
        <source>The file holds values of the wrong type, so they were not applied: %1.</source>
        <translation>Le fichier contient des valeurs d’un mauvais type, elles n’ont donc pas été appliquées&#xa0;: %1.</translation>
    </message>
</context>
<context>
    <name>UpdateChecker</name>
    <message>
        <source>GitHub answered with HTTP status %1: %2</source>
        <translation>GitHub a répondu avec le statut HTTP %1&#xa0;: %2</translation>
    </message>
    <message>
        <source>GitHub answered with HTTP status %1.</source>
        <translation>GitHub a répondu avec le statut HTTP %1.</translation>
    </message>
    <message>
        <source>The release information is not valid JSON.</source>
        <translation>Les informations de version ne sont pas un JSON valide.</translation>
    </message>
    <message>
        <source>The release information does not name a version.</source>
        <translation>Les informations de version n’indiquent aucune version.</translation>
    </message>
    <message>
        <source>The release version %1 is not a valid version number.</source>
        <translation>La version publiée %1 n’est pas un numéro de version valide.</translation>
    </message>
    <message>
        <source>The installed version %1 is not a valid version number.</source>
        <translation>La version installée %1 n’est pas un numéro de version valide.</translation>
    </message>
</context>
</TS>
