#ifndef LCOMPILERS_LSP_TEXT_DOCUMENT_H
#define LCOMPILERS_LSP_TEXT_DOCUMENT_H

#include <cstddef>
#include <filesystem>
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
    TextDocument(const std::string &uri);
    TextDocument(const std::string &uri, const std::string &text);
    auto load() -> void;
    auto getUri() -> const DocumentUri &;
    auto getPath() -> const fs::path &;
    auto getText() -> const std::string &;
    auto setText(const std::string &text) -> void;
    auto apply(
      ptr_vector<TextDocumentContentChangeEvent> &changes
    ) -> void;
  private:
    DocumentUri uri;
    fs::path path;
    std::string text;
    std::stringstream ss;
    std::vector<std::size_t> lineIndices;

    auto validateUriAndSetPath() -> void;
    auto indexLines() -> void;

    auto from(
      const TextDocumentContentChangeEvent &event
    ) const -> std::size_t;
    auto from(
      const PartialTextDocumentContentChangeEvent &event
    ) const -> std::size_t;
    auto from(
      const WholeTextDocumentContentChangeEvent &event
    ) const -> std::size_t;

    auto decompose(
      const TextDocumentContentChangeEvent &event,
      std::size_t &j,
      std::size_t &k,
      std::string &patch
    ) const -> void;
    auto decompose(
      const PartialTextDocumentContentChangeEvent &event,
      std::size_t &j,
      std::size_t &k,
      std::string &patch
    ) const -> void;
    auto decompose(
      const WholeTextDocumentContentChangeEvent &event,
      std::size_t &j,
      std::size_t &k,
      std::string &patch
    ) const -> void;
  };

} // namespace LCompilers::LanguageServerProtocol

#endif // LCOMPILERS_LSP_TEXT_DOCUMENT_H
