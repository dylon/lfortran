#include <cctype>
#include <stdexcept>

#include <lsp/lsp_exception.h>
#include <lsp/lsp_json_parser.h>
#include <lsp/specification.h>

namespace LCompilers::LanguageServerProtocol {

  LspJsonParser::LspJsonParser(const std::string &message)
    : message(message)
  {
    // empty
  }

  auto LspJsonParser::parse() -> std::unique_ptr<LSPAny> {
    dropWhitespace();
    if (hasNext()) {
      switch (nextChar()) {
      case '{': {
        std::unique_ptr<LSPAny> object = std::make_unique<LSPAny>();
        (*object) = parseObject();
        dropWhitespace();
        if (hasNext()) {
          throw LSP_EXCEPTION(
            ErrorCodes::PARSE_ERROR,
            "Not a valid JSON object (trailing characters): '" + message + "'"
          );
        }
        return object;
      }
      case '[': {
        std::unique_ptr<LSPAny> array = std::make_unique<LSPAny>();
        (*array) = parseArray();
        dropWhitespace();
        if (hasNext()) {
          throw LSP_EXCEPTION(
            ErrorCodes::PARSE_ERROR,
            "Not a valid JSON array (trailing characters): '" + message + "'"
          );
        }
        return array;
      }
      default: {
        throw LSP_EXCEPTION(
          ErrorCodes::PARSE_ERROR,
          "Not a valid JSON message: '" + message + "'"
        );
      }
      }
    }
    throw LSP_EXCEPTION(
      ErrorCodes::PARSE_ERROR,
      "Cannot parse empty message."
    );
  }

  auto LspJsonParser::dropWhitespace() -> void {
    do {
      switch (peekChar()) {
      case ' ': // fallthrough
      case '\t': // fallthrough
      case '\r': // fallthrough
      case '\n': {
        advance();
        break;
      }
      default: {
        return;
      }
      }
    } while (hasNext());
  }

  auto LspJsonParser::parseObject() -> LSPObject {
    LSPObject object;
    bool hasAttribute = false;
    dropWhitespace();
    while (hasNext()) {
      unsigned char c = nextChar();
      switch (c) {
      case '"': {
        const string_t key = parseString();
        dropWhitespace();
        if (hasNext()) {
          unsigned char c = nextChar();
          if (c == ':') {
            dropWhitespace();
            std::unique_ptr<LSPAny> value = parseValue();
            object.emplace(key, std::move(value));
            hasAttribute = true;
          } else {
            ss.str("");
            ss << "Expected ':' to follow object attribute name, not: '"
               << c << "'";
            throw LSP_EXCEPTION(
              ErrorCodes::PARSE_ERROR,
              ss.str()
            );
          }
        } else {
          throw LSP_EXCEPTION(
            ErrorCodes::PARSE_ERROR,
            "Reached end-of-message while parsing object attribute."
          );
        }
        break;
      }
      case ',': {
        if (!hasAttribute) {
          throw LSP_EXCEPTION(
            ErrorCodes::PARSE_ERROR,
            "Found out-of-sequence separator (',') while parsing object."
          );
        }
        hasAttribute = false;
        dropWhitespace();
        break;
      }
      case '}': {
        return object;
      }
      default: {
        ss.str("");
        ss << "Found invalid character while parsing object: '";
        escapeAndBuffer(c);
        ss << '\'';
        throw LSP_EXCEPTION(
          ErrorCodes::PARSE_ERROR,
          ss.str()
        );
      }
      }
    }
    throw LSP_EXCEPTION(
      ErrorCodes::PARSE_ERROR,
      "Reached end-of-message while parsing object."
    );
  }

  auto LspJsonParser::parseArray() -> LSPArray {
    LSPArray array;
    bool hasValue = false;
    dropWhitespace();
    while (hasNext()) {
      unsigned char c = peekChar();
      switch (c) {
      case ',': {
        if (!hasValue) {
          throw LSP_EXCEPTION(
            ErrorCodes::PARSE_ERROR,
            "Found out-of-sequence separator (',') while parsing array"
          );
        }
        hasValue = false;
        break;
      }
      case ']': {
        advance();
        return array;
      }
      default: {
        std::unique_ptr<LSPAny> value = parseValue();
        array.push_back(std::move(value));
        hasValue = true;
      }
      }
    }
    throw LSP_EXCEPTION(
      ErrorCodes::PARSE_ERROR,
      "Reached end-of-message while parsing array."
    );
  }

