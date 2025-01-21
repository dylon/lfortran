#include <lsp/language_server.h>

namespace LCompilers::LanguageServer {

  LanguageServer::LanguageServer(MessageQueue &outgoingMessages)
    : outgoingMessages(outgoingMessages)
  {
    // empty
  }

} // namespace LCompilers::LanguageServer
