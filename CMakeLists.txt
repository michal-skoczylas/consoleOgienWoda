cmake_minimum_required(VERSION 3.14)

project(consoleOgienWoda LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(consoleOgienWoda
  main.cpp
  player.h player.cpp
  level.h level.cpp
  game.h game.cpp
  fireplayer.h fireplayer.cpp
  waterplayer.h waterplayer.cpp
)
target_link_libraries(consoleOgienWoda Qt${QT_VERSION_MAJOR}::Core sfml-graphics sfml-audio)

include(GNUInstallDirs)
install(TARGETS consoleOgienWoda
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
