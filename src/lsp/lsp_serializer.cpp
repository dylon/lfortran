#include "lsp/lsp_language_server.h"
#include <format>
#include <iostream>
#include <stdexcept>
#include <utility>

#include <rapidjson/rapidjson.h>

#include <lsp/specification.h>
#include <lsp/lsp_exception.h>
#include <lsp/lsp_serializer.h>

namespace LCompilers::LanguageServerProtocol {

  auto JsonRpcLspSerializer::prepareDocument() const -> rapidjson::Document {
    rapidjson::Document document;
    rapidjson::Document::AllocatorType &allocator = document.GetAllocator();
    document.SetObject();
    document.AddMember(
      "jsonrpc",
      rapidjson::Value(
        JSON_RPC_VERSION.c_str(),
        JSON_RPC_VERSION.length(),
        allocator
      ),
      allocator
    );
    return document;
  }

  auto JsonRpcLspSerializer::serializeNotification(
    const NotificationMessage &notification
  ) const -> std::string {
    rapidjson::Document document = prepareDocument();
    rapidjson::Document::AllocatorType &allocator = document.GetAllocator();
    document.AddMember(
      "method",
      rapidjson::Value(
        notification.method.c_str(),
        notification.method.length(),
        allocator
      ),
      allocator
    );
    if (notification.params.has_value()) {
      rapidjson::Value params = lspToJson(
        notification.params.value(),
        allocator
      );
      document.AddMember("params", params, allocator);
    }
    return serialize(document);
  }

  auto JsonRpcLspSerializer::lspToJson(
    const MessageParams &params,
    rapidjson::Document::AllocatorType &allocator
  ) const -> rapidjson::Value {
    switch (static_cast<MessageParamsType>(params.index())) {
    case MessageParamsType::LSP_OBJECT: {
      const LSPObject &object =
        *std::get<std::unique_ptr<LSPObject>>(params);
      return lspToJson(object, allocator);
    }
    case MessageParamsType::LSP_ARRAY: {
      const LSPArray &array =
        *std::get<std::unique_ptr<LSPArray>>(params);
      return lspToJson(array, allocator);
    }
    }
    throw std::runtime_error(
      "JsonRpcLspSerializer::lspToJson should not reach the end of the function."
    );
  }

  auto JsonRpcLspSerializer::lspToJson(
    const LSPObject &object,
    rapidjson::Document::AllocatorType &allocator
  ) const -> rapidjson::Value {
    rapidjson::Value json;
    json.SetObject();
    for (const auto &[lspName, lspValue] : object) {
      rapidjson::Value fieldName(
        lspName.c_str(),
        lspName.length(),
        allocator
      );
      rapidjson::Value fieldValue = lspToJson(*lspValue, allocator);
      json.AddMember(fieldName, fieldValue, allocator);
    }
    return json;
  }

  auto JsonRpcLspSerializer::lspToJson(
    const LSPArray &array,
    rapidjson::Document::AllocatorType &allocator
  ) const -> rapidjson::Value {
    rapidjson::Value json;
    json.SetArray();
    for (const auto &lspValue : array) {
      rapidjson::Value jsonValue = lspToJson(*lspValue, allocator);
      json.PushBack(jsonValue, allocator);
    }
    return json;
  }

  auto JsonRpcLspSerializer::serializeRequest(
    const RequestMessage &request
  ) const -> std::string {
    rapidjson::Document document = prepareDocument();
    rapidjson::Document::AllocatorType &allocator = document.GetAllocator();
    document.AddMember(
      "id",
      lspToJson(request.id, allocator),
      allocator
    );
    document.AddMember(
      "method",
      stringToJson(request.method, allocator),
      allocator
    );
    if (request.params.has_value()) {
      rapidjson::Value params = lspToJson(
        request.params.value(),
        allocator
      );
      document.AddMember("params", params, allocator);
    }
    return serialize(document);
  }

  auto JsonRpcLspSerializer::lspToJson(
    const RequestId &requestId,
    rapidjson::Document::AllocatorType &allocator
  ) const -> rapidjson::Value {
    switch (static_cast<RequestIdType>(requestId.index())) {
    case RequestIdType::LSP_INTEGER: {
      int value = std::get<int>(requestId);
      return intToJson(value, allocator);
    }
    case RequestIdType::LSP_STRING: {
      const std::string &value = std::get<std::string>(requestId);
      return stringToJson(value, allocator);
    }
    }
    throw std::runtime_error(
      "JsonRpcLspSerializer::lspToJson should not reach the end of the function."
    );
  }

  auto JsonRpcLspSerializer::intToJson(
    int value,
    rapidjson::Document::AllocatorType &allocator
  ) const -> rapidjson::Value {
    return rapidjson::Value(value);
  }

