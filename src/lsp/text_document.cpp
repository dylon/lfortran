#include <algorithm>
#include <format>
#include <iostream>
#include <mutex>
#include <stdexcept>

#include <lsp/lsp_exception.h>
#include <lsp/specification.h>
#include <lsp/text_document.h>

namespace LCompilers::LanguageServerProtocol {

  TextDocument::TextDocument(
    const DocumentUri &uri,
    const std::string &text
  ) : _uri(uri)
    , _text(text)
  {
    validateUriAndSetPath();
    // std::cerr  // DEBUG
    //   << "================================================================================" << std::endl
    //   << _text
    //   << "================================================================================" << std::endl;
    indexLines();
  }

  auto TextDocument::validateUriAndSetPath() -> void {
    std::string path = std::regex_replace(_uri, RE_FILE_URI, "");
    _path = fs::canonical(path);
  }

  auto TextDocument::uri() -> const DocumentUri & {
    return _uri;
  }

  auto TextDocument::path() -> const fs::path & {
    return _path;
  }

  auto TextDocument::text() -> const std::string & {
    return _text;
  }

  auto TextDocument::setText(const std::string &text) -> void {
    std::unique_lock<std::recursive_mutex> reentrantock(reentrantMutex);
    _text = text;
    // std::cerr  // DEBUG
    //   << "================================================================================" << std::endl
    //   << _text
    //   << "================================================================================" << std::endl;
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

    {
      std::unique_lock<std::recursive_mutex> reentrantock(reentrantMutex);

      ss.str("");
      std::size_t i = 0;
      for (const auto &change : changes) {
        std::size_t j;
        std::size_t k;
        std::string patch;
        decompose(*change, j, k, patch);
        if (i < _text.length()) {
          // std::cerr
          //   << "text[" << i << ":" << j << "] "
          //   << "= text.substr(" << i << ", " << (j - i) << ") "
          //   << "= \"" << _text.substr(i, (j - i)) << "\""
          //   << std::endl;
          ss << _text.substr(i, (j - i));
        }
        // std::cerr
        //   << "text[" << j << ":" << k << "] = \"" << patch << "\""
        //   << std::endl;
        ss << patch;
        i = k;
      }
      if (i < _text.length()) {
        // std::cerr
        //   << "text[" << i << ":" << _text.length() << "] "
        //   << "= text.substr(" << i << ", " << (_text.length() - i) << ") "
        //   << "= \"" << _text.substr(i, (_text.length() - i)) << "\""
        //   << std::endl;
        ss << _text.substr(i, (_text.length() - i));
      }
      std::string text = ss.str();
      setText(text);
    }
  }

  auto TextDocument::indexLines() -> void {
    lineIndices.clear();
    lineIndices.push_back(0);
    // std::cerr << "lineIndices[0] = 0" << std::endl;
    for (std::size_t index = 0; index < _text.length(); ++index) {
      unsigned char c = _text[index];
      switch (c) {
      case '\r': {
        if (((index + 1) < _text.length()) && (_text[index + 1] == '\n')) {
          ++index;
        }
      } // fallthrough
      case '\n': {
        // std::cerr << "lineIndices[" << lineIndices.size() << "] = " << (index + 1) << std::endl;
        lineIndices.push_back(index + 1);
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

    if (start.line > end.line) {
      throw LspException(
        ErrorCodes::InvalidParams,
        std::format(
          "start.line must be <= end.line, but {} > {}",
          start.line,
          end.line
        )
      );
    }

    if ((start.line == end.line) && (start.character > end.character)) {
      throw LspException(
        ErrorCodes::InvalidParams,
        std::format(
          "start.character must be <= end.character when colinear, but {} > {}",
          start.character,
          end.character
        )
      );
    }

    if (start.line < lineIndices.size()) {
      j = lineIndices[start.line] + start.character;
    } else if (start.line == lineIndices[lineIndices.size() - 1] + 1) {
      j = _text.length();
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        std::format(
          "start.line must be <= {} but was: {}",
          lineIndices[lineIndices.size() - 1] + 1,
          start.line
        )
      );
    }

    if (end.line < lineIndices.size()) {
      k = lineIndices[end.line] + end.character;
    } else {
      k = j + event.text.length();
    }

    patch = event.text;
  }

  auto TextDocument::decompose(
    const WholeTextDocumentContentChangeEvent &event,
    std::size_t &j,
    std::size_t &k,
    std::string &patch
  ) const -> void {
    j = 0;
    k = _text.length();
    patch = event.text;
  }

} // namespace LCompilers::LanguageServerProtocol
