#include <cctype>
#include <sstream>
#include <utility>

#include <libasr/exception.h>
#include <lsp/lexer.h>

namespace LCompilers::LanguageServiceProvider::AST {

  Token::Token(const std::size_t line,
               const std::size_t column,
               const std::size_t position,
               const std::string &text,
               const TokenType &type)
    : line(line)
    , column(column)
    , position(position)
    , text(std::move(text))
    , type(type) {
  }

  Token::Token()
    : line(0)
    , column(0)
    , position(0)
    , text("")
    , type(TokenType::END_OF_FILE) {
  }

  static inline auto isid(const unsigned char c) -> bool {
    return std::isalnum(c) || (c == '_');
  }

  static inline auto isquote(const unsigned char c) -> bool {
    return (c == '"') || (c == '\'');
  }

  static auto isbinary(const int c) -> int {
    return (c == '0') || (c == '1');
  }

  static auto isoctal(const int c) -> int {
    return ('0' <= c) && (c <= '7');
  }

  static inline auto isexp(int c) -> int {
    return (c == 'e') || (c == 'E') || (c == 'd') || (c == 'D');
  }

  static inline auto issgn(int c) -> int {
    return (c == '+') || (c == '-');
  }

  Lexer::Lexer(const std::string &text,
               std::size_t line,
               std::size_t column,
               std::size_t length)
    : text(text)
    , line(line)
    , column(column)
    , position(0)
    , length(length) {
    updatePosition();
    initialize();
  }

  Lexer::Lexer(const std::string &text)
    : text(text)
    , line(0)
    , column(0)
    , position(0)
    , length(text.length()) {
    initialize();
  }

  auto Lexer::sanityCheck() -> void {
    if (length > (text.length() - position)) {
      throw LCompilers::LCompilersException("length exceeds available text");
    }
  }

  auto Lexer::initialize() -> void {
    sanityCheck();
  }

  auto Lexer::updatePosition() -> void {
    for (std::size_t position = 0, line = 0, column = 0;
         (position < text.length())
           && ((line < this->line)
               || ((line == this->line) && (column <= this->column)));
         position++) {
      if ((line == this->line) && (column == this->column)) {
        this->position = position;
        return;
      }
      if (text[position] == '\n') {
        line++;
        column = 0;
      } else if (text[position] == '\r') {
        if (position + 1 < text.length() && text[position + 1] == '\n') {
          position++;
        }
        line++;
        column = 0;
      } else {
        column++;
      }
    }
    throw LCompilers::LCompilersException("Invalid line or column");
  }

  auto Lexer::nextToken() -> Token {
    if (pending.size() > 0) {
      Token token = pending.front();
      pending.pop();
      return token;
    }

    if (length == 0) {
      return {line, column, position, "", TokenType::END_OF_FILE};
    }

    const unsigned char c = currChar();
    switch (c) {
    case '!':
      parseComment();
      break;
    case '\n':  // fallthrough
    case '\r':
      parseNewline();
      break;
    case '\'':
      parseString(c, TokenType::SINGLE_QUOTE);
      break;
    case '"':
      parseString(c, TokenType::DOUBLE_QUOTE);
      break;
    case ';':
      parse1(c, TokenType::STMT_SEP);
      break;
    case '=':
      parse3('=', '>', TokenType::ASSIGN, TokenType::EQ_CMP, TokenType::MAPS_TO);
      break;
    case '+':
      parse1(c, TokenType::PLUS);
      break;
    case '-':
      parse1(c, TokenType::MINUS);
      break;
    case '*':
      parse2('*', TokenType::STAR, TokenType::POWER);
      break;
    case '/':
      parse4('=', '/', ')',
             TokenType::SLASH,
             TokenType::NE_CMP,
             TokenType::CAT_OP,
             TokenType::END_ARRAY);
      break;
    case '(':
      parse2('/', TokenType::LPAR, TokenType::BEGIN_ARRAY);
      break;
    case ')':
      parse1(c, TokenType::RPAR);
      break;
    case ',':
      parse1(c, TokenType::COMMA);
      break;
    case '.':
      parseDot();
      break;
    case ':':
      parse2(':', TokenType::COLON, TokenType::DEFINE);
      break;
    case '%':
      parse1(c, TokenType::ACCESSOR);
      break;
    case '&':
      dropLineContinuation();
      break;
    case '<':
      parse2('=', TokenType::LT_CMP, TokenType::LE_CMP);
      break;
    case '>':
      parse2('=', TokenType::GT_CMP, TokenType::GE_CMP);
      break;
    case '?':
      parse1(c, TokenType::QUESTION);
      break;
    case '$':
      parse1(c, TokenType::DOLLAR);
      break;
    case ' ':  // fallthrough
    case '\t':
      dropBlanks();
      break;
    default:
      if (((c == 'b') || (c == 'B'))
          && hasMoreThan1()
          && isquote(text[position + 1])) {
        parseQuotedInteger(TokenType::BINARY_CONSTANT, isbinary);
      } else if (((c == 'o') || (c == 'O'))
                 && hasMoreThan1()
                 && isquote(text[position + 1])) {
        parseQuotedInteger(TokenType::OCTAL_CONSTANT, isoctal);
      } else if (((c == 'z') || (c == 'Z'))
                 && hasMoreThan1()
                 && isquote(text[position + 1])) {
        parseQuotedInteger(TokenType::HEX_CONSTANT, ::isxdigit);
      } else if (std::isdigit(c)) {
        parseInteger();
      } else if (std::isalpha(c)) {
        parseName();
      } else {
        throw LCompilers::LCompilersException(
          "Unsupported character in current position.");
      }
    }

    return nextToken();
  }

