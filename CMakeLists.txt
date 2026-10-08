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

# Need at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Flags for everything this file builds - the application and the tests
add_compile_options(-Wall -Wextra -Wpedantic)

# Qt specific preprocessing
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
# The tests live in tests/ but include ui_*.h generated from src/*.ui
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(Qt6 COMPONENTS Core Gui Widgets Concurrent REQUIRED)

# Everything the application is made of except main(). The tests link the very
# same list, so they exercise the real code instead of a copy of it.
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

add_executable(hashcat-gui WIN32
    src/main.cpp
    src/hashcat-gui.rc
    ${HASHCAT_GUI_SOURCES}
)

# config.h is generated, it lives in the build tree
target_include_directories(hashcat-gui PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(hashcat-gui PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Concurrent)

# Translations.
#
# LinguistTools is optional on purpose: a machine without qt6-tools still
# builds, it just gets the untranslated source strings and no .qm files.
#
# SOURCE_TARGETS keeps lupdate on this target alone: without it Qt collects
# every target in the project, the test executables included, and a tr()
# added to a test would land in the catalogs.
find_package(Qt6 COMPONENTS LinguistTools QUIET)
if(Qt6LinguistTools_FOUND)
    qt_add_translations(hashcat-gui
        SOURCE_TARGETS hashcat-gui
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
find_package(Qt6 COMPONENTS Test QUIET)
if(Qt6Test_FOUND)
    enable_testing()

    foreach(test IN ITEMS
        tst_commandbuilder
        tst_hashcatinfoparser
        tst_helperutils
        tst_mainwindow
        tst_widgetstateserializer)
        add_executable(${test} tests/${test}.cpp ${HASHCAT_GUI_SOURCES})
        target_include_directories(${test} PRIVATE src ${CMAKE_CURRENT_BINARY_DIR})
        target_link_libraries(${test} PRIVATE
            Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Concurrent Qt6::Test)
        add_test(NAME ${test} COMMAND ${test})
    endforeach()

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
        tst_widgetstateserializer
        PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen" TIMEOUT 60)
else()
    message(STATUS "Qt6 Test module not found - building without tests")
endif()

install(TARGETS hashcat-gui RUNTIME DESTINATION bin)
