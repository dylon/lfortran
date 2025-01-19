#include <format>
#include <stdexcept>
#include <utility>

#include <rapidjson/rapidjson.h>

#include <lsp/specification.h>
#include <lsp/lsp_exception.h>
#include <lsp/lsp_serializer.h>

namespace LCompilers::LanguageServerProtocol {

  auto JsonRpcLspSerializer::serializeResponse(
    const ResponseMessage &response
  ) const -> std::string {
    rapidjson::Document document;
    rapidjson::Document::AllocatorType &allocator = document.GetAllocator();
    document.SetObject();
    document.AddMember(
      "jsonrpc",
      rapidjson::Value(
        response.jsonrpc.c_str(),
        response.jsonrpc.length(),
        allocator
      ),
      allocator
    );
    setResponseId(document, *response.id);
    if (response.result.has_value()) {
      rapidjson::Value result = lspToJson(
        *response.result.value(),
        document.GetAllocator()
      );
      document.AddMember("result", result, document.GetAllocator());
    }
    if (response.error.has_value()) {
      setResponseError(*response.error.value(), document);
    }
    return serialize(document);
  }

  auto JsonRpcLspSerializer::setResponseId(
    rapidjson::Document &document,
    const ResponseId &id
  ) const -> void {
    rapidjson::Document::AllocatorType &allocator = document.GetAllocator();
    switch (id.type) {
    case ResponseIdType::LSP_INTEGER: {
      document.AddMember(
        "id",
        rapidjson::Value(std::get<integer>(id.value)),
        allocator
      );
      break;
    }
    case ResponseIdType::LSP_STRING: {
      std::string stringId = std::get<std::string>(id.value);
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
    rapidjson::Document &document
  ) const -> void {
    rapidjson::Document::AllocatorType &allocator = document.GetAllocator();
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
    rapidjson::Value value;
    switch (lspAny.type) {
    case LSPAnyType::LSP_OBJECT: {
      value.SetObject();
      for (const auto &[lspName, lspValue]
             : *std::get<std::unique_ptr<LSPObject>>(lspAny.value)) {
        rapidjson::Value fieldName(
          lspName.c_str(),
          lspName.length(),
          allocator
        );
        rapidjson::Value fieldValue = lspToJson(*lspValue, allocator);
        value.AddMember(fieldName, fieldValue, allocator);
      }
      break;
    }
    case LSPAnyType::LSP_ARRAY: {
      value.SetArray();
      for (const auto &lspValue
             : *std::get<std::unique_ptr<LSPArray>>(lspAny.value)) {
        rapidjson::Value jsonValue = lspToJson(*lspValue, allocator);
        value.PushBack(jsonValue, allocator);
      }
      break;
    }
    case LSPAnyType::LSP_STRING: {
      const string &stringValue = std::get<string>(lspAny.value);
      value.SetString(
        stringValue.c_str(),
        stringValue.length(),
        allocator
      );
      break;
    }
    case LSPAnyType::LSP_INTEGER: {
      value.SetInt(std::get<integer>(lspAny.value));
      break;
    }
    case LSPAnyType::LSP_UINTEGER: {
      value.SetUint(std::get<uinteger>(lspAny.value));
      break;
    }
    case LSPAnyType::LSP_DECIMAL: {
      value.SetDouble(std::get<decimal>(lspAny.value));
      break;
    }
    case LSPAnyType::LSP_BOOLEAN: {
      value.SetBool(std::get<boolean>(lspAny.value));
      break;
    }
    case LSPAnyType::LSP_NULL: {
      value.SetNull();
      break;
    }
    }
    return value;
  }

  auto JsonRpcLspDeserializer::deserializeRequest(
    const rapidjson::Document &document
  ) const -> RequestMessage {
    RequestMessage request;
    request.jsonrpc = document["jsonrpc"].GetString();
    request.id = buildRequestId(document["id"]);
    request.method = document["method"].GetString();
    if (document.HasMember("params")) {
      request.params = buildRequestParams(document["params"]);
    }
    return request;
  }

  auto JsonRpcLspDeserializer::buildRequestId(
    const rapidjson::Value &jsonId
  ) const -> std::unique_ptr<RequestId> {
    std::unique_ptr<RequestId> requestId = std::make_unique<RequestId>();
    switch (jsonId.GetType()) {
    case rapidjson::kStringType: {
      requestId->type = RequestIdType::LSP_STRING;
      requestId->value = jsonId.GetString();
      break;
    }
    case rapidjson::kNumberType: {
      requestId->type = RequestIdType::LSP_INTEGER;
      requestId->value = jsonId.GetInt();
      break;
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
    return requestId;
  }

  auto JsonRpcLspDeserializer::deserializeNotification(
    const rapidjson::Document &document
  ) const -> NotificationMessage {
    NotificationMessage notification;
    notification.jsonrpc = document["jsonrpc"].GetString();
    notification.method = document["method"].GetString();
    if (document.HasMember("params")) {
      notification.params = buildNotificationParams(document["params"]);
    }
    return notification;
  }

  auto JsonRpcLspDeserializer::buildRequestParams(
    const rapidjson::Value &jsonParams
  ) const -> std::unique_ptr<RequestParams> {
    std::unique_ptr<RequestParams> requestParams =
      std::make_unique<RequestParams>();
    switch (jsonParams.GetType()) {
    case rapidjson::kArrayType: {
      requestParams->type = RequestParamsType::LSP_ARRAY;
      std::unique_ptr<LSPArray> lspArray = std::make_unique<LSPArray>();
      for (rapidjson::Value::ConstValueIterator iter = jsonParams.Begin();
           iter != jsonParams.End();
           ++iter) {
        lspArray->push_back(jsonToLsp(*iter));
      }
      requestParams->value = std::move(lspArray);
      break;
    }
    case rapidjson::kObjectType: {
      requestParams->type = RequestParamsType::LSP_OBJECT;
      std::unique_ptr<LSPObject> lspObject = std::make_unique<LSPObject>();
      for (rapidjson::Value::ConstMemberIterator iter = jsonParams.MemberBegin();
           iter != jsonParams.MemberEnd();
           ++iter) {
        (*lspObject)[iter->name.GetString()] = jsonToLsp(iter->value);
      }
      requestParams->value = std::move(lspObject);
      break;
    }
    default: {
      throw LspException(
        ErrorCodes::InvalidParams,
        std::format(
          "Unsupported RequestParams type: {}",
          static_cast<int>(jsonParams.GetType())
        )
      );
    }
    }
    return requestParams;
  }

  auto JsonRpcLspDeserializer::buildNotificationParams(
    const rapidjson::Value &jsonParams
  ) const -> std::unique_ptr<NotificationParams> {
    std::unique_ptr<NotificationParams> notificationParams =
      std::make_unique<NotificationParams>();
    switch (jsonParams.GetType()) {
    case rapidjson::kArrayType: {
      notificationParams->type = NotificationParamsType::LSP_ARRAY;
      std::unique_ptr<LSPArray> lspArray = std::make_unique<LSPArray>();
      for (rapidjson::Value::ConstValueIterator iter = jsonParams.Begin();
           iter != jsonParams.End();
           ++iter) {
        lspArray->push_back(jsonToLsp(*iter));
      }
      notificationParams->value = std::move(lspArray);
      break;
    }
    case rapidjson::kObjectType: {
      notificationParams->type = NotificationParamsType::LSP_OBJECT;
      std::unique_ptr<LSPObject> lspObject = std::make_unique<LSPObject>();
      for (rapidjson::Value::ConstMemberIterator iter = jsonParams.MemberBegin();
           iter != jsonParams.MemberEnd();
           ++iter) {
        (*lspObject)[iter->name.GetString()] = jsonToLsp(iter->value);
      }
      notificationParams->value = std::move(lspObject);
      break;
    }
    default: {
      throw LspException(
        ErrorCodes::InvalidParams,
        std::format(
          "Unsupported NotificationParams type: {}",
          static_cast<int>(jsonParams.GetType())
        )
      );
    }
    }
    return notificationParams;
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