  auto Lexer::parseComment() -> void {
    const std::size_t line = this->line;
    const std::size_t column = this->column;
    const std::size_t position = this->position;

    std::stringstream ss;
    for (; notEmpty(); advance()) {
      const unsigned char c = text[this->position];
      if (c == '\n' || c == '\r') {
        break;
      }
      ss << c;
    }

    std::string text;
    ss >> text;

    pending.emplace(line, column, position, text, TokenType::COMMENT);
  }

  auto Lexer::parseNewline() -> void {
    const std::size_t line = this->line;
    const std::size_t column = this->column;
    const std::size_t position = this->position;
    std::string text;

    const unsigned char c = currChar();
    if (c == '\n') {
      text = "\n";
      this->position++;
      this->length--;
    } else if (c == '\r') {
      if (hasMoreThan1() && (nextChar() == '\n')) {
        text = "\r\n";
        this->position += 2;
        this->length -= 2;
      } else {
        text = "\r";
        this->position++;
        this->length--;
      }
    } else {
      throw LCompilers::LCompilersException("Expected newline");
    }

    this->line++;
    this->column = 0;

    pending.emplace(line, column, position, text, TokenType::NEWLINE);
  }

  static inline auto isNewline(const unsigned char c) -> bool {
    return (c == '\n') || (c == '\r');
  }

  auto Lexer::parseString(const unsigned char quote,
                          TokenType type,
                          std::size_t line,
                          std::size_t column,
                          std::size_t position,
                          std::stringstream &ss) -> void {
    std::string text(1, quote);
    pending.emplace(line, column, position, text, type);
    advance();

    if (isEmpty()) {
      throw LCompilers::LCompilersException(
        "Reached EOF while parsing string.");
    }

    line = this->line;
    column = this->column;
    position = this->position;

    pending.emplace();
    Token &token = pending.back();
    token.line = line;
    token.column = column;
    token.position = position;
    token.type = TokenType::REP_CHAR_SEQ;

    ss.str("");
    for (; notEmpty(); advance()) {
      const unsigned char c = currChar();
      if (c == quote) {
        if (hasMoreThan1() && (nextChar() == quote)) {
          ss << quote;
          advance(2);
        } else {
          line = this->line;
          column = this->column;
          position = this->position;
          text = std::string(1, c);
          pending.emplace(line, column, position, text, type);

          ss >> text;
          token.text = text;
          advance();
          break;
        }
      } else if ((c == '&') && nextNonBlankIsNewline()) {
        dropLineContinuation();
      } else {
        ss << c;
      }
    }
  }

