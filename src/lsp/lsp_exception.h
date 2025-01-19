#ifndef LCOMPILERS_LSP_EXCEPTION_H
#define LCOMPILERS_LSP_EXCEPTION_H

#include <source_location>
#include <stdexcept>
#include <string>

#include <lsp/specification.h>

namespace LCompilers::LanguageServerProtocol {

  class LspException : public std::logic_error {
  public:
    LspException(
      ErrorCodes code,
      const std::string &message,
      const std::source_location location = std::source_location::current()
    );
    auto code() const -> ErrorCodes;
    auto where() const -> const std::source_location &;
  protected:
    ErrorCodes _code;
    std::source_location _location;
  };

} // namespace LCompilers::LanguageServerProtocol

#endif // LCOMPILERS_LSP_EXCEPTION_H
