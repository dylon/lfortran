#pragma once

#include <cstddef>
#include <filesystem>
#include <mutex>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include <lsp/logger.h>
#include <lsp/specification.h>

namespace LCompilers::LanguageServerProtocol {
  namespace fs = std::filesystem;

  namespace lsl = LCompilers::LanguageServer::Logging;

  const std::regex RE_FILE_URI(
    "^file:(?://)?",
    std::regex_constants::ECMAScript | std::regex_constants::icase
  );

  class TextDocument {
  public:
    TextDocument(
      const std::string &uri,
      const std::string &languageId,
      int version,
      const std::string &text,
      lsl::Logger &logger
    );
    TextDocument(TextDocument &&other) noexcept;  // move constructor
    auto uri() const -> const DocumentUri &;
    auto path() const -> const fs::path &;
    auto languageId() const -> const std::string &;
    auto version() const -> int;
    auto text() const -> const std::string &;
    auto setText(const std::string &text) -> void;
    auto apply(
      std::vector<TextDocumentContentChangeEvent> &changes,
      int version
    ) -> void;
  private:
    DocumentUri _uri;
    std::string _languageId;
    int _version;
    std::string _text;
    lsl::Logger &logger;
    fs::path _path;
    std::stringstream ss;
    std::vector<std::size_t> lineIndices;
    std::recursive_mutex reentrantMutex;

    auto validateUriAndSetPath() -> void;
    auto indexLines() -> void;

    auto from(
      const TextDocumentContentChangeEvent &event
    ) const -> std::size_t;
    auto from(
      const TextDocumentContentChangeEvent_0 &event
    ) const -> std::size_t;
    auto from(
      const TextDocumentContentChangeEvent_1 &event
    ) const -> std::size_t;

    auto decompose(
      const TextDocumentContentChangeEvent &event,
      std::size_t &j,
      std::size_t &k,
      std::string &patch
    ) -> void;
    auto decompose(
      const TextDocumentContentChangeEvent_0 &event,
      std::size_t &j,
      std::size_t &k,
      std::string &patch
    ) -> void;
    auto decompose(
      const TextDocumentContentChangeEvent_1 &event,
      std::size_t &j,
      std::size_t &k,
      std::string &patch
    ) -> void;
  };

} // namespace LCompilers::LanguageServerProtocol