  auto Lexer::parseString(const unsigned char quote,
                          const std::size_t line,
                          const std::size_t column,
                          const std::size_t position,
                          std::stringstream &ss) -> void {
    if (quote == '"') {
      parseString(quote, TokenType::DOUBLE_QUOTE, line, column, position, ss);
    } else if (quote == '\'') {
      parseString(quote, TokenType::SINGLE_QUOTE, line, column, position, ss);
    } else {
      throw LCompilers::LCompilersException(
        "quote parameter must be either a double- or single-quote.");
    }
  }

  auto Lexer::parseString(const unsigned char quote,
                          TokenType type) -> void {
    const std::size_t line = this->line;
    const std::size_t column = this->column;
    const std::size_t position = this->position;

    std::stringstream ss;
    parseString(quote, type, line, column, position, ss);
  }

  auto Lexer::nextNonBlankIsNewline() -> bool {
    for (std::size_t i = position + 1; i < text.length(); advance()) {
      const unsigned char c = text[i];
      if (!std::isblank(c)) {
        return (c == '\n') || (c == '\r');
      }
    }
    return false;
  }

  auto Lexer::dropBlanks() -> void {
    for (; notEmpty(); advance()) {
      const unsigned char c = text[position];
      if (!std::isblank(c)) {
        break;
      }
    }
  }

  auto Lexer::parse1(const unsigned char c, TokenType type) -> void {
    std::string text(1, c);
    pending.emplace(line, column, position, text, type);
    advance();
  }

  auto Lexer::parse2(const unsigned char suffix,
                     TokenType candidate,
                     TokenType alternate) -> void {
    const std::size_t line = this->line;
    const std::size_t column = this->column;
    const std::size_t position = this->position;
    std::string text;
    TokenType type;
    if (hasMoreThan1() && (nextChar() == suffix)) {
      text = this->text.substr(position, 2);
      type = alternate;
      advance(2);
    } else {
      text = this->text.substr(position, 1);
      type = candidate;
      advance();
    }
    pending.emplace(line, column, position, text, type);
  }

  auto Lexer::parse3(const unsigned char s1,
                     const unsigned char s2,
                     TokenType t1,
                     TokenType t2,
                     TokenType t3) -> void {
    const std::size_t line = this->line;
    const std::size_t position = this->position;
    const std::size_t column = this->column;

    TokenType type = t1;
    if (hasMoreThan1()) {
      const unsigned char d = nextChar();
      if (d == s1) {
        type = t2;
      } else if (d == s2) {
        type = t3;
      }
    }

    std::string text;
    if (type != t1) {
      text = this->text.substr(this->position, 2);
      advance(2);
    } else {
      text = this->text.substr(this->position, 1);
      advance();
    }

    pending.emplace(line, column, position, text, type);
  }

  auto Lexer::parse4(const unsigned char s1,
                     const unsigned char s2,
                     const unsigned char s3,
                     TokenType t1,
                     TokenType t2,
                     TokenType t3,
                     TokenType t4) -> void {
    const std::size_t line = this->line;
    const std::size_t position = this->position;
    const std::size_t column = this->column;

    TokenType type = t1;
    if (hasMoreThan1()) {
      const unsigned char d = nextChar();
      if (d == s1) {
        type = t2;
      } else if (d == s2) {
        type = t3;
      } else if (d == s3) {
        type = t4;
      }
    }

    std::string text;
    if (type != t1) {
      text = this->text.substr(this->position, 2);
      advance(2);
    } else {
      text = this->text.substr(this->position, 1);
      advance();
    }

    pending.emplace(line, column, position, text, type);
  }

