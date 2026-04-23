cmake_minimum_required(VERSION 3.20)
project(MusicPlayer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Multimedia
    MultimediaWidgets
)

set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/PlayerControls.cpp
    src/PlayerControls.h
    src/PlaylistModel.cpp
    src/PlaylistModel.h
    resources/resources.qrc
)

if(WIN32)
    set(CMAKE_WIN32_EXECUTABLE TRUE)
endif()

add_executable(MusicPlayer ${SOURCES})

target_link_libraries(MusicPlayer PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::MultimediaWidgets
)

if(MSVC)
    target_compile_options(MusicPlayer PRIVATE /W4 /WX- /utf-8)
else()
    target_compile_options(MusicPlayer PRIVATE -Wall -Wextra -Wpedantic)
endif()
