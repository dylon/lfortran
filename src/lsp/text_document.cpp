#include <algorithm>
#include <format>
#include <fstream>
#include <stdexcept>

#include <lsp/specification.h>
#include <lsp/text_document.h>

namespace LCompilers::LanguageServerProtocol {

  TextDocument::TextDocument(const std::string &uri)
    : uri(uri)
  {
    // empty
  }

  TextDocument::TextDocument(
    const DocumentUri &uri,
    const std::string &text
  ) : uri(uri)
    , text(text)
  {
    validateUriAndSetPath();
  }

  auto TextDocument::load() -> void {
    validateUriAndSetPath();

    std::ifstream file(path);
    if (!file.is_open()) {
      throw std::runtime_error(
        std::format("Cannot open file for reading: {}", path.string())
      );
    }

    ss.str("");
    ss << file.rdbuf();
    file.close();
    std::string text = ss.str();
    setText(text);
  }

  auto TextDocument::validateUriAndSetPath() -> void {
    std::string path = std::regex_replace(uri, RE_FILE_URI, "");
    this->path = fs::canonical(path);
  }

  auto TextDocument::getUri() -> const DocumentUri & {
    return uri;
  }

  auto TextDocument::getPath() -> const fs::path & {
    return path;
  }

  auto TextDocument::getText() -> const std::string & {
    return text;
  }

  auto TextDocument::setText(const std::string &text) -> void {
    this->text = text;
    indexLines();
  }

  auto TextDocument::apply(
    ptr_vector<TextDocumentContentChangeEvent> &changes
  ) -> void {
    std::sort(
      changes.begin(),
      changes.end(),
      [this](auto &a, auto &b) {
        return from(*a) < from(*b);
      }
    );

    ss.str("");
    std::size_t i = 0;
    for (const auto &change : changes) {
      std::size_t j;
      std::size_t k;
      std::string patch;
      decompose(*change, j, k, patch);
      ss << text.substr(i, j);
      ss << patch;
      i = k;
    }
    ss << text.substr(i, text.length());
    std::string text = ss.str();
    setText(text);
  }

  auto TextDocument::indexLines() -> void {
    lineIndices.clear();
    lineIndices.push_back(0);
    for (std::size_t index = 0; index < text.length(); ++index) {
      unsigned char c = text[index];
      switch (c) {
      case '\r': {
        if (((index + 1) < text.length()) && (text[index + 1] == '\n')) {
          ++index;
        }
      } // fallthrough
      case '\n': {
        lineIndices.push_back(index);
      }
      }
    }
  }

  auto TextDocument::from(
    const TextDocumentContentChangeEvent &event
  ) const -> std::size_t {
    switch (event.type) {
    case TextDocumentContentChangeEventType::PARTIAL_TEXT_DOCUMENT: {
      return from(
        *std::get<std::unique_ptr<PartialTextDocumentContentChangeEvent>>(
          event.value
        )
      );
    }
    case TextDocumentContentChangeEventType::WHOLE_TEXT_DOCUMENT: {
      return from(
        *std::get<std::unique_ptr<WholeTextDocumentContentChangeEvent>>(
          event.value
        )
      );
    }
    }
    throw std::runtime_error("This should be unreachable.");
  }

  auto TextDocument::from(
    const PartialTextDocumentContentChangeEvent &event
  ) const -> std::size_t {
    const Range &range = *event.range;
    const Position &start = *range.start;
    std::size_t index = lineIndices[start.line] + start.character;
    return index;
  }

  auto TextDocument::from(
    const WholeTextDocumentContentChangeEvent &event
  ) const -> std::size_t {
    return 0;
  }

  auto TextDocument::decompose(
    const TextDocumentContentChangeEvent &event,
    std::size_t &j,
    std::size_t &k,
    std::string &patch
  ) const -> void {
    switch (event.type) {
    case TextDocumentContentChangeEventType::PARTIAL_TEXT_DOCUMENT: {
      const PartialTextDocumentContentChangeEvent &partial =
        *std::get<std::unique_ptr<PartialTextDocumentContentChangeEvent>>(
          event.value
        );
      decompose(partial, j, k, patch);
      break;
    }
    case TextDocumentContentChangeEventType::WHOLE_TEXT_DOCUMENT: {
      const WholeTextDocumentContentChangeEvent &whole =
        *std::get<std::unique_ptr<WholeTextDocumentContentChangeEvent>>(
          event.value
        );
      decompose(whole, j, k, patch);
      break;
    }
    }
  }

  auto TextDocument::decompose(
    const PartialTextDocumentContentChangeEvent &event,
    std::size_t &j,
    std::size_t &k,
    std::string &patch
  ) const -> void {
    const Range &range = *event.range;
    const Position &start = *range.start;
    const Position &end = *range.end;
    j = lineIndices[start.line] + start.character;
    k = lineIndices[end.line] + start.character;
    patch = event.text;
  }

  auto TextDocument::decompose(
    const WholeTextDocumentContentChangeEvent &event,
    std::size_t &j,
    std::size_t &k,
    std::string &patch
  ) const -> void {
    j = 0;
    k = text.length();
    patch = event.text;
  }

} // namespace LCompilers::LanguageServerProtocol
