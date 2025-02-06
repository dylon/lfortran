#include <lsp/language_server.h>

namespace LCompilers::LanguageServer {

  LanguageServer::LanguageServer(
    MessageQueue &outgoingMessages,
    lsl::Logger &logger
  ) : outgoingMessages(outgoingMessages)
    , logger(logger)
  {
    // empty
  }

} // namespace LCompilers::LanguageServer
