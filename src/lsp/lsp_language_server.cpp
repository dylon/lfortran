// -----------------------------------------------------------------------------
// NOTE: This file was generated from Microsoft's Language Server Protocol (LSP)
// specification. Please do not edit it by hand.
// -----------------------------------------------------------------------------

#include <cctype>
#include <format>
#include <iostream>
#include <mutex>
#include <stdexcept>

#include <lsp/specification.h>
#include <lsp/lsp_exception.h>
#include <lsp/lsp_language_server.h>

namespace LCompilers::LanguageServerProtocol {

  LspLanguageServer::LspLanguageServer(
    ls::MessageQueue &outgoingMessages
  ) : ls::LanguageServer(outgoingMessages)
  {
    // empty
  }

  auto LspLanguageServer::nextId() -> RequestId {
    return serialId++;
  }

  std::string LspLanguageServer::serve(const std::string &request) {
    ResponseMessage response;
    try {
      // The language server protocol always uses “2.0” as the jsonrpc version.
      response.jsonrpc = JSON_RPC_VERSION;
      response.id = nullptr;

      rapidjson::Document document = deserializer.deserialize(request);
      if (document.HasParseError()) {
        throw LspException(
          ErrorCodes::PARSE_ERROR,
          std::format(
            "Invalid JSON request (error={}): {}",
            static_cast<int>(document.GetParseError()),
            request
          )
        );
      }

      if (document.HasMember("id")) {
        const rapidjson::Value &idValue = document["id"];
        if (idValue.IsString()) {
          response.id = idValue.GetString();
        } else if (idValue.IsInt()) {
          response.id = idValue.GetInt();
        } else if (!idValue.IsNull()) { // null => notification
          throw LspException(
            ErrorCodes::INVALID_PARAMS,
            std::format(
              "Unsupported type for id attribute: {}",
              static_cast<int>(idValue.GetType())
            )
          );
        }
      }

      if (document.HasMember("method")) {
        const std::string &method = document["method"].GetString();
        if (isIncomingRequest(method)) {
          if (static_cast<ResponseIdType>(response.id.index()) == ResponseIdType::NULL_TYPE) {
            throw LspException(
              ErrorCodes::INVALID_PARAMS,
              std::format(
                "Missing request method=\"{}\" attribute: id",
                method
              )
            );
          }
          RequestMessage request = deserializer.deserializeRequest(document);
          response.jsonrpc = request.jsonrpc;
          dispatch(response, request);
        } else if (isIncomingNotification(method)) {
          if (static_cast<ResponseIdType>(response.id.index()) != ResponseIdType::NULL_TYPE) {
            throw LspException(
              ErrorCodes::INVALID_PARAMS,
              std::format(
                "Notification method=\"{}\" must not contain the attribute: id",
                method
              )
            );
          }
          NotificationMessage notification =
            deserializer.deserializeNotification(document);
          response.jsonrpc = notification.jsonrpc;
          dispatch(response, notification);
        } else {
          throw LspException(
            ErrorCodes::INVALID_REQUEST,
            std::format("Unsupported method: \"{}\"", method)
          );
        }
      } else {
        throw LspException(
          ErrorCodes::INVALID_REQUEST,
          "Missing required attribute: method"
        );
      }
    } catch (const LspException &e) {
      const std::source_location &where = e.where();
      std::cerr
        << "[" << where.file_name() << ":" << where.line() << ":" << where.column() << "] "
        << e.what()
        << std::endl;
      std::unique_ptr<ResponseError> error =
        std::make_unique<ResponseError>();
      switch (static_cast<ErrorCodeType>(e.code().index())) {
      case ErrorCodeType::ERROR_CODES: {
        ErrorCodes errorCode = std::get<ErrorCodes>(e.code());
        error->code = static_cast<int>(errorCode);
        break;
      }
      case ErrorCodeType::LSP_ERROR_CODES: {
        LSPErrorCodes errorCode = std::get<LSPErrorCodes>(e.code());
        error->code = static_cast<int>(errorCode);
        break;
      }
      }
      error->message = e.what();
      response.error = std::move(error);
    } catch (const std::exception &e) {
      std::cerr << "Caught unhandled exception: " << e.what() << std::endl;
      std::unique_ptr<ResponseError> error =
        std::make_unique<ResponseError>();
      error->code = static_cast<int>(ErrorCodes::INTERNAL_ERROR);
      error->message =
        "An unexpected exception occurred. If it continues, please file a ticket.";
      response.error = std::move(error);
    }
    return serializer.serializeResponse(response);
  }

  auto LspLanguageServer::isInitialized() const -> bool {
    return _initialized;
  }

  auto LspLanguageServer::isShutdown() const -> bool {
    return _shutdown;
  }

  auto LspLanguageServer::isTerminated() const -> bool {
    return _exit;
  }

  auto LspLanguageServer::isRunning() const -> bool {
    return !_shutdown;
  }

  auto LspLanguageServer::initializeParams() const -> const InitializeParams & {
    if (_initializeParams) {
      return *_initializeParams;
    }
    throw std::logic_error(
      "LspLanguageServer::initialize must be called before LspLanguageServer::initializeParams"
    );
  }

  auto LspLanguageServer::assertInitialized() -> void{
    if (!_initialized) {
      throw LspException(
        ErrorCodes::SERVER_NOT_INITIALIZED,
        "Method \"initialize\" must be called first."
      );
    }
  }

  auto LspLanguageServer::assertRunning() -> void {
    if (_shutdown) {
      throw LspException(
        LSPErrorCodes::REQUEST_FAILED,
        "Server has shutdown and cannot accept new requests."
      );
    }
  }

  auto LspLanguageServer::prepare(
    std::ostream &os,
    const std::string &response
  ) const -> void {
    os << "Content-Type: application/vscode-jsonrpc; charset=utf-8\r\n"
       << "Content-Length: " << response.length() << "\r\n"
       << "\r\n"
       << response;
  }

  auto LspLanguageServer::prepare(
    std::stringstream &ss,
    const std::string &response
  ) const -> void {
    ss << "Content-Type: application/vscode-jsonrpc; charset=utf-8\r\n"
       << "Content-Length: " << response.length() << "\r\n"
       << "\r\n"
       << response;
  }

