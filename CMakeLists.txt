cmake_minimum_required(VERSION 3.14)

project(consoleOgienWoda LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Znajdowanie pakietów Qt z komponentami GUI
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(consoleOgienWoda
  main.cpp
  player.h player.cpp
  level.h level.cpp
  game.h game.cpp
  fireplayer.h fireplayer.cpp
  waterplayer.h waterplayer.cpp
  mainwindow.h mainwindow.cpp mainwindow.ui # Dodaj pliki GUI
)

# Linkowanie bibliotek Qt GUI
target_link_libraries(consoleOgienWoda Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets sfml-graphics sfml-audio)

# Kopiowanie katalogu assets do katalogu wyjściowego
add_custom_command(TARGET consoleOgienWoda POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:consoleOgienWoda>/assets)

include(GNUInstallDirs)
install(TARGETS consoleOgienWoda
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
