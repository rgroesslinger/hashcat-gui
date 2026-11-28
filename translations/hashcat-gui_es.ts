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
        <source>Ask GitHub whether a newer release is available.</source>
        <translation>Preguntar a GitHub si hay una versión más reciente disponible.</translation>
    </message>
    <message>
        <source>Copy the application, toolkit, system and hashcat versions to the clipboard.</source>
        <translation>Copiar al portapapeles las versiones de la aplicación, el toolkit, el sistema y hashcat.</translation>
    </message>
    <message>
        <source>Check for updates</source>
        <translation>Buscar actualizaciones</translation>
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
        <source>The configured hashcat executable is not found.</source>
        <translation>No se encontró el ejecutable de hashcat configurado.</translation>
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
    <name>AttackModeView</name>
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
        <source>PCFG</source>
        <translation>PCFG</translation>
    </message>
    <message>
        <source>Table</source>
        <translation>Tabla</translation>
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
        <source>This mode needs at least one wordlist.</source>
        <translation>Este modo necesita al menos una lista de palabras.</translation>
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
        <source>This mode needs a wordlist and a mask.</source>
        <translation>Este modo necesita una lista de palabras y una máscara.</translation>
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
</context>
<context>
    <name>BackendInfoDialog</name>
    <message>
        <source>Backend information</source>
        <translation>Información del backend</translation>
    </message>
    <message>
        <source>Query hashcat again for backend information.</source>
        <translation>Consultar de nuevo a hashcat para obtener información del backend.</translation>
    </message>
    <message>
        <source>&amp;Refresh</source>
        <translation>Actu&amp;alizar</translation>
    </message>
    <message>
        <source>Stop the running query.</source>
        <translation>Detener la consulta en curso.</translation>
    </message>
    <message>
        <source>C&amp;ancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Copy the backend information to the clipboard.</source>
        <translation>Copiar la información del backend al portapapeles.</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>C&amp;opiar</translation>
    </message>
    <message>
        <source>Close the dialog.</source>
        <translation>Cerrar el diálogo.</translation>
    </message>
    <message>
        <source>Cl&amp;ose</source>
        <translation>C&amp;errar</translation>
    </message>
    <message>
        <source>No hashcat executable is configured. Set one in Settings.</source>
        <translation>No hay ningún ejecutable de hashcat configurado. Defina uno en Ajustes.</translation>
    </message>
    <message>
        <source>The configured hashcat executable is not found:
%1</source>
        <translation>No se encontró el ejecutable de hashcat configurado:
%1</translation>
    </message>
    <message>
        <source>Reading backend information…</source>
        <translation>Leyendo información del backend…</translation>
    </message>
    <message>
        <source>Backend query cancelled.</source>
        <translation>Consulta de backend cancelada.</translation>
    </message>
    <message>
        <source>hashcat timed out while reading backend information.</source>
        <translation>hashcat agotó el tiempo de espera al leer la información del backend.</translation>
    </message>
    <message>
        <source>hashcat could not read backend information.</source>
        <translation>hashcat no pudo leer la información del backend.</translation>
    </message>
    <message>
        <source>hashcat could not read backend information:
%1</source>
        <translation>hashcat no pudo leer la información del backend:
%1</translation>
    </message>
    <message>
        <source>Backend information reported by hashcat.</source>
        <translation>Información del backend notificada por hashcat.</translation>
    </message>
</context>
<context>
    <name>ExampleSession</name>
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
    <name>HashcatRunner</name>
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
        <source>hashcat failed to run
%1</source>
        <translation>Error al ejecutar hashcat
%1</translation>
    </message>
</context>
<context>
    <name>IdentifyDialog</name>
    <message>
        <source>Identify hash</source>
        <translation>Identificar hash</translation>
    </message>
    <message>
        <source>&amp;Hash or hash file:</source>
        <translation>&amp;Hash o archivo de hashes:</translation>
    </message>
    <message>
        <source>Paste a hash or select a hash file to identify.</source>
        <translation>Pegue un hash o seleccione un archivo de hashes para identificarlo.</translation>
    </message>
    <message>
        <source>Paste a hash or select a hash file</source>
        <translation>Pegue un hash o seleccione un archivo de hashes</translation>
    </message>
    <message>
        <source>Choose a hash file.</source>
        <translation>Elegir un archivo de hashes.</translation>
    </message>
    <message>
        <source>&amp;Browse…</source>
        <translation>&amp;Examinar…</translation>
    </message>
    <message>
        <source>Identify the hash type of the entered hash.</source>
        <translation>Identificar el tipo de hash del hash introducido.</translation>
    </message>
    <message>
        <source>&amp;Identify</source>
        <translation>&amp;Identificar</translation>
    </message>
    <message>
        <source>Paste a hash or choose a hash file, then select Identify.</source>
        <translation>Pegue un hash o elija un archivo de hashes y luego seleccione Identificar.</translation>
    </message>
    <message>
        <source>Identification progress</source>
        <translation>Progreso de la identificación</translation>
    </message>
    <message>
        <source>Elapsed time</source>
        <translation>Tiempo transcurrido</translation>
    </message>
    <message>
        <source>Matching hash types; select one and apply it to the attack.</source>
        <translation>Tipos de hash coincidentes; seleccione uno y aplíquelo al ataque.</translation>
    </message>
    <message>
        <source>Mode</source>
        <translation>Modo</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nombre</translation>
    </message>
    <message>
        <source>Category</source>
        <translation>Categoría</translation>
    </message>
    <message>
        <source>Use the selected hash type in the attack.</source>
        <translation>Usar el tipo de hash seleccionado en el ataque.</translation>
    </message>
    <message>
        <source>Apply mode to attack</source>
        <translation>Aplicar modo al ataque</translation>
    </message>
    <message>
        <source>Stop the running identification.</source>
        <translation>Detener la identificación en curso.</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Close the dialog.</source>
        <translation>Cerrar el diálogo.</translation>
    </message>
    <message>
        <source>Cl&amp;ose</source>
        <translation>Ce&amp;rrar</translation>
    </message>
    <message>
        <source>Elapsed: %1 s</source>
        <translation>Transcurrido: %1 s</translation>
    </message>
    <message>
        <source>Multiple lines pasted. Use Browse to select a hash file, or paste one hash.</source>
        <translation>Se pegaron varias líneas. Use Examinar para seleccionar un archivo de hashes o pegue un solo hash.</translation>
    </message>
    <message>
        <source>Input changed. Identify it again.</source>
        <translation>La entrada ha cambiado. Identifíquela de nuevo.</translation>
    </message>
    <message>
        <source>Select a hash file</source>
        <translation>Seleccionar un archivo de hashes</translation>
    </message>
    <message>
        <source>Enter a hash or select a hash file first.</source>
        <translation>Introduzca un hash o seleccione un archivo de hashes primero.</translation>
    </message>
    <message>
        <source>No hashcat executable is configured. Set one in Settings.</source>
        <translation>No hay ningún ejecutable de hashcat configurado. Defina uno en Ajustes.</translation>
    </message>
    <message>
        <source>The configured hashcat executable is not found:
%1</source>
        <translation>No se encontró el ejecutable de hashcat configurado:
%1</translation>
    </message>
    <message>
        <source>Identifying hash…</source>
        <translation>Identificando el hash…</translation>
    </message>
    <message>
        <source>Identification cancelled.</source>
        <translation>Identificación cancelada.</translation>
    </message>
    <message>
        <source>hashcat timed out while identifying the hash.</source>
        <translation>hashcat agotó el tiempo de espera al identificar el hash.</translation>
    </message>
    <message>
        <source>No hash modes match this structure.</source>
        <translation>Ningún modo de hash coincide con esta estructura.</translation>
    </message>
    <message>
        <source>1 hash mode matches this structure. Select it and apply the mode and hash to the attack.</source>
        <translation>1 modo de hash coincide con esta estructura. Selecciónelo y aplique el modo y el hash al ataque.</translation>
    </message>
    <message>
        <source>1 hash mode matches this structure. Select it and apply the mode to the attack.</source>
        <translation>1 modo de hash coincide con esta estructura. Selecciónelo y aplique el modo al ataque.</translation>
    </message>
    <message>
        <source>%1 hash modes match this structure. Select one and apply the mode and hash to the attack.</source>
        <translation>%1 modos de hash coinciden con esta estructura. Seleccione uno y aplique el modo y el hash al ataque.</translation>
    </message>
    <message>
        <source>%1 hash modes match this structure. Select one and apply the mode to the attack.</source>
        <translation>%1 modos de hash coinciden con esta estructura. Seleccione uno y aplique el modo al ataque.</translation>
    </message>
    <message>
        <source>Cancelling…</source>
        <translation>Cancelando…</translation>
    </message>
    <message>
        <source>Apply mode and hash to attack</source>
        <translation>Aplicar modo y hash al ataque</translation>
    </message>
    <message>
        <source>Another hashcat instance is using this session. Close it and try again.</source>
        <translation>Otra instancia de hashcat está usando esta sesión. Ciérrela e inténtelo de nuevo.</translation>
    </message>
    <message>
        <source>hashcat could not load any hashes from the input.</source>
        <translation>hashcat no pudo cargar ningún hash de la entrada.</translation>
    </message>
    <message>
        <source>The input contains characters hashcat cannot encode.</source>
        <translation>La entrada contiene caracteres que hashcat no puede codificar.</translation>
    </message>
    <message>
        <source>hashcat reported:
%1</source>
        <translation>hashcat informó:
%1</translation>
    </message>
    <message>
        <source>hashcat’s output could not be parsed. Check the hashcat version or report a bug.</source>
        <translation>No se pudo analizar la salida de hashcat. Compruebe la versión de hashcat o informe de un error.</translation>
    </message>
</context>
<context>
    <name>LaunchMessage</name>
    <message>
        <source>Windows executable names include their extension, for example hashcat.exe.</source>
        <translation>Los nombres de los ejecutables de Windows incluyen su extensión, por ejemplo hashcat.exe.</translation>
    </message>
    <message>
        <source>hash file</source>
        <translation>archivo de hashes</translation>
    </message>
    <message>
        <source>outfile</source>
        <translation>archivo de salida</translation>
    </message>
    <message>
        <source>potfile</source>
        <translation>potfile</translation>
    </message>
    <message>
        <source>wordlist</source>
        <translation>lista de palabras</translation>
    </message>
    <message>
        <source>rules file</source>
        <translation>archivo de reglas</translation>
    </message>
    <message>
        <source>table</source>
        <translation>tabla</translation>
    </message>
    <message>
        <source>cache folder</source>
        <translation>carpeta de caché</translation>
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
        <source>The configured hashcat path is not an executable file:
%1
Navigate to %2 → %3 to change it.</source>
        <translation>La ruta de hashcat configurada no es un archivo ejecutable:
%1
Vaya a %2 → %3 para cambiarla.</translation>
    </message>
    <message>
        <source>Navigate to &lt;b&gt;%1 → %2&lt;/b&gt; to select the terminal used for launching.</source>
        <translation>Vaya a &lt;b&gt;%1 → %2&lt;/b&gt; para seleccionar el terminal usado para el inicio.</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not available. Available terminals: %2
Navigate to %3 → %4 to change it.</source>
        <translation>El terminal configurado “%1” no está disponible. Terminales disponibles: %2
Vaya a %3 → %4 para cambiarlo.</translation>
    </message>
    <message>
        <source>This mode needs at least one wordlist.</source>
        <translation>Este modo necesita al menos una lista de palabras.</translation>
    </message>
    <message>
        <source>This mode needs exactly %1 wordlists (left, right).
Currently selected: %2.</source>
        <translation>Este modo necesita exactamente %1 listas de palabras (izquierda, derecha).
Seleccionadas actualmente: %2.</translation>
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
        <source>This mode needs a mask.</source>
        <translation>Este modo necesita una máscara.</translation>
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
        <source>A custom charset is used only inside a mask, and this run named no mask. Add a mask= setting that references ?1–?4, for example mask=?1?1?1?1, or clear the custom charset.</source>
        <translation>Un juego de caracteres personalizado solo se usa dentro de una máscara, y esta ejecución no indicó ninguna máscara. Añada un ajuste mask= que haga referencia a ?1–?4, por ejemplo mask=?1?1?1?1, o borre el juego de caracteres personalizado.</translation>
    </message>
    <message>
        <source>The PCFG feed reads at most %1 rulesets.
Currently selected: %2.</source>
        <translation>El feed PCFG lee como máximo %1 conjuntos de reglas.
Seleccionados actualmente: %2.</translation>
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
        <source>Enter an outfile format: one or more comma-separated numbers from 1 to 6.</source>
        <translation>Introduzca un formato para el archivo de salida: uno o más números del 1 al 6 separados por comas.</translation>
    </message>
    <message>
        <source>The outfile format must be one or more comma-separated numbers from 1 to 6:
%1</source>
        <translation>El formato del archivo de salida debe ser uno o más números del 1 al 6 separados por comas:
%1</translation>
    </message>
    <message>
        <source>The cache path must be an existing directory:
%1</source>
        <translation>La ruta de la caché debe ser un directorio existente:
%1</translation>
    </message>
    <message>
        <source>The potfile path must be a writable file, or a new file in an existing writable folder:
%1</source>
        <translation>La ruta del potfile debe ser un archivo en el que se pueda escribir, o un archivo nuevo en una carpeta existente en la que se pueda escribir:
%1</translation>
    </message>
    <message>
        <source>The skip or limit value must be a whole number from 0 to %1:
%2</source>
        <translation>El valor de omitir o límite debe ser un número entero entre 0 y %1:
%2</translation>
    </message>
    <message>
        <source>The device types must be 1 (CPU) or 2 (GPU), separated by commas:
%1</source>
        <translation>Los tipos de dispositivo deben ser 1 (CPU) o 2 (GPU), separados por comas:
%1</translation>
    </message>
    <message>
        <source>The CPU affinity must be a comma-separated list of CPU numbers:
%1</source>
        <translation>La afinidad de CPU debe ser una lista de números de CPU separados por comas:
%1</translation>
    </message>
    <message>
        <source>The backend devices must be a comma-separated list of device numbers starting at 1:
%1</source>
        <translation>Los dispositivos backend deben ser una lista de números de dispositivo separados por comas que empiece en 1:
%1</translation>
    </message>
    <message>
        <source>The session name must not contain a path separator:
%1</source>
        <translation>El nombre de sesión no debe contener un separador de ruta:
%1</translation>
    </message>
    <message>
        <source>A ruleset folder needs Grammar/grammar.txt; choose a rules file instead:
%1</source>
        <translation>Una carpeta de conjunto de reglas necesita Grammar/grammar.txt; elija en su lugar un archivo de reglas:
%1</translation>
    </message>
    <message>
        <source>A table has to be a file; hashcat does not read a folder:
%1</source>
        <translation>Una tabla debe ser un archivo; hashcat no lee carpetas:
%1</translation>
    </message>
    <message>
        <source>The %1 and the %2 use the same path:
%3
hashcat writes one or both of them, so a run could change the file. Choose different paths.</source>
        <translation>El %1 y el %2 usan la misma ruta:
%3
hashcat escribe uno o ambos, por lo que una ejecución podría modificar el archivo. Elija rutas diferentes.</translation>
    </message>
    <message>
        <source>The increment cannot be combined with the speed-only option.</source>
        <translation>El incremento no se puede combinar con la opción de solo velocidad.</translation>
    </message>
    <message>
        <source>Removing cracked hashes is not possible in the association attack mode.</source>
        <translation>No es posible quitar los hashes descifrados en el modo de ataque de asociación.</translation>
    </message>
    <message>
        <source>Keep guessing cannot be combined with removing cracked hashes from the hash file. Clear either option.</source>
        <translation>Seguir adivinando no se puede combinar con quitar los hashes descifrados del archivo de hashes. Desactive una de las dos opciones.</translation>
    </message>
    <message>
        <source>This value contains a control character that cannot be passed to hashcat:
%1
Remove the character and try again.</source>
        <translation>Este valor contiene un carácter de control que no se puede pasar a hashcat:
%1
Elimine el carácter e inténtelo de nuevo.</translation>
    </message>
    <message>
        <source>The configured terminal “%1” is not available.</source>
        <translation>El terminal configurado “%1” no está disponible.</translation>
    </message>
    <message>
        <source>The command line is too long for a PowerShell terminal. The encoded script must fit into 32,767 characters, which leaves room for about 12,000 ASCII characters of hashcat arguments. Shorten the argument lists or their values and try again.</source>
        <translation>La línea de comandos es demasiado larga para un terminal PowerShell. El script codificado debe caber en 32&#xa0;767 caracteres, lo que deja espacio para unos 12&#xa0;000 caracteres ASCII de argumentos de hashcat. Acorte las listas de argumentos o sus valores e inténtelo de nuevo.</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
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
        <source>Move selected wordlist up</source>
        <translation>Subir la lista de palabras seleccionada</translation>
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
        <source>Move selected ruleset up.</source>
        <translation>Subir el conjunto de reglas seleccionado.</translation>
    </message>
    <message>
        <source>Move selected ruleset up</source>
        <translation>Subir el conjunto de reglas seleccionado</translation>
    </message>
    <message>
        <source>Move selected ruleset down.</source>
        <translation>Bajar el conjunto de reglas seleccionado.</translation>
    </message>
    <message>
        <source>Move selected ruleset down</source>
        <translation>Bajar el conjunto de reglas seleccionado</translation>
    </message>
    <message>
        <source>Sca&amp;le:</source>
        <translation>Esca&amp;la:</translation>
    </message>
    <message>
        <source>Quantization steps per bit (1–64).</source>
        <translation>Pasos de cuantización por bit (1–64).</translation>
    </message>
    <message>
        <source>&amp;Costmax:</source>
        <translation>&amp;Coste máximo:</translation>
    </message>
    <message>
        <source>Highest cost level to enumerate (1–64).</source>
        <translation>Nivel de coste máximo a enumerar (1–64).</translation>
    </message>
    <message>
        <source>&amp;Weights:</source>
        <translation>&amp;Pesos:</translation>
    </message>
    <message>
        <source>&amp;Kbits:</source>
        <translation>&amp;Kilobits:</translation>
    </message>
    <message>
        <source>Bits one inner loop may span (0–27).</source>
        <translation>Bits que puede abarcar un bucle interior (0–27).</translation>
    </message>
    <message>
        <source>Ma&amp;xword:</source>
        <translation>Palabra má&amp;xima:</translation>
    </message>
    <message>
        <source>Maximum candidate length the kernel builds, in 4-byte words: 0 (auto), 16 or 32.</source>
        <translation>Longitud máxima de candidato que construye el kernel, en palabras de 4 bytes: 0 (auto), 16 o 32.</translation>
    </message>
    <message>
        <source>Max&amp;gain:</source>
        <translation>&amp;Ganancia máxima:</translation>
    </message>
    <message>
        <source>Space-separated key=value settings: omen, cache, lookup, pwmin, pwmax, hintaccount, hintwords, hintfile, hintrank, hintrepeat. Rulesets go first.</source>
        <translation>Ajustes key=value separados por espacios: omen, cache, lookup, pwmin, pwmax, hintaccount, hintwords, hintfile, hintrank, hintrepeat. Los conjuntos de reglas van primero.</translation>
    </message>
    <message>
        <source>Tables</source>
        <translation>Tablas</translation>
    </message>
    <message>
        <source>Add one or more table files.</source>
        <translation>Añadir uno o más archivos de tablas.</translation>
    </message>
    <message>
        <source>Add table</source>
        <translation>Añadir tabla</translation>
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
        <source>Move selected table up.</source>
        <translation>Subir la tabla seleccionada.</translation>
    </message>
    <message>
        <source>Move selected table down.</source>
        <translation>Bajar la tabla seleccionada.</translation>
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
        <source>Add feed argument</source>
        <translation>Añadir argumento de feed</translation>
    </message>
    <message>
        <source>Feed arguments</source>
        <translation>Argumentos de feed</translation>
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
        <source>Move selected feed argument up.</source>
        <translation>Subir el argumento de feed seleccionado.</translation>
    </message>
    <message>
        <source>Move selected feed argument up</source>
        <translation>Subir el argumento de feed seleccionado</translation>
    </message>
    <message>
        <source>Move selected feed argument down.</source>
        <translation>Bajar el argumento de feed seleccionado.</translation>
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
        <source>Minimum mask length; 0 omits the flag and uses hashcat’s default (1).</source>
        <translation>Longitud mínima de la máscara; 0 omite la opción y usa el valor predeterminado de hashcat (1).</translation>
    </message>
    <message>
        <source>Maximum mask length; 0 omits the flag and uses hashcat’s default (the mask length).</source>
        <translation>Longitud máxima de la máscara; 0 omite la opción y usa el valor predeterminado de hashcat (la longitud de la máscara).</translation>
    </message>
    <message>
        <source>Grow the mask from the right end instead of the left (hashcat -ii).</source>
        <translation>Ampliar la máscara desde el extremo derecho en lugar del izquierdo (hashcat -ii).</translation>
    </message>
    <message>
        <source>Right-to-left</source>
        <translation>De derecha a izquierda</translation>
    </message>
    <message>
        <source>Use ?1 in the mask to insert this charset.</source>
        <translation>Use ?1 en la máscara para insertar este juego de caracteres.</translation>
    </message>
    <message>
        <source>Use ?2 in the mask to insert this charset.</source>
        <translation>Use ?2 en la máscara para insertar este juego de caracteres.</translation>
    </message>
    <message>
        <source>Use ?3 in the mask to insert this charset.</source>
        <translation>Use ?3 en la máscara para insertar este juego de caracteres.</translation>
    </message>
    <message>
        <source>Use ?4 in the mask to insert this charset.</source>
        <translation>Use ?4 en la máscara para insertar este juego de caracteres.</translation>
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
        <source>&amp;Session name:</source>
        <translation>Nombre de &amp;sesión:</translation>
    </message>
    <message>
        <source>R&amp;untime limit:</source>
        <translation>Límite de e&amp;jecución:</translation>
    </message>
    <message>
        <source>&amp;Device types:</source>
        <translation>Tipos de &amp;dispositivo:</translation>
    </message>
    <message>
        <source>Potfile:</source>
        <translation>Potfile:</translation>
    </message>
    <message>
        <source>Read and write recovered hashes in this potfile instead of the default one.</source>
        <translation>Leer y escribir los hashes recuperados en este potfile en lugar del predeterminado.</translation>
    </message>
    <message>
        <source>Choose the potfile path.</source>
        <translation>Elegir la ruta del potfile.</translation>
    </message>
    <message>
        <source>Choose potfile</source>
        <translation>Elegir potfile</translation>
    </message>
    <message>
        <source>Copy command to clipboard.</source>
        <translation>Copiar el comando al portapapeles.</translation>
    </message>
    <message>
        <source>Command preview</source>
        <translation>Vista previa del comando</translation>
    </message>
    <message>
        <source>Ctrl+Shift+I</source>
        <translation>Ctrl+Shift+I</translation>
    </message>
    <message>
        <source>Copy the unencoded PowerShell script that starts hashcat.</source>
        <translation>Copiar el script de PowerShell sin codificar que inicia hashcat.</translation>
    </message>
    <message>
        <source>&amp;Identify Hash…</source>
        <translation>&amp;Identificar hash…</translation>
    </message>
    <message>
        <source>Ask hashcat which hash modes match a hash.</source>
        <translation>Preguntar a hashcat qué modos de hash coinciden con un hash.</translation>
    </message>
    <message>
        <source>&amp;Backend Information…</source>
        <translation>Información del &amp;backend…</translation>
    </message>
    <message>
        <source>Show the devices and backends hashcat detected.</source>
        <translation>Mostrar los dispositivos y backends detectados por hashcat.</translation>
    </message>
    <message>
        <source>Check Against &amp;Potfile…</source>
        <translation>Comprobar con el pot&amp;file…</translation>
    </message>
    <message>
        <source>Check whether the hashes in the hash field are already in the potfile.</source>
        <translation>Comprobar si los hashes del campo de hashes ya están en el potfile.</translation>
    </message>
    <message>
        <source>Potfile I&amp;nspector…</source>
        <translation>I&amp;nspector de potfile…</translation>
    </message>
    <message>
        <source>Browse an explicit potfile without hash-type verification.</source>
        <translation>Examinar un potfile explícito sin verificación del tipo de hash.</translation>
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
        <source>Add files…</source>
        <translation>Añadir archivos…</translation>
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
        <source>Space-separated key=value settings for attack mode 9, e.g. phases=words,rules or rules.rulefile=…. Only used without a wordlist. The default phases=words,rules,pcfg never ends (the pcfg phase is unbounded); set phases=words,rules for a bounded run.</source>
        <translation>Ajustes key=value separados por espacios para el modo de ataque 9, p. ej. phases=words,rules o rules.rulefile=…. Solo se usan sin lista de palabras. El valor predeterminado phases=words,rules,pcfg nunca termina (la fase pcfg no tiene límite); use phases=words,rules para una ejecución acotada.</translation>
    </message>
    <message>
        <source>PCFG</source>
        <translation>PCFG</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Nombre:</translation>
    </message>
    <message>
        <source>A bundled or installed ruleset name, such as default-passwords or hints. A slash means a path; hashcat searches its profile and shared pcfg folders first.</source>
        <translation>Nombre de un conjunto de reglas incluido o instalado, como default-passwords o hints. Una barra indica una ruta; hashcat busca primero en su carpeta de perfil y en las carpetas pcfg compartidas.</translation>
    </message>
    <message>
        <source>default-passwords, hints, or a ruleset path</source>
        <translation>default-passwords, hints o la ruta de un conjunto de reglas</translation>
    </message>
    <message>
        <source>Add a bundled or installed ruleset by name.</source>
        <translation>Añadir por nombre un conjunto de reglas incluido o instalado.</translation>
    </message>
    <message>
        <source>Add ruleset name</source>
        <translation>Añadir nombre de conjunto de reglas</translation>
    </message>
    <message>
        <source>A ruleset folder needs a Grammar/grammar.txt.</source>
        <translation>Una carpeta de conjunto de reglas necesita un Grammar/grammar.txt.</translation>
    </message>
    <message>
        <source>One positive number per ruleset, colon-separated, e.g. 2:1; the count must match the number of rulesets.</source>
        <translation>Un número positivo por conjunto de reglas, separados por dos puntos, p. ej. 2:1; la cantidad debe coincidir con el número de conjuntos de reglas.</translation>
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
        <source>1.0–64.0; decimal point is a dot, not the locale separator.</source>
        <translation>1.0–64.0; el separador decimal es un punto, no el separador de la configuración regional.</translation>
    </message>
    <message>
        <source>Walk:</source>
        <translation>Walk:</translation>
    </message>
    <message>
        <source>1 (default) steps to the next base word where the ordering allows (the fast path); 0 unranks every base word. The candidates are identical either way.</source>
        <translation>1 (predeterminado) avanza a la siguiente palabra base cuando el orden lo permite (la vía rápida); 0 reconstruye cada palabra base a partir de su posición (unranking). Los candidatos son idénticos en ambos casos.</translation>
    </message>
    <message>
        <source>Mask:</source>
        <translation>Máscara:</translation>
    </message>
    <message>
        <source>A mask selects the candidates the grammar may spell. ?1–?4 insert a custom charset. Cannot combine with omen=1, or hintaccount without hintwords/hintfile.</source>
        <translation>Una máscara selecciona los candidatos que la gramática puede generar. ?1–?4 insertan un juego de caracteres personalizado. No se puede combinar con omen=1, ni con hintaccount sin hintwords/hintfile.</translation>
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
        <source>Move selected table up</source>
        <translation>Subir la tabla seleccionada</translation>
    </message>
    <message>
        <source>Move selected table down</source>
        <translation>Bajar la tabla seleccionada</translation>
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
        <source>Copy as PowerShell Script</source>
        <translation>Copiar como script de PowerShell</translation>
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
        <source>Name hashcat uses for its restore file and status output.</source>
        <translation>Nombre que hashcat usa para su archivo de restauración y la salida de estado.</translation>
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
        <source>General</source>
        <translation>General</translation>
    </message>
    <message>
        <source>Ignore username in hash file</source>
        <translation>Ignorar el nombre de usuario en el archivo de hashes</translation>
    </message>
    <message>
        <source>H&amp;ash file or hash:</source>
        <translation>Hash o archi&amp;vo de hashes:</translation>
    </message>
    <message>
        <source>Keep attacking a hash after it is cracked, to find further plaintexts such as collisions. Cannot be combined with removing cracked hashes.</source>
        <translation>Seguir atacando un hash después de descifrarlo para encontrar otras soluciones en claro, como colisiones. No se puede combinar con quitar los hashes descifrados.</translation>
    </message>
    <message>
        <source>Keep guessing</source>
        <translation>Seguir adivinando</translation>
    </message>
    <message>
        <source>&amp;Mode:</source>
        <translation>&amp;Modo:</translation>
    </message>
    <message>
        <source>Hash t&amp;ype:</source>
        <translation>&amp;Tipo de hash:</translation>
    </message>
    <message>
        <source>Hash &amp;recipe:</source>
        <translation>&amp;Receta de hash:</translation>
    </message>
    <message>
        <source>Wordlists</source>
        <translation>Listas de palabras</translation>
    </message>
    <message>
        <source>Move selected wordlist up.</source>
        <translation>Subir la lista de palabras seleccionada.</translation>
    </message>
    <message>
        <source>Move selected wordlist down.</source>
        <translation>Bajar la lista de palabras seleccionada.</translation>
    </message>
    <message>
        <source>Add a folder; hashcat reads every file directly inside it as one wordlist.</source>
        <translation>Añadir una carpeta; hashcat lee cada archivo situado directamente dentro de ella como una lista de palabras.</translation>
    </message>
    <message>
        <source>&amp;Encoding from:</source>
        <translation>Codificación de &amp;origen:</translation>
    </message>
    <message>
        <source>Enc&amp;oding to:</source>
        <translation>Codificación de d&amp;estino:</translation>
    </message>
    <message>
        <source>PCFG rulesets</source>
        <translation>Conjuntos de reglas PCFG</translation>
    </message>
    <message>
        <source>Increment the mask length from the minimum to the maximum.</source>
        <translation>Incrementar la longitud de la máscara del mínimo al máximo.</translation>
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
        <source>Settings exported to “%1”.</source>
        <translation>Ajustes exportados a “%1”.</translation>
    </message>
    <message>
        <source>OpenCL device types to use, separated by commas.</source>
        <translation>Tipos de dispositivo OpenCL que se usarán, separados por comas.</translation>
    </message>
    <message>
        <source>Hash type changed</source>
        <translation>Tipo de hash cambiado</translation>
    </message>
    <message>
        <source>The hash types could not be refreshed; showing the previous list.</source>
        <translation>No se pudieron actualizar los tipos de hash; se muestra la lista anterior.</translation>
    </message>
    <message>
        <source>The hash algorithm (-m). Auto-detect omits -m and lets hashcat identify the type. Type to search the list.</source>
        <translation>El algoritmo de hash (-m). “Detección automática” omite -m y deja que hashcat identifique el tipo. Escriba para buscar en la lista.</translation>
    </message>
    <message>
        <source>Use ?1–?4 in a mask to insert a custom charset. A mask is used by Brute-force, the hybrid modes and PCFG’s mask= setting. In PCFG a custom charset needs mask=; a charset the mask does not reference is ignored.</source>
        <translation>Use ?1–?4 en una máscara para insertar un juego de caracteres personalizado. Fuerza bruta, los modos híbridos y el ajuste mask= de PCFG usan una máscara. En PCFG, un juego de caracteres personalizado necesita mask=; si la máscara no hace referencia a un juego de caracteres, este se ignora.</translation>
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
        <source>Enter a hash or select a hash file first.</source>
        <translation>Introduzca un hash o seleccione un archivo de hashes primero.</translation>
    </message>
    <message>
        <source>No hashcat executable is configured. Set one in Settings.</source>
        <translation>No hay ningún ejecutable de hashcat configurado. Defina uno en Ajustes.</translation>
    </message>
    <message>
        <source>Check Against Potfile</source>
        <translation>Comprobar con el potfile</translation>
    </message>
    <message>
        <source>Checking potfile…</source>
        <translation>Comprobando el potfile…</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Check cancelled.</source>
        <translation>Comprobación cancelada.</translation>
    </message>
    <message>
        <source>hashcat could not query the potfile.</source>
        <translation>hashcat no pudo consultar el potfile.</translation>
    </message>
    <message>
        <source>Identify hash</source>
        <translation>Identificar hash</translation>
    </message>
    <message>
        <source>The hash type list is unavailable or still loading. Nothing was changed. Check the hashcat path in Settings or try again once the list is available.</source>
        <translation>La lista de tipos de hash no está disponible o todavía se está cargando. No se ha cambiado nada. Compruebe la ruta de hashcat en Ajustes o inténtelo de nuevo cuando la lista esté disponible.</translation>
    </message>
    <message>
        <source>Hash mode %1 is not in this hashcat’s hash-type list. Nothing was changed.</source>
        <translation>El modo de hash %1 no está en la lista de tipos de hash de este hashcat. No se ha cambiado nada.</translation>
    </message>
    <message>
        <source>The session file could not be read, so the session was not restored.

