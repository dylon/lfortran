// -----------------------------------------------------------------------------
// NOTE: This file was generated from Microsoft's Language Server Protocol (LSP)
// specification. Please do not edit it by hand.
// -----------------------------------------------------------------------------

#include <stdexcept>

#include <lsp/specification.h>
namespace LCompilers::LanguageServerProtocol {
  std::map<
    SemanticTokenTypes,
    std::string
  > SemanticTokenTypesNames = {
    {SemanticTokenTypes::NAMESPACE, "NAMESPACE"},
    {SemanticTokenTypes::TYPE, "TYPE"},
    {SemanticTokenTypes::CLASS, "CLASS"},
    {SemanticTokenTypes::ENUM, "ENUM"},
    {SemanticTokenTypes::INTERFACE, "INTERFACE"},
    {SemanticTokenTypes::STRUCT, "STRUCT"},
    {SemanticTokenTypes::TYPE_PARAMETER, "TYPE_PARAMETER"},
    {SemanticTokenTypes::PARAMETER, "PARAMETER"},
    {SemanticTokenTypes::VARIABLE, "VARIABLE"},
    {SemanticTokenTypes::PROPERTY, "PROPERTY"},
    {SemanticTokenTypes::ENUM_MEMBER, "ENUM_MEMBER"},
    {SemanticTokenTypes::EVENT, "EVENT"},
    {SemanticTokenTypes::FUNCTION, "FUNCTION"},
    {SemanticTokenTypes::METHOD, "METHOD"},
    {SemanticTokenTypes::MACRO, "MACRO"},
    {SemanticTokenTypes::KEYWORD, "KEYWORD"},
    {SemanticTokenTypes::MODIFIER, "MODIFIER"},
    {SemanticTokenTypes::COMMENT, "COMMENT"},
    {SemanticTokenTypes::STRING_TYPE, "STRING_TYPE"},
    {SemanticTokenTypes::NUMBER, "NUMBER"},
    {SemanticTokenTypes::REGEXP, "REGEXP"},
    {SemanticTokenTypes::OPERATOR, "OPERATOR"},
    {SemanticTokenTypes::DECORATOR, "DECORATOR"},
  };

  std::map<
    SemanticTokenTypes,
    string_t
  > SemanticTokenTypesValues = {
    {SemanticTokenTypes::NAMESPACE, "namespace"},
    {SemanticTokenTypes::TYPE, "type"},
    {SemanticTokenTypes::CLASS, "class"},
    {SemanticTokenTypes::ENUM, "enum"},
    {SemanticTokenTypes::INTERFACE, "interface"},
    {SemanticTokenTypes::STRUCT, "struct"},
    {SemanticTokenTypes::TYPE_PARAMETER, "typeParameter"},
    {SemanticTokenTypes::PARAMETER, "parameter"},
    {SemanticTokenTypes::VARIABLE, "variable"},
    {SemanticTokenTypes::PROPERTY, "property"},
    {SemanticTokenTypes::ENUM_MEMBER, "enumMember"},
    {SemanticTokenTypes::EVENT, "event"},
    {SemanticTokenTypes::FUNCTION, "function"},
    {SemanticTokenTypes::METHOD, "method"},
    {SemanticTokenTypes::MACRO, "macro"},
    {SemanticTokenTypes::KEYWORD, "keyword"},
    {SemanticTokenTypes::MODIFIER, "modifier"},
    {SemanticTokenTypes::COMMENT, "comment"},
    {SemanticTokenTypes::STRING_TYPE, "string"},
    {SemanticTokenTypes::NUMBER, "number"},
    {SemanticTokenTypes::REGEXP, "regexp"},
    {SemanticTokenTypes::OPERATOR, "operator"},
    {SemanticTokenTypes::DECORATOR, "decorator"},
  };

  auto semanticTokenTypesByName(
    const std::string &name
  ) -> SemanticTokenTypes {
    for (const auto &[enum_name, enum_value]
        : SemanticTokenTypesNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid SemanticTokenTypes name: " + name
    );
  }

