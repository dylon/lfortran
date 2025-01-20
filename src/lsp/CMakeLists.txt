cmake_minimum_required(VERSION 3.10)

project(lfortran-lsp)

# if (NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20
    CACHE STRING "C++ standard" FORCE)
# endif ()

if (NOT LFORTRAN_VERSION)
  set(LFORTRAN_VERSION "0.1-git"
    CACHE STRING "LFortran version" FORCE)
endif ()

set(LFORTRAN_LSP_SRC
  specification.cpp
  lsp_exception.cpp
  text_document.cpp
  thread_pool.cpp
  serializer.cpp
  lsp_serializer.cpp
  lsp_transformer.cpp
  lsp_language_server.cpp
  lfortran_lsp_language_server.cpp
  request_parser.cpp
  lsp_request_parser.cpp
  communication_protocol.cpp
)

add_library(lfortran_lsp STATIC ${LFORTRAN_LSP_SRC})
target_include_directories(lfortran_lsp BEFORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(lfortran_lsp BEFORE PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/..)
target_link_libraries(lfortran_lsp asr p::rapidjson)
target_compile_definitions(lfortran_lsp PRIVATE ASIO_NO_TYPEID)

set(LLANGUAGE_SERVER_SRC
  main.cpp
)

add_executable(llanguage_server ${LLANGUAGE_SERVER_SRC})
target_include_directories(llanguage_server BEFORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(llanguage_server BEFORE PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/..)
target_link_libraries(llanguage_server lfortran_lsp)
target_compile_definitions(llanguage_server PRIVATE ASIO_NO_TYPEID)