%1</source>
        <translation>No se pudo leer el archivo de sesión, por lo que la sesión no se restauró.

%1</translation>
    </message>
    <message>
        <source>Move the file aside to start with a fresh session? It is renamed, never deleted.</source>
        <translation>¿Mover el archivo a un lado para empezar con una sesión nueva? Se renombra, no se elimina.</translation>
    </message>
    <message>
        <source>Move File Aside</source>
        <translation>Mover archivo a un lado</translation>
    </message>
    <message>
        <source>Keep File</source>
        <translation>Conservar archivo</translation>
    </message>
    <message>
        <source>The session file could not be moved to:
%1</source>
        <translation>No se pudo mover el archivo de sesión a:
%1</translation>
    </message>
    <message>
        <source>The session file was moved to:
%1</source>
        <translation>El archivo de sesión se movió a:
%1</translation>
    </message>
    <message>
        <source>The session names the hash type %1, which this hashcat version does not support. Auto-detect was selected.</source>
        <translation>La sesión indica el tipo de hash %1, que esta versión de hashcat no admite. Se seleccionó “Detección automática”.</translation>
    </message>
    <message>
        <source>The selected hash type %1 is not offered by this hashcat version. Auto-detect was selected.</source>
        <translation>Esta versión de hashcat no ofrece el tipo de hash seleccionado %1. Se seleccionó “Detección automática”.</translation>
    </message>
    <message>
        <source>Set the hashcat path under %1 → %2 to load the available hash types.</source>
        <translation>Defina la ruta de hashcat en %1 → %2 para cargar los tipos de hash disponibles.</translation>
    </message>
    <message>
        <source>The configured hashcat executable is not found: %1</source>
        <translation>No se encontró el ejecutable de hashcat configurado: %1</translation>
    </message>
    <message>
        <source>Auto-detect</source>
        <translation>Detección automática</translation>
    </message>
    <message>
        <source>Open cache directory</source>
        <translation>Abrir directorio de la caché</translation>
    </message>
    <message>
        <source>Wait until the hash type list has loaded.</source>
        <translation>Espere a que se cargue la lista de tipos de hash.</translation>
    </message>
    <message>
        <source>The hash type list is still loading. Wait until it has loaded and try again.</source>
        <translation>La lista de tipos de hash todavía se está cargando. Espere a que termine y vuelva a intentarlo.</translation>
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
        <source>Copied to the clipboard</source>
        <translation>Copiado al portapapeles</translation>
    </message>
    <message>
        <source>Select the hashcat executable under %1 → %2 first.</source>
        <translation>Seleccione primero el ejecutable de hashcat en %1 → %2.</translation>
    </message>
    <message>
        <source>The configured hashcat executable is not found:
%1</source>
        <translation>No se encontró el ejecutable de hashcat configurado:
%1</translation>
    </message>
    <message>
        <source>PowerShell script copied to the clipboard</source>
        <translation>Script de PowerShell copiado al portapapeles</translation>
    </message>
    <message>
        <source>Launch failed</source>
        <translation>Error al iniciar</translation>
    </message>
</context>
<context>
    <name>PathListController</name>
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
</context>
<context>
    <name>PotfileCheck</name>
    <message>
        <source>The hashcat path is not configured.</source>
        <translation>La ruta de hashcat no está configurada.</translation>
    </message>
    <message>
        <source>Enter a hash or select a hash file first.</source>
        <translation>Introduzca un hash o seleccione un archivo de hashes primero.</translation>
    </message>
    <message>
        <source>Choose a hash type. Potfile checks need an explicit hash type; Auto-detect is best-effort and can miscount. Use Tools → Identify Hash if you do not know the type.</source>
        <translation>Elija un tipo de hash. Las comprobaciones del potfile necesitan un tipo de hash explícito; la detección automática es aproximada y puede contar mal. Use Herramientas → Identificar hash si no conoce el tipo.</translation>
    </message>
    <message>
        <source>The remaining hashes are binary data; the count is unknown.</source>
        <translation>Los hashes restantes son datos binarios; se desconoce su número.</translation>
    </message>
    <message>
        <source>Choose a hash type.</source>
        <translation>Elija un tipo de hash.</translation>
    </message>
    <message>
        <source>hashcat could not load any hashes from the input.</source>
        <translation>hashcat no pudo cargar ningún hash de la entrada.</translation>
    </message>
    <message>
        <source>hashcat could not parse the input for the selected hash type. Check the hash and the hash type.</source>
        <translation>hashcat no pudo analizar la entrada para el tipo de hash seleccionado. Compruebe el hash y el tipo de hash.</translation>
    </message>
    <message>
        <source>hashcat could not query the potfile.</source>
        <translation>hashcat no pudo consultar el potfile.</translation>
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
        <translation>Exportar los resultados del potfile</translation>
    </message>
    <message>
        <source>Export failed</source>
        <translation>Error al exportar</translation>
    </message>
    <message>
        <source>Choose a path other than the potfile.</source>
        <translation>Elija una ruta distinta del potfile.</translation>
    </message>
    <message>
        <source>The destination is named hashcat.potfile. If this is hashcat’s live potfile, exporting overwrites it.

