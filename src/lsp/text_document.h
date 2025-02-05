#ifndef LCOMPILERS_LSP_TEXT_DOCUMENT_H
#define LCOMPILERS_LSP_TEXT_DOCUMENT_H

#include <cstddef>
#include <filesystem>
#include <mutex>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include <lsp/specification.h>

namespace LCompilers::LanguageServerProtocol {
  namespace fs = std::filesystem;

  const std::regex RE_FILE_URI(
    "^file:(?://)?",
    std::regex_constants::ECMAScript | std::regex_constants::icase
  );

  class TextDocument {
  public:
    TextDocument(const std::string &uri, const std::string &text);
    auto uri() -> const DocumentUri &;
    auto path() -> const fs::path &;
    auto text() -> const std::string &;
    auto setText(const std::string &text) -> void;
    auto apply(
      std::vector<TextDocumentContentChangeEvent> &changes
    ) -> void;
  private:
    DocumentUri _uri;
    fs::path _path;
    std::string _text;
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
    ) const -> void;
    auto decompose(
      const TextDocumentContentChangeEvent_0 &event,
      std::size_t &j,
      std::size_t &k,
      std::string &patch
    ) const -> void;
    auto decompose(
      const TextDocumentContentChangeEvent_1 &event,
      std::size_t &j,
      std::size_t &k,
      std::string &patch
    ) const -> void;
  };

} // namespace LCompilers::LanguageServerProtocol

#endif // LCOMPILERS_LSP_TEXT_DOCUMENT_H