  auto Lexer::parseDot() -> void {
    const std::size_t line = this->line;
    const std::size_t column = this->column;
    const std::size_t position = this->position;

    std::stringstream ss;
    ss << '.';
    advance();

    if (notEmpty() && std::isdigit(currChar())) {
      pending.emplace(line, column, position, ".", TokenType::DECIMAL_POINT);
      ss.str("");
      parseReal(ss);
      return;
    }

    unsigned char c;
    TokenType type;
    std::string text;

    c = currChar("Reached EOF before end of operation.");
    advance();
    switch (std::toupper(c)) {
    case 'a':  // fallthrough
    case 'A': {
      ss << c;
      c = currChar("Reached EOF before end of operation.");
      if (std::toupper(c) == 'N') {
        ss << c;
        c = currChar("Reached EOF before end of operation.");
        if (std::toupper(c) == 'D') {
          ss << c;
          c = currChar("Reached EOF before end of operation.");
          if (c == '.') {  //-> .AND.
            ss << c;
            type = TokenType::LOG_AND;
          } else {  //-> .CUSTOM.
            parseCustomOp(line, column, position, ss);
            return;
          }
        } else {  //-> .CUSTOM.
          parseCustomOp(line, column, position, ss);
          return;
        }
      } else {  //-> .CUSTOM.
          parseCustomOp(line, column, position, ss);
          return;
        }
      break;
    }
    case 'e':  // fallthrough
    case 'E': {
      ss << c;
      c = currChar("Reached EOF before end of operation.");
      if (std::toupper(c) == 'Q') {
        ss << c;
        c = currChar("Reached EOF before end of operation.");
        switch (std::toupper(c)) {
        case '.': {  //-> .EQ.
          ss << c;
          type = TokenType::EQ_CMP;
          break;
        }
        case 'V': {
          ss << c;
          c = currChar("Reached EOF before end of operation.");
          if (std::toupper(c) == '.') {  //-> .EQV.
            ss << c;
            type = TokenType::LOG_EQV;
          } else {  //-> .CUSTOM.
            parseCustomOp(line, column, position, ss);
            return;
          }
          break;
        }
        default: {  //-> .CUSTOM.
          parseCustomOp(line, column, position, ss);
          return;
        }
        }
      } else {  //-> .CUSTOM.
        parseCustomOp(line, column, position, ss);
        return;
      }
      break;
    }
    case 'f':  // fallthrough
    case 'F': {
      ss << c;
      c = currChar("Reached EOF before end of operation.");
      if (std::toupper(c) == 'A') {
        ss << c;
        c = currChar("Reached EOF before end of operation.");
        if (std::toupper(c) == 'L') {
          ss << c;
          c = currChar("Reached EOF before end of operation.");
          if (std::toupper(c) == 'S') {
            ss << c;
            c = currChar("Reached EOF before end of operation.");
            if (std::toupper(c) == 'E') {
              ss << c;
              c = currChar("Reached EOF before end of operation.");
              if (c == '.') {  //-> .FALSE.
                ss << c;
                ss >> text;
                type = TokenType::LOGICAL;
                pending.emplace(line, column, position, text, type);
                parseTrailingKind(ss);
                return;
              } else {  //-> .CUSTOM.
                parseCustomOp(line, column, position, ss);
                return;
              }
            } else {  //-> .CUSTOM.
              parseCustomOp(line, column, position, ss);
              return;
            }
          } else {  //-> .CUSTOM.
            parseCustomOp(line, column, position, ss);
            return;
          }
        } else {  //-> .CUSTOM.
          parseCustomOp(line, column, position, ss);
          return;
        }
      } else {  //-> .CUSTOM.
        parseCustomOp(line, column, position, ss);
        return;
      }
      break;
    }
    case 'g':  // fallthrough
    case 'G': {
      ss << c;
      c = currChar("Reached EOF before end of operation.");
      switch (std::toupper(c)) {
      case 'E': {
        ss << c;
        c = currChar("Reached EOF before end of operation.");
        if (c == '.') {  //-> .GE.
          ss << c;
          type = TokenType::GE_CMP;
        } else {  //-> .CUSTOM.
          parseCustomOp(line, column, position, ss);
          return;
        }
        break;
      }
      case 'T': {
        ss << c;
        c = currChar("Reached EOF before end of operation.");
        if (c == '.') {  //-> .GT.
          ss << c;
          type = TokenType::GT_CMP;
        } else {  //-> .CUSTOM.
          parseCustomOp(line, column, position, ss);
          return;
        }
        break;
      }
      default: {  //-> .CUSTOM.
        parseCustomOp(line, column, position, ss);
        return;
      }
      }
      break;
    }
    case 'l':  // fallthrough
    case 'L': {
      ss << c;
      c = currChar("Reached EOF before end of operation.");
      switch (std::toupper(c)) {
      case 'E': {
        ss << c;
        c = currChar("Reached EOF before end of operation.");
        if (c == '.') {  //-> .LE.
          ss << c;
          type = TokenType::LE_CMP;
        } else {  //-> .CUSTOM.
          parseCustomOp(line, column, position, ss);
          return;
        }
        break;
      }
      case 'T': {
        ss << c;
        c = currChar("Reached EOF before end of operation.");
        if (c == '.') {  //-> .GT.
          ss << c;
          type = TokenType::GT_CMP;
        } else {  //-> .CUSTOM.
          parseCustomOp(line, column, position, ss);
          return;
        }
        break;
      }
      default: {  //-> .CUSTOM.
        parseCustomOp(line, column, position, ss);
        return;
      }
      }
      break;
    }
    case 'n':  // fallthrough
    case 'N': {
      ss << c;
      c = currChar("Reached EOF before end of operation.");
      switch (std::toupper(c)) {
      case 'E': {
        ss << c;
        c = currChar("Reached EOF before end of operation.");
        switch (std::toupper(c)) {
        case '.': {  //-> .NE.
          ss << c;
          type = TokenType::NE_CMP;
          break;
        }
        case 'Q': {
          ss << c;
          c = currChar("Reached EOF before end of operation.");
          if (std::toupper(c) == 'V') {
            ss << c;
            c = currChar("Reached EOF before end of operation.");
            if (c == '.') {  //-> .NEQV.
              ss << c;
              type = TokenType::LOG_NEQV;
            } else {  //-> .CUSTOM.
              parseCustomOp(line, column, position, ss);
              return;
            }
          } else {  //-> .CUSTOM.
            parseCustomOp(line, column, position, ss);
            return;
          }
          break;
        }
        default: {  //-> .CUSTOM.
          parseCustomOp(line, column, position, ss);
          return;
        }
        }
        break;
      }
      case 'O': {
        ss << c;
        c = currChar("Reached EOF before end of operation.");
        if (std::toupper(c) == 'T') {
          ss << c;
          c = currChar("Reached EOF before end of operation.");
          if (c == '.') {  //-> .NOT.
            ss << c;
            type = TokenType::LOG_NOT;
          } else {  //-> .CUSTOM.
            parseCustomOp(line, column, position, ss);
            return;
          }
        } else {  //-> .CUSTOM.
          parseCustomOp(line, column, position, ss);
          return;
        }
        break;
      }
      default: {  //-> .CUSTOM.
        parseCustomOp(line, column, position, ss);
        return;
      }
      }
      break;
    }
    case 'o':  // fallthrough
    case 'O': {
      ss << c;
      c = currChar("Reached EOF before end of operation.");
      if (std::toupper(c) == 'R') {
        ss << c;
        c = currChar("Reached EOF before end of operation.");
        if (c == '.') {  //-> .OR.
          ss << c;
          type = TokenType::LOG_OR;
        } else {  //-> .CUSTOM.
          parseCustomOp(line, column, position, ss);
          return;
        }
      } else {  //-> .CUSTOM.
        parseCustomOp(line, column, position, ss);
        return;
      }
      break;
    }
    case 't':  // fallthrough
    case 'T': {
      ss << c;
      c = currChar("Reached EOF before end of operation.");
      if (std::toupper(c) == 'R') {
        ss << c;
        c = currChar("Reached EOF before end of operation.");
        if (std::toupper(c) == 'U') {
          ss << c;
          c = currChar("Reached EOF before end of operation.");
          if (std::toupper(c) == 'E') {
            ss << c;
            c = currChar("Reached EOF before end of operation.");
            if (c == '.') {  //-> .TRUE.
              ss << c;
              ss >> text;
              type = TokenType::LOGICAL;
              pending.emplace(line, column, position, text, type);
              parseTrailingKind(ss);
              return;
            } else {  //-> .CUSTOM.
              parseCustomOp(line, column, position, ss);
              return;
            }
          } else {  //-> .CUSTOM.
            parseCustomOp(line, column, position, ss);
            return;
          }
        } else {  //-> .CUSTOM.
          parseCustomOp(line, column, position, ss);
          return;
        }
      } else {  //-> .CUSTOM.
        parseCustomOp(line, column, position, ss);
        return;
      }
      break;
    }
    default: {  //-> .CUSTOM.
      parseCustomOp(line, column, position, ss);
      return;
    }
    }

    ss >> text;

    pending.emplace(line, column, position, text, type);
  }