  auto LspJsonParser::parseValue() -> std::unique_ptr<LSPAny> {
    dropWhitespace();
    if (hasNext()) {
      std::unique_ptr<LSPAny> value;
      switch (peekChar()) {
      case '"': {
        advance();
        value = std::make_unique<LSPAny>();
        (*value) = parseString();
        break;
      }
      case '-': // passthrough
      case '0': // passthrough
      case '1': // passthrough
      case '2': // passthrough
      case '3': // passthrough
      case '4': // passthrough
      case '5': // passthrough
      case '6': // passthrough
      case '7': // passthrough
      case '8': // passthrough
      case '9': {
        value = parseNumber();
        break;
      }
      case '{': {
        advance();
        value = std::make_unique<LSPAny>();
        (*value) = parseObject();
        break;
      }
      case '[': {
        advance();
        value = std::make_unique<LSPAny>();
        (*value) = parseArray();
        break;
      }
      case 't': {
        advance();
        value = parseTrue();
        break;
      }
      case 'f': {
        advance();
        value = parseFalse();
        break;
      }
      case 'n': {
        advance();
        value = parseNull();
        break;
      }
      }
      dropWhitespace();
      return value;
    }
    throw LSP_EXCEPTION(
      ErrorCodes::PARSE_ERROR,
      "Reached end-of-message while parsing value."
    );
  }

  auto LspJsonParser::parseString() -> string_t {
    ss.str("");
    bool escaped = false;
    while (hasNext()) {
      unsigned char c = nextChar();
      switch (c) {
      case '\\': {
        if (escaped) {
          ss << c;
        }
        escaped = !escaped;
        break;
      }
      case 'n': {
        if (!escaped) {
          ss << c;
        } else {
          ss << '\n';
          escaped = false;
        }
        break;
      }
      case '\n': {
        ss << c;
        escaped = false;
        break;
      }
      case 't': {
        if (!escaped) {
          ss << c;
        } else {
          ss << '\t';
          escaped = false;
        }
        break;
      }
      case '\t': {
        ss << c;
        escaped = false;
        break;
      }
      case 'b': {
        if (!escaped) {
          ss << c;
        } else {
          ss << '\b';
          escaped = false;
        }
        break;
      }
      case '\b': {
        ss << '\b';
        escaped = false;
        break;
      }
      case 'r': {
        if (!escaped) {
          ss << c;
        } else {
          ss << '\r';
          escaped = false;
        }
        break;
      }
      case '\r': {
        ss << c;
        escaped = false;
        break;
      }
      case 'f': {
        if (!escaped) {
          ss << c;
        } else {
          ss << '\f';
          escaped = false;
        }
        break;
      }
      case '\f': {
        ss << c;
        escaped = false;
        break;
      }
      case '\'': {
        ss << c;
        escaped = false;
        break;
      }
      case 'u': {
        if (!escaped) {
          ss << c;
        } else {
          ss << "\\u";
          std::size_t i;
          for (i = 0; (i < 4) && hasNext(); ++i) {
            unsigned char c = nextChar();
            if (std::isxdigit(static_cast<int>(c))) {
              ss << c;
            } else {
              ss.str("");
              ss << "Found non-hex digit while parsing unicode character: '";
              escapeAndBuffer(c);
              ss << '\'';
              throw LSP_EXCEPTION(
                ErrorCodes::PARSE_ERROR,
                ss.str()
              );
            }
          }
          if (i < 4) {
            throw LSP_EXCEPTION(
              ErrorCodes::PARSE_ERROR,
              "Reached end-of-message while parsing unicode literal."
            );
          }
          escaped = false;
        }
        break;
      }
      case '"': {
        if (escaped) {
          ss << c;
          escaped = false;
        } else {
          return ss.str();
        }
        break;
      }
      default: {
        ss << c;
        escaped = false;
      }
      }
    }
    throw LSP_EXCEPTION(
      ErrorCodes::PARSE_ERROR,
      "Cannot parse string without closing parenthesis."
    );
  }

  auto LspJsonParser::escapeAndBuffer(unsigned char c) -> void {
    switch (c) {
    case '\n': {
      ss << "\\n";
      break;
    }
    case '\t': {
      ss << "\\t";
      break;
    }
    case '\b': {
      ss << "\\b";
      break;
    }
    case '\r': {
      ss << "\\r";
      break;
    }
    case '\f': {
      ss << "\\f";
      break;
    }
    default: {
      ss << c;
    }
    }
  }

  auto LspJsonParser::parseNumber() -> std::unique_ptr<LSPAny> {
    bool isNegated = false;
    bool hasDigit = false;
    ss.str("");
    while (hasNext()) {
      unsigned char c = peekChar();
      switch (c) {
      case '-': {
        if (isNegated) {
          throw LSP_EXCEPTION(
            ErrorCodes::PARSE_ERROR,
            "Found double-negative while parsing number"
          );
        }
        isNegated = true;
        ss << c;
        advance();
        break;
      }
      case '0': {
        hasDigit = true;
        ss << c;
        advance();
        switch (peekChar()) {
        case '.': {
          ss << c;
          advance();
          return parseFraction();
        }
        case 'e': // fallthrough
        case 'E': {
          ss << c;
          advance();
          return parseExponent();
        }
        }
        break;
      }
      case '1': // fallthrough
      case '2': // fallthrough
      case '3': // fallthrough
      case '4': // fallthrough
      case '5': // fallthrough
      case '6': // fallthrough
      case '7': // fallthrough
      case '8': // fallthrough
      case '9': {
        hasDigit = true;
        ss << c;
        advance();
        break;
      }
      default: {
        if (hasDigit) {
          std::unique_ptr<LSPAny> number = std::make_unique<LSPAny>();
          (*number) = std::stoi(ss.str());
          return number;
        }
        throw LSP_EXCEPTION(
          ErrorCodes::PARSE_ERROR,
          "Failed to parse number: no digits."
        );
      }
      }
    }
    throw LSP_EXCEPTION(
      ErrorCodes::PARSE_ERROR,
      "Reached end-of-message while parsing number."
    );
  }

