#ifndef LCOMPILERS_LSP_AST_LEXER_H
#define LCOMPILERS_LSP_AST_LEXER_H

#include <cstddef>
#include <functional>
#include <queue>
#include <string>

namespace LCompilers::LanguageServiceProvider::AST {

  enum class TokenType {
    END_OF_FILE,

    COMMENT,

    SINGLE_QUOTE,
    DOUBLE_QUOTE,
    REP_CHAR_SEQ,
    CAT_OP,

    MAPS_TO,

    COLON,

    DEFINE,

    ACCESSOR,

    DOLLAR,
    QUESTION,

    EQ_CMP,
    NE_CMP,
    GE_CMP,
    GT_CMP,
    LE_CMP,
    LT_CMP,

    ASSIGN,

    PLUS,
    MINUS,
    STAR,
    SLASH,
    POWER,

    LOG_NOT,
    LOG_AND,
    LOG_OR,
    LOG_EQV,
    LOG_NEQV,

    CUSTOM_OP,

    LPAR,
    RPAR,

    BEGIN_ARRAY,
    END_ARRAY,

    NEWLINE,
    STMT_SEP,
    COMMA,

    UNDERSCORE,
    DECIMAL_POINT,
    EXPONENT_LETTER,

    LETTER,

    SYMBOL,

    KEYWORD,
    NAME,
    DIGIT_STRING,
    SCALAR_INT_CONSTANT_NAME,

    BINARY_CONSTANT,
    OCTAL_CONSTANT,
    HEX_CONSTANT,

    LOGICAL,
  };

  struct Token {
    std::size_t line;
    std::size_t column;
    std::size_t position;
    std::string text;
    TokenType type;

    Token(const std::size_t line,
          const std::size_t column,
          const std::size_t position,
          const std::string &text,
          const TokenType &type);

    Token();
  };

  class Lexer {
  public:
    Lexer(const std::string &text,
          std::size_t line,
          std::size_t column,
          std::size_t length);
    Lexer(const std::string &text);
    auto nextToken() -> Token;
  private:
    const std::string &text;
    std::size_t line;
    std::size_t column;
    std::size_t position;
    std::size_t length;

    std::queue<Token> pending;

    auto updatePosition() -> void;
    auto sanityCheck() -> void;
    auto initialize() -> void;

    auto parseComment() -> void;
    auto parseNewline() -> void;
    auto parseString(const unsigned char quote,
                     TokenType type) -> void;
    auto parseString(const unsigned char quote,
                     std::size_t line,
                     std::size_t column,
                     std::size_t position,
                     std::stringstream &ss) -> void;
    auto parseString(const unsigned char quote,
                     TokenType type,
                     const std::size_t line,
                     const std::size_t column,
                     const std::size_t position,
                     std::stringstream &ss) -> void;
    auto parse1(const unsigned char c, TokenType type) -> void;
    auto parse2(const unsigned char suffix,
                TokenType candidate,
                TokenType alternate) -> void;
    auto parse3(const unsigned char s1,
                const unsigned char s2,
                TokenType t1,
                TokenType t2,
                TokenType t3) -> void;
    auto parse4(const unsigned char s1,
                const unsigned char s2,
                const unsigned char s3,
                TokenType t1,
                TokenType t2,
                TokenType t3,
                TokenType t4) -> void;
    auto parseDot() -> void;
    auto parseCustomOp(const std::size_t line,
                       const std::size_t column,
                       const std::size_t position,
                       std::stringstream &ss) -> void;
    auto dropLineContinuation() -> void;
    auto dropBlanks() -> void;
    auto parseInteger() -> void;
    auto parseQuotedInteger(TokenType type,
                            std::function<int(int)>) -> void;
    auto parseName(std::stringstream &ss) -> void;
    auto parseName() -> void;
    auto parseReal(std::stringstream &ss) -> void;
    auto parseTrailingKind(std::stringstream &ss) -> void;

    auto advance() -> void;
    auto advance(std::size_t amount) -> void;

    auto nextNonBlankIsNewline() -> bool;

    auto assertNotEmpty(const std::string &message) const -> void;

    auto currChar(const std::string &message) -> unsigned char;

    inline auto currChar() const -> unsigned char {
      return text[position];
    }

    inline auto nextChar() const -> unsigned char {
      return text[position + 1];
    }

    inline auto isEmpty() const -> bool {
      return length == 0;
    }

    inline auto notEmpty() const -> bool {
      return length > 0;
    }

    inline auto hasMoreThan1() const -> bool {
      return length > 1;
    }
  };

} // namespace LCompilers::LanguageServiceProvider::AST

#endif // LCOMPILERS_LSP_AST_LEXER_H
