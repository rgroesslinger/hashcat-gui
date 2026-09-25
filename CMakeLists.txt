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

# Qt specific preprocessing
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets Concurrent REQUIRED)

add_executable(hashcat-gui WIN32
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
    src/main.cpp
    src/mainwindow.h
    src/mainwindow.cpp
    src/settingsdialog.h
    src/settingsdialog.cpp
    src/settingsmanager.h
    src/settingsmanager.cpp
    src/widgetstateserializer.h
    src/widgetstateserializer.cpp
    src/resources.qrc
    src/hashcat-gui.rc
    src/aboutdialog.ui
    src/mainwindow.ui
    src/settingsdialog.ui
)

# config.h is generated, it lives in the build tree
target_include_directories(hashcat-gui PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_compile_options(hashcat-gui PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(hashcat-gui PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Concurrent)

# Translations.
#
# LinguistTools is optional on purpose: a machine without qt6-tools still
# builds, it just gets the untranslated source strings and no .qm files.
# SOURCE_TARGETS keeps lupdate on this target - a future test target would
# otherwise contribute its own strings, including the expected command lines.
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

install(TARGETS hashcat-gui RUNTIME DESTINATION bin)