Export anyway?</source>
        <translation>El destino se llama hashcat.potfile. Si es el potfile activo de hashcat, la exportación lo sobrescribirá.

¿Exportar de todos modos?</translation>
    </message>
    <message>
        <source>The file already exists:
%1

Overwrite it?</source>
        <translation>El archivo ya existe:
%1

¿Sobrescribirlo?</translation>
    </message>
    <message>
        <source>1 entry</source>
        <translation>1 entrada</translation>
    </message>
    <message>
        <source>%1 entries</source>
        <translation>%1 entradas</translation>
    </message>
    <message>
        <source>Export %1 as %2 to:
%3

The file will contain plaintext passwords. Store it locally and protect it. Spreadsheet applications may evaluate cells beginning with =, +, -, or @ as formulas; open the file as text or disable formula evaluation.</source>
        <translation>Exportar %1 como %2 a:
%3

El archivo contendrá contraseñas en texto en claro. Guárdelo localmente y protéjalo. Las hojas de cálculo pueden interpretar como fórmulas las celdas que empiezan por =, +, - o @; abra el archivo como texto o desactive la evaluación de fórmulas.</translation>
    </message>
    <message>
        <source>

Only %1 of %2 entries are included.</source>
        <translation>

Solo se incluyen %1 de %2 entradas.</translation>
    </message>
    <message>
        <source>Export plaintexts</source>
        <translation>Exportar textos en claro</translation>
    </message>
    <message>
        <source>Export</source>
        <translation>Exportar</translation>
    </message>
    <message>
        <source>Export complete</source>
        <translation>Exportación completada</translation>
    </message>
    <message>
        <source>Exported %1 to:
%2</source>
        <translation>Se exportó %1 a:
%2</translation>
    </message>
</context>
<context>
    <name>PotfileExporter</name>
    <message>
        <source>Could not open the export file: %1</source>
        <translation>No se pudo abrir el archivo de exportación: %1</translation>
    </message>
    <message>
        <source>Could not write the export file: %1</source>
        <translation>No se pudo escribir el archivo de exportación: %1</translation>
    </message>
    <message>
        <source>Could not save the export file: %1</source>
        <translation>No se pudo guardar el archivo de exportación: %1</translation>
    </message>
</context>
<context>
    <name>PotfileInspectorDialog</name>
    <message>
        <source>Potfile inspector</source>
        <translation>Inspector de potfile</translation>
    </message>
    <message>
        <source>Parsed directly; duplicates are exact repeated lines. Separator: ‘:’.</source>
        <translation>Analizado directamente; los duplicados son líneas repetidas exactamente. Separador: ‘:’.</translation>
    </message>
    <message>
        <source>&amp;Potfile:</source>
        <translation>&amp;Potfile:</translation>
    </message>
    <message>
        <source>&amp;Browse…</source>
        <translation>&amp;Examinar…</translation>
    </message>
    <message>
        <source>Read the selected potfile again.</source>
        <translation>Leer de nuevo el potfile seleccionado.</translation>
    </message>
    <message>
        <source>&amp;Reload</source>
        <translation>&amp;Volver a cargar</translation>
    </message>
    <message>
        <source>Stop reading the potfile.</source>
        <translation>Detener la lectura del potfile.</translation>
    </message>
    <message>
        <source>C&amp;ancel</source>
        <translation>C&amp;ancelar</translation>
    </message>
    <message>
        <source>The prefilled path is a heuristic; hashcat may use another path.</source>
        <translation>La ruta rellenada previamente es una heurística; hashcat puede usar otra ruta.</translation>
    </message>
    <message>
        <source>Potfile reading progress</source>
        <translation>Progreso de la lectura del potfile</translation>
    </message>
    <message>
        <source>&amp;Statistics…</source>
        <translation>E&amp;stadísticas…</translation>
    </message>
    <message>
        <source>Show aggregate password statistics for the visible entries.</source>
        <translation>Mostrar estadísticas agregadas de contraseñas para las entradas visibles.</translation>
    </message>
    <message>
        <source>&amp;Export…</source>
        <translation>E&amp;xportar…</translation>
    </message>
    <message>
        <source>Export the visible entries as CSV, TSV or JSON.</source>
        <translation>Exportar las entradas visibles como CSV, TSV o JSON.</translation>
    </message>
    <message>
        <source>Close the dialog.</source>
        <translation>Cerrar el diálogo.</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <source>Dup</source>
        <translation>Duplicados</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Línea</translation>
    </message>
    <message>
        <source>This potfile has no entries.</source>
        <translation>Este potfile no tiene entradas.</translation>
    </message>
    <message>
        <source>Choose a potfile.</source>
        <translation>Elegir un potfile.</translation>
    </message>
    <message>
        <source>Select a potfile</source>
        <translation>Seleccionar un potfile</translation>
    </message>
    <message>
        <source>Cancelling…</source>
        <translation>Cancelando…</translation>
    </message>
    <message>
        <source>Potfile: %1</source>
        <translation>Potfile: %1</translation>
    </message>
    <message>
        <source>Potfile not found: %1</source>
        <translation>Potfile no encontrado: %1</translation>
    </message>
    <message>
        <source>Reading potfile…</source>
        <translation>Leyendo el potfile…</translation>
    </message>
    <message>
        <source>Reading cancelled.</source>
        <translation>Lectura cancelada.</translation>
    </message>
    <message>
        <source>dup</source>
        <translation>duplicados</translation>
    </message>
    <message>
        <source>1 entry</source>
        <translation>1 entrada</translation>
    </message>
    <message>
        <source>%1 entries</source>
        <translation>%1 entradas</translation>
    </message>
    <message>
        <source>1 duplicate line</source>
        <translation>1 línea duplicada</translation>
    </message>
    <message>
        <source>1 duplicate line (at least)</source>
        <translation>1 línea duplicada (como mínimo)</translation>
    </message>
    <message>
        <source>%1 duplicate lines</source>
        <translation>%1 líneas duplicadas</translation>
    </message>
    <message>
        <source>%1 duplicate lines (at least)</source>
        <translation>%1 líneas duplicadas (como mínimo)</translation>
    </message>
    <message>
        <source>duplicate count incomplete</source>
        <translation>recuento de duplicados incompleto</translation>
    </message>
    <message>
        <source>1 malformed line</source>
        <translation>1 línea mal formada</translation>
    </message>
    <message>
        <source>%1 malformed lines</source>
        <translation>%1 líneas mal formadas</translation>
    </message>
    <message>
        <source>1 encrypted entry</source>
        <translation>1 entrada cifrada</translation>
    </message>
    <message>
        <source>%1 encrypted entries</source>
        <translation>%1 entradas cifradas</translation>
    </message>
    <message>
        <source>1 entry without a password</source>
        <translation>1 entrada sin contraseña</translation>
    </message>
    <message>
        <source>%1 entries without a password</source>
        <translation>%1 entradas sin contraseña</translation>
    </message>
    <message>
        <source>1 incomplete line skipped</source>
        <translation>1 línea incompleta omitida</translation>
    </message>
    <message>
        <source>%1 incomplete lines skipped</source>
        <translation>%1 líneas incompletas omitidas</translation>
    </message>
    <message>
        <source> · showing first %1 of %2</source>
        <translation> · mostrando las primeras %1 de %2</translation>
    </message>
    <message>
        <source> · %1 hidden by filter</source>
        <translation> · %1 ocultas por el filtro</translation>
    </message>
    <message>
        <source>Reveal plaintexts to export the real values.</source>
        <translation>Revele los textos en claro para exportar los valores reales.</translation>
    </message>
