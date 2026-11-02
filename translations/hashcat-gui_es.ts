<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="es" sourcelanguage="en">
<context>
    <name>AboutDialog</name>
    <message>
        <source>About</source>
        <translation>Acerca de hashcat-gui</translation>
    </message>
    <message>
        <source>Graphical user interface for hashcat, the password recovery utility</source>
        <translation>Interfaz gráfica para hashcat, la utilidad de recuperación de contraseñas</translation>
    </message>
    <message>
        <source>Version information</source>
        <translation>Información de versión</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Desconocido</translation>
    </message>
    <message>
        <source>Updates</source>
        <translation>Actualizaciones</translation>
    </message>
    <message>
        <source>Checking for updates…</source>
        <translation>Buscando actualizaciones…</translation>
    </message>
    <message>
        <source>System information</source>
        <translation>Información del sistema</translation>
    </message>
    <message>
        <source>Operating system</source>
        <translation>Sistema operativo</translation>
    </message>
    <message>
        <source>Links</source>
        <translation>Enlaces</translation>
    </message>
    <message>
        <source>License</source>
        <translation>Licencia</translation>
    </message>
    <message>
        <source>Copy the application, toolkit, system and hashcat versions to the clipboard</source>
        <translation>Copiar al portapapeles las versiones de la aplicación, el toolkit, el sistema y hashcat</translation>
    </message>
    <message>
        <source>Copy details</source>
        <translation>Copiar detalles</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Copied to the clipboard</source>
        <translation>Copiado al portapapeles</translation>
    </message>
    <message>
        <source>Project page</source>
        <translation>Página del proyecto</translation>
    </message>
    <message>
        <source>Report an issue</source>
        <translation>Informar de un problema</translation>
    </message>
    <message>
        <source>hashcat</source>
        <translation>hashcat</translation>
    </message>
    <message>
        <source>hashcat FAQ</source>
        <translation>Preguntas frecuentes de hashcat</translation>
    </message>
    <message>
        <source>hashcat-gui is licensed under the %1.</source>
        <translation>hashcat-gui se distribuye bajo la licencia %1.</translation>
    </message>
    <message>
        <source>GNU GPL v3.0 or later</source>
        <translation>GNU GPL v3.0 o posterior</translation>
    </message>
    <message>
        <source>Not configured</source>
        <translation>No configurado</translation>
    </message>
    <message>
        <source>Error: %1</source>
        <translation>Error: %1</translation>
    </message>
    <message>
        <source>A newer version is available: %1.</source>
        <translation>Hay una versión más reciente disponible: %1.</translation>
    </message>
    <message>
        <source>Release page</source>
        <translation>Página de la versión</translation>
    </message>
    <message>
        <source>No updates available.</source>
        <translation>No hay actualizaciones disponibles.</translation>
    </message>
    <message>
        <source>Could not check for updates.</source>
        <translation>No se pudo comprobar si hay actualizaciones.</translation>
    </message>
    <message>
        <source>Could not check for updates: %1</source>
        <translation>No se pudo comprobar si hay actualizaciones: %1</translation>
    </message>
</context>
<context>
    <name>HashcatInfoParser</name>
    <message>
        <source>Invalid JSON returned from hashcat.</source>
        <translation>hashcat devolvió un JSON no válido.</translation>
    </message>
    <message>
        <source>hashcat did not report any hash types.</source>
        <translation>hashcat no informó de ningún tipo de hash.</translation>
    </message>
</context>
<context>
    <name>HelperUtils</name>
    <message>
        <source>The hashcat path is not configured.</source>
        <translation>La ruta de hashcat no está configurada.</translation>
    </message>
    <message>
        <source>Failed to start hashcat
%1</source>
        <translation>No se pudo iniciar hashcat
%1</translation>
    </message>
    <message>
        <source>hashcat timed out
%1</source>
        <translation>hashcat agotó el tiempo de espera
