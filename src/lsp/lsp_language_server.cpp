// -----------------------------------------------------------------------------
// NOTE: This file was generated from Microsoft's Language Server Protocol (LSP)
// specification. Please do not edit it by hand.
// -----------------------------------------------------------------------------

#include <cctype>
#include <iostream>
#include <stdexcept>

#include <lsp/specification.h>
#include <lsp/lsp_exception.h>
#include <lsp/lsp_json_parser.h>
#include <lsp/lsp_language_server.h>

namespace LCompilers::LanguageServerProtocol {

  LspLanguageServer::LspLanguageServer(
    ls::MessageQueue &incomingMessages,
    ls::MessageQueue &outgoingMessages,
    std::size_t numRequestThreads,
    std::size_t numWorkerThreads,
    lsl::Logger &logger,
    const std::string &configSection
  ) : ls::LanguageServer(
      incomingMessages,
      outgoingMessages,
      numRequestThreads,
      numWorkerThreads,
      logger
    )
    , configSection(configSection)
    , transformer(logger)
  {
    // empty
  }

  auto LspLanguageServer::nextId() -> int {
    return serialId++;
  }

  auto LspLanguageServer::handle(
    const std::string &message,
    std::size_t sendId
  ) -> void {
    ResponseMessage response;
    try {
      // The language server protocol always uses “2.0” as the jsonrpc version.
      response.jsonrpc = JSON_RPC_VERSION;
      response.id = nullptr;

      LspJsonParser parser(message);
      std::unique_ptr<LSPAny> document = parser.parse();
      LSPAnyType documentType = static_cast<LSPAnyType>(document->index());

      if (documentType != LSPAnyType::OBJECT_TYPE) {
        // TODO: Add support for batched messages, i.e. multiple messages within
        // an array.
        if (documentType == LSPAnyType::ARRAY_TYPE) {
          throw LSP_EXCEPTION(
            ErrorCodes::INVALID_PARAMS,
            "Batched requests are not supported (currently)."
          );
        }
        throw LSP_EXCEPTION(
          ErrorCodes::INVALID_PARAMS,
          "Invalid request message: " + message
        );
      }

      const LSPObject &object = std::get<LSPObject>(*document);
      LSPObject::const_iterator iter;

      if ((iter = object.find("id")) != object.end()) {
        response.id = transformer.anyToResponseId(*iter->second);
      }

      if ((iter = object.find("method")) != object.end()) {
        const std::string &method =
          transformer.anyToString(*iter->second);
        if (isIncomingRequest(method)) {
          if (static_cast<ResponseIdType>(response.id.index()) ==
              ResponseIdType::NULL_TYPE) {
            std::stringstream ss;
            ss << "Missing request method=\""
               << method << "\" attribute: id";
            throw LSP_EXCEPTION(ErrorCodes::INVALID_PARAMS, ss.str());
          }
          std::unique_ptr<RequestMessage> request =
            transformer.anyToRequestMessage(*document);
          response.jsonrpc = request->jsonrpc;
          dispatch(response, *request);
        } else if (isIncomingNotification(method)) {
          if (static_cast<ResponseIdType>(response.id.index()) !=
              ResponseIdType::NULL_TYPE) {
            std::stringstream ss;
            ss << "Notification method=\""
               << method
               << "\" must not contain the attribute: id";
            throw LSP_EXCEPTION(ErrorCodes::INVALID_PARAMS, ss.str());
          }
          std::unique_ptr<NotificationMessage> notification =
            transformer.anyToNotificationMessage(*document);
          response.jsonrpc = notification->jsonrpc;
          dispatch(response, *notification);
        } else {
          std::stringstream ss;
          ss << "Unsupported method: \"" << method << "\"";
          throw LSP_EXCEPTION(ErrorCodes::INVALID_REQUEST, ss.str());
        }
      } else if ((iter = object.find("result")) != object.end()) {
        notifySent();
        response.result = transformer.copy(iter->second);
        dispatch(response);
        return;
      } else if ((iter = object.find("error")) != object.end()) {
        notifySent();
        response.error = transformer.anyToResponseError(*iter->second);
        dispatch(response);
        return;
      } else {
        throw LSP_EXCEPTION(
          ErrorCodes::INVALID_REQUEST,
          "Missing required attribute: method"
        );
      }
    } catch (const LspException &e) {
      logger
        << "[" << e.file() << ":" << e.line() << "] "
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
      logger << "Caught unhandled exception: " << e.what() << std::endl;
      std::unique_ptr<ResponseError> error =
        std::make_unique<ResponseError>();
      error->code = static_cast<int>(ErrorCodes::INTERNAL_ERROR);
      error->message =
        "An unexpected exception occurred. If it continues, please file a ticket.";
      response.error = std::move(error);
    }
    std::unique_ptr<LSPAny> any = transformer.responseMessageToAny(response);
    const std::string message = serializer.serialize(*any);
    send(message, sendId);
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
      throw LSP_EXCEPTION(
        ErrorCodes::SERVER_NOT_INITIALIZED,
        "Method \"initialize\" must be called first."
      );
    }
  }

  auto LspLanguageServer::assertRunning() -> void {
    if (_shutdown) {
      throw LSP_EXCEPTION(
        LSPErrorCodes::REQUEST_FAILED,
        "Server has shutdown and cannot accept new requests."
      );
    }
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
        throw LSP_EXCEPTION(
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
        receiveTextDocument_implementation(*requestParams);
      response.result = transformer.textDocumentImplementationResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_TYPE_DEFINITION: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<TypeDefinitionParams> requestParams =
        transformer.asTextDocumentTypeDefinitionParams(messageParams);
      TextDocumentTypeDefinitionResult result =
        receiveTextDocument_typeDefinition(*requestParams);
      response.result = transformer.textDocumentTypeDefinitionResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_DOCUMENT_COLOR: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<DocumentColorParams> requestParams =
        transformer.asTextDocumentDocumentColorParams(messageParams);
      TextDocumentDocumentColorResult result =
        receiveTextDocument_documentColor(*requestParams);
      response.result = transformer.textDocumentDocumentColorResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_COLOR_PRESENTATION: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<ColorPresentationParams> requestParams =
        transformer.asTextDocumentColorPresentationParams(messageParams);
      TextDocumentColorPresentationResult result =
        receiveTextDocument_colorPresentation(*requestParams);
      response.result = transformer.textDocumentColorPresentationResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_FOLDING_RANGE: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<FoldingRangeParams> requestParams =
        transformer.asTextDocumentFoldingRangeParams(messageParams);
      TextDocumentFoldingRangeResult result =
        receiveTextDocument_foldingRange(*requestParams);
      response.result = transformer.textDocumentFoldingRangeResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_DECLARATION: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<DeclarationParams> requestParams =
        transformer.asTextDocumentDeclarationParams(messageParams);
      TextDocumentDeclarationResult result =
        receiveTextDocument_declaration(*requestParams);
      response.result = transformer.textDocumentDeclarationResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_SELECTION_RANGE: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<SelectionRangeParams> requestParams =
        transformer.asTextDocumentSelectionRangeParams(messageParams);
      TextDocumentSelectionRangeResult result =
        receiveTextDocument_selectionRange(*requestParams);
      response.result = transformer.textDocumentSelectionRangeResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_PREPARE_CALL_HIERARCHY: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<CallHierarchyPrepareParams> requestParams =
        transformer.asTextDocumentPrepareCallHierarchyParams(messageParams);
      TextDocumentPrepareCallHierarchyResult result =
        receiveTextDocument_prepareCallHierarchy(*requestParams);
      response.result = transformer.textDocumentPrepareCallHierarchyResultToAny(result);
      break;
    }
    case IncomingRequest::CALL_HIERARCHY_INCOMING_CALLS: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<CallHierarchyIncomingCallsParams> requestParams =
        transformer.asCallHierarchyIncomingCallsParams(messageParams);
      CallHierarchyIncomingCallsResult result =
        receiveCallHierarchy_incomingCalls(*requestParams);
      response.result = transformer.callHierarchyIncomingCallsResultToAny(result);
      break;
    }
    case IncomingRequest::CALL_HIERARCHY_OUTGOING_CALLS: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<CallHierarchyOutgoingCallsParams> requestParams =
        transformer.asCallHierarchyOutgoingCallsParams(messageParams);
      CallHierarchyOutgoingCallsResult result =
        receiveCallHierarchy_outgoingCalls(*requestParams);
      response.result = transformer.callHierarchyOutgoingCallsResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_SEMANTIC_TOKENS_FULL: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<SemanticTokensParams> requestParams =
        transformer.asTextDocumentSemanticTokensFullParams(messageParams);
      TextDocumentSemanticTokensFullResult result =
        receiveTextDocument_semanticTokens_full(*requestParams);
      response.result = transformer.textDocumentSemanticTokensFullResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_SEMANTIC_TOKENS_FULL_DELTA: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<SemanticTokensDeltaParams> requestParams =
        transformer.asTextDocumentSemanticTokensFullDeltaParams(messageParams);
      TextDocumentSemanticTokensFullDeltaResult result =
        receiveTextDocument_semanticTokens_full_delta(*requestParams);
      response.result = transformer.textDocumentSemanticTokensFullDeltaResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_SEMANTIC_TOKENS_RANGE: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<SemanticTokensRangeParams> requestParams =
        transformer.asTextDocumentSemanticTokensRangeParams(messageParams);
      TextDocumentSemanticTokensRangeResult result =
        receiveTextDocument_semanticTokens_range(*requestParams);
      response.result = transformer.textDocumentSemanticTokensRangeResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_LINKED_EDITING_RANGE: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<LinkedEditingRangeParams> requestParams =
        transformer.asTextDocumentLinkedEditingRangeParams(messageParams);
      TextDocumentLinkedEditingRangeResult result =
        receiveTextDocument_linkedEditingRange(*requestParams);
      response.result = transformer.textDocumentLinkedEditingRangeResultToAny(result);
      break;
    }
    case IncomingRequest::WORKSPACE_WILL_CREATE_FILES: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<CreateFilesParams> requestParams =
        transformer.asWorkspaceWillCreateFilesParams(messageParams);
      WorkspaceWillCreateFilesResult result =
        receiveWorkspace_willCreateFiles(*requestParams);
      response.result = transformer.workspaceWillCreateFilesResultToAny(result);
      break;
    }
    case IncomingRequest::WORKSPACE_WILL_RENAME_FILES: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<RenameFilesParams> requestParams =
        transformer.asWorkspaceWillRenameFilesParams(messageParams);
      WorkspaceWillRenameFilesResult result =
        receiveWorkspace_willRenameFiles(*requestParams);
      response.result = transformer.workspaceWillRenameFilesResultToAny(result);
      break;
    }
    case IncomingRequest::WORKSPACE_WILL_DELETE_FILES: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<DeleteFilesParams> requestParams =
        transformer.asWorkspaceWillDeleteFilesParams(messageParams);
      WorkspaceWillDeleteFilesResult result =
        receiveWorkspace_willDeleteFiles(*requestParams);
      response.result = transformer.workspaceWillDeleteFilesResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_MONIKER: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<MonikerParams> requestParams =
        transformer.asTextDocumentMonikerParams(messageParams);
      TextDocumentMonikerResult result =
        receiveTextDocument_moniker(*requestParams);
      response.result = transformer.textDocumentMonikerResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_PREPARE_TYPE_HIERARCHY: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<TypeHierarchyPrepareParams> requestParams =
        transformer.asTextDocumentPrepareTypeHierarchyParams(messageParams);
      TextDocumentPrepareTypeHierarchyResult result =
        receiveTextDocument_prepareTypeHierarchy(*requestParams);
      response.result = transformer.textDocumentPrepareTypeHierarchyResultToAny(result);
      break;
    }
    case IncomingRequest::TYPE_HIERARCHY_SUPERTYPES: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<TypeHierarchySupertypesParams> requestParams =
        transformer.asTypeHierarchySupertypesParams(messageParams);
      TypeHierarchySupertypesResult result =
        receiveTypeHierarchy_supertypes(*requestParams);
      response.result = transformer.typeHierarchySupertypesResultToAny(result);
      break;
    }
    case IncomingRequest::TYPE_HIERARCHY_SUBTYPES: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<TypeHierarchySubtypesParams> requestParams =
        transformer.asTypeHierarchySubtypesParams(messageParams);
      TypeHierarchySubtypesResult result =
        receiveTypeHierarchy_subtypes(*requestParams);
      response.result = transformer.typeHierarchySubtypesResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_INLINE_VALUE: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<InlineValueParams> requestParams =
        transformer.asTextDocumentInlineValueParams(messageParams);
      TextDocumentInlineValueResult result =
        receiveTextDocument_inlineValue(*requestParams);
      response.result = transformer.textDocumentInlineValueResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_INLAY_HINT: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<InlayHintParams> requestParams =
        transformer.asTextDocumentInlayHintParams(messageParams);
      TextDocumentInlayHintResult result =
        receiveTextDocument_inlayHint(*requestParams);
      response.result = transformer.textDocumentInlayHintResultToAny(result);
      break;
    }
    case IncomingRequest::INLAY_HINT_RESOLVE: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<InlayHint> requestParams =
        transformer.asInlayHintResolveParams(messageParams);
      InlayHintResolveResult result =
        receiveInlayHint_resolve(*requestParams);
      response.result = transformer.inlayHintResolveResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_DIAGNOSTIC: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<DocumentDiagnosticParams> requestParams =
        transformer.asTextDocumentDiagnosticParams(messageParams);
      TextDocumentDiagnosticResult result =
        receiveTextDocument_diagnostic(*requestParams);
      response.result = transformer.textDocumentDiagnosticResultToAny(result);
      break;
    }
    case IncomingRequest::WORKSPACE_DIAGNOSTIC: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<WorkspaceDiagnosticParams> requestParams =
        transformer.asWorkspaceDiagnosticParams(messageParams);
      WorkspaceDiagnosticResult result =
        receiveWorkspace_diagnostic(*requestParams);
      response.result = transformer.workspaceDiagnosticResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_INLINE_COMPLETION: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<InlineCompletionParams> requestParams =
        transformer.asTextDocumentInlineCompletionParams(messageParams);
      TextDocumentInlineCompletionResult result =
        receiveTextDocument_inlineCompletion(*requestParams);
      response.result = transformer.textDocumentInlineCompletionResultToAny(result);
      break;
    }
    case IncomingRequest::INITIALIZE: {
      try {
        MessageParams &messageParams = requireMessageParams(request);
        std::unique_ptr<InitializeParams> requestParams =
          transformer.asInitializeParams(messageParams);
        InitializeResult result =
          receiveInitialize(*requestParams);
        response.result = transformer.initializeResultToAny(result);
        _initializeParams = std::move(requestParams);
      } catch (LspException &e) {
        bool expected = true;
        if (!_initialized.compare_exchange_strong(expected, false)) {
          throw LSP_EXCEPTION(
            ErrorCodes::INVALID_REQUEST,
            "Server initialization out of sync."
          );
        }
        throw e;
      }
      break;
    }
    case IncomingRequest::SHUTDOWN: {
      ShutdownResult result = receiveShutdown();
      response.result = transformer.shutdownResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_WILL_SAVE_WAIT_UNTIL: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<WillSaveTextDocumentParams> requestParams =
        transformer.asTextDocumentWillSaveWaitUntilParams(messageParams);
      TextDocumentWillSaveWaitUntilResult result =
        receiveTextDocument_willSaveWaitUntil(*requestParams);
      response.result = transformer.textDocumentWillSaveWaitUntilResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_COMPLETION: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<CompletionParams> requestParams =
        transformer.asTextDocumentCompletionParams(messageParams);
      TextDocumentCompletionResult result =
        receiveTextDocument_completion(*requestParams);
      response.result = transformer.textDocumentCompletionResultToAny(result);
      break;
    }
    case IncomingRequest::COMPLETION_ITEM_RESOLVE: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<CompletionItem> requestParams =
        transformer.asCompletionItemResolveParams(messageParams);
      CompletionItemResolveResult result =
        receiveCompletionItem_resolve(*requestParams);
      response.result = transformer.completionItemResolveResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_HOVER: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<HoverParams> requestParams =
        transformer.asTextDocumentHoverParams(messageParams);
      TextDocumentHoverResult result =
        receiveTextDocument_hover(*requestParams);
      response.result = transformer.textDocumentHoverResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_SIGNATURE_HELP: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<SignatureHelpParams> requestParams =
        transformer.asTextDocumentSignatureHelpParams(messageParams);
      TextDocumentSignatureHelpResult result =
        receiveTextDocument_signatureHelp(*requestParams);
      response.result = transformer.textDocumentSignatureHelpResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_DEFINITION: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<DefinitionParams> requestParams =
        transformer.asTextDocumentDefinitionParams(messageParams);
      TextDocumentDefinitionResult result =
        receiveTextDocument_definition(*requestParams);
      response.result = transformer.textDocumentDefinitionResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_REFERENCES: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<ReferenceParams> requestParams =
        transformer.asTextDocumentReferencesParams(messageParams);
      TextDocumentReferencesResult result =
        receiveTextDocument_references(*requestParams);
      response.result = transformer.textDocumentReferencesResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_DOCUMENT_HIGHLIGHT: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<DocumentHighlightParams> requestParams =
        transformer.asTextDocumentDocumentHighlightParams(messageParams);
      TextDocumentDocumentHighlightResult result =
        receiveTextDocument_documentHighlight(*requestParams);
      response.result = transformer.textDocumentDocumentHighlightResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_DOCUMENT_SYMBOL: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<DocumentSymbolParams> requestParams =
        transformer.asTextDocumentDocumentSymbolParams(messageParams);
      TextDocumentDocumentSymbolResult result =
        receiveTextDocument_documentSymbol(*requestParams);
      response.result = transformer.textDocumentDocumentSymbolResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_CODE_ACTION: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<CodeActionParams> requestParams =
        transformer.asTextDocumentCodeActionParams(messageParams);
      TextDocumentCodeActionResult result =
        receiveTextDocument_codeAction(*requestParams);
      response.result = transformer.textDocumentCodeActionResultToAny(result);
      break;
    }
    case IncomingRequest::CODE_ACTION_RESOLVE: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<CodeAction> requestParams =
        transformer.asCodeActionResolveParams(messageParams);
      CodeActionResolveResult result =
        receiveCodeAction_resolve(*requestParams);
      response.result = transformer.codeActionResolveResultToAny(result);
      break;
    }
    case IncomingRequest::WORKSPACE_SYMBOL: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<WorkspaceSymbolParams> requestParams =
        transformer.asWorkspaceSymbolParams(messageParams);
      WorkspaceSymbolResult result =
        receiveWorkspace_symbol(*requestParams);
      response.result = transformer.workspaceSymbolResultToAny(result);
      break;
    }
    case IncomingRequest::WORKSPACE_SYMBOL_RESOLVE: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<WorkspaceSymbol> requestParams =
        transformer.asWorkspaceSymbolResolveParams(messageParams);
      WorkspaceSymbolResolveResult result =
        receiveWorkspaceSymbol_resolve(*requestParams);
      response.result = transformer.workspaceSymbolResolveResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_CODE_LENS: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<CodeLensParams> requestParams =
        transformer.asTextDocumentCodeLensParams(messageParams);
      TextDocumentCodeLensResult result =
        receiveTextDocument_codeLens(*requestParams);
      response.result = transformer.textDocumentCodeLensResultToAny(result);
      break;
    }
    case IncomingRequest::CODE_LENS_RESOLVE: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<CodeLens> requestParams =
        transformer.asCodeLensResolveParams(messageParams);
      CodeLensResolveResult result =
        receiveCodeLens_resolve(*requestParams);
      response.result = transformer.codeLensResolveResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_DOCUMENT_LINK: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<DocumentLinkParams> requestParams =
        transformer.asTextDocumentDocumentLinkParams(messageParams);
      TextDocumentDocumentLinkResult result =
        receiveTextDocument_documentLink(*requestParams);
      response.result = transformer.textDocumentDocumentLinkResultToAny(result);
      break;
    }
    case IncomingRequest::DOCUMENT_LINK_RESOLVE: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<DocumentLink> requestParams =
        transformer.asDocumentLinkResolveParams(messageParams);
      DocumentLinkResolveResult result =
        receiveDocumentLink_resolve(*requestParams);
      response.result = transformer.documentLinkResolveResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_FORMATTING: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<DocumentFormattingParams> requestParams =
        transformer.asTextDocumentFormattingParams(messageParams);
      TextDocumentFormattingResult result =
        receiveTextDocument_formatting(*requestParams);
      response.result = transformer.textDocumentFormattingResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_RANGE_FORMATTING: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<DocumentRangeFormattingParams> requestParams =
        transformer.asTextDocumentRangeFormattingParams(messageParams);
      TextDocumentRangeFormattingResult result =
        receiveTextDocument_rangeFormatting(*requestParams);
      response.result = transformer.textDocumentRangeFormattingResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_RANGES_FORMATTING: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<DocumentRangesFormattingParams> requestParams =
        transformer.asTextDocumentRangesFormattingParams(messageParams);
      TextDocumentRangesFormattingResult result =
        receiveTextDocument_rangesFormatting(*requestParams);
      response.result = transformer.textDocumentRangesFormattingResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_ON_TYPE_FORMATTING: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<DocumentOnTypeFormattingParams> requestParams =
        transformer.asTextDocumentOnTypeFormattingParams(messageParams);
      TextDocumentOnTypeFormattingResult result =
        receiveTextDocument_onTypeFormatting(*requestParams);
      response.result = transformer.textDocumentOnTypeFormattingResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_RENAME: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<RenameParams> requestParams =
        transformer.asTextDocumentRenameParams(messageParams);
      TextDocumentRenameResult result =
        receiveTextDocument_rename(*requestParams);
      response.result = transformer.textDocumentRenameResultToAny(result);
      break;
    }
    case IncomingRequest::TEXT_DOCUMENT_PREPARE_RENAME: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<PrepareRenameParams> requestParams =
        transformer.asTextDocumentPrepareRenameParams(messageParams);
      TextDocumentPrepareRenameResult result =
        receiveTextDocument_prepareRename(*requestParams);
      response.result = transformer.textDocumentPrepareRenameResultToAny(result);
      break;
    }
    case IncomingRequest::WORKSPACE_EXECUTE_COMMAND: {
      MessageParams &messageParams = requireMessageParams(request);
      std::unique_ptr<ExecuteCommandParams> requestParams =
        transformer.asWorkspaceExecuteCommandParams(messageParams);
      WorkspaceExecuteCommandResult result =
        receiveWorkspace_executeCommand(*requestParams);
      response.result = transformer.workspaceExecuteCommandResultToAny(result);
      break;
    }
    default: {
    invalidMethod:
      std::stringstream ss;
      ss << "Unsupported request method: \"" << request.method << "\"";
      throw LSP_EXCEPTION(ErrorCodes::METHOD_NOT_FOUND, ss.str());
    }
    }
  }

  auto LspLanguageServer::dispatch(
    ResponseMessage &/*response*/,
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
      receiveWorkspace_didChangeWorkspaceFolders(*notificationParams);
      break;
    }
    case IncomingNotification::WINDOW_WORK_DONE_PROGRESS_CANCEL: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<WorkDoneProgressCancelParams> notificationParams =
        transformer.asWindowWorkDoneProgressCancelParams(messageParams);
      receiveWindow_workDoneProgress_cancel(*notificationParams);
      break;
    }
    case IncomingNotification::WORKSPACE_DID_CREATE_FILES: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<CreateFilesParams> notificationParams =
        transformer.asWorkspaceDidCreateFilesParams(messageParams);
      receiveWorkspace_didCreateFiles(*notificationParams);
      break;
    }
    case IncomingNotification::WORKSPACE_DID_RENAME_FILES: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<RenameFilesParams> notificationParams =
        transformer.asWorkspaceDidRenameFilesParams(messageParams);
      receiveWorkspace_didRenameFiles(*notificationParams);
      break;
    }
    case IncomingNotification::WORKSPACE_DID_DELETE_FILES: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<DeleteFilesParams> notificationParams =
        transformer.asWorkspaceDidDeleteFilesParams(messageParams);
      receiveWorkspace_didDeleteFiles(*notificationParams);
      break;
    }
    case IncomingNotification::NOTEBOOK_DOCUMENT_DID_OPEN: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<DidOpenNotebookDocumentParams> notificationParams =
        transformer.asNotebookDocumentDidOpenParams(messageParams);
      receiveNotebookDocument_didOpen(*notificationParams);
      break;
    }
    case IncomingNotification::NOTEBOOK_DOCUMENT_DID_CHANGE: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<DidChangeNotebookDocumentParams> notificationParams =
        transformer.asNotebookDocumentDidChangeParams(messageParams);
      receiveNotebookDocument_didChange(*notificationParams);
      break;
    }
    case IncomingNotification::NOTEBOOK_DOCUMENT_DID_SAVE: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<DidSaveNotebookDocumentParams> notificationParams =
        transformer.asNotebookDocumentDidSaveParams(messageParams);
      receiveNotebookDocument_didSave(*notificationParams);
      break;
    }
    case IncomingNotification::NOTEBOOK_DOCUMENT_DID_CLOSE: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<DidCloseNotebookDocumentParams> notificationParams =
        transformer.asNotebookDocumentDidCloseParams(messageParams);
      receiveNotebookDocument_didClose(*notificationParams);
      break;
    }
    case IncomingNotification::INITIALIZED: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<InitializedParams> notificationParams =
        transformer.asInitializedParams(messageParams);
      receiveInitialized(*notificationParams);
      break;
    }
    case IncomingNotification::EXIT: {
      receiveExit();
      break;
    }
    case IncomingNotification::WORKSPACE_DID_CHANGE_CONFIGURATION: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<DidChangeConfigurationParams> notificationParams =
        transformer.asWorkspaceDidChangeConfigurationParams(messageParams);
      receiveWorkspace_didChangeConfiguration(*notificationParams);
      break;
    }
    case IncomingNotification::TEXT_DOCUMENT_DID_OPEN: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<DidOpenTextDocumentParams> notificationParams =
        transformer.asTextDocumentDidOpenParams(messageParams);
      receiveTextDocument_didOpen(*notificationParams);
      break;
    }
    case IncomingNotification::TEXT_DOCUMENT_DID_CHANGE: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<DidChangeTextDocumentParams> notificationParams =
        transformer.asTextDocumentDidChangeParams(messageParams);
      receiveTextDocument_didChange(*notificationParams);
      break;
    }
    case IncomingNotification::TEXT_DOCUMENT_DID_CLOSE: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<DidCloseTextDocumentParams> notificationParams =
        transformer.asTextDocumentDidCloseParams(messageParams);
      receiveTextDocument_didClose(*notificationParams);
      break;
    }
    case IncomingNotification::TEXT_DOCUMENT_DID_SAVE: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<DidSaveTextDocumentParams> notificationParams =
        transformer.asTextDocumentDidSaveParams(messageParams);
      receiveTextDocument_didSave(*notificationParams);
      break;
    }
    case IncomingNotification::TEXT_DOCUMENT_WILL_SAVE: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<WillSaveTextDocumentParams> notificationParams =
        transformer.asTextDocumentWillSaveParams(messageParams);
      receiveTextDocument_willSave(*notificationParams);
      break;
    }
    case IncomingNotification::WORKSPACE_DID_CHANGE_WATCHED_FILES: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<DidChangeWatchedFilesParams> notificationParams =
        transformer.asWorkspaceDidChangeWatchedFilesParams(messageParams);
      receiveWorkspace_didChangeWatchedFiles(*notificationParams);
      break;
    }
    case IncomingNotification::SET_TRACE: {
      MessageParams &messageParams = requireMessageParams(notification);
      std::unique_ptr<SetTraceParams> notificationParams =
        transformer.asSetTraceParams(messageParams);
      receiveSetTrace(*notificationParams);
      break;
    }
    default: {
    invalidMethod:
      std::stringstream ss;
      ss << "Unsupported notification method: \"" << notification.method << "\"";
      throw LSP_EXCEPTION(ErrorCodes::METHOD_NOT_FOUND, ss.str());
    }
    }
  }
  auto LspLanguageServer::dispatch(ResponseMessage &response) -> void {
    ResponseIdType responseIdType =
      static_cast<ResponseIdType>(response.id.index());
    if (responseIdType != ResponseIdType::INTEGER_TYPE) {
      auto loggerLock = logger.lock();
      logger
        << "Cannot dispatch response with id of type ResponseIdType::"
        << ResponseIdTypeNames.at(responseIdType)
        << std::endl;
    }
    int responseId = std::get<int>(response.id);
    std::string method;
    {
      std::unique_lock<std::mutex> callbackLock(callbackMutex);
      auto iter = callbacksById.find(responseId);
      if (iter != callbacksById.end()) {
        method = iter->second;
        callbacksById.erase(iter);
      } else {
        auto loggerLock = logger.lock();
        logger << "Cannot locate request with id: " << responseId << std::endl;
        return;
      }
    }

    OutgoingRequest request;
    try {
      request = outgoingRequestByValue(method);
    } catch (std::invalid_argument &e) {
      goto invalidMethod;
    }

    switch (request) {
    case OutgoingRequest::WORKSPACE_WORKSPACE_FOLDERS: {
      if (!response.result.has_value()) {
        auto loggerLock = logger.lock();
        logger << "Missing required attribute for method \"workspace/workspaceFolders\": result" << std::endl;
        return;
      }
      std::unique_ptr<LSPAny> &result = response.result.value();
      WorkspaceWorkspaceFoldersResult params =
        transformer.anyToWorkspaceWorkspaceFoldersResult(*result);
      receiveWorkspace_workspaceFolders(params);
      break;
    }
    case OutgoingRequest::WORKSPACE_CONFIGURATION: {
      if (!response.result.has_value()) {
        auto loggerLock = logger.lock();
        logger << "Missing required attribute for method \"workspace/configuration\": result" << std::endl;
        return;
      }
      std::unique_ptr<LSPAny> &result = response.result.value();
      WorkspaceConfigurationResult params =
        transformer.anyToWorkspaceConfigurationResult(*result);
      receiveWorkspace_configuration(params);
      break;
    }
    case OutgoingRequest::WORKSPACE_FOLDING_RANGE_REFRESH: {
      if (!response.result.has_value()) {
        auto loggerLock = logger.lock();
        logger << "Missing required attribute for method \"workspace/foldingRange/refresh\": result" << std::endl;
        return;
      }
      std::unique_ptr<LSPAny> &result = response.result.value();
      WorkspaceFoldingRangeRefreshResult params =
        transformer.anyToWorkspaceFoldingRangeRefreshResult(*result);
      receiveWorkspace_foldingRange_refresh(params);
      break;
    }
    case OutgoingRequest::WINDOW_WORK_DONE_PROGRESS_CREATE: {
      if (!response.result.has_value()) {
        auto loggerLock = logger.lock();
        logger << "Missing required attribute for method \"window/workDoneProgress/create\": result" << std::endl;
        return;
      }
      std::unique_ptr<LSPAny> &result = response.result.value();
      WindowWorkDoneProgressCreateResult params =
        transformer.anyToWindowWorkDoneProgressCreateResult(*result);
      receiveWindow_workDoneProgress_create(params);
      break;
    }
    case OutgoingRequest::WORKSPACE_SEMANTIC_TOKENS_REFRESH: {
      if (!response.result.has_value()) {
        auto loggerLock = logger.lock();
        logger << "Missing required attribute for method \"workspace/semanticTokens/refresh\": result" << std::endl;
        return;
      }
      std::unique_ptr<LSPAny> &result = response.result.value();
      WorkspaceSemanticTokensRefreshResult params =
        transformer.anyToWorkspaceSemanticTokensRefreshResult(*result);
      receiveWorkspace_semanticTokens_refresh(params);
      break;
    }
    case OutgoingRequest::WINDOW_SHOW_DOCUMENT: {
      if (!response.result.has_value()) {
        auto loggerLock = logger.lock();
        logger << "Missing required attribute for method \"window/showDocument\": result" << std::endl;
        return;
      }
      std::unique_ptr<LSPAny> &result = response.result.value();
      std::unique_ptr<WindowShowDocumentResult> params =
        transformer.anyToWindowShowDocumentResult(*result);
      receiveWindow_showDocument(*params);
      break;
    }
    case OutgoingRequest::WORKSPACE_INLINE_VALUE_REFRESH: {
      if (!response.result.has_value()) {
        auto loggerLock = logger.lock();
        logger << "Missing required attribute for method \"workspace/inlineValue/refresh\": result" << std::endl;
        return;
      }
      std::unique_ptr<LSPAny> &result = response.result.value();
      WorkspaceInlineValueRefreshResult params =
        transformer.anyToWorkspaceInlineValueRefreshResult(*result);
      receiveWorkspace_inlineValue_refresh(params);
      break;
    }
    case OutgoingRequest::WORKSPACE_INLAY_HINT_REFRESH: {
      if (!response.result.has_value()) {
        auto loggerLock = logger.lock();
        logger << "Missing required attribute for method \"workspace/inlayHint/refresh\": result" << std::endl;
        return;
      }
      std::unique_ptr<LSPAny> &result = response.result.value();
      WorkspaceInlayHintRefreshResult params =
        transformer.anyToWorkspaceInlayHintRefreshResult(*result);
      receiveWorkspace_inlayHint_refresh(params);
      break;
    }
    case OutgoingRequest::WORKSPACE_DIAGNOSTIC_REFRESH: {
      if (!response.result.has_value()) {
        auto loggerLock = logger.lock();
        logger << "Missing required attribute for method \"workspace/diagnostic/refresh\": result" << std::endl;
        return;
      }
      std::unique_ptr<LSPAny> &result = response.result.value();
      WorkspaceDiagnosticRefreshResult params =
        transformer.anyToWorkspaceDiagnosticRefreshResult(*result);
      receiveWorkspace_diagnostic_refresh(params);
      break;
    }
    case OutgoingRequest::CLIENT_REGISTER_CAPABILITY: {
      if (!response.result.has_value()) {
        auto loggerLock = logger.lock();
        logger << "Missing required attribute for method \"client/registerCapability\": result" << std::endl;
        return;
      }
      std::unique_ptr<LSPAny> &result = response.result.value();
      ClientRegisterCapabilityResult params =
        transformer.anyToClientRegisterCapabilityResult(*result);
      receiveClient_registerCapability(params);
      break;
    }
    case OutgoingRequest::CLIENT_UNREGISTER_CAPABILITY: {
      if (!response.result.has_value()) {
        auto loggerLock = logger.lock();
        logger << "Missing required attribute for method \"client/unregisterCapability\": result" << std::endl;
        return;
      }
      std::unique_ptr<LSPAny> &result = response.result.value();
      ClientUnregisterCapabilityResult params =
        transformer.anyToClientUnregisterCapabilityResult(*result);
      receiveClient_unregisterCapability(params);
      break;
    }
    case OutgoingRequest::WINDOW_SHOW_MESSAGE_REQUEST: {
      if (!response.result.has_value()) {
        auto loggerLock = logger.lock();
        logger << "Missing required attribute for method \"window/showMessageRequest\": result" << std::endl;
        return;
      }
      std::unique_ptr<LSPAny> &result = response.result.value();
      WindowShowMessageRequestResult params =
        transformer.anyToWindowShowMessageRequestResult(*result);
      receiveWindow_showMessageRequest(params);
      break;
    }
    case OutgoingRequest::WORKSPACE_CODE_LENS_REFRESH: {
      if (!response.result.has_value()) {
        auto loggerLock = logger.lock();
        logger << "Missing required attribute for method \"workspace/codeLens/refresh\": result" << std::endl;
        return;
      }
      std::unique_ptr<LSPAny> &result = response.result.value();
      WorkspaceCodeLensRefreshResult params =
        transformer.anyToWorkspaceCodeLensRefreshResult(*result);
      receiveWorkspace_codeLens_refresh(params);
      break;
    }
    case OutgoingRequest::WORKSPACE_APPLY_EDIT: {
      if (!response.result.has_value()) {
        auto loggerLock = logger.lock();
        logger << "Missing required attribute for method \"workspace/applyEdit\": result" << std::endl;
        return;
      }
      std::unique_ptr<LSPAny> &result = response.result.value();
      std::unique_ptr<WorkspaceApplyEditResult> params =
        transformer.anyToWorkspaceApplyEditResult(*result);
      receiveWorkspace_applyEdit(*params);
      break;
    }
    default: {
    invalidMethod:
      auto loggerLock = logger.lock();
      logger << "Unsupported request method: \"" << method << "\"";
    }
    }
  }
  auto LspLanguageServer::requireMessageParams(
    RequestMessage &request
  ) const -> MessageParams & {
    if (request.params.has_value()) {
      return request.params.value();
    }
    std::stringstream ss;
    ss << "RequestMessage.params must be defined for method=\"" << request.method << "\"";
    throw LSP_EXCEPTION(ErrorCodes::INVALID_PARAMS, ss.str());
  }

  auto LspLanguageServer::requireMessageParams(
    NotificationMessage &notification
  ) const -> MessageParams & {
    if (notification.params.has_value()) {
      return notification.params.value();
    }
    std::stringstream ss;
    ss << "NotificationMessage.params must be defined for method=\""
       << notification.method << "\"";
    throw LSP_EXCEPTION(ErrorCodes::INVALID_PARAMS, ss.str());
  }

  // ================= //
  // Incoming Requests //
  // ================= //

  // request: "textDocument/implementation"
  auto LspLanguageServer::receiveTextDocument_implementation(
    ImplementationParams &/*params*/
  ) -> TextDocumentImplementationResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/implementation\""
    );
  }

  // request: "textDocument/typeDefinition"
  auto LspLanguageServer::receiveTextDocument_typeDefinition(
    TypeDefinitionParams &/*params*/
  ) -> TextDocumentTypeDefinitionResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/typeDefinition\""
    );
  }

  // request: "textDocument/documentColor"
  auto LspLanguageServer::receiveTextDocument_documentColor(
    DocumentColorParams &/*params*/
  ) -> TextDocumentDocumentColorResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/documentColor\""
    );
  }

  // request: "textDocument/colorPresentation"
  auto LspLanguageServer::receiveTextDocument_colorPresentation(
    ColorPresentationParams &/*params*/
  ) -> TextDocumentColorPresentationResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/colorPresentation\""
    );
  }

  // request: "textDocument/foldingRange"
  auto LspLanguageServer::receiveTextDocument_foldingRange(
    FoldingRangeParams &/*params*/
  ) -> TextDocumentFoldingRangeResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/foldingRange\""
    );
  }

  // request: "textDocument/declaration"
  auto LspLanguageServer::receiveTextDocument_declaration(
    DeclarationParams &/*params*/
  ) -> TextDocumentDeclarationResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/declaration\""
    );
  }

  // request: "textDocument/selectionRange"
  auto LspLanguageServer::receiveTextDocument_selectionRange(
    SelectionRangeParams &/*params*/
  ) -> TextDocumentSelectionRangeResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/selectionRange\""
    );
  }

  // request: "textDocument/prepareCallHierarchy"
  auto LspLanguageServer::receiveTextDocument_prepareCallHierarchy(
    CallHierarchyPrepareParams &/*params*/
  ) -> TextDocumentPrepareCallHierarchyResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/prepareCallHierarchy\""
    );
  }

  // request: "callHierarchy/incomingCalls"
  auto LspLanguageServer::receiveCallHierarchy_incomingCalls(
    CallHierarchyIncomingCallsParams &/*params*/
  ) -> CallHierarchyIncomingCallsResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"callHierarchy/incomingCalls\""
    );
  }

  // request: "callHierarchy/outgoingCalls"
  auto LspLanguageServer::receiveCallHierarchy_outgoingCalls(
    CallHierarchyOutgoingCallsParams &/*params*/
  ) -> CallHierarchyOutgoingCallsResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"callHierarchy/outgoingCalls\""
    );
  }

  // request: "textDocument/semanticTokens/full"
  auto LspLanguageServer::receiveTextDocument_semanticTokens_full(
    SemanticTokensParams &/*params*/
  ) -> TextDocumentSemanticTokensFullResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/semanticTokens/full\""
    );
  }

  // request: "textDocument/semanticTokens/full/delta"
  auto LspLanguageServer::receiveTextDocument_semanticTokens_full_delta(
    SemanticTokensDeltaParams &/*params*/
  ) -> TextDocumentSemanticTokensFullDeltaResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/semanticTokens/full/delta\""
    );
  }

  // request: "textDocument/semanticTokens/range"
  auto LspLanguageServer::receiveTextDocument_semanticTokens_range(
    SemanticTokensRangeParams &/*params*/
  ) -> TextDocumentSemanticTokensRangeResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/semanticTokens/range\""
    );
  }

  // request: "textDocument/linkedEditingRange"
  auto LspLanguageServer::receiveTextDocument_linkedEditingRange(
    LinkedEditingRangeParams &/*params*/
  ) -> TextDocumentLinkedEditingRangeResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/linkedEditingRange\""
    );
  }

  // request: "workspace/willCreateFiles"
  auto LspLanguageServer::receiveWorkspace_willCreateFiles(
    CreateFilesParams &/*params*/
  ) -> WorkspaceWillCreateFilesResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"workspace/willCreateFiles\""
    );
  }

  // request: "workspace/willRenameFiles"
  auto LspLanguageServer::receiveWorkspace_willRenameFiles(
    RenameFilesParams &/*params*/
  ) -> WorkspaceWillRenameFilesResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"workspace/willRenameFiles\""
    );
  }

  // request: "workspace/willDeleteFiles"
  auto LspLanguageServer::receiveWorkspace_willDeleteFiles(
    DeleteFilesParams &/*params*/
  ) -> WorkspaceWillDeleteFilesResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"workspace/willDeleteFiles\""
    );
  }

  // request: "textDocument/moniker"
  auto LspLanguageServer::receiveTextDocument_moniker(
    MonikerParams &/*params*/
  ) -> TextDocumentMonikerResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/moniker\""
    );
  }

  // request: "textDocument/prepareTypeHierarchy"
  auto LspLanguageServer::receiveTextDocument_prepareTypeHierarchy(
    TypeHierarchyPrepareParams &/*params*/
  ) -> TextDocumentPrepareTypeHierarchyResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/prepareTypeHierarchy\""
    );
  }

  // request: "typeHierarchy/supertypes"
  auto LspLanguageServer::receiveTypeHierarchy_supertypes(
    TypeHierarchySupertypesParams &/*params*/
  ) -> TypeHierarchySupertypesResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"typeHierarchy/supertypes\""
    );
  }

  // request: "typeHierarchy/subtypes"
  auto LspLanguageServer::receiveTypeHierarchy_subtypes(
    TypeHierarchySubtypesParams &/*params*/
  ) -> TypeHierarchySubtypesResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"typeHierarchy/subtypes\""
    );
  }

  // request: "textDocument/inlineValue"
  auto LspLanguageServer::receiveTextDocument_inlineValue(
    InlineValueParams &/*params*/
  ) -> TextDocumentInlineValueResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/inlineValue\""
    );
  }

  // request: "textDocument/inlayHint"
  auto LspLanguageServer::receiveTextDocument_inlayHint(
    InlayHintParams &/*params*/
  ) -> TextDocumentInlayHintResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/inlayHint\""
    );
  }

  // request: "inlayHint/resolve"
  auto LspLanguageServer::receiveInlayHint_resolve(
    InlayHint &/*params*/
  ) -> InlayHintResolveResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"inlayHint/resolve\""
    );
  }

  // request: "textDocument/diagnostic"
  auto LspLanguageServer::receiveTextDocument_diagnostic(
    DocumentDiagnosticParams &/*params*/
  ) -> TextDocumentDiagnosticResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/diagnostic\""
    );
  }

  // request: "workspace/diagnostic"
  auto LspLanguageServer::receiveWorkspace_diagnostic(
    WorkspaceDiagnosticParams &/*params*/
  ) -> WorkspaceDiagnosticResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"workspace/diagnostic\""
    );
  }

  // request: "textDocument/inlineCompletion"
  auto LspLanguageServer::receiveTextDocument_inlineCompletion(
    InlineCompletionParams &/*params*/
  ) -> TextDocumentInlineCompletionResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/inlineCompletion\""
    );
  }

  // request: "initialize"
  auto LspLanguageServer::receiveInitialize(
    InitializeParams &/*params*/
  ) -> InitializeResult {
    InitializeResult result;

    std::unique_ptr<ServerCapabilities> capabilities =
      std::make_unique<ServerCapabilities>();

    return result;
  }

  // request: "shutdown"
  auto LspLanguageServer::receiveShutdown() -> ShutdownResult {
    bool shutdown = false;
    if (_shutdown.compare_exchange_strong(shutdown, true)) {
      logger << "Shutting down server." << std::endl;
    }
    return nullptr;
  }

  // request: "textDocument/willSaveWaitUntil"
  auto LspLanguageServer::receiveTextDocument_willSaveWaitUntil(
    WillSaveTextDocumentParams &/*params*/
  ) -> TextDocumentWillSaveWaitUntilResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/willSaveWaitUntil\""
    );
  }

  // request: "textDocument/completion"
  auto LspLanguageServer::receiveTextDocument_completion(
    CompletionParams &/*params*/
  ) -> TextDocumentCompletionResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/completion\""
    );
  }

  // request: "completionItem/resolve"
  auto LspLanguageServer::receiveCompletionItem_resolve(
    CompletionItem &/*params*/
  ) -> CompletionItemResolveResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"completionItem/resolve\""
    );
  }

  // request: "textDocument/hover"
  auto LspLanguageServer::receiveTextDocument_hover(
    HoverParams &/*params*/
  ) -> TextDocumentHoverResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/hover\""
    );
  }

  // request: "textDocument/signatureHelp"
  auto LspLanguageServer::receiveTextDocument_signatureHelp(
    SignatureHelpParams &/*params*/
  ) -> TextDocumentSignatureHelpResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/signatureHelp\""
    );
  }

  // request: "textDocument/definition"
  auto LspLanguageServer::receiveTextDocument_definition(
    DefinitionParams &/*params*/
  ) -> TextDocumentDefinitionResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/definition\""
    );
  }

  // request: "textDocument/references"
  auto LspLanguageServer::receiveTextDocument_references(
    ReferenceParams &/*params*/
  ) -> TextDocumentReferencesResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/references\""
    );
  }

  // request: "textDocument/documentHighlight"
  auto LspLanguageServer::receiveTextDocument_documentHighlight(
    DocumentHighlightParams &/*params*/
  ) -> TextDocumentDocumentHighlightResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/documentHighlight\""
    );
  }

  // request: "textDocument/documentSymbol"
  auto LspLanguageServer::receiveTextDocument_documentSymbol(
    DocumentSymbolParams &/*params*/
  ) -> TextDocumentDocumentSymbolResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/documentSymbol\""
    );
  }

  // request: "textDocument/codeAction"
  auto LspLanguageServer::receiveTextDocument_codeAction(
    CodeActionParams &/*params*/
  ) -> TextDocumentCodeActionResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/codeAction\""
    );
  }

  // request: "codeAction/resolve"
  auto LspLanguageServer::receiveCodeAction_resolve(
    CodeAction &/*params*/
  ) -> CodeActionResolveResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"codeAction/resolve\""
    );
  }

  // request: "workspace/symbol"
  auto LspLanguageServer::receiveWorkspace_symbol(
    WorkspaceSymbolParams &/*params*/
  ) -> WorkspaceSymbolResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"workspace/symbol\""
    );
  }

  // request: "workspaceSymbol/resolve"
  auto LspLanguageServer::receiveWorkspaceSymbol_resolve(
    WorkspaceSymbol &/*params*/
  ) -> WorkspaceSymbolResolveResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"workspaceSymbol/resolve\""
    );
  }

  // request: "textDocument/codeLens"
  auto LspLanguageServer::receiveTextDocument_codeLens(
    CodeLensParams &/*params*/
  ) -> TextDocumentCodeLensResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/codeLens\""
    );
  }

  // request: "codeLens/resolve"
  auto LspLanguageServer::receiveCodeLens_resolve(
    CodeLens &/*params*/
  ) -> CodeLensResolveResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"codeLens/resolve\""
    );
  }

  // request: "textDocument/documentLink"
  auto LspLanguageServer::receiveTextDocument_documentLink(
    DocumentLinkParams &/*params*/
  ) -> TextDocumentDocumentLinkResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/documentLink\""
    );
  }

  // request: "documentLink/resolve"
  auto LspLanguageServer::receiveDocumentLink_resolve(
    DocumentLink &/*params*/
  ) -> DocumentLinkResolveResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"documentLink/resolve\""
    );
  }

  // request: "textDocument/formatting"
  auto LspLanguageServer::receiveTextDocument_formatting(
    DocumentFormattingParams &/*params*/
  ) -> TextDocumentFormattingResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/formatting\""
    );
  }

  // request: "textDocument/rangeFormatting"
  auto LspLanguageServer::receiveTextDocument_rangeFormatting(
    DocumentRangeFormattingParams &/*params*/
  ) -> TextDocumentRangeFormattingResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/rangeFormatting\""
    );
  }

  // request: "textDocument/rangesFormatting"
  auto LspLanguageServer::receiveTextDocument_rangesFormatting(
    DocumentRangesFormattingParams &/*params*/
  ) -> TextDocumentRangesFormattingResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/rangesFormatting\""
    );
  }

  // request: "textDocument/onTypeFormatting"
  auto LspLanguageServer::receiveTextDocument_onTypeFormatting(
    DocumentOnTypeFormattingParams &/*params*/
  ) -> TextDocumentOnTypeFormattingResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/onTypeFormatting\""
    );
  }

  // request: "textDocument/rename"
  auto LspLanguageServer::receiveTextDocument_rename(
    RenameParams &/*params*/
  ) -> TextDocumentRenameResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/rename\""
    );
  }

  // request: "textDocument/prepareRename"
  auto LspLanguageServer::receiveTextDocument_prepareRename(
    PrepareRenameParams &/*params*/
  ) -> TextDocumentPrepareRenameResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/prepareRename\""
    );
  }

  // request: "workspace/executeCommand"
  auto LspLanguageServer::receiveWorkspace_executeCommand(
    ExecuteCommandParams &/*params*/
  ) -> WorkspaceExecuteCommandResult {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"workspace/executeCommand\""
    );
  }

  // ====================== //
  // Incoming Notifications //
  // ====================== //

  // notification: "workspace/didChangeWorkspaceFolders"
  auto LspLanguageServer::receiveWorkspace_didChangeWorkspaceFolders(
    DidChangeWorkspaceFoldersParams &/*params*/
  ) -> void {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"workspace/didChangeWorkspaceFolders\""
    );
  }

  // notification: "window/workDoneProgress/cancel"
  auto LspLanguageServer::receiveWindow_workDoneProgress_cancel(
    WorkDoneProgressCancelParams &/*params*/
  ) -> void {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"window/workDoneProgress/cancel\""
    );
  }

  // notification: "workspace/didCreateFiles"
  auto LspLanguageServer::receiveWorkspace_didCreateFiles(
    CreateFilesParams &/*params*/
  ) -> void {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"workspace/didCreateFiles\""
    );
  }

  // notification: "workspace/didRenameFiles"
  auto LspLanguageServer::receiveWorkspace_didRenameFiles(
    RenameFilesParams &/*params*/
  ) -> void {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"workspace/didRenameFiles\""
    );
  }

  // notification: "workspace/didDeleteFiles"
  auto LspLanguageServer::receiveWorkspace_didDeleteFiles(
    DeleteFilesParams &/*params*/
  ) -> void {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"workspace/didDeleteFiles\""
    );
  }

  // notification: "notebookDocument/didOpen"
  auto LspLanguageServer::receiveNotebookDocument_didOpen(
    DidOpenNotebookDocumentParams &/*params*/
  ) -> void {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"notebookDocument/didOpen\""
    );
  }

  // notification: "notebookDocument/didChange"
  auto LspLanguageServer::receiveNotebookDocument_didChange(
    DidChangeNotebookDocumentParams &/*params*/
  ) -> void {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"notebookDocument/didChange\""
    );
  }

  // notification: "notebookDocument/didSave"
  auto LspLanguageServer::receiveNotebookDocument_didSave(
    DidSaveNotebookDocumentParams &/*params*/
  ) -> void {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"notebookDocument/didSave\""
    );
  }

  // notification: "notebookDocument/didClose"
  auto LspLanguageServer::receiveNotebookDocument_didClose(
    DidCloseNotebookDocumentParams &/*params*/
  ) -> void {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"notebookDocument/didClose\""
    );
  }

  // notification: "initialized"
  auto LspLanguageServer::receiveInitialized(
    InitializedParams &/*params*/
  ) -> void {
    // empty
  }

  // notification: "exit"
  auto LspLanguageServer::receiveExit() -> void {
    bool exit = false;
    if (_exit.compare_exchange_strong(exit, true)) {
      logger << "Exiting server." << std::endl;
      bool shutdown = false;
      if (_shutdown.compare_exchange_strong(shutdown, true)) {
        auto loggerLock = logger.lock();
        logger
          << "Server exited before being notified to shutdown!"
          << std::endl;
      }
      incomingMessages.stop();
      requestPool.stop();
      workerPool.stop();
      requestPool.join();
      workerPool.join();
      listener.join();
    }
  }

  // notification: "workspace/didChangeConfiguration"
  auto LspLanguageServer::receiveWorkspace_didChangeConfiguration(
    DidChangeConfigurationParams &/*params*/
  ) -> void {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"workspace/didChangeConfiguration\""
    );
  }

  // notification: "textDocument/didOpen"
  auto LspLanguageServer::receiveTextDocument_didOpen(
    DidOpenTextDocumentParams &/*params*/
  ) -> void {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/didOpen\""
    );
  }

  // notification: "textDocument/didChange"
  auto LspLanguageServer::receiveTextDocument_didChange(
    DidChangeTextDocumentParams &/*params*/
  ) -> void {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/didChange\""
    );
  }

  // notification: "textDocument/didClose"
  auto LspLanguageServer::receiveTextDocument_didClose(
    DidCloseTextDocumentParams &/*params*/
  ) -> void {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/didClose\""
    );
  }

  // notification: "textDocument/didSave"
  auto LspLanguageServer::receiveTextDocument_didSave(
    DidSaveTextDocumentParams &/*params*/
  ) -> void {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/didSave\""
    );
  }

  // notification: "textDocument/willSave"
  auto LspLanguageServer::receiveTextDocument_willSave(
    WillSaveTextDocumentParams &/*params*/
  ) -> void {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"textDocument/willSave\""
    );
  }

  // notification: "workspace/didChangeWatchedFiles"
  auto LspLanguageServer::receiveWorkspace_didChangeWatchedFiles(
    DidChangeWatchedFilesParams &/*params*/
  ) -> void {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"workspace/didChangeWatchedFiles\""
    );
  }

  // notification: "$/setTrace"
  auto LspLanguageServer::receiveSetTrace(
    SetTraceParams &/*params*/
  ) -> void {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"$/setTrace\""
    );
  }

  // ================= //
  // Outgoing Requests //
  // ================= //

  // request: "workspace/workspaceFolders"
  auto LspLanguageServer::sendWorkspace_workspaceFolders() -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    int requestId = nextId();
    request.id = requestId;
    {
      std::unique_lock<std::mutex> callbackLock(callbackMutex);
      callbacksById.emplace(requestId, "workspace/workspaceFolders");
    }
    request.method = "workspace/workspaceFolders";
    std::unique_ptr<LSPAny> any = transformer.requestMessageToAny(request);
    const std::string message = serializer.serialize(*any);
    send(message);
  }

  auto LspLanguageServer::receiveWorkspace_workspaceFolders(
    WorkspaceWorkspaceFoldersResult &/*params*/
  ) -> void {
    auto loggerLock = logger.lock();
    logger << "No handler exists for method: \"workspace/workspaceFolders\"" << std::endl;
  }

  // request: "workspace/configuration"
  auto LspLanguageServer::sendWorkspace_configuration(
    ConfigurationParams &params
  ) -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    int requestId = nextId();
    request.id = requestId;
    {
      std::unique_lock<std::mutex> callbackLock(callbackMutex);
      callbacksById.emplace(requestId, "workspace/configuration");
    }
    request.method = "workspace/configuration";
    request.params = transformer.asMessageParams(params);
    std::unique_ptr<LSPAny> any = transformer.requestMessageToAny(request);
    const std::string message = serializer.serialize(*any);
    send(message);
  }

  auto LspLanguageServer::receiveWorkspace_configuration(
    WorkspaceConfigurationResult &/*params*/
  ) -> void {
    auto loggerLock = logger.lock();
    logger << "No handler exists for method: \"workspace/configuration\"" << std::endl;
  }

  // request: "workspace/foldingRange/refresh"
  auto LspLanguageServer::sendWorkspace_foldingRange_refresh() -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    int requestId = nextId();
    request.id = requestId;
    {
      std::unique_lock<std::mutex> callbackLock(callbackMutex);
      callbacksById.emplace(requestId, "workspace/foldingRange/refresh");
    }
    request.method = "workspace/foldingRange/refresh";
    std::unique_ptr<LSPAny> any = transformer.requestMessageToAny(request);
    const std::string message = serializer.serialize(*any);
    send(message);
  }

  auto LspLanguageServer::receiveWorkspace_foldingRange_refresh(
    WorkspaceFoldingRangeRefreshResult /*params*/
  ) -> void {
    auto loggerLock = logger.lock();
    logger << "No handler exists for method: \"workspace/foldingRange/refresh\"" << std::endl;
  }

  // request: "window/workDoneProgress/create"
  auto LspLanguageServer::sendWindow_workDoneProgress_create(
    WorkDoneProgressCreateParams &params
  ) -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    int requestId = nextId();
    request.id = requestId;
    {
      std::unique_lock<std::mutex> callbackLock(callbackMutex);
      callbacksById.emplace(requestId, "window/workDoneProgress/create");
    }
    request.method = "window/workDoneProgress/create";
    request.params = transformer.asMessageParams(params);
    std::unique_ptr<LSPAny> any = transformer.requestMessageToAny(request);
    const std::string message = serializer.serialize(*any);
    send(message);
  }

  auto LspLanguageServer::receiveWindow_workDoneProgress_create(
    WindowWorkDoneProgressCreateResult /*params*/
  ) -> void {
    auto loggerLock = logger.lock();
    logger << "No handler exists for method: \"window/workDoneProgress/create\"" << std::endl;
  }

  // request: "workspace/semanticTokens/refresh"
  auto LspLanguageServer::sendWorkspace_semanticTokens_refresh() -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    int requestId = nextId();
    request.id = requestId;
    {
      std::unique_lock<std::mutex> callbackLock(callbackMutex);
      callbacksById.emplace(requestId, "workspace/semanticTokens/refresh");
    }
    request.method = "workspace/semanticTokens/refresh";
    std::unique_ptr<LSPAny> any = transformer.requestMessageToAny(request);
    const std::string message = serializer.serialize(*any);
    send(message);
  }

  auto LspLanguageServer::receiveWorkspace_semanticTokens_refresh(
    WorkspaceSemanticTokensRefreshResult /*params*/
  ) -> void {
    auto loggerLock = logger.lock();
    logger << "No handler exists for method: \"workspace/semanticTokens/refresh\"" << std::endl;
  }

  // request: "window/showDocument"
  auto LspLanguageServer::sendWindow_showDocument(
    ShowDocumentParams &params
  ) -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    int requestId = nextId();
    request.id = requestId;
    {
      std::unique_lock<std::mutex> callbackLock(callbackMutex);
      callbacksById.emplace(requestId, "window/showDocument");
    }
    request.method = "window/showDocument";
    request.params = transformer.asMessageParams(params);
    std::unique_ptr<LSPAny> any = transformer.requestMessageToAny(request);
    const std::string message = serializer.serialize(*any);
    send(message);
  }

  auto LspLanguageServer::receiveWindow_showDocument(
    WindowShowDocumentResult &/*params*/
  ) -> void {
    auto loggerLock = logger.lock();
    logger << "No handler exists for method: \"window/showDocument\"" << std::endl;
  }

  // request: "workspace/inlineValue/refresh"
  auto LspLanguageServer::sendWorkspace_inlineValue_refresh() -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    int requestId = nextId();
    request.id = requestId;
    {
      std::unique_lock<std::mutex> callbackLock(callbackMutex);
      callbacksById.emplace(requestId, "workspace/inlineValue/refresh");
    }
    request.method = "workspace/inlineValue/refresh";
    std::unique_ptr<LSPAny> any = transformer.requestMessageToAny(request);
    const std::string message = serializer.serialize(*any);
    send(message);
  }

  auto LspLanguageServer::receiveWorkspace_inlineValue_refresh(
    WorkspaceInlineValueRefreshResult /*params*/
  ) -> void {
    auto loggerLock = logger.lock();
    logger << "No handler exists for method: \"workspace/inlineValue/refresh\"" << std::endl;
  }

  // request: "workspace/inlayHint/refresh"
  auto LspLanguageServer::sendWorkspace_inlayHint_refresh() -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    int requestId = nextId();
    request.id = requestId;
    {
      std::unique_lock<std::mutex> callbackLock(callbackMutex);
      callbacksById.emplace(requestId, "workspace/inlayHint/refresh");
    }
    request.method = "workspace/inlayHint/refresh";
    std::unique_ptr<LSPAny> any = transformer.requestMessageToAny(request);
    const std::string message = serializer.serialize(*any);
    send(message);
  }

  auto LspLanguageServer::receiveWorkspace_inlayHint_refresh(
    WorkspaceInlayHintRefreshResult /*params*/
  ) -> void {
    auto loggerLock = logger.lock();
    logger << "No handler exists for method: \"workspace/inlayHint/refresh\"" << std::endl;
  }

  // request: "workspace/diagnostic/refresh"
  auto LspLanguageServer::sendWorkspace_diagnostic_refresh() -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    int requestId = nextId();
    request.id = requestId;
    {
      std::unique_lock<std::mutex> callbackLock(callbackMutex);
      callbacksById.emplace(requestId, "workspace/diagnostic/refresh");
    }
    request.method = "workspace/diagnostic/refresh";
    std::unique_ptr<LSPAny> any = transformer.requestMessageToAny(request);
    const std::string message = serializer.serialize(*any);
    send(message);
  }

  auto LspLanguageServer::receiveWorkspace_diagnostic_refresh(
    WorkspaceDiagnosticRefreshResult /*params*/
  ) -> void {
    auto loggerLock = logger.lock();
    logger << "No handler exists for method: \"workspace/diagnostic/refresh\"" << std::endl;
  }

  // request: "client/registerCapability"
  auto LspLanguageServer::sendClient_registerCapability(
    RegistrationParams &params
  ) -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    int requestId = nextId();
    request.id = requestId;
    {
      std::unique_lock<std::mutex> callbackLock(callbackMutex);
      callbacksById.emplace(requestId, "client/registerCapability");
    }
    request.method = "client/registerCapability";
    request.params = transformer.asMessageParams(params);
    std::unique_ptr<LSPAny> any = transformer.requestMessageToAny(request);
    const std::string message = serializer.serialize(*any);
    send(message);
  }

  auto LspLanguageServer::receiveClient_registerCapability(
    ClientRegisterCapabilityResult /*params*/
  ) -> void {
    auto loggerLock = logger.lock();
    logger << "No handler exists for method: \"client/registerCapability\"" << std::endl;
  }

  // request: "client/unregisterCapability"
  auto LspLanguageServer::sendClient_unregisterCapability(
    UnregistrationParams &params
  ) -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    int requestId = nextId();
    request.id = requestId;
    {
      std::unique_lock<std::mutex> callbackLock(callbackMutex);
      callbacksById.emplace(requestId, "client/unregisterCapability");
    }
    request.method = "client/unregisterCapability";
    request.params = transformer.asMessageParams(params);
    std::unique_ptr<LSPAny> any = transformer.requestMessageToAny(request);
    const std::string message = serializer.serialize(*any);
    send(message);
  }

  auto LspLanguageServer::receiveClient_unregisterCapability(
    ClientUnregisterCapabilityResult /*params*/
  ) -> void {
    auto loggerLock = logger.lock();
    logger << "No handler exists for method: \"client/unregisterCapability\"" << std::endl;
  }

  // request: "window/showMessageRequest"
  auto LspLanguageServer::sendWindow_showMessageRequest(
    ShowMessageRequestParams &params
  ) -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    int requestId = nextId();
    request.id = requestId;
    {
      std::unique_lock<std::mutex> callbackLock(callbackMutex);
      callbacksById.emplace(requestId, "window/showMessageRequest");
    }
    request.method = "window/showMessageRequest";
    request.params = transformer.asMessageParams(params);
    std::unique_ptr<LSPAny> any = transformer.requestMessageToAny(request);
    const std::string message = serializer.serialize(*any);
    send(message);
  }

  auto LspLanguageServer::receiveWindow_showMessageRequest(
    WindowShowMessageRequestResult &/*params*/
  ) -> void {
    auto loggerLock = logger.lock();
    logger << "No handler exists for method: \"window/showMessageRequest\"" << std::endl;
  }

  // request: "workspace/codeLens/refresh"
  auto LspLanguageServer::sendWorkspace_codeLens_refresh() -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    int requestId = nextId();
    request.id = requestId;
    {
      std::unique_lock<std::mutex> callbackLock(callbackMutex);
      callbacksById.emplace(requestId, "workspace/codeLens/refresh");
    }
    request.method = "workspace/codeLens/refresh";
    std::unique_ptr<LSPAny> any = transformer.requestMessageToAny(request);
    const std::string message = serializer.serialize(*any);
    send(message);
  }

  auto LspLanguageServer::receiveWorkspace_codeLens_refresh(
    WorkspaceCodeLensRefreshResult /*params*/
  ) -> void {
    auto loggerLock = logger.lock();
    logger << "No handler exists for method: \"workspace/codeLens/refresh\"" << std::endl;
  }

  // request: "workspace/applyEdit"
  auto LspLanguageServer::sendWorkspace_applyEdit(
    ApplyWorkspaceEditParams &params
  ) -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    int requestId = nextId();
    request.id = requestId;
    {
      std::unique_lock<std::mutex> callbackLock(callbackMutex);
      callbacksById.emplace(requestId, "workspace/applyEdit");
    }
    request.method = "workspace/applyEdit";
    request.params = transformer.asMessageParams(params);
    std::unique_ptr<LSPAny> any = transformer.requestMessageToAny(request);
    const std::string message = serializer.serialize(*any);
    send(message);
  }

  auto LspLanguageServer::receiveWorkspace_applyEdit(
    WorkspaceApplyEditResult &/*params*/
  ) -> void {
    auto loggerLock = logger.lock();
    logger << "No handler exists for method: \"workspace/applyEdit\"" << std::endl;
  }

  // ====================== //
  // Outgoing Notifications //
  // ====================== //

  // notification: "window/showMessage"
  auto LspLanguageServer::sendWindow_showMessage(
    ShowMessageParams &params
  ) -> void {
    NotificationMessage notification;
    notification.jsonrpc = JSON_RPC_VERSION;
    notification.method = "window/showMessage";
    notification.params = transformer.asMessageParams(params);
    std::unique_ptr<LSPAny> any = transformer.notificationMessageToAny(notification);
    const std::string message = serializer.serialize(*any);
    send(message);
  }

  // notification: "window/logMessage"
  auto LspLanguageServer::sendWindow_logMessage(
    LogMessageParams &params
  ) -> void {
    NotificationMessage notification;
    notification.jsonrpc = JSON_RPC_VERSION;
    notification.method = "window/logMessage";
    notification.params = transformer.asMessageParams(params);
    std::unique_ptr<LSPAny> any = transformer.notificationMessageToAny(notification);
    const std::string message = serializer.serialize(*any);
    send(message);
  }

  // notification: "telemetry/event"
  auto LspLanguageServer::sendTelemetry_event(
    LSPAny &params
  ) -> void {
    NotificationMessage notification;
    notification.jsonrpc = JSON_RPC_VERSION;
    notification.method = "telemetry/event";
    notification.params = transformer.asMessageParams(params);
    std::unique_ptr<LSPAny> any = transformer.notificationMessageToAny(notification);
    const std::string message = serializer.serialize(*any);
    send(message);
  }

  // notification: "textDocument/publishDiagnostics"
  auto LspLanguageServer::sendTextDocument_publishDiagnostics(
    PublishDiagnosticsParams &params
  ) -> void {
    NotificationMessage notification;
    notification.jsonrpc = JSON_RPC_VERSION;
    notification.method = "textDocument/publishDiagnostics";
    notification.params = transformer.asMessageParams(params);
    std::unique_ptr<LSPAny> any = transformer.notificationMessageToAny(notification);
    const std::string message = serializer.serialize(*any);
    send(message);
  }

  // notification: "$/logTrace"
  auto LspLanguageServer::sendLogTrace(
    LogTraceParams &params
  ) -> void {
    NotificationMessage notification;
    notification.jsonrpc = JSON_RPC_VERSION;
    notification.method = "$/logTrace";
    notification.params = transformer.asMessageParams(params);
    std::unique_ptr<LSPAny> any = transformer.notificationMessageToAny(notification);
    const std::string message = serializer.serialize(*any);
    send(message);
  }

} // namespace LCompilers::LanguageServerProtocol