  auto semanticTokenTypesByValue(
    const string_t &value
  ) -> SemanticTokenTypes {
    for (const auto &[enum_name, enum_value]
        : SemanticTokenTypesValues) {
      if (value == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid SemanticTokenTypes value: " + value
    );
  }

  std::map<
    SemanticTokenModifiers,
    std::string
  > SemanticTokenModifiersNames = {
    {SemanticTokenModifiers::DECLARATION, "DECLARATION"},
    {SemanticTokenModifiers::DEFINITION, "DEFINITION"},
    {SemanticTokenModifiers::READONLY, "READONLY"},
    {SemanticTokenModifiers::STATIC, "STATIC"},
    {SemanticTokenModifiers::DEPRECATED, "DEPRECATED"},
    {SemanticTokenModifiers::ABSTRACT, "ABSTRACT"},
    {SemanticTokenModifiers::ASYNC, "ASYNC"},
    {SemanticTokenModifiers::MODIFICATION, "MODIFICATION"},
    {SemanticTokenModifiers::DOCUMENTATION, "DOCUMENTATION"},
    {SemanticTokenModifiers::DEFAULT_LIBRARY, "DEFAULT_LIBRARY"},
  };

  std::map<
    SemanticTokenModifiers,
    string_t
  > SemanticTokenModifiersValues = {
    {SemanticTokenModifiers::DECLARATION, "declaration"},
    {SemanticTokenModifiers::DEFINITION, "definition"},
    {SemanticTokenModifiers::READONLY, "readonly"},
    {SemanticTokenModifiers::STATIC, "static"},
    {SemanticTokenModifiers::DEPRECATED, "deprecated"},
    {SemanticTokenModifiers::ABSTRACT, "abstract"},
    {SemanticTokenModifiers::ASYNC, "async"},
    {SemanticTokenModifiers::MODIFICATION, "modification"},
    {SemanticTokenModifiers::DOCUMENTATION, "documentation"},
    {SemanticTokenModifiers::DEFAULT_LIBRARY, "defaultLibrary"},
  };

  auto semanticTokenModifiersByName(
    const std::string &name
  ) -> SemanticTokenModifiers {
    for (const auto &[enum_name, enum_value]
        : SemanticTokenModifiersNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid SemanticTokenModifiers name: " + name
    );
  }

  auto semanticTokenModifiersByValue(
    const string_t &value
  ) -> SemanticTokenModifiers {
    for (const auto &[enum_name, enum_value]
        : SemanticTokenModifiersValues) {
      if (value == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid SemanticTokenModifiers value: " + value
    );
  }

  std::map<
    DocumentDiagnosticReportKind,
    std::string
  > DocumentDiagnosticReportKindNames = {
    {DocumentDiagnosticReportKind::FULL, "FULL"},
    {DocumentDiagnosticReportKind::UNCHANGED, "UNCHANGED"},
  };

  std::map<
    DocumentDiagnosticReportKind,
    string_t
  > DocumentDiagnosticReportKindValues = {
    {DocumentDiagnosticReportKind::FULL, "full"},
    {DocumentDiagnosticReportKind::UNCHANGED, "unchanged"},
  };

  auto documentDiagnosticReportKindByName(
    const std::string &name
  ) -> DocumentDiagnosticReportKind {
    for (const auto &[enum_name, enum_value]
        : DocumentDiagnosticReportKindNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid DocumentDiagnosticReportKind name: " + name
    );
  }

  auto documentDiagnosticReportKindByValue(
    const string_t &value
  ) -> DocumentDiagnosticReportKind {
    for (const auto &[enum_name, enum_value]
        : DocumentDiagnosticReportKindValues) {
      if (value == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid DocumentDiagnosticReportKind value: " + value
    );
  }

  std::map<
    ErrorCodes,
    std::string
  > ErrorCodesNames = {
    {ErrorCodes::PARSE_ERROR, "PARSE_ERROR"},
    {ErrorCodes::INVALID_REQUEST, "INVALID_REQUEST"},
    {ErrorCodes::METHOD_NOT_FOUND, "METHOD_NOT_FOUND"},
    {ErrorCodes::INVALID_PARAMS, "INVALID_PARAMS"},
    {ErrorCodes::INTERNAL_ERROR, "INTERNAL_ERROR"},
    {ErrorCodes::SERVER_NOT_INITIALIZED, "SERVER_NOT_INITIALIZED"},
    {ErrorCodes::UNKNOWN_ERROR_CODE, "UNKNOWN_ERROR_CODE"},
  };

  auto errorCodesByName(
    const std::string &name
  ) -> ErrorCodes {
    for (const auto &[enum_name, enum_value]
        : ErrorCodesNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid ErrorCodes name: " + name
    );
  }

  auto errorCodesByValue(
    integer_t value
  ) -> ErrorCodes {
    for (const auto &[field_name, field_value]
        : ErrorCodesNames) {
      if (value == static_cast<integer_t>(field_name)) {
        return field_name;
      }
    }
    throw std::invalid_argument(
      "Invalid ErrorCodes value: " + value
    );
  }

  std::map<
    LSPErrorCodes,
    std::string
  > LSPErrorCodesNames = {
    {LSPErrorCodes::REQUEST_FAILED, "REQUEST_FAILED"},
    {LSPErrorCodes::SERVER_CANCELLED, "SERVER_CANCELLED"},
    {LSPErrorCodes::CONTENT_MODIFIED, "CONTENT_MODIFIED"},
    {LSPErrorCodes::REQUEST_CANCELLED, "REQUEST_CANCELLED"},
  };

  auto lspErrorCodesByName(
    const std::string &name
  ) -> LSPErrorCodes {
    for (const auto &[enum_name, enum_value]
        : LSPErrorCodesNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid LSPErrorCodes name: " + name
    );
  }

  auto lspErrorCodesByValue(
    integer_t value
  ) -> LSPErrorCodes {
    for (const auto &[field_name, field_value]
        : LSPErrorCodesNames) {
      if (value == static_cast<integer_t>(field_name)) {
        return field_name;
      }
    }
    throw std::invalid_argument(
      "Invalid LSPErrorCodes value: " + value
    );
  }

  std::map<
    FoldingRangeKind,
    std::string
  > FoldingRangeKindNames = {
    {FoldingRangeKind::COMMENT, "COMMENT"},
    {FoldingRangeKind::IMPORTS, "IMPORTS"},
    {FoldingRangeKind::REGION, "REGION"},
  };

  std::map<
    FoldingRangeKind,
    string_t
  > FoldingRangeKindValues = {
    {FoldingRangeKind::COMMENT, "comment"},
    {FoldingRangeKind::IMPORTS, "imports"},
    {FoldingRangeKind::REGION, "region"},
  };

  auto foldingRangeKindByName(
    const std::string &name
  ) -> FoldingRangeKind {
    for (const auto &[enum_name, enum_value]
        : FoldingRangeKindNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid FoldingRangeKind name: " + name
    );
  }

  auto foldingRangeKindByValue(
    const string_t &value
  ) -> FoldingRangeKind {
    for (const auto &[enum_name, enum_value]
        : FoldingRangeKindValues) {
      if (value == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid FoldingRangeKind value: " + value
    );
  }

  std::map<
    SymbolKind,
    std::string
  > SymbolKindNames = {
    {SymbolKind::FILE, "FILE"},
    {SymbolKind::MODULE, "MODULE"},
    {SymbolKind::NAMESPACE, "NAMESPACE"},
    {SymbolKind::PACKAGE, "PACKAGE"},
    {SymbolKind::CLASS, "CLASS"},
    {SymbolKind::METHOD, "METHOD"},
    {SymbolKind::PROPERTY, "PROPERTY"},
    {SymbolKind::FIELD, "FIELD"},
    {SymbolKind::CONSTRUCTOR, "CONSTRUCTOR"},
    {SymbolKind::ENUM, "ENUM"},
    {SymbolKind::INTERFACE, "INTERFACE"},
    {SymbolKind::FUNCTION, "FUNCTION"},
    {SymbolKind::VARIABLE, "VARIABLE"},
    {SymbolKind::CONSTANT, "CONSTANT"},
    {SymbolKind::STRING_TYPE, "STRING_TYPE"},
    {SymbolKind::NUMBER, "NUMBER"},
    {SymbolKind::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {SymbolKind::ARRAY_TYPE, "ARRAY_TYPE"},
    {SymbolKind::OBJECT_TYPE, "OBJECT_TYPE"},
    {SymbolKind::KEY, "KEY"},
    {SymbolKind::NULL_TYPE, "NULL_TYPE"},
    {SymbolKind::ENUM_MEMBER, "ENUM_MEMBER"},
    {SymbolKind::STRUCT, "STRUCT"},
    {SymbolKind::EVENT, "EVENT"},
    {SymbolKind::OPERATOR, "OPERATOR"},
    {SymbolKind::TYPE_PARAMETER, "TYPE_PARAMETER"},
  };

  auto symbolKindByName(
    const std::string &name
  ) -> SymbolKind {
    for (const auto &[enum_name, enum_value]
        : SymbolKindNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid SymbolKind name: " + name
    );
  }

  auto symbolKindByValue(
    uinteger_t value
  ) -> SymbolKind {
    for (const auto &[field_name, field_value]
        : SymbolKindNames) {
      if (value == static_cast<uinteger_t>(field_name)) {
        return field_name;
      }
    }
    throw std::invalid_argument(
      "Invalid SymbolKind value: " + value
    );
  }

  std::map<
    SymbolTag,
    std::string
  > SymbolTagNames = {
    {SymbolTag::DEPRECATED, "DEPRECATED"},
  };

  auto symbolTagByName(
    const std::string &name
  ) -> SymbolTag {
    for (const auto &[enum_name, enum_value]
        : SymbolTagNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid SymbolTag name: " + name
    );
  }

  auto symbolTagByValue(
    uinteger_t value
  ) -> SymbolTag {
    for (const auto &[field_name, field_value]
        : SymbolTagNames) {
      if (value == static_cast<uinteger_t>(field_name)) {
        return field_name;
      }
    }
    throw std::invalid_argument(
      "Invalid SymbolTag value: " + value
    );
  }

  std::map<
    UniquenessLevel,
    std::string
  > UniquenessLevelNames = {
    {UniquenessLevel::DOCUMENT, "DOCUMENT"},
    {UniquenessLevel::PROJECT, "PROJECT"},
    {UniquenessLevel::GROUP, "GROUP"},
    {UniquenessLevel::SCHEME, "SCHEME"},
    {UniquenessLevel::GLOBAL, "GLOBAL"},
  };

  std::map<
    UniquenessLevel,
    string_t
  > UniquenessLevelValues = {
    {UniquenessLevel::DOCUMENT, "document"},
    {UniquenessLevel::PROJECT, "project"},
    {UniquenessLevel::GROUP, "group"},
    {UniquenessLevel::SCHEME, "scheme"},
    {UniquenessLevel::GLOBAL, "global"},
  };

  auto uniquenessLevelByName(
    const std::string &name
  ) -> UniquenessLevel {
    for (const auto &[enum_name, enum_value]
        : UniquenessLevelNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid UniquenessLevel name: " + name
    );
  }

  auto uniquenessLevelByValue(
    const string_t &value
  ) -> UniquenessLevel {
    for (const auto &[enum_name, enum_value]
        : UniquenessLevelValues) {
      if (value == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid UniquenessLevel value: " + value
    );
  }

  std::map<
    MonikerKind,
    std::string
  > MonikerKindNames = {
    {MonikerKind::IMPORT, "IMPORT"},
    {MonikerKind::EXPORT, "EXPORT"},
    {MonikerKind::LOCAL, "LOCAL"},
  };

  std::map<
    MonikerKind,
    string_t
  > MonikerKindValues = {
    {MonikerKind::IMPORT, "import"},
    {MonikerKind::EXPORT, "export"},
    {MonikerKind::LOCAL, "local"},
  };

  auto monikerKindByName(
    const std::string &name
  ) -> MonikerKind {
    for (const auto &[enum_name, enum_value]
        : MonikerKindNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid MonikerKind name: " + name
    );
  }

  auto monikerKindByValue(
    const string_t &value
  ) -> MonikerKind {
    for (const auto &[enum_name, enum_value]
        : MonikerKindValues) {
      if (value == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid MonikerKind value: " + value
    );
  }

  std::map<
    InlayHintKind,
    std::string
  > InlayHintKindNames = {
    {InlayHintKind::TYPE, "TYPE"},
    {InlayHintKind::PARAMETER, "PARAMETER"},
  };

  auto inlayHintKindByName(
    const std::string &name
  ) -> InlayHintKind {
    for (const auto &[enum_name, enum_value]
        : InlayHintKindNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid InlayHintKind name: " + name
    );
  }

  auto inlayHintKindByValue(
    uinteger_t value
  ) -> InlayHintKind {
    for (const auto &[field_name, field_value]
        : InlayHintKindNames) {
      if (value == static_cast<uinteger_t>(field_name)) {
        return field_name;
      }
    }
    throw std::invalid_argument(
      "Invalid InlayHintKind value: " + value
    );
  }

  std::map<
    MessageType,
    std::string
  > MessageTypeNames = {
    {MessageType::ERROR, "ERROR"},
    {MessageType::WARNING, "WARNING"},
    {MessageType::INFO, "INFO"},
    {MessageType::LOG, "LOG"},
    {MessageType::DEBUG, "DEBUG"},
  };

  auto messageTypeByName(
    const std::string &name
  ) -> MessageType {
    for (const auto &[enum_name, enum_value]
        : MessageTypeNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid MessageType name: " + name
    );
  }

  auto messageTypeByValue(
    uinteger_t value
  ) -> MessageType {
    for (const auto &[field_name, field_value]
        : MessageTypeNames) {
      if (value == static_cast<uinteger_t>(field_name)) {
        return field_name;
      }
    }
    throw std::invalid_argument(
      "Invalid MessageType value: " + value
    );
  }

  std::map<
    TextDocumentSyncKind,
    std::string
  > TextDocumentSyncKindNames = {
    {TextDocumentSyncKind::NONE, "NONE"},
    {TextDocumentSyncKind::FULL, "FULL"},
    {TextDocumentSyncKind::INCREMENTAL, "INCREMENTAL"},
  };

  auto textDocumentSyncKindByName(
    const std::string &name
  ) -> TextDocumentSyncKind {
    for (const auto &[enum_name, enum_value]
        : TextDocumentSyncKindNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid TextDocumentSyncKind name: " + name
    );
  }

  auto textDocumentSyncKindByValue(
    uinteger_t value
  ) -> TextDocumentSyncKind {
    for (const auto &[field_name, field_value]
        : TextDocumentSyncKindNames) {
      if (value == static_cast<uinteger_t>(field_name)) {
        return field_name;
      }
    }
    throw std::invalid_argument(
      "Invalid TextDocumentSyncKind value: " + value
    );
  }

  std::map<
    TextDocumentSaveReason,
    std::string
  > TextDocumentSaveReasonNames = {
    {TextDocumentSaveReason::MANUAL, "MANUAL"},
    {TextDocumentSaveReason::AFTER_DELAY, "AFTER_DELAY"},
    {TextDocumentSaveReason::FOCUS_OUT, "FOCUS_OUT"},
  };

  auto textDocumentSaveReasonByName(
    const std::string &name
  ) -> TextDocumentSaveReason {
    for (const auto &[enum_name, enum_value]
        : TextDocumentSaveReasonNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid TextDocumentSaveReason name: " + name
    );
  }

  auto textDocumentSaveReasonByValue(
    uinteger_t value
  ) -> TextDocumentSaveReason {
    for (const auto &[field_name, field_value]
        : TextDocumentSaveReasonNames) {
      if (value == static_cast<uinteger_t>(field_name)) {
        return field_name;
      }
    }
    throw std::invalid_argument(
      "Invalid TextDocumentSaveReason value: " + value
    );
  }

  std::map<
    CompletionItemKind,
    std::string
  > CompletionItemKindNames = {
    {CompletionItemKind::TEXT, "TEXT"},
    {CompletionItemKind::METHOD, "METHOD"},
    {CompletionItemKind::FUNCTION, "FUNCTION"},
    {CompletionItemKind::CONSTRUCTOR, "CONSTRUCTOR"},
    {CompletionItemKind::FIELD, "FIELD"},
    {CompletionItemKind::VARIABLE, "VARIABLE"},
    {CompletionItemKind::CLASS, "CLASS"},
    {CompletionItemKind::INTERFACE, "INTERFACE"},
    {CompletionItemKind::MODULE, "MODULE"},
    {CompletionItemKind::PROPERTY, "PROPERTY"},
    {CompletionItemKind::UNIT, "UNIT"},
    {CompletionItemKind::VALUE, "VALUE"},
    {CompletionItemKind::ENUM, "ENUM"},
    {CompletionItemKind::KEYWORD, "KEYWORD"},
    {CompletionItemKind::SNIPPET, "SNIPPET"},
    {CompletionItemKind::COLOR, "COLOR"},
    {CompletionItemKind::FILE, "FILE"},
    {CompletionItemKind::REFERENCE, "REFERENCE"},
    {CompletionItemKind::FOLDER, "FOLDER"},
    {CompletionItemKind::ENUM_MEMBER, "ENUM_MEMBER"},
    {CompletionItemKind::CONSTANT, "CONSTANT"},
    {CompletionItemKind::STRUCT, "STRUCT"},
    {CompletionItemKind::EVENT, "EVENT"},
    {CompletionItemKind::OPERATOR, "OPERATOR"},
    {CompletionItemKind::TYPE_PARAMETER, "TYPE_PARAMETER"},
  };

  auto completionItemKindByName(
    const std::string &name
  ) -> CompletionItemKind {
    for (const auto &[enum_name, enum_value]
        : CompletionItemKindNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid CompletionItemKind name: " + name
    );
  }

  auto completionItemKindByValue(
    uinteger_t value
  ) -> CompletionItemKind {
    for (const auto &[field_name, field_value]
        : CompletionItemKindNames) {
      if (value == static_cast<uinteger_t>(field_name)) {
        return field_name;
      }
    }
    throw std::invalid_argument(
      "Invalid CompletionItemKind value: " + value
    );
  }

  std::map<
    CompletionItemTag,
    std::string
  > CompletionItemTagNames = {
    {CompletionItemTag::DEPRECATED, "DEPRECATED"},
  };

  auto completionItemTagByName(
    const std::string &name
  ) -> CompletionItemTag {
    for (const auto &[enum_name, enum_value]
        : CompletionItemTagNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid CompletionItemTag name: " + name
    );
  }

  auto completionItemTagByValue(
    uinteger_t value
  ) -> CompletionItemTag {
    for (const auto &[field_name, field_value]
        : CompletionItemTagNames) {
      if (value == static_cast<uinteger_t>(field_name)) {
        return field_name;
      }
    }
    throw std::invalid_argument(
      "Invalid CompletionItemTag value: " + value
    );
  }

  std::map<
    InsertTextFormat,
    std::string
  > InsertTextFormatNames = {
    {InsertTextFormat::PLAIN_TEXT, "PLAIN_TEXT"},
    {InsertTextFormat::SNIPPET, "SNIPPET"},
  };

  auto insertTextFormatByName(
    const std::string &name
  ) -> InsertTextFormat {
    for (const auto &[enum_name, enum_value]
        : InsertTextFormatNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid InsertTextFormat name: " + name
    );
  }

  auto insertTextFormatByValue(
    uinteger_t value
  ) -> InsertTextFormat {
    for (const auto &[field_name, field_value]
        : InsertTextFormatNames) {
      if (value == static_cast<uinteger_t>(field_name)) {
        return field_name;
      }
    }
    throw std::invalid_argument(
      "Invalid InsertTextFormat value: " + value
    );
  }

  std::map<
    InsertTextMode,
    std::string
  > InsertTextModeNames = {
    {InsertTextMode::AS_IS, "AS_IS"},
    {InsertTextMode::ADJUST_INDENTATION, "ADJUST_INDENTATION"},
  };

  auto insertTextModeByName(
    const std::string &name
  ) -> InsertTextMode {
    for (const auto &[enum_name, enum_value]
        : InsertTextModeNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid InsertTextMode name: " + name
    );
  }

  auto insertTextModeByValue(
    uinteger_t value
  ) -> InsertTextMode {
    for (const auto &[field_name, field_value]
        : InsertTextModeNames) {
      if (value == static_cast<uinteger_t>(field_name)) {
        return field_name;
      }
    }
    throw std::invalid_argument(
      "Invalid InsertTextMode value: " + value
    );
  }

  std::map<
    DocumentHighlightKind,
    std::string
  > DocumentHighlightKindNames = {
    {DocumentHighlightKind::TEXT, "TEXT"},
    {DocumentHighlightKind::READ, "READ"},
    {DocumentHighlightKind::WRITE, "WRITE"},
  };

  auto documentHighlightKindByName(
    const std::string &name
  ) -> DocumentHighlightKind {
    for (const auto &[enum_name, enum_value]
        : DocumentHighlightKindNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid DocumentHighlightKind name: " + name
    );
  }

  auto documentHighlightKindByValue(
    uinteger_t value
  ) -> DocumentHighlightKind {
    for (const auto &[field_name, field_value]
        : DocumentHighlightKindNames) {
      if (value == static_cast<uinteger_t>(field_name)) {
        return field_name;
      }
    }
    throw std::invalid_argument(
      "Invalid DocumentHighlightKind value: " + value
    );
  }

  std::map<
    CodeActionKind,
    std::string
  > CodeActionKindNames = {
    {CodeActionKind::EMPTY, "EMPTY"},
    {CodeActionKind::QUICK_FIX, "QUICK_FIX"},
    {CodeActionKind::REFACTOR, "REFACTOR"},
    {CodeActionKind::REFACTOR_EXTRACT, "REFACTOR_EXTRACT"},
    {CodeActionKind::REFACTOR_INLINE, "REFACTOR_INLINE"},
    {CodeActionKind::REFACTOR_REWRITE, "REFACTOR_REWRITE"},
    {CodeActionKind::SOURCE, "SOURCE"},
    {CodeActionKind::SOURCE_ORGANIZE_IMPORTS, "SOURCE_ORGANIZE_IMPORTS"},
    {CodeActionKind::SOURCE_FIX_ALL, "SOURCE_FIX_ALL"},
  };

  std::map<
    CodeActionKind,
    string_t
  > CodeActionKindValues = {
    {CodeActionKind::EMPTY, ""},
    {CodeActionKind::QUICK_FIX, "quickfix"},
    {CodeActionKind::REFACTOR, "refactor"},
    {CodeActionKind::REFACTOR_EXTRACT, "refactor.extract"},
    {CodeActionKind::REFACTOR_INLINE, "refactor.inline"},
    {CodeActionKind::REFACTOR_REWRITE, "refactor.rewrite"},
    {CodeActionKind::SOURCE, "source"},
    {CodeActionKind::SOURCE_ORGANIZE_IMPORTS, "source.organizeImports"},
    {CodeActionKind::SOURCE_FIX_ALL, "source.fixAll"},
  };

  auto codeActionKindByName(
    const std::string &name
  ) -> CodeActionKind {
    for (const auto &[enum_name, enum_value]
        : CodeActionKindNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid CodeActionKind name: " + name
    );
  }

  auto codeActionKindByValue(
    const string_t &value
  ) -> CodeActionKind {
    for (const auto &[enum_name, enum_value]
        : CodeActionKindValues) {
      if (value == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid CodeActionKind value: " + value
    );
  }

  std::map<
    TraceValues,
    std::string
  > TraceValuesNames = {
    {TraceValues::OFF, "OFF"},
    {TraceValues::MESSAGES, "MESSAGES"},
    {TraceValues::VERBOSE, "VERBOSE"},
  };

  std::map<
    TraceValues,
    string_t
  > TraceValuesValues = {
    {TraceValues::OFF, "off"},
    {TraceValues::MESSAGES, "messages"},
    {TraceValues::VERBOSE, "verbose"},
  };

  auto traceValuesByName(
    const std::string &name
  ) -> TraceValues {
    for (const auto &[enum_name, enum_value]
        : TraceValuesNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid TraceValues name: " + name
    );
  }

  auto traceValuesByValue(
    const string_t &value
  ) -> TraceValues {
    for (const auto &[enum_name, enum_value]
        : TraceValuesValues) {
      if (value == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid TraceValues value: " + value
    );
  }

  std::map<
    MarkupKind,
    std::string
  > MarkupKindNames = {
    {MarkupKind::PLAIN_TEXT, "PLAIN_TEXT"},
    {MarkupKind::MARKDOWN, "MARKDOWN"},
  };

  std::map<
    MarkupKind,
    string_t
  > MarkupKindValues = {
    {MarkupKind::PLAIN_TEXT, "plaintext"},
    {MarkupKind::MARKDOWN, "markdown"},
  };

  auto markupKindByName(
    const std::string &name
  ) -> MarkupKind {
    for (const auto &[enum_name, enum_value]
        : MarkupKindNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid MarkupKind name: " + name
    );
  }

  auto markupKindByValue(
    const string_t &value
  ) -> MarkupKind {
    for (const auto &[enum_name, enum_value]
        : MarkupKindValues) {
      if (value == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid MarkupKind value: " + value
    );
  }

  std::map<
    InlineCompletionTriggerKind,
    std::string
  > InlineCompletionTriggerKindNames = {
    {InlineCompletionTriggerKind::INVOKED, "INVOKED"},
    {InlineCompletionTriggerKind::AUTOMATIC, "AUTOMATIC"},
  };

  auto inlineCompletionTriggerKindByName(
    const std::string &name
  ) -> InlineCompletionTriggerKind {
    for (const auto &[enum_name, enum_value]
        : InlineCompletionTriggerKindNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid InlineCompletionTriggerKind name: " + name
    );
  }

  auto inlineCompletionTriggerKindByValue(
    uinteger_t value
  ) -> InlineCompletionTriggerKind {
    for (const auto &[field_name, field_value]
        : InlineCompletionTriggerKindNames) {
      if (value == static_cast<uinteger_t>(field_name)) {
        return field_name;
      }
    }
    throw std::invalid_argument(
      "Invalid InlineCompletionTriggerKind value: " + value
    );
  }

  std::map<
    PositionEncodingKind,
    std::string
  > PositionEncodingKindNames = {
    {PositionEncodingKind::UTF8, "UTF8"},
    {PositionEncodingKind::UTF16, "UTF16"},
    {PositionEncodingKind::UTF32, "UTF32"},
  };

  std::map<
    PositionEncodingKind,
    string_t
  > PositionEncodingKindValues = {
    {PositionEncodingKind::UTF8, "utf-8"},
    {PositionEncodingKind::UTF16, "utf-16"},
    {PositionEncodingKind::UTF32, "utf-32"},
  };

  auto positionEncodingKindByName(
    const std::string &name
  ) -> PositionEncodingKind {
    for (const auto &[enum_name, enum_value]
        : PositionEncodingKindNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid PositionEncodingKind name: " + name
    );
  }

  auto positionEncodingKindByValue(
    const string_t &value
  ) -> PositionEncodingKind {
    for (const auto &[enum_name, enum_value]
        : PositionEncodingKindValues) {
      if (value == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid PositionEncodingKind value: " + value
    );
  }

  std::map<
    FileChangeType,
    std::string
  > FileChangeTypeNames = {
    {FileChangeType::CREATED, "CREATED"},
    {FileChangeType::CHANGED, "CHANGED"},
    {FileChangeType::DELETED, "DELETED"},
  };

  auto fileChangeTypeByName(
    const std::string &name
  ) -> FileChangeType {
    for (const auto &[enum_name, enum_value]
        : FileChangeTypeNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid FileChangeType name: " + name
    );
  }

  auto fileChangeTypeByValue(
    uinteger_t value
  ) -> FileChangeType {
    for (const auto &[field_name, field_value]
        : FileChangeTypeNames) {
      if (value == static_cast<uinteger_t>(field_name)) {
        return field_name;
      }
    }
    throw std::invalid_argument(
      "Invalid FileChangeType value: " + value
    );
  }

  std::map<
    WatchKind,
    std::string
  > WatchKindNames = {
    {WatchKind::CREATE, "CREATE"},
    {WatchKind::CHANGE, "CHANGE"},
    {WatchKind::DELETE, "DELETE"},
  };

  auto watchKindByName(
    const std::string &name
  ) -> WatchKind {
    for (const auto &[enum_name, enum_value]
        : WatchKindNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid WatchKind name: " + name
    );
  }

  auto watchKindByValue(
    uinteger_t value
  ) -> WatchKind {
    for (const auto &[field_name, field_value]
        : WatchKindNames) {
      if (value == static_cast<uinteger_t>(field_name)) {
        return field_name;
      }
    }
    throw std::invalid_argument(
      "Invalid WatchKind value: " + value
    );
  }

  std::map<
    DiagnosticSeverity,
    std::string
  > DiagnosticSeverityNames = {
    {DiagnosticSeverity::ERROR, "ERROR"},
    {DiagnosticSeverity::WARNING, "WARNING"},
    {DiagnosticSeverity::INFORMATION, "INFORMATION"},
    {DiagnosticSeverity::HINT, "HINT"},
  };

  auto diagnosticSeverityByName(
    const std::string &name
  ) -> DiagnosticSeverity {
    for (const auto &[enum_name, enum_value]
        : DiagnosticSeverityNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid DiagnosticSeverity name: " + name
    );
  }

  auto diagnosticSeverityByValue(
    uinteger_t value
  ) -> DiagnosticSeverity {
    for (const auto &[field_name, field_value]
        : DiagnosticSeverityNames) {
      if (value == static_cast<uinteger_t>(field_name)) {
        return field_name;
      }
    }
    throw std::invalid_argument(
      "Invalid DiagnosticSeverity value: " + value
    );
  }

  std::map<
    DiagnosticTag,
    std::string
  > DiagnosticTagNames = {
    {DiagnosticTag::UNNECESSARY, "UNNECESSARY"},
    {DiagnosticTag::DEPRECATED, "DEPRECATED"},
  };

  auto diagnosticTagByName(
    const std::string &name
  ) -> DiagnosticTag {
    for (const auto &[enum_name, enum_value]
        : DiagnosticTagNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid DiagnosticTag name: " + name
    );
  }

  auto diagnosticTagByValue(
    uinteger_t value
  ) -> DiagnosticTag {
    for (const auto &[field_name, field_value]
        : DiagnosticTagNames) {
      if (value == static_cast<uinteger_t>(field_name)) {
        return field_name;
      }
    }
    throw std::invalid_argument(
      "Invalid DiagnosticTag value: " + value
    );
  }

  std::map<
    CompletionTriggerKind,
    std::string
  > CompletionTriggerKindNames = {
    {CompletionTriggerKind::INVOKED, "INVOKED"},
    {CompletionTriggerKind::TRIGGER_CHARACTER, "TRIGGER_CHARACTER"},
    {CompletionTriggerKind::TRIGGER_FOR_INCOMPLETE_COMPLETIONS, "TRIGGER_FOR_INCOMPLETE_COMPLETIONS"},
  };

  auto completionTriggerKindByName(
    const std::string &name
  ) -> CompletionTriggerKind {
    for (const auto &[enum_name, enum_value]
        : CompletionTriggerKindNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid CompletionTriggerKind name: " + name
    );
  }

  auto completionTriggerKindByValue(
    uinteger_t value
  ) -> CompletionTriggerKind {
    for (const auto &[field_name, field_value]
        : CompletionTriggerKindNames) {
      if (value == static_cast<uinteger_t>(field_name)) {
        return field_name;
      }
    }
    throw std::invalid_argument(
      "Invalid CompletionTriggerKind value: " + value
    );
  }

  std::map<
    SignatureHelpTriggerKind,
    std::string
  > SignatureHelpTriggerKindNames = {
    {SignatureHelpTriggerKind::INVOKED, "INVOKED"},
    {SignatureHelpTriggerKind::TRIGGER_CHARACTER, "TRIGGER_CHARACTER"},
    {SignatureHelpTriggerKind::CONTENT_CHANGE, "CONTENT_CHANGE"},
  };

  auto signatureHelpTriggerKindByName(
    const std::string &name
  ) -> SignatureHelpTriggerKind {
    for (const auto &[enum_name, enum_value]
        : SignatureHelpTriggerKindNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid SignatureHelpTriggerKind name: " + name
    );
  }

  auto signatureHelpTriggerKindByValue(
    uinteger_t value
  ) -> SignatureHelpTriggerKind {
    for (const auto &[field_name, field_value]
        : SignatureHelpTriggerKindNames) {
      if (value == static_cast<uinteger_t>(field_name)) {
        return field_name;
      }
    }
    throw std::invalid_argument(
      "Invalid SignatureHelpTriggerKind value: " + value
    );
  }

  std::map<
    CodeActionTriggerKind,
    std::string
  > CodeActionTriggerKindNames = {
    {CodeActionTriggerKind::INVOKED, "INVOKED"},
    {CodeActionTriggerKind::AUTOMATIC, "AUTOMATIC"},
  };

  auto codeActionTriggerKindByName(
    const std::string &name
  ) -> CodeActionTriggerKind {
    for (const auto &[enum_name, enum_value]
        : CodeActionTriggerKindNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid CodeActionTriggerKind name: " + name
    );
  }

  auto codeActionTriggerKindByValue(
    uinteger_t value
  ) -> CodeActionTriggerKind {
    for (const auto &[field_name, field_value]
        : CodeActionTriggerKindNames) {
      if (value == static_cast<uinteger_t>(field_name)) {
        return field_name;
      }
    }
    throw std::invalid_argument(
      "Invalid CodeActionTriggerKind value: " + value
    );
  }

  std::map<
    FileOperationPatternKind,
    std::string
  > FileOperationPatternKindNames = {
    {FileOperationPatternKind::FILE, "FILE"},
    {FileOperationPatternKind::FOLDER, "FOLDER"},
  };

  std::map<
    FileOperationPatternKind,
    string_t
  > FileOperationPatternKindValues = {
    {FileOperationPatternKind::FILE, "file"},
    {FileOperationPatternKind::FOLDER, "folder"},
  };

  auto fileOperationPatternKindByName(
    const std::string &name
  ) -> FileOperationPatternKind {
    for (const auto &[enum_name, enum_value]
        : FileOperationPatternKindNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid FileOperationPatternKind name: " + name
    );
  }

  auto fileOperationPatternKindByValue(
    const string_t &value
  ) -> FileOperationPatternKind {
    for (const auto &[enum_name, enum_value]
        : FileOperationPatternKindValues) {
      if (value == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid FileOperationPatternKind value: " + value
    );
  }

  std::map<
    NotebookCellKind,
    std::string
  > NotebookCellKindNames = {
    {NotebookCellKind::MARKUP, "MARKUP"},
    {NotebookCellKind::CODE, "CODE"},
  };

  auto notebookCellKindByName(
    const std::string &name
  ) -> NotebookCellKind {
    for (const auto &[enum_name, enum_value]
        : NotebookCellKindNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid NotebookCellKind name: " + name
    );
  }

  auto notebookCellKindByValue(
    uinteger_t value
  ) -> NotebookCellKind {
    for (const auto &[field_name, field_value]
        : NotebookCellKindNames) {
      if (value == static_cast<uinteger_t>(field_name)) {
        return field_name;
      }
    }
    throw std::invalid_argument(
      "Invalid NotebookCellKind value: " + value
    );
  }

  std::map<
    ResourceOperationKind,
    std::string
  > ResourceOperationKindNames = {
    {ResourceOperationKind::CREATE, "CREATE"},
    {ResourceOperationKind::RENAME, "RENAME"},
    {ResourceOperationKind::DELETE, "DELETE"},
  };

  std::map<
    ResourceOperationKind,
    string_t
  > ResourceOperationKindValues = {
    {ResourceOperationKind::CREATE, "create"},
    {ResourceOperationKind::RENAME, "rename"},
    {ResourceOperationKind::DELETE, "delete"},
  };

  auto resourceOperationKindByName(
    const std::string &name
  ) -> ResourceOperationKind {
    for (const auto &[enum_name, enum_value]
        : ResourceOperationKindNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid ResourceOperationKind name: " + name
    );
  }

  auto resourceOperationKindByValue(
    const string_t &value
  ) -> ResourceOperationKind {
    for (const auto &[enum_name, enum_value]
        : ResourceOperationKindValues) {
      if (value == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid ResourceOperationKind value: " + value
    );
  }

  std::map<
    FailureHandlingKind,
    std::string
  > FailureHandlingKindNames = {
    {FailureHandlingKind::ABORT, "ABORT"},
    {FailureHandlingKind::TRANSACTIONAL, "TRANSACTIONAL"},
    {FailureHandlingKind::TEXT_ONLY_TRANSACTIONAL, "TEXT_ONLY_TRANSACTIONAL"},
    {FailureHandlingKind::UNDO, "UNDO"},
  };

  std::map<
    FailureHandlingKind,
    string_t
  > FailureHandlingKindValues = {
    {FailureHandlingKind::ABORT, "abort"},
    {FailureHandlingKind::TRANSACTIONAL, "transactional"},
    {FailureHandlingKind::TEXT_ONLY_TRANSACTIONAL, "textOnlyTransactional"},
    {FailureHandlingKind::UNDO, "undo"},
  };

  auto failureHandlingKindByName(
    const std::string &name
  ) -> FailureHandlingKind {
    for (const auto &[enum_name, enum_value]
        : FailureHandlingKindNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid FailureHandlingKind name: " + name
    );
  }

  auto failureHandlingKindByValue(
    const string_t &value
  ) -> FailureHandlingKind {
    for (const auto &[enum_name, enum_value]
        : FailureHandlingKindValues) {
      if (value == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid FailureHandlingKind value: " + value
    );
  }

  std::map<
    PrepareSupportDefaultBehavior,
    std::string
  > PrepareSupportDefaultBehaviorNames = {
    {PrepareSupportDefaultBehavior::IDENTIFIER, "IDENTIFIER"},
  };

  auto prepareSupportDefaultBehaviorByName(
    const std::string &name
  ) -> PrepareSupportDefaultBehavior {
    for (const auto &[enum_name, enum_value]
        : PrepareSupportDefaultBehaviorNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid PrepareSupportDefaultBehavior name: " + name
    );
  }

  auto prepareSupportDefaultBehaviorByValue(
    uinteger_t value
  ) -> PrepareSupportDefaultBehavior {
    for (const auto &[field_name, field_value]
        : PrepareSupportDefaultBehaviorNames) {
      if (value == static_cast<uinteger_t>(field_name)) {
        return field_name;
      }
    }
    throw std::invalid_argument(
      "Invalid PrepareSupportDefaultBehavior value: " + value
    );
  }

  std::map<
    TokenFormat,
    std::string
  > TokenFormatNames = {
    {TokenFormat::RELATIVE, "RELATIVE"},
  };

  std::map<
    TokenFormat,
    string_t
  > TokenFormatValues = {
    {TokenFormat::RELATIVE, "relative"},
  };

  auto tokenFormatByName(
    const std::string &name
  ) -> TokenFormat {
    for (const auto &[enum_name, enum_value]
        : TokenFormatNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid TokenFormat name: " + name
    );
  }

  auto tokenFormatByValue(
    const string_t &value
  ) -> TokenFormat {
    for (const auto &[enum_name, enum_value]
        : TokenFormatValues) {
      if (value == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid TokenFormat value: " + value
    );
  }

  std::map<TextDocumentRegistrationOptions_documentSelectorType, std::string> TextDocumentRegistrationOptions_documentSelectorTypeNames = {
    {TextDocumentRegistrationOptions_documentSelectorType::DOCUMENT_SELECTOR, "DOCUMENT_SELECTOR"},
    {TextDocumentRegistrationOptions_documentSelectorType::NULL_TYPE, "NULL_TYPE"},
  };

  std::map<WorkspaceEdit_documentChangesType, std::string> WorkspaceEdit_documentChangesTypeNames = {
    {WorkspaceEdit_documentChangesType::TEXT_DOCUMENT_EDIT, "TEXT_DOCUMENT_EDIT"},
    {WorkspaceEdit_documentChangesType::CREATE_FILE, "CREATE_FILE"},
    {WorkspaceEdit_documentChangesType::RENAME_FILE, "RENAME_FILE"},
    {WorkspaceEdit_documentChangesType::DELETE_FILE, "DELETE_FILE"},
  };

  std::map<InlayHint_labelType, std::string> InlayHint_labelTypeNames = {
    {InlayHint_labelType::STRING_TYPE, "STRING_TYPE"},
    {InlayHint_labelType::INLAY_HINT_LABEL_PART_ARRAY, "INLAY_HINT_LABEL_PART_ARRAY"},
  };

  std::map<InlayHint_tooltipType, std::string> InlayHint_tooltipTypeNames = {
    {InlayHint_tooltipType::STRING_TYPE, "STRING_TYPE"},
    {InlayHint_tooltipType::MARKUP_CONTENT, "MARKUP_CONTENT"},
  };

  std::map<DocumentDiagnosticReportPartialResult_relatedDocumentsType, std::string> DocumentDiagnosticReportPartialResult_relatedDocumentsTypeNames = {
    {DocumentDiagnosticReportPartialResult_relatedDocumentsType::FULL_DOCUMENT_DIAGNOSTIC_REPORT, "FULL_DOCUMENT_DIAGNOSTIC_REPORT"},
    {DocumentDiagnosticReportPartialResult_relatedDocumentsType::UNCHANGED_DOCUMENT_DIAGNOSTIC_REPORT, "UNCHANGED_DOCUMENT_DIAGNOSTIC_REPORT"},
  };

  std::map<InlineCompletionItem_insertTextType, std::string> InlineCompletionItem_insertTextTypeNames = {
    {InlineCompletionItem_insertTextType::STRING_TYPE, "STRING_TYPE"},
    {InlineCompletionItem_insertTextType::STRING_VALUE, "STRING_VALUE"},
  };

  std::map<DidChangeConfigurationRegistrationOptions_sectionType, std::string> DidChangeConfigurationRegistrationOptions_sectionTypeNames = {
    {DidChangeConfigurationRegistrationOptions_sectionType::STRING_TYPE, "STRING_TYPE"},
    {DidChangeConfigurationRegistrationOptions_sectionType::STRING_TYPE_ARRAY, "STRING_TYPE_ARRAY"},
  };

  std::map<CompletionItem_documentationType, std::string> CompletionItem_documentationTypeNames = {
    {CompletionItem_documentationType::STRING_TYPE, "STRING_TYPE"},
    {CompletionItem_documentationType::MARKUP_CONTENT, "MARKUP_CONTENT"},
  };

  std::map<CompletionItem_textEditType, std::string> CompletionItem_textEditTypeNames = {
    {CompletionItem_textEditType::TEXT_EDIT, "TEXT_EDIT"},
    {CompletionItem_textEditType::INSERT_REPLACE_EDIT, "INSERT_REPLACE_EDIT"},
  };

  std::map<CompletionList_itemDefaults_editRangeType, std::string> CompletionList_itemDefaults_editRangeTypeNames = {
    {CompletionList_itemDefaults_editRangeType::RANGE, "RANGE"},
    {CompletionList_itemDefaults_editRangeType::COMPLETION_LIST_ITEM_DEFAULTS_EDIT_RANGE_1, "COMPLETION_LIST_ITEM_DEFAULTS_EDIT_RANGE_1"},
  };

  std::map<Hover_contentsType, std::string> Hover_contentsTypeNames = {
    {Hover_contentsType::MARKUP_CONTENT, "MARKUP_CONTENT"},
    {Hover_contentsType::MARKED_STRING, "MARKED_STRING"},
    {Hover_contentsType::MARKED_STRING_ARRAY, "MARKED_STRING_ARRAY"},
  };

  std::map<WorkspaceSymbol_locationType, std::string> WorkspaceSymbol_locationTypeNames = {
    {WorkspaceSymbol_locationType::LOCATION, "LOCATION"},
    {WorkspaceSymbol_locationType::WORKSPACE_SYMBOL_LOCATION_1, "WORKSPACE_SYMBOL_LOCATION_1"},
  };

  std::map<CancelParams_idType, std::string> CancelParams_idTypeNames = {
    {CancelParams_idType::INTEGER_TYPE, "INTEGER_TYPE"},
    {CancelParams_idType::STRING_TYPE, "STRING_TYPE"},
  };

  std::map<SemanticTokensOptions_rangeType, std::string> SemanticTokensOptions_rangeTypeNames = {
    {SemanticTokensOptions_rangeType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {SemanticTokensOptions_rangeType::SEMANTIC_TOKENS_OPTIONS_RANGE_1, "SEMANTIC_TOKENS_OPTIONS_RANGE_1"},
  };

  std::map<SemanticTokensOptions_fullType, std::string> SemanticTokensOptions_fullTypeNames = {
    {SemanticTokensOptions_fullType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {SemanticTokensOptions_fullType::SEMANTIC_TOKENS_OPTIONS_FULL_1, "SEMANTIC_TOKENS_OPTIONS_FULL_1"},
  };

  std::map<TextDocumentEdit_editsType, std::string> TextDocumentEdit_editsTypeNames = {
    {TextDocumentEdit_editsType::TEXT_EDIT, "TEXT_EDIT"},
    {TextDocumentEdit_editsType::ANNOTATED_TEXT_EDIT, "ANNOTATED_TEXT_EDIT"},
  };

  std::map<InlayHintLabelPart_tooltipType, std::string> InlayHintLabelPart_tooltipTypeNames = {
    {InlayHintLabelPart_tooltipType::STRING_TYPE, "STRING_TYPE"},
    {InlayHintLabelPart_tooltipType::MARKUP_CONTENT, "MARKUP_CONTENT"},
  };

  std::map<RelatedFullDocumentDiagnosticReport_relatedDocumentsType, std::string> RelatedFullDocumentDiagnosticReport_relatedDocumentsTypeNames = {
    {RelatedFullDocumentDiagnosticReport_relatedDocumentsType::FULL_DOCUMENT_DIAGNOSTIC_REPORT, "FULL_DOCUMENT_DIAGNOSTIC_REPORT"},
    {RelatedFullDocumentDiagnosticReport_relatedDocumentsType::UNCHANGED_DOCUMENT_DIAGNOSTIC_REPORT, "UNCHANGED_DOCUMENT_DIAGNOSTIC_REPORT"},
  };

  std::map<RelatedUnchangedDocumentDiagnosticReport_relatedDocumentsType, std::string> RelatedUnchangedDocumentDiagnosticReport_relatedDocumentsTypeNames = {
    {RelatedUnchangedDocumentDiagnosticReport_relatedDocumentsType::FULL_DOCUMENT_DIAGNOSTIC_REPORT, "FULL_DOCUMENT_DIAGNOSTIC_REPORT"},
    {RelatedUnchangedDocumentDiagnosticReport_relatedDocumentsType::UNCHANGED_DOCUMENT_DIAGNOSTIC_REPORT, "UNCHANGED_DOCUMENT_DIAGNOSTIC_REPORT"},
  };

  std::map<_InitializeParams_processIdType, std::string> _InitializeParams_processIdTypeNames = {
    {_InitializeParams_processIdType::INTEGER_TYPE, "INTEGER_TYPE"},
    {_InitializeParams_processIdType::NULL_TYPE, "NULL_TYPE"},
  };

  std::map<_InitializeParams_rootPathType, std::string> _InitializeParams_rootPathTypeNames = {
    {_InitializeParams_rootPathType::STRING_TYPE, "STRING_TYPE"},
    {_InitializeParams_rootPathType::NULL_TYPE, "NULL_TYPE"},
  };

  std::map<_InitializeParams_rootUriType, std::string> _InitializeParams_rootUriTypeNames = {
    {_InitializeParams_rootUriType::DOCUMENT_URI, "DOCUMENT_URI"},
    {_InitializeParams_rootUriType::NULL_TYPE, "NULL_TYPE"},
  };

  std::map<WorkspaceFoldersInitializeParams_workspaceFoldersType, std::string> WorkspaceFoldersInitializeParams_workspaceFoldersTypeNames = {
    {WorkspaceFoldersInitializeParams_workspaceFoldersType::WORKSPACE_FOLDER_ARRAY, "WORKSPACE_FOLDER_ARRAY"},
    {WorkspaceFoldersInitializeParams_workspaceFoldersType::NULL_TYPE, "NULL_TYPE"},
  };

  std::map<ServerCapabilities_textDocumentSyncType, std::string> ServerCapabilities_textDocumentSyncTypeNames = {
    {ServerCapabilities_textDocumentSyncType::TEXT_DOCUMENT_SYNC_OPTIONS, "TEXT_DOCUMENT_SYNC_OPTIONS"},
    {ServerCapabilities_textDocumentSyncType::TEXT_DOCUMENT_SYNC_KIND, "TEXT_DOCUMENT_SYNC_KIND"},
  };

  std::map<ServerCapabilities_notebookDocumentSyncType, std::string> ServerCapabilities_notebookDocumentSyncTypeNames = {
    {ServerCapabilities_notebookDocumentSyncType::NOTEBOOK_DOCUMENT_SYNC_OPTIONS, "NOTEBOOK_DOCUMENT_SYNC_OPTIONS"},
    {ServerCapabilities_notebookDocumentSyncType::NOTEBOOK_DOCUMENT_SYNC_REGISTRATION_OPTIONS, "NOTEBOOK_DOCUMENT_SYNC_REGISTRATION_OPTIONS"},
  };

  std::map<ServerCapabilities_hoverProviderType, std::string> ServerCapabilities_hoverProviderTypeNames = {
    {ServerCapabilities_hoverProviderType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {ServerCapabilities_hoverProviderType::HOVER_OPTIONS, "HOVER_OPTIONS"},
  };

  std::map<ServerCapabilities_declarationProviderType, std::string> ServerCapabilities_declarationProviderTypeNames = {
    {ServerCapabilities_declarationProviderType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {ServerCapabilities_declarationProviderType::DECLARATION_OPTIONS, "DECLARATION_OPTIONS"},
    {ServerCapabilities_declarationProviderType::DECLARATION_REGISTRATION_OPTIONS, "DECLARATION_REGISTRATION_OPTIONS"},
  };

  std::map<ServerCapabilities_definitionProviderType, std::string> ServerCapabilities_definitionProviderTypeNames = {
    {ServerCapabilities_definitionProviderType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {ServerCapabilities_definitionProviderType::DEFINITION_OPTIONS, "DEFINITION_OPTIONS"},
  };

  std::map<ServerCapabilities_typeDefinitionProviderType, std::string> ServerCapabilities_typeDefinitionProviderTypeNames = {
    {ServerCapabilities_typeDefinitionProviderType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {ServerCapabilities_typeDefinitionProviderType::TYPE_DEFINITION_OPTIONS, "TYPE_DEFINITION_OPTIONS"},
    {ServerCapabilities_typeDefinitionProviderType::TYPE_DEFINITION_REGISTRATION_OPTIONS, "TYPE_DEFINITION_REGISTRATION_OPTIONS"},
  };

  std::map<ServerCapabilities_implementationProviderType, std::string> ServerCapabilities_implementationProviderTypeNames = {
    {ServerCapabilities_implementationProviderType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {ServerCapabilities_implementationProviderType::IMPLEMENTATION_OPTIONS, "IMPLEMENTATION_OPTIONS"},
    {ServerCapabilities_implementationProviderType::IMPLEMENTATION_REGISTRATION_OPTIONS, "IMPLEMENTATION_REGISTRATION_OPTIONS"},
  };

  std::map<ServerCapabilities_referencesProviderType, std::string> ServerCapabilities_referencesProviderTypeNames = {
    {ServerCapabilities_referencesProviderType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {ServerCapabilities_referencesProviderType::REFERENCE_OPTIONS, "REFERENCE_OPTIONS"},
  };

  std::map<ServerCapabilities_documentHighlightProviderType, std::string> ServerCapabilities_documentHighlightProviderTypeNames = {
    {ServerCapabilities_documentHighlightProviderType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {ServerCapabilities_documentHighlightProviderType::DOCUMENT_HIGHLIGHT_OPTIONS, "DOCUMENT_HIGHLIGHT_OPTIONS"},
  };

  std::map<ServerCapabilities_documentSymbolProviderType, std::string> ServerCapabilities_documentSymbolProviderTypeNames = {
    {ServerCapabilities_documentSymbolProviderType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {ServerCapabilities_documentSymbolProviderType::DOCUMENT_SYMBOL_OPTIONS, "DOCUMENT_SYMBOL_OPTIONS"},
  };

  std::map<ServerCapabilities_codeActionProviderType, std::string> ServerCapabilities_codeActionProviderTypeNames = {
    {ServerCapabilities_codeActionProviderType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {ServerCapabilities_codeActionProviderType::CODE_ACTION_OPTIONS, "CODE_ACTION_OPTIONS"},
  };

  std::map<ServerCapabilities_colorProviderType, std::string> ServerCapabilities_colorProviderTypeNames = {
    {ServerCapabilities_colorProviderType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {ServerCapabilities_colorProviderType::DOCUMENT_COLOR_OPTIONS, "DOCUMENT_COLOR_OPTIONS"},
    {ServerCapabilities_colorProviderType::DOCUMENT_COLOR_REGISTRATION_OPTIONS, "DOCUMENT_COLOR_REGISTRATION_OPTIONS"},
  };

  std::map<ServerCapabilities_workspaceSymbolProviderType, std::string> ServerCapabilities_workspaceSymbolProviderTypeNames = {
    {ServerCapabilities_workspaceSymbolProviderType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {ServerCapabilities_workspaceSymbolProviderType::WORKSPACE_SYMBOL_OPTIONS, "WORKSPACE_SYMBOL_OPTIONS"},
  };

  std::map<ServerCapabilities_documentFormattingProviderType, std::string> ServerCapabilities_documentFormattingProviderTypeNames = {
    {ServerCapabilities_documentFormattingProviderType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {ServerCapabilities_documentFormattingProviderType::DOCUMENT_FORMATTING_OPTIONS, "DOCUMENT_FORMATTING_OPTIONS"},
  };

  std::map<ServerCapabilities_documentRangeFormattingProviderType, std::string> ServerCapabilities_documentRangeFormattingProviderTypeNames = {
    {ServerCapabilities_documentRangeFormattingProviderType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {ServerCapabilities_documentRangeFormattingProviderType::DOCUMENT_RANGE_FORMATTING_OPTIONS, "DOCUMENT_RANGE_FORMATTING_OPTIONS"},
  };

  std::map<ServerCapabilities_renameProviderType, std::string> ServerCapabilities_renameProviderTypeNames = {
    {ServerCapabilities_renameProviderType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {ServerCapabilities_renameProviderType::RENAME_OPTIONS, "RENAME_OPTIONS"},
  };

  std::map<ServerCapabilities_foldingRangeProviderType, std::string> ServerCapabilities_foldingRangeProviderTypeNames = {
    {ServerCapabilities_foldingRangeProviderType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {ServerCapabilities_foldingRangeProviderType::FOLDING_RANGE_OPTIONS, "FOLDING_RANGE_OPTIONS"},
    {ServerCapabilities_foldingRangeProviderType::FOLDING_RANGE_REGISTRATION_OPTIONS, "FOLDING_RANGE_REGISTRATION_OPTIONS"},
  };

  std::map<ServerCapabilities_selectionRangeProviderType, std::string> ServerCapabilities_selectionRangeProviderTypeNames = {
    {ServerCapabilities_selectionRangeProviderType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {ServerCapabilities_selectionRangeProviderType::SELECTION_RANGE_OPTIONS, "SELECTION_RANGE_OPTIONS"},
    {ServerCapabilities_selectionRangeProviderType::SELECTION_RANGE_REGISTRATION_OPTIONS, "SELECTION_RANGE_REGISTRATION_OPTIONS"},
  };

  std::map<ServerCapabilities_callHierarchyProviderType, std::string> ServerCapabilities_callHierarchyProviderTypeNames = {
    {ServerCapabilities_callHierarchyProviderType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {ServerCapabilities_callHierarchyProviderType::CALL_HIERARCHY_OPTIONS, "CALL_HIERARCHY_OPTIONS"},
    {ServerCapabilities_callHierarchyProviderType::CALL_HIERARCHY_REGISTRATION_OPTIONS, "CALL_HIERARCHY_REGISTRATION_OPTIONS"},
  };

  std::map<ServerCapabilities_linkedEditingRangeProviderType, std::string> ServerCapabilities_linkedEditingRangeProviderTypeNames = {
    {ServerCapabilities_linkedEditingRangeProviderType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {ServerCapabilities_linkedEditingRangeProviderType::LINKED_EDITING_RANGE_OPTIONS, "LINKED_EDITING_RANGE_OPTIONS"},
    {ServerCapabilities_linkedEditingRangeProviderType::LINKED_EDITING_RANGE_REGISTRATION_OPTIONS, "LINKED_EDITING_RANGE_REGISTRATION_OPTIONS"},
  };

  std::map<ServerCapabilities_semanticTokensProviderType, std::string> ServerCapabilities_semanticTokensProviderTypeNames = {
    {ServerCapabilities_semanticTokensProviderType::SEMANTIC_TOKENS_OPTIONS, "SEMANTIC_TOKENS_OPTIONS"},
    {ServerCapabilities_semanticTokensProviderType::SEMANTIC_TOKENS_REGISTRATION_OPTIONS, "SEMANTIC_TOKENS_REGISTRATION_OPTIONS"},
  };

  std::map<ServerCapabilities_monikerProviderType, std::string> ServerCapabilities_monikerProviderTypeNames = {
    {ServerCapabilities_monikerProviderType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {ServerCapabilities_monikerProviderType::MONIKER_OPTIONS, "MONIKER_OPTIONS"},
    {ServerCapabilities_monikerProviderType::MONIKER_REGISTRATION_OPTIONS, "MONIKER_REGISTRATION_OPTIONS"},
  };

  std::map<ServerCapabilities_typeHierarchyProviderType, std::string> ServerCapabilities_typeHierarchyProviderTypeNames = {
    {ServerCapabilities_typeHierarchyProviderType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {ServerCapabilities_typeHierarchyProviderType::TYPE_HIERARCHY_OPTIONS, "TYPE_HIERARCHY_OPTIONS"},
    {ServerCapabilities_typeHierarchyProviderType::TYPE_HIERARCHY_REGISTRATION_OPTIONS, "TYPE_HIERARCHY_REGISTRATION_OPTIONS"},
  };

  std::map<ServerCapabilities_inlineValueProviderType, std::string> ServerCapabilities_inlineValueProviderTypeNames = {
    {ServerCapabilities_inlineValueProviderType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {ServerCapabilities_inlineValueProviderType::INLINE_VALUE_OPTIONS, "INLINE_VALUE_OPTIONS"},
    {ServerCapabilities_inlineValueProviderType::INLINE_VALUE_REGISTRATION_OPTIONS, "INLINE_VALUE_REGISTRATION_OPTIONS"},
  };

  std::map<ServerCapabilities_inlayHintProviderType, std::string> ServerCapabilities_inlayHintProviderTypeNames = {
    {ServerCapabilities_inlayHintProviderType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {ServerCapabilities_inlayHintProviderType::INLAY_HINT_OPTIONS, "INLAY_HINT_OPTIONS"},
    {ServerCapabilities_inlayHintProviderType::INLAY_HINT_REGISTRATION_OPTIONS, "INLAY_HINT_REGISTRATION_OPTIONS"},
  };

  std::map<ServerCapabilities_diagnosticProviderType, std::string> ServerCapabilities_diagnosticProviderTypeNames = {
    {ServerCapabilities_diagnosticProviderType::DIAGNOSTIC_OPTIONS, "DIAGNOSTIC_OPTIONS"},
    {ServerCapabilities_diagnosticProviderType::DIAGNOSTIC_REGISTRATION_OPTIONS, "DIAGNOSTIC_REGISTRATION_OPTIONS"},
  };

  std::map<ServerCapabilities_inlineCompletionProviderType, std::string> ServerCapabilities_inlineCompletionProviderTypeNames = {
    {ServerCapabilities_inlineCompletionProviderType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {ServerCapabilities_inlineCompletionProviderType::INLINE_COMPLETION_OPTIONS, "INLINE_COMPLETION_OPTIONS"},
  };

  std::map<Diagnostic_codeType, std::string> Diagnostic_codeTypeNames = {
    {Diagnostic_codeType::INTEGER_TYPE, "INTEGER_TYPE"},
    {Diagnostic_codeType::STRING_TYPE, "STRING_TYPE"},
  };

  std::map<SignatureInformation_documentationType, std::string> SignatureInformation_documentationTypeNames = {
    {SignatureInformation_documentationType::STRING_TYPE, "STRING_TYPE"},
    {SignatureInformation_documentationType::MARKUP_CONTENT, "MARKUP_CONTENT"},
  };

  std::map<OptionalVersionedTextDocumentIdentifier_versionType, std::string> OptionalVersionedTextDocumentIdentifier_versionTypeNames = {
    {OptionalVersionedTextDocumentIdentifier_versionType::INTEGER_TYPE, "INTEGER_TYPE"},
    {OptionalVersionedTextDocumentIdentifier_versionType::NULL_TYPE, "NULL_TYPE"},
  };

  std::map<WorkspaceFullDocumentDiagnosticReport_versionType, std::string> WorkspaceFullDocumentDiagnosticReport_versionTypeNames = {
    {WorkspaceFullDocumentDiagnosticReport_versionType::INTEGER_TYPE, "INTEGER_TYPE"},
    {WorkspaceFullDocumentDiagnosticReport_versionType::NULL_TYPE, "NULL_TYPE"},
  };

  std::map<WorkspaceUnchangedDocumentDiagnosticReport_versionType, std::string> WorkspaceUnchangedDocumentDiagnosticReport_versionTypeNames = {
    {WorkspaceUnchangedDocumentDiagnosticReport_versionType::INTEGER_TYPE, "INTEGER_TYPE"},
    {WorkspaceUnchangedDocumentDiagnosticReport_versionType::NULL_TYPE, "NULL_TYPE"},
  };

  std::map<TextDocumentSyncOptions_saveType, std::string> TextDocumentSyncOptions_saveTypeNames = {
    {TextDocumentSyncOptions_saveType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {TextDocumentSyncOptions_saveType::SAVE_OPTIONS, "SAVE_OPTIONS"},
  };

  std::map<NotebookDocumentSyncOptions_notebookSelector_1_notebookType, std::string> NotebookDocumentSyncOptions_notebookSelector_1_notebookTypeNames = {
    {NotebookDocumentSyncOptions_notebookSelector_1_notebookType::STRING_TYPE, "STRING_TYPE"},
    {NotebookDocumentSyncOptions_notebookSelector_1_notebookType::NOTEBOOK_DOCUMENT_FILTER, "NOTEBOOK_DOCUMENT_FILTER"},
  };

  std::map<NotebookDocumentSyncOptions_notebookSelector_0_notebookType, std::string> NotebookDocumentSyncOptions_notebookSelector_0_notebookTypeNames = {
    {NotebookDocumentSyncOptions_notebookSelector_0_notebookType::STRING_TYPE, "STRING_TYPE"},
    {NotebookDocumentSyncOptions_notebookSelector_0_notebookType::NOTEBOOK_DOCUMENT_FILTER, "NOTEBOOK_DOCUMENT_FILTER"},
  };

  std::map<NotebookDocumentSyncOptions_notebookSelectorType, std::string> NotebookDocumentSyncOptions_notebookSelectorTypeNames = {
    {NotebookDocumentSyncOptions_notebookSelectorType::NOTEBOOK_DOCUMENT_SYNC_OPTIONS_NOTEBOOK_SELECTOR_0, "NOTEBOOK_DOCUMENT_SYNC_OPTIONS_NOTEBOOK_SELECTOR_0"},
    {NotebookDocumentSyncOptions_notebookSelectorType::NOTEBOOK_DOCUMENT_SYNC_OPTIONS_NOTEBOOK_SELECTOR_1, "NOTEBOOK_DOCUMENT_SYNC_OPTIONS_NOTEBOOK_SELECTOR_1"},
  };

  std::map<WorkspaceFoldersServerCapabilities_changeNotificationsType, std::string> WorkspaceFoldersServerCapabilities_changeNotificationsTypeNames = {
    {WorkspaceFoldersServerCapabilities_changeNotificationsType::STRING_TYPE, "STRING_TYPE"},
    {WorkspaceFoldersServerCapabilities_changeNotificationsType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
  };

  std::map<ParameterInformation_labelType, std::string> ParameterInformation_labelTypeNames = {
    {ParameterInformation_labelType::STRING_TYPE, "STRING_TYPE"},
    {ParameterInformation_labelType::PAIR_OF_UINTEGER_TYPE_AND_UINTEGER_TYPE, "PAIR_OF_UINTEGER_TYPE_AND_UINTEGER_TYPE"},
  };

  std::map<ParameterInformation_documentationType, std::string> ParameterInformation_documentationTypeNames = {
    {ParameterInformation_documentationType::STRING_TYPE, "STRING_TYPE"},
    {ParameterInformation_documentationType::MARKUP_CONTENT, "MARKUP_CONTENT"},
  };

  std::map<NotebookCellTextDocumentFilter_notebookType, std::string> NotebookCellTextDocumentFilter_notebookTypeNames = {
    {NotebookCellTextDocumentFilter_notebookType::STRING_TYPE, "STRING_TYPE"},
    {NotebookCellTextDocumentFilter_notebookType::NOTEBOOK_DOCUMENT_FILTER, "NOTEBOOK_DOCUMENT_FILTER"},
  };

  std::map<RelativePattern_baseUriType, std::string> RelativePattern_baseUriTypeNames = {
    {RelativePattern_baseUriType::WORKSPACE_FOLDER, "WORKSPACE_FOLDER"},
    {RelativePattern_baseUriType::URI, "URI"},
  };

  std::map<SemanticTokensClientCapabilities_requests_fullType, std::string> SemanticTokensClientCapabilities_requests_fullTypeNames = {
    {SemanticTokensClientCapabilities_requests_fullType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {SemanticTokensClientCapabilities_requests_fullType::SEMANTIC_TOKENS_CLIENT_CAPABILITIES_REQUESTS_FULL_1, "SEMANTIC_TOKENS_CLIENT_CAPABILITIES_REQUESTS_FULL_1"},
  };

  std::map<SemanticTokensClientCapabilities_requests_rangeType, std::string> SemanticTokensClientCapabilities_requests_rangeTypeNames = {
    {SemanticTokensClientCapabilities_requests_rangeType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {SemanticTokensClientCapabilities_requests_rangeType::SEMANTIC_TOKENS_CLIENT_CAPABILITIES_REQUESTS_RANGE_1, "SEMANTIC_TOKENS_CLIENT_CAPABILITIES_REQUESTS_RANGE_1"},
  };

  std::map<RequestIdType, std::string> RequestIdTypeNames = {
    {RequestIdType::INTEGER_TYPE, "INTEGER_TYPE"},
    {RequestIdType::STRING_TYPE, "STRING_TYPE"},
  };

  std::map<MessageParamsType, std::string> MessageParamsTypeNames = {
    {MessageParamsType::ARRAY_TYPE, "ARRAY_TYPE"},
    {MessageParamsType::OBJECT_TYPE, "OBJECT_TYPE"},
  };

  std::map<ResponseIdType, std::string> ResponseIdTypeNames = {
    {ResponseIdType::INTEGER_TYPE, "INTEGER_TYPE"},
    {ResponseIdType::STRING_TYPE, "STRING_TYPE"},
    {ResponseIdType::NULL_TYPE, "NULL_TYPE"},
  };

  std::map<DefinitionType, std::string> DefinitionTypeNames = {
    {DefinitionType::LOCATION, "LOCATION"},
    {DefinitionType::LOCATION_ARRAY, "LOCATION_ARRAY"},
  };

  std::map<LSPAnyType, std::string> LSPAnyTypeNames = {
    {LSPAnyType::OBJECT_TYPE, "OBJECT_TYPE"},
    {LSPAnyType::ARRAY_TYPE, "ARRAY_TYPE"},
    {LSPAnyType::STRING_TYPE, "STRING_TYPE"},
    {LSPAnyType::INTEGER_TYPE, "INTEGER_TYPE"},
    {LSPAnyType::UINTEGER_TYPE, "UINTEGER_TYPE"},
    {LSPAnyType::DECIMAL_TYPE, "DECIMAL_TYPE"},
    {LSPAnyType::BOOLEAN_TYPE, "BOOLEAN_TYPE"},
    {LSPAnyType::NULL_TYPE, "NULL_TYPE"},
  };

  std::map<DeclarationType, std::string> DeclarationTypeNames = {
    {DeclarationType::LOCATION, "LOCATION"},
    {DeclarationType::LOCATION_ARRAY, "LOCATION_ARRAY"},
  };

  std::map<InlineValueType, std::string> InlineValueTypeNames = {
    {InlineValueType::INLINE_VALUE_TEXT, "INLINE_VALUE_TEXT"},
    {InlineValueType::INLINE_VALUE_VARIABLE_LOOKUP, "INLINE_VALUE_VARIABLE_LOOKUP"},
    {InlineValueType::INLINE_VALUE_EVALUATABLE_EXPRESSION, "INLINE_VALUE_EVALUATABLE_EXPRESSION"},
  };

  std::map<DocumentDiagnosticReportType, std::string> DocumentDiagnosticReportTypeNames = {
    {DocumentDiagnosticReportType::RELATED_FULL_DOCUMENT_DIAGNOSTIC_REPORT, "RELATED_FULL_DOCUMENT_DIAGNOSTIC_REPORT"},
    {DocumentDiagnosticReportType::RELATED_UNCHANGED_DOCUMENT_DIAGNOSTIC_REPORT, "RELATED_UNCHANGED_DOCUMENT_DIAGNOSTIC_REPORT"},
  };

  std::map<PrepareRenameResultType, std::string> PrepareRenameResultTypeNames = {
    {PrepareRenameResultType::RANGE, "RANGE"},
    {PrepareRenameResultType::PREPARE_RENAME_RESULT_1, "PREPARE_RENAME_RESULT_1"},
    {PrepareRenameResultType::PREPARE_RENAME_RESULT_2, "PREPARE_RENAME_RESULT_2"},
  };

  std::map<ProgressTokenType, std::string> ProgressTokenTypeNames = {
    {ProgressTokenType::INTEGER_TYPE, "INTEGER_TYPE"},
    {ProgressTokenType::STRING_TYPE, "STRING_TYPE"},
  };

  std::map<WorkspaceDocumentDiagnosticReportType, std::string> WorkspaceDocumentDiagnosticReportTypeNames = {
    {WorkspaceDocumentDiagnosticReportType::WORKSPACE_FULL_DOCUMENT_DIAGNOSTIC_REPORT, "WORKSPACE_FULL_DOCUMENT_DIAGNOSTIC_REPORT"},
    {WorkspaceDocumentDiagnosticReportType::WORKSPACE_UNCHANGED_DOCUMENT_DIAGNOSTIC_REPORT, "WORKSPACE_UNCHANGED_DOCUMENT_DIAGNOSTIC_REPORT"},
  };

  std::map<TextDocumentContentChangeEventType, std::string> TextDocumentContentChangeEventTypeNames = {
    {TextDocumentContentChangeEventType::TEXT_DOCUMENT_CONTENT_CHANGE_EVENT_0, "TEXT_DOCUMENT_CONTENT_CHANGE_EVENT_0"},
    {TextDocumentContentChangeEventType::TEXT_DOCUMENT_CONTENT_CHANGE_EVENT_1, "TEXT_DOCUMENT_CONTENT_CHANGE_EVENT_1"},
  };

  std::map<MarkedStringType, std::string> MarkedStringTypeNames = {
    {MarkedStringType::STRING_TYPE, "STRING_TYPE"},
    {MarkedStringType::MARKED_STRING_1, "MARKED_STRING_1"},
  };

  std::map<DocumentFilterType, std::string> DocumentFilterTypeNames = {
    {DocumentFilterType::TEXT_DOCUMENT_FILTER, "TEXT_DOCUMENT_FILTER"},
    {DocumentFilterType::NOTEBOOK_CELL_TEXT_DOCUMENT_FILTER, "NOTEBOOK_CELL_TEXT_DOCUMENT_FILTER"},
  };

  std::map<GlobPatternType, std::string> GlobPatternTypeNames = {
    {GlobPatternType::PATTERN, "PATTERN"},
    {GlobPatternType::RELATIVE_PATTERN, "RELATIVE_PATTERN"},
  };

  std::map<TextDocumentFilterType, std::string> TextDocumentFilterTypeNames = {
    {TextDocumentFilterType::TEXT_DOCUMENT_FILTER_0, "TEXT_DOCUMENT_FILTER_0"},
    {TextDocumentFilterType::TEXT_DOCUMENT_FILTER_1, "TEXT_DOCUMENT_FILTER_1"},
    {TextDocumentFilterType::TEXT_DOCUMENT_FILTER_2, "TEXT_DOCUMENT_FILTER_2"},
  };

  std::map<NotebookDocumentFilterType, std::string> NotebookDocumentFilterTypeNames = {
    {NotebookDocumentFilterType::NOTEBOOK_DOCUMENT_FILTER_0, "NOTEBOOK_DOCUMENT_FILTER_0"},
    {NotebookDocumentFilterType::NOTEBOOK_DOCUMENT_FILTER_1, "NOTEBOOK_DOCUMENT_FILTER_1"},
    {NotebookDocumentFilterType::NOTEBOOK_DOCUMENT_FILTER_2, "NOTEBOOK_DOCUMENT_FILTER_2"},
  };

  std::map<
    IncomingRequest,
    std::string
  > IncomingRequestNames = {
    {IncomingRequest::TEXT_DOCUMENT_IMPLEMENTATION, "TEXT_DOCUMENT_IMPLEMENTATION"},
    {IncomingRequest::TEXT_DOCUMENT_TYPE_DEFINITION, "TEXT_DOCUMENT_TYPE_DEFINITION"},
    {IncomingRequest::TEXT_DOCUMENT_DOCUMENT_COLOR, "TEXT_DOCUMENT_DOCUMENT_COLOR"},
    {IncomingRequest::TEXT_DOCUMENT_COLOR_PRESENTATION, "TEXT_DOCUMENT_COLOR_PRESENTATION"},
    {IncomingRequest::TEXT_DOCUMENT_FOLDING_RANGE, "TEXT_DOCUMENT_FOLDING_RANGE"},
    {IncomingRequest::TEXT_DOCUMENT_DECLARATION, "TEXT_DOCUMENT_DECLARATION"},
    {IncomingRequest::TEXT_DOCUMENT_SELECTION_RANGE, "TEXT_DOCUMENT_SELECTION_RANGE"},
    {IncomingRequest::TEXT_DOCUMENT_PREPARE_CALL_HIERARCHY, "TEXT_DOCUMENT_PREPARE_CALL_HIERARCHY"},
    {IncomingRequest::CALL_HIERARCHY_INCOMING_CALLS, "CALL_HIERARCHY_INCOMING_CALLS"},
    {IncomingRequest::CALL_HIERARCHY_OUTGOING_CALLS, "CALL_HIERARCHY_OUTGOING_CALLS"},
    {IncomingRequest::TEXT_DOCUMENT_SEMANTIC_TOKENS_FULL, "TEXT_DOCUMENT_SEMANTIC_TOKENS_FULL"},
    {IncomingRequest::TEXT_DOCUMENT_SEMANTIC_TOKENS_FULL_DELTA, "TEXT_DOCUMENT_SEMANTIC_TOKENS_FULL_DELTA"},
    {IncomingRequest::TEXT_DOCUMENT_SEMANTIC_TOKENS_RANGE, "TEXT_DOCUMENT_SEMANTIC_TOKENS_RANGE"},
    {IncomingRequest::TEXT_DOCUMENT_LINKED_EDITING_RANGE, "TEXT_DOCUMENT_LINKED_EDITING_RANGE"},
    {IncomingRequest::WORKSPACE_WILL_CREATE_FILES, "WORKSPACE_WILL_CREATE_FILES"},
    {IncomingRequest::WORKSPACE_WILL_RENAME_FILES, "WORKSPACE_WILL_RENAME_FILES"},
    {IncomingRequest::WORKSPACE_WILL_DELETE_FILES, "WORKSPACE_WILL_DELETE_FILES"},
    {IncomingRequest::TEXT_DOCUMENT_MONIKER, "TEXT_DOCUMENT_MONIKER"},
    {IncomingRequest::TEXT_DOCUMENT_PREPARE_TYPE_HIERARCHY, "TEXT_DOCUMENT_PREPARE_TYPE_HIERARCHY"},
    {IncomingRequest::TYPE_HIERARCHY_SUPERTYPES, "TYPE_HIERARCHY_SUPERTYPES"},
    {IncomingRequest::TYPE_HIERARCHY_SUBTYPES, "TYPE_HIERARCHY_SUBTYPES"},
    {IncomingRequest::TEXT_DOCUMENT_INLINE_VALUE, "TEXT_DOCUMENT_INLINE_VALUE"},
    {IncomingRequest::TEXT_DOCUMENT_INLAY_HINT, "TEXT_DOCUMENT_INLAY_HINT"},
    {IncomingRequest::INLAY_HINT_RESOLVE, "INLAY_HINT_RESOLVE"},
    {IncomingRequest::TEXT_DOCUMENT_DIAGNOSTIC, "TEXT_DOCUMENT_DIAGNOSTIC"},
    {IncomingRequest::WORKSPACE_DIAGNOSTIC, "WORKSPACE_DIAGNOSTIC"},
    {IncomingRequest::TEXT_DOCUMENT_INLINE_COMPLETION, "TEXT_DOCUMENT_INLINE_COMPLETION"},
    {IncomingRequest::INITIALIZE, "INITIALIZE"},
    {IncomingRequest::SHUTDOWN, "SHUTDOWN"},
    {IncomingRequest::TEXT_DOCUMENT_WILL_SAVE_WAIT_UNTIL, "TEXT_DOCUMENT_WILL_SAVE_WAIT_UNTIL"},
    {IncomingRequest::TEXT_DOCUMENT_COMPLETION, "TEXT_DOCUMENT_COMPLETION"},
    {IncomingRequest::COMPLETION_ITEM_RESOLVE, "COMPLETION_ITEM_RESOLVE"},
    {IncomingRequest::TEXT_DOCUMENT_HOVER, "TEXT_DOCUMENT_HOVER"},
    {IncomingRequest::TEXT_DOCUMENT_SIGNATURE_HELP, "TEXT_DOCUMENT_SIGNATURE_HELP"},
    {IncomingRequest::TEXT_DOCUMENT_DEFINITION, "TEXT_DOCUMENT_DEFINITION"},
    {IncomingRequest::TEXT_DOCUMENT_REFERENCES, "TEXT_DOCUMENT_REFERENCES"},
    {IncomingRequest::TEXT_DOCUMENT_DOCUMENT_HIGHLIGHT, "TEXT_DOCUMENT_DOCUMENT_HIGHLIGHT"},
    {IncomingRequest::TEXT_DOCUMENT_DOCUMENT_SYMBOL, "TEXT_DOCUMENT_DOCUMENT_SYMBOL"},
    {IncomingRequest::TEXT_DOCUMENT_CODE_ACTION, "TEXT_DOCUMENT_CODE_ACTION"},
    {IncomingRequest::CODE_ACTION_RESOLVE, "CODE_ACTION_RESOLVE"},
    {IncomingRequest::WORKSPACE_SYMBOL, "WORKSPACE_SYMBOL"},
    {IncomingRequest::WORKSPACE_SYMBOL_RESOLVE, "WORKSPACE_SYMBOL_RESOLVE"},
    {IncomingRequest::TEXT_DOCUMENT_CODE_LENS, "TEXT_DOCUMENT_CODE_LENS"},
    {IncomingRequest::CODE_LENS_RESOLVE, "CODE_LENS_RESOLVE"},
    {IncomingRequest::TEXT_DOCUMENT_DOCUMENT_LINK, "TEXT_DOCUMENT_DOCUMENT_LINK"},
    {IncomingRequest::DOCUMENT_LINK_RESOLVE, "DOCUMENT_LINK_RESOLVE"},
    {IncomingRequest::TEXT_DOCUMENT_FORMATTING, "TEXT_DOCUMENT_FORMATTING"},
    {IncomingRequest::TEXT_DOCUMENT_RANGE_FORMATTING, "TEXT_DOCUMENT_RANGE_FORMATTING"},
    {IncomingRequest::TEXT_DOCUMENT_RANGES_FORMATTING, "TEXT_DOCUMENT_RANGES_FORMATTING"},
    {IncomingRequest::TEXT_DOCUMENT_ON_TYPE_FORMATTING, "TEXT_DOCUMENT_ON_TYPE_FORMATTING"},
    {IncomingRequest::TEXT_DOCUMENT_RENAME, "TEXT_DOCUMENT_RENAME"},
    {IncomingRequest::TEXT_DOCUMENT_PREPARE_RENAME, "TEXT_DOCUMENT_PREPARE_RENAME"},
    {IncomingRequest::WORKSPACE_EXECUTE_COMMAND, "WORKSPACE_EXECUTE_COMMAND"},
  };

  std::map<
    IncomingRequest,
    string_t
  > IncomingRequestValues = {
    {IncomingRequest::TEXT_DOCUMENT_IMPLEMENTATION, "textDocument/implementation"},
    {IncomingRequest::TEXT_DOCUMENT_TYPE_DEFINITION, "textDocument/typeDefinition"},
    {IncomingRequest::TEXT_DOCUMENT_DOCUMENT_COLOR, "textDocument/documentColor"},
    {IncomingRequest::TEXT_DOCUMENT_COLOR_PRESENTATION, "textDocument/colorPresentation"},
    {IncomingRequest::TEXT_DOCUMENT_FOLDING_RANGE, "textDocument/foldingRange"},
    {IncomingRequest::TEXT_DOCUMENT_DECLARATION, "textDocument/declaration"},
    {IncomingRequest::TEXT_DOCUMENT_SELECTION_RANGE, "textDocument/selectionRange"},
    {IncomingRequest::TEXT_DOCUMENT_PREPARE_CALL_HIERARCHY, "textDocument/prepareCallHierarchy"},
    {IncomingRequest::CALL_HIERARCHY_INCOMING_CALLS, "callHierarchy/incomingCalls"},
    {IncomingRequest::CALL_HIERARCHY_OUTGOING_CALLS, "callHierarchy/outgoingCalls"},
    {IncomingRequest::TEXT_DOCUMENT_SEMANTIC_TOKENS_FULL, "textDocument/semanticTokens/full"},
    {IncomingRequest::TEXT_DOCUMENT_SEMANTIC_TOKENS_FULL_DELTA, "textDocument/semanticTokens/full/delta"},
    {IncomingRequest::TEXT_DOCUMENT_SEMANTIC_TOKENS_RANGE, "textDocument/semanticTokens/range"},
    {IncomingRequest::TEXT_DOCUMENT_LINKED_EDITING_RANGE, "textDocument/linkedEditingRange"},
    {IncomingRequest::WORKSPACE_WILL_CREATE_FILES, "workspace/willCreateFiles"},
    {IncomingRequest::WORKSPACE_WILL_RENAME_FILES, "workspace/willRenameFiles"},
    {IncomingRequest::WORKSPACE_WILL_DELETE_FILES, "workspace/willDeleteFiles"},
    {IncomingRequest::TEXT_DOCUMENT_MONIKER, "textDocument/moniker"},
    {IncomingRequest::TEXT_DOCUMENT_PREPARE_TYPE_HIERARCHY, "textDocument/prepareTypeHierarchy"},
    {IncomingRequest::TYPE_HIERARCHY_SUPERTYPES, "typeHierarchy/supertypes"},
    {IncomingRequest::TYPE_HIERARCHY_SUBTYPES, "typeHierarchy/subtypes"},
    {IncomingRequest::TEXT_DOCUMENT_INLINE_VALUE, "textDocument/inlineValue"},
    {IncomingRequest::TEXT_DOCUMENT_INLAY_HINT, "textDocument/inlayHint"},
    {IncomingRequest::INLAY_HINT_RESOLVE, "inlayHint/resolve"},
    {IncomingRequest::TEXT_DOCUMENT_DIAGNOSTIC, "textDocument/diagnostic"},
    {IncomingRequest::WORKSPACE_DIAGNOSTIC, "workspace/diagnostic"},
    {IncomingRequest::TEXT_DOCUMENT_INLINE_COMPLETION, "textDocument/inlineCompletion"},
    {IncomingRequest::INITIALIZE, "initialize"},
    {IncomingRequest::SHUTDOWN, "shutdown"},
    {IncomingRequest::TEXT_DOCUMENT_WILL_SAVE_WAIT_UNTIL, "textDocument/willSaveWaitUntil"},
    {IncomingRequest::TEXT_DOCUMENT_COMPLETION, "textDocument/completion"},
    {IncomingRequest::COMPLETION_ITEM_RESOLVE, "completionItem/resolve"},
    {IncomingRequest::TEXT_DOCUMENT_HOVER, "textDocument/hover"},
    {IncomingRequest::TEXT_DOCUMENT_SIGNATURE_HELP, "textDocument/signatureHelp"},
    {IncomingRequest::TEXT_DOCUMENT_DEFINITION, "textDocument/definition"},
    {IncomingRequest::TEXT_DOCUMENT_REFERENCES, "textDocument/references"},
    {IncomingRequest::TEXT_DOCUMENT_DOCUMENT_HIGHLIGHT, "textDocument/documentHighlight"},
    {IncomingRequest::TEXT_DOCUMENT_DOCUMENT_SYMBOL, "textDocument/documentSymbol"},
    {IncomingRequest::TEXT_DOCUMENT_CODE_ACTION, "textDocument/codeAction"},
    {IncomingRequest::CODE_ACTION_RESOLVE, "codeAction/resolve"},
    {IncomingRequest::WORKSPACE_SYMBOL, "workspace/symbol"},
    {IncomingRequest::WORKSPACE_SYMBOL_RESOLVE, "workspaceSymbol/resolve"},
    {IncomingRequest::TEXT_DOCUMENT_CODE_LENS, "textDocument/codeLens"},
    {IncomingRequest::CODE_LENS_RESOLVE, "codeLens/resolve"},
    {IncomingRequest::TEXT_DOCUMENT_DOCUMENT_LINK, "textDocument/documentLink"},
    {IncomingRequest::DOCUMENT_LINK_RESOLVE, "documentLink/resolve"},
    {IncomingRequest::TEXT_DOCUMENT_FORMATTING, "textDocument/formatting"},
    {IncomingRequest::TEXT_DOCUMENT_RANGE_FORMATTING, "textDocument/rangeFormatting"},
    {IncomingRequest::TEXT_DOCUMENT_RANGES_FORMATTING, "textDocument/rangesFormatting"},
    {IncomingRequest::TEXT_DOCUMENT_ON_TYPE_FORMATTING, "textDocument/onTypeFormatting"},
    {IncomingRequest::TEXT_DOCUMENT_RENAME, "textDocument/rename"},
    {IncomingRequest::TEXT_DOCUMENT_PREPARE_RENAME, "textDocument/prepareRename"},
    {IncomingRequest::WORKSPACE_EXECUTE_COMMAND, "workspace/executeCommand"},
  };

  auto incomingRequestByName(
    const std::string &name
  ) -> IncomingRequest {
    for (const auto &[enum_name, enum_value]
        : IncomingRequestNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid IncomingRequest name: " + name
    );
  }

  auto incomingRequestByValue(
    const string_t &value
  ) -> IncomingRequest {
    for (const auto &[enum_name, enum_value]
        : IncomingRequestValues) {
      if (value == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid IncomingRequest value: " + value
    );
  }

  auto isIncomingRequest(const std::string &value) -> bool {
    for (const auto &[enum_key, enum_value] : IncomingRequestValues) {
      if (value == enum_value) {
        return true;
      }
    }
    return false;
  }

  std::map<
    OutgoingRequest,
    std::string
  > OutgoingRequestNames = {
    {OutgoingRequest::WORKSPACE_WORKSPACE_FOLDERS, "WORKSPACE_WORKSPACE_FOLDERS"},
    {OutgoingRequest::WORKSPACE_CONFIGURATION, "WORKSPACE_CONFIGURATION"},
    {OutgoingRequest::WORKSPACE_FOLDING_RANGE_REFRESH, "WORKSPACE_FOLDING_RANGE_REFRESH"},
    {OutgoingRequest::WINDOW_WORK_DONE_PROGRESS_CREATE, "WINDOW_WORK_DONE_PROGRESS_CREATE"},
    {OutgoingRequest::WORKSPACE_SEMANTIC_TOKENS_REFRESH, "WORKSPACE_SEMANTIC_TOKENS_REFRESH"},
    {OutgoingRequest::WINDOW_SHOW_DOCUMENT, "WINDOW_SHOW_DOCUMENT"},
    {OutgoingRequest::WORKSPACE_INLINE_VALUE_REFRESH, "WORKSPACE_INLINE_VALUE_REFRESH"},
    {OutgoingRequest::WORKSPACE_INLAY_HINT_REFRESH, "WORKSPACE_INLAY_HINT_REFRESH"},
    {OutgoingRequest::WORKSPACE_DIAGNOSTIC_REFRESH, "WORKSPACE_DIAGNOSTIC_REFRESH"},
    {OutgoingRequest::CLIENT_REGISTER_CAPABILITY, "CLIENT_REGISTER_CAPABILITY"},
    {OutgoingRequest::CLIENT_UNREGISTER_CAPABILITY, "CLIENT_UNREGISTER_CAPABILITY"},
    {OutgoingRequest::WINDOW_SHOW_MESSAGE_REQUEST, "WINDOW_SHOW_MESSAGE_REQUEST"},
    {OutgoingRequest::WORKSPACE_CODE_LENS_REFRESH, "WORKSPACE_CODE_LENS_REFRESH"},
    {OutgoingRequest::WORKSPACE_APPLY_EDIT, "WORKSPACE_APPLY_EDIT"},
  };

  std::map<
    OutgoingRequest,
    string_t
  > OutgoingRequestValues = {
    {OutgoingRequest::WORKSPACE_WORKSPACE_FOLDERS, "workspace/workspaceFolders"},
    {OutgoingRequest::WORKSPACE_CONFIGURATION, "workspace/configuration"},
    {OutgoingRequest::WORKSPACE_FOLDING_RANGE_REFRESH, "workspace/foldingRange/refresh"},
    {OutgoingRequest::WINDOW_WORK_DONE_PROGRESS_CREATE, "window/workDoneProgress/create"},
    {OutgoingRequest::WORKSPACE_SEMANTIC_TOKENS_REFRESH, "workspace/semanticTokens/refresh"},
    {OutgoingRequest::WINDOW_SHOW_DOCUMENT, "window/showDocument"},
    {OutgoingRequest::WORKSPACE_INLINE_VALUE_REFRESH, "workspace/inlineValue/refresh"},
    {OutgoingRequest::WORKSPACE_INLAY_HINT_REFRESH, "workspace/inlayHint/refresh"},
    {OutgoingRequest::WORKSPACE_DIAGNOSTIC_REFRESH, "workspace/diagnostic/refresh"},
    {OutgoingRequest::CLIENT_REGISTER_CAPABILITY, "client/registerCapability"},
    {OutgoingRequest::CLIENT_UNREGISTER_CAPABILITY, "client/unregisterCapability"},
    {OutgoingRequest::WINDOW_SHOW_MESSAGE_REQUEST, "window/showMessageRequest"},
    {OutgoingRequest::WORKSPACE_CODE_LENS_REFRESH, "workspace/codeLens/refresh"},
    {OutgoingRequest::WORKSPACE_APPLY_EDIT, "workspace/applyEdit"},
  };

  auto outgoingRequestByName(
    const std::string &name
  ) -> OutgoingRequest {
    for (const auto &[enum_name, enum_value]
        : OutgoingRequestNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid OutgoingRequest name: " + name
    );
  }

  auto outgoingRequestByValue(
    const string_t &value
  ) -> OutgoingRequest {
    for (const auto &[enum_name, enum_value]
        : OutgoingRequestValues) {
      if (value == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid OutgoingRequest value: " + value
    );
  }

  std::map<
    IncomingNotification,
    std::string
  > IncomingNotificationNames = {
    {IncomingNotification::WORKSPACE_DID_CHANGE_WORKSPACE_FOLDERS, "WORKSPACE_DID_CHANGE_WORKSPACE_FOLDERS"},
    {IncomingNotification::WINDOW_WORK_DONE_PROGRESS_CANCEL, "WINDOW_WORK_DONE_PROGRESS_CANCEL"},
    {IncomingNotification::WORKSPACE_DID_CREATE_FILES, "WORKSPACE_DID_CREATE_FILES"},
    {IncomingNotification::WORKSPACE_DID_RENAME_FILES, "WORKSPACE_DID_RENAME_FILES"},
    {IncomingNotification::WORKSPACE_DID_DELETE_FILES, "WORKSPACE_DID_DELETE_FILES"},
    {IncomingNotification::NOTEBOOK_DOCUMENT_DID_OPEN, "NOTEBOOK_DOCUMENT_DID_OPEN"},
    {IncomingNotification::NOTEBOOK_DOCUMENT_DID_CHANGE, "NOTEBOOK_DOCUMENT_DID_CHANGE"},
    {IncomingNotification::NOTEBOOK_DOCUMENT_DID_SAVE, "NOTEBOOK_DOCUMENT_DID_SAVE"},
    {IncomingNotification::NOTEBOOK_DOCUMENT_DID_CLOSE, "NOTEBOOK_DOCUMENT_DID_CLOSE"},
    {IncomingNotification::INITIALIZED, "INITIALIZED"},
    {IncomingNotification::EXIT, "EXIT"},
    {IncomingNotification::WORKSPACE_DID_CHANGE_CONFIGURATION, "WORKSPACE_DID_CHANGE_CONFIGURATION"},
    {IncomingNotification::TEXT_DOCUMENT_DID_OPEN, "TEXT_DOCUMENT_DID_OPEN"},
    {IncomingNotification::TEXT_DOCUMENT_DID_CHANGE, "TEXT_DOCUMENT_DID_CHANGE"},
    {IncomingNotification::TEXT_DOCUMENT_DID_CLOSE, "TEXT_DOCUMENT_DID_CLOSE"},
    {IncomingNotification::TEXT_DOCUMENT_DID_SAVE, "TEXT_DOCUMENT_DID_SAVE"},
    {IncomingNotification::TEXT_DOCUMENT_WILL_SAVE, "TEXT_DOCUMENT_WILL_SAVE"},
    {IncomingNotification::WORKSPACE_DID_CHANGE_WATCHED_FILES, "WORKSPACE_DID_CHANGE_WATCHED_FILES"},
    {IncomingNotification::SET_TRACE, "SET_TRACE"},
  };

  std::map<
    IncomingNotification,
    string_t
  > IncomingNotificationValues = {
    {IncomingNotification::WORKSPACE_DID_CHANGE_WORKSPACE_FOLDERS, "workspace/didChangeWorkspaceFolders"},
    {IncomingNotification::WINDOW_WORK_DONE_PROGRESS_CANCEL, "window/workDoneProgress/cancel"},
    {IncomingNotification::WORKSPACE_DID_CREATE_FILES, "workspace/didCreateFiles"},
    {IncomingNotification::WORKSPACE_DID_RENAME_FILES, "workspace/didRenameFiles"},
    {IncomingNotification::WORKSPACE_DID_DELETE_FILES, "workspace/didDeleteFiles"},
    {IncomingNotification::NOTEBOOK_DOCUMENT_DID_OPEN, "notebookDocument/didOpen"},
    {IncomingNotification::NOTEBOOK_DOCUMENT_DID_CHANGE, "notebookDocument/didChange"},
    {IncomingNotification::NOTEBOOK_DOCUMENT_DID_SAVE, "notebookDocument/didSave"},
    {IncomingNotification::NOTEBOOK_DOCUMENT_DID_CLOSE, "notebookDocument/didClose"},
    {IncomingNotification::INITIALIZED, "initialized"},
    {IncomingNotification::EXIT, "exit"},
    {IncomingNotification::WORKSPACE_DID_CHANGE_CONFIGURATION, "workspace/didChangeConfiguration"},
    {IncomingNotification::TEXT_DOCUMENT_DID_OPEN, "textDocument/didOpen"},
    {IncomingNotification::TEXT_DOCUMENT_DID_CHANGE, "textDocument/didChange"},
    {IncomingNotification::TEXT_DOCUMENT_DID_CLOSE, "textDocument/didClose"},
    {IncomingNotification::TEXT_DOCUMENT_DID_SAVE, "textDocument/didSave"},
    {IncomingNotification::TEXT_DOCUMENT_WILL_SAVE, "textDocument/willSave"},
    {IncomingNotification::WORKSPACE_DID_CHANGE_WATCHED_FILES, "workspace/didChangeWatchedFiles"},
    {IncomingNotification::SET_TRACE, "$/setTrace"},
  };

  auto incomingNotificationByName(
    const std::string &name
  ) -> IncomingNotification {
    for (const auto &[enum_name, enum_value]
        : IncomingNotificationNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid IncomingNotification name: " + name
    );
  }

  auto incomingNotificationByValue(
    const string_t &value
  ) -> IncomingNotification {
    for (const auto &[enum_name, enum_value]
        : IncomingNotificationValues) {
      if (value == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid IncomingNotification value: " + value
    );
  }

  auto isIncomingNotification(const std::string &value) -> bool {
    for (const auto &[enum_key, enum_value] : IncomingNotificationValues) {
      if (value == enum_value) {
        return true;
      }
    }
    return false;
  }

  std::map<
    OutgoingNotification,
    std::string
  > OutgoingNotificationNames = {
    {OutgoingNotification::WINDOW_SHOW_MESSAGE, "WINDOW_SHOW_MESSAGE"},
    {OutgoingNotification::WINDOW_LOG_MESSAGE, "WINDOW_LOG_MESSAGE"},
    {OutgoingNotification::TELEMETRY_EVENT, "TELEMETRY_EVENT"},
    {OutgoingNotification::TEXT_DOCUMENT_PUBLISH_DIAGNOSTICS, "TEXT_DOCUMENT_PUBLISH_DIAGNOSTICS"},
    {OutgoingNotification::LOG_TRACE, "LOG_TRACE"},
  };

  std::map<
    OutgoingNotification,
    string_t
  > OutgoingNotificationValues = {
    {OutgoingNotification::WINDOW_SHOW_MESSAGE, "window/showMessage"},
    {OutgoingNotification::WINDOW_LOG_MESSAGE, "window/logMessage"},
    {OutgoingNotification::TELEMETRY_EVENT, "telemetry/event"},
    {OutgoingNotification::TEXT_DOCUMENT_PUBLISH_DIAGNOSTICS, "textDocument/publishDiagnostics"},
    {OutgoingNotification::LOG_TRACE, "$/logTrace"},
  };

  auto outgoingNotificationByName(
    const std::string &name
  ) -> OutgoingNotification {
    for (const auto &[enum_name, enum_value]
        : OutgoingNotificationNames) {
      if (name == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid OutgoingNotification name: " + name
    );
  }

  auto outgoingNotificationByValue(
    const string_t &value
  ) -> OutgoingNotification {
    for (const auto &[enum_name, enum_value]
        : OutgoingNotificationValues) {
      if (value == enum_value) {
        return enum_name;
      }
    }
    throw std::invalid_argument(
      "Invalid OutgoingNotification value: " + value
    );
  }

} // namespace LCompilers::LanguageServerProtocol
