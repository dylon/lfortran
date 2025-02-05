#ifndef LCOMPILERS_LSP_EXCEPTION_H
#define LCOMPILERS_LSP_EXCEPTION_H

#include <source_location>
#include <stdexcept>
#include <string>
#include <variant>

#include <lsp/specification.h>

namespace LCompilers::LanguageServerProtocol {

  enum class ErrorCodeType {
    ERROR_CODES,
    LSP_ERROR_CODES,
  };

  typedef std::variant<
    ErrorCodes,
    LSPErrorCodes
  > ErrorCode;

  class LspException : public std::logic_error {
  public:
    LspException(
      ErrorCode code,
      const std::string &message,
      const std::source_location location = std::source_location::current()
    );
    auto code() const -> const ErrorCode &;
    auto where() const -> const std::source_location &;
  protected:
    ErrorCode _code;
    std::source_location _location;
  };

} // namespace LCompilers::LanguageServerProtocol

#endif // LCOMPILERS_LSP_EXCEPTION_H