  auto Lexer::parseCustomOp(const std::size_t line,
                            const std::size_t column,
                            const std::size_t position,
                            std::stringstream &ss) -> void {
    for (; notEmpty(); advance()) {
      const unsigned char c = currChar();
      if (c == '.') {
        ss << c;
        break;
      } else if (std::isalpha(c)) {
        ss << c;
      } else {
        throw LCompilers::LCompilersException(
          "Expected custom operator name to end with a period.");
      }
    }

    std::string text;
    ss >> text;

    pending.emplace(line, column, position, text, TokenType::CUSTOM_OP);
  }

  auto Lexer::dropLineContinuation() -> void {
    if (text[position] != '&') {
      throw LCompilers::LCompilersException(
        "Expected ampersand at beginning of line continuation.");
    }

    advance();

    dropBlanks();
    while (isNewline(text[position])) {
      parseNewline();
      dropBlanks();
    }

    while (notEmpty() && (text[position] == '!')) {
      parseComment();
      parseNewline();
      dropBlanks();
    }

    if (notEmpty() && (text[position] == '&')) {
      // drop the leading ampersand
      advance();
    }
  }

  auto Lexer::parseInteger() -> void {
    std::size_t line = this->line;
    std::size_t column = this->column;
    std::size_t position = this->position;

    std::stringstream ss;
    unsigned char c;

    std::string text;
    unsigned int length = 0;
    for (; notEmpty(); advance()) {
      c = currChar();
      if ((c == '.') || isexp(c)) {
        if (length > 0) {
          ss >> text;
          pending.emplace(line, column, position, text, TokenType::DIGIT_STRING);
        }
        line = this->line;
        column = this->column;
        position = this->position;
        pending.emplace(line, column, position, ".", TokenType::DECIMAL_POINT);
        advance();
        ss.str("");
        parseReal(ss);
        return;
      } else if (!std::isdigit(c)) {
        break;
      }
      ss << c;
      length++;
    }

    ss >> text;
    pending.emplace(line, column, position, text, TokenType::DIGIT_STRING);

    parseTrailingKind(ss);
  }

