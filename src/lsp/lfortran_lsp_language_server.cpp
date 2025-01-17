#include <format>
#include <iostream>
#include <stdexcept>

#include <lsp/specification.h>
#include <lsp/lfortran_lsp_language_server.h>

namespace LCompilers::LanguageServerProtocol {

  std::string LFortranLspLanguageServer::serve(const std::string &request) {
    rapidjson::Document document = deserializer.deserialize(request);
    ResponseMessage response;
    if (document.HasMember("id")) {
      RequestMessage request = deserializer.deserializeRequest(document);
      response = dispatch(request);
      response.jsonrpc = request.jsonrpc;
      setResponseId(request, response);
    } else {
      NotificationMessage notification =
        deserializer.deserializeNotification(document);
      response = dispatch(notification);
      response.jsonrpc = notification.jsonrpc;
      setResponseId(notification, response);
    }
    return serializer.serializeResponse(response);
  }

  auto LFortranLspLanguageServer::setResponseId(
    const RequestMessage &request,
    ResponseMessage &response
  ) const -> void {
    response.id = std::make_unique<ResponseId>();
    switch (request.id->type) {
    case RequestIdType::LSP_INTEGER: {
      response.id->type = ResponseIdType::LSP_INTEGER;
      response.id->value = std::get<integer>(request.id->value);
      break;
    }
    case RequestIdType::LSP_STRING: {
      response.id->type = ResponseIdType::LSP_STRING;
      response.id->value = std::get<string>(request.id->value);
      break;
    }
    }
  }

  auto LFortranLspLanguageServer::setResponseId(
    const NotificationMessage &notification,
    ResponseMessage &response
  ) const -> void {
    response.id = std::make_unique<ResponseId>();
    response.id->type = ResponseIdType::LSP_NULL;
    response.id->value = nullptr;
  }

  auto LFortranLspLanguageServer::dispatch(
    const RequestMessage &request
  ) -> ResponseMessage {
    if (request.method == "initialize") {
      if (request.params.has_value() || true) {
        InitializeParams params =
          buildInitializeParams(*request.params.value());
        InitializeResult result = initialize(params);
        return buildResponseMessage(result);
      } else {
        throw std::logic_error(
          "RequestMessage.params must be defined for "
          "RequestMessage.method=\"initialize\""
        );
      }
    } else {
      throw std::logic_error(
        std::format(
          "Unsupported RequestMessage.method: \"{}\"",
          request.method
        )
      );
    }
  }

  auto LFortranLspLanguageServer::buildResponseMessage(
    const InitializeResult &result
  ) const -> ResponseMessage {
    ResponseMessage response;
    response.result = lspToAny(result);
    return response;
  }

  auto LFortranLspLanguageServer::buildInitializeParams(
    const RequestParams &requestParams
  ) const -> InitializeParams {
    InitializeParams initializeParams;
    if (requestParams.type == RequestParamsType::LSP_OBJECT) {
      LSPObject &lspObject =
        *std::get<std::unique_ptr<LSPObject>>(requestParams.value);
      if (lspObject.contains("capabilities")) {
        initializeParams.capabilities =
          buildClientCapabilities(*lspObject.at("capabilities"));
      } else {
        throw std::logic_error(
          "Missing required attribute: InitializeParams.capabilities"
        );
      }
    } else {
      throw std::logic_error(
        std::format(
          "Unsupported RequestParams.type: {}",
          static_cast<int>(requestParams.type)
        )
      );
    }
    return initializeParams;
  }

  auto LFortranLspLanguageServer::buildClientCapabilities(
    const LSPAny &lspAny
  ) const -> std::unique_ptr<ClientCapabilities> {
    std::unique_ptr<ClientCapabilities> capabilities =
      std::make_unique<ClientCapabilities>();
    return capabilities;
  }

  auto LFortranLspLanguageServer::initialize(
    const InitializeParams &params
  ) -> InitializeResult {
    InitializeResult result;
    std::unique_ptr<ServerCapabilities> capabilities =
      std::make_unique<ServerCapabilities>();
    result.capabilities = std::move(capabilities);
    return result;
  }

  auto LFortranLspLanguageServer::dispatch(
    const NotificationMessage &notification
  ) -> ResponseMessage {
    ResponseMessage response;
    return response;
  }

  auto LFortranLspLanguageServer::lspToAny(
    const InitializeResult &result
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->type = LSPAnyType::LSP_OBJECT;
    std::unique_ptr<LSPObject> value = std::make_unique<LSPObject>();
    (*value)["capabilities"] = lspToAny(*result.capabilities);
    any->value = std::move(value);
    return any;
  }

  auto LFortranLspLanguageServer::lspToAny(
    const ServerCapabilities &capabilities
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->type = LSPAnyType::LSP_OBJECT;
    std::unique_ptr<LSPObject> value = std::make_unique<LSPObject>();
    any->value = std::move(value);
    return any;
  }

} // namespace LCompilers::LanguageServerProtocol
