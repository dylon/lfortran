cmake_minimum_required(VERSION 3.10)

project(lcompilers-lsp)

set(LCOMPILERS_LSP_SRC
  specification.cpp
  lsp_exception.cpp
  lsp_json_parser.cpp
  lsp_json_serializer.cpp
  logger.cpp
  text_document.cpp
  message_queue.cpp
  thread_pool.cpp
  lsp_transformer.cpp
  language_server.cpp
  lsp_language_server.cpp
  base_lsp_language_server.cpp
  request_parser.cpp
  lsp_request_parser.cpp
  communication_protocol.cpp
)

add_library(lcompilers_lsp STATIC ${LCOMPILERS_LSP_SRC})
target_include_directories(lcompilers_lsp BEFORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(lcompilers_lsp BEFORE PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/..)
target_link_libraries(lcompilers_lsp asr)