  auto LspLanguageServer::dispatch(
    ResponseMessage &response,
    RequestMessage &request
  ) -> void {
    IncomingRequest method;
    try {
      method = incomingRequestByValue(request.method);
    } catch (std::invalid_argument &e) {
      goto invalidMethod;
    }
    assertRunning();
    if (method != IncomingRequest::INITIALIZE) {
      assertInitialized();
    } else {
      bool expected = false;  // a reference is required ...
      if (!_initialized.compare_exchange_strong(expected, true)) {
        throw LspException(
          ErrorCodes::INVALID_REQUEST,
          "Server may be initialized only once."
        );
      }
    }
    switch (method) {
    case IncomingRequest::TEXT_DOCUMENT_IMPLEMENTATION: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<ImplementationParams> requestParams =
        transformer.asTextDocumentImplementationParams(messageParams);
      TextDocumentImplementationResult result =
        handleTextDocumentImplementation(*requestParams);
      response.result = transformer.textDocumentImplementationResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_TYPE_DEFINITION: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<TypeDefinitionParams> requestParams =
        transformer.asTextDocumentTypeDefinitionParams(messageParams);
      TextDocumentTypeDefinitionResult result =
        handleTextDocumentTypeDefinition(*requestParams);
      response.result = transformer.textDocumentTypeDefinitionResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_DOCUMENT_COLOR: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<DocumentColorParams> requestParams =
        transformer.asTextDocumentDocumentColorParams(messageParams);
      TextDocumentDocumentColorResult result =
        handleTextDocumentDocumentColor(*requestParams);
      response.result = transformer.textDocumentDocumentColorResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_COLOR_PRESENTATION: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<ColorPresentationParams> requestParams =
        transformer.asTextDocumentColorPresentationParams(messageParams);
      TextDocumentColorPresentationResult result =
        handleTextDocumentColorPresentation(*requestParams);
      response.result = transformer.textDocumentColorPresentationResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_FOLDING_RANGE: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<FoldingRangeParams> requestParams =
        transformer.asTextDocumentFoldingRangeParams(messageParams);
      TextDocumentFoldingRangeResult result =
        handleTextDocumentFoldingRange(*requestParams);
      response.result = transformer.textDocumentFoldingRangeResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_DECLARATION: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<DeclarationParams> requestParams =
        transformer.asTextDocumentDeclarationParams(messageParams);
      TextDocumentDeclarationResult result =
        handleTextDocumentDeclaration(*requestParams);
      response.result = transformer.textDocumentDeclarationResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_SELECTION_RANGE: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<SelectionRangeParams> requestParams =
        transformer.asTextDocumentSelectionRangeParams(messageParams);
      TextDocumentSelectionRangeResult result =
        handleTextDocumentSelectionRange(*requestParams);
      response.result = transformer.textDocumentSelectionRangeResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_PREPARE_CALL_HIERARCHY: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<CallHierarchyPrepareParams> requestParams =
        transformer.asTextDocumentPrepareCallHierarchyParams(messageParams);
      TextDocumentPrepareCallHierarchyResult result =
        handleTextDocumentPrepareCallHierarchy(*requestParams);
      response.result = transformer.textDocumentPrepareCallHierarchyResultToAny(result);
      break;
    }
    case IncomingRequest::CALL_HIERARCHY_INCOMING_CALLS: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<CallHierarchyIncomingCallsParams> requestParams =
        transformer.asCallHierarchyIncomingCallsParams(messageParams);
      CallHierarchyIncomingCallsResult result =
        handleCallHierarchyIncomingCalls(*requestParams);
      response.result = transformer.callHierarchyIncomingCallsResultToAny(result);
      break;
    }
    case IncomingRequest::CALL_HIERARCHY_OUTGOING_CALLS: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<CallHierarchyOutgoingCallsParams> requestParams =
        transformer.asCallHierarchyOutgoingCallsParams(messageParams);
      CallHierarchyOutgoingCallsResult result =
        handleCallHierarchyOutgoingCalls(*requestParams);
      response.result = transformer.callHierarchyOutgoingCallsResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_SEMANTIC_TOKENS_FULL: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<SemanticTokensParams> requestParams =
        transformer.asTextDocumentSemanticTokensFullParams(messageParams);
      TextDocumentSemanticTokensFullResult result =
        handleTextDocumentSemanticTokensFull(*requestParams);
      response.result = transformer.textDocumentSemanticTokensFullResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_SEMANTIC_TOKENS_FULL_DELTA: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<SemanticTokensDeltaParams> requestParams =
        transformer.asTextDocumentSemanticTokensFullDeltaParams(messageParams);
      TextDocumentSemanticTokensFullDeltaResult result =
        handleTextDocumentSemanticTokensFullDelta(*requestParams);
      response.result = transformer.textDocumentSemanticTokensFullDeltaResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_SEMANTIC_TOKENS_RANGE: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<SemanticTokensRangeParams> requestParams =
        transformer.asTextDocumentSemanticTokensRangeParams(messageParams);
      TextDocumentSemanticTokensRangeResult result =
        handleTextDocumentSemanticTokensRange(*requestParams);
      response.result = transformer.textDocumentSemanticTokensRangeResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_LINKED_EDITING_RANGE: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<LinkedEditingRangeParams> requestParams =
        transformer.asTextDocumentLinkedEditingRangeParams(messageParams);
      TextDocumentLinkedEditingRangeResult result =
        handleTextDocumentLinkedEditingRange(*requestParams);
      response.result = transformer.textDocumentLinkedEditingRangeResultToAny(result);
      break;
    }
    case IncomingRequest::WORKSPACE_WILL_CREATE_FILES: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<CreateFilesParams> requestParams =
        transformer.asWorkspaceWillCreateFilesParams(messageParams);
      WorkspaceWillCreateFilesResult result =
        handleWorkspaceWillCreateFiles(*requestParams);
      response.result = transformer.workspaceWillCreateFilesResultToAny(result);
      break;
    }
    case IncomingRequest::WORKSPACE_WILL_RENAME_FILES: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<RenameFilesParams> requestParams =
        transformer.asWorkspaceWillRenameFilesParams(messageParams);
      WorkspaceWillRenameFilesResult result =
        handleWorkspaceWillRenameFiles(*requestParams);
      response.result = transformer.workspaceWillRenameFilesResultToAny(result);
      break;
    }
    case IncomingRequest::WORKSPACE_WILL_DELETE_FILES: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<DeleteFilesParams> requestParams =
        transformer.asWorkspaceWillDeleteFilesParams(messageParams);
      WorkspaceWillDeleteFilesResult result =
        handleWorkspaceWillDeleteFiles(*requestParams);
      response.result = transformer.workspaceWillDeleteFilesResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_MONIKER: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<MonikerParams> requestParams =
        transformer.asTextDocumentMonikerParams(messageParams);
      TextDocumentMonikerResult result =
        handleTextDocumentMoniker(*requestParams);
      response.result = transformer.textDocumentMonikerResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_PREPARE_TYPE_HIERARCHY: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<TypeHierarchyPrepareParams> requestParams =
        transformer.asTextDocumentPrepareTypeHierarchyParams(messageParams);
      TextDocumentPrepareTypeHierarchyResult result =
        handleTextDocumentPrepareTypeHierarchy(*requestParams);
      response.result = transformer.textDocumentPrepareTypeHierarchyResultToAny(result);
      break;
    }
    case IncomingRequest::TYPE_HIERARCHY_SUPERTYPES: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<TypeHierarchySupertypesParams> requestParams =
        transformer.asTypeHierarchySupertypesParams(messageParams);
      TypeHierarchySupertypesResult result =
        handleTypeHierarchySupertypes(*requestParams);
      response.result = transformer.typeHierarchySupertypesResultToAny(result);
      break;
    }
    case IncomingRequest::TYPE_HIERARCHY_SUBTYPES: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<TypeHierarchySubtypesParams> requestParams =
        transformer.asTypeHierarchySubtypesParams(messageParams);
      TypeHierarchySubtypesResult result =
        handleTypeHierarchySubtypes(*requestParams);
      response.result = transformer.typeHierarchySubtypesResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_INLINE_VALUE: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<InlineValueParams> requestParams =
        transformer.asTextDocumentInlineValueParams(messageParams);
      TextDocumentInlineValueResult result =
        handleTextDocumentInlineValue(*requestParams);
      response.result = transformer.textDocumentInlineValueResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_INLAY_HINT: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<InlayHintParams> requestParams =
        transformer.asTextDocumentInlayHintParams(messageParams);
      TextDocumentInlayHintResult result =
        handleTextDocumentInlayHint(*requestParams);
      response.result = transformer.textDocumentInlayHintResultToAny(result);
      break;
    }
    case IncomingRequest::INLAY_HINT_RESOLVE: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<InlayHint> requestParams =
        transformer.asInlayHintResolveParams(messageParams);
      InlayHintResolveResult result =
        handleInlayHintResolve(*requestParams);
      response.result = transformer.inlayHintResolveResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_DIAGNOSTIC: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<DocumentDiagnosticParams> requestParams =
        transformer.asTextDocumentDiagnosticParams(messageParams);
      TextDocumentDiagnosticResult result =
        handleTextDocumentDiagnostic(*requestParams);
      response.result = transformer.textDocumentDiagnosticResultToAny(result);
      break;
    }
    case IncomingRequest::WORKSPACE_DIAGNOSTIC: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<WorkspaceDiagnosticParams> requestParams =
        transformer.asWorkspaceDiagnosticParams(messageParams);
      WorkspaceDiagnosticResult result =
        handleWorkspaceDiagnostic(*requestParams);
      response.result = transformer.workspaceDiagnosticResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_INLINE_COMPLETION: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<InlineCompletionParams> requestParams =
        transformer.asTextDocumentInlineCompletionParams(messageParams);
      TextDocumentInlineCompletionResult result =
        handleTextDocumentInlineCompletion(*requestParams);
      response.result = transformer.textDocumentInlineCompletionResultToAny(result);
      break;
    }
    case IncomingRequest::INITIALIZE: {
      try {
        MessageParams &messageParams = requireMessageParams(request);
        std::unique_ptr<InitializeParams> requestParams =
          transformer.asInitializeParams(messageParams);
        InitializeResult result =
          handleInitialize(*requestParams);
        response.result = transformer.initializeResultToAny(result);
        _initializeParams = std::move(requestParams);
      } catch (LspException &e) {
        bool expected = true;
        if (!_initialized.compare_exchange_strong(expected, false)) {
          throw LspException(
            ErrorCodes::INVALID_REQUEST,
            "Server initialization out of sync."
          );
        }
        throw e;
      }
      break;
    }
    case IncomingRequest::SHUTDOWN: {
      ShutdownResult result = handleShutdown();
      response.result = transformer.shutdownResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_WILL_SAVE_WAIT_UNTIL: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<WillSaveTextDocumentParams> requestParams =
        transformer.asTextDocumentWillSaveWaitUntilParams(messageParams);
      TextDocumentWillSaveWaitUntilResult result =
        handleTextDocumentWillSaveWaitUntil(*requestParams);
      response.result = transformer.textDocumentWillSaveWaitUntilResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_COMPLETION: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<CompletionParams> requestParams =
        transformer.asTextDocumentCompletionParams(messageParams);
      TextDocumentCompletionResult result =
        handleTextDocumentCompletion(*requestParams);
      response.result = transformer.textDocumentCompletionResultToAny(result);
      break;
    }
    case IncomingRequest::COMPLETION_ITEM_RESOLVE: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<CompletionItem> requestParams =
        transformer.asCompletionItemResolveParams(messageParams);
      CompletionItemResolveResult result =
        handleCompletionItemResolve(*requestParams);
      response.result = transformer.completionItemResolveResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_HOVER: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<HoverParams> requestParams =
        transformer.asTextDocumentHoverParams(messageParams);
      TextDocumentHoverResult result =
        handleTextDocumentHover(*requestParams);
      response.result = transformer.textDocumentHoverResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_SIGNATURE_HELP: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<SignatureHelpParams> requestParams =
        transformer.asTextDocumentSignatureHelpParams(messageParams);
      TextDocumentSignatureHelpResult result =
        handleTextDocumentSignatureHelp(*requestParams);
      response.result = transformer.textDocumentSignatureHelpResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_DEFINITION: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<DefinitionParams> requestParams =
        transformer.asTextDocumentDefinitionParams(messageParams);
      TextDocumentDefinitionResult result =
        handleTextDocumentDefinition(*requestParams);
      response.result = transformer.textDocumentDefinitionResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_REFERENCES: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<ReferenceParams> requestParams =
        transformer.asTextDocumentReferencesParams(messageParams);
      TextDocumentReferencesResult result =
        handleTextDocumentReferences(*requestParams);
      response.result = transformer.textDocumentReferencesResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_DOCUMENT_HIGHLIGHT: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<DocumentHighlightParams> requestParams =
        transformer.asTextDocumentDocumentHighlightParams(messageParams);
      TextDocumentDocumentHighlightResult result =
        handleTextDocumentDocumentHighlight(*requestParams);
      response.result = transformer.textDocumentDocumentHighlightResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_DOCUMENT_SYMBOL: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<DocumentSymbolParams> requestParams =
        transformer.asTextDocumentDocumentSymbolParams(messageParams);
      TextDocumentDocumentSymbolResult result =
        handleTextDocumentDocumentSymbol(*requestParams);
      response.result = transformer.textDocumentDocumentSymbolResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_CODE_ACTION: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<CodeActionParams> requestParams =
        transformer.asTextDocumentCodeActionParams(messageParams);
      TextDocumentCodeActionResult result =
        handleTextDocumentCodeAction(*requestParams);
      response.result = transformer.textDocumentCodeActionResultToAny(result);
      break;
    }
    case IncomingRequest::CODE_ACTION_RESOLVE: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<CodeAction> requestParams =
        transformer.asCodeActionResolveParams(messageParams);
      CodeActionResolveResult result =
        handleCodeActionResolve(*requestParams);
      response.result = transformer.codeActionResolveResultToAny(result);
      break;
    }
    case IncomingRequest::WORKSPACE_SYMBOL: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<WorkspaceSymbolParams> requestParams =
        transformer.asWorkspaceSymbolParams(messageParams);
      WorkspaceSymbolResult result =
        handleWorkspaceSymbol(*requestParams);
      response.result = transformer.workspaceSymbolResultToAny(result);
      break;
    }
    case IncomingRequest::WORKSPACE_SYMBOL_RESOLVE: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<WorkspaceSymbol> requestParams =
        transformer.asWorkspaceSymbolResolveParams(messageParams);
      WorkspaceSymbolResolveResult result =
        handleWorkspaceSymbolResolve(*requestParams);
      response.result = transformer.workspaceSymbolResolveResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_CODE_LENS: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<CodeLensParams> requestParams =
        transformer.asTextDocumentCodeLensParams(messageParams);
      TextDocumentCodeLensResult result =
        handleTextDocumentCodeLens(*requestParams);
      response.result = transformer.textDocumentCodeLensResultToAny(result);
      break;
    }
    case IncomingRequest::CODE_LENS_RESOLVE: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<CodeLens> requestParams =
        transformer.asCodeLensResolveParams(messageParams);
      CodeLensResolveResult result =
        handleCodeLensResolve(*requestParams);
      response.result = transformer.codeLensResolveResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_DOCUMENT_LINK: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<DocumentLinkParams> requestParams =
        transformer.asTextDocumentDocumentLinkParams(messageParams);
      TextDocumentDocumentLinkResult result =
        handleTextDocumentDocumentLink(*requestParams);
      response.result = transformer.textDocumentDocumentLinkResultToAny(result);
      break;
    }
    case IncomingRequest::DOCUMENT_LINK_RESOLVE: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<DocumentLink> requestParams =
        transformer.asDocumentLinkResolveParams(messageParams);
      DocumentLinkResolveResult result =
        handleDocumentLinkResolve(*requestParams);
      response.result = transformer.documentLinkResolveResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_FORMATTING: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<DocumentFormattingParams> requestParams =
        transformer.asTextDocumentFormattingParams(messageParams);
      TextDocumentFormattingResult result =
        handleTextDocumentFormatting(*requestParams);
      response.result = transformer.textDocumentFormattingResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_RANGE_FORMATTING: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<DocumentRangeFormattingParams> requestParams =
        transformer.asTextDocumentRangeFormattingParams(messageParams);
      TextDocumentRangeFormattingResult result =
        handleTextDocumentRangeFormatting(*requestParams);
      response.result = transformer.textDocumentRangeFormattingResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_RANGES_FORMATTING: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<DocumentRangesFormattingParams> requestParams =
        transformer.asTextDocumentRangesFormattingParams(messageParams);
      TextDocumentRangesFormattingResult result =
        handleTextDocumentRangesFormatting(*requestParams);
      response.result = transformer.textDocumentRangesFormattingResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_ON_TYPE_FORMATTING: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<DocumentOnTypeFormattingParams> requestParams =
        transformer.asTextDocumentOnTypeFormattingParams(messageParams);
      TextDocumentOnTypeFormattingResult result =
        handleTextDocumentOnTypeFormatting(*requestParams);
      response.result = transformer.textDocumentOnTypeFormattingResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_RENAME: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<RenameParams> requestParams =
        transformer.asTextDocumentRenameParams(messageParams);
      TextDocumentRenameResult result =
        handleTextDocumentRename(*requestParams);
      response.result = transformer.textDocumentRenameResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_PREPARE_RENAME: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<PrepareRenameParams> requestParams =
        transformer.asTextDocumentPrepareRenameParams(messageParams);
      TextDocumentPrepareRenameResult result =
        handleTextDocumentPrepareRename(*requestParams);
      response.result = transformer.textDocumentPrepareRenameResultToAny(result);
      break;
    }
    case IncomingRequest::WORKSPACE_EXECUTE_COMMAND: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<ExecuteCommandParams> requestParams =
        transformer.asWorkspaceExecuteCommandParams(messageParams);
      WorkspaceExecuteCommandResult result =
        handleWorkspaceExecuteCommand(*requestParams);
      response.result = transformer.workspaceExecuteCommandResultToAny(result);
      break;
    }
    default: {
    invalidMethod:
      throw LspException(
        ErrorCodes::METHOD_NOT_FOUND,
        std::format(
          "Unsupported request method: \"{}\"",
          request.method
        )
      );
    }
    }
  }

  auto LspLanguageServer::dispatch(
    ResponseMessage &response,
    NotificationMessage &notification
  ) -> void {
    IncomingNotification method;
    try {
      method = incomingNotificationByValue(notification.method);
    } catch (std::invalid_argument &e) {
      goto invalidMethod;
    }
    if (method != IncomingNotification::EXIT) {
      if (!_initialized) {
        // Notifications should be dropped, except for the exit notification.
        // This will allow the exit of a server without an initialize request.
        return;
      }
      assertRunning();
    }
    switch (method) {
    case IncomingNotification::WORKSPACE_DID_CHANGE_WORKSPACE_FOLDERS: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<DidChangeWorkspaceFoldersParams> notificationParams =
        transformer.asWorkspaceDidChangeWorkspaceFoldersParams(messageParams);
      handleWorkspaceDidChangeWorkspaceFolders(*notificationParams);
      break;
    }
    case IncomingNotification::WINDOW_WORK_DONE_PROGRESS_CANCEL: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<WorkDoneProgressCancelParams> notificationParams =
        transformer.asWindowWorkDoneProgressCancelParams(messageParams);
      handleWindowWorkDoneProgressCancel(*notificationParams);
      break;
    }
    case IncomingNotification::WORKSPACE_DID_CREATE_FILES: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<CreateFilesParams> notificationParams =
        transformer.asWorkspaceDidCreateFilesParams(messageParams);
      handleWorkspaceDidCreateFiles(*notificationParams);
      break;
    }
    case IncomingNotification::WORKSPACE_DID_RENAME_FILES: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<RenameFilesParams> notificationParams =
        transformer.asWorkspaceDidRenameFilesParams(messageParams);
      handleWorkspaceDidRenameFiles(*notificationParams);
      break;
    }
    case IncomingNotification::WORKSPACE_DID_DELETE_FILES: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<DeleteFilesParams> notificationParams =
        transformer.asWorkspaceDidDeleteFilesParams(messageParams);
      handleWorkspaceDidDeleteFiles(*notificationParams);
      break;
    }
    case IncomingNotification::NOTEBOOK_DOCUMENT_DID_OPEN: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<DidOpenNotebookDocumentParams> notificationParams =
        transformer.asNotebookDocumentDidOpenParams(messageParams);
      handleNotebookDocumentDidOpen(*notificationParams);
      break;
    }
    case IncomingNotification::NOTEBOOK_DOCUMENT_DID_CHANGE: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<DidChangeNotebookDocumentParams> notificationParams =
        transformer.asNotebookDocumentDidChangeParams(messageParams);
      handleNotebookDocumentDidChange(*notificationParams);
      break;
    }
    case IncomingNotification::NOTEBOOK_DOCUMENT_DID_SAVE: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<DidSaveNotebookDocumentParams> notificationParams =
        transformer.asNotebookDocumentDidSaveParams(messageParams);
      handleNotebookDocumentDidSave(*notificationParams);
      break;
    }
    case IncomingNotification::NOTEBOOK_DOCUMENT_DID_CLOSE: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<DidCloseNotebookDocumentParams> notificationParams =
        transformer.asNotebookDocumentDidCloseParams(messageParams);
      handleNotebookDocumentDidClose(*notificationParams);
      break;
    }
    case IncomingNotification::INITIALIZED: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<InitializedParams> notificationParams =
        transformer.asInitializedParams(messageParams);
      handleInitialized(*notificationParams);
      break;
    }
    case IncomingNotification::EXIT: {
      handleExit();
      break;
    }
    case IncomingNotification::WORKSPACE_DID_CHANGE_CONFIGURATION: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<DidChangeConfigurationParams> notificationParams =
        transformer.asWorkspaceDidChangeConfigurationParams(messageParams);
      handleWorkspaceDidChangeConfiguration(*notificationParams);
      break;
    }
    case IncomingNotification::TEXT_DOCUMENT_DID_OPEN: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<DidOpenTextDocumentParams> notificationParams =
        transformer.asTextDocumentDidOpenParams(messageParams);
      handleTextDocumentDidOpen(*notificationParams);
      break;
    }
    case IncomingNotification::TEXT_DOCUMENT_DID_CHANGE: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<DidChangeTextDocumentParams> notificationParams =
        transformer.asTextDocumentDidChangeParams(messageParams);
      handleTextDocumentDidChange(*notificationParams);
      break;
    }
    case IncomingNotification::TEXT_DOCUMENT_DID_CLOSE: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<DidCloseTextDocumentParams> notificationParams =
        transformer.asTextDocumentDidCloseParams(messageParams);
      handleTextDocumentDidClose(*notificationParams);
      break;
    }
    case IncomingNotification::TEXT_DOCUMENT_DID_SAVE: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<DidSaveTextDocumentParams> notificationParams =
        transformer.asTextDocumentDidSaveParams(messageParams);
      handleTextDocumentDidSave(*notificationParams);
      break;
    }
    case IncomingNotification::TEXT_DOCUMENT_WILL_SAVE: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<WillSaveTextDocumentParams> notificationParams =
        transformer.asTextDocumentWillSaveParams(messageParams);
      handleTextDocumentWillSave(*notificationParams);
      break;
    }
    case IncomingNotification::WORKSPACE_DID_CHANGE_WATCHED_FILES: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<DidChangeWatchedFilesParams> notificationParams =
        transformer.asWorkspaceDidChangeWatchedFilesParams(messageParams);
      handleWorkspaceDidChangeWatchedFiles(*notificationParams);
      break;
    }
    case IncomingNotification::SET_TRACE: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<SetTraceParams> notificationParams =
        transformer.asSetTraceParams(messageParams);
      handleSetTrace(*notificationParams);
      break;
    }
    default: {
    invalidMethod:
      throw LspException(
        ErrorCodes::METHOD_NOT_FOUND,
        std::format(
          "Unsupported notification method: \"{}\"",
          notification.method
        )
      );
    }
    }
  }
  auto LspLanguageServer::requireMessageParams(
    RequestMessage &request
  ) const -> MessageParams & {
    if (request.params.has_value()) {
      return request.params.value();
    }
    throw LspException(
      ErrorCodes::INVALID_PARAMS,
      std::format(
        "RequestMessage.params must be defined for method=\"{}\"",
        request.method
      )
    );
  }

  auto LspLanguageServer::requireMessageParams(
    NotificationMessage &notification
  ) const -> MessageParams & {
    if (notification.params.has_value()) {
      return notification.params.value();
    }
    throw LspException(
      ErrorCodes::INVALID_PARAMS,
      std::format(
        "NotificationMessage.params must be defined for method=\"{}\"",
        notification.method
      )
    );
  }

  // ================= //
  // Incoming Requests //
  // ================= //

  // request: "textDocument/implementation"
  auto LspLanguageServer::handleTextDocumentImplementation(
    ImplementationParams &params
  ) -> TextDocumentImplementationResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/implementation\""
    );
  }

  // request: "textDocument/typeDefinition"
  auto LspLanguageServer::handleTextDocumentTypeDefinition(
    TypeDefinitionParams &params
  ) -> TextDocumentTypeDefinitionResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/typeDefinition\""
    );
  }

  // request: "textDocument/documentColor"
  auto LspLanguageServer::handleTextDocumentDocumentColor(
    DocumentColorParams &params
  ) -> TextDocumentDocumentColorResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/documentColor\""
    );
  }

  // request: "textDocument/colorPresentation"
  auto LspLanguageServer::handleTextDocumentColorPresentation(
    ColorPresentationParams &params
  ) -> TextDocumentColorPresentationResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/colorPresentation\""
    );
  }

  // request: "textDocument/foldingRange"
  auto LspLanguageServer::handleTextDocumentFoldingRange(
    FoldingRangeParams &params
  ) -> TextDocumentFoldingRangeResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/foldingRange\""
    );
  }

  // request: "textDocument/declaration"
  auto LspLanguageServer::handleTextDocumentDeclaration(
    DeclarationParams &params
  ) -> TextDocumentDeclarationResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/declaration\""
    );
  }

  // request: "textDocument/selectionRange"
  auto LspLanguageServer::handleTextDocumentSelectionRange(
    SelectionRangeParams &params
  ) -> TextDocumentSelectionRangeResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/selectionRange\""
    );
  }

  // request: "textDocument/prepareCallHierarchy"
  auto LspLanguageServer::handleTextDocumentPrepareCallHierarchy(
    CallHierarchyPrepareParams &params
  ) -> TextDocumentPrepareCallHierarchyResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/prepareCallHierarchy\""
    );
  }

  // request: "callHierarchy/incomingCalls"
  auto LspLanguageServer::handleCallHierarchyIncomingCalls(
    CallHierarchyIncomingCallsParams &params
  ) -> CallHierarchyIncomingCallsResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"callHierarchy/incomingCalls\""
    );
  }

  // request: "callHierarchy/outgoingCalls"
  auto LspLanguageServer::handleCallHierarchyOutgoingCalls(
    CallHierarchyOutgoingCallsParams &params
  ) -> CallHierarchyOutgoingCallsResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"callHierarchy/outgoingCalls\""
    );
  }

  // request: "textDocument/semanticTokens/full"
  auto LspLanguageServer::handleTextDocumentSemanticTokensFull(
    SemanticTokensParams &params
  ) -> TextDocumentSemanticTokensFullResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/semanticTokens/full\""
    );
  }

  // request: "textDocument/semanticTokens/full/delta"
  auto LspLanguageServer::handleTextDocumentSemanticTokensFullDelta(
    SemanticTokensDeltaParams &params
  ) -> TextDocumentSemanticTokensFullDeltaResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/semanticTokens/full/delta\""
    );
  }

  // request: "textDocument/semanticTokens/range"
  auto LspLanguageServer::handleTextDocumentSemanticTokensRange(
    SemanticTokensRangeParams &params
  ) -> TextDocumentSemanticTokensRangeResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/semanticTokens/range\""
    );
  }

  // request: "textDocument/linkedEditingRange"
  auto LspLanguageServer::handleTextDocumentLinkedEditingRange(
    LinkedEditingRangeParams &params
  ) -> TextDocumentLinkedEditingRangeResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/linkedEditingRange\""
    );
  }

  // request: "workspace/willCreateFiles"
  auto LspLanguageServer::handleWorkspaceWillCreateFiles(
    CreateFilesParams &params
  ) -> WorkspaceWillCreateFilesResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"workspace/willCreateFiles\""
    );
  }

  // request: "workspace/willRenameFiles"
  auto LspLanguageServer::handleWorkspaceWillRenameFiles(
    RenameFilesParams &params
  ) -> WorkspaceWillRenameFilesResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"workspace/willRenameFiles\""
    );
  }

  // request: "workspace/willDeleteFiles"
  auto LspLanguageServer::handleWorkspaceWillDeleteFiles(
    DeleteFilesParams &params
  ) -> WorkspaceWillDeleteFilesResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"workspace/willDeleteFiles\""
    );
  }

  // request: "textDocument/moniker"
  auto LspLanguageServer::handleTextDocumentMoniker(
    MonikerParams &params
  ) -> TextDocumentMonikerResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/moniker\""
    );
  }

  // request: "textDocument/prepareTypeHierarchy"
  auto LspLanguageServer::handleTextDocumentPrepareTypeHierarchy(
    TypeHierarchyPrepareParams &params
  ) -> TextDocumentPrepareTypeHierarchyResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/prepareTypeHierarchy\""
    );
  }

  // request: "typeHierarchy/supertypes"
  auto LspLanguageServer::handleTypeHierarchySupertypes(
    TypeHierarchySupertypesParams &params
  ) -> TypeHierarchySupertypesResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"typeHierarchy/supertypes\""
    );
  }

  // request: "typeHierarchy/subtypes"
  auto LspLanguageServer::handleTypeHierarchySubtypes(
    TypeHierarchySubtypesParams &params
  ) -> TypeHierarchySubtypesResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"typeHierarchy/subtypes\""
    );
  }

  // request: "textDocument/inlineValue"
  auto LspLanguageServer::handleTextDocumentInlineValue(
    InlineValueParams &params
  ) -> TextDocumentInlineValueResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/inlineValue\""
    );
  }

  // request: "textDocument/inlayHint"
  auto LspLanguageServer::handleTextDocumentInlayHint(
    InlayHintParams &params
  ) -> TextDocumentInlayHintResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/inlayHint\""
    );
  }

  // request: "inlayHint/resolve"
  auto LspLanguageServer::handleInlayHintResolve(
    InlayHint &params
  ) -> InlayHintResolveResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"inlayHint/resolve\""
    );
  }

  // request: "textDocument/diagnostic"
  auto LspLanguageServer::handleTextDocumentDiagnostic(
    DocumentDiagnosticParams &params
  ) -> TextDocumentDiagnosticResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/diagnostic\""
    );
  }

  // request: "workspace/diagnostic"
  auto LspLanguageServer::handleWorkspaceDiagnostic(
    WorkspaceDiagnosticParams &params
  ) -> WorkspaceDiagnosticResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"workspace/diagnostic\""
    );
  }

  // request: "textDocument/inlineCompletion"
  auto LspLanguageServer::handleTextDocumentInlineCompletion(
    InlineCompletionParams &params
  ) -> TextDocumentInlineCompletionResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/inlineCompletion\""
    );
  }

  // request: "initialize"
  auto LspLanguageServer::handleInitialize(
    InitializeParams &params
  ) -> InitializeResult {
    InitializeResult result;

    std::unique_ptr<ServerCapabilities> capabilities =
      std::make_unique<ServerCapabilities>();

    // ------------------------- //
    // TextDocument Sync Options //
    // ------------------------- //
    ServerCapabilities_textDocumentSync textDocumentSync;
    std::unique_ptr<TextDocumentSyncOptions> textDocumentSyncOptions =
      std::make_unique<TextDocumentSyncOptions>();
    textDocumentSyncOptions->openClose = true;
    textDocumentSyncOptions->change = TextDocumentSyncKind::INCREMENTAL;
    TextDocumentSyncOptions_save save;
    std::unique_ptr<SaveOptions> saveOptions = std::make_unique<SaveOptions>();
    saveOptions->includeText = true;
    save = std::move(saveOptions);
    textDocumentSyncOptions->save = std::move(save);
    textDocumentSync = std::move(textDocumentSyncOptions);
    capabilities->textDocumentSync = std::move(textDocumentSync);
    result.capabilities = std::move(capabilities);

    return result;
  }

  // request: "shutdown"
  auto LspLanguageServer::handleShutdown() -> ShutdownResult {
    bool shutdown = false;
    if (_shutdown.compare_exchange_strong(shutdown, true)) {
      std::cerr << "Shutting down server." << std::endl;
    }
    return nullptr;
  }

  // request: "textDocument/willSaveWaitUntil"
  auto LspLanguageServer::handleTextDocumentWillSaveWaitUntil(
    WillSaveTextDocumentParams &params
  ) -> TextDocumentWillSaveWaitUntilResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/willSaveWaitUntil\""
    );
  }

  // request: "textDocument/completion"
  auto LspLanguageServer::handleTextDocumentCompletion(
    CompletionParams &params
  ) -> TextDocumentCompletionResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/completion\""
    );
  }

  // request: "completionItem/resolve"
  auto LspLanguageServer::handleCompletionItemResolve(
    CompletionItem &params
  ) -> CompletionItemResolveResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"completionItem/resolve\""
    );
  }

  // request: "textDocument/hover"
  auto LspLanguageServer::handleTextDocumentHover(
    HoverParams &params
  ) -> TextDocumentHoverResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/hover\""
    );
  }

  // request: "textDocument/signatureHelp"
  auto LspLanguageServer::handleTextDocumentSignatureHelp(
    SignatureHelpParams &params
  ) -> TextDocumentSignatureHelpResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/signatureHelp\""
    );
  }

  // request: "textDocument/definition"
  auto LspLanguageServer::handleTextDocumentDefinition(
    DefinitionParams &params
  ) -> TextDocumentDefinitionResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/definition\""
    );
  }

  // request: "textDocument/references"
  auto LspLanguageServer::handleTextDocumentReferences(
    ReferenceParams &params
  ) -> TextDocumentReferencesResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/references\""
    );
  }

  // request: "textDocument/documentHighlight"
  auto LspLanguageServer::handleTextDocumentDocumentHighlight(
    DocumentHighlightParams &params
  ) -> TextDocumentDocumentHighlightResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/documentHighlight\""
    );
  }

  // request: "textDocument/documentSymbol"
  auto LspLanguageServer::handleTextDocumentDocumentSymbol(
    DocumentSymbolParams &params
  ) -> TextDocumentDocumentSymbolResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/documentSymbol\""
    );
  }

  // request: "textDocument/codeAction"
  auto LspLanguageServer::handleTextDocumentCodeAction(
    CodeActionParams &params
  ) -> TextDocumentCodeActionResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/codeAction\""
    );
  }

  // request: "codeAction/resolve"
  auto LspLanguageServer::handleCodeActionResolve(
    CodeAction &params
  ) -> CodeActionResolveResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"codeAction/resolve\""
    );
  }

  // request: "workspace/symbol"
  auto LspLanguageServer::handleWorkspaceSymbol(
    WorkspaceSymbolParams &params
  ) -> WorkspaceSymbolResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"workspace/symbol\""
    );
  }

  // request: "workspaceSymbol/resolve"
  auto LspLanguageServer::handleWorkspaceSymbolResolve(
    WorkspaceSymbol &params
  ) -> WorkspaceSymbolResolveResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"workspaceSymbol/resolve\""
    );
  }

  // request: "textDocument/codeLens"
  auto LspLanguageServer::handleTextDocumentCodeLens(
    CodeLensParams &params
  ) -> TextDocumentCodeLensResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/codeLens\""
    );
  }

  // request: "codeLens/resolve"
  auto LspLanguageServer::handleCodeLensResolve(
    CodeLens &params
  ) -> CodeLensResolveResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"codeLens/resolve\""
    );
  }

  // request: "textDocument/documentLink"
  auto LspLanguageServer::handleTextDocumentDocumentLink(
    DocumentLinkParams &params
  ) -> TextDocumentDocumentLinkResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/documentLink\""
    );
  }

  // request: "documentLink/resolve"
  auto LspLanguageServer::handleDocumentLinkResolve(
    DocumentLink &params
  ) -> DocumentLinkResolveResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"documentLink/resolve\""
    );
  }

  // request: "textDocument/formatting"
  auto LspLanguageServer::handleTextDocumentFormatting(
    DocumentFormattingParams &params
  ) -> TextDocumentFormattingResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/formatting\""
    );
  }

  // request: "textDocument/rangeFormatting"
  auto LspLanguageServer::handleTextDocumentRangeFormatting(
    DocumentRangeFormattingParams &params
  ) -> TextDocumentRangeFormattingResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/rangeFormatting\""
    );
  }

  // request: "textDocument/rangesFormatting"
  auto LspLanguageServer::handleTextDocumentRangesFormatting(
    DocumentRangesFormattingParams &params
  ) -> TextDocumentRangesFormattingResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/rangesFormatting\""
    );
  }

  // request: "textDocument/onTypeFormatting"
  auto LspLanguageServer::handleTextDocumentOnTypeFormatting(
    DocumentOnTypeFormattingParams &params
  ) -> TextDocumentOnTypeFormattingResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/onTypeFormatting\""
    );
  }

  // request: "textDocument/rename"
  auto LspLanguageServer::handleTextDocumentRename(
    RenameParams &params
  ) -> TextDocumentRenameResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/rename\""
    );
  }

  // request: "textDocument/prepareRename"
  auto LspLanguageServer::handleTextDocumentPrepareRename(
    PrepareRenameParams &params
  ) -> TextDocumentPrepareRenameResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/prepareRename\""
    );
  }

  // request: "workspace/executeCommand"
  auto LspLanguageServer::handleWorkspaceExecuteCommand(
    ExecuteCommandParams &params
  ) -> WorkspaceExecuteCommandResult {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"workspace/executeCommand\""
    );
  }

  // ====================== //
  // Incoming Notifications //
  // ====================== //

  // notification: "workspace/didChangeWorkspaceFolders"
  auto LspLanguageServer::handleWorkspaceDidChangeWorkspaceFolders(
    DidChangeWorkspaceFoldersParams &params
  ) -> void {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"workspace/didChangeWorkspaceFolders\""
    );
  }

  // notification: "window/workDoneProgress/cancel"
  auto LspLanguageServer::handleWindowWorkDoneProgressCancel(
    WorkDoneProgressCancelParams &params
  ) -> void {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"window/workDoneProgress/cancel\""
    );
  }

  // notification: "workspace/didCreateFiles"
  auto LspLanguageServer::handleWorkspaceDidCreateFiles(
    CreateFilesParams &params
  ) -> void {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"workspace/didCreateFiles\""
    );
  }

  // notification: "workspace/didRenameFiles"
  auto LspLanguageServer::handleWorkspaceDidRenameFiles(
    RenameFilesParams &params
  ) -> void {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"workspace/didRenameFiles\""
    );
  }

  // notification: "workspace/didDeleteFiles"
  auto LspLanguageServer::handleWorkspaceDidDeleteFiles(
    DeleteFilesParams &params
  ) -> void {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"workspace/didDeleteFiles\""
    );
  }

  // notification: "notebookDocument/didOpen"
  auto LspLanguageServer::handleNotebookDocumentDidOpen(
    DidOpenNotebookDocumentParams &params
  ) -> void {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"notebookDocument/didOpen\""
    );
  }

  // notification: "notebookDocument/didChange"
  auto LspLanguageServer::handleNotebookDocumentDidChange(
    DidChangeNotebookDocumentParams &params
  ) -> void {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"notebookDocument/didChange\""
    );
  }

  // notification: "notebookDocument/didSave"
  auto LspLanguageServer::handleNotebookDocumentDidSave(
    DidSaveNotebookDocumentParams &params
  ) -> void {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"notebookDocument/didSave\""
    );
  }

  // notification: "notebookDocument/didClose"
  auto LspLanguageServer::handleNotebookDocumentDidClose(
    DidCloseNotebookDocumentParams &params
  ) -> void {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"notebookDocument/didClose\""
    );
  }

  // notification: "initialized"
  auto LspLanguageServer::handleInitialized(
    InitializedParams &params
  ) -> void {
    // empty
  }

  // notification: "exit"
  auto LspLanguageServer::handleExit() -> void {
    bool exit = false;
    if (_exit.compare_exchange_strong(exit, true)) {
      std::cerr << "Exiting server." << std::endl;
      bool shutdown = false;
      if (_shutdown.compare_exchange_strong(shutdown, true)) {
        std::cerr
          << "Server exited before being notified to shutdown!"
          << std::endl;
      }
    }
  }

  // notification: "workspace/didChangeConfiguration"
  auto LspLanguageServer::handleWorkspaceDidChangeConfiguration(
    DidChangeConfigurationParams &params
  ) -> void {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"workspace/didChangeConfiguration\""
    );
  }

  // notification: "textDocument/didOpen"
  auto LspLanguageServer::handleTextDocumentDidOpen(
    DidOpenTextDocumentParams &params
  ) -> void {
    const TextDocumentItem &textDocumentItem = *params.textDocument;
    const DocumentUri &uri = textDocumentItem.uri;
    const std::string &text = textDocumentItem.text;
    {
      std::unique_lock<std::shared_mutex> writeLock(readWriteMutex);
      textDocuments.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(uri),
        std::forward_as_tuple(uri, text)
      );
    }
  }

  // notification: "textDocument/didChange"
  auto LspLanguageServer::handleTextDocumentDidChange(
    DidChangeTextDocumentParams &params
  ) -> void {
    const DocumentUri &uri = params.textDocument->uri;
    {
      std::shared_lock<std::shared_mutex> readLock(readWriteMutex);
      TextDocument &textDocument = textDocuments.at(uri);
      readLock.unlock();
      textDocument.apply(params.contentChanges);
    }
  }

  // notification: "textDocument/didClose"
  auto LspLanguageServer::handleTextDocumentDidClose(
    DidCloseTextDocumentParams &params
  ) -> void {
    const DocumentUri &uri = params.textDocument->uri;
    {
      std::shared_lock<std::shared_mutex> readLock(readWriteMutex);
      auto pos = textDocuments.find(uri);
      readLock.unlock();
      if (pos != textDocuments.end()) {
        std::unique_lock<std::shared_mutex> writeLock(readWriteMutex);
        pos = textDocuments.find(uri);
        if (pos != textDocuments.end()) {
          textDocuments.erase(pos);
        }
      }
    }
  }

  // notification: "textDocument/didSave"
  auto LspLanguageServer::handleTextDocumentDidSave(
    DidSaveTextDocumentParams &params
  ) -> void {
    if (params.text.has_value()) {
      const std::string &text = params.text.value();
      const DocumentUri &uri = params.textDocument->uri;
      {
        std::shared_lock<std::shared_mutex> readLock(readWriteMutex);
        TextDocument &textDocument = textDocuments.at(uri);
        readLock.unlock();
        textDocument.setText(text);
      }
    }
  }

  // notification: "textDocument/willSave"
  auto LspLanguageServer::handleTextDocumentWillSave(
    WillSaveTextDocumentParams &params
  ) -> void {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/willSave\""
    );
  }

  // notification: "workspace/didChangeWatchedFiles"
  auto LspLanguageServer::handleWorkspaceDidChangeWatchedFiles(
    DidChangeWatchedFilesParams &params
  ) -> void {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"workspace/didChangeWatchedFiles\""
    );
  }

  // notification: "$/setTrace"
  auto LspLanguageServer::handleSetTrace(
    SetTraceParams &params
  ) -> void {
    throw LspException(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"$/setTrace\""
    );
  }

  // ================= //
  // Outgoing Requests //
  // ================= //

  // request: "workspace/workspaceFolders"
  auto LspLanguageServer::requestWorkspaceWorkspaceFolders() -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    request.id = nextId();
    request.method = "workspace/workspaceFolders";
    const std::string message = serializer.serializeRequest(request);
    outgoingMessages.enqueue(message);
  }

  // request: "workspace/configuration"
  auto LspLanguageServer::requestWorkspaceConfiguration(
    ConfigurationParams &params
  ) -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    request.id = nextId();
    request.method = "workspace/configuration";
    request.params = transformer.asMessageParams(params);
    const std::string message = serializer.serializeRequest(request);
    outgoingMessages.enqueue(message);
  }

  // request: "workspace/foldingRange/refresh"
  auto LspLanguageServer::requestWorkspaceFoldingRangeRefresh() -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    request.id = nextId();
    request.method = "workspace/foldingRange/refresh";
    const std::string message = serializer.serializeRequest(request);
    outgoingMessages.enqueue(message);
  }

  // request: "window/workDoneProgress/create"
  auto LspLanguageServer::requestWindowWorkDoneProgressCreate(
    WorkDoneProgressCreateParams &params
  ) -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    request.id = nextId();
    request.method = "window/workDoneProgress/create";
    request.params = transformer.asMessageParams(params);
    const std::string message = serializer.serializeRequest(request);
    outgoingMessages.enqueue(message);
  }

  // request: "workspace/semanticTokens/refresh"
  auto LspLanguageServer::requestWorkspaceSemanticTokensRefresh() -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    request.id = nextId();
    request.method = "workspace/semanticTokens/refresh";
    const std::string message = serializer.serializeRequest(request);
    outgoingMessages.enqueue(message);
  }

  // request: "window/showDocument"
  auto LspLanguageServer::requestWindowShowDocument(
    ShowDocumentParams &params
  ) -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    request.id = nextId();
    request.method = "window/showDocument";
    request.params = transformer.asMessageParams(params);
    const std::string message = serializer.serializeRequest(request);
    outgoingMessages.enqueue(message);
  }

  // request: "workspace/inlineValue/refresh"
  auto LspLanguageServer::requestWorkspaceInlineValueRefresh() -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    request.id = nextId();
    request.method = "workspace/inlineValue/refresh";
    const std::string message = serializer.serializeRequest(request);
    outgoingMessages.enqueue(message);
  }

  // request: "workspace/inlayHint/refresh"
  auto LspLanguageServer::requestWorkspaceInlayHintRefresh() -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    request.id = nextId();
    request.method = "workspace/inlayHint/refresh";
    const std::string message = serializer.serializeRequest(request);
    outgoingMessages.enqueue(message);
  }

  // request: "workspace/diagnostic/refresh"
  auto LspLanguageServer::requestWorkspaceDiagnosticRefresh() -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    request.id = nextId();
    request.method = "workspace/diagnostic/refresh";
    const std::string message = serializer.serializeRequest(request);
    outgoingMessages.enqueue(message);
  }

  // request: "client/registerCapability"
  auto LspLanguageServer::requestClientRegisterCapability(
    RegistrationParams &params
  ) -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    request.id = nextId();
    request.method = "client/registerCapability";
    request.params = transformer.asMessageParams(params);
    const std::string message = serializer.serializeRequest(request);
    outgoingMessages.enqueue(message);
  }

  // request: "client/unregisterCapability"
  auto LspLanguageServer::requestClientUnregisterCapability(
    UnregistrationParams &params
  ) -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    request.id = nextId();
    request.method = "client/unregisterCapability";
    request.params = transformer.asMessageParams(params);
    const std::string message = serializer.serializeRequest(request);
    outgoingMessages.enqueue(message);
  }

  // request: "window/showMessageRequest"
  auto LspLanguageServer::requestWindowShowMessageRequest(
    ShowMessageRequestParams &params
  ) -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    request.id = nextId();
    request.method = "window/showMessageRequest";
    request.params = transformer.asMessageParams(params);
    const std::string message = serializer.serializeRequest(request);
    outgoingMessages.enqueue(message);
  }

  // request: "workspace/codeLens/refresh"
  auto LspLanguageServer::requestWorkspaceCodeLensRefresh() -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    request.id = nextId();
    request.method = "workspace/codeLens/refresh";
    const std::string message = serializer.serializeRequest(request);
    outgoingMessages.enqueue(message);
  }

  // request: "workspace/applyEdit"
  auto LspLanguageServer::requestWorkspaceApplyEdit(
    ApplyWorkspaceEditParams &params
  ) -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    request.id = nextId();
    request.method = "workspace/applyEdit";
    request.params = transformer.asMessageParams(params);
    const std::string message = serializer.serializeRequest(request);
    outgoingMessages.enqueue(message);
  }

  // ====================== //
  // Outgoing Notifications //
  // ====================== //

  // notification: "window/showMessage"
  auto LspLanguageServer::notifyWindowShowMessage(
    ShowMessageParams &params
  ) -> void {
    NotificationMessage notification;
    notification.jsonrpc = JSON_RPC_VERSION;
    notification.method = "window/showMessage";
    notification.params = transformer.asMessageParams(params);
    const std::string message = serializer.serializeNotification(notification);
    outgoingMessages.enqueue(message);
  }

  // notification: "window/logMessage"
  auto LspLanguageServer::notifyWindowLogMessage(
    LogMessageParams &params
  ) -> void {
    NotificationMessage notification;
    notification.jsonrpc = JSON_RPC_VERSION;
    notification.method = "window/logMessage";
    notification.params = transformer.asMessageParams(params);
    const std::string message = serializer.serializeNotification(notification);
    outgoingMessages.enqueue(message);
  }

  // notification: "telemetry/event"
  auto LspLanguageServer::notifyTelemetryEvent(
    LSPAny &params
  ) -> void {
    NotificationMessage notification;
    notification.jsonrpc = JSON_RPC_VERSION;
    notification.method = "telemetry/event";
    notification.params = transformer.asMessageParams(params);
    const std::string message = serializer.serializeNotification(notification);
    outgoingMessages.enqueue(message);
  }

  // notification: "textDocument/publishDiagnostics"
  auto LspLanguageServer::notifyTextDocumentPublishDiagnostics(
    PublishDiagnosticsParams &params
  ) -> void {
    NotificationMessage notification;
    notification.jsonrpc = JSON_RPC_VERSION;
    notification.method = "textDocument/publishDiagnostics";
    notification.params = transformer.asMessageParams(params);
    const std::string message = serializer.serializeNotification(notification);
    outgoingMessages.enqueue(message);
  }

  // notification: "$/logTrace"
  auto LspLanguageServer::notifyLogTrace(
    LogTraceParams &params
  ) -> void {
    NotificationMessage notification;
    notification.jsonrpc = JSON_RPC_VERSION;
    notification.method = "$/logTrace";
    notification.params = transformer.asMessageParams(params);
    const std::string message = serializer.serializeNotification(notification);
    outgoingMessages.enqueue(message);
  }

} // namespace LCompilers::LanguageServerProtocol
