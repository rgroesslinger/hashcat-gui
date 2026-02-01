cmake_minimum_required(VERSION 3.20)
project(hashcat-gui)

# Force C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Qt specific preprocessing
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

add_executable(hashcat-gui
    src/helperutils.cpp
    src/main.cpp
    src/mainwindow.cpp
    src/aboutdialog.cpp
    src/settingsdialog.cpp
    src/settingsmanager.cpp
    src/resources.qrc
)

target_compile_options(hashcat-gui PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(hashcat-gui PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
