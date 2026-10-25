cmake_minimum_required(VERSION 3.24)
project(hashcat-gui
    VERSION 0.8.0
    DESCRIPTION "Qt-based GUI for hashcat"
    HOMEPAGE_URL "https://github.com/rgroesslinger/hashcat-gui/"
    LANGUAGES CXX
)

# The standard install destinations the install() rules below write to.
include(GNUInstallDirs)

# Build in Release mode by default.
#
# Only for single-configuration generators: with a multi-config generator
# CMAKE_BUILD_TYPE is deliberately empty and CMAKE_CONFIGURATION_TYPES drives
# the build.
# CACHE ... FORCE so the value lands in CMakeCache.txt and stays user-overridable
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type specified - defaulting to Release")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# The compilation database the generators support feeds clang-tidy
# and editor integrations. Generators without support ignore the variable
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Flags for everything this file builds - the application and the tests
add_compile_options(-Wall -Wextra -Wpedantic)

# Compile in strict standard mode: no extensions
set(CMAKE_CXX_EXTENSIONS OFF)

# Qt specific preprocessing
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
# The tests live in tests/ but include ui_*.h generated from src/*.ui
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(Qt6 6.8 COMPONENTS Core Gui Widgets Concurrent REQUIRED)

# Everything the application is made of except main().
set(HASHCAT_GUI_SOURCES
    src/aboutdialog.h
    src/aboutdialog.cpp
    src/appconstants.h
    src/commandbuilder.h
    src/commandbuilder.cpp
    src/hashcatinfoparser.h
    src/hashcatinfoparser.cpp
    src/hashcatoptions.h
    src/helperutils.h
    src/helperutils.cpp
    src/mainwindow.h
    src/mainwindow.cpp
    src/settingsdialog.h
    src/settingsdialog.cpp
    src/settingsmanager.h
    src/settingsmanager.cpp
    src/statefile.h
    src/statefile.cpp
    src/widgetstateserializer.h
    src/widgetstateserializer.cpp
    src/wordlistmonitor.h
    src/wordlistmonitor.cpp
    src/resources.qrc
    src/aboutdialog.ui
    src/mainwindow.ui
    src/settingsdialog.ui
)

# The shared sources are compiled once into an object library the application
# and every test link against. Object files rather than a static library, so
# the qrc objects and static initializers cannot be dropped as unreferenced.
add_library(hashcat-gui-core OBJECT ${HASHCAT_GUI_SOURCES})
target_include_directories(hashcat-gui-core PUBLIC src)
target_link_libraries(hashcat-gui-core PUBLIC Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Concurrent)
target_compile_features(hashcat-gui-core PUBLIC cxx_std_20)

# Reject Qt's implicit byte/ASCII <-> QString conversions so every literal has
# to state its encoding (QLatin1StringView, QStringLiteral or fromUtf8).
# PRIVATE, and on each production target, so the tests stay exempt.
set(HASHCAT_GUI_UTF8_DEFINITIONS QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII)
target_compile_definitions(hashcat-gui-core PRIVATE ${HASHCAT_GUI_UTF8_DEFINITIONS})

qt_add_executable(hashcat-gui WIN32
    src/main.cpp
    src/hashcat-gui.rc
)

target_link_libraries(hashcat-gui PRIVATE hashcat-gui-core)

# main.cpp is compiled here, not into hashcat-gui-core, so it needs the same
# UTF-8 guard the object library above carries.
target_compile_definitions(hashcat-gui PRIVATE
    ${HASHCAT_GUI_UTF8_DEFINITIONS}
    HASHCAT_GUI_VERSION="${PROJECT_VERSION}")

# The translation catalogs, one per line. Named "hashcat-gui_<code>", and the
# single list the build (below) and the test embedding derive their catalogs from.
set(HASHCAT_GUI_TRANSLATIONS de es fr it sr ja zh_CN ko)

# A developer is fine with a build without translations; a packager is not:
# the .qm files are compiled into the binary, so a package built without the
# linguist tools ships an untranslated program. The requirement can therefore
# be switched on, and a packager should switch it on.
option(HASHCAT_GUI_REQUIRE_TRANSLATIONS
    "Fail the configure step when the Qt6 LinguistTools are missing" OFF)

# LinguistTools is optional: without it the build still works, just with no
# .qm files.
find_package(Qt6 COMPONENTS LinguistTools QUIET)
if(HASHCAT_GUI_REQUIRE_TRANSLATIONS AND NOT Qt6LinguistTools_FOUND)
    message(FATAL_ERROR
        "HASHCAT_GUI_REQUIRE_TRANSLATIONS is on, but the Qt6 LinguistTools were not found.")
endif()

if(Qt6LinguistTools_FOUND)
    # Derive the .ts sources from the single catalog list.
    set(HASHCAT_GUI_TS_FILES)
    foreach(code IN LISTS HASHCAT_GUI_TRANSLATIONS)
        list(APPEND HASHCAT_GUI_TS_FILES translations/hashcat-gui_${code}.ts)
    endforeach()

    # SOURCE_TARGETS keeps lupdate off the tests, so a tr() added to a test
    # cannot land in the catalogs.
    # Do not carry obsolete entries and line numbers in translations
    qt_add_translations(hashcat-gui
        SOURCE_TARGETS hashcat-gui hashcat-gui-core
        LUPDATE_OPTIONS -no-obsolete -locations none
        TS_FILES ${HASHCAT_GUI_TS_FILES}
    )
