#include <format>

#include <lsp/lsp_exception.h>

namespace LCompilers::LanguageServerProtocol {

  LspException::LspException(
    ErrorCode code,
    const std::string &message,
    const std::source_location location
  ) : std::logic_error(message)
    , _code(code)
    , _location(location)
  {
    // empty
  }

  auto LspException::code() const -> const ErrorCode & {
    return _code;
  }

  auto LspException::where() const -> const std::source_location & {
    return _location;
  }

} // namespace LCompilers::LanguageServerProtocol