  auto Lexer::parseQuotedInteger(TokenType type, std::function<int(int)> isnumber) -> void {
    const std::size_t line = this->line;
    const std::size_t column = this->column;
    const std::size_t position = this->position;

    std::stringstream ss;
    ss << text[this->position];

    const unsigned char quote = text[this->position + 1];
    ss << quote;

    advance(2);

    for (; notEmpty(); advance()) {
      const unsigned char c = text[this->position];
      if (c == quote) {
        ss << c;
        break;
      } else if (isnumber(c)) {
        ss << c;
      } else {
        throw LCompilers::LCompilersException(
          "Expected quoted integer to end with a quotation mark.");
      }
    }

    std::string text;
    ss >> text;

    pending.emplace(line, column, position, text, type);
  }

  auto Lexer::parseReal(std::stringstream &ss) -> void {
    std::size_t line = this->line;
    std::size_t column = this->column;
    std::size_t position = this->position;

    unsigned char c;

    unsigned int length = 0;
    for (; notEmpty(); advance()) {
      c = currChar();
      if (!std::isdigit(c)) {
        break;
      }
      ss << c;
      length++;
    }

    std::string text;

    if (length > 0) {
      ss >> text;
      pending.emplace(line, column, position, text, TokenType::DIGIT_STRING);
    }

    if (notEmpty()) {
      c = currChar();
      if (isexp(c)) {
        line = this->line;
        column = this->column;
        position = this->position;
        text = std::string(1, c);
        pending.emplace(line, column, position, text, TokenType::EXPONENT_LETTER);
        advance();

        if (notEmpty()) {
          line = this->line;
          column = this->column;
          position = this->position;

          c = currChar();
          if (c == '+') {
            text = std::string(1, c);
            pending.emplace(line, column, position, text, TokenType::PLUS);
            advance();
          } else if (c == '-') {
            text = std::string(1, c);
            pending.emplace(line, column, position, text, TokenType::MINUS);
            advance();
          }

          line = this->line;
          column = this->column;
          position = this->position;

          ss.str("");
          length = 0;
          for (; notEmpty(); advance()) {
            c = currChar();
            if (!std::isdigit(c)) {
              break;
            }
            ss << c;
            length++;
          }

          if (length > 0) {
            ss >> text;
            pending.emplace(line, column, position, text, TokenType::DIGIT_STRING);
          } else {
            throw LCompilers::LCompilersException(
              "Reached EOF while parsing exponent.");
          }
        } else {
          throw LCompilers::LCompilersException(
            "Reached EOF while parsing exponent.");
        }
      }
    }

    parseTrailingKind(ss);
  }

