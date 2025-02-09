#include <sstream>
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
    case MessageParamsType::OBJECT_TYPE: {
      const LSPObject &object = std::get<LSPObject>(params);
      return lspToJson(object, allocator);
    }
    case MessageParamsType::ARRAY_TYPE: {
      const LSPArray &array = std::get<LSPArray>(params);
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
    case RequestIdType::INTEGER_TYPE: {
      int value = std::get<int>(requestId);
      return intToJson(value, allocator);
    }
    case RequestIdType::STRING_TYPE: {
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
    rapidjson::Document::AllocatorType &/*allocator*/
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
    case ResponseIdType::INTEGER_TYPE: {
      document.AddMember(
        "id",
        rapidjson::Value(std::get<integer_t>(id)),
        allocator
      );
      break;
    }
    case ResponseIdType::STRING_TYPE: {
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
    case ResponseIdType::NULL_TYPE: {
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
    switch (static_cast<LSPAnyType>(lspAny.index())) {
    case LSPAnyType::OBJECT_TYPE: {
      const LSPObject &object = std::get<LSPObject>(lspAny);
      return lspToJson(object, allocator);
    }
    case LSPAnyType::ARRAY_TYPE: {
      const LSPArray &array = std::get<LSPArray>(lspAny);
      return lspToJson(array, allocator);
    }
    case LSPAnyType::STRING_TYPE: {
      rapidjson::Value value;
      const string_t &stringValue = std::get<std::string>(lspAny);
      value.SetString(
        stringValue.c_str(),
        stringValue.length(),
        allocator
      );
      return value;
    }
    case LSPAnyType::INTEGER_TYPE: {
      rapidjson::Value value;
      value.SetInt(std::get<integer_t>(lspAny));
      return value;
    }
    case LSPAnyType::UINTEGER_TYPE: {
      rapidjson::Value value;
      value.SetUint(std::get<uinteger_t>(lspAny));
      return value;
    }
    case LSPAnyType::DECIMAL_TYPE: {
      rapidjson::Value value;
      value.SetDouble(std::get<decimal_t>(lspAny));
      return value;
    }
    case LSPAnyType::BOOLEAN_TYPE: {
      rapidjson::Value value;
      value.SetBool(std::get<boolean_t>(lspAny));
      return value;
    }
    case LSPAnyType::NULL_TYPE: {
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
      std::stringstream ss;
      ss << "Unsupported RequestId type: "
         << static_cast<int>(jsonId.GetType());
      throw LSP_EXCEPTION(ErrorCodes::INVALID_PARAMS, ss.str());
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
      LSPArray lspArray;
      for (rapidjson::Value::ConstValueIterator iter = jsonParams.Begin();
           iter != jsonParams.End();
           ++iter) {
        lspArray.push_back(jsonToLsp(*iter));
      }
      return lspArray;
    }
    case rapidjson::kObjectType: {
      LSPObject lspObject;
      for (rapidjson::Value::ConstMemberIterator iter = jsonParams.MemberBegin();
           iter != jsonParams.MemberEnd();
           ++iter) {
        lspObject.emplace(iter->name.GetString(), jsonToLsp(iter->value));
      }
      return lspObject;
    }
    default: {
      std::stringstream ss;
      ss << "Unsupported MessageParams type: "
         << static_cast<int>(jsonParams.GetType());
      throw LSP_EXCEPTION(ErrorCodes::INVALID_PARAMS, ss.str());
    }
    }
  }

  auto JsonRpcLspDeserializer::jsonToLsp(
    const rapidjson::Value &json
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> lspAny = std::make_unique<LSPAny>();
    switch (json.GetType()) {
    case rapidjson::kNullType: {
      (*lspAny) = nullptr;
      break;
    }
    case rapidjson::kFalseType: // fallthrough
    case rapidjson::kTrueType: {
      (*lspAny) = json.GetBool();
      break;
    }
    case rapidjson::kObjectType: {
      LSPObject lspObject;
      for (rapidjson::Value::ConstMemberIterator iter = json.MemberBegin();
           iter != json.MemberEnd();
           ++iter) {
        lspObject.emplace(iter->name.GetString(), jsonToLsp(iter->value));
      }
      (*lspAny) = std::move(lspObject);
      break;
    }
    case rapidjson::kArrayType: {
      LSPArray lspArray;
      for (rapidjson::Value::ConstValueIterator iter = json.Begin();
           iter != json.End();
           ++iter) {
        lspArray.push_back(jsonToLsp(*iter));
      }
      (*lspAny) = std::move(lspArray);
      break;
    }
    case rapidjson::kStringType: {
      (*lspAny) = json.GetString();
      break;
    }
    case rapidjson::kNumberType: {
      if (json.IsInt()) {
        (*lspAny) = json.GetInt();
      } else if (json.IsDouble()) {
        (*lspAny) = json.GetDouble();
      } else if (json.IsUint()) {
        (*lspAny) = json.GetUint();
      } else {
        std::stringstream ss;
        ss << "Unsupported JSON number type: "
           << static_cast<int>(json.GetType());
        throw LSP_EXCEPTION(ErrorCodes::INVALID_PARAMS, ss.str());
      }
      break;
    }
    }
    return lspAny;
  }

} // namespace LCompilers::LanguageServerProtocol