  auto LspJsonParser::parseFraction() -> std::unique_ptr<LSPAny> {
    bool hasDigit = false;
    while (hasNext()) {
      unsigned char c = peekChar();
      switch (c) {
      case '0': // fallthrough
      case '1': // fallthrough
      case '2': // fallthrough
      case '3': // fallthrough
      case '4': // fallthrough
      case '5': // fallthrough
      case '6': // fallthrough
      case '7': // fallthrough
      case '8': // fallthrough
      case '9': {
        hasDigit = true;
        ss << c;
        advance();
        break;
      }
      case 'e': // fallthrough
      case 'E': {
        ss << c;
        advance();
        return parseExponent();
      }
      default: {
        if (hasDigit) {
          std::unique_ptr<LSPAny> number = std::make_unique<LSPAny>();
          (*number) = std::stod(ss.str());
          return number;
        }
        throw LSP_EXCEPTION(
          ErrorCodes::PARSE_ERROR,
          "Failed to parse fraction: no digits."
        );
      }
      }
    }
    throw LSP_EXCEPTION(
      ErrorCodes::PARSE_ERROR,
      "Reached end-of-message while parsing decimal."
    );
  }

  auto LspJsonParser::parseExponent() -> std::unique_ptr<LSPAny> {
    bool hasDigit = false;
    bool hasSign = false;
    while (hasNext()) {
      unsigned char c = peekChar();
      switch (c) {
      case '-': // fallthrough
      case '+': {
        if (hasSign) {
          throw LSP_EXCEPTION(
            ErrorCodes::PARSE_ERROR,
            "Found multiple signs for exponent."
          );
        }
        hasSign = true;
        ss << c;
        advance();
        break;
      }
      case '0': // fallthrough
      case '1': // fallthrough
      case '2': // fallthrough
      case '3': // fallthrough
      case '4': // fallthrough
      case '5': // fallthrough
      case '6': // fallthrough
      case '7': // fallthrough
      case '8': // fallthrough
      case '9': {
        hasDigit = true;
        ss << c;
        advance();
        break;
      }
      default: {
        if (hasDigit) {
          std::unique_ptr<LSPAny> number = std::make_unique<LSPAny>();
          (*number) = std::stod(ss.str());
          return number;
        }
        throw LSP_EXCEPTION(
          ErrorCodes::PARSE_ERROR,
          "Failed to parse exponent: no digits."
        );
      }
      }
    }
    throw LSP_EXCEPTION(
      ErrorCodes::PARSE_ERROR,
      "Reached end-of-message while parsing exponent."
    );
  }

  auto LspJsonParser::parseTrue() -> std::unique_ptr<LSPAny> {
    // if (hasNext() && ('t' == nextChar())) {
      if (hasNext() && ('r' == nextChar())) {
        if (hasNext() && ('u' == nextChar())) {
          if (hasNext() && ('e' == nextChar())) {
            std::unique_ptr<LSPAny> literal = std::make_unique<LSPAny>();
            (*literal) = true;
            return literal;
          }
        }
      }
    // }
    throw LSP_EXCEPTION(
      ErrorCodes::PARSE_ERROR,
      "Failed to parse literal: true"
    );
  }

  auto LspJsonParser::parseFalse() -> std::unique_ptr<LSPAny> {
    // if (hasNext() && ('f' == nextChar())) {
      if (hasNext() && ('a' == nextChar())) {
        if (hasNext() && ('l' == nextChar())) {
          if (hasNext() && ('s' == nextChar())) {
            if (hasNext() && ('e' == nextChar())) {
              std::unique_ptr<LSPAny> literal = std::make_unique<LSPAny>();
              (*literal) = false;
              return literal;
            }
          }
        }
      }
    // }
    throw LSP_EXCEPTION(
      ErrorCodes::PARSE_ERROR,
      "Failed to parse literal: false"
    );
  }

  auto LspJsonParser::parseNull() -> std::unique_ptr<LSPAny> {
    // if (hasNext() && ('n' == nextChar())) {
      if (hasNext() && ('u' == nextChar())) {
        if (hasNext() && ('l' == nextChar())) {
          if (hasNext() && ('l' == nextChar())) {
            std::unique_ptr<LSPAny> literal = std::make_unique<LSPAny>();
            (*literal) = nullptr;
            return literal;
          }
        }
      }
    // }
    throw LSP_EXCEPTION(
      ErrorCodes::PARSE_ERROR,
      "Failed to parse literal: true"
    );
  }

} // namespace LCompilers::LanguageServerProtocol
