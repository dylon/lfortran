cmake_minimum_required(VERSION 3.10)

project(lfortran-lsp)

if (NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17
    CACHE STRING "C++ standard" FORCE)
endif ()

if (NOT LFORTRAN_VERSION)
  set(LFORTRAN_VERSION "0.1-git"
    CACHE STRING "LFortran version" FORCE)
endif ()

set(SRC
  specification.cpp
  lfortran_language_server.cpp
)

add_library(lfortran_lsp STATIC ${SRC})

target_include_directories(lfortran_lsp BEFORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(lfortran_lsp BEFORE PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/..)

target_link_libraries(asr)
