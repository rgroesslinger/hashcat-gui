cmake_minimum_required(VERSION 3.24)
project(hashcat-gui
    VERSION 0.8.0
    DESCRIPTION "Qt-based GUI for hashcat"
    HOMEPAGE_URL "https://github.com/rgroesslinger/hashcat-gui/"
    LANGUAGES CXX
)

# The standard install destinations the install() rules below write to.
include(GNUInstallDirs)

# Release by default. Single-config only: with a multi-config generator
# CMAKE_CONFIGURATION_TYPES drives the build. CACHE ... FORCE so the value is
# visible in CMakeCache.txt and a -D on the command line still wins.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type specified - defaulting to Release")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Feeds the analyzers and editor tooling; generators without support ignore it.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# GCC and Clang are the only supported compilers (Linux, Windows/MSYS2); the
# language guard keeps the flags away from windres.
add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-Wall;-Wextra;-Wpedantic>")

# Strict standard mode: no extensions
set(CMAKE_CXX_EXTENSIONS OFF)

# LinguistTools and Test are optional: without them, no .qm files and no tests.
find_package(Qt6 6.8 REQUIRED
    COMPONENTS Core Gui Widgets Concurrent Network
    OPTIONAL_COMPONENTS LinguistTools Test)

# Qt's project defaults (AUTOMOC, AUTOUIC, IDE folders); needs Qt found first.
qt_standard_project_setup()
# It also appends $ORIGIN and $ORIGIN/<libdir> to the install RPATH for bundles
# that ship their own libraries; this binary links only system Qt, so drop it.
set(CMAKE_INSTALL_RPATH "")
# qt_standard_project_setup() covers MOC and UIC, not RCC.
set(CMAKE_AUTORCC ON)
# The tests live in tests/ but include ui_*.h generated from src/*.ui
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src)

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
    src/updatechecker.h
    src/updatechecker.cpp
    src/widgetstateserializer.h
    src/widgetstateserializer.cpp
    src/pathmonitor.h
    src/pathmonitor.cpp
    src/resources.qrc
    src/aboutdialog.ui
    src/mainwindow.ui
    src/settingsdialog.ui
)

# Object library, not static: a static one would drop the qrc objects and
# static initializers as unreferenced.
add_library(hashcat-gui-core OBJECT ${HASHCAT_GUI_SOURCES})
target_include_directories(hashcat-gui-core PUBLIC src)
target_link_libraries(hashcat-gui-core PUBLIC
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Concurrent Qt6::Network)
target_compile_features(hashcat-gui-core PUBLIC cxx_std_20)

# No implicit byte/ASCII <-> QString conversions, so every literal states its
# encoding; PRIVATE on the production targets so the tests stay exempt.
set(HASHCAT_GUI_UTF8_DEFINITIONS QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII)
target_compile_definitions(hashcat-gui-core PRIVATE ${HASHCAT_GUI_UTF8_DEFINITIONS})

qt_add_executable(hashcat-gui WIN32
    src/main.cpp
    src/hashcat-gui.rc
)

target_link_libraries(hashcat-gui PRIVATE hashcat-gui-core)

# main.cpp is not in the object library, so it needs the same guard.
target_compile_definitions(hashcat-gui PRIVATE
    ${HASHCAT_GUI_UTF8_DEFINITIONS}
    HASHCAT_GUI_VERSION="${PROJECT_VERSION}")

# Catalogs are hashcat-gui_<code>: .ts in the source tree, .qm in the build tree
# (they do not exist at configure time, but qt_add_resources only needs paths).
set(HASHCAT_GUI_TRANSLATIONS de es fr it ja ko pl pt_BR ru sr tr uk zh_CN zh_TW)

set(HASHCAT_GUI_TS_FILES ${HASHCAT_GUI_TRANSLATIONS})
list(TRANSFORM HASHCAT_GUI_TS_FILES PREPEND translations/hashcat-gui_)
list(TRANSFORM HASHCAT_GUI_TS_FILES APPEND .ts)
set(HASHCAT_GUI_QM_FILES ${HASHCAT_GUI_TRANSLATIONS})
list(TRANSFORM HASHCAT_GUI_QM_FILES PREPEND ${CMAKE_CURRENT_BINARY_DIR}/hashcat-gui_)
list(TRANSFORM HASHCAT_GUI_QM_FILES APPEND .qm)

# The .qm files are compiled into the binary, so a package built without the
# linguist tools would ship untranslated; a packager turns this on.
option(HASHCAT_GUI_REQUIRE_TRANSLATIONS
    "Fail the configure step when the Qt6 LinguistTools are missing" OFF)

if(HASHCAT_GUI_REQUIRE_TRANSLATIONS AND NOT Qt6LinguistTools_FOUND)
    message(FATAL_ERROR
        "HASHCAT_GUI_REQUIRE_TRANSLATIONS is on, but the Qt6 LinguistTools were not found.")
endif()

if(Qt6LinguistTools_FOUND)
    # SOURCE_TARGETS keeps lupdate off the tests, so a tr() added to a test
    # cannot land in the catalogs; -no-obsolete/-locations keep them clean.
    qt_add_translations(hashcat-gui
        SOURCE_TARGETS hashcat-gui hashcat-gui-core
        LUPDATE_OPTIONS -no-obsolete -locations none
        TS_FILES ${HASHCAT_GUI_TS_FILES}
    )
else()
    message(STATUS "Qt6 LinguistTools not found - building without translations")
endif()