%1</translation>
    </message>
    <message>
        <source>No supported terminal was found on this system. hashcat-gui needs one to show the hashcat output.</source>
        <translation>No se encontró ningún terminal compatible en este sistema. hashcat-gui necesita uno para mostrar la salida de hashcat.</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <source>Hash file or hash:</source>
        <translation>Archivo de hashes o hash:</translation>
    </message>
    <message>
        <source>The hash, a file of hashes, or a .hc22000/.hccapx WPA capture.</source>
        <translation>El hash, un archivo de hashes o una captura WPA .hc22000/.hccapx.</translation>
    </message>
    <message>
        <source>Choose a file of hashes.</source>
        <translation>Elegir un archivo de hashes.</translation>
    </message>
    <message>
        <source>Salt is hex</source>
        <translation>El salt es hex</translation>
    </message>
    <message>
        <source>Rewrite the hash file without cracked hashes, including potfile-known ones; the potfile is untouched. Refused in attack mode 9.</source>
        <translation>Reescribir el archivo de hashes sin los hashes descifrados, incluidos los ya conocidos por el potfile; el potfile no se modifica. Se rechaza en el modo de ataque 9.</translation>
    </message>
    <message>
        <source>Remove hash from hash list once it is cracked</source>
        <translation>Quitar el hash de la lista una vez descifrado</translation>
    </message>
    <message>
        <source>Ignore the username before the mode’s separator (usually “:”), so accounts sharing a hash are cracked once.</source>
        <translation>Ignorar el nombre de usuario antes del separador del modo (normalmente “:”), de modo que las cuentas que comparten un hash se descifren una sola vez.</translation>
    </message>
    <message>
        <source>Hash recipe, e.g. “md5(md5(salt) . pass)”. Wrap it in the shell’s outer quotes and put any inner strings in the other kind of quotes; PowerShell needs a backtick before $.</source>
        <translation>Receta de hash, p. ej. “md5(md5(salt) . pass)”. Encierre la receta entre las comillas exteriores de la shell y las cadenas interiores entre las del otro tipo; PowerShell necesita un acento grave antes de $.</translation>
    </message>
    <message>
        <source>Move item up</source>
        <translation>Subir elemento</translation>
    </message>
    <message>
        <source>Move selected wordlist up</source>
        <translation>Subir la lista de palabras seleccionada</translation>
    </message>
    <message>
        <source>Move item down</source>
        <translation>Bajar elemento</translation>
    </message>
    <message>
        <source>Move selected wordlist down</source>
        <translation>Bajar la lista de palabras seleccionada</translation>
    </message>
    <message>
        <source>Add one or more wordlist files. Each checked row is passed to hashcat.</source>
        <translation>Añadir uno o más archivos de listas de palabras. Cada fila marcada se pasa a hashcat.</translation>
    </message>
    <message>
        <source>Remove the selected wordlists.</source>
        <translation>Quitar las listas de palabras seleccionadas.</translation>
    </message>
    <message>
        <source>Remove wordlists</source>
        <translation>Quitar listas de palabras</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Quitar</translation>
    </message>
    <message>
        <source>Wordlist is hex</source>
        <translation>La lista de palabras es hex</translation>
    </message>
    <message>
        <source>Encoding from:</source>
        <translation>Codificación de origen:</translation>
    </message>
    <message>
        <source>Encoding to:</source>
        <translation>Codificación de destino:</translation>
    </message>
    <message>
        <source>Space-separated key=value settings for attack mode 9, e.g. phases=words,rules or rules.rulefile=…. Only used without a wordlist.</source>
        <translation>Ajustes key=value separados por espacios para el modo de ataque 9, p. ej. phases=words,rules o rules.rulefile=…. Solo se usan sin lista de palabras.</translation>
    </message>
    <message>
        <source>A ruleset file, a folder with Grammar/grammar.txt, or a bundled name. Unchecked rows are skipped; none runs default-passwords.</source>
        <translation>Un archivo de conjunto de reglas, una carpeta con Grammar/grammar.txt o un nombre incluido. Las filas sin marcar se omiten; si no hay ninguna marcada, se ejecuta default-passwords.</translation>
    </message>
    <message>
        <source>Add one or more PCFG ruleset files.</source>
        <translation>Añadir uno o más archivos de conjuntos de reglas PCFG.</translation>
    </message>
    <message>
        <source>Add PCFG rulesets</source>
        <translation>Añadir conjuntos de reglas PCFG</translation>
    </message>
    <message>
        <source>Remove the selected rulesets.</source>
        <translation>Quitar los conjuntos de reglas seleccionados.</translation>
    </message>
    <message>
        <source>Remove rulesets</source>
        <translation>Quitar conjuntos de reglas</translation>
    </message>
    <message>
        <source>Move selected ruleset up</source>
        <translation>Subir el conjunto de reglas seleccionado</translation>
    </message>
    <message>
        <source>Move selected ruleset down</source>
        <translation>Bajar el conjunto de reglas seleccionado</translation>
    </message>
    <message>
        <source>One plain number per ruleset, colon-separated, e.g. 2:1; the count must match the number of rulesets.</source>
        <translation>Un número simple por conjunto de reglas, separados por dos puntos, p. ej. 2:1; la cantidad debe coincidir con el número de conjuntos de reglas.</translation>
    </message>
    <message>
        <source>Maximum candidate length the kernel builds, in 4-byte words: 0 (auto), 16 or 32.</source>
        <translation>Longitud máxima de candidato que construye el kernel, en palabras de 4 bytes: 0 (auto), 16 o 32.</translation>
    </message>
    <message>
        <source>1 steps to the next base word where the order allows instead of unranking it; the candidates are identical, only the fast path is off.</source>
        <translation>1 pasa a la siguiente palabra base cuando el orden lo permite, en lugar de calcularla por unranking; los candidatos son idénticos, solo se desactiva la vía rápida.</translation>
    </message>
    <message>
        <source>Only enumerate candidates this mask admits; excludes omen=1, and hintaccount unless hint words come from hintwords or hintfile.</source>
        <translation>Enumerar solo los candidatos que admite esta máscara; excluye omen=1 y hintaccount, salvo que las palabras de pista procedan de hintwords o hintfile.</translation>
    </message>
    <message>
        <source>Space-separated key=value settings: omen, cache, lookup, pwmin, pwmax, hintaccount, hintwords, hintfile, hintrank, hintrepeat. Rulesets go first.</source>
        <translation>Ajustes key=value separados por espacios: omen, cache, lookup, pwmin, pwmax, hintaccount, hintwords, hintfile, hintrank, hintrepeat. Los conjuntos de reglas van primero.</translation>
    </message>
    <message>
        <source>Add one or more table files.</source>
        <translation>Añadir uno o más archivos de tablas.</translation>
    </message>
    <message>
        <source>Remove the selected tables.</source>
        <translation>Quitar las tablas seleccionadas.</translation>
    </message>
    <message>
        <source>Remove tables</source>
        <translation>Quitar tablas</translation>
    </message>
    <message>
        <source>1 (default) keeps a token unchanged as a choice, 0 drops it so the table only yields variations. identity=0 excludes single=1.</source>
        <translation>1 (predeterminado) mantiene un token sin cambios como opción; 0 lo descarta, de modo que la tabla solo produce variaciones. identity=0 excluye single=1.</translation>
    </message>
    <message>
        <source>1 reads the wordlist as patterns: no word is returned unchanged and unmatched words are dropped. 0 (default) keeps them.</source>
        <translation>1 lee la lista de palabras como patrones: no se devuelve ninguna palabra sin cambios y se descartan las palabras que no coinciden. 0 (predeterminado) las conserva.</translation>
    </message>
    <message>
        <source>single=1: one substitution per candidate instead of the cross product; fewer candidates, more host work on fast hashes.</source>
        <translation>single=1: una sustitución por candidato en lugar del producto cartesiano; menos candidatos y más trabajo del host con hashes rápidos.</translation>
    </message>
    <message>
        <source>Keep only the first N replacements of each source word; 0 means no limit. Applies to single=1 too.</source>
        <translation>Conservar solo las primeras N sustituciones de cada palabra de origen; 0 significa sin límite. También se aplica a single=1.</translation>
    </message>
    <message>
        <source>How much of a word’s cross product the host enumerates before giving up; 0 means no limit.</source>
        <translation>Cuánto del producto cartesiano de una palabra enumera el host antes de rendirse; 0 significa sin límite.</translation>
    </message>
    <message>
        <source>Key=value settings separated by spaces; everything after the first one is a setting, so tables come first.</source>
        <translation>Ajustes key=value separados por espacios; todo lo que va después del primero es un ajuste, así que las tablas van primero.</translation>
    </message>
    <message>
        <source>Feed name or path. A slash means a path; otherwise hashcat tries feed_&lt;name&gt;, rust_&lt;name&gt;, then &lt;name&gt; in its feeds directory.</source>
        <translation>Nombre o ruta del feed. Una barra indica una ruta; de lo contrario, hashcat prueba feed_&lt;name&gt;, rust_&lt;name&gt; y luego &lt;name&gt; en su directorio de feeds.</translation>
    </message>
    <message>
        <source>Browse for a feed plugin instead of naming it.</source>
        <translation>Examinar en busca de un plugin de feed en lugar de indicar su nombre.</translation>
    </message>
    <message>
        <source>Browse for a feed plugin</source>
        <translation>Examinar en busca de un plugin de feed</translation>
    </message>
    <message>
        <source>Argument passed to the feed; meaning and count depend on the feed (the wordlist feed takes wordlist paths).</source>
        <translation>Argumento que se pasa al feed; su significado y cantidad dependen del feed (el feed wordlist admite rutas de listas de palabras).</translation>
    </message>
    <message>
        <source>Add an argument for the feed, in order.</source>
        <translation>Añadir un argumento para el feed, en orden.</translation>
    </message>
    <message>
        <source>Remove the selected feed arguments.</source>
        <translation>Quitar los argumentos de feed seleccionados.</translation>
    </message>
    <message>
        <source>Remove feed arguments</source>
        <translation>Quitar argumentos de feed</translation>
    </message>
    <message>
        <source>Move selected feed argument up</source>
        <translation>Subir el argumento de feed seleccionado</translation>
    </message>
    <message>
        <source>Move selected feed argument down</source>
        <translation>Bajar el argumento de feed seleccionado</translation>
    </message>
    <message>
        <source>Read rules from the files below instead of generating them.</source>
        <translation>Leer las reglas de los archivos siguientes en lugar de generarlas.</translation>
    </message>
    <message>
        <source>Rules file 1:</source>
        <translation>Archivo de reglas 1:</translation>
    </message>
    <message>
        <source>Choose the rules file for slot 1.</source>
        <translation>Elegir el archivo de reglas para la ranura 1.</translation>
    </message>
    <message>
        <source>Open rules file 1</source>
        <translation>Abrir archivo de reglas 1</translation>
    </message>
    <message>
        <source>Rules file 2:</source>
        <translation>Archivo de reglas 2:</translation>
    </message>
    <message>
        <source>Choose the rules file for slot 2.</source>
        <translation>Elegir el archivo de reglas para la ranura 2.</translation>
    </message>
    <message>
        <source>Open rules file 2</source>
        <translation>Abrir archivo de reglas 2</translation>
    </message>
    <message>
        <source>Rules file 3:</source>
        <translation>Archivo de reglas 3:</translation>
    </message>
    <message>
        <source>Choose the rules file for slot 3.</source>
        <translation>Elegir el archivo de reglas para la ranura 3.</translation>
    </message>
    <message>
        <source>Open rules file 3</source>
        <translation>Abrir archivo de reglas 3</translation>
    </message>
    <message>
        <source>Number of random rules hashcat generates (-g). Only used with Generate rules; no rules file is read.</source>
        <translation>Número de reglas aleatorias que genera hashcat (-g). Solo se usa con “Generar reglas”; no se lee ningún archivo de reglas.</translation>
    </message>
    <message>
        <source>Treat every -r file as one list: rule counts add instead of multiplying.</source>
        <translation>Tratar cada archivo -r como una sola lista: los recuentos de reglas se suman en lugar de multiplicarse.</translation>
    </message>
    <message>
        <source>Wordlist rules</source>
        <translation>Reglas de la lista de palabras</translation>
    </message>
    <message>
        <source>Left rule:</source>
        <translation>Regla izquierda:</translation>
    </message>
    <message>
        <source>Right rule:</source>
        <translation>Regla derecha:</translation>
    </message>
    <message>
        <source>Interpret the mask and custom charsets as hexadecimal bytes.</source>
        <translation>Interpretar la máscara y los juegos de caracteres personalizados como bytes hexadecimales.</translation>
    </message>
    <message>
        <source>Charset is hex</source>
        <translation>El juego de caracteres es hex</translation>
    </message>
    <message>
        <source>Ctrl+Shift+I</source>
        <translation>Ctrl+Shift+I</translation>
    </message>
    <message>
        <source>Increment mask length</source>
        <translation>Incrementar la longitud de la máscara</translation>
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
        <source>Use hashcat’s optimized kernels (-O); they cap the maximum password length.</source>
        <translation>Usar los kernels optimizados de hashcat (-O); limitan la longitud máxima de la contraseña.</translation>
    </message>
    <message>
        <source>Pin hashcat to these CPU devices, separated by commas. Leave empty to let hashcat decide.</source>
        <translation>Fijar hashcat a estos dispositivos de CPU, separados por comas. Déjelo vacío para que hashcat decida.</translation>
    </message>
    <message>
        <source>Devices to use, separated by commas. Leave empty for all devices.</source>
        <translation>Dispositivos que se usarán, separados por comas. Déjelo vacío para usar todos los dispositivos.</translation>
    </message>
    <message>
        <source>Abort above</source>
        <translation>Abortar por encima de</translation>
    </message>
    <message>
        <source>Outfile</source>
        <translation>Archivo de salida</translation>
    </message>
    <message>
        <source>Save…</source>
        <translation>Guardar…</translation>
    </message>
    <message>
        <source>Write the outfile as JSON. The outfile format is ignored.</source>
        <translation>Escribir el archivo de salida como JSON. El formato del archivo de salida se ignora.</translation>
    </message>
    <message>
        <source>Write JSON</source>
        <translation>Escribir JSON</translation>
    </message>
    <message>
        <source>Do not write recovered hashes to the potfile.</source>
        <translation>No escribir los hashes recuperados en el potfile.</translation>
    </message>
    <message>
        <source>Load Example Session</source>
        <translation>Cargar sesión de ejemplo</translation>
    </message>
    <message>
        <source>Reset Session</source>
        <translation>Restablecer sesión</translation>
    </message>
    <message>
        <source>Mode:</source>
        <translation>Modo:</translation>
    </message>
    <message>
        <source>Attack</source>
        <translation>Ataque</translation>
    </message>
    <message>
        <source>Open…</source>
        <translation>Abrir…</translation>
    </message>
    <message>
        <source>Assume the salt is given in hexadecimal.</source>
        <translation>Asumir que el salt está dado en hexadecimal.</translation>
    </message>
    <message>
        <source>Attack mode. Each choice reveals its own options below.</source>
        <translation>Modo de ataque. Cada opción muestra sus propias opciones debajo.</translation>
    </message>
    <message>
        <source>Hash type:</source>
        <translation>Tipo de hash:</translation>
    </message>
    <message>
        <source>Hash recipe:</source>
        <translation>Receta de hash:</translation>
    </message>
    <message>
        <source>Add files…</source>
        <translation>Añadir archivos…</translation>
    </message>
    <message>
        <source>hashcat reads every file directly inside the folder as one wordlist.</source>
        <translation>hashcat lee cada archivo situado directamente dentro de la carpeta como una lista de palabras.</translation>
    </message>
    <message>
        <source>Add folder…</source>
        <translation>Añadir carpeta…</translation>
    </message>
    <message>
        <source>Only checked rows are used. A folder is one wordlist: hashcat reads the files directly inside it.</source>
        <translation>Solo se usan las filas marcadas. Una carpeta es una lista de palabras: hashcat lee los archivos situados directamente dentro de ella.</translation>
    </message>
    <message>
        <source>Assume words in the wordlist are given in hexadecimal.</source>
        <translation>Asumir que las palabras de la lista de palabras están dadas en hexadecimal.</translation>
    </message>
    <message>
        <source>Settings (no wordlist):</source>
        <translation>Ajustes (sin lista de palabras):</translation>
    </message>
    <message>
        <source>PCFG</source>
        <translation>PCFG</translation>
    </message>
    <message>
        <source>A ruleset folder needs a Grammar/grammar.txt.</source>
        <translation>Una carpeta de conjunto de reglas necesita un Grammar/grammar.txt.</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Scale:</translation>
    </message>
    <message>
        <source>Quantization steps per bit (1–64)</source>
        <translation>Pasos de cuantización por bit (1–64)</translation>
    </message>
    <message>
        <source>Costmax:</source>
        <translation>Costmax:</translation>
    </message>
    <message>
        <source>Highest cost level to enumerate (1–64)</source>
        <translation>Nivel de coste más alto que enumerar (1–64)</translation>
    </message>
    <message>
        <source>Weights:</source>
        <translation>Weights:</translation>
    </message>
    <message>
        <source>Threads:</source>
        <translation>Hilos:</translation>
    </message>
    <message>
        <source>Cores that generate base words (0–16); 0 runs on the calling thread.</source>
        <translation>Núcleos que generan palabras base (0–16); 0 se ejecuta en el hilo que llama.</translation>
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
        <translation>Bits que puede abarcar un bucle interno (0–27)</translation>
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
        <translation>1.0–64.0; el separador decimal es un punto, no el separador de la configuración regional.</translation>
    </message>
    <message>
        <source>Walk:</source>
        <translation>Walk:</translation>
    </message>
    <message>
        <source>Mask:</source>
        <translation>Máscara:</translation>
    </message>
    <message>
        <source>Extra settings (key=value):</source>
        <translation>Ajustes adicionales (key=value):</translation>
    </message>
    <message>
        <source>Table</source>
        <translation>Tabla</translation>
    </message>
    <message>
        <source>Every checked row after the wordlist is a table.</source>
        <translation>Cada fila marcada después de la lista de palabras es una tabla.</translation>
    </message>
    <message>
        <source>Add tables…</source>
        <translation>Añadir tablas…</translation>
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
        <translation>Feed genérico</translation>
    </message>
    <message>
        <source>Feed:</source>
        <translation>Feed:</translation>
    </message>
    <message>
        <source>wordlist, stdin, random, association, pcfg, table, hybrid, mask, or a feed path</source>
        <translation>wordlist, stdin, random, association, pcfg, table, hybrid, mask o una ruta de feed</translation>
    </message>
    <message>
        <source>Browse…</source>
        <translation>Examinar…</translation>
    </message>
    <message>
        <source>Argument:</source>
        <translation>Argumento:</translation>
    </message>
    <message>
        <source>Add</source>
        <translation>Añadir</translation>
    </message>
    <message>
        <source>Arguments the feed reads, in order; only checked rows are used.</source>
        <translation>Argumentos que lee el feed, en orden; solo se usan las filas marcadas.</translation>
    </message>
    <message>
        <source>Rules</source>
        <translation>Reglas</translation>
    </message>
    <message>
        <source>Use rules files:</source>
        <translation>Usar archivos de reglas:</translation>
    </message>
    <message>
        <source>Enable rules file slot 1 for this run.</source>
        <translation>Activar la ranura 1 de archivo de reglas para esta ejecución.</translation>
    </message>
    <message>
        <source>Rules file for slot 1; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>Archivo de reglas para la ranura 1; se aplica a cada palabra. Déjelo vacío para no usar la ranura.</translation>
    </message>
    <message>
        <source>Enable rules file slot 2 for this run.</source>
        <translation>Activar la ranura 2 de archivo de reglas para esta ejecución.</translation>
    </message>
    <message>
        <source>Rules file for slot 2; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>Archivo de reglas para la ranura 2; se aplica a cada palabra. Déjelo vacío para no usar la ranura.</translation>
    </message>
    <message>
        <source>Enable rules file slot 3 for this run.</source>
        <translation>Activar la ranura 3 de archivo de reglas para esta ejecución.</translation>
    </message>
    <message>
        <source>Rules file for slot 3; applied to each word. Leave empty to leave the slot unused.</source>
        <translation>Archivo de reglas para la ranura 3; se aplica a cada palabra. Déjelo vacío para no usar la ranura.</translation>
    </message>
    <message>
        <source>Generate the rules automatically instead of reading them from a file.</source>
        <translation>Generar las reglas automáticamente en lugar de leerlas de un archivo.</translation>
    </message>
    <message>
        <source>Generate rules:</source>
        <translation>Generar reglas:</translation>
    </message>
    <message>
        <source>Join -r files into one list</source>
        <translation>Unir los archivos -r en una sola lista</translation>
    </message>
    <message>
        <source>Single rule applied to each word from the left wordlist.</source>
        <translation>Regla única que se aplica a cada palabra de la lista de palabras izquierda.</translation>
    </message>
    <message>
        <source>Single rule applied to each word from the right wordlist.</source>
        <translation>Regla única que se aplica a cada palabra de la lista de palabras derecha.</translation>
    </message>
    <message>
        <source>Mask</source>
        <translation>Máscara</translation>
    </message>
    <message>
        <source>Built-in charsets:</source>
        <translation>Juegos de caracteres integrados:</translation>
    </message>
    <message>
        <source>Custom charsets</source>
        <translation>Juegos de caracteres personalizados</translation>
    </message>
    <message>
        <source>Charset 1:</source>
        <translation>Juego de caracteres 1:</translation>
    </message>
    <message>
        <source>Charset 2:</source>
        <translation>Juego de caracteres 2:</translation>
    </message>
    <message>
        <source>Charset 3:</source>
        <translation>Juego de caracteres 3:</translation>
    </message>
    <message>
        <source>Charset 4:</source>
        <translation>Juego de caracteres 4:</translation>
    </message>
    <message>
        <source>Session and limits</source>
        <translation>Sesión y límites</translation>
    </message>
    <message>
        <source>Session name:</source>
        <translation>Nombre de la sesión:</translation>
    </message>
    <message>
        <source>Name hashcat uses for its restore file and status output.</source>
        <translation>Nombre que hashcat usa para su archivo de restauración y la salida de estado.</translation>
    </message>
    <message>
        <source>Runtime limit:</source>
        <translation>Límite de ejecución:</translation>
    </message>
    <message>
        <source>unlimited</source>
        <translation>ilimitado</translation>
    </message>
    <message>
        <source>Skip:</source>
        <translation>Omitir:</translation>
    </message>
    <message>
        <source>Limit:</source>
        <translation>Límite:</translation>
    </message>
    <message>
        <source>Keep updating the run status (speed, progress, ETA) instead of printing one final line.</source>
        <translation>Seguir actualizando el estado de la ejecución (velocidad, progreso, tiempo restante) en lugar de imprimir una sola línea final.</translation>
    </message>
    <message>
        <source>Update status automatically</source>
        <translation>Actualizar el estado automáticamente</translation>
    </message>
    <message>
        <source>Seconds between status updates; only used when automatic status is enabled.</source>
        <translation>Segundos entre actualizaciones de estado; solo se usa cuando el estado automático está activado.</translation>
    </message>
    <message>
        <source>Ignore warnings and start hashcat anyway. Use with care.</source>
        <translation>Ignorar las advertencias e iniciar hashcat de todos modos. Úselo con cuidado.</translation>
    </message>
    <message>
        <source>Force (ignore warnings)</source>
        <translation>Forzar (ignorar advertencias)</translation>
    </message>
    <message>
        <source>Performance</source>
        <translation>Rendimiento</translation>
    </message>
    <message>
        <source>Cache path:</source>
        <translation>Ruta de la caché:</translation>
    </message>
    <message>
        <source>Directory hashcat keeps its cache in. Must exist.</source>
        <translation>Directorio en el que hashcat guarda su caché. Debe existir.</translation>
    </message>
    <message>
        <source>Choose the cache directory.</source>
        <translation>Elegir el directorio de la caché.</translation>
    </message>
    <message>
        <source>Choose cache directory</source>
        <translation>Elegir directorio de la caché</translation>
    </message>
    <message>
        <source>Resources</source>
        <translation>Recursos</translation>
    </message>
    <message>
        <source>CPU affinity:</source>
        <translation>Afinidad de CPU:</translation>
    </message>
    <message>
        <source>Backend devices:</source>
        <translation>Dispositivos backend:</translation>
    </message>
    <message>
        <source>Device types:</source>
        <translation>Tipos de dispositivo:</translation>
    </message>
    <message>
        <source>Abort when any backend device exceeds this temperature in °C; 0 disables it.</source>
        <translation>Abortar cuando cualquier dispositivo backend supere esta temperatura en °C; 0 lo desactiva.</translation>
    </message>
    <message>
        <source>Write recovered hashes to the file below, in the selected formats.</source>
        <translation>Escribir los hashes recuperados en el archivo indicado abajo, en los formatos seleccionados.</translation>
    </message>
    <message>
        <source>Write recovered hashes to file:</source>
        <translation>Escribir los hashes recuperados en el archivo:</translation>
    </message>
    <message>
        <source>The file the recovered hashes are written to. Format variables:

&lt;hash&gt; = current hash filename or hash
&lt;unixtime&gt; = current Unix timestamp</source>
        <translation>El archivo en el que se escriben los hashes recuperados. Variables de formato:

&lt;hash&gt; = nombre del archivo de hashes actual o hash
&lt;unixtime&gt; = marca de tiempo Unix actual</translation>
    </message>
    <message>
        <source>Choose the outfile path.</source>
        <translation>Elegir la ruta del archivo de salida.</translation>
    </message>
    <message>
        <source>Choose outfile</source>
        <translation>Elegir archivo de salida</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Formato:</translation>
    </message>
    <message>
        <source>Do not write the potfile</source>
        <translation>No escribir el potfile</translation>
    </message>
    <message>
        <source>Run hashcat in a terminal with these options.</source>
        <translation>Ejecutar hashcat en un terminal con estas opciones.</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>Iniciar</translation>
    </message>
    <message>
        <source>Copy command to clipboard</source>
        <translation>Copiar comando al portapapeles</translation>
    </message>
    <message>
        <source>General</source>
        <translation>General</translation>
    </message>
    <message>
        <source>Ignore username in hash file</source>
        <translation>Ignorar el nombre de usuario en el archivo de hashes</translation>
    </message>
    <message>
        <source>Wordlists</source>
        <translation>Listas de palabras</translation>
    </message>
    <message>
        <source>Increment the mask length from the minimum to the maximum.</source>
        <translation>Incrementar la longitud de la máscara del mínimo al máximo.</translation>
    </message>
    <message>
        <source>Minimum mask length; 0 uses hashcat’s default.</source>
        <translation>Longitud mínima de la máscara; 0 usa el valor predeterminado de hashcat.</translation>
    </message>
    <message>
        <source>Maximum mask length; 0 uses hashcat’s default.</source>
        <translation>Longitud máxima de la máscara; 0 usa el valor predeterminado de hashcat.</translation>
    </message>
    <message>
        <source>Abort the session after this many seconds; 0 runs until hashcat finishes.</source>
        <translation>Abortar la sesión tras este número de segundos; 0 se ejecuta hasta que hashcat termine.</translation>
    </message>
    <message>
        <source>&#xa0;s</source>
        <translation>&#xa0;s</translation>
    </message>
    <message>
        <source>Skip this many candidates from the start.</source>
        <translation>Omitir este número de candidatos desde el principio.</translation>
    </message>
    <message>
        <source>Only test this many candidates after the skipped ones.</source>
        <translation>Probar solo este número de candidatos después de los omitidos.</translation>
    </message>
    <message>
        <source>Enable optimized kernels (limits password length)</source>
        <translation>Activar los kernels optimizados (limita la longitud de la contraseña)</translation>
    </message>
    <message>
        <source>Print only the attack’s speed, then quit; no cracking.</source>
        <translation>Mostrar solo la velocidad del ataque y salir; sin descifrar.</translation>
    </message>
    <message>
        <source>Return expected speed of the attack, then quit</source>
        <translation>Mostrar la velocidad esperada del ataque y salir</translation>
    </message>
    <message>
        <source>&#xa0;°C</source>
        <translation>&#xa0;°C</translation>
    </message>
    <message>
        <source>Copy command</source>
        <translation>Copiar comando</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>Ay&amp;uda</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Herramientas</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Archivo</translation>
    </message>
    <message>
        <source>Ctrl+Q</source>
        <translation>Ctrl+Q</translation>
    </message>
    <message>
        <source>Settings…</source>
        <translation>Ajustes…</translation>
    </message>
    <message>
        <source>Ctrl+,</source>
        <translation>Ctrl+,</translation>
    </message>
    <message>
        <source>Import Settings…</source>
        <translation>Importar ajustes…</translation>
    </message>
    <message>
        <source>Ctrl+Shift+O</source>
        <translation>Ctrl+Shift+O</translation>
    </message>
    <message>
        <source>Export Settings…</source>
        <translation>Exportar ajustes…</translation>
    </message>
    <message>
        <source>Ctrl+Shift+S</source>
        <translation>Ctrl+Shift+S</translation>
    </message>
    <message>
        <source>Export Session…</source>
        <translation>Exportar sesión…</translation>
    </message>
    <message>
        <source>Ctrl+Shift+E</source>
        <translation>Ctrl+Shift+E</translation>
    </message>
    <message>
        <source>Import Session…</source>
        <translation>Importar sesión…</translation>
    </message>
    <message>
        <source>About</source>
        <translation>Acerca de hashcat-gui</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation>Salir</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>Acerca de Qt</translation>
    </message>
    <message>
        <source>JSON Files (*.json)</source>
        <translation>Archivos JSON (*.json)</translation>
    </message>
    <message>
        <source>The session file could not be read, so the session was not restored. The file was left in place.

