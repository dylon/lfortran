#ifndef LCOMPILERS_LSP_SERIALIZER_H
#define LCOMPILERS_LSP_SERIALIZER_H

#include <memory>

#include <lsp/serializer.h>
#include <lsp/specification.h>

namespace LCompilers::LanguageServerProtocol {

  namespace ls = LCompilers::LanguageServer;

  class JsonRpcLspSerializer : public ls::JsonSerializer {
  public:
    auto serializeResponse(
      const ResponseMessage &response
    ) const -> std::string;
  private:
    auto setResponseId(
      rapidjson::Document &document,
      const ResponseId &id
    ) const -> void;
    auto setResponseError(
      const ResponseError &error,
      rapidjson::Document &document
    ) const -> void;
    auto lspToJson(
      const LSPAny &lspAny,
      rapidjson::Document::AllocatorType &allocator
    ) const -> rapidjson::Value;
  };

  class JsonRpcLspDeserializer : public ls::JsonDeserializer {
  public:
    auto deserializeRequest(
      const rapidjson::Document &document
    ) const -> RequestMessage;
    auto deserializeNotification(
      const rapidjson::Document &document
    ) const -> NotificationMessage;
  private:
    auto buildRequestId(
      const rapidjson::Value &jsonId
    ) const -> std::unique_ptr<RequestId>;
    auto buildRequestParams(
      const rapidjson::Value &jsonParams
    ) const -> std::unique_ptr<RequestParams>;
    auto buildNotificationParams(
      const rapidjson::Value &jsonParams
    ) const -> std::unique_ptr<NotificationParams>;
    auto jsonToLsp(
      const rapidjson::Value &json
    ) const -> std::unique_ptr<LSPAny>;
  };

} // namespace LCompilers::LanguageServerProtocol

#endif // LCOMPILERS_LSP_SERIALIZER_H
