# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/michal/QT_Apps/consoleOgienWoda/build/Desktop_Qt_6_6_2_GCC_64bit-Debug/_deps/json-src"
  "/home/michal/QT_Apps/consoleOgienWoda/build/Desktop_Qt_6_6_2_GCC_64bit-Debug/_deps/json-build"
  "/home/michal/QT_Apps/consoleOgienWoda/build/Desktop_Qt_6_6_2_GCC_64bit-Debug/_deps/json-subbuild/json-populate-prefix"
  "/home/michal/QT_Apps/consoleOgienWoda/build/Desktop_Qt_6_6_2_GCC_64bit-Debug/_deps/json-subbuild/json-populate-prefix/tmp"
  "/home/michal/QT_Apps/consoleOgienWoda/build/Desktop_Qt_6_6_2_GCC_64bit-Debug/_deps/json-subbuild/json-populate-prefix/src/json-populate-stamp"
  "/home/michal/QT_Apps/consoleOgienWoda/build/Desktop_Qt_6_6_2_GCC_64bit-Debug/_deps/json-subbuild/json-populate-prefix/src"
  "/home/michal/QT_Apps/consoleOgienWoda/build/Desktop_Qt_6_6_2_GCC_64bit-Debug/_deps/json-subbuild/json-populate-prefix/src/json-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/michal/QT_Apps/consoleOgienWoda/build/Desktop_Qt_6_6_2_GCC_64bit-Debug/_deps/json-subbuild/json-populate-prefix/src/json-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/michal/QT_Apps/consoleOgienWoda/build/Desktop_Qt_6_6_2_GCC_64bit-Debug/_deps/json-subbuild/json-populate-prefix/src/json-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