%1</source>
        <translation>No se pudo leer el archivo de sesión, por lo que la sesión no se restauró. El archivo se dejó tal como estaba.

%1</translation>
    </message>
    <message>
        <source>Save failed</source>
        <translation>Error al guardar</translation>
    </message>
    <message>
        <source>Outfile format to use, separated by commas.
Default: 1,2</source>
        <translation>Formato del archivo de salida, separado por comas.
Predeterminado: 1,2</translation>
    </message>
    <message>
        <source>Common encodings:</source>
        <translation>Codificaciones comunes:</translation>
    </message>
    <message>
        <source>Force the internal wordlist encoding from this code. Leave empty to let hashcat decide.</source>
        <translation>Forzar la codificación interna de la lista de palabras desde este código. Déjelo vacío para que hashcat decida.</translation>
    </message>
    <message>
        <source>Force the internal wordlist encoding to this code. Leave empty to let hashcat decide.</source>
        <translation>Forzar la codificación interna de la lista de palabras a este código. Déjelo vacío para que hashcat decida.</translation>
    </message>
    <message>
        <source>Exported</source>
        <translation>Exportado</translation>
    </message>
    <message>
        <source>Export failed</source>
        <translation>Error al exportar</translation>
    </message>
    <message>
        <source>Import failed</source>
        <translation>Error al importar</translation>
    </message>
    <message>
        <source>Imported</source>
        <translation>Importado</translation>
    </message>
    <message>
        <source>The file was made by an older version of hashcat-gui (format version %1; this version reads format version %2).</source>
        <translation>El archivo fue creado por una versión anterior de hashcat-gui (versión de formato %1; esta versión lee la versión de formato %2).</translation>
    </message>
    <message>
        <source>The file holds sections this import does not use: %1.</source>
        <translation>El archivo contiene secciones que esta importación no usa: %1.</translation>
    </message>
    <message>
        <source>The file is in format version %1, but this version reads format version %2, so it was not applied. Update hashcat-gui to read it.</source>
        <translation>El archivo tiene la versión de formato %1, pero esta versión lee la versión de formato %2, por lo que no se aplicó. Actualice hashcat-gui para leerlo.</translation>
    </message>
    <message>
        <source>The session file is in format version %1, but this version reads format version %2, so the session was not restored. The file was left as it is, so the session of this run is not stored either. Update hashcat-gui to read it.</source>
        <translation>El archivo de sesión tiene la versión de formato %1, pero esta versión lee la versión de formato %2, por lo que la sesión no se restauró. El archivo se dejó tal como estaba, así que tampoco se guarda la sesión de esta ejecución. Actualice hashcat-gui para leerlo.</translation>
    </message>
    <message>
        <source>Settings exported to %1.</source>
        <translation>Ajustes exportados a %1.</translation>
    </message>
    <message>
        <source>OpenCL device types to use, separated by commas.</source>
        <translation>Tipos de dispositivo OpenCL que se usarán, separados por comas.</translation>
    </message>
    <message>
        <source>Export session</source>
        <translation>Exportar sesión</translation>
    </message>
    <message>
        <source>Session exported to “%1”.</source>
        <translation>Sesión exportada a “%1”.</translation>
    </message>
    <message>
        <source>Import session</source>
        <translation>Importar sesión</translation>
    </message>
    <message>
        <source>Session imported from “%1”.</source>
        <translation>Sesión importada desde “%1”.</translation>
    </message>
    <message>
        <source>Session imported from “%1”.