</context>
<context>
    <name>PotfileParser</name>
    <message>
        <source>Could not open the potfile: %1</source>
        <translation>No se pudo abrir el potfile: %1</translation>
    </message>
    <message>
        <source>Could not read the potfile: %1</source>
        <translation>No se pudo leer el potfile: %1</translation>
    </message>
</context>
<context>
    <name>PotfileResultsDialog</name>
    <message>
        <source>Cracked hashes</source>
        <translation>Hashes descifrados</translation>
    </message>
    <message>
        <source>&amp;Statistics…</source>
        <translation>E&amp;stadísticas…</translation>
    </message>
    <message>
        <source>Show aggregate password statistics for the visible results.</source>
        <translation>Mostrar estadísticas agregadas de contraseñas para los resultados visibles.</translation>
    </message>
    <message>
        <source>&amp;Export…</source>
        <translation>&amp;Exportar…</translation>
    </message>
    <message>
        <source>Export the visible results as CSV, TSV or JSON.</source>
        <translation>Exportar los resultados visibles como CSV, TSV o JSON.</translation>
    </message>
    <message>
        <source>Close the dialog.</source>
        <translation>Cerrar el diálogo.</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <source>Username</source>
        <translation>Nombre de usuario</translation>
    </message>
    <message>
        <source>Filter hash or username (plaintext after Reveal)</source>
        <translation>Filtrar hash o nombre de usuario (texto en claro tras revelar)</translation>
    </message>
    <message>
        <source>No cracked hashes for this input.</source>
        <translation>No hay hashes descifrados para esta entrada.</translation>
    </message>
    <message>
        <source>Cracked hashes — mode %1</source>
        <translation>Hashes descifrados — modo %1</translation>
    </message>
    <message>
        <source>Potfile: hashcat’s default potfile. hashcat matches potfile entries by hash; the selected hash type narrows the search.</source>
        <translation>Potfile: el potfile predeterminado de hashcat. hashcat compara las entradas del potfile por hash; el tipo de hash seleccionado limita la búsqueda.</translation>
    </message>
    <message>
        <source>Potfile: %1. hashcat matches potfile entries by hash; the selected hash type narrows the search.</source>
        <translation>Potfile: %1. hashcat compara las entradas del potfile por hash; el tipo de hash seleccionado limita la búsqueda.</translation>
    </message>
    <message>
        <source>%1 of %2 cracked</source>
        <translation>%1 de %2 descifrados</translation>
    </message>
    <message>
        <source>%1 cracked (remaining unknown)</source>
        <translation>%1 descifrados (resto desconocido)</translation>
    </message>
    <message>
        <source>Check cancelled.</source>
        <translation>Comprobación cancelada.</translation>
    </message>
    <message>
        <source>hashcat could not query the potfile.</source>
        <translation>hashcat no pudo consultar el potfile.</translation>
    </message>
    <message>
        <source> · showing first %1 of %2</source>
        <translation> · mostrando las primeras %1 de %2</translation>
    </message>
    <message>
        <source>1 unreadable result line skipped.</source>
        <translation>1 línea de resultado ilegible omitida.</translation>
    </message>
    <message>
        <source>%1 unreadable result lines skipped.</source>
        <translation>%1 líneas de resultado ilegibles omitidas.</translation>
    </message>
    <message>
        <source> · %1 encrypted</source>
        <translation> · %1 cifrados</translation>
    </message>
    <message>
        <source> · includes a binary hash file</source>
        <translation> · incluye un archivo de hashes binario</translation>
    </message>
    <message>
        <source> · %1 hidden by filter</source>
        <translation> · %1 ocultas por el filtro</translation>
    </message>
    <message>
        <source>Reveal plaintexts to export the real values.</source>
        <translation>Revele los textos en claro para exportar los valores reales.</translation>
    </message>
</context>
<context>
    <name>PotfileStatisticsDialog</name>
    <message>
        <source>Password statistics</source>
        <translation>Estadísticas de contraseñas</translation>
    </message>
    <message>
        <source>Password length distribution</source>
        <translation>Distribución de la longitud de las contraseñas</translation>
    </message>
    <message>
        <source>Length</source>
        <translation>Longitud</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation>Contraseñas</translation>
    </message>
    <message>
        <source>Close the dialog.</source>
        <translation>Cerrar el diálogo.</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <source>1 entry</source>
        <translation>1 entrada</translation>
    </message>
    <message>
        <source>%1 entries</source>
        <translation>%1 entradas</translation>
    </message>
    <message>
        <source>1 password analyzed</source>
        <translation>1 contraseña analizada</translation>
    </message>
    <message>
        <source>%1 passwords analyzed</source>
        <translation>%1 contraseñas analizadas</translation>
    </message>
    <message>
        <source>1 duplicate line</source>
        <translation>1 línea duplicada</translation>
    </message>
    <message>
        <source>%1 duplicate lines</source>
        <translation>%1 líneas duplicadas</translation>
    </message>
    <message>
        <source>duplicate lines: unknown</source>
        <translation>líneas duplicadas: desconocido</translation>
    </message>
    <message>
        <source>%1 · %2</source>
        <translation>%1 · %2</translation>
    </message>
    <message>
        <source>%1 empty · %2 encrypted · %3 · %4 unique plaintexts</source>
        <translation>%1 vacías · %2 cifradas · %3 · %4 textos en claro únicos</translation>
    </message>
    <message>
        <source>Length: —</source>
        <translation>Longitud: —</translation>
    </message>
    <message>
        <source>Length: min %1 · max %2 · average %3</source>
        <translation>Longitud: mín. %1 · máx. %2 · media %3</translation>
    </message>
    <message>
        <source>Based on the visible %1 of %2 entries.</source>
        <translation>Basado en las %1 entradas visibles de %2.</translation>
    </message>
    <message>
        <source>No plaintext passwords to analyze.</source>
        <translation>No hay contraseñas en texto en claro que analizar.</translation>
    </message>
    <message>
        <source>Passwords containing:
lowercase: %1 (%2%)
uppercase: %3 (%4%)
digits: %5 (%6%)
symbols: %7 (%8%)
whitespace: %9 (%10%)
non-ASCII: %11 (%12%)</source>
        <translation>Contraseñas que contienen:
minúsculas: %1 (%2%)
mayúsculas: %3 (%4%)
dígitos: %5 (%6%)
símbolos: %7 (%8%)
espacios en blanco: %9 (%10%)
no ASCII: %11 (%12%)</translation>
    </message>
    <message>
        <source>Calculating statistics…</source>
        <translation>Calculando estadísticas…</translation>
    </message>
</context>
<context>
    <name>PotfileTableView</name>
    <message>
        <source>Filter hash (plaintext after Reveal)</source>
        <translation>Filtrar hash (texto en claro tras revelar)</translation>
    </message>
    <message>
        <source>Plaintext is searched only after Reveal plaintexts is enabled.</source>
        <translation>El texto en claro solo se busca después de activar “Revelar textos en claro”.</translation>
    </message>
    <message>
        <source>Copy selected</source>
        <translation>Copiar seleccionados</translation>
    </message>
    <message>
        <source>Reveal plaintexts to copy the real values.</source>
        <translation>Revele los textos en claro para copiar los valores reales.</translation>
    </message>
    <message>
        <source>Copy all</source>
        <translation>Copiar todo</translation>
    </message>
    <message>
        <source>Reveal plaintexts</source>
        <translation>Revelar textos en claro</translation>
    </message>
    <message>
        <source>Show or mask plaintext values.</source>
        <translation>Mostrar u ocultar los valores en claro.</translation>
    </message>
    <message>
        <source>Hash</source>
        <translation>Hash</translation>
    </message>
    <message>
        <source>Plaintext</source>
        <translation>Texto en claro</translation>
    </message>
    <message>
        <source>Copied 1 row</source>
        <translation>Se copió 1 fila</translation>
    </message>
    <message>
        <source>Copied %1 rows</source>
        <translation>Se copiaron %1 filas</translation>
    </message>
    <message>
        <source>Copy selected rows</source>
        <translation>Copiar las filas seleccionadas</translation>
    </message>
    <message>
        <source>Copies tab-separated rows. Encrypted rows copy the stored $HCENC$ token.</source>
        <translation>Copia filas separadas por tabulaciones. Las filas cifradas copian el token $HCENC$ almacenado.</translation>
    </message>
    <message>
        <source>Hide plaintexts</source>
        <translation>Ocultar textos en claro</translation>
    </message>
    <message>
        <source>Binary hash file; hashcat reports the file name.</source>
        <translation>Archivo de hashes binario; hashcat informa el nombre del archivo.</translation>
    </message>
    <message>
        <source>No rows match the filter.</source>
        <translation>Ninguna fila coincide con el filtro.</translation>
    </message>
    <message>
        <source>(encrypted)</source>
        <translation>(cifrado)</translation>
    </message>
    <message>
        <source>(empty)</source>
        <translation>(vacío)</translation>
    </message>
    <message>
        <source>(file) %1</source>
        <translation>(archivo) %1</translation>
    </message>
</context>
<context>
    <name>SessionTransfer</name>
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
        <source>Path to the hashcat executable, or a bare name such as hashcat to look up on PATH.</source>
        <translation>Ruta del ejecutable de hashcat, o un nombre simple como hashcat que se buscará en PATH.</translation>
    </message>
    <message>
        <source>Browse for the hashcat executable.</source>
        <translation>Examinar en busca del ejecutable de hashcat.</translation>
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
        <source>Interface language. Takes effect after a restart.</source>
        <translation>Idioma de la interfaz. Se aplica después de reiniciar.</translation>
    </message>
    <message>
        <source>&amp;Color scheme</source>
        <translation>Esque&amp;ma de color</translation>
    </message>
    <message>
        <source>The file the last session is stored in.</source>
        <translation>El archivo en el que se guarda la última sesión.</translation>
    </message>
    <message>
        <source>Open the folder the session is stored in.</source>
        <translation>Abrir la carpeta en la que se guarda la sesión.</translation>
    </message>
    <message>
        <source>The file the settings are stored in.</source>
        <translation>El archivo en el que se guardan los ajustes.</translation>
    </message>
    <message>
        <source>Open the folder the settings are stored in.</source>
        <translation>Abrir la carpeta en la que se guardan los ajustes.</translation>
    </message>
    <message>
        <source>Not all color schemes are available on all platforms.</source>
        <translation>No todos los esquemas de color están disponibles en todas las plataformas.</translation>
    </message>
    <message>
        <source>&amp;Use short parameters</source>
        <translation>Usar parámetros &amp;cortos</translation>
    </message>
    <message>
        <source>Sho&amp;w the exact command in the terminal</source>
        <translation>Mostrar &amp;el comando exacto en el terminal</translation>
    </message>
    <message>
        <source>Print the exact command hashcat receives in the launch terminal, so a mangled value is visible.</source>
        <translation>Imprimir en el terminal de inicio el comando exacto que recibe hashcat, para que se vea un valor alterado.</translation>
    </message>
    <message>
        <source>Session and files</source>
        <translation>Sesión y archivos</translation>
    </message>
    <message>
        <source>Restore the session saved when the application last closed.</source>
        <translation>Restaurar la sesión guardada cuando se cerró la aplicación por última vez.</translation>
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
        <source>Open folder</source>
        <translation>Abrir carpeta</translation>
    </message>
    <message>
        <source>Settings &amp;file</source>
        <translation>Archivo de &amp;ajustes</translation>
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
        <source>The terminal hashcat is started in.</source>
        <translation>El terminal en el que se inicia hashcat.</translation>
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
        <source>Settings saved</source>
        <translation>Ajustes guardados</translation>
    </message>
    <message>
        <source>The old settings file was moved to:
%1</source>
        <translation>El archivo de ajustes anterior se movió a:
%1</translation>
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
        <source>Settings not saved</source>
        <translation>Ajustes no guardados</translation>
    </message>
    <message>
        <source>Move the file aside and save these values? It is renamed, never deleted.</source>
        <translation>¿Mover el archivo a un lado y guardar estos valores? Se renombra, no se elimina.</translation>
    </message>
    <message>
        <source>Move File Aside and Save</source>
        <translation>Mover archivo a un lado y guardar</translation>
    </message>
    <message>
        <source>Keep File</source>
        <translation>Conservar archivo</translation>
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
    <name>SettingsManager</name>
    <message>
        <source>The settings could not be written to “%1”. Check the file permissions.</source>
        <translation>No se pudieron escribir los ajustes en “%1”. Compruebe los permisos del archivo.</translation>
    </message>
    <message>
        <source>The settings file “%1” is not in a readable format.</source>
        <translation>El archivo de ajustes “%1” no está en un formato legible.</translation>
    </message>
    <message>
        <source>The settings file could not be moved to “%1”.</source>
        <translation>No se pudo mover el archivo de ajustes a “%1”.</translation>
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
        <source>The section names a version this build cannot understand.</source>
        <translation>La sección indica una versión que esta compilación no puede interpretar.</translation>
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
        <source>The “%1” section names a version that is not a number.</source>
        <translation>La sección “%1” indica una versión que no es un número.</translation>
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
    <name>TerminalCatalog</name>
    <message>
        <source>No supported terminal was found. ptyxis is installed, but it changes “$” values before hashcat sees them, so it is not offered. Install another terminal; see the FAQ.</source>
        <translation>No se encontró ningún terminal compatible. ptyxis está instalado, pero modifica los valores “$” antes de que hashcat los vea, por lo que no se ofrece. Instale otro terminal; consulte las preguntas frecuentes.</translation>
    </message>
    <message>
        <source>No supported terminal was found on this system. hashcat-gui needs one to show the hashcat output.</source>
        <translation>No se encontró ningún terminal compatible en este sistema. hashcat-gui necesita uno para mostrar la salida de hashcat.</translation>
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