  auto JsonRpcLspSerializer::stringToJson(
    const std::string &value,
    rapidjson::Document::AllocatorType &allocator
  ) const -> rapidjson::Value {
    return rapidjson::Value(value.c_str(), value.length(), allocator);
  }

  auto JsonRpcLspSerializer::serializeResponse(
    const ResponseMessage &response
  ) const -> std::string {
    rapidjson::Document document = prepareDocument();
    rapidjson::Document::AllocatorType &allocator = document.GetAllocator();
    setResponseId(document, response.id, allocator);
    if (response.result.has_value()) {
      rapidjson::Value result = lspToJson(
        *response.result.value(),
        allocator
      );
      document.AddMember("result", result, allocator);
    }
    if (response.error.has_value()) {
      setResponseError(*response.error.value(), document, allocator);
    }
    return serialize(document);
  }

  auto JsonRpcLspSerializer::setResponseId(
    rapidjson::Document &document,
    const ResponseId &id,
    rapidjson::Document::AllocatorType &allocator
  ) const -> void {
    switch (static_cast<ResponseIdType>(id.index())) {
    case ResponseIdType::LSP_INTEGER: {
      document.AddMember(
        "id",
        rapidjson::Value(std::get<integer>(id)),
        allocator
      );
      break;
    }
    case ResponseIdType::LSP_STRING: {
      std::string stringId = std::get<std::string>(id);
      document.AddMember(
        "id",
        rapidjson::Value(
          stringId.c_str(),
          stringId.length(),
          allocator
        ),
        allocator
      );
      break;
    }
    case ResponseIdType::LSP_NULL: {
      document.AddMember(
        "id",
        rapidjson::Value(rapidjson::kNullType),
        allocator
      );
      break;
    }
    }
  }

  auto JsonRpcLspSerializer::setResponseError(
    const ResponseError &error,
    rapidjson::Document &document,
    rapidjson::Document::AllocatorType &allocator
  ) const -> void {
    rapidjson::Value value(rapidjson::kObjectType);
    value.AddMember("code", error.code, allocator);
    value.AddMember(
      "message",
      rapidjson::Value(
        error.message.c_str(),
        error.message.length(),
        allocator
      ),
      allocator
    );
    if (error.data.has_value()) {
      rapidjson::Value data = lspToJson(*error.data.value(), allocator);
      value.AddMember("data", data, allocator);
    }
    document.AddMember("error", value, allocator);
  }

  auto JsonRpcLspSerializer::lspToJson(
    const LSPAny &lspAny,
    rapidjson::Document::AllocatorType &allocator
  ) const -> rapidjson::Value {
    switch (lspAny.type) {
    case LSPAnyType::LSP_OBJECT: {
      const LSPObject &object =
        *std::get<std::unique_ptr<LSPObject>>(lspAny.value);
      return lspToJson(object, allocator);
    }
    case LSPAnyType::LSP_ARRAY: {
      const LSPArray &array =
        *std::get<std::unique_ptr<LSPArray>>(lspAny.value);
      return lspToJson(array, allocator);
    }
    case LSPAnyType::LSP_STRING: {
      rapidjson::Value value;
      const string &stringValue = std::get<string>(lspAny.value);
      value.SetString(
        stringValue.c_str(),
        stringValue.length(),
        allocator
      );
      return value;
    }
    case LSPAnyType::LSP_INTEGER: {
      rapidjson::Value value;
      value.SetInt(std::get<integer>(lspAny.value));
      return value;
    }
    case LSPAnyType::LSP_UINTEGER: {
      rapidjson::Value value;
      value.SetUint(std::get<uinteger>(lspAny.value));
      return value;
    }
    case LSPAnyType::LSP_DECIMAL: {
      rapidjson::Value value;
      value.SetDouble(std::get<decimal>(lspAny.value));
      return value;
    }
    case LSPAnyType::LSP_BOOLEAN: {
      rapidjson::Value value;
      value.SetBool(std::get<boolean>(lspAny.value));
      return value;
    }
    case LSPAnyType::LSP_NULL: {
      rapidjson::Value value;
      value.SetNull();
      return value;
    }
    }
    throw std::runtime_error(
      "JsonRpcLspSerializer::lspToJson should not reach the end of the function."
    );
  }

  auto JsonRpcLspDeserializer::deserializeRequest(
    const rapidjson::Document &document
  ) const -> RequestMessage {
    RequestMessage request;
    request.jsonrpc = document["jsonrpc"].GetString();
    request.id = buildRequestId(document["id"]);
    request.method = document["method"].GetString();
    if (document.HasMember("params")) {
      request.params = buildMessageParams(document["params"]);
    }
    return request;
  }

