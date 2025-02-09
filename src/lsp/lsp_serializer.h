#pragma once

#include <memory>

#include <lsp/serializer.h>
#include <lsp/specification.h>

namespace LCompilers::LanguageServerProtocol {

  namespace ls = LCompilers::LanguageServer;

  class JsonRpcLspSerializer : public ls::JsonSerializer {
  public:
    auto serializeNotification(
      const NotificationMessage &notification
    ) const -> std::string;
    auto serializeRequest(
      const RequestMessage &request
    ) const -> std::string;
    auto serializeResponse(
      const ResponseMessage &response
    ) const -> std::string;
  private:
    auto prepareDocument() const -> rapidjson::Document;

    auto setResponseId(
      rapidjson::Document &document,
      const ResponseId &id,
      rapidjson::Document::AllocatorType &allocator
    ) const -> void;

    auto setResponseError(
      const ResponseError &error,
      rapidjson::Document &document,
      rapidjson::Document::AllocatorType &allocator
    ) const -> void;

    auto lspToJson(
      const LSPAny &lspAny,
      rapidjson::Document::AllocatorType &allocator
    ) const -> rapidjson::Value;
    auto lspToJson(
      const LSPObject &object,
      rapidjson::Document::AllocatorType &allocator
    ) const -> rapidjson::Value;
    auto lspToJson(
      const LSPArray &array,
      rapidjson::Document::AllocatorType &allocator
    ) const -> rapidjson::Value;
    auto lspToJson(
      const MessageParams &params,
      rapidjson::Document::AllocatorType &allocator
    ) const -> rapidjson::Value;
    auto lspToJson(
      const RequestId &requestId,
      rapidjson::Document::AllocatorType &allocator
    ) const -> rapidjson::Value;

    auto intToJson(
      int value,
      rapidjson::Document::AllocatorType &allocator
    ) const -> rapidjson::Value;
    auto stringToJson(
      const std::string &value,
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
    auto jsonToLsp(
      const rapidjson::Value &json
    ) const -> std::unique_ptr<LSPAny>;
  private:
    auto buildRequestId(
      const rapidjson::Value &jsonId
    ) const -> RequestId;
    auto buildMessageParams(
      const rapidjson::Value &jsonParams
    ) const -> MessageParams;
  };

} // namespace LCompilers::LanguageServerProtocol