%2</source>
        <translation>Sesión importada desde “%1”.

%2</translation>
    </message>
    <message>
        <source>Export settings</source>
        <translation>Exportar ajustes</translation>
    </message>
    <message>
        <source>Import settings</source>
        <translation>Importar ajustes</translation>
    </message>
    <message>
        <source>Settings loaded from “%1”.</source>
        <translation>Ajustes cargados desde “%1”.</translation>
    </message>
    <message>
        <source>Review them and save to apply them.</source>
        <translation>Revíselos y guarde para aplicarlos.</translation>
    </message>
    <message>
        <source>Load example session</source>
        <translation>Cargar sesión de ejemplo</translation>
    </message>
    <message>
        <source>Reset session</source>
        <translation>Restablecer sesión</translation>
    </message>
    <message>
        <source>Reset every field to its default? The current session is lost.</source>
        <translation>¿Restablecer todos los campos a sus valores predeterminados? La sesión actual se perderá.</translation>
    </message>
    <message>
        <source>Session warning</source>
        <translation>Aviso de sesión</translation>
    </message>
    <message>
        <source>Updating…</source>
        <translation>Actualizando…</translation>
    </message>
    <message>
        <source>Failed to load hash types</source>
        <translation>Error al cargar los tipos de hash</translation>
    </message>
    <message>
        <source>This mode needs a wordlist and a mask.</source>
        <translation>Este modo necesita una lista de palabras y una máscara.</translation>
    </message>
    <message>
        <source>This mode needs at least one wordlist.</source>
        <translation>Este modo necesita al menos una lista de palabras.</translation>
    </message>
    <message>
        <source>Open hash file</source>
        <translation>Abrir archivo de hashes</translation>
    </message>
    <message>
        <source>Save output file</source>
        <translation>Guardar archivo de salida</translation>
    </message>
    <message>
        <source>Add wordlists</source>
        <translation>Añadir listas de palabras</translation>
    </message>
    <message>
        <source>Open rules file</source>
        <translation>Abrir archivo de reglas</translation>
    </message>
    <message>
        <source>Could not start “%1”: %2</source>
        <translation>No se pudo iniciar “%1”: %2</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not available. Available terminals: %2