  auto Lexer::parseName(std::stringstream &ss) -> void {
    if (notEmpty()) {
      unsigned char c = currChar();
      if (!std::isalpha(c)) {
        throw LCompilers::LCompilersException(
          "A name must begin with a letter");
      }
      ss << c;
      advance();

      unsigned char b = c;
      for (; notEmpty(); advance()) {
        c = currChar();
        if (isquote(c) && (b == '_')) {
          parseString(c, line, column, position, ss);
          return;
        } else if (!isid(c)) {
          break;
        }
        ss << c;
        b = c;
      }
    } else {
      throw LCompilers::LCompilersException(
        "Reached EOF while parsing a name.");
    }
  }

  auto Lexer::parseName() -> void {
    const std::size_t line = this->line;
    const std::size_t column = this->column;
    const std::size_t position = this->position;

    std::stringstream ss;
    parseName(ss);

    std::string text;
    ss >> text;

    if (text.length() == 0) {
      throw LCompilers::LCompilersException("Failed to parse name.");
    }

    pending.emplace(line, column, position, text, TokenType::NAME);
  }

  auto Lexer::advance() -> void {
    advance(1);
  }

  auto Lexer::advance(std::size_t amount) -> void {
    position += amount;
    column += amount;
    length -= amount;
  }

  auto Lexer::assertNotEmpty(const std::string &message) const -> void {
    if (isEmpty()) {
      throw LCompilers::LCompilersException(message);
    }
  }

  auto Lexer::currChar(const std::string &message) -> unsigned char {
    if (isEmpty()) {
      throw LCompilers::LCompilersException(message);
    }
    const unsigned char c = currChar();
    advance();
    return c;
  }

  auto Lexer::parseTrailingKind(std::stringstream &ss) -> void {
    if (notEmpty()) {
      unsigned char c = currChar();
      if (c == '_') {
        std::size_t line = this->line;
        std::size_t column = this->column;
        std::size_t position = this->position;
        pending.emplace(line, column, position, "_", TokenType::UNDERSCORE);
        advance();
        if (notEmpty()) {
          line = this->line;
          column = this->column;
          position = this->position;

          c = currChar();
          if (isquote(c)) {
            parseString(c, line, column, position, ss);
            return;
          }

          ss.str("");
          TokenType type;
          if (std::isalpha(c)) {
            type = TokenType::SCALAR_INT_CONSTANT_NAME;
            parseName(ss);
          } else if (std::isdigit(c)) {
            type = TokenType::DIGIT_STRING;
            ss << c;
            advance();
            for (; notEmpty(); advance()) {
              if (!std::isdigit(c)) {
                break;
              }
              ss << c;
            }
          } else {
            throw LCompilers::LCompilersException(
              "Expected type identifier to be a string of characters or digits.");
          }

          std::string text;
          ss >> text;

          pending.emplace(line, column, position, text, type);
        } else {
          throw LCompilers::LCompilersException(
            "Reached EOF while parsing an integer.");
        }
      }
    }
  }

} // namespace LCompilers::LanguageServiceProvider::AST