# The analyzers parse ui_*.h and moc output, which only the autogen targets
# produce. Building those also compiles hashcat-gui-core (a test's autogen
# depends on the object library it links), but links nothing. The lists come
# from the build's own source lists, not a glob, so every file is in the
# compilation database; main.cpp includes nothing generated, so it needs no
# autogen. Tests are added below when built.
set(ANALYSIS_SOURCES ${HASHCAT_GUI_SOURCES} src/main.cpp)
list(FILTER ANALYSIS_SOURCES INCLUDE REGEX "\\.cpp$")
list(TRANSFORM ANALYSIS_SOURCES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
set(ANALYSIS_DEPS hashcat-gui-core_autogen)

# Without Qt6::Test there are no tests, which CI turns into a failure through
# ctest --no-tests=error. -DBUILD_TESTING=OFF drops the test binaries on purpose.
option(BUILD_TESTING "Build the test executables" ON)
if(BUILD_TESTING)
    enable_testing()
endif()

# Drives the executables, their registration and the analysis inputs.
set(HASHCAT_GUI_TESTS
    tst_aboutdialog
    tst_commandbuilder
    tst_hashcatinfoparser
    tst_helperutils
    tst_mainwindow
    tst_pathmonitor
    tst_settingsdialog
    tst_statefile
    tst_translations
    tst_updatechecker
    tst_widgetstateserializer
)

if(NOT BUILD_TESTING)
    message(STATUS "BUILD_TESTING is off - building without tests")
elseif(Qt6Test_FOUND)
    foreach(test IN LISTS HASHCAT_GUI_TESTS)
        add_executable(${test} tests/${test}.cpp)
        target_link_libraries(${test} PRIVATE hashcat-gui-core Qt6::Test)
        add_test(NAME ${test} COMMAND ${test})

        # The offscreen plugin ships with qt6-base; the timeout fails a test
        # stuck on a dialog or a hashcat query instead of hanging the job
        # (longest legitimate wait: 20 s).
        set_tests_properties(${test}
            PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen" TIMEOUT 60)

        # Built tests are in the compilation database, so they can be analyzed.
        list(APPEND ANALYSIS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests/${test}.cpp)
        list(APPEND ANALYSIS_DEPS ${test}_autogen)
    endforeach()

    # The test only proves :/i18n answers if its binary embeds the catalogs like
    # the application; without LinguistTools there are none and it skips.
    if(Qt6LinguistTools_FOUND)
        target_compile_definitions(tst_translations PRIVATE HASHCAT_GUI_QM_EMBEDDED)
        qt_add_resources(tst_translations "tst_translations_translations"
            PREFIX "/i18n"
            BASE ${CMAKE_CURRENT_BINARY_DIR}
            FILES ${HASHCAT_GUI_QM_FILES})
    endif()
else()
    message(STATUS "Qt6 Test module not found - building without tests")
endif()

# `--target clang-format` reformats in place; no clang-format, no target. The
# glob is here so a build without the tool skips CONFIGURE_DEPENDS re-checks.
find_program(CLANG_FORMAT NAMES clang-format)
if(CLANG_FORMAT)
    file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h)
    add_custom_target(clang-format
        COMMAND ${CLANG_FORMAT} -i ${PROJECT_SOURCES}
        COMMENT "Formatting sources with clang-format"
        VERBATIM)
endif()

# One custom target per source file plus an aggregate `name`, so
#   cmake --build <dir> --target <name> -j N
# parallelizes like a normal build. ${ARGN} is the command, the source file is
# appended. No USES_TERMINAL: Ninja would put the command in its serial pool.
function(add_per_file_target name)
    set(per_file_targets)
    foreach(src IN LISTS ANALYSIS_SOURCES)
        file(RELATIVE_PATH rel ${CMAKE_CURRENT_SOURCE_DIR} ${src})
        string(MAKE_C_IDENTIFIER "${name}_${rel}" tgt)
        add_custom_target(${tgt}
            COMMAND ${ARGN} ${src}
            COMMENT "${name}: ${rel}"
            VERBATIM)
        add_dependencies(${tgt} ${ANALYSIS_DEPS})
        list(APPEND per_file_targets ${tgt})
    endforeach()
    add_custom_target(${name})
    add_dependencies(${name} ${per_file_targets})
endfunction()

# `--target clazy`, one job per file. --header-filter keeps Qt's own headers out
# wherever Qt is installed; no clazy, no target.
find_program(CLAZY_STANDALONE NAMES clazy-standalone)
if(CLAZY_STANDALONE)
    set(CLAZY_CHECKS "level0,level1" CACHE STRING
        "clazy checks to run (levels and/or check names, comma-separated)")
    add_per_file_target(clazy
        ${CLAZY_STANDALONE} --checks=${CLAZY_CHECKS}
        "--header-filter=${CMAKE_CURRENT_SOURCE_DIR}/(src|tests)/.*"
        -p ${CMAKE_BINARY_DIR})
endif()

# `--target clang-tidy`, one job per file; .clang-tidy supplies the checks.
find_program(CLANG_TIDY NAMES clang-tidy)
if(CLANG_TIDY)
    add_per_file_target(clang-tidy
        ${CLANG_TIDY} -quiet -p ${CMAKE_BINARY_DIR})
endif()

install(TARGETS hashcat-gui RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Freedesktop integration
if(UNIX)
    # The icon tree is mirrored in the source as a hicolor theme
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/icons/hicolor
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons)

    # Icon=hashcat-gui resolves by name through the hicolor tree above.
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/hashcat-gui.desktop
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
endif()

# Packaging. CPack takes the package name, version, description and homepage
# from project() above.
include(CPack)