Navigate to %3 → %4 to change it.</source>
        <translation>El terminal configurado “%1” no está disponible. Terminales disponibles: %2
Vaya a %3 → %4 para cambiarlo.</translation>
    </message>
    <message>
        <source>This mode needs exactly %1 wordlists (left, right).
Currently selected: %2.</source>
        <translation>Este modo necesita exactamente %1 listas de palabras (izquierda, derecha).
Seleccionadas actualmente: %2.</translation>
    </message>
    <message>
        <source>The session names the hash type %1, which this hashcat version does not support.</source>
        <translation>La sesión indica el tipo de hash %1, que esta versión de hashcat no admite.</translation>
    </message>
    <message>
        <source>Password candidate mask. ?1–?4 insert a custom charset, ?w inserts the wordlist word and ?q a second wordlist word in the Hybrid (mask + ?w/?q) mode, and ?? is a literal ?.</source>
        <translation>Máscara de candidatos de contraseña. ?1–?4 insertan un juego de caracteres personalizado, ?w inserta la palabra de la lista de palabras y ?q una segunda palabra de la lista de palabras en el modo Híbrido (máscara + ?w/?q), y ?? es un ? literal.</translation>
    </message>
    <message>
        <source>Load the %1 example? The current session is lost.

The example cracks a test hash; hashcat should recover the password %2.</source>
        <translation>¿Cargar el ejemplo %1? La sesión actual se perderá.

El ejemplo descifra un hash de prueba; hashcat debería recuperar la contraseña %2.</translation>
    </message>
    <message>
        <source>Could not create the example directory “%1”.</source>
        <translation>No se pudo crear el directorio de ejemplo “%1”.</translation>
    </message>
    <message>
        <source>Could not read the bundled file “%1”: %2</source>
        <translation>No se pudo leer el archivo incluido “%1”: %2</translation>
    </message>
    <message>
        <source>Could not write “%1”: %2</source>
        <translation>No se pudo escribir “%1”: %2</translation>
    </message>
    <message>
        <source>Straight</source>
        <translation>Diccionario</translation>
    </message>
    <message>
        <source>Combination</source>
        <translation>Combinación</translation>
    </message>
    <message>
        <source>Brute-force</source>
        <translation>Fuerza bruta</translation>
    </message>
    <message>
        <source>Hybrid Wordlist + Mask</source>
        <translation>Híbrido: lista de palabras + máscara</translation>
    </message>
    <message>
        <source>Hybrid Mask + Wordlist</source>
        <translation>Híbrido: máscara + lista de palabras</translation>
    </message>
    <message>
        <source>Generic (feed)</source>
        <translation>Genérico (feed)</translation>
    </message>
    <message>
        <source>Association</source>
        <translation>Asociación</translation>
    </message>
    <message>
        <source>Hybrid (mask + ?w/?q)</source>
        <translation>Híbrido (máscara + ?w/?q)</translation>
    </message>
    <message>
        <source>Failed to obtain supported hash types.
Error: %1</source>
        <translation>No se pudieron obtener los tipos de hash compatibles.
Error: %1</translation>
    </message>
    <message>
        <source>The hash types could not be loaded. Check the hashcat path in Settings.</source>
        <translation>No se pudieron cargar los tipos de hash. Compruebe la ruta de hashcat en Ajustes.</translation>
    </message>
    <message>
        <source>

This can be caused by Windows Defender or other antivirus software. See the &lt;a href=&quot;%1&quot;&gt;FAQ entry&lt;/a&gt; for a remedy.</source>
        <translation>

Esto puede deberse a Windows Defender u otro software antivirus. Consulte la &lt;a href=&quot;%1&quot;&gt;entrada de las preguntas frecuentes&lt;/a&gt; para ver cómo solucionarlo.</translation>
    </message>
    <message>
        <source>Set the hashcat path under %1 → %2 to load the available hash types.</source>
        <translation>Defina la ruta de hashcat en %1 → %2 para cargar los tipos de hash disponibles.</translation>
    </message>
    <message>
        <source>This mode needs exactly 2 wordlists (left, right).</source>
        <translation>Este modo necesita exactamente 2 listas de palabras (izquierda, derecha).</translation>
    </message>
    <message>
        <source>This mode needs a mask.</source>
        <translation>Este modo necesita una máscara.</translation>
    </message>
    <message>
        <source>This mode runs hashcat’s bundled default-passwords ruleset; name one or more to use your own.</source>
        <translation>Este modo ejecuta el conjunto de reglas default-passwords incluido con hashcat; indique uno o más para usar los suyos.</translation>
    </message>
    <message>
        <source>This mode needs one wordlist and at least one table.</source>
        <translation>Este modo necesita una lista de palabras y al menos una tabla.</translation>
    </message>
    <message>
        <source>This mode needs a feed. Arguments depend on the feed: random and stdin need none, wordlist needs a path.</source>
        <translation>Este modo necesita un feed. Los argumentos dependen del feed: random y stdin no necesitan ninguno, wordlist necesita una ruta.</translation>
    </message>
    <message>
        <source>This mode needs at most one wordlist. Without one the words come from the hash file and the settings line can bound the run; with one the two files are paired by line.</source>
        <translation>Este modo necesita como máximo una lista de palabras. Sin ella, las palabras proceden del archivo de hashes y la línea de ajustes puede acotar la ejecución; con una, los dos archivos se emparejan línea por línea.</translation>
    </message>
    <message>
        <source>This mode needs a mask with exactly one ?w; a ?q after ?w names a second wordlist.</source>
        <translation>Este modo necesita una máscara con exactamente un ?w; un ?q después de ?w designa una segunda lista de palabras.</translation>
    </message>
    <message>
        <source>Open cache directory</source>
        <translation>Abrir directorio de la caché</translation>
    </message>
    <message>
        <source>Add wordlist folder</source>
        <translation>Añadir carpeta de listas de palabras</translation>
    </message>
    <message>
        <source>Add rulesets</source>
        <translation>Añadir conjuntos de reglas</translation>
    </message>
    <message>
        <source>Add ruleset folder</source>
        <translation>Añadir carpeta de conjunto de reglas</translation>
    </message>
    <message>
        <source>Add tables</source>
        <translation>Añadir tablas</translation>
    </message>
    <message>
        <source>Open feed file</source>
        <translation>Abrir archivo de feed</translation>
    </message>
    <message>
        <source>This folder has no wordlists. hashcat reads the regular files directly inside it, not subfolders.</source>
        <translation>Esta carpeta no tiene listas de palabras. hashcat lee los archivos normales situados directamente dentro de ella, no las subcarpetas.</translation>
    </message>
    <message>
        <source>This wordlist does not exist:
%1</source>
        <translation>Esta lista de palabras no existe:
%1</translation>
    </message>
    <message>
        <source>This ruleset folder has no Grammar/grammar.txt:
%1</source>
        <translation>Esta carpeta de conjunto de reglas no tiene Grammar/grammar.txt:
%1</translation>
    </message>
    <message>
        <source>This ruleset does not exist:
%1</source>
        <translation>Este conjunto de reglas no existe:
%1</translation>
    </message>
    <message>
        <source>A table must be a file; hashcat does not read a folder:
%1</source>
        <translation>Una tabla debe ser un archivo; hashcat no lee una carpeta:
%1</translation>
    </message>
    <message>
        <source>This table does not exist:
%1</source>
        <translation>Esta tabla no existe:
%1</translation>
    </message>
    <message>
        <source>Copied to the clipboard</source>
        <translation>Copiado al portapapeles</translation>
    </message>
    <message>
        <source>Launch failed</source>
        <translation>Error al iniciar</translation>
    </message>
    <message>
        <source>Please choose a hash file.</source>
        <translation>Elija un archivo de hashes.</translation>
    </message>
    <message>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to configure the path to the hashcat executable.</source>
        <translation>Vaya a &lt;b&gt;%1 → %2&lt;/b&gt; para configurar la ruta del ejecutable de hashcat.</translation>
    </message>
    <message>
        <source>The configured hashcat executable does not exist:
%1
Navigate to %2 → %3 to change it.</source>
        <translation>El ejecutable de hashcat configurado no existe:
%1
Vaya a %2 → %3 para cambiarlo.</translation>
    </message>
    <message>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to select the terminal used for launching.</source>
        <translation>Vaya a &lt;b&gt;%1 → %2&lt;/b&gt; para seleccionar el terminal usado para el inicio.</translation>
    </message>
    <message>
        <source>Maximum number of wordlists for this mode: %1.
Currently selected: %2.</source>
        <translation>Número máximo de listas de palabras para este modo: %1.
Seleccionadas actualmente: %2.</translation>
    </message>
    <message>
        <source>hashcat pairs the two files by line here; this GUI offers only one wordlist for that role.</source>
        <translation>Aquí hashcat empareja los dos archivos línea por línea; esta interfaz ofrece solo una lista de palabras para ese papel.</translation>
    </message>
    <message>
        <source>This wordlist has to be a file: hashcat reads it as the multiplied wordlist (the ?q side), and a folder cannot be used there:
%1</source>
        <translation>Esta lista de palabras debe ser un archivo: hashcat la lee como la lista de palabras multiplicada (el lado ?q) y allí no se puede usar una carpeta:
%1</translation>
    </message>
    <message>
        <source>This mode needs at least one table.</source>
        <translation>Este modo necesita al menos una tabla.</translation>
    </message>
    <message>
        <source>This mode needs a feed: a bundled name (wordlist, stdin, random, …) or the path of a feed file.</source>
        <translation>Este modo necesita un feed: un nombre incluido (wordlist, stdin, random, …) o la ruta de un archivo de feed.</translation>
    </message>
    <message>
        <source>This mode pairs the wordlists with the mask; the mask needs a ?w for the word.</source>
        <translation>Este modo empareja las listas de palabras con la máscara; la máscara necesita un ?w para la palabra.</translation>
    </message>
    <message>
        <source>The mask contains more than one ?w.</source>
        <translation>La máscara contiene más de un ?w.</translation>
    </message>
    <message>
        <source>The mask contains a ?q before the first ?w.</source>
        <translation>La máscara contiene un ?q antes del primer ?w.</translation>
    </message>
    <message>
        <source>The mask contains more than one ?q.</source>
        <translation>La máscara contiene más de un ?q.</translation>
    </message>
    <message>
        <source>The mask names a second word with ?q, but only one wordlist is selected.</source>
        <translation>La máscara designa una segunda palabra con ?q, pero solo hay una lista de palabras seleccionada.</translation>
    </message>
    <message>
        <source>Two wordlists are selected, but the mask has no ?q for the second one.</source>
        <translation>Hay dos listas de palabras seleccionadas, pero la máscara no tiene ?q para la segunda.</translation>
    </message>
    <message>
        <source>The increment is not offered in this hybrid mode.</source>
        <translation>El incremento no se ofrece en este modo híbrido.</translation>
    </message>
    <message>
        <source>The ?w and ?q markers are only offered in the hybrid mode (mask + wordlists).</source>
        <translation>Los marcadores ?w y ?q solo se ofrecen en el modo híbrido (máscara + listas de palabras).</translation>
    </message>
    <message>
        <source>The setting “%1” is not valid for this mode.</source>
        <translation>El ajuste “%1” no es válido para este modo.</translation>
    </message>
    <message>
        <source>The token “%1” is not a setting; settings are written as key=value.</source>
        <translation>El token “%1” no es un ajuste; los ajustes se escriben como key=value.</translation>
    </message>
    <message>
        <source>The settings line applies only while the words come from the hash file. Clear either the settings line or the wordlist list.</source>
        <translation>La línea de ajustes solo se aplica mientras las palabras proceden del archivo de hashes. Borre la línea de ajustes o la lista de listas de palabras.</translation>
    </message>
    <message>
        <source>The hash type 4000 describes its hashes as a recipe, for example
md5(md5(salt) . pass). Enter it in the hash recipe field.</source>
        <translation>El tipo de hash 4000 describe sus hashes como una receta, por ejemplo
md5(md5(salt) . pass). Introdúzcala en el campo de receta de hash.</translation>
    </message>
    <message>
        <source>The minimum increment length is larger than the maximum.</source>
        <translation>La longitud mínima del incremento es mayor que la máxima.</translation>
    </message>
    <message>
        <source>The cache path must be an existing directory:
%1</source>
        <translation>La ruta de la caché debe ser un directorio existente:
%1</translation>
    </message>
    <message>
        <source>The increment cannot be combined with the speed-only option.</source>
        <translation>El incremento no se puede combinar con la opción de solo velocidad.</translation>
    </message>
    <message>
        <source>Removing cracked hashes is not possible in the association attack mode.</source>
        <translation>No es posible quitar los hashes descifrados en el modo de ataque de asociación.</translation>
    </message>
