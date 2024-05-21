# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/fabio/Dev/cpp/lib-mcu/raygui-widget/build-vscode/_deps/dpplib-src"
  "/home/fabio/Dev/cpp/lib-mcu/raygui-widget/build-vscode/_deps/dpplib-build"
  "/home/fabio/Dev/cpp/lib-mcu/raygui-widget/build-vscode/_deps/dpplib-subbuild/dpplib-populate-prefix"
  "/home/fabio/Dev/cpp/lib-mcu/raygui-widget/build-vscode/_deps/dpplib-subbuild/dpplib-populate-prefix/tmp"
  "/home/fabio/Dev/cpp/lib-mcu/raygui-widget/build-vscode/_deps/dpplib-subbuild/dpplib-populate-prefix/src/dpplib-populate-stamp"
  "/home/fabio/Dev/cpp/lib-mcu/raygui-widget/build-vscode/_deps/dpplib-subbuild/dpplib-populate-prefix/src"
  "/home/fabio/Dev/cpp/lib-mcu/raygui-widget/build-vscode/_deps/dpplib-subbuild/dpplib-populate-prefix/src/dpplib-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/fabio/Dev/cpp/lib-mcu/raygui-widget/build-vscode/_deps/dpplib-subbuild/dpplib-populate-prefix/src/dpplib-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/fabio/Dev/cpp/lib-mcu/raygui-widget/build-vscode/_deps/dpplib-subbuild/dpplib-populate-prefix/src/dpplib-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