  auto JsonRpcLspDeserializer::buildRequestId(
    const rapidjson::Value &jsonId
  ) const -> RequestId {
    switch (jsonId.GetType()) {
    case rapidjson::kStringType: {
      return jsonId.GetString();
    }
    case rapidjson::kNumberType: {
      return jsonId.GetInt();
    }
    default: {
      throw LspException(
        ErrorCodes::InvalidParams,
        std::format(
          "Unsupported RequestId type: {}",
          static_cast<int>(jsonId.GetType())
        )
      );
    }
    }
  }

  auto JsonRpcLspDeserializer::deserializeNotification(
    const rapidjson::Document &document
  ) const -> NotificationMessage {
    NotificationMessage notification;
    notification.jsonrpc = document["jsonrpc"].GetString();
    notification.method = document["method"].GetString();
    if (document.HasMember("params")) {
      notification.params = buildMessageParams(document["params"]);
    }
    return notification;
  }

  auto JsonRpcLspDeserializer::buildMessageParams(
    const rapidjson::Value &jsonParams
  ) const -> MessageParams {
    switch (jsonParams.GetType()) {
    case rapidjson::kArrayType: {
      std::unique_ptr<LSPArray> lspArray = std::make_unique<LSPArray>();
      for (rapidjson::Value::ConstValueIterator iter = jsonParams.Begin();
           iter != jsonParams.End();
           ++iter) {
        lspArray->push_back(jsonToLsp(*iter));
      }
      return lspArray;
    }
    case rapidjson::kObjectType: {
      std::unique_ptr<LSPObject> lspObject = std::make_unique<LSPObject>();
      for (rapidjson::Value::ConstMemberIterator iter = jsonParams.MemberBegin();
           iter != jsonParams.MemberEnd();
           ++iter) {
        (*lspObject)[iter->name.GetString()] = jsonToLsp(iter->value);
      }
      return lspObject;
    }
    default: {
      throw LspException(
        ErrorCodes::InvalidParams,
        std::format(
          "Unsupported MessageParams type: {}",
          static_cast<int>(jsonParams.GetType())
        )
      );
    }
    }
  }

  auto JsonRpcLspDeserializer::jsonToLsp(
    const rapidjson::Value &json
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> lspAny = std::make_unique<LSPAny>();
    switch (json.GetType()) {
    case rapidjson::kNullType: {
      lspAny->type = LSPAnyType::LSP_NULL;
      lspAny->value = nullptr;
      break;
    }
    case rapidjson::kFalseType: // fallthrough
    case rapidjson::kTrueType: {
      lspAny->type = LSPAnyType::LSP_BOOLEAN;
      lspAny->value = json.GetBool();
      break;
    }
    case rapidjson::kObjectType: {
      lspAny->type = LSPAnyType::LSP_OBJECT;
      std::unique_ptr<LSPObject> lspObject = std::make_unique<LSPObject>();
      for (rapidjson::Value::ConstMemberIterator iter = json.MemberBegin();
           iter != json.MemberEnd();
           ++iter) {
        (*lspObject)[iter->name.GetString()] = jsonToLsp(iter->value);
      }
      lspAny->value = std::move(lspObject);
      break;
    }
    case rapidjson::kArrayType: {
      lspAny->type = LSPAnyType::LSP_ARRAY;
      std::unique_ptr<LSPArray> lspArray = std::make_unique<LSPArray>();
      for (rapidjson::Value::ConstValueIterator iter = json.Begin();
           iter != json.End();
           ++iter) {
        lspArray->push_back(jsonToLsp(*iter));
      }
      lspAny->value = std::move(lspArray);
      break;
    }
    case rapidjson::kStringType: {
      lspAny->type = LSPAnyType::LSP_STRING;
      lspAny->value = json.GetString();
      break;
    }
    case rapidjson::kNumberType: {
      if (json.IsInt()) {
        lspAny->type = LSPAnyType::LSP_INTEGER;
        lspAny->value = json.GetInt();
      } else if (json.IsDouble()) {
        lspAny->type = LSPAnyType::LSP_DECIMAL;
        lspAny->value = json.GetDouble();
      } else if (json.IsUint()) {
        lspAny->type = LSPAnyType::LSP_UINTEGER;
        lspAny->value = json.GetUint();
      } else {
        throw LspException(
          ErrorCodes::InvalidParams,
          std::format(
            "Unsupported JSON number type: {}",
            static_cast<int>(json.GetType())
          )
        );
      }
      break;
    }
    }
    return lspAny;
  }

} // namespace LCompilers::LanguageServerProtocol