else()
    message(STATUS "Qt6 LinguistTools not found - building without translations")
endif()

# Qt6::Test is optional: without it the build still works and runs no tests,
# which CI notices through ctest --no-tests=error. -DBUILD_TESTING=OFF skips
# the test binaries on purpose.
include(CTest)
find_package(Qt6 COMPONENTS Test QUIET)

# The single list of tests: it drives both the executables built below and the
# run properties they are registered with.
set(HASHCAT_GUI_TESTS
    tst_aboutdialog
    tst_commandbuilder
    tst_hashcatinfoparser
    tst_helperutils
    tst_mainwindow
    tst_settingsdialog
    tst_statefile
    tst_translations
    tst_widgetstateserializer
    tst_wordlistmonitor
)

if(NOT BUILD_TESTING)
    message(STATUS "BUILD_TESTING is off - building without tests")
elseif(Qt6Test_FOUND)
    foreach(test IN LISTS HASHCAT_GUI_TESTS)
        add_executable(${test} tests/${test}.cpp)
        target_link_libraries(${test} PRIVATE hashcat-gui-core Qt6::Test)
        add_test(NAME ${test} COMMAND ${test})
    endforeach()

    # tst_translations only proves :/i18n answers at runtime if its binary embeds
    # the catalogs the way the application does; without LinguistTools there are
    # none, and the test skips instead of failing. The embedded .qm files are
    # derived from the same catalog list as the build above.
    if(Qt6LinguistTools_FOUND)
        target_compile_definitions(tst_translations PRIVATE HASHCAT_GUI_QM_EMBEDDED)

        # The .qm files do not exist yet at configure time - qt_add_translations
        # produces them at build time - but qt_add_resources only needs their
        # paths, listed here from the single catalog list.
        set(HASHCAT_GUI_QM_FILES)
        foreach(code IN LISTS HASHCAT_GUI_TRANSLATIONS)
            list(APPEND HASHCAT_GUI_QM_FILES
                ${CMAKE_CURRENT_BINARY_DIR}/hashcat-gui_${code}.qm)
        endforeach()

        qt_add_resources(tst_translations "tst_translations_translations"
            PREFIX "/i18n"
            BASE ${CMAKE_CURRENT_BINARY_DIR}
            FILES ${HASHCAT_GUI_QM_FILES})
    endif()

    # Headless CI: nothing the tests show, not even the main window, is drawn, and
    # the offscreen plugin ships with qt6-base everywhere. TIMEOUT 60 s fails a
    # test stuck on a modal dialog or a hashcat query that never returns, instead
    # of hanging the job (longest legitimate wait: 20 s).
    set_tests_properties(${HASHCAT_GUI_TESTS}
        PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen" TIMEOUT 60)
else()
    message(STATUS "Qt6 Test module not found - building without tests")
endif()

# The sources clang-format rewrites in place
file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h)

# The sources the analyzers run over: the .cpp files only - a header is
# analyzed as part of its translation unit.
file(GLOB ANALYSIS_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)

# `--target clang-format` reformats the sources in place with the repo's
# .clang-format; no clang-format, no target.
find_program(CLANG_FORMAT NAMES clang-format)
if(CLANG_FORMAT)
    add_custom_target(clang-format
        COMMAND ${CLANG_FORMAT} -i ${PROJECT_SOURCES}
        COMMENT "Formatting sources with clang-format")
endif()

# `--target clazy` runs clazy over the sources from the compilation database.
# The checks live in the CLAZY_CHECKS cache variable, and CLAZY_IGNORE_DIRS
# keeps the report off Qt's headers; no clazy, no target.
find_program(CLAZY_STANDALONE NAMES clazy-standalone)
if(CLAZY_STANDALONE)
    set(CLAZY_CHECKS "level0,level1" CACHE STRING
        "clazy checks to run (levels and/or check names, comma-separated)")
    add_custom_target(clazy
        COMMAND ${CMAKE_COMMAND} -E env "CLAZY_IGNORE_DIRS=.*/usr/include.*"
                ${CLAZY_STANDALONE} --checks=${CLAZY_CHECKS}
                -p ${CMAKE_BINARY_DIR}/compile_commands.json
                ${ANALYSIS_SOURCES}
        COMMENT "Running clazy over the sources")
endif()

# `--target clang-tidy` runs clang-tidy over the sources from the compilation
# database; .clang-tidy supplies the checks, so no cache variable is needed.
find_program(CLANG_TIDY NAMES clang-tidy)
if(CLANG_TIDY)
    add_custom_target(clang-tidy
        COMMAND ${CLANG_TIDY} -quiet -p ${CMAKE_BINARY_DIR} ${ANALYSIS_SOURCES}
        COMMENT "Running clang-tidy over the sources")
endif()

# The icon tree is mirrored in the source as a hicolor theme
# (src/icons/hicolor/<size>/apps/), so one install(DIRECTORY) copies every size
# in the layout the icon theme spec expects.
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/icons/hicolor
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons)

# Icon=hashcat-gui resolves by name through the hicolor tree above.
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/hashcat-gui.desktop
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)

install(TARGETS hashcat-gui RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Packaging. CPack derives the archive name/version from these variables.
set(CPACK_PACKAGE_NAME "hashcat-gui")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_HOMEPAGE_URL "${PROJECT_HOMEPAGE_URL}")
include(CPack)
