cmake_minimum_required(VERSION 3.24)
project(hashcat-gui CXX)

# Set version
set(GUI_VERSION 0.7.1)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h
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
    src/aboutdialog.ui
    src/mainwindow.ui
    src/settingsdialog.ui
)

target_compile_options(hashcat-gui PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(hashcat-gui PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Concurrent)

install(TARGETS hashcat-gui RUNTIME DESTINATION bin)
