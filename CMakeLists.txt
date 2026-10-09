cmake_minimum_required(VERSION 3.24)
project(hashcat-gui CXX)

# Set version
set(GUI_VERSION 0.7.1)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
    @ONLY
)

# Build in Release mode by default
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type specified - defaulting to Release")
    set(CMAKE_BUILD_TYPE Release)
endif()

message(STATUS "Building with CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")

# Need at least C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# The compilation database the generators support (Ninja, Makefiles) feeds
# clang-tidy and editor integrations such as clangd. Generators without
# support ignore the variable, so it is safe to set unconditionally.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Flags for everything this file builds - the application and the tests
add_compile_options(-Wall -Wextra -Wpedantic)

# Qt specific preprocessing
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
# The tests live in tests/ but include ui_*.h generated from src/*.ui
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(Qt6 COMPONENTS Core Gui Widgets Concurrent REQUIRED)

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
    src/widgetstateserializer.h
    src/widgetstateserializer.cpp
    src/resources.qrc
    src/aboutdialog.ui
    src/mainwindow.ui
    src/settingsdialog.ui
)

# clang-tidy runs as part of compilation when asked for, which applies to
# every target below - the application object library and the tests alike.
# Off by default: the analysis slows every compile down and produces noise
# on toolchains whose system headers it cannot follow (MinGW).
option(ENABLE_CLANG_TIDY "Run clang-tidy during compilation" OFF)
if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY NAMES clang-tidy REQUIRED)
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY})
    message(STATUS "clang-tidy enabled: ${CLANG_TIDY}")
endif()

# The sources are built once, into an object library the application and
# every test link against, instead of being compiled again for each target.
# Object files rather than a static library: that puts the qrc objects and
# the static initializers directly on the link line, where nothing can drop
# them as unreferenced.
add_library(hashcat-gui-core OBJECT ${HASHCAT_GUI_SOURCES})
target_include_directories(hashcat-gui-core PUBLIC src)
target_link_libraries(hashcat-gui-core PUBLIC Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Concurrent)

add_executable(hashcat-gui WIN32
    src/main.cpp
    src/hashcat-gui.rc
)

# config.h is generated, it lives in the build tree
target_include_directories(hashcat-gui PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(hashcat-gui PRIVATE hashcat-gui-core)

# Translations.
#
# LinguistTools is optional on purpose: a machine without qt6-tools still
# builds, it just gets the untranslated source strings and no .qm files.
#
# SOURCE_TARGETS keeps lupdate on the application's own sources alone:
# without it Qt collects every target in the project, the test executables
# included, and a tr() added to a test would land in the catalogs.
find_package(Qt6 COMPONENTS LinguistTools QUIET)
if(Qt6LinguistTools_FOUND)
    qt_add_translations(hashcat-gui
        SOURCE_TARGETS hashcat-gui hashcat-gui-core
        TS_FILES
            translations/hashcat-gui_de.ts
            translations/hashcat-gui_sr.ts
    )
else()
    message(STATUS "Qt6 LinguistTools not found - building without translations")
endif()

# Tests.
#
# Qt6::Test ships with qtbase, but not every installation has the development
# files for it. Without them the build still works and simply runs no tests -
# which a CI job notices, because its ctest runs with --no-tests=error.
#
# BUILD_TESTING is the standard CMake switch: -DBUILD_TESTING=OFF skips the
# test binaries entirely.
include(CTest)
find_package(Qt6 COMPONENTS Test QUIET)
if(NOT BUILD_TESTING)
    message(STATUS "BUILD_TESTING is off - building without tests")
elseif(Qt6Test_FOUND)
    foreach(test IN ITEMS
        tst_commandbuilder
        tst_hashcatinfoparser
        tst_helperutils
        tst_mainwindow
        tst_settingsdialog
        tst_translations
        tst_widgetstateserializer)
        add_executable(${test} tests/${test}.cpp)
        target_link_libraries(${test} PRIVATE hashcat-gui-core Qt6::Test)
        add_test(NAME ${test} COMMAND ${test})
    endforeach()

    # tst_translations can only prove that :/i18n answers at runtime if its
    # binary carries the catalogs the way the application carries them. The
    # files are the ones qt_add_translations() derives from the .ts list
    # above; a build without LinguistTools produces neither the files nor
    # the definition, and the test skips instead of failing.
    if(Qt6LinguistTools_FOUND)
        target_compile_definitions(tst_translations PRIVATE HASHCAT_GUI_QM_EMBEDDED)
        qt_add_resources(tst_translations "tst_translations_translations"
            PREFIX "/i18n"
            BASE ${CMAKE_CURRENT_BINARY_DIR}
            FILES
                ${CMAKE_CURRENT_BINARY_DIR}/hashcat-gui_de.qm
                ${CMAKE_CURRENT_BINARY_DIR}/hashcat-gui_sr.qm)
    endif()

    # The CI runners have no display and none is needed: nothing the tests
    # show, not even the main window they construct. The offscreen plugin
    # ships with qt6-base on every platform we build for.
    #
    # TIMEOUT caps each test at 60 s: a test that stumbles into a modal
    # dialog nobody closes (or a hashcat query that never returns) must fail
    # instead of hanging the whole CI job. The longest legitimate wait is
    # AppConstants::Hashcat::QueryTimeoutMs (20 s).
    set_tests_properties(
        tst_commandbuilder
        tst_hashcatinfoparser
        tst_helperutils
        tst_mainwindow
        tst_settingsdialog
        tst_translations
        tst_widgetstateserializer
        PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen" TIMEOUT 60)
else()
    message(STATUS "Qt6 Test module not found - building without tests")
endif()

# The source files the tooling targets. clazy analyzes headers as part of
# their translation units, so its target uses the .cpp globs only.
file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h)
file(GLOB CLAZY_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)

# `cmake --build build --target format` reformats the sources in place with
# the repository's .clang-format. A machine without clang-format simply has
# no such target.
find_program(CLANG_FORMAT NAMES clang-format)
if(CLANG_FORMAT)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i ${PROJECT_SOURCES}
        COMMENT "Formatting sources with clang-format")
endif()

# `cmake --build build --target clazy` runs clazy over the sources using the
# compilation database, in the same spirit as the format target. clazy has no
# config file of its own; the check selection lives in the CLAZY_CHECKS cache
# variable, and CLAZY_IGNORE_DIRS keeps the report on this project's files
# instead of Qt's headers. A machine without clazy simply has no such target.
find_program(CLAZY_STANDALONE NAMES clazy-standalone)
if(CLAZY_STANDALONE)
    set(CLAZY_CHECKS "level0,level1" CACHE STRING
        "clazy checks to run (levels and/or check names, comma-separated)")
    add_custom_target(clazy
        COMMAND ${CMAKE_COMMAND} -E env "CLAZY_IGNORE_DIRS=.*/usr/include.*"
                ${CLAZY_STANDALONE} --checks=${CLAZY_CHECKS}
                -p ${CMAKE_BINARY_DIR}/compile_commands.json
                ${CLAZY_SOURCES}
        COMMENT "Running clazy over the sources")
endif()

install(TARGETS hashcat-gui RUNTIME DESTINATION bin)