</context>
<context>
    <name>SettingsDialog</name>
    <message>
        <source>Settings</source>
        <translation>Ajustes</translation>
    </message>
    <message>
        <source>hashcat executable</source>
        <translation>ejecutable de hashcat</translation>
    </message>
    <message>
        <source>Not configured</source>
        <translation>No configurado</translation>
    </message>
    <message>
        <source>hashcat</source>
        <translation>hashcat</translation>
    </message>
    <message>
        <source>Select…</source>
        <translation>Seleccionar…</translation>
    </message>
    <message>
        <source>&amp;Terminal</source>
        <translation>&amp;Terminal</translation>
    </message>
    <message>
        <source>&amp;Language</source>
        <translation>&amp;Idioma</translation>
    </message>
    <message>
        <source>&amp;Color scheme</source>
        <translation>Esque&amp;ma de color</translation>
    </message>
    <message>
        <source>Not all color schemes are available on all platforms.</source>
        <translation>No todos los esquemas de color están disponibles en todas las plataformas.</translation>
    </message>
    <message>
        <source>&amp;Parameters</source>
        <translation>&amp;Parámetros</translation>
    </message>
    <message>
        <source>&amp;Use short parameters</source>
        <translation>Usar parámetros &amp;cortos</translation>
    </message>
    <message>
        <source>Session and files</source>
        <translation>Sesión y archivos</translation>
    </message>
    <message>
        <source>&amp;Restore the last session at startup</source>
        <translation>&amp;Restaurar la última sesión al iniciar</translation>
    </message>
    <message>
        <source>&amp;Session file</source>
        <translation>Archivo de &amp;sesión</translation>
    </message>
    <message>
        <source>The file the last session is stored in</source>
        <translation>El archivo donde se guarda la última sesión</translation>
    </message>
    <message>
        <source>Open folder</source>
        <translation>Abrir carpeta</translation>
    </message>
    <message>
        <source>Open the folder the session is stored in</source>
        <translation>Abrir la carpeta donde se guarda la sesión</translation>
    </message>
    <message>
        <source>Settings &amp;file</source>
        <translation>Archivo de &amp;ajustes</translation>
    </message>
    <message>
        <source>The file the settings are stored in</source>
        <translation>El archivo donde se guardan los ajustes</translation>
    </message>
    <message>
        <source>Open the folder the settings are stored in</source>
        <translation>Abrir la carpeta donde se guardan los ajustes</translation>
    </message>
    <message>
        <source>General</source>
        <translation>General</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Guardar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Download from %1</source>
        <translation>Descargar desde %1</translation>
    </message>
    <message>
        <source>Use the short option names (-m) instead of the long ones (--hash-type) in the command.</source>
        <translation>Usar los nombres cortos de las opciones (-m) en lugar de los largos (--hash-type) en el comando.</translation>
    </message>
    <message>
        <source>Select a terminal</source>
        <translation>Seleccione un terminal</translation>
    </message>
    <message>
        <source>System default</source>
        <translation>Predeterminado del sistema</translation>
    </message>
    <message>
        <source>Light</source>
        <translation>Claro</translation>
    </message>
    <message>
        <source>Dark</source>
        <translation>Oscuro</translation>
    </message>
    <message>
        <source>The hashcat path “%1” was not found on this computer.</source>
        <translation>La ruta de hashcat “%1” no se encontró en este equipo.</translation>
    </message>
    <message>
        <source>The terminal “%1” is not installed and was not applied.</source>
        <translation>El terminal “%1” no está instalado y no se aplicó.</translation>
    </message>
    <message>
        <source>The color scheme “%1” is not offered and was not applied.</source>
        <translation>El esquema de color “%1” no se ofrece y no se aplicó.</translation>
    </message>
    <message>
        <source>The language “%1” is not offered and was not applied.</source>
        <translation>El idioma “%1” no se ofrece y no se aplicó.</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not installed.</source>
        <translation>El terminal configurado “%1” no está instalado.</translation>
    </message>
    <message>
        <source>Executable Files (*.exe *.bin hashcat);;All Files (*)</source>
        <translation>Archivos ejecutables (*.exe *.bin hashcat);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>No hashcat executable is configured; hashcat cannot be started.</source>
        <translation>No hay ningún ejecutable de hashcat configurado; no se puede iniciar hashcat.</translation>
    </message>
    <message>
        <source>The configured hashcat executable was not found on this computer.</source>
        <translation>El ejecutable de hashcat configurado no se encontró en este equipo.</translation>
    </message>
    <message>
        <source>Select the terminal hashcat is started in.</source>
        <translation>Seleccione el terminal en el que se inicia hashcat.</translation>
    </message>
    <message>
        <source>Select the hashcat executable</source>
        <translation>Seleccionar el ejecutable de hashcat</translation>
    </message>
    <message>
        <source>Invalid file</source>
        <translation>Archivo no válido</translation>
    </message>
    <message>
        <source>The selected file is not an executable.</source>
        <translation>El archivo seleccionado no es un ejecutable.</translation>
    </message>
    <message>
        <source>Restart required</source>
        <translation>Reinicio necesario</translation>
    </message>
    <message>
        <source>Restart hashcat-gui to apply the new language.</source>
        <translation>Reinicie hashcat-gui para aplicar el nuevo idioma.</translation>
    </message>
</context>
<context>
    <name>StateFile</name>
    <message>
        <source>The file is not valid JSON: %1</source>
        <translation>El archivo no es un JSON válido: %1</translation>
    </message>
    <message>
        <source>The file does not contain a JSON object.</source>
        <translation>El archivo no contiene un objeto JSON.</translation>
    </message>
    <message>
        <source>The file does not contain any sections.</source>
        <translation>El archivo no contiene ninguna sección.</translation>
    </message>
    <message>
        <source>Could not open “%1” for writing: %2</source>
        <translation>No se pudo abrir “%1” para escritura: %2</translation>
    </message>
    <message>
        <source>Could not write to “%1”: %2</source>
        <translation>No se pudo escribir en “%1”: %2</translation>
    </message>
    <message>
        <source>Could not open “%1” for reading: %2</source>
        <translation>No se pudo abrir “%1” para lectura: %2</translation>
    </message>
    <message>
        <source>The file does not contain a “%1” section.</source>
        <translation>El archivo no contiene una sección “%1”.</translation>
    </message>
    <message>
        <source>The file does not contain a “%1” section; it holds %2.</source>
        <translation>El archivo no contiene una sección “%1”; contiene %2.</translation>
    </message>
    <message>
        <source>The “%1” section does not contain any values.</source>
        <translation>La sección “%1” no contiene ningún valor.</translation>
    </message>
    <message>
        <source> and </source>
        <translation> y </translation>
    </message>
    <message>
        <source> and %1 more</source>
        <translation> y %1 más</translation>
    </message>
    <message>
        <source>The file names values this version does not know, so they were not applied: %1.</source>
        <translation>El archivo menciona valores que esta versión no conoce, por lo que no se aplicaron: %1.</translation>
    </message>
    <message>
        <source>The file holds values of the wrong type, so they were not applied: %1.</source>
        <translation>El archivo contiene valores de tipo incorrecto, por lo que no se aplicaron: %1.</translation>
    </message>
</context>
<context>
    <name>UpdateChecker</name>
    <message>
        <source>GitHub answered with HTTP status %1: %2</source>
        <translation>GitHub respondió con el estado HTTP %1: %2</translation>
    </message>
    <message>
        <source>GitHub answered with HTTP status %1.</source>
        <translation>GitHub respondió con el estado HTTP %1.</translation>
    </message>
    <message>
        <source>The release information is not valid JSON.</source>
        <translation>La información de la versión no es un JSON válido.</translation>
    </message>
    <message>
        <source>The release information does not name a version.</source>
        <translation>La información de la versión no indica ninguna versión.</translation>
    </message>
    <message>
        <source>The release version %1 is not a valid version number.</source>
        <translation>La versión publicada %1 no es un número de versión válido.</translation>
    </message>
    <message>
        <source>The installed version %1 is not a valid version number.</source>
        <translation>La versión instalada %1 no es un número de versión válido.</translation>
    </message>
</context>
</TS>
