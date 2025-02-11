// -----------------------------------------------------------------------------
// NOTE: This file was generated from Microsoft's Language Server Protocol (LSP)
// specification. Please do not edit it by hand.
// -----------------------------------------------------------------------------

#pragma once

#include <cstddef>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

/**
 * Interface definitions from the LSP 3.17.0 specification.
 * See: https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification
 */
namespace LCompilers::LanguageServerProtocol {
  const std::string JSON_RPC_VERSION = "2.0";
  const std::string LSP_VERSION = "3.17.0";

  typedef int integer_t;
  typedef unsigned int uinteger_t;
  typedef double decimal_t;
  typedef bool boolean_t;
  typedef std::nullptr_t null_t;
  typedef std::string string_t;

  typedef string_t URI;
  typedef string_t DocumentUri;
  typedef string_t RegExp;

  struct LSPAny;  // Forward declaration

  /**
   * LSP object definition.
   * @since 3.17.0
   */
  typedef std::map<string_t, std::unique_ptr<LSPAny>> LSPObject;

  /**
   * LSP arrays.
   * @since 3.17.0
   */
  typedef std::vector<std::unique_ptr<LSPAny>> LSPArray;

  enum class LSPAnyType {
    OBJECT_TYPE,
    ARRAY_TYPE,
    STRING_TYPE,
    INTEGER_TYPE,
    UINTEGER_TYPE,
    DECIMAL_TYPE,
    BOOLEAN_TYPE,
    NULL_TYPE,
  };

  extern std::map<LSPAnyType, std::string> LSPAnyTypeNames;

  typedef std::variant<
    LSPObject,
    LSPArray,
    string_t,
    integer_t,
    uinteger_t,
    decimal_t,
    boolean_t,
    null_t
  > LSPAnyBase;

  struct LSPAny
    : public LSPAnyBase
  {
    using LSPAnyBase::variant;
  };

  /**
   * A set of predefined token types. This set is not fixed
   * an clients can specify additional token types via the
   * corresponding client capabilities.
   *
   * @since 3.16.0
   */
  enum class SemanticTokenTypes {
    NAMESPACE,
    /**
     * Represents a generic type. Acts as a fallback for types which can't be mapped to
     * a specific type like class or enum.
     */
    TYPE,
    CLASS,
    ENUM,
    INTERFACE,
    STRUCT,
    TYPE_PARAMETER,
    PARAMETER,
    VARIABLE,
    PROPERTY,
    ENUM_MEMBER,
    EVENT,
    FUNCTION,
    METHOD,
    MACRO,
    KEYWORD,
    MODIFIER,
    COMMENT,
    STRING_TYPE,
    NUMBER,
    REGEXP,
    OPERATOR,
    /**
     * @since 3.17.0
     */
    DECORATOR,
  };

  extern std::map<
    SemanticTokenTypes,
    std::string
  > SemanticTokenTypesNames;

  extern std::map<
    SemanticTokenTypes,
    string_t
  > SemanticTokenTypesValues;

  auto semanticTokenTypesByName(
    const std::string &name
  ) -> SemanticTokenTypes;

  auto semanticTokenTypesByValue(
    const string_t &value
  ) -> SemanticTokenTypes;

  /**
   * A set of predefined token modifiers. This set is not fixed
   * an clients can specify additional token types via the
   * corresponding client capabilities.
   *
   * @since 3.16.0
   */
  enum class SemanticTokenModifiers {
    DECLARATION,
    DEFINITION,
    READONLY,
    STATIC,
    DEPRECATED,
    ABSTRACT,
    ASYNC,
    MODIFICATION,
    DOCUMENTATION,
    DEFAULT_LIBRARY,
  };

  extern std::map<
    SemanticTokenModifiers,
    std::string
  > SemanticTokenModifiersNames;

  extern std::map<
    SemanticTokenModifiers,
    string_t
  > SemanticTokenModifiersValues;

  auto semanticTokenModifiersByName(
    const std::string &name
  ) -> SemanticTokenModifiers;

  auto semanticTokenModifiersByValue(
    const string_t &value
  ) -> SemanticTokenModifiers;

  /**
   * The document diagnostic report kinds.
   *
   * @since 3.17.0
   */
  enum class DocumentDiagnosticReportKind {
    /**
     * A diagnostic report with a full
     * set of problems.
     */
    FULL,
    /**
     * A report indicating that the last
     * returned report is still accurate.
     */
    UNCHANGED,
  };

  extern std::map<
    DocumentDiagnosticReportKind,
    std::string
  > DocumentDiagnosticReportKindNames;

  extern std::map<
    DocumentDiagnosticReportKind,
    string_t
  > DocumentDiagnosticReportKindValues;

  auto documentDiagnosticReportKindByName(
    const std::string &name
  ) -> DocumentDiagnosticReportKind;

  auto documentDiagnosticReportKindByValue(
    const string_t &value
  ) -> DocumentDiagnosticReportKind;

  /**
   * Predefined error codes.
   */
  enum class ErrorCodes {
    PARSE_ERROR = -32700,
    INVALID_REQUEST = -32600,
    METHOD_NOT_FOUND = -32601,
    INVALID_PARAMS = -32602,
    INTERNAL_ERROR = -32603,
    /**
     * Error code indicating that a server received a notification or
     * request before the server has received the `initialize` request.
     */
    SERVER_NOT_INITIALIZED = -32002,
    UNKNOWN_ERROR_CODE = -32001,
  };

  extern std::map<
    ErrorCodes,
    std::string
  > ErrorCodesNames;

  auto errorCodesByName(
    const std::string &name
  ) -> ErrorCodes;

  auto errorCodesByValue(
    integer_t value
  ) -> ErrorCodes;

  enum class LSPErrorCodes {
    /**
     * A request failed but it was syntactically correct, e.g the
     * method name was known and the parameters were valid. The error
     * message should contain human readable information about why
     * the request failed.
     *
     * @since 3.17.0
     */
    REQUEST_FAILED = -32803,
    /**
     * The server cancelled the request. This error code should
     * only be used for requests that explicitly support being
     * server cancellable.
     *
     * @since 3.17.0
     */
    SERVER_CANCELLED = -32802,
    /**
     * The server detected that the content of a document got
     * modified outside normal conditions. A server should
     * NOT send this error code if it detects a content change
     * in it unprocessed messages. The result even computed
     * on an older state might still be useful for the client.
     *
     * If a client decides that a result is not of any use anymore
     * the client should cancel the request.
     */
    CONTENT_MODIFIED = -32801,
    /**
     * The client has canceled a request and a server has detected
     * the cancel.
     */
    REQUEST_CANCELLED = -32800,
  };

  extern std::map<
    LSPErrorCodes,
    std::string
  > LSPErrorCodesNames;

  auto lspErrorCodesByName(
    const std::string &name
  ) -> LSPErrorCodes;

  auto lspErrorCodesByValue(
    integer_t value
  ) -> LSPErrorCodes;

  /**
   * A set of predefined range kinds.
   */
  enum class FoldingRangeKind {
    /**
     * Folding range for a comment
     */
    COMMENT,
    /**
     * Folding range for an import or include
     */
    IMPORTS,
    /**
     * Folding range for a region (e.g. `#region`)
     */
    REGION,
  };

  extern std::map<
    FoldingRangeKind,
    std::string
  > FoldingRangeKindNames;

  extern std::map<
    FoldingRangeKind,
    string_t
  > FoldingRangeKindValues;

  auto foldingRangeKindByName(
    const std::string &name
  ) -> FoldingRangeKind;

  auto foldingRangeKindByValue(
    const string_t &value
  ) -> FoldingRangeKind;

  /**
   * A symbol kind.
   */
  enum class SymbolKind {
    FILE = 1,
    MODULE = 2,
    NAMESPACE = 3,
    PACKAGE = 4,
    CLASS = 5,
    METHOD = 6,
    PROPERTY = 7,
    FIELD = 8,
    CONSTRUCTOR = 9,
    ENUM = 10,
    INTERFACE = 11,
    FUNCTION = 12,
    VARIABLE = 13,
    CONSTANT = 14,
    STRING_TYPE = 15,
    NUMBER = 16,
    BOOLEAN_TYPE = 17,
    ARRAY_TYPE = 18,
    OBJECT_TYPE = 19,
    KEY = 20,
    NULL_TYPE = 21,
    ENUM_MEMBER = 22,
    STRUCT = 23,
    EVENT = 24,
    OPERATOR = 25,
    TYPE_PARAMETER = 26,
  };

  extern std::map<
    SymbolKind,
    std::string
  > SymbolKindNames;

  auto symbolKindByName(
    const std::string &name
  ) -> SymbolKind;

  auto symbolKindByValue(
    uinteger_t value
  ) -> SymbolKind;

  /**
   * Symbol tags are extra annotations that tweak the rendering of a symbol.
   *
   * @since 3.16
   */
  enum class SymbolTag {
    /**
     * Render a symbol as obsolete, usually using a strike-out.
     */
    DEPRECATED = 1,
  };

  extern std::map<
    SymbolTag,
    std::string
  > SymbolTagNames;

  auto symbolTagByName(
    const std::string &name
  ) -> SymbolTag;

  auto symbolTagByValue(
    uinteger_t value
  ) -> SymbolTag;

  /**
   * Moniker uniqueness level to define scope of the moniker.
   *
   * @since 3.16.0
   */
  enum class UniquenessLevel {
    /**
     * The moniker is only unique inside a document
     */
    DOCUMENT,
    /**
     * The moniker is unique inside a project for which a dump got created
     */
    PROJECT,
    /**
     * The moniker is unique inside the group to which a project belongs
     */
    GROUP,
    /**
     * The moniker is unique inside the moniker scheme.
     */
    SCHEME,
    /**
     * The moniker is globally unique
     */
    GLOBAL,
  };

  extern std::map<
    UniquenessLevel,
    std::string
  > UniquenessLevelNames;

  extern std::map<
    UniquenessLevel,
    string_t
  > UniquenessLevelValues;

  auto uniquenessLevelByName(
    const std::string &name
  ) -> UniquenessLevel;

  auto uniquenessLevelByValue(
    const string_t &value
  ) -> UniquenessLevel;

  /**
   * The moniker kind.
   *
   * @since 3.16.0
   */
  enum class MonikerKind {
    /**
     * The moniker represent a symbol that is imported into a project
     */
    IMPORT,
    /**
     * The moniker represents a symbol that is exported from a project
     */
    EXPORT,
    /**
     * The moniker represents a symbol that is local to a project (e.g. a local
     * variable of a function, a class not visible outside the project, ...)
     */
    LOCAL,
  };

  extern std::map<
    MonikerKind,
    std::string
  > MonikerKindNames;

  extern std::map<
    MonikerKind,
    string_t
  > MonikerKindValues;

  auto monikerKindByName(
    const std::string &name
  ) -> MonikerKind;

  auto monikerKindByValue(
    const string_t &value
  ) -> MonikerKind;

  /**
   * Inlay hint kinds.
   *
   * @since 3.17.0
   */
  enum class InlayHintKind {
    /**
     * An inlay hint that for a type annotation.
     */
    TYPE = 1,
    /**
     * An inlay hint that is for a parameter.
     */
    PARAMETER = 2,
  };

  extern std::map<
    InlayHintKind,
    std::string
  > InlayHintKindNames;

  auto inlayHintKindByName(
    const std::string &name
  ) -> InlayHintKind;

  auto inlayHintKindByValue(
    uinteger_t value
  ) -> InlayHintKind;

  /**
   * The message type
   */
  enum class MessageType {
    /**
     * An error message.
     */
    ERROR = 1,
    /**
     * A warning message.
     */
    WARNING = 2,
    /**
     * An information message.
     */
    INFO = 3,
    /**
     * A log message.
     */
    LOG = 4,
    /**
     * A debug message.
     *
     * @since 3.18.0
     */
    DEBUG = 5,
  };

  extern std::map<
    MessageType,
    std::string
  > MessageTypeNames;

  auto messageTypeByName(
    const std::string &name
  ) -> MessageType;

  auto messageTypeByValue(
    uinteger_t value
  ) -> MessageType;

  /**
   * Defines how the host (editor) should sync
   * document changes to the language server.
   */
  enum class TextDocumentSyncKind {
    /**
     * Documents should not be synced at all.
     */
    NONE = 0,
    /**
     * Documents are synced by always sending the full content
     * of the document.
     */
    FULL = 1,
    /**
     * Documents are synced by sending the full content on open.
     * After that only incremental updates to the document are
     * send.
     */
    INCREMENTAL = 2,
  };

  extern std::map<
    TextDocumentSyncKind,
    std::string
  > TextDocumentSyncKindNames;

  auto textDocumentSyncKindByName(
    const std::string &name
  ) -> TextDocumentSyncKind;

  auto textDocumentSyncKindByValue(
    uinteger_t value
  ) -> TextDocumentSyncKind;

  /**
   * Represents reasons why a text document is saved.
   */
  enum class TextDocumentSaveReason {
    /**
     * Manually triggered, e.g. by the user pressing save, by starting debugging,
     * or by an API call.
     */
    MANUAL = 1,
    /**
     * Automatic after a delay.
     */
    AFTER_DELAY = 2,
    /**
     * When the editor lost focus.
     */
    FOCUS_OUT = 3,
  };

  extern std::map<
    TextDocumentSaveReason,
    std::string
  > TextDocumentSaveReasonNames;

  auto textDocumentSaveReasonByName(
    const std::string &name
  ) -> TextDocumentSaveReason;

  auto textDocumentSaveReasonByValue(
    uinteger_t value
  ) -> TextDocumentSaveReason;

  /**
   * The kind of a completion entry.
   */
  enum class CompletionItemKind {
    TEXT = 1,
    METHOD = 2,
    FUNCTION = 3,
    CONSTRUCTOR = 4,
    FIELD = 5,
    VARIABLE = 6,
    CLASS = 7,
    INTERFACE = 8,
    MODULE = 9,
    PROPERTY = 10,
    UNIT = 11,
    VALUE = 12,
    ENUM = 13,
    KEYWORD = 14,
    SNIPPET = 15,
    COLOR = 16,
    FILE = 17,
    REFERENCE = 18,
    FOLDER = 19,
    ENUM_MEMBER = 20,
    CONSTANT = 21,
    STRUCT = 22,
    EVENT = 23,
    OPERATOR = 24,
    TYPE_PARAMETER = 25,
  };

  extern std::map<
    CompletionItemKind,
    std::string
  > CompletionItemKindNames;

  auto completionItemKindByName(
    const std::string &name
  ) -> CompletionItemKind;

  auto completionItemKindByValue(
    uinteger_t value
  ) -> CompletionItemKind;

  /**
   * Completion item tags are extra annotations that tweak the rendering of a completion
   * item.
   *
   * @since 3.15.0
   */
  enum class CompletionItemTag {
    /**
     * Render a completion as obsolete, usually using a strike-out.
     */
    DEPRECATED = 1,
  };

  extern std::map<
    CompletionItemTag,
    std::string
  > CompletionItemTagNames;

  auto completionItemTagByName(
    const std::string &name
  ) -> CompletionItemTag;

  auto completionItemTagByValue(
    uinteger_t value
  ) -> CompletionItemTag;

  /**
   * Defines whether the insert text in a completion item should be interpreted as
   * plain text or a snippet.
   */
  enum class InsertTextFormat {
    /**
     * The primary text to be inserted is treated as a plain string.
     */
    PLAIN_TEXT = 1,
    /**
     * The primary text to be inserted is treated as a snippet.
     *
     * A snippet can define tab stops and placeholders with `$1`, `$2`
     * and `${3:foo}`. `$0` defines the final tab stop, it defaults to
     * the end of the snippet. Placeholders with equal identifiers are linked,
     * that is typing in one will update others too.
     *
     * See also: https://microsoft.github.io/language-server-protocol/specifications/specification-current/#snippet_syntax
     */
    SNIPPET = 2,
  };

  extern std::map<
    InsertTextFormat,
    std::string
  > InsertTextFormatNames;

  auto insertTextFormatByName(
    const std::string &name
  ) -> InsertTextFormat;

  auto insertTextFormatByValue(
    uinteger_t value
  ) -> InsertTextFormat;

  /**
   * How whitespace and indentation is handled during completion
   * item insertion.
   *
   * @since 3.16.0
   */
  enum class InsertTextMode {
    /**
     * The insertion or replace strings is taken as it is. If the
     * value is multi line the lines below the cursor will be
     * inserted using the indentation defined in the string value.
     * The client will not apply any kind of adjustments to the
     * string.
     */
    AS_IS = 1,
    /**
     * The editor adjusts leading whitespace of new lines so that
     * they match the indentation up to the cursor of the line for
     * which the item is accepted.
     *
     * Consider a line like this: <2tabs><cursor><3tabs>foo. Accepting a
     * multi line completion item is indented using 2 tabs and all
     * following lines inserted will be indented using 2 tabs as well.
     */
    ADJUST_INDENTATION = 2,
  };

  extern std::map<
    InsertTextMode,
    std::string
  > InsertTextModeNames;

  auto insertTextModeByName(
    const std::string &name
  ) -> InsertTextMode;

  auto insertTextModeByValue(
    uinteger_t value
  ) -> InsertTextMode;

  /**
   * A document highlight kind.
   */
  enum class DocumentHighlightKind {
    /**
     * A textual occurrence.
     */
    TEXT = 1,
    /**
     * Read-access of a symbol, like reading a variable.
     */
    READ = 2,
    /**
     * Write-access of a symbol, like writing to a variable.
     */
    WRITE = 3,
  };

  extern std::map<
    DocumentHighlightKind,
    std::string
  > DocumentHighlightKindNames;

  auto documentHighlightKindByName(
    const std::string &name
  ) -> DocumentHighlightKind;

  auto documentHighlightKindByValue(
    uinteger_t value
  ) -> DocumentHighlightKind;

  /**
   * A set of predefined code action kinds
   */
  enum class CodeActionKind {
    /**
     * Empty kind.
     */
    EMPTY,
    /**
     * Base kind for quickfix actions: 'quickfix'
     */
    QUICK_FIX,
    /**
     * Base kind for refactoring actions: 'refactor'
     */
    REFACTOR,
    /**
     * Base kind for refactoring extraction actions: 'refactor.extract'
     *
     * Example extract actions:
     *
     * - Extract method
     * - Extract function
     * - Extract variable
     * - Extract interface from class
     * - ...
     */
    REFACTOR_EXTRACT,
    /**
     * Base kind for refactoring inline actions: 'refactor.inline'
     *
     * Example inline actions:
     *
     * - Inline function
     * - Inline variable
     * - Inline constant
     * - ...
     */
    REFACTOR_INLINE,
    /**
     * Base kind for refactoring rewrite actions: 'refactor.rewrite'
     *
     * Example rewrite actions:
     *
     * - Convert JavaScript function to class
     * - Add or remove parameter
     * - Encapsulate field
     * - Make method static
     * - Move method to base class
     * - ...
     */
    REFACTOR_REWRITE,
    /**
     * Base kind for source actions: `source`
     *
     * Source code actions apply to the entire file.
     */
    SOURCE,
    /**
     * Base kind for an organize imports source action: `source.organizeImports`
     */
    SOURCE_ORGANIZE_IMPORTS,
    /**
     * Base kind for auto-fix source actions: `source.fixAll`.
     *
     * Fix all actions automatically fix errors that have a clear fix that do not require user input.
     * They should not suppress errors or perform unsafe fixes such as generating new types or classes.
     *
     * @since 3.15.0
     */
    SOURCE_FIX_ALL,
  };

  extern std::map<
    CodeActionKind,
    std::string
  > CodeActionKindNames;

  extern std::map<
    CodeActionKind,
    string_t
  > CodeActionKindValues;

  auto codeActionKindByName(
    const std::string &name
  ) -> CodeActionKind;

  auto codeActionKindByValue(
    const string_t &value
  ) -> CodeActionKind;

  enum class TraceValues {
    /**
     * Turn tracing off.
     */
    OFF,
    /**
     * Trace messages only.
     */
    MESSAGES,
    /**
     * Verbose message tracing.
     */
    VERBOSE,
  };

  extern std::map<
    TraceValues,
    std::string
  > TraceValuesNames;

  extern std::map<
    TraceValues,
    string_t
  > TraceValuesValues;

  auto traceValuesByName(
    const std::string &name
  ) -> TraceValues;

  auto traceValuesByValue(
    const string_t &value
  ) -> TraceValues;

  /**
   * Describes the content type that a client supports in various
   * result literals like `Hover`, `ParameterInfo` or `CompletionItem`.
   *
   * Please note that `MarkupKinds` must not start with a `$`. This kinds
   * are reserved for internal usage.
   */
  enum class MarkupKind {
    /**
     * Plain text is supported as a content format
     */
    PLAIN_TEXT,
    /**
     * Markdown is supported as a content format
     */
    MARKDOWN,
  };

  extern std::map<
    MarkupKind,
    std::string
  > MarkupKindNames;

  extern std::map<
    MarkupKind,
    string_t
  > MarkupKindValues;

  auto markupKindByName(
    const std::string &name
  ) -> MarkupKind;

  auto markupKindByValue(
    const string_t &value
  ) -> MarkupKind;

  /**
   * Describes how an {@link InlineCompletionItemProvider inline completion provider} was triggered.
   *
   * @since 3.18.0
   * @proposed
   */
  enum class InlineCompletionTriggerKind {
    /**
     * Completion was triggered explicitly by a user gesture.
     */
    INVOKED = 0,
    /**
     * Completion was triggered automatically while editing.
     */
    AUTOMATIC = 1,
  };

  extern std::map<
    InlineCompletionTriggerKind,
    std::string
  > InlineCompletionTriggerKindNames;

  auto inlineCompletionTriggerKindByName(
    const std::string &name
  ) -> InlineCompletionTriggerKind;

  auto inlineCompletionTriggerKindByValue(
    uinteger_t value
  ) -> InlineCompletionTriggerKind;

  /**
   * A set of predefined position encoding kinds.
   *
   * @since 3.17.0
   */
  enum class PositionEncodingKind {
    /**
     * Character offsets count UTF-8 code units (e.g. bytes).
     */
    UTF8,
    /**
     * Character offsets count UTF-16 code units.
     *
     * This is the default and must always be supported
     * by servers
     */
    UTF16,
    /**
     * Character offsets count UTF-32 code units.
     *
     * Implementation note: these are the same as Unicode codepoints,
     * so this `PositionEncodingKind` may also be used for an
     * encoding-agnostic representation of character offsets.
     */
    UTF32,
  };

  extern std::map<
    PositionEncodingKind,
    std::string
  > PositionEncodingKindNames;

  extern std::map<
    PositionEncodingKind,
    string_t
  > PositionEncodingKindValues;

  auto positionEncodingKindByName(
    const std::string &name
  ) -> PositionEncodingKind;

  auto positionEncodingKindByValue(
    const string_t &value
  ) -> PositionEncodingKind;

  /**
   * The file event type
   */
  enum class FileChangeType {
    /**
     * The file got created.
     */
    CREATED = 1,
    /**
     * The file got changed.
     */
    CHANGED = 2,
    /**
     * The file got deleted.
     */
    DELETED = 3,
  };

  extern std::map<
    FileChangeType,
    std::string
  > FileChangeTypeNames;

  auto fileChangeTypeByName(
    const std::string &name
  ) -> FileChangeType;

  auto fileChangeTypeByValue(
    uinteger_t value
  ) -> FileChangeType;

  enum class WatchKind {
    /**
     * Interested in create events.
     */
    CREATE = 1,
    /**
     * Interested in change events
     */
    CHANGE = 2,
    /**
     * Interested in delete events
     */
    DELETE = 4,
  };

  extern std::map<
    WatchKind,
    std::string
  > WatchKindNames;

  auto watchKindByName(
    const std::string &name
  ) -> WatchKind;

  auto watchKindByValue(
    uinteger_t value
  ) -> WatchKind;

  /**
   * The diagnostic's severity.
   */
  enum class DiagnosticSeverity {
    /**
     * Reports an error.
     */
    ERROR = 1,
    /**
     * Reports a warning.
     */
    WARNING = 2,
    /**
     * Reports an information.
     */
    INFORMATION = 3,
    /**
     * Reports a hint.
     */
    HINT = 4,
  };

  extern std::map<
    DiagnosticSeverity,
    std::string
  > DiagnosticSeverityNames;

  auto diagnosticSeverityByName(
    const std::string &name
  ) -> DiagnosticSeverity;

  auto diagnosticSeverityByValue(
    uinteger_t value
  ) -> DiagnosticSeverity;

  /**
   * The diagnostic tags.
   *
   * @since 3.15.0
   */
  enum class DiagnosticTag {
    /**
     * Unused or unnecessary code.
     *
     * Clients are allowed to render diagnostics with this tag faded out instead of having
     * an error squiggle.
     */
    UNNECESSARY = 1,
    /**
     * Deprecated or obsolete code.
     *
     * Clients are allowed to rendered diagnostics with this tag strike through.
     */
    DEPRECATED = 2,
  };

  extern std::map<
    DiagnosticTag,
    std::string
  > DiagnosticTagNames;

  auto diagnosticTagByName(
    const std::string &name
  ) -> DiagnosticTag;

  auto diagnosticTagByValue(
    uinteger_t value
  ) -> DiagnosticTag;

  /**
   * How a completion was triggered
   */
  enum class CompletionTriggerKind {
    /**
     * Completion was triggered by typing an identifier (24x7 code
     * complete), manual invocation (e.g Ctrl+Space) or via API.
     */
    INVOKED = 1,
    /**
     * Completion was triggered by a trigger character specified by
     * the `triggerCharacters` properties of the `CompletionRegistrationOptions`.
     */
    TRIGGER_CHARACTER = 2,
    /**
     * Completion was re-triggered as current completion list is incomplete
     */
    TRIGGER_FOR_INCOMPLETE_COMPLETIONS = 3,
  };

  extern std::map<
    CompletionTriggerKind,
    std::string
  > CompletionTriggerKindNames;

  auto completionTriggerKindByName(
    const std::string &name
  ) -> CompletionTriggerKind;

  auto completionTriggerKindByValue(
    uinteger_t value
  ) -> CompletionTriggerKind;

  /**
   * How a signature help was triggered.
   *
   * @since 3.15.0
   */
  enum class SignatureHelpTriggerKind {
    /**
     * Signature help was invoked manually by the user or by a command.
     */
    INVOKED = 1,
    /**
     * Signature help was triggered by a trigger character.
     */
    TRIGGER_CHARACTER = 2,
    /**
     * Signature help was triggered by the cursor moving or by the document content changing.
     */
    CONTENT_CHANGE = 3,
  };

  extern std::map<
    SignatureHelpTriggerKind,
    std::string
  > SignatureHelpTriggerKindNames;

  auto signatureHelpTriggerKindByName(
    const std::string &name
  ) -> SignatureHelpTriggerKind;

  auto signatureHelpTriggerKindByValue(
    uinteger_t value
  ) -> SignatureHelpTriggerKind;

  /**
   * The reason why code actions were requested.
   *
   * @since 3.17.0
   */
  enum class CodeActionTriggerKind {
    /**
     * Code actions were explicitly requested by the user or by an extension.
     */
    INVOKED = 1,
    /**
     * Code actions were requested automatically.
     *
     * This typically happens when current selection in a file changes, but can
     * also be triggered when file content changes.
     */
    AUTOMATIC = 2,
  };

  extern std::map<
    CodeActionTriggerKind,
    std::string
  > CodeActionTriggerKindNames;

  auto codeActionTriggerKindByName(
    const std::string &name
  ) -> CodeActionTriggerKind;

  auto codeActionTriggerKindByValue(
    uinteger_t value
  ) -> CodeActionTriggerKind;

  /**
   * A pattern kind describing if a glob pattern matches a file a folder or
   * both.
   *
   * @since 3.16.0
   */
  enum class FileOperationPatternKind {
    /**
     * The pattern matches a file only.
     */
    FILE,
    /**
     * The pattern matches a folder only.
     */
    FOLDER,
  };

  extern std::map<
    FileOperationPatternKind,
    std::string
  > FileOperationPatternKindNames;

  extern std::map<
    FileOperationPatternKind,
    string_t
  > FileOperationPatternKindValues;

  auto fileOperationPatternKindByName(
    const std::string &name
  ) -> FileOperationPatternKind;

  auto fileOperationPatternKindByValue(
    const string_t &value
  ) -> FileOperationPatternKind;

  /**
   * A notebook cell kind.
   *
   * @since 3.17.0
   */
  enum class NotebookCellKind {
    /**
     * A markup-cell is formatted source that is used for display.
     */
    MARKUP = 1,
    /**
     * A code-cell is source code.
     */
    CODE = 2,
  };

  extern std::map<
    NotebookCellKind,
    std::string
  > NotebookCellKindNames;

  auto notebookCellKindByName(
    const std::string &name
  ) -> NotebookCellKind;

  auto notebookCellKindByValue(
    uinteger_t value
  ) -> NotebookCellKind;

  enum class ResourceOperationKind {
    /**
     * Supports creating new files and folders.
     */
    CREATE,
    /**
     * Supports renaming existing files and folders.
     */
    RENAME,
    /**
     * Supports deleting existing files and folders.
     */
    DELETE,
  };

  extern std::map<
    ResourceOperationKind,
    std::string
  > ResourceOperationKindNames;

  extern std::map<
    ResourceOperationKind,
    string_t
  > ResourceOperationKindValues;

  auto resourceOperationKindByName(
    const std::string &name
  ) -> ResourceOperationKind;

  auto resourceOperationKindByValue(
    const string_t &value
  ) -> ResourceOperationKind;

  enum class FailureHandlingKind {
    /**
     * Applying the workspace change is simply aborted if one of the changes provided
     * fails. All operations executed before the failing operation stay executed.
     */
    ABORT,
    /**
     * All operations are executed transactional. That means they either all
     * succeed or no changes at all are applied to the workspace.
     */
    TRANSACTIONAL,
    /**
     * If the workspace edit contains only textual file changes they are executed transactional.
     * If resource changes (create, rename or delete file) are part of the change the failure
     * handling strategy is abort.
     */
    TEXT_ONLY_TRANSACTIONAL,
    /**
     * The client tries to undo the operations already executed. But there is no
     * guarantee that this is succeeding.
     */
    UNDO,
  };

  extern std::map<
    FailureHandlingKind,
    std::string
  > FailureHandlingKindNames;

  extern std::map<
    FailureHandlingKind,
    string_t
  > FailureHandlingKindValues;

  auto failureHandlingKindByName(
    const std::string &name
  ) -> FailureHandlingKind;

  auto failureHandlingKindByValue(
    const string_t &value
  ) -> FailureHandlingKind;

  enum class PrepareSupportDefaultBehavior {
    /**
     * The client's default behavior is to select the identifier
     * according the to language's syntax rule.
     */
    IDENTIFIER = 1,
  };

  extern std::map<
    PrepareSupportDefaultBehavior,
    std::string
  > PrepareSupportDefaultBehaviorNames;

  auto prepareSupportDefaultBehaviorByName(
    const std::string &name
  ) -> PrepareSupportDefaultBehavior;

  auto prepareSupportDefaultBehaviorByValue(
    uinteger_t value
  ) -> PrepareSupportDefaultBehavior;

  enum class TokenFormat {
    RELATIVE,
  };

  extern std::map<
    TokenFormat,
    std::string
  > TokenFormatNames;

  extern std::map<
    TokenFormat,
    string_t
  > TokenFormatValues;

  auto tokenFormatByName(
    const std::string &name
  ) -> TokenFormat;

  auto tokenFormatByValue(
    const string_t &value
  ) -> TokenFormat;

  /**
   * A general message as defined by JSON-RPC. The language server protocol
   * always uses “2.0” as the jsonrpc version.
   */
  struct Message
  {
    string_t jsonrpc;
  };

  struct ResponseError
  {
    /**
     * A number indicating the error type that occurred.
     */
    integer_t code;
    /**
     * A string providing a short description of the error.
     */
    string_t message;
    /**
     * A primitive or structured value that contains additional information about
     * the error. Can be omitted.
     */
    std::optional<std::unique_ptr<LSPAny>> data;
  };

  /**
   * A workspace folder inside a client.
   */
  struct WorkspaceFolder
  {
    /**
     * The associated URI for this workspace folder.
     */
    URI uri;
    /**
     * The name of the workspace folder. Used to refer to this
     * workspace folder in the user interface.
     */
    string_t name;
  };

  struct WorkDoneProgressOptions
  {
    std::optional<boolean_t> workDoneProgress;
  };

  /**
   * Represents a folding range. To be valid, start and end line must be bigger than zero and smaller
   * than the number of lines in the document. Clients are free to ignore invalid ranges.
   */
  struct FoldingRange
  {
    /**
     * The zero-based start line of the range to fold. The folded area starts after the line's last character.
     * To be valid, the end must be zero or larger and smaller than the number of lines in the document.
     */
    uinteger_t startLine;
    /**
     * The zero-based character offset from where the folded range starts. If not defined, defaults to the length of the start line.
     */
    std::optional<uinteger_t> startCharacter;
    /**
     * The zero-based end line of the range to fold. The folded area ends with the line's last character.
     * To be valid, the end must be zero or larger and smaller than the number of lines in the document.
     */
    uinteger_t endLine;
    /**
     * The zero-based character offset before the folded range ends. If not defined, defaults to the length of the end line.
     */
    std::optional<uinteger_t> endCharacter;
    /**
     * Describes the kind of the folding range such as `comment' or 'region'. The kind
     * is used to categorize folding ranges and used by commands like 'Fold all comments'.
     * See {@link FoldingRangeKind} for an enumeration of standardized kinds.
     */
    std::optional<FoldingRangeKind> kind;
    /**
     * The text that the client should show when the specified range is
     * collapsed. If not defined or not supported by the client, a default
     * will be chosen by the client.
     *
     * @since 3.17.0
     */
    std::optional<string_t> collapsedText;
  };

  /**
   * @since 3.16.0
   */
  struct SemanticTokens
  {
    /**
     * An optional result id. If provided and clients support delta updating
     * the client will include the result id in the next semantic token request.
     * A server can then instead of computing all semantic tokens again simply
     * send a delta.
     */
    std::optional<string_t> resultId;
    /**
     * The actual tokens.
     */
    std::vector<uinteger_t> data;
  };

  /**
   * @since 3.16.0
   */
  struct SemanticTokensPartialResult
  {
    std::vector<uinteger_t> data;
  };

  /**
   * The result of a showDocument request.
   *
   * @since 3.16.0
   */
  struct ShowDocumentResult
  {
    /**
     * A boolean indicating if the show was successful.
     */
    boolean_t success;
  };

  /**
   * Moniker definition to match LSIF 0.5 moniker definition.
   *
   * @since 3.16.0
   */
  struct Moniker
  {
    /**
     * The scheme of the moniker. For example tsc or .Net
     */
    string_t scheme;
    /**
     * The identifier of the moniker. The value is opaque in LSIF however
     * schema owners are allowed to define the structure if they want.
     */
    string_t identifier;
    /**
     * The scope in which the moniker is unique
     */
    UniquenessLevel unique;
    /**
     * The moniker kind if known.
     */
    std::optional<MonikerKind> kind;
  };

  /**
   * Cancellation data returned from a diagnostic request.
   *
   * @since 3.17.0
   */
  struct DiagnosticServerCancellationData
  {
    boolean_t retriggerRequest;
  };

  /**
   * The data type of the ResponseError if the
   * initialize request fails.
   */
  struct InitializeError
  {
    /**
     * Indicates whether the client execute the following retry logic:
     * (1) show the message provided by the ResponseError to the user
     * (2) user selects retry or cancel
     * (3) if user selected retry the initialize method is sent again.
     */
    boolean_t retry;
  };

  struct InitializedParams
  {
  };

  /**
   * The parameters of a change configuration notification.
   */
  struct DidChangeConfigurationParams
  {
    /**
     * The actual changed settings
     */
    std::unique_ptr<LSPAny> settings;
  };

  enum class DidChangeConfigurationRegistrationOptions_sectionType {
    STRING_TYPE,
    STRING_TYPE_ARRAY,
  };

  extern std::map<DidChangeConfigurationRegistrationOptions_sectionType, std::string> DidChangeConfigurationRegistrationOptions_sectionTypeNames;

  typedef std::variant<
    string_t,
    std::vector<string_t>
  > DidChangeConfigurationRegistrationOptions_section;

  struct DidChangeConfigurationRegistrationOptions
  {
    std::optional<DidChangeConfigurationRegistrationOptions_section> section;
  };

  /**
   * The parameters of a notification message.
   */
  struct ShowMessageParams
  {
    /**
     * The message type. See {@link MessageType}
     */
    MessageType type;
    /**
     * The actual message.
     */
    string_t message;
  };

  struct MessageActionItem
  {
    /**
     * A short title like 'Retry', 'Open Log' etc.
     */
    string_t title;
  };

  struct ShowMessageRequestParams
  {
    /**
     * The message type. See {@link MessageType}
     */
    MessageType type;
    /**
     * The actual message.
     */
    string_t message;
    /**
     * The message action items to present.
     */
    std::optional<std::vector<std::unique_ptr<MessageActionItem>>> actions;
  };

  /**
   * The log message parameters.
   */
  struct LogMessageParams
  {
    /**
     * The message type. See {@link MessageType}
     */
    MessageType type;
    /**
     * The actual message.
     */
    string_t message;
  };

  /**
   * Represents a reference to a command. Provides a title which
   * will be used to represent a command in the UI and, optionally,
   * an array of arguments which will be passed to the command handler
   * function when invoked.
   */
  struct Command
  {
    /**
     * Title of the command, like `save`.
     */
    string_t title;
    /**
     * The identifier of the actual command handler.
     */
    string_t command;
    /**
     * Arguments that the command handler should be
     * invoked with.
     */
    std::optional<std::vector<std::unique_ptr<LSPAny>>> arguments;
  };

  /**
   * The result returned from the apply workspace edit request.
   *
   * @since 3.17 renamed from ApplyWorkspaceEditResponse
   */
  struct ApplyWorkspaceEditResult
  {
    /**
     * Indicates whether the edit was applied or not.
     */
    boolean_t applied;
    /**
     * An optional textual description for why the edit was not applied.
     * This may be used by the server for diagnostic logging or to provide
     * a suitable error for a request that triggered the edit.
     */
    std::optional<string_t> failureReason;
    /**
     * Depending on the client's failure handling strategy `failedChange` might
     * contain the index of the change that failed. This property is only available
     * if the client signals a `failureHandlingStrategy` in its client capabilities.
     */
    std::optional<uinteger_t> failedChange;
  };

  struct WorkDoneProgressBegin
  {
    string_t kind;
    /**
     * Mandatory title of the progress operation. Used to briefly inform about
     * the kind of operation being performed.
     *
     * Examples: "Indexing" or "Linking dependencies".
     */
    string_t title;
    /**
     * Controls if a cancel button should show to allow the user to cancel the
     * long running operation. Clients that don't support cancellation are allowed
     * to ignore the setting.
     */
    std::optional<boolean_t> cancellable;
    /**
     * Optional, more detailed associated progress message. Contains
     * complementary information to the `title`.
     *
     * Examples: "3/25 files", "project/src/module2", "node_modules/some_dep".
     * If unset, the previous progress message (if any) is still valid.
     */
    std::optional<string_t> message;
    /**
     * Optional progress percentage to display (value 100 is considered 100%).
     * If not provided infinite progress is assumed and clients are allowed
     * to ignore the `percentage` value in subsequent report notifications.
     *
     * The value should be steadily rising. Clients are free to ignore values
     * that are not following this rule. The value range is [0, 100].
     */
    std::optional<uinteger_t> percentage;
  };

  struct WorkDoneProgressReport
  {
    string_t kind;
    /**
     * Controls enablement state of a cancel button.
     *
     * Clients that don't support cancellation or don't support controlling the button's
     * enablement state are allowed to ignore the property.
     */
    std::optional<boolean_t> cancellable;
    /**
     * Optional, more detailed associated progress message. Contains
     * complementary information to the `title`.
     *
     * Examples: "3/25 files", "project/src/module2", "node_modules/some_dep".
     * If unset, the previous progress message (if any) is still valid.
     */
    std::optional<string_t> message;
    /**
     * Optional progress percentage to display (value 100 is considered 100%).
     * If not provided infinite progress is assumed and clients are allowed
     * to ignore the `percentage` value in subsequent report notifications.
     *
     * The value should be steadily rising. Clients are free to ignore values
     * that are not following this rule. The value range is [0, 100].
     */
    std::optional<uinteger_t> percentage;
  };

  struct WorkDoneProgressEnd
  {
    string_t kind;
    /**
     * Optional, a final message indicating to for example indicate the outcome
     * of the operation.
     */
    std::optional<string_t> message;
  };

  struct SetTraceParams
  {
    TraceValues value;
  };

  struct LogTraceParams
  {
    string_t message;
    std::optional<string_t> verbose;
  };

  enum class CancelParams_idType {
    INTEGER_TYPE,
    STRING_TYPE,
  };

  extern std::map<CancelParams_idType, std::string> CancelParams_idTypeNames;

  typedef std::variant<
    integer_t,
    string_t
  > CancelParams_id;

  struct CancelParams
  {
    /**
     * The request id to cancel.
     */
    CancelParams_id id;
  };

  struct ImplementationOptions
  {
    std::optional<boolean_t> workDoneProgress;
  };

  /**
   * Static registration options to be returned in the initialize
   * request.
   */
  struct StaticRegistrationOptions
  {
    /**
     * The id used to register the request. The id can be used to deregister
     * the request again. See also Registration#id.
     */
    std::optional<string_t> id;
  };

  struct TypeDefinitionOptions
  {
    std::optional<boolean_t> workDoneProgress;
  };

  /**
   * The workspace folder change event.
   */
  struct WorkspaceFoldersChangeEvent
  {
    /**
     * The array of added workspace folders
     */
    std::vector<std::unique_ptr<WorkspaceFolder>> added;
    /**
     * The array of the removed workspace folders
     */
    std::vector<std::unique_ptr<WorkspaceFolder>> removed;
  };

  /**
   * The parameters of a `workspace/didChangeWorkspaceFolders` notification.
   */
  struct DidChangeWorkspaceFoldersParams
  {
    /**
     * The actual workspace folder change event.
     */
    std::unique_ptr<WorkspaceFoldersChangeEvent> event;
  };

  struct ConfigurationItem
  {
    /**
     * The scope to get the configuration section for.
     */
    std::optional<URI> scopeUri;
    /**
     * The configuration section asked for.
     */
    std::optional<string_t> section;
  };

  /**
   * The parameters of a configuration request.
   */
  struct ConfigurationParams
  {
    std::vector<std::unique_ptr<ConfigurationItem>> items;
  };

  /**
   * A literal to identify a text document in the client.
   */
  struct TextDocumentIdentifier
  {
    /**
     * The text document's uri.
     */
    DocumentUri uri;
  };

  /**
   * The parameters sent in a will save text document notification.
   */
  struct WillSaveTextDocumentParams
  {
    /**
     * The document that will be saved.
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
    /**
     * The 'TextDocumentSaveReason'.
     */
    TextDocumentSaveReason reason;
  };

  /**
   * The parameters sent in a close text document notification
   */
  struct DidCloseTextDocumentParams
  {
    /**
     * The document that was closed.
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
  };

  /**
   * The parameters sent in a save text document notification
   */
  struct DidSaveTextDocumentParams
  {
    /**
     * The document that was saved.
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
    /**
     * Optional the content when saved. Depends on the includeText value
     * when the save notification was requested.
     */
    std::optional<string_t> text;
  };

  /**
   * Represents a color in RGBA space.
   */
  struct Color
  {
    /**
     * The red component of this color in the range [0-1].
     */
    decimal_t red;
    /**
     * The green component of this color in the range [0-1].
     */
    decimal_t green;
    /**
     * The blue component of this color in the range [0-1].
     */
    decimal_t blue;
    /**
     * The alpha component of this color in the range [0-1].
     */
    decimal_t alpha;
  };

  struct DocumentColorOptions
  {
    std::optional<boolean_t> workDoneProgress;
  };

  struct FoldingRangeOptions
  {
    std::optional<boolean_t> workDoneProgress;
  };

  struct DeclarationOptions
  {
    std::optional<boolean_t> workDoneProgress;
  };

  /**
   * Position in a text document expressed as zero-based line and character
   * offset. Prior to 3.17 the offsets were always based on a UTF-16 string
   * representation. So a string of the form `a𐐀b` the character offset of the
   * character `a` is 0, the character offset of `𐐀` is 1 and the character
   * offset of b is 3 since `𐐀` is represented using two code units in UTF-16.
   * Since 3.17 clients and servers can agree on a different string encoding
   * representation (e.g. UTF-8). The client announces it's supported encoding
   * via the client capability [`general.positionEncodings`](https://microsoft.github.io/language-server-protocol/specifications/specification-current/#clientCapabilities).
   * The value is an array of position encodings the client supports, with
   * decreasing preference (e.g. the encoding at index `0` is the most preferred
   * one). To stay backwards compatible the only mandatory encoding is UTF-16
   * represented via the string `utf-16`. The server can pick one of the
   * encodings offered by the client and signals that encoding back to the
   * client via the initialize result's property
   * [`capabilities.positionEncoding`](https://microsoft.github.io/language-server-protocol/specifications/specification-current/#serverCapabilities). If the string value
   * `utf-16` is missing from the client's capability `general.positionEncodings`
   * servers can safely assume that the client supports UTF-16. If the server
   * omits the position encoding in its initialize result the encoding defaults
   * to the string value `utf-16`. Implementation considerations: since the
   * conversion from one encoding into another requires the content of the
   * file / line the conversion is best done where the file is read which is
   * usually on the server side.
   *
   * Positions are line end character agnostic. So you can not specify a position
   * that denotes `\r|\n` or `\n|` where `|` represents the character offset.
   *
   * @since 3.17.0 - support for negotiated position encoding.
   */
  struct Position
  {
    /**
     * Line position in a document (zero-based).
     *
     * If a line number is greater than the number of lines in a document, it defaults back to the number of lines in the document.
     * If a line number is negative, it defaults to 0.
     */
    uinteger_t line;
    /**
     * Character offset on a line in a document (zero-based).
     *
     * The meaning of this offset is determined by the negotiated
     * `PositionEncodingKind`.
     *
     * If the character value is greater than the line length it defaults back to the
     * line length.
     */
    uinteger_t character;
  };

  /**
   * A parameter literal used in requests to pass a text document and a position inside that
   * document.
   */
  struct TextDocumentPositionParams
  {
    /**
     * The text document.
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
    /**
     * The position inside the text document.
     */
    std::unique_ptr<Position> position;
  };

  /**
   * A range in a text document expressed as (zero-based) start and end positions.
   *
   * If you want to specify a range that contains a line including the line ending
   * character(s) then use an end position denoting the start of the next line.
   * For example:
   * ```ts
   * {
   *     start: { line: 5, character: 23 }
   *     end : { line 6, character : 0 }
   * }
   * ```
   */
  struct Range
  {
    /**
     * The range's start position.
     */
    std::unique_ptr<Position> start;
    /**
     * The range's end position.
     */
    std::unique_ptr<Position> end;
  };

  /**
   * Represents the connection of two locations. Provides additional metadata over normal {@link Location locations},
   * including an origin range.
   */
  struct LocationLink
  {
    /**
     * Span of the origin of this link.
     *
     * Used as the underlined span for mouse interaction. Defaults to the word range at
     * the definition position.
     */
    std::optional<std::unique_ptr<Range>> originSelectionRange;
    /**
     * The target resource identifier of this link.
     */
    DocumentUri targetUri;
    /**
     * The full target range of this link. If the target for example is a symbol then target range is the
     * range enclosing this symbol not including leading/trailing whitespace but everything else
     * like comments. This information is typically used to highlight the range in the editor.
     */
    std::unique_ptr<Range> targetRange;
    /**
     * The range that should be selected and revealed when this link is being followed, e.g the name of a function.
     * Must be contained by the `targetRange`. See also `DocumentSymbol#range`
     */
    std::unique_ptr<Range> targetSelectionRange;
  };

  /**
   * A document link is a range in a text document that links to an internal or external resource, like another
   * text document or a web site.
   */
  struct DocumentLink
  {
    /**
     * The range this link applies to.
     */
    std::unique_ptr<Range> range;
    /**
     * The uri this link points to. If missing a resolve request is sent later.
     */
    std::optional<URI> target;
    /**
     * The tooltip text when you hover over this link.
     *
     * If a tooltip is provided, is will be displayed in a string that includes instructions on how to
     * trigger the link, such as `{0} (ctrl + click)`. The specific instructions vary depending on OS,
     * user settings, and localization.
     *
     * @since 3.15.0
     */
    std::optional<string_t> tooltip;
    /**
     * A data entry field that is preserved on a document link between a
     * DocumentLinkRequest and a DocumentLinkResolveRequest.
     */
    std::optional<std::unique_ptr<LSPAny>> data;
  };

  /**
   * Represents a color range from a document.
   */
  struct ColorInformation
  {
    /**
     * The range in the document where this color appears.
     */
    std::unique_ptr<Range> range;
    /**
     * The actual color value for this color range.
     */
    std::unique_ptr<Color> color;
  };

  /**
   * A text edit applicable to a text document.
   */
  struct TextEdit
  {
    /**
     * The range of the text document to be manipulated. To insert
     * text into a document create a range where start === end.
     */
    std::unique_ptr<Range> range;
    /**
     * The string to be inserted. For delete operations use an
     * empty string.
     */
    string_t newText;
  };

  struct ColorPresentation
  {
    /**
     * The label of this color presentation. It will be shown on the color
     * picker header. By default this is also the text that is inserted when selecting
     * this color presentation.
     */
    string_t label;
    /**
     * An {@link TextEdit edit} which is applied to a document when selecting
     * this presentation for the color.  When `falsy` the {@link ColorPresentation.label label}
     * is used.
     */
    std::optional<std::unique_ptr<TextEdit>> textEdit;
    /**
     * An optional array of additional {@link TextEdit text edits} that are applied when
     * selecting this color presentation. Edits must not overlap with the main {@link ColorPresentation.textEdit edit} nor with themselves.
     */
    std::optional<std::vector<std::unique_ptr<TextEdit>>> additionalTextEdits;
  };

  /**
   * Params to show a resource in the UI.
   *
   * @since 3.16.0
   */
  struct ShowDocumentParams
  {
    /**
     * The uri to show.
     */
    URI uri;
    /**
     * Indicates to show the resource in an external program.
     * To show, for example, `https://code.visualstudio.com/`
     * in the default WEB browser set `external` to `true`.
     */
    std::optional<boolean_t> external;
    /**
     * An optional property to indicate whether the editor
     * showing the document should take focus or not.
     * Clients might ignore this property if an external
     * program is started.
     */
    std::optional<boolean_t> takeFocus;
    /**
     * An optional selection range if the document is a text
     * document. Clients might ignore the property if an
     * external program is started or the file is not a text
     * file.
     */
    std::optional<std::unique_ptr<Range>> selection;
  };

  /**
   * Represents programming constructs like functions or constructors in the context
   * of call hierarchy.
   *
   * @since 3.16.0
   */
  struct CallHierarchyItem
  {
    /**
     * The name of this item.
     */
    string_t name;
    /**
     * The kind of this item.
     */
    SymbolKind kind;
    /**
     * Tags for this item.
     */
    std::optional<std::vector<SymbolTag>> tags;
    /**
     * More detail for this item, e.g. the signature of a function.
     */
    std::optional<string_t> detail;
    /**
     * The resource identifier of this item.
     */
    DocumentUri uri;
    /**
     * The range enclosing this symbol not including leading/trailing whitespace but everything else, e.g. comments and code.
     */
    std::unique_ptr<Range> range;
    /**
     * The range that should be selected and revealed when this symbol is being picked, e.g. the name of a function.
     * Must be contained by the {@link CallHierarchyItem.range `range`}.
     */
    std::unique_ptr<Range> selectionRange;
    /**
     * A data entry field that is preserved between a call hierarchy prepare and
     * incoming calls or outgoing calls requests.
     */
    std::optional<std::unique_ptr<LSPAny>> data;
  };

  /**
   * Represents an incoming call, e.g. a caller of a method or constructor.
   *
   * @since 3.16.0
   */
  struct CallHierarchyIncomingCall
  {
    /**
     * The item that makes the call.
     */
    std::unique_ptr<CallHierarchyItem> from;
    /**
     * The ranges at which the calls appear. This is relative to the caller
     * denoted by {@link CallHierarchyIncomingCall.from `this.from`}.
     */
    std::vector<std::unique_ptr<Range>> fromRanges;
  };

  /**
   * Represents an outgoing call, e.g. calling a getter from a method or a method from a constructor etc.
   *
   * @since 3.16.0
   */
  struct CallHierarchyOutgoingCall
  {
    /**
     * The item that is called.
     */
    std::unique_ptr<CallHierarchyItem> to;
    /**
     * The range at which this item is called. This is the range relative to the caller, e.g the item
     * passed to {@link CallHierarchyItemProvider.provideCallHierarchyOutgoingCalls `provideCallHierarchyOutgoingCalls`}
     * and not {@link CallHierarchyOutgoingCall.to `this.to`}.
     */
    std::vector<std::unique_ptr<Range>> fromRanges;
  };

  /**
   * Represents a location inside a resource, such as a line
   * inside a text file.
   */
  struct Location
  {
    DocumentUri uri;
    std::unique_ptr<Range> range;
  };

  /**
   * The result of a linked editing range request.
   *
   * @since 3.16.0
   */
  struct LinkedEditingRanges
  {
    /**
     * A list of ranges that can be edited together. The ranges must have
     * identical length and contain identical text content. The ranges cannot overlap.
     */
    std::vector<std::unique_ptr<Range>> ranges;
    /**
     * An optional word pattern (regular expression) that describes valid contents for
     * the given ranges. If no pattern is provided, the client configuration's word
     * pattern will be used.
     */
    std::optional<string_t> wordPattern;
  };

  /**
   * A document highlight is a range inside a text document which deserves
   * special attention. Usually a document highlight is visualized by changing
   * the background color of its range.
   */
  struct DocumentHighlight
  {
    /**
     * The range this highlight applies to.
     */
    std::unique_ptr<Range> range;
    /**
     * The highlight kind, default is {@link DocumentHighlightKind.Text text}.
     */
    std::optional<DocumentHighlightKind> kind;
  };

  /**
   * A code lens represents a {@link Command command} that should be shown along with
   * source text, like the number of references, a way to run tests, etc.
   *
   * A code lens is _unresolved_ when no command is associated to it. For performance
   * reasons the creation of a code lens and resolving should be done in two stages.
   */
  struct CodeLens
  {
    /**
     * The range in which this code lens is valid. Should only span a single line.
     */
    std::unique_ptr<Range> range;
    /**
     * The command this code lens represents.
     */
    std::optional<std::unique_ptr<Command>> command;
    /**
     * A data entry field that is preserved on a code lens item between
     * a {@link CodeLensRequest} and a {@link CodeLensResolveRequest}
     */
    std::optional<std::unique_ptr<LSPAny>> data;
  };

  /**
   * @since 3.17.0
   */
  struct TypeHierarchyItem
  {
    /**
     * The name of this item.
     */
    string_t name;
    /**
     * The kind of this item.
     */
    SymbolKind kind;
    /**
     * Tags for this item.
     */
    std::optional<std::vector<SymbolTag>> tags;
    /**
     * More detail for this item, e.g. the signature of a function.
     */
    std::optional<string_t> detail;
    /**
     * The resource identifier of this item.
     */
    DocumentUri uri;
    /**
     * The range enclosing this symbol not including leading/trailing whitespace
     * but everything else, e.g. comments and code.
     */
    std::unique_ptr<Range> range;
    /**
     * The range that should be selected and revealed when this symbol is being
     * picked, e.g. the name of a function. Must be contained by the
     * {@link TypeHierarchyItem.range `range`}.
     */
    std::unique_ptr<Range> selectionRange;
    /**
     * A data entry field that is preserved between a type hierarchy prepare and
     * supertypes or subtypes requests. It could also be used to identify the
     * type hierarchy in the server, helping improve the performance on
     * resolving supertypes and subtypes.
     */
    std::optional<std::unique_ptr<LSPAny>> data;
  };

  struct SelectionRangeOptions
  {
    std::optional<boolean_t> workDoneProgress;
  };

  /**
   * Call hierarchy options used during static registration.
   *
   * @since 3.16.0
   */
  struct CallHierarchyOptions
  {
    std::optional<boolean_t> workDoneProgress;
  };

  /**
   * @since 3.16.0
   */
  struct SemanticTokensEdit
  {
    /**
     * The start offset of the edit.
     */
    uinteger_t start;
    /**
     * The count of elements to remove.
     */
    uinteger_t deleteCount;
    /**
     * The elements to insert.
     */
    std::optional<std::vector<uinteger_t>> data;
  };

  /**
   * @since 3.16.0
   */
  struct SemanticTokensDeltaPartialResult
  {
    std::vector<std::unique_ptr<SemanticTokensEdit>> edits;
  };

  /**
   * @since 3.16.0
   */
  struct SemanticTokensDelta
  {
    std::optional<string_t> resultId;
    /**
     * The semantic token edits to transform a previous result into a new result.
     */
    std::vector<std::unique_ptr<SemanticTokensEdit>> edits;
  };

  struct LinkedEditingRangeOptions
  {
    std::optional<boolean_t> workDoneProgress;
  };

  /**
   * Represents information on a file/folder create.
   *
   * @since 3.16.0
   */
  struct FileCreate
  {
    /**
     * A file:// URI for the location of the file/folder being created.
     */
    string_t uri;
  };

  /**
   * The parameters sent in notifications/requests for user-initiated creation of
   * files.
   *
   * @since 3.16.0
   */
  struct CreateFilesParams
  {
    /**
     * An array of all files/folders created in this operation.
     */
    std::vector<std::unique_ptr<FileCreate>> files;
  };

  /**
   * Additional information that describes document changes.
   *
   * @since 3.16.0
   */
  struct ChangeAnnotation
  {
    /**
     * A human-readable string describing the actual change. The string
     * is rendered prominent in the user interface.
     */
    string_t label;
    /**
     * A flag which indicates that user confirmation is needed
     * before applying the change.
     */
    std::optional<boolean_t> needsConfirmation;
    /**
     * A human-readable string which is rendered less prominent in
     * the user interface.
     */
    std::optional<string_t> description;
  };

  /**
   * Represents information on a file/folder rename.
   *
   * @since 3.16.0
   */
  struct FileRename
  {
    /**
     * A file:// URI for the original location of the file/folder being renamed.
     */
    string_t oldUri;
    /**
     * A file:// URI for the new location of the file/folder being renamed.
     */
    string_t newUri;
  };

  /**
   * The parameters sent in notifications/requests for user-initiated renames of
   * files.
   *
   * @since 3.16.0
   */
  struct RenameFilesParams
  {
    /**
     * An array of all files/folders renamed in this operation. When a folder is renamed, only
     * the folder will be included, and not its children.
     */
    std::vector<std::unique_ptr<FileRename>> files;
  };

  /**
   * Represents information on a file/folder delete.
   *
   * @since 3.16.0
   */
  struct FileDelete
  {
    /**
     * A file:// URI for the location of the file/folder being deleted.
     */
    string_t uri;
  };

  /**
   * The parameters sent in notifications/requests for user-initiated deletes of
   * files.
   *
   * @since 3.16.0
   */
  struct DeleteFilesParams
  {
    /**
     * An array of all files/folders deleted in this operation.
     */
    std::vector<std::unique_ptr<FileDelete>> files;
  };

  struct MonikerOptions
  {
    std::optional<boolean_t> workDoneProgress;
  };

  /**
   * Type hierarchy options used during static registration.
   *
   * @since 3.17.0
   */
  struct TypeHierarchyOptions
  {
    std::optional<boolean_t> workDoneProgress;
  };

  /**
   * @since 3.17.0
   */
  struct InlineValueContext
  {
    /**
     * The stack frame (as a DAP Id) where the execution has stopped.
     */
    integer_t frameId;
    /**
     * The document range where execution has stopped.
     * Typically the end position of the range denotes the line where the inline values are shown.
     */
    std::unique_ptr<Range> stoppedLocation;
  };

  /**
   * Provide inline value as text.
   *
   * @since 3.17.0
   */
  struct InlineValueText
  {
    /**
     * The document range for which the inline value applies.
     */
    std::unique_ptr<Range> range;
    /**
     * The text of the inline value.
     */
    string_t text;
  };

  /**
   * Provide inline value through a variable lookup.
   * If only a range is specified, the variable name will be extracted from the underlying document.
   * An optional variable name can be used to override the extracted name.
   *
   * @since 3.17.0
   */
  struct InlineValueVariableLookup
  {
    /**
     * The document range for which the inline value applies.
     * The range is used to extract the variable name from the underlying document.
     */
    std::unique_ptr<Range> range;
    /**
     * If specified the name of the variable to look up.
     */
    std::optional<string_t> variableName;
    /**
     * How to perform the lookup.
     */
    boolean_t caseSensitiveLookup;
  };

  /**
   * Provide an inline value through an expression evaluation.
   * If only a range is specified, the expression will be extracted from the underlying document.
   * An optional expression can be used to override the extracted expression.
   *
   * @since 3.17.0
   */
  struct InlineValueEvaluatableExpression
  {
    /**
     * The document range for which the inline value applies.
     * The range is used to extract the evaluatable expression from the underlying document.
     */
    std::unique_ptr<Range> range;
    /**
     * If specified the expression overrides the extracted expression.
     */
    std::optional<string_t> expression;
  };

  /**
   * Inline value options used during static registration.
   *
   * @since 3.17.0
   */
  struct InlineValueOptions
  {
    std::optional<boolean_t> workDoneProgress;
  };

  /**
   * A `MarkupContent` literal represents a string value which content is interpreted base on its
   * kind flag. Currently the protocol supports `plaintext` and `markdown` as markup kinds.
   *
   * If the kind is `markdown` then the value can contain fenced code blocks like in GitHub issues.
   * See https://help.github.com/articles/creating-and-highlighting-code-blocks/#syntax-highlighting
   *
   * Here is an example how such a string can be constructed using JavaScript / TypeScript:
   * ```ts
   * let markdown: MarkdownContent = {
   *  kind: MarkupKind.Markdown,
   *  value: [
   *    '# Header',
   *    'Some text',
   *    '```typescript',
   *    'someCode();',
   *    '```'
   *  ].join('\n')
   * };
   * ```
   *
   * *Please Note* that clients might sanitize the return markdown. A client could decide to
   * remove HTML from the markdown to avoid script execution.
   */
  struct MarkupContent
  {
    /**
     * The type of the Markup
     */
    MarkupKind kind;
    /**
     * The content itself
     */
    string_t value;
  };

  enum class InlayHintLabelPart_tooltipType {
    STRING_TYPE,
    MARKUP_CONTENT,
  };

  extern std::map<InlayHintLabelPart_tooltipType, std::string> InlayHintLabelPart_tooltipTypeNames;

  typedef std::variant<
    string_t,
    std::unique_ptr<MarkupContent>
  > InlayHintLabelPart_tooltip;

  /**
   * An inlay hint label part allows for interactive and composite labels
   * of inlay hints.
   *
   * @since 3.17.0
   */
  struct InlayHintLabelPart
  {
    /**
     * The value of this label part.
     */
    string_t value;
    /**
     * The tooltip text when you hover over this label part. Depending on
     * the client capability `inlayHint.resolveSupport` clients might resolve
     * this property late using the resolve request.
     */
    std::optional<InlayHintLabelPart_tooltip> tooltip;
    /**
     * An optional source code location that represents this
     * label part.
     *
     * The editor will use this location for the hover and for code navigation
     * features: This part will become a clickable link that resolves to the
     * definition of the symbol at the given location (not necessarily the
     * location itself), it shows the hover that shows at the given location,
     * and it shows a context menu with further code navigation commands.
     *
     * Depending on the client capability `inlayHint.resolveSupport` clients
     * might resolve this property late using the resolve request.
     */
    std::optional<std::unique_ptr<Location>> location;
    /**
     * An optional command for this label part.
     *
     * Depending on the client capability `inlayHint.resolveSupport` clients
     * might resolve this property late using the resolve request.
     */
    std::optional<std::unique_ptr<Command>> command;
  };

  enum class InlayHint_labelType {
    STRING_TYPE,
    INLAY_HINT_LABEL_PART_ARRAY,
  };

  extern std::map<InlayHint_labelType, std::string> InlayHint_labelTypeNames;

  typedef std::variant<
    string_t,
    std::vector<std::unique_ptr<InlayHintLabelPart>>
  > InlayHint_label;

  enum class InlayHint_tooltipType {
    STRING_TYPE,
    MARKUP_CONTENT,
  };

  extern std::map<InlayHint_tooltipType, std::string> InlayHint_tooltipTypeNames;

  typedef std::variant<
    string_t,
    std::unique_ptr<MarkupContent>
  > InlayHint_tooltip;

  /**
   * Inlay hint information.
   *
   * @since 3.17.0
   */
  struct InlayHint
  {
    /**
     * The position of this hint.
     *
     * If multiple hints have the same position, they will be shown in the order
     * they appear in the response.
     */
    std::unique_ptr<Position> position;
    /**
     * The label of this hint. A human readable string or an array of
     * InlayHintLabelPart label parts.
     *
     * *Note* that neither the string nor the label part can be empty.
     */
    InlayHint_label label;
    /**
     * The kind of this hint. Can be omitted in which case the client
     * should fall back to a reasonable default.
     */
    std::optional<InlayHintKind> kind;
    /**
     * Optional text edits that are performed when accepting this inlay hint.
     *
     * *Note* that edits are expected to change the document so that the inlay
     * hint (or its nearest variant) is now part of the document and the inlay
     * hint itself is now obsolete.
     */
    std::optional<std::vector<std::unique_ptr<TextEdit>>> textEdits;
    /**
     * The tooltip text when you hover over this item.
     */
    std::optional<InlayHint_tooltip> tooltip;
    /**
     * Render padding before the hint.
     *
     * Note: Padding should use the editor's background color, not the
     * background color of the hint itself. That means padding can be used
     * to visually align/separate an inlay hint.
     */
    std::optional<boolean_t> paddingLeft;
    /**
     * Render padding after the hint.
     *
     * Note: Padding should use the editor's background color, not the
     * background color of the hint itself. That means padding can be used
     * to visually align/separate an inlay hint.
     */
    std::optional<boolean_t> paddingRight;
    /**
     * A data entry field that is preserved on an inlay hint between
     * a `textDocument/inlayHint` and a `inlayHint/resolve` request.
     */
    std::optional<std::unique_ptr<LSPAny>> data;
  };

  /**
   * Inlay hint options used during static registration.
   *
   * @since 3.17.0
   */
  struct InlayHintOptions
  {
    std::optional<boolean_t> workDoneProgress;
    /**
     * The server provides support to resolve additional
     * information for an inlay hint item.
     */
    std::optional<boolean_t> resolveProvider;
  };

  /**
   * A diagnostic report indicating that the last returned
   * report is still accurate.
   *
   * @since 3.17.0
   */
  struct UnchangedDocumentDiagnosticReport
  {
    /**
     * A document diagnostic report indicating
     * no changes to the last result. A server can
     * only return `unchanged` if result ids are
     * provided.
     */
    string_t kind;
    /**
     * A result id which will be sent on the next
     * diagnostic request for the same document.
     */
    string_t resultId;
  };

  /**
   * Diagnostic options.
   *
   * @since 3.17.0
   */
  struct DiagnosticOptions
  {
    std::optional<boolean_t> workDoneProgress;
    /**
     * An optional identifier under which the diagnostics are
     * managed by the client.
     */
    std::optional<string_t> identifier;
    /**
     * Whether the language has inter file dependencies meaning that
     * editing code in one file can result in a different diagnostic
     * set in another file. Inter file dependencies are common for
     * most programming languages and typically uncommon for linters.
     */
    boolean_t interFileDependencies;
    /**
     * The server provides support for workspace diagnostics as well.
     */
    boolean_t workspaceDiagnostics;
  };

  /**
   * A previous result id in a workspace pull request.
   *
   * @since 3.17.0
   */
  struct PreviousResultId
  {
    /**
     * The URI for which the client knowns a
     * result id.
     */
    DocumentUri uri;
    /**
     * The value of the previous result id.
     */
    string_t value;
  };

  /**
   * An item to transfer a text document from the client to the
   * server.
   */
  struct TextDocumentItem
  {
    /**
     * The text document's uri.
     */
    DocumentUri uri;
    /**
     * The text document's language identifier.
     */
    string_t languageId;
    /**
     * The version number of this document (it will increase after each
     * change, including undo/redo).
     */
    integer_t version;
    /**
     * The content of the opened text document.
     */
    string_t text;
  };

  /**
   * The parameters sent in an open text document notification
   */
  struct DidOpenTextDocumentParams
  {
    /**
     * The document that was opened.
     */
    std::unique_ptr<TextDocumentItem> textDocument;
  };

  /**
   * A versioned notebook document identifier.
   *
   * @since 3.17.0
   */
  struct VersionedNotebookDocumentIdentifier
  {
    /**
     * The version number of this notebook document.
     */
    integer_t version;
    /**
     * The notebook document's uri.
     */
    URI uri;
  };

  /**
   * A literal to identify a notebook document in the client.
   *
   * @since 3.17.0
   */
  struct NotebookDocumentIdentifier
  {
    /**
     * The notebook document's uri.
     */
    URI uri;
  };

  /**
   * The params sent in a save notebook document notification.
   *
   * @since 3.17.0
   */
  struct DidSaveNotebookDocumentParams
  {
    /**
     * The notebook document that got saved.
     */
    std::unique_ptr<NotebookDocumentIdentifier> notebookDocument;
  };

  /**
   * The params sent in a close notebook document notification.
   *
   * @since 3.17.0
   */
  struct DidCloseNotebookDocumentParams
  {
    /**
     * The notebook document that got closed.
     */
    std::unique_ptr<NotebookDocumentIdentifier> notebookDocument;
    /**
     * The text documents that represent the content
     * of a notebook cell that got closed.
     */
    std::vector<std::unique_ptr<TextDocumentIdentifier>> cellTextDocuments;
  };

  /**
   * A string value used as a snippet is a template which allows to insert text
   * and to control the editor cursor when insertion happens.
   *
   * A snippet can define tab stops and placeholders with `$1`, `$2`
   * and `${3:foo}`. `$0` defines the final tab stop, it defaults to
   * the end of the snippet. Variables are defined with `$name` and
   * `${name:default value}`.
   *
   * @since 3.18.0
   * @proposed
   */
  struct StringValue
  {
    /**
     * The kind of string value.
     */
    string_t kind;
    /**
     * The snippet string.
     */
    string_t value;
  };

  enum class InlineCompletionItem_insertTextType {
    STRING_TYPE,
    STRING_VALUE,
  };

  extern std::map<InlineCompletionItem_insertTextType, std::string> InlineCompletionItem_insertTextTypeNames;

  typedef std::variant<
    string_t,
    std::unique_ptr<StringValue>
  > InlineCompletionItem_insertText;

  /**
   * An inline completion item represents a text snippet that is proposed inline to complete text that is being typed.
   *
   * @since 3.18.0
   * @proposed
   */
  struct InlineCompletionItem
  {
    /**
     * The text to replace the range with. Must be set.
     */
    InlineCompletionItem_insertText insertText;
    /**
     * A text that is used to decide if this inline completion should be shown. When `falsy` the {@link InlineCompletionItem.insertText} is used.
     */
    std::optional<string_t> filterText;
    /**
     * The range to replace. Must begin and end on the same line.
     */
    std::optional<std::unique_ptr<Range>> range;
    /**
     * An optional {@link Command} that is executed *after* inserting this completion.
     */
    std::optional<std::unique_ptr<Command>> command;
  };

  /**
   * Represents a collection of {@link InlineCompletionItem inline completion items} to be presented in the editor.
   *
   * @since 3.18.0
   * @proposed
   */
  struct InlineCompletionList
  {
    /**
     * The inline completion items
     */
    std::vector<std::unique_ptr<InlineCompletionItem>> items;
  };

  /**
   * Inline completion options used during static registration.
   *
   * @since 3.18.0
   * @proposed
   */
  struct InlineCompletionOptions
  {
    std::optional<boolean_t> workDoneProgress;
  };

  /**
   * General parameters to register for a notification or to register a provider.
   */
  struct Registration
  {
    /**
     * The id used to register the request. The id can be used to deregister
     * the request again.
     */
    string_t id;
    /**
     * The method / capability to register for.
     */
    string_t method;
    /**
     * Options necessary for the registration.
     */
    std::optional<std::unique_ptr<LSPAny>> registerOptions;
  };

  struct RegistrationParams
  {
    std::vector<std::unique_ptr<Registration>> registrations;
  };

  /**
   * General parameters to unregister a request or notification.
   */
  struct Unregistration
  {
    /**
     * The id used to unregister the request or notification. Usually an id
     * provided during the register request.
     */
    string_t id;
    /**
     * The method to unregister for.
     */
    string_t method;
  };

  struct UnregistrationParams
  {
    std::vector<std::unique_ptr<Unregistration>> unregisterations;
  };

  enum class WorkspaceFoldersInitializeParams_workspaceFoldersType {
    WORKSPACE_FOLDER_ARRAY,
    NULL_TYPE,
  };

  extern std::map<WorkspaceFoldersInitializeParams_workspaceFoldersType, std::string> WorkspaceFoldersInitializeParams_workspaceFoldersTypeNames;

  typedef std::variant<
    std::vector<std::unique_ptr<WorkspaceFolder>>,
    null_t
  > WorkspaceFoldersInitializeParams_workspaceFolders;

  struct WorkspaceFoldersInitializeParams
  {
    /**
     * The workspace folders configured in the client when the server starts.
     *
     * This property is only available if the client supports workspace folders.
     * It can be `null` if the client supports workspace folders but none are
     * configured.
     *
     * @since 3.6.0
     */
    std::optional<WorkspaceFoldersInitializeParams_workspaceFolders> workspaceFolders;
  };

  /**
   * A text document identifier to denote a specific version of a text document.
   */
  struct VersionedTextDocumentIdentifier
    : public TextDocumentIdentifier
  {
    /**
     * The version number of this document.
     */
    integer_t version;
  };

  /**
   * Save options.
   */
  struct SaveOptions
  {
    /**
     * The client is supposed to include the content on save.
     */
    std::optional<boolean_t> includeText;
  };

  /**
   * An event describing a file change.
   */
  struct FileEvent
  {
    /**
     * The file's uri.
     */
    DocumentUri uri;
    /**
     * The change type.
     */
    FileChangeType type;
  };

  /**
   * The watched files change notification's parameters.
   */
  struct DidChangeWatchedFilesParams
  {
    /**
     * The actual file events.
     */
    std::vector<std::unique_ptr<FileEvent>> changes;
  };

  /**
   * Contains additional information about the context in which a completion request is triggered.
   */
  struct CompletionContext
  {
    /**
     * How the completion was triggered.
     */
    CompletionTriggerKind triggerKind;
    /**
     * The trigger character (a single character) that has trigger code complete.
     * Is undefined if `triggerKind !== CompletionTriggerKind.TriggerCharacter`
     */
    std::optional<string_t> triggerCharacter;
  };

  /**
   * Additional details for a completion item label.
   *
   * @since 3.17.0
   */
  struct CompletionItemLabelDetails
  {
    /**
     * An optional string which is rendered less prominently directly after {@link CompletionItem.label label},
     * without any spacing. Should be used for function signatures and type annotations.
     */
    std::optional<string_t> detail;
    /**
     * An optional string which is rendered less prominently after {@link CompletionItem.detail}. Should be used
     * for fully qualified names and file paths.
     */
    std::optional<string_t> description;
  };

  /**
   * A special text edit to provide an insert and a replace operation.
   *
   * @since 3.16.0
   */
  struct InsertReplaceEdit
  {
    /**
     * The string to be inserted.
     */
    string_t newText;
    /**
     * The range if the insert is requested
     */
    std::unique_ptr<Range> insert;
    /**
     * The range if the replace is requested.
     */
    std::unique_ptr<Range> replace;
  };

  enum class CompletionItem_documentationType {
    STRING_TYPE,
    MARKUP_CONTENT,
  };

  extern std::map<CompletionItem_documentationType, std::string> CompletionItem_documentationTypeNames;

  typedef std::variant<
    string_t,
    std::unique_ptr<MarkupContent>
  > CompletionItem_documentation;

  enum class CompletionItem_textEditType {
    TEXT_EDIT,
    INSERT_REPLACE_EDIT,
  };

  extern std::map<CompletionItem_textEditType, std::string> CompletionItem_textEditTypeNames;

  typedef std::variant<
    std::unique_ptr<TextEdit>,
    std::unique_ptr<InsertReplaceEdit>
  > CompletionItem_textEdit;

  /**
   * A completion item represents a text snippet that is
   * proposed to complete text that is being typed.
   */
  struct CompletionItem
  {
    /**
     * The label of this completion item.
     *
     * The label property is also by default the text that
     * is inserted when selecting this completion.
     *
     * If label details are provided the label itself should
     * be an unqualified name of the completion item.
     */
    string_t label;
    /**
     * Additional details for the label
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<CompletionItemLabelDetails>> labelDetails;
    /**
     * The kind of this completion item. Based of the kind
     * an icon is chosen by the editor.
     */
    std::optional<CompletionItemKind> kind;
    /**
     * Tags for this completion item.
     *
     * @since 3.15.0
     */
    std::optional<std::vector<CompletionItemTag>> tags;
    /**
     * A human-readable string with additional information
     * about this item, like type or symbol information.
     */
    std::optional<string_t> detail;
    /**
     * A human-readable string that represents a doc-comment.
     */
    std::optional<CompletionItem_documentation> documentation;
    /**
     * Indicates if this item is deprecated.
     * @deprecated Use `tags` instead.
     */
    std::optional<boolean_t> deprecated;
    /**
     * Select this item when showing.
     *
     * *Note* that only one completion item can be selected and that the
     * tool / client decides which item that is. The rule is that the *first*
     * item of those that match best is selected.
     */
    std::optional<boolean_t> preselect;
    /**
     * A string that should be used when comparing this item
     * with other items. When `falsy` the {@link CompletionItem.label label}
     * is used.
     */
    std::optional<string_t> sortText;
    /**
     * A string that should be used when filtering a set of
     * completion items. When `falsy` the {@link CompletionItem.label label}
     * is used.
     */
    std::optional<string_t> filterText;
    /**
     * A string that should be inserted into a document when selecting
     * this completion. When `falsy` the {@link CompletionItem.label label}
     * is used.
     *
     * The `insertText` is subject to interpretation by the client side.
     * Some tools might not take the string literally. For example
     * VS Code when code complete is requested in this example
     * `con<cursor position>` and a completion item with an `insertText` of
     * `console` is provided it will only insert `sole`. Therefore it is
     * recommended to use `textEdit` instead since it avoids additional client
     * side interpretation.
     */
    std::optional<string_t> insertText;
    /**
     * The format of the insert text. The format applies to both the
     * `insertText` property and the `newText` property of a provided
     * `textEdit`. If omitted defaults to `InsertTextFormat.PlainText`.
     *
     * Please note that the insertTextFormat doesn't apply to
     * `additionalTextEdits`.
     */
    std::optional<InsertTextFormat> insertTextFormat;
    /**
     * How whitespace and indentation is handled during completion
     * item insertion. If not provided the clients default value depends on
     * the `textDocument.completion.insertTextMode` client capability.
     *
     * @since 3.16.0
     */
    std::optional<InsertTextMode> insertTextMode;
    /**
     * An {@link TextEdit edit} which is applied to a document when selecting
     * this completion. When an edit is provided the value of
     * {@link CompletionItem.insertText insertText} is ignored.
     *
     * Most editors support two different operations when accepting a completion
     * item. One is to insert a completion text and the other is to replace an
     * existing text with a completion text. Since this can usually not be
     * predetermined by a server it can report both ranges. Clients need to
     * signal support for `InsertReplaceEdits` via the
     * `textDocument.completion.insertReplaceSupport` client capability
     * property.
     *
     * *Note 1:* The text edit's range as well as both ranges from an insert
     * replace edit must be a [single line] and they must contain the position
     * at which completion has been requested.
     * *Note 2:* If an `InsertReplaceEdit` is returned the edit's insert range
     * must be a prefix of the edit's replace range, that means it must be
     * contained and starting at the same position.
     *
     * @since 3.16.0 additional type `InsertReplaceEdit`
     */
    std::optional<CompletionItem_textEdit> textEdit;
    /**
     * The edit text used if the completion item is part of a CompletionList and
     * CompletionList defines an item default for the text edit range.
     *
     * Clients will only honor this property if they opt into completion list
     * item defaults using the capability `completionList.itemDefaults`.
     *
     * If not provided and a list's default range is provided the label
     * property is used as a text.
     *
     * @since 3.17.0
     */
    std::optional<string_t> textEditText;
    /**
     * An optional array of additional {@link TextEdit text edits} that are applied when
     * selecting this completion. Edits must not overlap (including the same insert position)
     * with the main {@link CompletionItem.textEdit edit} nor with themselves.
     *
     * Additional text edits should be used to change text unrelated to the current cursor position
     * (for example adding an import statement at the top of the file if the completion item will
     * insert an unqualified type).
     */
    std::optional<std::vector<std::unique_ptr<TextEdit>>> additionalTextEdits;
    /**
     * An optional set of characters that when pressed while this completion is active will accept it first and
     * then type that character. *Note* that all commit characters should have `length=1` and that superfluous
     * characters will be ignored.
     */
    std::optional<std::vector<string_t>> commitCharacters;
    /**
     * An optional {@link Command command} that is executed *after* inserting this completion. *Note* that
     * additional modifications to the current document should be described with the
     * {@link CompletionItem.additionalTextEdits additionalTextEdits}-property.
     */
    std::optional<std::unique_ptr<Command>> command;
    /**
     * A data entry field that is preserved on a completion item between a
     * {@link CompletionRequest} and a {@link CompletionResolveRequest}.
     */
    std::optional<std::unique_ptr<LSPAny>> data;
  };

  struct CompletionList_itemDefaults_editRange_1
  {
    std::unique_ptr<Range> insert;
    std::unique_ptr<Range> replace;
  };

  enum class CompletionList_itemDefaults_editRangeType {
    RANGE,
    COMPLETION_LIST_ITEM_DEFAULTS_EDIT_RANGE_1,
  };

  extern std::map<CompletionList_itemDefaults_editRangeType, std::string> CompletionList_itemDefaults_editRangeTypeNames;

  typedef std::variant<
    std::unique_ptr<Range>,
    std::unique_ptr<CompletionList_itemDefaults_editRange_1>
  > CompletionList_itemDefaults_editRange;

  struct CompletionList_itemDefaults
  {
    std::optional<std::vector<string_t>> commitCharacters;
    std::optional<CompletionList_itemDefaults_editRange> editRange;
    std::optional<InsertTextFormat> insertTextFormat;
    std::optional<InsertTextMode> insertTextMode;
    std::optional<std::unique_ptr<LSPAny>> data;
  };

  /**
   * Represents a collection of {@link CompletionItem completion items} to be presented
   * in the editor.
   */
  struct CompletionList
  {
    /**
     * This list it not complete. Further typing results in recomputing this list.
     *
     * Recomputed lists have all their items replaced (not appended) in the
     * incomplete completion sessions.
     */
    boolean_t isIncomplete;
    /**
     * In many cases the items of an actual completion result share the same
     * value for properties like `commitCharacters` or the range of a text
     * edit. A completion list can therefore define item defaults which will
     * be used if a completion item itself doesn't specify the value.
     *
     * If a completion list specifies a default value and a completion item
     * also specifies a corresponding value the one from the item is used.
     *
     * Servers are only allowed to return default values if the client
     * signals support for this via the `completionList.itemDefaults`
     * capability.
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<CompletionList_itemDefaults>> itemDefaults;
    /**
     * The completion items.
     */
    std::vector<std::unique_ptr<CompletionItem>> items;
  };

  struct CompletionOptions_completionItem
  {
    std::optional<boolean_t> labelDetailsSupport;
  };

  /**
   * Completion options.
   */
  struct CompletionOptions
  {
    std::optional<boolean_t> workDoneProgress;
    /**
     * Most tools trigger completion request automatically without explicitly requesting
     * it using a keyboard shortcut (e.g. Ctrl+Space). Typically they do so when the user
     * starts to type an identifier. For example if the user types `c` in a JavaScript file
     * code complete will automatically pop up present `console` besides others as a
     * completion item. Characters that make up identifiers don't need to be listed here.
     *
     * If code complete should automatically be trigger on characters not being valid inside
     * an identifier (for example `.` in JavaScript) list them in `triggerCharacters`.
     */
    std::optional<std::vector<string_t>> triggerCharacters;
    /**
     * The list of all possible characters that commit a completion. This field can be used
     * if clients don't support individual commit characters per completion item. See
     * `ClientCapabilities.textDocument.completion.completionItem.commitCharactersSupport`
     *
     * If a server provides both `allCommitCharacters` and commit characters on an individual
     * completion item the ones on the completion item win.
     *
     * @since 3.2.0
     */
    std::optional<std::vector<string_t>> allCommitCharacters;
    /**
     * The server provides support to resolve additional
     * information for a completion item.
     */
    std::optional<boolean_t> resolveProvider;
    /**
     * The server supports the following `CompletionItem` specific
     * capabilities.
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<CompletionOptions_completionItem>> completionItem;
  };

  /**
   * Hover options.
   */
  struct HoverOptions
  {
    std::optional<boolean_t> workDoneProgress;
  };

  /**
   * Server Capabilities for a {@link SignatureHelpRequest}.
   */
  struct SignatureHelpOptions
  {
    std::optional<boolean_t> workDoneProgress;
    /**
     * List of characters that trigger signature help automatically.
     */
    std::optional<std::vector<string_t>> triggerCharacters;
    /**
     * List of characters that re-trigger signature help.
     *
     * These trigger characters are only active when signature help is already showing. All trigger characters
     * are also counted as re-trigger characters.
     *
     * @since 3.15.0
     */
    std::optional<std::vector<string_t>> retriggerCharacters;
  };

  /**
   * Server Capabilities for a {@link DefinitionRequest}.
   */
  struct DefinitionOptions
  {
    std::optional<boolean_t> workDoneProgress;
  };

  /**
   * Value-object that contains additional information when
   * requesting references.
   */
  struct ReferenceContext
  {
    /**
     * Include the declaration of the current symbol.
     */
    boolean_t includeDeclaration;
  };

  /**
   * Reference options.
   */
  struct ReferenceOptions
  {
    std::optional<boolean_t> workDoneProgress;
  };

  /**
   * Provider options for a {@link DocumentHighlightRequest}.
   */
  struct DocumentHighlightOptions
  {
    std::optional<boolean_t> workDoneProgress;
  };

  /**
   * A base for all symbol information.
   */
  struct BaseSymbolInformation
  {
    /**
     * The name of this symbol.
     */
    string_t name;
    /**
     * The kind of this symbol.
     */
    SymbolKind kind;
    /**
     * Tags for this symbol.
     *
     * @since 3.16.0
     */
    std::optional<std::vector<SymbolTag>> tags;
    /**
     * The name of the symbol containing this symbol. This information is for
     * user interface purposes (e.g. to render a qualifier in the user interface
     * if necessary). It can't be used to re-infer a hierarchy for the document
     * symbols.
     */
    std::optional<string_t> containerName;
  };

  struct WorkspaceSymbol_location_1
  {
    DocumentUri uri;
  };

  enum class WorkspaceSymbol_locationType {
    LOCATION,
    WORKSPACE_SYMBOL_LOCATION_1,
  };

  extern std::map<WorkspaceSymbol_locationType, std::string> WorkspaceSymbol_locationTypeNames;

  typedef std::variant<
    std::unique_ptr<Location>,
    std::unique_ptr<WorkspaceSymbol_location_1>
  > WorkspaceSymbol_location;

  /**
   * A special workspace symbol that supports locations without a range.
   *
   * See also SymbolInformation.
   *
   * @since 3.17.0
   */
  struct WorkspaceSymbol
    : public BaseSymbolInformation
  {
    /**
     * The location of the symbol. Whether a server is allowed to
     * return a location without a range depends on the client
     * capability `workspace.symbol.resolveSupport`.
     *
     * See SymbolInformation#location for more details.
     */
    WorkspaceSymbol_location location;
    /**
     * A data entry field that is preserved on a workspace symbol between a
     * workspace symbol request and a workspace symbol resolve request.
     */
    std::optional<std::unique_ptr<LSPAny>> data;
  };

  /**
   * Represents information about programming constructs like variables, classes,
   * interfaces etc.
   */
  struct SymbolInformation
    : public BaseSymbolInformation
  {
    /**
     * Indicates if this symbol is deprecated.
     *
     * @deprecated Use tags instead
     */
    std::optional<boolean_t> deprecated;
    /**
     * The location of this symbol. The location's range is used by a tool
     * to reveal the location in the editor. If the symbol is selected in the
     * tool the range's start information is used to position the cursor. So
     * the range usually spans more than the actual symbol's name and does
     * normally include things like visibility modifiers.
     *
     * The range doesn't have to denote a node range in the sense of an abstract
     * syntax tree. It can therefore not be used to re-construct a hierarchy of
     * the symbols.
     */
    std::unique_ptr<Location> location;
  };

  /**
   * Provider options for a {@link DocumentSymbolRequest}.
   */
  struct DocumentSymbolOptions
  {
    std::optional<boolean_t> workDoneProgress;
    /**
     * A human-readable string that is shown when multiple outlines trees
     * are shown for the same document.
     *
     * @since 3.16.0
     */
    std::optional<string_t> label;
  };

  /**
   * Provider options for a {@link CodeActionRequest}.
   */
  struct CodeActionOptions
  {
    std::optional<boolean_t> workDoneProgress;
    /**
     * CodeActionKinds that this server may return.
     *
     * The list of kinds may be generic, such as `CodeActionKind.Refactor`, or the server
     * may list out every specific kind they provide.
     */
    std::optional<std::vector<CodeActionKind>> codeActionKinds;
    /**
     * The server provides support to resolve additional
     * information for a code action.
     *
     * @since 3.16.0
     */
    std::optional<boolean_t> resolveProvider;
  };

  /**
   * Server capabilities for a {@link WorkspaceSymbolRequest}.
   */
  struct WorkspaceSymbolOptions
  {
    std::optional<boolean_t> workDoneProgress;
    /**
     * The server provides support to resolve additional
     * information for a workspace symbol.
     *
     * @since 3.17.0
     */
    std::optional<boolean_t> resolveProvider;
  };

  /**
   * Registration options for a {@link WorkspaceSymbolRequest}.
   */
  struct WorkspaceSymbolRegistrationOptions
    : public WorkspaceSymbolOptions
  {
  };

  /**
   * Code Lens provider options of a {@link CodeLensRequest}.
   */
  struct CodeLensOptions
  {
    std::optional<boolean_t> workDoneProgress;
    /**
     * Code lens has a resolve provider as well.
     */
    std::optional<boolean_t> resolveProvider;
  };

  /**
   * Provider options for a {@link DocumentLinkRequest}.
   */
  struct DocumentLinkOptions
  {
    std::optional<boolean_t> workDoneProgress;
    /**
     * Document links have a resolve provider as well.
     */
    std::optional<boolean_t> resolveProvider;
  };

  /**
   * Value-object describing what options formatting should use.
   */
  struct FormattingOptions
  {
    /**
     * Size of a tab in spaces.
     */
    uinteger_t tabSize;
    /**
     * Prefer spaces over tabs.
     */
    boolean_t insertSpaces;
    /**
     * Trim trailing whitespace on a line.
     *
     * @since 3.15.0
     */
    std::optional<boolean_t> trimTrailingWhitespace;
    /**
     * Insert a newline character at the end of the file if one does not exist.
     *
     * @since 3.15.0
     */
    std::optional<boolean_t> insertFinalNewline;
    /**
     * Trim all newlines after the final newline at the end of the file.
     *
     * @since 3.15.0
     */
    std::optional<boolean_t> trimFinalNewlines;
  };

  /**
   * The parameters of a {@link DocumentOnTypeFormattingRequest}.
   */
  struct DocumentOnTypeFormattingParams
  {
    /**
     * The document to format.
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
    /**
     * The position around which the on type formatting should happen.
     * This is not necessarily the exact position where the character denoted
     * by the property `ch` got typed.
     */
    std::unique_ptr<Position> position;
    /**
     * The character that has been typed that triggered the formatting
     * on type request. That is not necessarily the last character that
     * got inserted into the document since the client could auto insert
     * characters as well (e.g. like automatic brace completion).
     */
    string_t ch;
    /**
     * The formatting options.
     */
    std::unique_ptr<FormattingOptions> options;
  };

  /**
   * Provider options for a {@link DocumentFormattingRequest}.
   */
  struct DocumentFormattingOptions
  {
    std::optional<boolean_t> workDoneProgress;
  };

  /**
   * Provider options for a {@link DocumentRangeFormattingRequest}.
   */
  struct DocumentRangeFormattingOptions
  {
    std::optional<boolean_t> workDoneProgress;
    /**
     * Whether the server supports formatting multiple ranges at once.
     *
     * @since 3.18.0
     * @proposed
     */
    std::optional<boolean_t> rangesSupport;
  };

  /**
   * Provider options for a {@link DocumentOnTypeFormattingRequest}.
   */
  struct DocumentOnTypeFormattingOptions
  {
    /**
     * A character on which formatting should be triggered, like `{`.
     */
    string_t firstTriggerCharacter;
    /**
     * More trigger characters.
     */
    std::optional<std::vector<string_t>> moreTriggerCharacter;
  };

  /**
   * Provider options for a {@link RenameRequest}.
   */
  struct RenameOptions
  {
    std::optional<boolean_t> workDoneProgress;
    /**
     * Renames should be checked and tested before being executed.
     *
     * @since version 3.12.0
     */
    std::optional<boolean_t> prepareProvider;
  };

  /**
   * The server capabilities of a {@link ExecuteCommandRequest}.
   */
  struct ExecuteCommandOptions
  {
    std::optional<boolean_t> workDoneProgress;
    /**
     * The commands to be executed on the server
     */
    std::vector<string_t> commands;
  };

  /**
   * Registration options for a {@link ExecuteCommandRequest}.
   */
  struct ExecuteCommandRegistrationOptions
    : public ExecuteCommandOptions
  {
  };

  /**
   * @since 3.16.0
   */
  struct SemanticTokensLegend
  {
    /**
     * The token types a server uses.
     */
    std::vector<string_t> tokenTypes;
    /**
     * The token modifiers a server uses.
     */
    std::vector<string_t> tokenModifiers;
  };

  struct SemanticTokensOptions_range_1
  {
  };

  enum class SemanticTokensOptions_rangeType {
    BOOLEAN_TYPE,
    SEMANTIC_TOKENS_OPTIONS_RANGE_1,
  };

  extern std::map<SemanticTokensOptions_rangeType, std::string> SemanticTokensOptions_rangeTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<SemanticTokensOptions_range_1>
  > SemanticTokensOptions_range;

  struct SemanticTokensOptions_full_1
  {
    std::optional<boolean_t> delta;
  };

  enum class SemanticTokensOptions_fullType {
    BOOLEAN_TYPE,
    SEMANTIC_TOKENS_OPTIONS_FULL_1,
  };

  extern std::map<SemanticTokensOptions_fullType, std::string> SemanticTokensOptions_fullTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<SemanticTokensOptions_full_1>
  > SemanticTokensOptions_full;

  /**
   * @since 3.16.0
   */
  struct SemanticTokensOptions
  {
    std::optional<boolean_t> workDoneProgress;
    /**
     * The legend used by the server
     */
    std::unique_ptr<SemanticTokensLegend> legend;
    /**
     * Server supports providing semantic tokens for a specific range
     * of a document.
     */
    std::optional<SemanticTokensOptions_range> range;
    /**
     * Server supports providing semantic tokens for a full document.
     */
    std::optional<SemanticTokensOptions_full> full;
  };

  enum class OptionalVersionedTextDocumentIdentifier_versionType {
    INTEGER_TYPE,
    NULL_TYPE,
  };

  extern std::map<OptionalVersionedTextDocumentIdentifier_versionType, std::string> OptionalVersionedTextDocumentIdentifier_versionTypeNames;

  typedef std::variant<
    integer_t,
    null_t
  > OptionalVersionedTextDocumentIdentifier_version;

  /**
   * A text document identifier to optionally denote a specific version of a text document.
   */
  struct OptionalVersionedTextDocumentIdentifier
    : public TextDocumentIdentifier
  {
    /**
     * The version number of this document. If a versioned text document identifier
     * is sent from the server to the client and the file is not open in the editor
     * (the server has not received an open notification before) the server can send
     * `null` to indicate that the version is unknown and the content on disk is the
     * truth (as specified with document content ownership).
     */
    OptionalVersionedTextDocumentIdentifier_version version;
  };

  /**
   * Options to create a file.
   */
  struct CreateFileOptions
  {
    /**
     * Overwrite existing file. Overwrite wins over `ignoreIfExists`
     */
    std::optional<boolean_t> overwrite;
    /**
     * Ignore if exists.
     */
    std::optional<boolean_t> ignoreIfExists;
  };

  /**
   * Rename file options
   */
  struct RenameFileOptions
  {
    /**
     * Overwrite target if existing. Overwrite wins over `ignoreIfExists`
     */
    std::optional<boolean_t> overwrite;
    /**
     * Ignores if target exists.
     */
    std::optional<boolean_t> ignoreIfExists;
  };

  /**
   * Delete file options
   */
  struct DeleteFileOptions
  {
    /**
     * Delete the content recursively if a folder is denoted.
     */
    std::optional<boolean_t> recursive;
    /**
     * Ignore the operation if the file doesn't exist.
     */
    std::optional<boolean_t> ignoreIfNotExists;
  };

  enum class WorkspaceUnchangedDocumentDiagnosticReport_versionType {
    INTEGER_TYPE,
    NULL_TYPE,
  };

  extern std::map<WorkspaceUnchangedDocumentDiagnosticReport_versionType, std::string> WorkspaceUnchangedDocumentDiagnosticReport_versionTypeNames;

  typedef std::variant<
    integer_t,
    null_t
  > WorkspaceUnchangedDocumentDiagnosticReport_version;

  /**
   * An unchanged document diagnostic report for a workspace diagnostic result.
   *
   * @since 3.17.0
   */
  struct WorkspaceUnchangedDocumentDiagnosticReport
    : public UnchangedDocumentDiagnosticReport
  {
    /**
     * The URI for which diagnostic information is reported.
     */
    DocumentUri uri;
    /**
     * The version number for which the diagnostics are reported.
     * If the document is not marked as open `null` can be provided.
     */
    WorkspaceUnchangedDocumentDiagnosticReport_version version;
  };

  /**
   * Describes the currently selected completion item.
   *
   * @since 3.18.0
   * @proposed
   */
  struct SelectedCompletionInfo
  {
    /**
     * The range that will be replaced if this completion item is accepted.
     */
    std::unique_ptr<Range> range;
    /**
     * The text the range will be replaced with if this completion is accepted.
     */
    string_t text;
  };

  /**
   * Provides information about the context in which an inline completion was requested.
   *
   * @since 3.18.0
   * @proposed
   */
  struct InlineCompletionContext
  {
    /**
     * Describes how the inline completion was triggered.
     */
    InlineCompletionTriggerKind triggerKind;
    /**
     * Provides information about the currently selected item in the autocomplete widget if it is visible.
     */
    std::optional<std::unique_ptr<SelectedCompletionInfo>> selectedCompletionInfo;
  };

  enum class TextDocumentSyncOptions_saveType {
    BOOLEAN_TYPE,
    SAVE_OPTIONS,
  };

  extern std::map<TextDocumentSyncOptions_saveType, std::string> TextDocumentSyncOptions_saveTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<SaveOptions>
  > TextDocumentSyncOptions_save;

  struct TextDocumentSyncOptions
  {
    /**
     * Open and close notifications are sent to the server. If omitted open close notification should not
     * be sent.
     */
    std::optional<boolean_t> openClose;
    /**
     * Change notifications are sent to the server. See TextDocumentSyncKind.None, TextDocumentSyncKind.Full
     * and TextDocumentSyncKind.Incremental. If omitted it defaults to TextDocumentSyncKind.None.
     */
    std::optional<TextDocumentSyncKind> change;
    /**
     * If present will save notifications are sent to the server. If omitted the notification should not be
     * sent.
     */
    std::optional<boolean_t> willSave;
    /**
     * If present will save wait until requests are sent to the server. If omitted the request should not be
     * sent.
     */
    std::optional<boolean_t> willSaveWaitUntil;
    /**
     * If present save notifications are sent to the server. If omitted the notification should not be
     * sent.
     */
    std::optional<TextDocumentSyncOptions_save> save;
  };

  enum class WorkspaceFoldersServerCapabilities_changeNotificationsType {
    STRING_TYPE,
    BOOLEAN_TYPE,
  };

  extern std::map<WorkspaceFoldersServerCapabilities_changeNotificationsType, std::string> WorkspaceFoldersServerCapabilities_changeNotificationsTypeNames;

  typedef std::variant<
    string_t,
    boolean_t
  > WorkspaceFoldersServerCapabilities_changeNotifications;

  struct WorkspaceFoldersServerCapabilities
  {
    /**
     * The server has support for workspace folders
     */
    std::optional<boolean_t> supported;
    /**
     * Whether the server wants to receive workspace folder
     * change notifications.
     *
     * If a string is provided the string is treated as an ID
     * under which the notification is registered on the client
     * side. The ID can be used to unregister for these events
     * using the `client/unregisterCapability` request.
     */
    std::optional<WorkspaceFoldersServerCapabilities_changeNotifications> changeNotifications;
  };

  /**
   * Structure to capture a description for an error code.
   *
   * @since 3.16.0
   */
  struct CodeDescription
  {
    /**
     * An URI to open with more information about the diagnostic error.
     */
    URI href;
  };

  /**
   * Represents a related message and source code location for a diagnostic. This should be
   * used to point to code locations that cause or related to a diagnostics, e.g when duplicating
   * a symbol in a scope.
   */
  struct DiagnosticRelatedInformation
  {
    /**
     * The location of this related diagnostic information.
     */
    std::unique_ptr<Location> location;
    /**
     * The message of this related diagnostic information.
     */
    string_t message;
  };

  enum class Diagnostic_codeType {
    INTEGER_TYPE,
    STRING_TYPE,
  };

  extern std::map<Diagnostic_codeType, std::string> Diagnostic_codeTypeNames;

  typedef std::variant<
    integer_t,
    string_t
  > Diagnostic_code;

  /**
   * Represents a diagnostic, such as a compiler error or warning. Diagnostic objects
   * are only valid in the scope of a resource.
   */
  struct Diagnostic
  {
    /**
     * The range at which the message applies
     */
    std::unique_ptr<Range> range;
    /**
     * The diagnostic's severity. Can be omitted. If omitted it is up to the
     * client to interpret diagnostics as error, warning, info or hint.
     */
    std::optional<DiagnosticSeverity> severity;
    /**
     * The diagnostic's code, which usually appear in the user interface.
     */
    std::optional<Diagnostic_code> code;
    /**
     * An optional property to describe the error code.
     * Requires the code field (above) to be present/not null.
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<CodeDescription>> codeDescription;
    /**
     * A human-readable string describing the source of this
     * diagnostic, e.g. 'typescript' or 'super lint'. It usually
     * appears in the user interface.
     */
    std::optional<string_t> source;
    /**
     * The diagnostic's message. It usually appears in the user interface
     */
    string_t message;
    /**
     * Additional metadata about the diagnostic.
     *
     * @since 3.15.0
     */
    std::optional<std::vector<DiagnosticTag>> tags;
    /**
     * An array of related diagnostic information, e.g. when symbol-names within
     * a scope collide all definitions can be marked via this property.
     */
    std::optional<std::vector<std::unique_ptr<DiagnosticRelatedInformation>>> relatedInformation;
    /**
     * A data entry field that is preserved between a `textDocument/publishDiagnostics`
     * notification and `textDocument/codeAction` request.
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<LSPAny>> data;
  };

  /**
   * A diagnostic report with a full set of problems.
   *
   * @since 3.17.0
   */
  struct FullDocumentDiagnosticReport
  {
    /**
     * A full document diagnostic report.
     */
    string_t kind;
    /**
     * An optional result id. If provided it will
     * be sent on the next diagnostic request for the
     * same document.
     */
    std::optional<string_t> resultId;
    /**
     * The actual items.
     */
    std::vector<std::unique_ptr<Diagnostic>> items;
  };

  enum class RelatedFullDocumentDiagnosticReport_relatedDocumentsType {
    FULL_DOCUMENT_DIAGNOSTIC_REPORT,
    UNCHANGED_DOCUMENT_DIAGNOSTIC_REPORT,
  };

  extern std::map<RelatedFullDocumentDiagnosticReport_relatedDocumentsType, std::string> RelatedFullDocumentDiagnosticReport_relatedDocumentsTypeNames;

  typedef std::variant<
    std::unique_ptr<FullDocumentDiagnosticReport>,
    std::unique_ptr<UnchangedDocumentDiagnosticReport>
  > RelatedFullDocumentDiagnosticReport_relatedDocuments;

  /**
   * A full diagnostic report with a set of related documents.
   *
   * @since 3.17.0
   */
  struct RelatedFullDocumentDiagnosticReport
    : public FullDocumentDiagnosticReport
  {
    /**
     * Diagnostics of related documents. This information is useful
     * in programming languages where code in a file A can generate
     * diagnostics in a file B which A depends on. An example of
     * such a language is C/C++ where marco definitions in a file
     * a.cpp and result in errors in a header file b.hpp.
     *
     * @since 3.17.0
     */
    std::optional<std::map<DocumentUri, RelatedFullDocumentDiagnosticReport_relatedDocuments>> relatedDocuments;
  };

  enum class WorkspaceFullDocumentDiagnosticReport_versionType {
    INTEGER_TYPE,
    NULL_TYPE,
  };

  extern std::map<WorkspaceFullDocumentDiagnosticReport_versionType, std::string> WorkspaceFullDocumentDiagnosticReport_versionTypeNames;

  typedef std::variant<
    integer_t,
    null_t
  > WorkspaceFullDocumentDiagnosticReport_version;

  /**
   * A full document diagnostic report for a workspace diagnostic result.
   *
   * @since 3.17.0
   */
  struct WorkspaceFullDocumentDiagnosticReport
    : public FullDocumentDiagnosticReport
  {
    /**
     * The URI for which diagnostic information is reported.
     */
    DocumentUri uri;
    /**
     * The version number for which the diagnostics are reported.
     * If the document is not marked as open `null` can be provided.
     */
    WorkspaceFullDocumentDiagnosticReport_version version;
  };

  enum class RelatedUnchangedDocumentDiagnosticReport_relatedDocumentsType {
    FULL_DOCUMENT_DIAGNOSTIC_REPORT,
    UNCHANGED_DOCUMENT_DIAGNOSTIC_REPORT,
  };

  extern std::map<RelatedUnchangedDocumentDiagnosticReport_relatedDocumentsType, std::string> RelatedUnchangedDocumentDiagnosticReport_relatedDocumentsTypeNames;

  typedef std::variant<
    std::unique_ptr<FullDocumentDiagnosticReport>,
    std::unique_ptr<UnchangedDocumentDiagnosticReport>
  > RelatedUnchangedDocumentDiagnosticReport_relatedDocuments;

  /**
   * An unchanged diagnostic report with a set of related documents.
   *
   * @since 3.17.0
   */
  struct RelatedUnchangedDocumentDiagnosticReport
    : public UnchangedDocumentDiagnosticReport
  {
    /**
     * Diagnostics of related documents. This information is useful
     * in programming languages where code in a file A can generate
     * diagnostics in a file B which A depends on. An example of
     * such a language is C/C++ where marco definitions in a file
     * a.cpp and result in errors in a header file b.hpp.
     *
     * @since 3.17.0
     */
    std::optional<std::map<DocumentUri, RelatedUnchangedDocumentDiagnosticReport_relatedDocuments>> relatedDocuments;
  };

  enum class DocumentDiagnosticReportPartialResult_relatedDocumentsType {
    FULL_DOCUMENT_DIAGNOSTIC_REPORT,
    UNCHANGED_DOCUMENT_DIAGNOSTIC_REPORT,
  };

  extern std::map<DocumentDiagnosticReportPartialResult_relatedDocumentsType, std::string> DocumentDiagnosticReportPartialResult_relatedDocumentsTypeNames;

  typedef std::variant<
    std::unique_ptr<FullDocumentDiagnosticReport>,
    std::unique_ptr<UnchangedDocumentDiagnosticReport>
  > DocumentDiagnosticReportPartialResult_relatedDocuments;

  /**
   * A partial result for a document diagnostic report.
   *
   * @since 3.17.0
   */
  struct DocumentDiagnosticReportPartialResult
  {
    std::map<DocumentUri, DocumentDiagnosticReportPartialResult_relatedDocuments> relatedDocuments;
  };

  /**
   * Contains additional diagnostic information about the context in which
   * a {@link CodeActionProvider.provideCodeActions code action} is run.
   */
  struct CodeActionContext
  {
    /**
     * An array of diagnostics known on the client side overlapping the range provided to the
     * `textDocument/codeAction` request. They are provided so that the server knows which
     * errors are currently presented to the user for the given range. There is no guarantee
     * that these accurately reflect the error state of the resource. The primary parameter
     * to compute code actions is the provided range.
     */
    std::vector<std::unique_ptr<Diagnostic>> diagnostics;
    /**
     * Requested kind of actions to return.
     *
     * Actions not of this kind are filtered out by the client before being shown. So servers
     * can omit computing them.
     */
    std::optional<std::vector<CodeActionKind>> only;
    /**
     * The reason why code actions were requested.
     *
     * @since 3.17.0
     */
    std::optional<CodeActionTriggerKind> triggerKind;
  };

  /**
   * The publish diagnostic notification's parameters.
   */
  struct PublishDiagnosticsParams
  {
    /**
     * The URI for which diagnostic information is reported.
     */
    DocumentUri uri;
    /**
     * Optional the version number of the document the diagnostics are published for.
     *
     * @since 3.15.0
     */
    std::optional<integer_t> version;
    /**
     * An array of diagnostic information items.
     */
    std::vector<std::unique_ptr<Diagnostic>> diagnostics;
  };

  enum class ParameterInformation_labelType {
    STRING_TYPE,
    PAIR_OF_UINTEGER_TYPE_AND_UINTEGER_TYPE,
  };

  extern std::map<ParameterInformation_labelType, std::string> ParameterInformation_labelTypeNames;

  typedef std::variant<
    string_t,
    std::pair<uinteger_t, uinteger_t>
  > ParameterInformation_label;

  enum class ParameterInformation_documentationType {
    STRING_TYPE,
    MARKUP_CONTENT,
  };

  extern std::map<ParameterInformation_documentationType, std::string> ParameterInformation_documentationTypeNames;

  typedef std::variant<
    string_t,
    std::unique_ptr<MarkupContent>
  > ParameterInformation_documentation;

  /**
   * Represents a parameter of a callable-signature. A parameter can
   * have a label and a doc-comment.
   */
  struct ParameterInformation
  {
    /**
     * The label of this parameter information.
     *
     * Either a string or an inclusive start and exclusive end offsets within its containing
     * signature label. (see SignatureInformation.label). The offsets are based on a UTF-16
     * string representation as `Position` and `Range` does.
     *
     * *Note*: a label of type string should be a substring of its containing signature label.
     * Its intended use case is to highlight the parameter label part in the `SignatureInformation.label`.
     */
    ParameterInformation_label label;
    /**
     * The human-readable doc-comment of this parameter. Will be shown
     * in the UI but can be omitted.
     */
    std::optional<ParameterInformation_documentation> documentation;
  };

  enum class SignatureInformation_documentationType {
    STRING_TYPE,
    MARKUP_CONTENT,
  };

  extern std::map<SignatureInformation_documentationType, std::string> SignatureInformation_documentationTypeNames;

  typedef std::variant<
    string_t,
    std::unique_ptr<MarkupContent>
  > SignatureInformation_documentation;

  /**
   * Represents the signature of something callable. A signature
   * can have a label, like a function-name, a doc-comment, and
   * a set of parameters.
   */
  struct SignatureInformation
  {
    /**
     * The label of this signature. Will be shown in
     * the UI.
     */
    string_t label;
    /**
     * The human-readable doc-comment of this signature. Will be shown
     * in the UI but can be omitted.
     */
    std::optional<SignatureInformation_documentation> documentation;
    /**
     * The parameters of this signature.
     */
    std::optional<std::vector<std::unique_ptr<ParameterInformation>>> parameters;
    /**
     * The index of the active parameter.
     *
     * If provided, this is used in place of `SignatureHelp.activeParameter`.
     *
     * @since 3.16.0
     */
    std::optional<uinteger_t> activeParameter;
  };

  /**
   * Signature help represents the signature of something
   * callable. There can be multiple signature but only one
   * active and only one active parameter.
   */
  struct SignatureHelp
  {
    /**
     * One or more signatures.
     */
    std::vector<std::unique_ptr<SignatureInformation>> signatures;
    /**
     * The active signature. If omitted or the value lies outside the
     * range of `signatures` the value defaults to zero or is ignored if
     * the `SignatureHelp` has no signatures.
     *
     * Whenever possible implementors should make an active decision about
     * the active signature and shouldn't rely on a default value.
     *
     * In future version of the protocol this property might become
     * mandatory to better express this.
     */
    std::optional<uinteger_t> activeSignature;
    /**
     * The active parameter of the active signature. If omitted or the value
     * lies outside the range of `signatures[activeSignature].parameters`
     * defaults to 0 if the active signature has parameters. If
     * the active signature has no parameters it is ignored.
     * In future version of the protocol this property might become
     * mandatory to better express the active parameter if the
     * active signature does have any.
     */
    std::optional<uinteger_t> activeParameter;
  };

  /**
   * Additional information about the context in which a signature help request was triggered.
   *
   * @since 3.15.0
   */
  struct SignatureHelpContext
  {
    /**
     * Action that caused signature help to be triggered.
     */
    SignatureHelpTriggerKind triggerKind;
    /**
     * Character that caused signature help to be triggered.
     *
     * This is undefined when `triggerKind !== SignatureHelpTriggerKind.TriggerCharacter`
     */
    std::optional<string_t> triggerCharacter;
    /**
     * `true` if signature help was already showing when it was triggered.
     *
     * Retriggers occurs when the signature help is already active and can be caused by actions such as
     * typing a trigger character, a cursor move, or document content changes.
     */
    boolean_t isRetrigger;
    /**
     * The currently active `SignatureHelp`.
     *
     * The `activeSignatureHelp` has its `SignatureHelp.activeSignature` field updated based on
     * the user navigating through available signatures.
     */
    std::optional<std::unique_ptr<SignatureHelp>> activeSignatureHelp;
  };

  /**
   * Matching options for the file operation pattern.
   *
   * @since 3.16.0
   */
  struct FileOperationPatternOptions
  {
    /**
     * The pattern should be matched ignoring casing.
     */
    std::optional<boolean_t> ignoreCase;
  };

  /**
   * A pattern to describe in which file operation requests or notifications
   * the server is interested in receiving.
   *
   * @since 3.16.0
   */
  struct FileOperationPattern
  {
    /**
     * The glob pattern to match. Glob patterns can have the following syntax:
     * - `*` to match one or more characters in a path segment
     * - `?` to match on one character in a path segment
     * - `**` to match any number of path segments, including none
     * - `{}` to group sub patterns into an OR expression. (e.g. `**​/​*.{ts,js}` matches all TypeScript and JavaScript files)
     * - `[]` to declare a range of characters to match in a path segment (e.g., `example.[0-9]` to match on `example.0`, `example.1`, …)
     * - `[!...]` to negate a range of characters to match in a path segment (e.g., `example.[!0-9]` to match on `example.a`, `example.b`, but not `example.0`)
     */
    string_t glob;
    /**
     * Whether to match files or folders with this pattern.
     *
     * Matches both if undefined.
     */
    std::optional<FileOperationPatternKind> matches;
    /**
     * Additional options used during matching.
     */
    std::optional<std::unique_ptr<FileOperationPatternOptions>> options;
  };

  /**
   * A filter to describe in which file operation requests or notifications
   * the server is interested in receiving.
   *
   * @since 3.16.0
   */
  struct FileOperationFilter
  {
    /**
     * A Uri scheme like `file` or `untitled`.
     */
    std::optional<string_t> scheme;
    /**
     * The actual file operation pattern.
     */
    std::unique_ptr<FileOperationPattern> pattern;
  };

  /**
   * The options to register for file operations.
   *
   * @since 3.16.0
   */
  struct FileOperationRegistrationOptions
  {
    /**
     * The actual filters.
     */
    std::vector<std::unique_ptr<FileOperationFilter>> filters;
  };

  /**
   * Options for notifications/requests for user operations on files.
   *
   * @since 3.16.0
   */
  struct FileOperationOptions
  {
    /**
     * The server is interested in receiving didCreateFiles notifications.
     */
    std::optional<std::unique_ptr<FileOperationRegistrationOptions>> didCreate;
    /**
     * The server is interested in receiving willCreateFiles requests.
     */
    std::optional<std::unique_ptr<FileOperationRegistrationOptions>> willCreate;
    /**
     * The server is interested in receiving didRenameFiles notifications.
     */
    std::optional<std::unique_ptr<FileOperationRegistrationOptions>> didRename;
    /**
     * The server is interested in receiving willRenameFiles requests.
     */
    std::optional<std::unique_ptr<FileOperationRegistrationOptions>> willRename;
    /**
     * The server is interested in receiving didDeleteFiles file notifications.
     */
    std::optional<std::unique_ptr<FileOperationRegistrationOptions>> didDelete;
    /**
     * The server is interested in receiving willDeleteFiles file requests.
     */
    std::optional<std::unique_ptr<FileOperationRegistrationOptions>> willDelete;
  };

  struct ExecutionSummary
  {
    /**
     * A strict monotonically increasing value
     * indicating the execution order of a cell
     * inside a notebook.
     */
    uinteger_t executionOrder;
    /**
     * Whether the execution was successful or
     * not if known by the client.
     */
    std::optional<boolean_t> success;
  };

  /**
   * A notebook cell.
   *
   * A cell's document URI must be unique across ALL notebook
   * cells and can therefore be used to uniquely identify a
   * notebook cell or the cell's text document.
   *
   * @since 3.17.0
   */
  struct NotebookCell
  {
    /**
     * The cell's kind
     */
    NotebookCellKind kind;
    /**
     * The URI of the cell's text document
     * content.
     */
    DocumentUri document;
    /**
     * Additional metadata stored with the cell.
     *
     * Note: should always be an object literal (e.g. LSPObject)
     */
    std::optional<LSPObject> metadata;
    /**
     * Additional execution summary information
     * if supported by the client.
     */
    std::optional<std::unique_ptr<ExecutionSummary>> executionSummary;
  };

  /**
   * A change describing how to move a `NotebookCell`
   * array from state S to S'.
   *
   * @since 3.17.0
   */
  struct NotebookCellArrayChange
  {
    /**
     * The start oftest of the cell that changed.
     */
    uinteger_t start;
    /**
     * The deleted cells
     */
    uinteger_t deleteCount;
    /**
     * The new cells, if any
     */
    std::optional<std::vector<std::unique_ptr<NotebookCell>>> cells;
  };

  /**
   * A notebook document.
   *
   * @since 3.17.0
   */
  struct NotebookDocument
  {
    /**
     * The notebook document's uri.
     */
    URI uri;
    /**
     * The type of the notebook.
     */
    string_t notebookType;
    /**
     * The version number of this document (it will increase after each
     * change, including undo/redo).
     */
    integer_t version;
    /**
     * Additional metadata stored with the notebook
     * document.
     *
     * Note: should always be an object literal (e.g. LSPObject)
     */
    std::optional<LSPObject> metadata;
    /**
     * The cells of a notebook.
     */
    std::vector<std::unique_ptr<NotebookCell>> cells;
  };

  /**
   * The params sent in an open notebook document notification.
   *
   * @since 3.17.0
   */
  struct DidOpenNotebookDocumentParams
  {
    /**
     * The notebook document that got opened.
     */
    std::unique_ptr<NotebookDocument> notebookDocument;
    /**
     * The text documents that represent the content
     * of a notebook cell.
     */
    std::vector<std::unique_ptr<TextDocumentItem>> cellTextDocuments;
  };

  struct WorkspaceEditClientCapabilities_changeAnnotationSupport
  {
    std::optional<boolean_t> groupsOnLabel;
  };

  struct WorkspaceEditClientCapabilities
  {
    /**
     * The client supports versioned document changes in `WorkspaceEdit`s
     */
    std::optional<boolean_t> documentChanges;
    /**
     * The resource operations the client supports. Clients should at least
     * support 'create', 'rename' and 'delete' files and folders.
     *
     * @since 3.13.0
     */
    std::optional<std::vector<ResourceOperationKind>> resourceOperations;
    /**
     * The failure handling strategy of a client if applying the workspace edit
     * fails.
     *
     * @since 3.13.0
     */
    std::optional<FailureHandlingKind> failureHandling;
    /**
     * Whether the client normalizes line endings to the client specific
     * setting.
     * If set to `true` the client will normalize line ending characters
     * in a workspace edit to the client-specified new line
     * character.
     *
     * @since 3.16.0
     */
    std::optional<boolean_t> normalizesLineEndings;
    /**
     * Whether the client in general supports change annotations on text edits,
     * create file, rename file and delete file changes.
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<WorkspaceEditClientCapabilities_changeAnnotationSupport>> changeAnnotationSupport;
  };

  struct DidChangeConfigurationClientCapabilities
  {
    /**
     * Did change configuration notification supports dynamic registration.
     */
    std::optional<boolean_t> dynamicRegistration;
  };

  struct DidChangeWatchedFilesClientCapabilities
  {
    /**
     * Did change watched files notification supports dynamic registration. Please note
     * that the current protocol doesn't support static configuration for file changes
     * from the server side.
     */
    std::optional<boolean_t> dynamicRegistration;
    /**
     * Whether the client has support for {@link  RelativePattern relative pattern}
     * or not.
     *
     * @since 3.17.0
     */
    std::optional<boolean_t> relativePatternSupport;
  };

  struct WorkspaceSymbolClientCapabilities_symbolKind
  {
    std::optional<std::vector<SymbolKind>> valueSet;
  };

  struct WorkspaceSymbolClientCapabilities_tagSupport
  {
    std::vector<SymbolTag> valueSet;
  };

  struct WorkspaceSymbolClientCapabilities_resolveSupport
  {
    std::vector<string_t> properties;
  };

  /**
   * Client capabilities for a {@link WorkspaceSymbolRequest}.
   */
  struct WorkspaceSymbolClientCapabilities
  {
    /**
     * Symbol request supports dynamic registration.
     */
    std::optional<boolean_t> dynamicRegistration;
    /**
     * Specific capabilities for the `SymbolKind` in the `workspace/symbol` request.
     */
    std::optional<std::unique_ptr<WorkspaceSymbolClientCapabilities_symbolKind>> symbolKind;
    /**
     * The client supports tags on `SymbolInformation`.
     * Clients supporting tags have to handle unknown tags gracefully.
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<WorkspaceSymbolClientCapabilities_tagSupport>> tagSupport;
    /**
     * The client support partial workspace symbols. The client will send the
     * request `workspaceSymbol/resolve` to the server to resolve additional
     * properties.
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<WorkspaceSymbolClientCapabilities_resolveSupport>> resolveSupport;
  };

  /**
   * The client capabilities of a {@link ExecuteCommandRequest}.
   */
  struct ExecuteCommandClientCapabilities
  {
    /**
     * Execute command supports dynamic registration.
     */
    std::optional<boolean_t> dynamicRegistration;
  };

  /**
   * @since 3.16.0
   */
  struct SemanticTokensWorkspaceClientCapabilities
  {
    /**
     * Whether the client implementation supports a refresh request sent from
     * the server to the client.
     *
     * Note that this event is global and will force the client to refresh all
     * semantic tokens currently shown. It should be used with absolute care
     * and is useful for situation where a server for example detects a project
     * wide change that requires such a calculation.
     */
    std::optional<boolean_t> refreshSupport;
  };

  /**
   * @since 3.16.0
   */
  struct CodeLensWorkspaceClientCapabilities
  {
    /**
     * Whether the client implementation supports a refresh request sent from the
     * server to the client.
     *
     * Note that this event is global and will force the client to refresh all
     * code lenses currently shown. It should be used with absolute care and is
     * useful for situation where a server for example detect a project wide
     * change that requires such a calculation.
     */
    std::optional<boolean_t> refreshSupport;
  };

  /**
   * Capabilities relating to events from file operations by the user in the client.
   *
   * These events do not come from the file system, they come from user operations
   * like renaming a file in the UI.
   *
   * @since 3.16.0
   */
  struct FileOperationClientCapabilities
  {
    /**
     * Whether the client supports dynamic registration for file requests/notifications.
     */
    std::optional<boolean_t> dynamicRegistration;
    /**
     * The client has support for sending didCreateFiles notifications.
     */
    std::optional<boolean_t> didCreate;
    /**
     * The client has support for sending willCreateFiles requests.
     */
    std::optional<boolean_t> willCreate;
    /**
     * The client has support for sending didRenameFiles notifications.
     */
    std::optional<boolean_t> didRename;
    /**
     * The client has support for sending willRenameFiles requests.
     */
    std::optional<boolean_t> willRename;
    /**
     * The client has support for sending didDeleteFiles notifications.
     */
    std::optional<boolean_t> didDelete;
    /**
     * The client has support for sending willDeleteFiles requests.
     */
    std::optional<boolean_t> willDelete;
  };

  /**
   * Client workspace capabilities specific to inline values.
   *
   * @since 3.17.0
   */
  struct InlineValueWorkspaceClientCapabilities
  {
    /**
     * Whether the client implementation supports a refresh request sent from the
     * server to the client.
     *
     * Note that this event is global and will force the client to refresh all
     * inline values currently shown. It should be used with absolute care and is
     * useful for situation where a server for example detects a project wide
     * change that requires such a calculation.
     */
    std::optional<boolean_t> refreshSupport;
  };

  /**
   * Client workspace capabilities specific to inlay hints.
   *
   * @since 3.17.0
   */
  struct InlayHintWorkspaceClientCapabilities
  {
    /**
     * Whether the client implementation supports a refresh request sent from
     * the server to the client.
     *
     * Note that this event is global and will force the client to refresh all
     * inlay hints currently shown. It should be used with absolute care and
     * is useful for situation where a server for example detects a project wide
     * change that requires such a calculation.
     */
    std::optional<boolean_t> refreshSupport;
  };

  /**
   * Workspace client capabilities specific to diagnostic pull requests.
   *
   * @since 3.17.0
   */
  struct DiagnosticWorkspaceClientCapabilities
  {
    /**
     * Whether the client implementation supports a refresh request sent from
     * the server to the client.
     *
     * Note that this event is global and will force the client to refresh all
     * pulled diagnostics currently shown. It should be used with absolute care and
     * is useful for situation where a server for example detects a project wide
     * change that requires such a calculation.
     */
    std::optional<boolean_t> refreshSupport;
  };

  /**
   * Client workspace capabilities specific to folding ranges
   *
   * @since 3.18.0
   * @proposed
   */
  struct FoldingRangeWorkspaceClientCapabilities
  {
    /**
     * Whether the client implementation supports a refresh request sent from the
     * server to the client.
     *
     * Note that this event is global and will force the client to refresh all
     * folding ranges currently shown. It should be used with absolute care and is
     * useful for situation where a server for example detects a project wide
     * change that requires such a calculation.
     *
     * @since 3.18.0
     * @proposed
     */
    std::optional<boolean_t> refreshSupport;
  };

  /**
   * Workspace specific client capabilities.
   */
  struct WorkspaceClientCapabilities
  {
    /**
     * The client supports applying batch edits
     * to the workspace by supporting the request
     * 'workspace/applyEdit'
     */
    std::optional<boolean_t> applyEdit;
    /**
     * Capabilities specific to `WorkspaceEdit`s.
     */
    std::optional<std::unique_ptr<WorkspaceEditClientCapabilities>> workspaceEdit;
    /**
     * Capabilities specific to the `workspace/didChangeConfiguration` notification.
     */
    std::optional<std::unique_ptr<DidChangeConfigurationClientCapabilities>> didChangeConfiguration;
    /**
     * Capabilities specific to the `workspace/didChangeWatchedFiles` notification.
     */
    std::optional<std::unique_ptr<DidChangeWatchedFilesClientCapabilities>> didChangeWatchedFiles;
    /**
     * Capabilities specific to the `workspace/symbol` request.
     */
    std::optional<std::unique_ptr<WorkspaceSymbolClientCapabilities>> symbol;
    /**
     * Capabilities specific to the `workspace/executeCommand` request.
     */
    std::optional<std::unique_ptr<ExecuteCommandClientCapabilities>> executeCommand;
    /**
     * The client has support for workspace folders.
     *
     * @since 3.6.0
     */
    std::optional<boolean_t> workspaceFolders;
    /**
     * The client supports `workspace/configuration` requests.
     *
     * @since 3.6.0
     */
    std::optional<boolean_t> configuration;
    /**
     * Capabilities specific to the semantic token requests scoped to the
     * workspace.
     *
     * @since 3.16.0.
     */
    std::optional<std::unique_ptr<SemanticTokensWorkspaceClientCapabilities>> semanticTokens;
    /**
     * Capabilities specific to the code lens requests scoped to the
     * workspace.
     *
     * @since 3.16.0.
     */
    std::optional<std::unique_ptr<CodeLensWorkspaceClientCapabilities>> codeLens;
    /**
     * The client has support for file notifications/requests for user operations on files.
     *
     * Since 3.16.0
     */
    std::optional<std::unique_ptr<FileOperationClientCapabilities>> fileOperations;
    /**
     * Capabilities specific to the inline values requests scoped to the
     * workspace.
     *
     * @since 3.17.0.
     */
    std::optional<std::unique_ptr<InlineValueWorkspaceClientCapabilities>> inlineValue;
    /**
     * Capabilities specific to the inlay hint requests scoped to the
     * workspace.
     *
     * @since 3.17.0.
     */
    std::optional<std::unique_ptr<InlayHintWorkspaceClientCapabilities>> inlayHint;
    /**
     * Capabilities specific to the diagnostic requests scoped to the
     * workspace.
     *
     * @since 3.17.0.
     */
    std::optional<std::unique_ptr<DiagnosticWorkspaceClientCapabilities>> diagnostics;
    /**
     * Capabilities specific to the folding range requests scoped to the workspace.
     *
     * @since 3.18.0
     * @proposed
     */
    std::optional<std::unique_ptr<FoldingRangeWorkspaceClientCapabilities>> foldingRange;
  };

  struct TextDocumentSyncClientCapabilities
  {
    /**
     * Whether text document synchronization supports dynamic registration.
     */
    std::optional<boolean_t> dynamicRegistration;
    /**
     * The client supports sending will save notifications.
     */
    std::optional<boolean_t> willSave;
    /**
     * The client supports sending a will save request and
     * waits for a response providing text edits which will
     * be applied to the document before it is saved.
     */
    std::optional<boolean_t> willSaveWaitUntil;
    /**
     * The client supports did save notifications.
     */
    std::optional<boolean_t> didSave;
  };

  struct CompletionClientCapabilities_completionItem_insertTextModeSupport
  {
    std::vector<InsertTextMode> valueSet;
  };

  struct CompletionClientCapabilities_completionItem_resolveSupport
  {
    std::vector<string_t> properties;
  };

  struct CompletionClientCapabilities_completionItem_tagSupport
  {
    std::vector<CompletionItemTag> valueSet;
  };

  struct CompletionClientCapabilities_completionItem
  {
    std::optional<boolean_t> snippetSupport;
    std::optional<boolean_t> commitCharactersSupport;
    std::optional<std::vector<MarkupKind>> documentationFormat;
    std::optional<boolean_t> deprecatedSupport;
    std::optional<boolean_t> preselectSupport;
    std::optional<std::unique_ptr<CompletionClientCapabilities_completionItem_tagSupport>> tagSupport;
    std::optional<boolean_t> insertReplaceSupport;
    std::optional<std::unique_ptr<CompletionClientCapabilities_completionItem_resolveSupport>> resolveSupport;
    std::optional<std::unique_ptr<CompletionClientCapabilities_completionItem_insertTextModeSupport>> insertTextModeSupport;
    std::optional<boolean_t> labelDetailsSupport;
  };

  struct CompletionClientCapabilities_completionItemKind
  {
    std::optional<std::vector<CompletionItemKind>> valueSet;
  };

  struct CompletionClientCapabilities_completionList
  {
    std::optional<std::vector<string_t>> itemDefaults;
  };

  /**
   * Completion client capabilities
   */
  struct CompletionClientCapabilities
  {
    /**
     * Whether completion supports dynamic registration.
     */
    std::optional<boolean_t> dynamicRegistration;
    /**
     * The client supports the following `CompletionItem` specific
     * capabilities.
     */
    std::optional<std::unique_ptr<CompletionClientCapabilities_completionItem>> completionItem;
    std::optional<std::unique_ptr<CompletionClientCapabilities_completionItemKind>> completionItemKind;
    /**
     * Defines how the client handles whitespace and indentation
     * when accepting a completion item that uses multi line
     * text in either `insertText` or `textEdit`.
     *
     * @since 3.17.0
     */
    std::optional<InsertTextMode> insertTextMode;
    /**
     * The client supports to send additional context information for a
     * `textDocument/completion` request.
     */
    std::optional<boolean_t> contextSupport;
    /**
     * The client supports the following `CompletionList` specific
     * capabilities.
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<CompletionClientCapabilities_completionList>> completionList;
  };

  struct HoverClientCapabilities
  {
    /**
     * Whether hover supports dynamic registration.
     */
    std::optional<boolean_t> dynamicRegistration;
    /**
     * Client supports the following content formats for the content
     * property. The order describes the preferred format of the client.
     */
    std::optional<std::vector<MarkupKind>> contentFormat;
  };

  struct SignatureHelpClientCapabilities_signatureInformation_parameterInformation
  {
    std::optional<boolean_t> labelOffsetSupport;
  };

  struct SignatureHelpClientCapabilities_signatureInformation
  {
    std::optional<std::vector<MarkupKind>> documentationFormat;
    std::optional<std::unique_ptr<SignatureHelpClientCapabilities_signatureInformation_parameterInformation>> parameterInformation;
    std::optional<boolean_t> activeParameterSupport;
  };

  /**
   * Client Capabilities for a {@link SignatureHelpRequest}.
   */
  struct SignatureHelpClientCapabilities
  {
    /**
     * Whether signature help supports dynamic registration.
     */
    std::optional<boolean_t> dynamicRegistration;
    /**
     * The client supports the following `SignatureInformation`
     * specific properties.
     */
    std::optional<std::unique_ptr<SignatureHelpClientCapabilities_signatureInformation>> signatureInformation;
    /**
     * The client supports to send additional context information for a
     * `textDocument/signatureHelp` request. A client that opts into
     * contextSupport will also support the `retriggerCharacters` on
     * `SignatureHelpOptions`.
     *
     * @since 3.15.0
     */
    std::optional<boolean_t> contextSupport;
  };

  /**
   * @since 3.14.0
   */
  struct DeclarationClientCapabilities
  {
    /**
     * Whether declaration supports dynamic registration. If this is set to `true`
     * the client supports the new `DeclarationRegistrationOptions` return value
     * for the corresponding server capability as well.
     */
    std::optional<boolean_t> dynamicRegistration;
    /**
     * The client supports additional metadata in the form of declaration links.
     */
    std::optional<boolean_t> linkSupport;
  };

  /**
   * Client Capabilities for a {@link DefinitionRequest}.
   */
  struct DefinitionClientCapabilities
  {
    /**
     * Whether definition supports dynamic registration.
     */
    std::optional<boolean_t> dynamicRegistration;
    /**
     * The client supports additional metadata in the form of definition links.
     *
     * @since 3.14.0
     */
    std::optional<boolean_t> linkSupport;
  };

  /**
   * Since 3.6.0
   */
  struct TypeDefinitionClientCapabilities
  {
    /**
     * Whether implementation supports dynamic registration. If this is set to `true`
     * the client supports the new `TypeDefinitionRegistrationOptions` return value
     * for the corresponding server capability as well.
     */
    std::optional<boolean_t> dynamicRegistration;
    /**
     * The client supports additional metadata in the form of definition links.
     *
     * Since 3.14.0
     */
    std::optional<boolean_t> linkSupport;
  };

  /**
   * @since 3.6.0
   */
  struct ImplementationClientCapabilities
  {
    /**
     * Whether implementation supports dynamic registration. If this is set to `true`
     * the client supports the new `ImplementationRegistrationOptions` return value
     * for the corresponding server capability as well.
     */
    std::optional<boolean_t> dynamicRegistration;
    /**
     * The client supports additional metadata in the form of definition links.
     *
     * @since 3.14.0
     */
    std::optional<boolean_t> linkSupport;
  };

  /**
   * Client Capabilities for a {@link ReferencesRequest}.
   */
  struct ReferenceClientCapabilities
  {
    /**
     * Whether references supports dynamic registration.
     */
    std::optional<boolean_t> dynamicRegistration;
  };

  /**
   * Client Capabilities for a {@link DocumentHighlightRequest}.
   */
  struct DocumentHighlightClientCapabilities
  {
    /**
     * Whether document highlight supports dynamic registration.
     */
    std::optional<boolean_t> dynamicRegistration;
  };

  struct DocumentSymbolClientCapabilities_symbolKind
  {
    std::optional<std::vector<SymbolKind>> valueSet;
  };

  struct DocumentSymbolClientCapabilities_tagSupport
  {
    std::vector<SymbolTag> valueSet;
  };

  /**
   * Client Capabilities for a {@link DocumentSymbolRequest}.
   */
  struct DocumentSymbolClientCapabilities
  {
    /**
     * Whether document symbol supports dynamic registration.
     */
    std::optional<boolean_t> dynamicRegistration;
    /**
     * Specific capabilities for the `SymbolKind` in the
     * `textDocument/documentSymbol` request.
     */
    std::optional<std::unique_ptr<DocumentSymbolClientCapabilities_symbolKind>> symbolKind;
    /**
     * The client supports hierarchical document symbols.
     */
    std::optional<boolean_t> hierarchicalDocumentSymbolSupport;
    /**
     * The client supports tags on `SymbolInformation`. Tags are supported on
     * `DocumentSymbol` if `hierarchicalDocumentSymbolSupport` is set to true.
     * Clients supporting tags have to handle unknown tags gracefully.
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<DocumentSymbolClientCapabilities_tagSupport>> tagSupport;
    /**
     * The client supports an additional label presented in the UI when
     * registering a document symbol provider.
     *
     * @since 3.16.0
     */
    std::optional<boolean_t> labelSupport;
  };

  struct CodeActionClientCapabilities_codeActionLiteralSupport_codeActionKind
  {
    std::vector<CodeActionKind> valueSet;
  };

  struct CodeActionClientCapabilities_codeActionLiteralSupport
  {
    std::unique_ptr<CodeActionClientCapabilities_codeActionLiteralSupport_codeActionKind> codeActionKind;
  };

  struct CodeActionClientCapabilities_resolveSupport
  {
    std::vector<string_t> properties;
  };

  /**
   * The Client Capabilities of a {@link CodeActionRequest}.
   */
  struct CodeActionClientCapabilities
  {
    /**
     * Whether code action supports dynamic registration.
     */
    std::optional<boolean_t> dynamicRegistration;
    /**
     * The client support code action literals of type `CodeAction` as a valid
     * response of the `textDocument/codeAction` request. If the property is not
     * set the request can only return `Command` literals.
     *
     * @since 3.8.0
     */
    std::optional<std::unique_ptr<CodeActionClientCapabilities_codeActionLiteralSupport>> codeActionLiteralSupport;
    /**
     * Whether code action supports the `isPreferred` property.
     *
     * @since 3.15.0
     */
    std::optional<boolean_t> isPreferredSupport;
    /**
     * Whether code action supports the `disabled` property.
     *
     * @since 3.16.0
     */
    std::optional<boolean_t> disabledSupport;
    /**
     * Whether code action supports the `data` property which is
     * preserved between a `textDocument/codeAction` and a
     * `codeAction/resolve` request.
     *
     * @since 3.16.0
     */
    std::optional<boolean_t> dataSupport;
    /**
     * Whether the client supports resolving additional code action
     * properties via a separate `codeAction/resolve` request.
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<CodeActionClientCapabilities_resolveSupport>> resolveSupport;
    /**
     * Whether the client honors the change annotations in
     * text edits and resource operations returned via the
     * `CodeAction#edit` property by for example presenting
     * the workspace edit in the user interface and asking
     * for confirmation.
     *
     * @since 3.16.0
     */
    std::optional<boolean_t> honorsChangeAnnotations;
  };

  /**
   * The client capabilities  of a {@link CodeLensRequest}.
   */
  struct CodeLensClientCapabilities
  {
    /**
     * Whether code lens supports dynamic registration.
     */
    std::optional<boolean_t> dynamicRegistration;
  };

  /**
   * The client capabilities of a {@link DocumentLinkRequest}.
   */
  struct DocumentLinkClientCapabilities
  {
    /**
     * Whether document link supports dynamic registration.
     */
    std::optional<boolean_t> dynamicRegistration;
    /**
     * Whether the client supports the `tooltip` property on `DocumentLink`.
     *
     * @since 3.15.0
     */
    std::optional<boolean_t> tooltipSupport;
  };

  struct DocumentColorClientCapabilities
  {
    /**
     * Whether implementation supports dynamic registration. If this is set to `true`
     * the client supports the new `DocumentColorRegistrationOptions` return value
     * for the corresponding server capability as well.
     */
    std::optional<boolean_t> dynamicRegistration;
  };

  /**
   * Client capabilities of a {@link DocumentFormattingRequest}.
   */
  struct DocumentFormattingClientCapabilities
  {
    /**
     * Whether formatting supports dynamic registration.
     */
    std::optional<boolean_t> dynamicRegistration;
  };

  /**
   * Client capabilities of a {@link DocumentRangeFormattingRequest}.
   */
  struct DocumentRangeFormattingClientCapabilities
  {
    /**
     * Whether range formatting supports dynamic registration.
     */
    std::optional<boolean_t> dynamicRegistration;
    /**
     * Whether the client supports formatting multiple ranges at once.
     *
     * @since 3.18.0
     * @proposed
     */
    std::optional<boolean_t> rangesSupport;
  };

  /**
   * Client capabilities of a {@link DocumentOnTypeFormattingRequest}.
   */
  struct DocumentOnTypeFormattingClientCapabilities
  {
    /**
     * Whether on type formatting supports dynamic registration.
     */
    std::optional<boolean_t> dynamicRegistration;
  };

  struct RenameClientCapabilities
  {
    /**
     * Whether rename supports dynamic registration.
     */
    std::optional<boolean_t> dynamicRegistration;
    /**
     * Client supports testing for validity of rename operations
     * before execution.
     *
     * @since 3.12.0
     */
    std::optional<boolean_t> prepareSupport;
    /**
     * Client supports the default behavior result.
     *
     * The value indicates the default behavior used by the
     * client.
     *
     * @since 3.16.0
     */
    std::optional<PrepareSupportDefaultBehavior> prepareSupportDefaultBehavior;
    /**
     * Whether the client honors the change annotations in
     * text edits and resource operations returned via the
     * rename request's workspace edit by for example presenting
     * the workspace edit in the user interface and asking
     * for confirmation.
     *
     * @since 3.16.0
     */
    std::optional<boolean_t> honorsChangeAnnotations;
  };

  struct FoldingRangeClientCapabilities_foldingRangeKind
  {
    std::optional<std::vector<FoldingRangeKind>> valueSet;
  };

  struct FoldingRangeClientCapabilities_foldingRange
  {
    std::optional<boolean_t> collapsedText;
  };

  struct FoldingRangeClientCapabilities
  {
    /**
     * Whether implementation supports dynamic registration for folding range
     * providers. If this is set to `true` the client supports the new
     * `FoldingRangeRegistrationOptions` return value for the corresponding
     * server capability as well.
     */
    std::optional<boolean_t> dynamicRegistration;
    /**
     * The maximum number of folding ranges that the client prefers to receive
     * per document. The value serves as a hint, servers are free to follow the
     * limit.
     */
    std::optional<uinteger_t> rangeLimit;
    /**
     * If set, the client signals that it only supports folding complete lines.
     * If set, client will ignore specified `startCharacter` and `endCharacter`
     * properties in a FoldingRange.
     */
    std::optional<boolean_t> lineFoldingOnly;
    /**
     * Specific options for the folding range kind.
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<FoldingRangeClientCapabilities_foldingRangeKind>> foldingRangeKind;
    /**
     * Specific options for the folding range.
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<FoldingRangeClientCapabilities_foldingRange>> foldingRange;
  };

  struct SelectionRangeClientCapabilities
  {
    /**
     * Whether implementation supports dynamic registration for selection range providers. If this is set to `true`
     * the client supports the new `SelectionRangeRegistrationOptions` return value for the corresponding server
     * capability as well.
     */
    std::optional<boolean_t> dynamicRegistration;
  };

  struct PublishDiagnosticsClientCapabilities_tagSupport
  {
    std::vector<DiagnosticTag> valueSet;
  };

  /**
   * The publish diagnostic client capabilities.
   */
  struct PublishDiagnosticsClientCapabilities
  {
    /**
     * Whether the clients accepts diagnostics with related information.
     */
    std::optional<boolean_t> relatedInformation;
    /**
     * Client supports the tag property to provide meta data about a diagnostic.
     * Clients supporting tags have to handle unknown tags gracefully.
     *
     * @since 3.15.0
     */
    std::optional<std::unique_ptr<PublishDiagnosticsClientCapabilities_tagSupport>> tagSupport;
    /**
     * Whether the client interprets the version property of the
     * `textDocument/publishDiagnostics` notification's parameter.
     *
     * @since 3.15.0
     */
    std::optional<boolean_t> versionSupport;
    /**
     * Client supports a codeDescription property
     *
     * @since 3.16.0
     */
    std::optional<boolean_t> codeDescriptionSupport;
    /**
     * Whether code action supports the `data` property which is
     * preserved between a `textDocument/publishDiagnostics` and
     * `textDocument/codeAction` request.
     *
     * @since 3.16.0
     */
    std::optional<boolean_t> dataSupport;
  };

  /**
   * @since 3.16.0
   */
  struct CallHierarchyClientCapabilities
  {
    /**
     * Whether implementation supports dynamic registration. If this is set to `true`
     * the client supports the new `(TextDocumentRegistrationOptions & StaticRegistrationOptions)`
     * return value for the corresponding server capability as well.
     */
    std::optional<boolean_t> dynamicRegistration;
  };

  struct SemanticTokensClientCapabilities_requests_full_1
  {
    std::optional<boolean_t> delta;
  };

  enum class SemanticTokensClientCapabilities_requests_fullType {
    BOOLEAN_TYPE,
    SEMANTIC_TOKENS_CLIENT_CAPABILITIES_REQUESTS_FULL_1,
  };

  extern std::map<SemanticTokensClientCapabilities_requests_fullType, std::string> SemanticTokensClientCapabilities_requests_fullTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<SemanticTokensClientCapabilities_requests_full_1>
  > SemanticTokensClientCapabilities_requests_full;

  struct SemanticTokensClientCapabilities_requests_range_1
  {
  };

  enum class SemanticTokensClientCapabilities_requests_rangeType {
    BOOLEAN_TYPE,
    SEMANTIC_TOKENS_CLIENT_CAPABILITIES_REQUESTS_RANGE_1,
  };

  extern std::map<SemanticTokensClientCapabilities_requests_rangeType, std::string> SemanticTokensClientCapabilities_requests_rangeTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<SemanticTokensClientCapabilities_requests_range_1>
  > SemanticTokensClientCapabilities_requests_range;

  struct SemanticTokensClientCapabilities_requests
  {
    std::optional<SemanticTokensClientCapabilities_requests_range> range;
    std::optional<SemanticTokensClientCapabilities_requests_full> full;
  };

  /**
   * @since 3.16.0
   */
  struct SemanticTokensClientCapabilities
  {
    /**
     * Whether implementation supports dynamic registration. If this is set to `true`
     * the client supports the new `(TextDocumentRegistrationOptions & StaticRegistrationOptions)`
     * return value for the corresponding server capability as well.
     */
    std::optional<boolean_t> dynamicRegistration;
    /**
     * Which requests the client supports and might send to the server
     * depending on the server's capability. Please note that clients might not
     * show semantic tokens or degrade some of the user experience if a range
     * or full request is advertised by the client but not provided by the
     * server. If for example the client capability `requests.full` and
     * `request.range` are both set to true but the server only provides a
     * range provider the client might not render a minimap correctly or might
     * even decide to not show any semantic tokens at all.
     */
    std::unique_ptr<SemanticTokensClientCapabilities_requests> requests;
    /**
     * The token types that the client supports.
     */
    std::vector<string_t> tokenTypes;
    /**
     * The token modifiers that the client supports.
     */
    std::vector<string_t> tokenModifiers;
    /**
     * The token formats the clients supports.
     */
    std::vector<TokenFormat> formats;
    /**
     * Whether the client supports tokens that can overlap each other.
     */
    std::optional<boolean_t> overlappingTokenSupport;
    /**
     * Whether the client supports tokens that can span multiple lines.
     */
    std::optional<boolean_t> multilineTokenSupport;
    /**
     * Whether the client allows the server to actively cancel a
     * semantic token request, e.g. supports returning
     * LSPErrorCodes.ServerCancelled. If a server does the client
     * needs to retrigger the request.
     *
     * @since 3.17.0
     */
    std::optional<boolean_t> serverCancelSupport;
    /**
     * Whether the client uses semantic tokens to augment existing
     * syntax tokens. If set to `true` client side created syntax
     * tokens and semantic tokens are both used for colorization. If
     * set to `false` the client only uses the returned semantic tokens
     * for colorization.
     *
     * If the value is `undefined` then the client behavior is not
     * specified.
     *
     * @since 3.17.0
     */
    std::optional<boolean_t> augmentsSyntaxTokens;
  };

  /**
   * Client capabilities for the linked editing range request.
   *
   * @since 3.16.0
   */
  struct LinkedEditingRangeClientCapabilities
  {
    /**
     * Whether implementation supports dynamic registration. If this is set to `true`
     * the client supports the new `(TextDocumentRegistrationOptions & StaticRegistrationOptions)`
     * return value for the corresponding server capability as well.
     */
    std::optional<boolean_t> dynamicRegistration;
  };

  /**
   * Client capabilities specific to the moniker request.
   *
   * @since 3.16.0
   */
  struct MonikerClientCapabilities
  {
    /**
     * Whether moniker supports dynamic registration. If this is set to `true`
     * the client supports the new `MonikerRegistrationOptions` return value
     * for the corresponding server capability as well.
     */
    std::optional<boolean_t> dynamicRegistration;
  };

  /**
   * @since 3.17.0
   */
  struct TypeHierarchyClientCapabilities
  {
    /**
     * Whether implementation supports dynamic registration. If this is set to `true`
     * the client supports the new `(TextDocumentRegistrationOptions & StaticRegistrationOptions)`
     * return value for the corresponding server capability as well.
     */
    std::optional<boolean_t> dynamicRegistration;
  };

  /**
   * Client capabilities specific to inline values.
   *
   * @since 3.17.0
   */
  struct InlineValueClientCapabilities
  {
    /**
     * Whether implementation supports dynamic registration for inline value providers.
     */
    std::optional<boolean_t> dynamicRegistration;
  };

  struct InlayHintClientCapabilities_resolveSupport
  {
    std::vector<string_t> properties;
  };

  /**
   * Inlay hint client capabilities.
   *
   * @since 3.17.0
   */
  struct InlayHintClientCapabilities
  {
    /**
     * Whether inlay hints support dynamic registration.
     */
    std::optional<boolean_t> dynamicRegistration;
    /**
     * Indicates which properties a client can resolve lazily on an inlay
     * hint.
     */
    std::optional<std::unique_ptr<InlayHintClientCapabilities_resolveSupport>> resolveSupport;
  };

  /**
   * Client capabilities specific to diagnostic pull requests.
   *
   * @since 3.17.0
   */
  struct DiagnosticClientCapabilities
  {
    /**
     * Whether implementation supports dynamic registration. If this is set to `true`
     * the client supports the new `(TextDocumentRegistrationOptions & StaticRegistrationOptions)`
     * return value for the corresponding server capability as well.
     */
    std::optional<boolean_t> dynamicRegistration;
    /**
     * Whether the clients supports related documents for document diagnostic pulls.
     */
    std::optional<boolean_t> relatedDocumentSupport;
  };

  /**
   * Client capabilities specific to inline completions.
   *
   * @since 3.18.0
   * @proposed
   */
  struct InlineCompletionClientCapabilities
  {
    /**
     * Whether implementation supports dynamic registration for inline completion providers.
     */
    std::optional<boolean_t> dynamicRegistration;
  };

  /**
   * Text document specific client capabilities.
   */
  struct TextDocumentClientCapabilities
  {
    /**
     * Defines which synchronization capabilities the client supports.
     */
    std::optional<std::unique_ptr<TextDocumentSyncClientCapabilities>> synchronization;
    /**
     * Capabilities specific to the `textDocument/completion` request.
     */
    std::optional<std::unique_ptr<CompletionClientCapabilities>> completion;
    /**
     * Capabilities specific to the `textDocument/hover` request.
     */
    std::optional<std::unique_ptr<HoverClientCapabilities>> hover;
    /**
     * Capabilities specific to the `textDocument/signatureHelp` request.
     */
    std::optional<std::unique_ptr<SignatureHelpClientCapabilities>> signatureHelp;
    /**
     * Capabilities specific to the `textDocument/declaration` request.
     *
     * @since 3.14.0
     */
    std::optional<std::unique_ptr<DeclarationClientCapabilities>> declaration;
    /**
     * Capabilities specific to the `textDocument/definition` request.
     */
    std::optional<std::unique_ptr<DefinitionClientCapabilities>> definition;
    /**
     * Capabilities specific to the `textDocument/typeDefinition` request.
     *
     * @since 3.6.0
     */
    std::optional<std::unique_ptr<TypeDefinitionClientCapabilities>> typeDefinition;
    /**
     * Capabilities specific to the `textDocument/implementation` request.
     *
     * @since 3.6.0
     */
    std::optional<std::unique_ptr<ImplementationClientCapabilities>> implementation;
    /**
     * Capabilities specific to the `textDocument/references` request.
     */
    std::optional<std::unique_ptr<ReferenceClientCapabilities>> references;
    /**
     * Capabilities specific to the `textDocument/documentHighlight` request.
     */
    std::optional<std::unique_ptr<DocumentHighlightClientCapabilities>> documentHighlight;
    /**
     * Capabilities specific to the `textDocument/documentSymbol` request.
     */
    std::optional<std::unique_ptr<DocumentSymbolClientCapabilities>> documentSymbol;
    /**
     * Capabilities specific to the `textDocument/codeAction` request.
     */
    std::optional<std::unique_ptr<CodeActionClientCapabilities>> codeAction;
    /**
     * Capabilities specific to the `textDocument/codeLens` request.
     */
    std::optional<std::unique_ptr<CodeLensClientCapabilities>> codeLens;
    /**
     * Capabilities specific to the `textDocument/documentLink` request.
     */
    std::optional<std::unique_ptr<DocumentLinkClientCapabilities>> documentLink;
    /**
     * Capabilities specific to the `textDocument/documentColor` and the
     * `textDocument/colorPresentation` request.
     *
     * @since 3.6.0
     */
    std::optional<std::unique_ptr<DocumentColorClientCapabilities>> colorProvider;
    /**
     * Capabilities specific to the `textDocument/formatting` request.
     */
    std::optional<std::unique_ptr<DocumentFormattingClientCapabilities>> formatting;
    /**
     * Capabilities specific to the `textDocument/rangeFormatting` request.
     */
    std::optional<std::unique_ptr<DocumentRangeFormattingClientCapabilities>> rangeFormatting;
    /**
     * Capabilities specific to the `textDocument/onTypeFormatting` request.
     */
    std::optional<std::unique_ptr<DocumentOnTypeFormattingClientCapabilities>> onTypeFormatting;
    /**
     * Capabilities specific to the `textDocument/rename` request.
     */
    std::optional<std::unique_ptr<RenameClientCapabilities>> rename;
    /**
     * Capabilities specific to the `textDocument/foldingRange` request.
     *
     * @since 3.10.0
     */
    std::optional<std::unique_ptr<FoldingRangeClientCapabilities>> foldingRange;
    /**
     * Capabilities specific to the `textDocument/selectionRange` request.
     *
     * @since 3.15.0
     */
    std::optional<std::unique_ptr<SelectionRangeClientCapabilities>> selectionRange;
    /**
     * Capabilities specific to the `textDocument/publishDiagnostics` notification.
     */
    std::optional<std::unique_ptr<PublishDiagnosticsClientCapabilities>> publishDiagnostics;
    /**
     * Capabilities specific to the various call hierarchy requests.
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<CallHierarchyClientCapabilities>> callHierarchy;
    /**
     * Capabilities specific to the various semantic token request.
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<SemanticTokensClientCapabilities>> semanticTokens;
    /**
     * Capabilities specific to the `textDocument/linkedEditingRange` request.
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<LinkedEditingRangeClientCapabilities>> linkedEditingRange;
    /**
     * Client capabilities specific to the `textDocument/moniker` request.
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<MonikerClientCapabilities>> moniker;
    /**
     * Capabilities specific to the various type hierarchy requests.
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<TypeHierarchyClientCapabilities>> typeHierarchy;
    /**
     * Capabilities specific to the `textDocument/inlineValue` request.
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<InlineValueClientCapabilities>> inlineValue;
    /**
     * Capabilities specific to the `textDocument/inlayHint` request.
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<InlayHintClientCapabilities>> inlayHint;
    /**
     * Capabilities specific to the diagnostic pull model.
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<DiagnosticClientCapabilities>> diagnostic;
    /**
     * Client capabilities specific to inline completions.
     *
     * @since 3.18.0
     * @proposed
     */
    std::optional<std::unique_ptr<InlineCompletionClientCapabilities>> inlineCompletion;
  };

  /**
   * Notebook specific client capabilities.
   *
   * @since 3.17.0
   */
  struct NotebookDocumentSyncClientCapabilities
  {
    /**
     * Whether implementation supports dynamic registration. If this is
     * set to `true` the client supports the new
     * `(TextDocumentRegistrationOptions & StaticRegistrationOptions)`
     * return value for the corresponding server capability as well.
     */
    std::optional<boolean_t> dynamicRegistration;
    /**
     * The client supports sending execution summary data per cell.
     */
    std::optional<boolean_t> executionSummarySupport;
  };

  /**
   * Capabilities specific to the notebook document support.
   *
   * @since 3.17.0
   */
  struct NotebookDocumentClientCapabilities
  {
    /**
     * Capabilities specific to notebook document synchronization
     *
     * @since 3.17.0
     */
    std::unique_ptr<NotebookDocumentSyncClientCapabilities> synchronization;
  };

  struct ShowMessageRequestClientCapabilities_messageActionItem
  {
    std::optional<boolean_t> additionalPropertiesSupport;
  };

  /**
   * Show message request client capabilities
   */
  struct ShowMessageRequestClientCapabilities
  {
    /**
     * Capabilities specific to the `MessageActionItem` type.
     */
    std::optional<std::unique_ptr<ShowMessageRequestClientCapabilities_messageActionItem>> messageActionItem;
  };

  /**
   * Client capabilities for the showDocument request.
   *
   * @since 3.16.0
   */
  struct ShowDocumentClientCapabilities
  {
    /**
     * The client has support for the showDocument
     * request.
     */
    boolean_t support;
  };

  struct WindowClientCapabilities
  {
    /**
     * It indicates whether the client supports server initiated
     * progress using the `window/workDoneProgress/create` request.
     *
     * The capability also controls Whether client supports handling
     * of progress notifications. If set servers are allowed to report a
     * `workDoneProgress` property in the request specific server
     * capabilities.
     *
     * @since 3.15.0
     */
    std::optional<boolean_t> workDoneProgress;
    /**
     * Capabilities specific to the showMessage request.
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<ShowMessageRequestClientCapabilities>> showMessage;
    /**
     * Capabilities specific to the showDocument request.
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<ShowDocumentClientCapabilities>> showDocument;
  };

  /**
   * Client capabilities specific to regular expressions.
   *
   * @since 3.16.0
   */
  struct RegularExpressionsClientCapabilities
  {
    /**
     * The engine's name.
     */
    string_t engine;
    /**
     * The engine's version.
     */
    std::optional<string_t> version;
  };

  /**
   * Client capabilities specific to the used markdown parser.
   *
   * @since 3.16.0
   */
  struct MarkdownClientCapabilities
  {
    /**
     * The name of the parser.
     */
    string_t parser;
    /**
     * The version of the parser.
     */
    std::optional<string_t> version;
    /**
     * A list of HTML tags that the client allows / supports in
     * Markdown.
     *
     * @since 3.17.0
     */
    std::optional<std::vector<string_t>> allowedTags;
  };

  struct GeneralClientCapabilities_staleRequestSupport
  {
    boolean_t cancel;
    std::vector<string_t> retryOnContentModified;
  };

  /**
   * General client capabilities.
   *
   * @since 3.16.0
   */
  struct GeneralClientCapabilities
  {
    /**
     * Client capability that signals how the client
     * handles stale requests (e.g. a request
     * for which the client will not process the response
     * anymore since the information is outdated).
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<GeneralClientCapabilities_staleRequestSupport>> staleRequestSupport;
    /**
     * Client capabilities specific to regular expressions.
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<RegularExpressionsClientCapabilities>> regularExpressions;
    /**
     * Client capabilities specific to the client's markdown parser.
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<MarkdownClientCapabilities>> markdown;
    /**
     * The position encodings supported by the client. Client and server
     * have to agree on the same position encoding to ensure that offsets
     * (e.g. character position in a line) are interpreted the same on both
     * sides.
     *
     * To keep the protocol backwards compatible the following applies: if
     * the value 'utf-16' is missing from the array of position encodings
     * servers can assume that the client supports UTF-16. UTF-16 is
     * therefore a mandatory encoding.
     *
     * If omitted it defaults to ['utf-16'].
     *
     * Implementation considerations: since the conversion from one encoding
     * into another requires the content of the file / line the conversion
     * is best done where the file is read which is usually on the server
     * side.
     *
     * @since 3.17.0
     */
    std::optional<std::vector<PositionEncodingKind>> positionEncodings;
  };

  /**
   * Defines the capabilities provided by the client.
   */
  struct ClientCapabilities
  {
    /**
     * Workspace specific client capabilities.
     */
    std::optional<std::unique_ptr<WorkspaceClientCapabilities>> workspace;
    /**
     * Text document specific client capabilities.
     */
    std::optional<std::unique_ptr<TextDocumentClientCapabilities>> textDocument;
    /**
     * Capabilities specific to the notebook document support.
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<NotebookDocumentClientCapabilities>> notebookDocument;
    /**
     * Window specific client capabilities.
     */
    std::optional<std::unique_ptr<WindowClientCapabilities>> window;
    /**
     * General client capabilities.
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<GeneralClientCapabilities>> general;
    /**
     * Experimental client capabilities.
     */
    std::optional<std::unique_ptr<LSPAny>> experimental;
  };

  enum class RequestIdType {
    INTEGER_TYPE,
    STRING_TYPE,
  };

  extern std::map<RequestIdType, std::string> RequestIdTypeNames;

  typedef std::variant<
    integer_t,
    string_t
  > RequestId;

  enum class MessageParamsType {
    ARRAY_TYPE,
    OBJECT_TYPE,
  };

  extern std::map<MessageParamsType, std::string> MessageParamsTypeNames;

  typedef std::variant<
    LSPArray,
    LSPObject
  > MessageParams;

  struct NotificationMessage
    : public Message
  {
    /**
     * The method to be invoked.
     */
    string_t method;
    /**
     * The notification's params.
     */
    std::optional<MessageParams> params;
  };

  /**
   * A request message to describe a request between the client and the server.
   * Every processed request must send a response back to the sender of the
   * request.
   */
  struct RequestMessage
    : public Message
  {
    /**
     * The request id.
     */
    RequestId id;
    /**
     * The method to be invoked.
     */
    string_t method;
    /**
     * The method's params.
     */
    std::optional<MessageParams> params;
  };

  enum class ResponseIdType {
    INTEGER_TYPE,
    STRING_TYPE,
    NULL_TYPE,
  };

  extern std::map<ResponseIdType, std::string> ResponseIdTypeNames;

  typedef std::variant<
    integer_t,
    string_t,
    null_t
  > ResponseId;

  /**
   * A Response Message sent as a result of a request. If a request doesn’t
   * provide a result value the receiver of a request still needs to return a
   * response message to conform to the JSON-RPC specification. The result
   * property of the ResponseMessage should be set to null in this case to signal
   * a successful request.
   */
  struct ResponseMessage
    : public Message
  {
    /**
     * The request id.
     */
    ResponseId id;
    /**
     * The result of a request. This member is REQUIRED on success. This member
     * MUST NOT exist if there was an error invoking the method.
     */
    std::optional<std::unique_ptr<LSPAny>> result;
    /**
     * The error object in case a request fails.
     */
    std::optional<std::unique_ptr<ResponseError>> error;
  };

  enum class DefinitionType {
    LOCATION,
    LOCATION_ARRAY,
  };

  extern std::map<DefinitionType, std::string> DefinitionTypeNames;

  typedef std::variant<
    std::unique_ptr<Location>,
    std::vector<std::unique_ptr<Location>>
  > Definition;

  /**
   * Information about where a symbol is defined.
   *
   * Provides additional metadata over normal {@link Location location} definitions, including the range of
   * the defining symbol
   */
  typedef LocationLink DefinitionLink;

  enum class DeclarationType {
    LOCATION,
    LOCATION_ARRAY,
  };

  extern std::map<DeclarationType, std::string> DeclarationTypeNames;

  typedef std::variant<
    std::unique_ptr<Location>,
    std::vector<std::unique_ptr<Location>>
  > Declaration;

  /**
   * Information about where a symbol is declared.
   *
   * Provides additional metadata over normal {@link Location location} declarations, including the range of
   * the declaring symbol.
   *
   * Servers should prefer returning `DeclarationLink` over `Declaration` if supported
   * by the client.
   */
  typedef LocationLink DeclarationLink;

  enum class InlineValueType {
    INLINE_VALUE_TEXT,
    INLINE_VALUE_VARIABLE_LOOKUP,
    INLINE_VALUE_EVALUATABLE_EXPRESSION,
  };

  extern std::map<InlineValueType, std::string> InlineValueTypeNames;

  typedef std::variant<
    std::unique_ptr<InlineValueText>,
    std::unique_ptr<InlineValueVariableLookup>,
    std::unique_ptr<InlineValueEvaluatableExpression>
  > InlineValue;

  enum class DocumentDiagnosticReportType {
    RELATED_FULL_DOCUMENT_DIAGNOSTIC_REPORT,
    RELATED_UNCHANGED_DOCUMENT_DIAGNOSTIC_REPORT,
  };

  extern std::map<DocumentDiagnosticReportType, std::string> DocumentDiagnosticReportTypeNames;

  typedef std::variant<
    std::unique_ptr<RelatedFullDocumentDiagnosticReport>,
    std::unique_ptr<RelatedUnchangedDocumentDiagnosticReport>
  > DocumentDiagnosticReport;

  struct PrepareRenameResult_2
  {
    boolean_t defaultBehavior;
  };

  struct PrepareRenameResult_1
  {
    std::unique_ptr<Range> range;
    string_t placeholder;
  };

  enum class PrepareRenameResultType {
    RANGE,
    PREPARE_RENAME_RESULT_1,
    PREPARE_RENAME_RESULT_2,
  };

  extern std::map<PrepareRenameResultType, std::string> PrepareRenameResultTypeNames;

  typedef std::variant<
    std::unique_ptr<Range>,
    std::unique_ptr<PrepareRenameResult_1>,
    std::unique_ptr<PrepareRenameResult_2>
  > PrepareRenameResult;

  enum class ProgressTokenType {
    INTEGER_TYPE,
    STRING_TYPE,
  };

  extern std::map<ProgressTokenType, std::string> ProgressTokenTypeNames;

  typedef std::variant<
    integer_t,
    string_t
  > ProgressToken;

  struct WorkDoneProgressCreateParams
  {
    /**
     * The token to be used to report progress.
     */
    ProgressToken token;
  };

  struct ProgressParams
  {
    /**
     * The progress token provided by the client or server.
     */
    ProgressToken token;
    /**
     * The progress data.
     */
    std::unique_ptr<LSPAny> value;
  };

  struct PartialResultParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
  };

  struct WorkDoneProgressCancelParams
  {
    /**
     * The token to be used to report progress.
     */
    ProgressToken token;
  };

  struct WorkDoneProgressParams
  {
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
  };

  /**
   * A parameter literal used in inline completion requests.
   *
   * @since 3.18.0
   * @proposed
   */
  struct InlineCompletionParams
    : public TextDocumentPositionParams
  {
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    /**
     * Additional information about the context in which inline completions were
     * requested.
     */
    std::unique_ptr<InlineCompletionContext> context;
  };

  /**
   * The parameters of a {@link RenameRequest}.
   */
  struct RenameParams
  {
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    /**
     * The document to rename.
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
    /**
     * The position at which this request was sent.
     */
    std::unique_ptr<Position> position;
    /**
     * The new name of the symbol. If the given name is not valid the
     * request must return a {@link ResponseError} with an
     * appropriate message set.
     */
    string_t newName;
  };

  /**
   * The parameter of a `textDocument/prepareCallHierarchy` request.
   *
   * @since 3.16.0
   */
  struct CallHierarchyPrepareParams
    : public TextDocumentPositionParams
  {
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
  };

  /**
   * The parameter of a `typeHierarchy/supertypes` request.
   *
   * @since 3.17.0
   */
  struct TypeHierarchySupertypesParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    std::unique_ptr<TypeHierarchyItem> item;
  };

  /**
   * The parameters of a {@link CodeLensRequest}.
   */
  struct CodeLensParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    /**
     * The document to request code lens for.
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
  };

  /**
   * Parameters for a {@link DefinitionRequest}.
   */
  struct DefinitionParams
    : public TextDocumentPositionParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
  };

  /**
   * Parameters for a {@link DocumentColorRequest}.
   */
  struct DocumentColorParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    /**
     * The text document.
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
  };

  /**
   * The parameter of a `callHierarchy/incomingCalls` request.
   *
   * @since 3.16.0
   */
  struct CallHierarchyIncomingCallsParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    std::unique_ptr<CallHierarchyItem> item;
  };

  /**
   * Completion parameters
   */
  struct CompletionParams
    : public TextDocumentPositionParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    /**
     * The completion context. This is only available if the client specifies
     * to send this using the client capability `textDocument.completion.contextSupport === true`
     */
    std::optional<std::unique_ptr<CompletionContext>> context;
  };

  /**
   * Parameters for a {@link DocumentHighlightRequest}.
   */
  struct DocumentHighlightParams
    : public TextDocumentPositionParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
  };

  struct MonikerParams
    : public TextDocumentPositionParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
  };

  /**
   * The parameters of a {@link DocumentRangesFormattingRequest}.
   *
   * @since 3.18.0
   * @proposed
   */
  struct DocumentRangesFormattingParams
  {
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    /**
     * The document to format.
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
    /**
     * The ranges to format
     */
    std::vector<std::unique_ptr<Range>> ranges;
    /**
     * The format options
     */
    std::unique_ptr<FormattingOptions> options;
  };

  struct PrepareRenameParams
    : public TextDocumentPositionParams
  {
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
  };

  /**
   * Parameters of the document diagnostic request.
   *
   * @since 3.17.0
   */
  struct DocumentDiagnosticParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    /**
     * The text document.
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
    /**
     * The additional identifier  provided during registration.
     */
    std::optional<string_t> identifier;
    /**
     * The result id of a previous response if provided.
     */
    std::optional<string_t> previousResultId;
  };

  /**
   * A parameter literal used in inline value requests.
   *
   * @since 3.17.0
   */
  struct InlineValueParams
  {
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    /**
     * The text document.
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
    /**
     * The document range for which inline values should be computed.
     */
    std::unique_ptr<Range> range;
    /**
     * Additional information about the context in which inline values were
     * requested.
     */
    std::unique_ptr<InlineValueContext> context;
  };

  struct DeclarationParams
    : public TextDocumentPositionParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
  };

  /**
   * The parameters of a {@link DocumentFormattingRequest}.
   */
  struct DocumentFormattingParams
  {
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    /**
     * The document to format.
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
    /**
     * The format options.
     */
    std::unique_ptr<FormattingOptions> options;
  };

  struct ImplementationParams
    : public TextDocumentPositionParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
  };

  /**
   * @since 3.16.0
   */
  struct SemanticTokensParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    /**
     * The text document.
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
  };

  /**
   * The parameters of a {@link DocumentRangeFormattingRequest}.
   */
  struct DocumentRangeFormattingParams
  {
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    /**
     * The document to format.
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
    /**
     * The range to format
     */
    std::unique_ptr<Range> range;
    /**
     * The format options
     */
    std::unique_ptr<FormattingOptions> options;
  };

  /**
   * Parameters for a {@link FoldingRangeRequest}.
   */
  struct FoldingRangeParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    /**
     * The text document.
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
  };

  struct LinkedEditingRangeParams
    : public TextDocumentPositionParams
  {
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
  };

  /**
   * Parameters for a {@link SignatureHelpRequest}.
   */
  struct SignatureHelpParams
    : public TextDocumentPositionParams
  {
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    /**
     * The signature help context. This is only available if the client specifies
     * to send this using the client capability `textDocument.signatureHelp.contextSupport === true`
     *
     * @since 3.15.0
     */
    std::optional<std::unique_ptr<SignatureHelpContext>> context;
  };

  /**
   * Parameters for a {@link ColorPresentationRequest}.
   */
  struct ColorPresentationParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    /**
     * The text document.
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
    /**
     * The color to request presentations for.
     */
    std::unique_ptr<Color> color;
    /**
     * The range where the color would be inserted. Serves as a context.
     */
    std::unique_ptr<Range> range;
  };

  /**
   * The parameter of a `typeHierarchy/subtypes` request.
   *
   * @since 3.17.0
   */
  struct TypeHierarchySubtypesParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    std::unique_ptr<TypeHierarchyItem> item;
  };

  /**
   * A parameter literal used in selection range requests.
   */
  struct SelectionRangeParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    /**
     * The text document.
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
    /**
     * The positions inside the text document.
     */
    std::vector<std::unique_ptr<Position>> positions;
  };

  /**
   * Parameters for a {@link HoverRequest}.
   */
  struct HoverParams
    : public TextDocumentPositionParams
  {
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
  };

  /**
   * The parameter of a `textDocument/prepareTypeHierarchy` request.
   *
   * @since 3.17.0
   */
  struct TypeHierarchyPrepareParams
    : public TextDocumentPositionParams
  {
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
  };

  /**
   * A parameter literal used in inlay hint requests.
   *
   * @since 3.17.0
   */
  struct InlayHintParams
  {
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    /**
     * The text document.
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
    /**
     * The document range for which inlay hints should be computed.
     */
    std::unique_ptr<Range> range;
  };

  /**
   * Parameters for a {@link DocumentSymbolRequest}.
   */
  struct DocumentSymbolParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    /**
     * The text document.
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
  };

  /**
   * Parameters for a {@link ReferencesRequest}.
   */
  struct ReferenceParams
    : public TextDocumentPositionParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    std::unique_ptr<ReferenceContext> context;
  };

  enum class _InitializeParams_processIdType {
    INTEGER_TYPE,
    NULL_TYPE,
  };

  extern std::map<_InitializeParams_processIdType, std::string> _InitializeParams_processIdTypeNames;

  typedef std::variant<
    integer_t,
    null_t
  > _InitializeParams_processId;

  struct _InitializeParams_clientInfo
  {
    string_t name;
    std::optional<string_t> version;
  };

  enum class _InitializeParams_rootPathType {
    STRING_TYPE,
    NULL_TYPE,
  };

  extern std::map<_InitializeParams_rootPathType, std::string> _InitializeParams_rootPathTypeNames;

  typedef std::variant<
    string_t,
    null_t
  > _InitializeParams_rootPath;

  enum class _InitializeParams_rootUriType {
    DOCUMENT_URI,
    NULL_TYPE,
  };

  extern std::map<_InitializeParams_rootUriType, std::string> _InitializeParams_rootUriTypeNames;

  typedef std::variant<
    DocumentUri,
    null_t
  > _InitializeParams_rootUri;

  /**
   * The initialize parameters
   */
  struct _InitializeParams
  {
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    /**
     * The process Id of the parent process that started
     * the server.
     *
     * Is `null` if the process has not been started by another process.
     * If the parent process is not alive then the server should exit.
     */
    _InitializeParams_processId processId;
    /**
     * Information about the client
     *
     * @since 3.15.0
     */
    std::optional<std::unique_ptr<_InitializeParams_clientInfo>> clientInfo;
    /**
     * The locale the client is currently showing the user interface
     * in. This must not necessarily be the locale of the operating
     * system.
     *
     * Uses IETF language tags as the value's syntax
     * (See https://en.wikipedia.org/wiki/IETF_language_tag)
     *
     * @since 3.16.0
     */
    std::optional<string_t> locale;
    /**
     * The rootPath of the workspace. Is null
     * if no folder is open.
     *
     * @deprecated in favour of rootUri.
     */
    std::optional<_InitializeParams_rootPath> rootPath;
    /**
     * The rootUri of the workspace. Is null if no
     * folder is open. If both `rootPath` and `rootUri` are set
     * `rootUri` wins.
     *
     * @deprecated in favour of workspaceFolders.
     */
    _InitializeParams_rootUri rootUri;
    /**
     * The capabilities provided by the client (editor or tool)
     */
    std::unique_ptr<ClientCapabilities> capabilities;
    /**
     * User provided initialization options.
     */
    std::optional<std::unique_ptr<LSPAny>> initializationOptions;
    /**
     * The initial trace setting. If omitted trace is disabled ('off').
     */
    std::optional<TraceValues> trace;
  };

  struct InitializeParams
    : public _InitializeParams
    , public WorkspaceFoldersInitializeParams
  {
  };

  /**
   * @since 3.16.0
   */
  struct SemanticTokensRangeParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    /**
     * The text document.
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
    /**
     * The range the semantic tokens are requested for.
     */
    std::unique_ptr<Range> range;
  };

  /**
   * The parameters of a {@link CodeActionRequest}.
   */
  struct CodeActionParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    /**
     * The document in which the command was invoked.
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
    /**
     * The range for which the command was invoked.
     */
    std::unique_ptr<Range> range;
    /**
     * Context carrying additional information.
     */
    std::unique_ptr<CodeActionContext> context;
  };

  /**
   * The parameters of a {@link ExecuteCommandRequest}.
   */
  struct ExecuteCommandParams
  {
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    /**
     * The identifier of the actual command handler.
     */
    string_t command;
    /**
     * Arguments that the command should be invoked with.
     */
    std::optional<std::vector<std::unique_ptr<LSPAny>>> arguments;
  };

  /**
   * The parameter of a `callHierarchy/outgoingCalls` request.
   *
   * @since 3.16.0
   */
  struct CallHierarchyOutgoingCallsParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    std::unique_ptr<CallHierarchyItem> item;
  };

  struct TypeDefinitionParams
    : public TextDocumentPositionParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
  };

  /**
   * The parameters of a {@link WorkspaceSymbolRequest}.
   */
  struct WorkspaceSymbolParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    /**
     * A query string to filter symbols by. Clients may send an empty
     * string here to request all symbols.
     */
    string_t query;
  };

  /**
   * @since 3.16.0
   */
  struct SemanticTokensDeltaParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    /**
     * The text document.
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
    /**
     * The result id of a previous response. The result Id can either point to a full response
     * or a delta response depending on what was received last.
     */
    string_t previousResultId;
  };

  /**
   * The parameters of a {@link DocumentLinkRequest}.
   */
  struct DocumentLinkParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    /**
     * The document to provide document links for.
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
  };

  /**
   * Parameters of the workspace diagnostic request.
   *
   * @since 3.17.0
   */
  struct WorkspaceDiagnosticParams
  {
    /**
     * An optional token that a server can use to report partial results (e.g. streaming) to
     * the client.
     */
    std::optional<ProgressToken> partialResultToken;
    /**
     * An optional token that a server can use to report work done progress.
     */
    std::optional<ProgressToken> workDoneToken;
    /**
     * The additional identifier provided during registration.
     */
    std::optional<string_t> identifier;
    /**
     * The currently known diagnostic reports with their
     * previous result ids.
     */
    std::vector<std::unique_ptr<PreviousResultId>> previousResultIds;
  };

  /**
   * An identifier to refer to a change annotation stored with a workspace edit.
   */
  typedef string_t ChangeAnnotationIdentifier;

  /**
   * A special text edit with an additional change annotation.
   *
   * @since 3.16.0.
   */
  struct AnnotatedTextEdit
    : public TextEdit
  {
    /**
     * The actual identifier of the change annotation
     */
    ChangeAnnotationIdentifier annotationId;
  };

  enum class TextDocumentEdit_editsType {
    TEXT_EDIT,
    ANNOTATED_TEXT_EDIT,
  };

  extern std::map<TextDocumentEdit_editsType, std::string> TextDocumentEdit_editsTypeNames;

  typedef std::variant<
    std::unique_ptr<TextEdit>,
    std::unique_ptr<AnnotatedTextEdit>
  > TextDocumentEdit_edits;

  /**
   * Describes textual changes on a text document. A TextDocumentEdit describes all changes
   * on a document version Si and after they are applied move the document to version Si+1.
   * So the creator of a TextDocumentEdit doesn't need to sort the array of edits or do any
   * kind of ordering. However the edits must be non overlapping.
   */
  struct TextDocumentEdit
  {
    /**
     * The text document to change.
     */
    std::unique_ptr<OptionalVersionedTextDocumentIdentifier> textDocument;
    /**
     * The edits to be applied.
     *
     * @since 3.16.0 - support for AnnotatedTextEdit. This is guarded using a
     * client capability.
     */
    std::vector<TextDocumentEdit_edits> edits;
  };

  /**
   * A generic resource operation.
   */
  struct ResourceOperation
  {
    /**
     * The resource operation kind.
     */
    string_t kind;
    /**
     * An optional annotation identifier describing the operation.
     *
     * @since 3.16.0
     */
    std::optional<ChangeAnnotationIdentifier> annotationId;
  };

  /**
   * Create file operation.
   */
  struct CreateFile
    : public ResourceOperation
  {
    /**
     * A create
     */
    string_t kind;
    /**
     * The resource to create.
     */
    DocumentUri uri;
    /**
     * Additional options
     */
    std::optional<std::unique_ptr<CreateFileOptions>> options;
  };

  /**
   * Rename file operation
   */
  struct RenameFile
    : public ResourceOperation
  {
    /**
     * A rename
     */
    string_t kind;
    /**
     * The old (existing) location.
     */
    DocumentUri oldUri;
    /**
     * The new location.
     */
    DocumentUri newUri;
    /**
     * Rename options.
     */
    std::optional<std::unique_ptr<RenameFileOptions>> options;
  };

  /**
   * Delete file operation
   */
  struct DeleteFile
    : public ResourceOperation
  {
    /**
     * A delete
     */
    string_t kind;
    /**
     * The file to delete.
     */
    DocumentUri uri;
    /**
     * Delete options.
     */
    std::optional<std::unique_ptr<DeleteFileOptions>> options;
  };

  enum class WorkspaceEdit_documentChangesType {
    TEXT_DOCUMENT_EDIT,
    CREATE_FILE,
    RENAME_FILE,
    DELETE_FILE,
  };

  extern std::map<WorkspaceEdit_documentChangesType, std::string> WorkspaceEdit_documentChangesTypeNames;

  typedef std::variant<
    std::unique_ptr<TextDocumentEdit>,
    std::unique_ptr<CreateFile>,
    std::unique_ptr<RenameFile>,
    std::unique_ptr<DeleteFile>
  > WorkspaceEdit_documentChanges;

  /**
   * A workspace edit represents changes to many resources managed in the workspace. The edit
   * should either provide `changes` or `documentChanges`. If documentChanges are present
   * they are preferred over `changes` if the client can handle versioned document edits.
   *
   * Since version 3.13.0 a workspace edit can contain resource operations as well. If resource
   * operations are present clients need to execute the operations in the order in which they
   * are provided. So a workspace edit for example can consist of the following two changes:
   * (1) a create file a.txt and (2) a text document edit which insert text into file a.txt.
   *
   * An invalid sequence (e.g. (1) delete file a.txt and (2) insert text into file a.txt) will
   * cause failure of the operation. How the client recovers from the failure is described by
   * the client capability: `workspace.workspaceEdit.failureHandling`
   */
  struct WorkspaceEdit
  {
    /**
     * Holds changes to existing resources.
     */
    std::optional<std::map<DocumentUri, std::vector<std::unique_ptr<TextEdit>>>> changes;
    /**
     * Depending on the client capability `workspace.workspaceEdit.resourceOperations` document changes
     * are either an array of `TextDocumentEdit`s to express changes to n different text documents
     * where each text document edit addresses a specific version of a text document. Or it can contain
     * above `TextDocumentEdit`s mixed with create, rename and delete file / folder operations.
     *
     * Whether a client supports versioned document edits is expressed via
     * `workspace.workspaceEdit.documentChanges` client capability.
     *
     * If a client neither supports `documentChanges` nor `workspace.workspaceEdit.resourceOperations` then
     * only plain `TextEdit`s using the `changes` property are supported.
     */
    std::optional<std::vector<WorkspaceEdit_documentChanges>> documentChanges;
    /**
     * A map of change annotations that can be referenced in `AnnotatedTextEdit`s or create, rename and
     * delete file / folder operations.
     *
     * Whether clients honor this property depends on the client capability `workspace.changeAnnotationSupport`.
     *
     * @since 3.16.0
     */
    std::optional<std::map<ChangeAnnotationIdentifier, std::unique_ptr<ChangeAnnotation>>> changeAnnotations;
  };

  struct CodeAction_disabled
  {
    string_t reason;
  };

  /**
   * A code action represents a change that can be performed in code, e.g. to fix a problem or
   * to refactor code.
   *
   * A CodeAction must set either `edit` and/or a `command`. If both are supplied, the `edit` is applied first, then the `command` is executed.
   */
  struct CodeAction
  {
    /**
     * A short, human-readable, title for this code action.
     */
    string_t title;
    /**
     * The kind of the code action.
     *
     * Used to filter code actions.
     */
    std::optional<CodeActionKind> kind;
    /**
     * The diagnostics that this code action resolves.
     */
    std::optional<std::vector<std::unique_ptr<Diagnostic>>> diagnostics;
    /**
     * Marks this as a preferred action. Preferred actions are used by the `auto fix` command and can be targeted
     * by keybindings.
     *
     * A quick fix should be marked preferred if it properly addresses the underlying error.
     * A refactoring should be marked preferred if it is the most reasonable choice of actions to take.
     *
     * @since 3.15.0
     */
    std::optional<boolean_t> isPreferred;
    /**
     * Marks that the code action cannot currently be applied.
     *
     * Clients should follow the following guidelines regarding disabled code actions:
     *
     *   - Disabled code actions are not shown in automatic [lightbulbs](https://code.visualstudio.com/docs/editor/editingevolved#_code-action)
     *     code action menus.
     *
     *   - Disabled actions are shown as faded out in the code action menu when the user requests a more specific type
     *     of code action, such as refactorings.
     *
     *   - If the user has a [keybinding](https://code.visualstudio.com/docs/editor/refactoring#_keybindings-for-code-actions)
     *     that auto applies a code action and only disabled code actions are returned, the client should show the user an
     *     error message with `reason` in the editor.
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<CodeAction_disabled>> disabled;
    /**
     * The workspace edit this code action performs.
     */
    std::optional<std::unique_ptr<WorkspaceEdit>> edit;
    /**
     * A command this code action executes. If a code action
     * provides an edit and a command, first the edit is
     * executed and then the command.
     */
    std::optional<std::unique_ptr<Command>> command;
    /**
     * A data entry field that is preserved on a code action between
     * a `textDocument/codeAction` and a `codeAction/resolve` request.
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<LSPAny>> data;
  };

  /**
   * The parameters passed via an apply workspace edit request.
   */
  struct ApplyWorkspaceEditParams
  {
    /**
     * An optional label of the workspace edit. This label is
     * presented in the user interface for example on an undo
     * stack to undo the workspace edit.
     */
    std::optional<string_t> label;
    /**
     * The edits to apply.
     */
    std::unique_ptr<WorkspaceEdit> edit;
  };

  enum class WorkspaceDocumentDiagnosticReportType {
    WORKSPACE_FULL_DOCUMENT_DIAGNOSTIC_REPORT,
    WORKSPACE_UNCHANGED_DOCUMENT_DIAGNOSTIC_REPORT,
  };

  extern std::map<WorkspaceDocumentDiagnosticReportType, std::string> WorkspaceDocumentDiagnosticReportTypeNames;

  typedef std::variant<
    std::unique_ptr<WorkspaceFullDocumentDiagnosticReport>,
    std::unique_ptr<WorkspaceUnchangedDocumentDiagnosticReport>
  > WorkspaceDocumentDiagnosticReport;

  /**
   * A partial result for a workspace diagnostic report.
   *
   * @since 3.17.0
   */
  struct WorkspaceDiagnosticReportPartialResult
  {
    std::vector<WorkspaceDocumentDiagnosticReport> items;
  };

  /**
   * A workspace diagnostic report.
   *
   * @since 3.17.0
   */
  struct WorkspaceDiagnosticReport
  {
    std::vector<WorkspaceDocumentDiagnosticReport> items;
  };

  struct TextDocumentContentChangeEvent_1
  {
    string_t text;
  };

  struct TextDocumentContentChangeEvent_0
  {
    std::unique_ptr<Range> range;
    std::optional<uinteger_t> rangeLength;
    string_t text;
  };

  enum class TextDocumentContentChangeEventType {
    TEXT_DOCUMENT_CONTENT_CHANGE_EVENT_0,
    TEXT_DOCUMENT_CONTENT_CHANGE_EVENT_1,
  };

  extern std::map<TextDocumentContentChangeEventType, std::string> TextDocumentContentChangeEventTypeNames;

  typedef std::variant<
    std::unique_ptr<TextDocumentContentChangeEvent_0>,
    std::unique_ptr<TextDocumentContentChangeEvent_1>
  > TextDocumentContentChangeEvent;

  struct NotebookDocumentChangeEvent_cells_textContent
  {
    std::unique_ptr<VersionedTextDocumentIdentifier> document;
    std::vector<TextDocumentContentChangeEvent> changes;
  };

  struct NotebookDocumentChangeEvent_cells_structure
  {
    std::unique_ptr<NotebookCellArrayChange> array;
    std::optional<std::vector<std::unique_ptr<TextDocumentItem>>> didOpen;
    std::optional<std::vector<std::unique_ptr<TextDocumentIdentifier>>> didClose;
  };

  struct NotebookDocumentChangeEvent_cells
  {
    std::optional<std::unique_ptr<NotebookDocumentChangeEvent_cells_structure>> structure;
    std::optional<std::vector<std::unique_ptr<NotebookCell>>> data;
    std::optional<std::vector<std::unique_ptr<NotebookDocumentChangeEvent_cells_textContent>>> textContent;
  };

  /**
   * A change event for a notebook document.
   *
   * @since 3.17.0
   */
  struct NotebookDocumentChangeEvent
  {
    /**
     * The changed meta data if any.
     *
     * Note: should always be an object literal (e.g. LSPObject)
     */
    std::optional<LSPObject> metadata;
    /**
     * Changes to cells
     */
    std::optional<std::unique_ptr<NotebookDocumentChangeEvent_cells>> cells;
  };

  /**
   * The params sent in a change notebook document notification.
   *
   * @since 3.17.0
   */
  struct DidChangeNotebookDocumentParams
  {
    /**
     * The notebook document that did change. The version number points
     * to the version after all provided changes have been applied. If
     * only the text document content of a cell changes the notebook version
     * doesn't necessarily have to change.
     */
    std::unique_ptr<VersionedNotebookDocumentIdentifier> notebookDocument;
    /**
     * The actual changes to the notebook document.
     *
     * The changes describe single state changes to the notebook document.
     * So if there are two changes c1 (at array index 0) and c2 (at array
     * index 1) for a notebook in state S then c1 moves the notebook from
     * S to S' and c2 from S' to S''. So c1 is computed on the state S and
     * c2 is computed on the state S'.
     *
     * To mirror the content of a notebook using change events use the following approach:
     * - start with the same initial content
     * - apply the 'notebookDocument/didChange' notifications in the order you receive them.
     * - apply the `NotebookChangeEvent`s in a single notification in the order
     *   you receive them.
     */
    std::unique_ptr<NotebookDocumentChangeEvent> change;
  };

  /**
   * The change text document notification's parameters.
   */
  struct DidChangeTextDocumentParams
  {
    /**
     * The document that did change. The version number points
     * to the version after all provided content changes have
     * been applied.
     */
    std::unique_ptr<VersionedTextDocumentIdentifier> textDocument;
    /**
     * The actual content changes. The content changes describe single state changes
     * to the document. So if there are two content changes c1 (at array index 0) and
     * c2 (at array index 1) for a document in state S then c1 moves the document from
     * S to S' and c2 from S' to S''. So c1 is computed on the state S and c2 is computed
     * on the state S'.
     *
     * To mirror the content of a document using change events use the following approach:
     * - start with the same initial content
     * - apply the 'textDocument/didChange' notifications in the order you receive them.
     * - apply the `TextDocumentContentChangeEvent`s in a single notification in the order
     *   you receive them.
     */
    std::vector<TextDocumentContentChangeEvent> contentChanges;
  };

  struct MarkedString_1
  {
    string_t language;
    string_t value;
  };

  enum class MarkedStringType {
    STRING_TYPE,
    MARKED_STRING_1,
  };

  extern std::map<MarkedStringType, std::string> MarkedStringTypeNames;

  typedef std::variant<
    string_t,
    std::unique_ptr<MarkedString_1>
  > MarkedString;

  enum class Hover_contentsType {
    MARKUP_CONTENT,
    MARKED_STRING,
    MARKED_STRING_ARRAY,
  };

  extern std::map<Hover_contentsType, std::string> Hover_contentsTypeNames;

  typedef std::variant<
    std::unique_ptr<MarkupContent>,
    MarkedString,
    std::vector<MarkedString>
  > Hover_contents;

  /**
   * The result of a hover request.
   */
  struct Hover
  {
    /**
     * The hover's content
     */
    Hover_contents contents;
    /**
     * An optional range inside the text document that is used to
     * visualize the hover, e.g. by changing the background color.
     */
    std::optional<std::unique_ptr<Range>> range;
  };

  struct TextDocumentFilter_2
  {
    std::optional<string_t> language;
    std::optional<string_t> scheme;
    string_t pattern;
  };

  struct TextDocumentFilter_1
  {
    std::optional<string_t> language;
    string_t scheme;
    std::optional<string_t> pattern;
  };

  struct TextDocumentFilter_0
  {
    string_t language;
    std::optional<string_t> scheme;
    std::optional<string_t> pattern;
  };

  enum class TextDocumentFilterType {
    TEXT_DOCUMENT_FILTER_0,
    TEXT_DOCUMENT_FILTER_1,
    TEXT_DOCUMENT_FILTER_2,
  };

  extern std::map<TextDocumentFilterType, std::string> TextDocumentFilterTypeNames;

  typedef std::variant<
    std::unique_ptr<TextDocumentFilter_0>,
    std::unique_ptr<TextDocumentFilter_1>,
    std::unique_ptr<TextDocumentFilter_2>
  > TextDocumentFilter;

  struct NotebookDocumentFilter_2
  {
    std::optional<string_t> notebookType;
    std::optional<string_t> scheme;
    string_t pattern;
  };

  struct NotebookDocumentFilter_1
  {
    std::optional<string_t> notebookType;
    string_t scheme;
    std::optional<string_t> pattern;
  };

  struct NotebookDocumentFilter_0
  {
    string_t notebookType;
    std::optional<string_t> scheme;
    std::optional<string_t> pattern;
  };

  enum class NotebookDocumentFilterType {
    NOTEBOOK_DOCUMENT_FILTER_0,
    NOTEBOOK_DOCUMENT_FILTER_1,
    NOTEBOOK_DOCUMENT_FILTER_2,
  };

  extern std::map<NotebookDocumentFilterType, std::string> NotebookDocumentFilterTypeNames;

  typedef std::variant<
    std::unique_ptr<NotebookDocumentFilter_0>,
    std::unique_ptr<NotebookDocumentFilter_1>,
    std::unique_ptr<NotebookDocumentFilter_2>
  > NotebookDocumentFilter;

  enum class NotebookCellTextDocumentFilter_notebookType {
    STRING_TYPE,
    NOTEBOOK_DOCUMENT_FILTER,
  };

  extern std::map<NotebookCellTextDocumentFilter_notebookType, std::string> NotebookCellTextDocumentFilter_notebookTypeNames;

  typedef std::variant<
    string_t,
    NotebookDocumentFilter
  > NotebookCellTextDocumentFilter_notebook;

  /**
   * A notebook cell text document filter denotes a cell text
   * document by different properties.
   *
   * @since 3.17.0
   */
  struct NotebookCellTextDocumentFilter
  {
    /**
     * A filter that matches against the notebook
     * containing the notebook cell. If a string
     * value is provided it matches against the
     * notebook type. '*' matches every notebook.
     */
    NotebookCellTextDocumentFilter_notebook notebook;
    /**
     * A language id like `python`.
     *
     * Will be matched against the language id of the
     * notebook cell document. '*' matches every language.
     */
    std::optional<string_t> language;
  };

  enum class DocumentFilterType {
    TEXT_DOCUMENT_FILTER,
    NOTEBOOK_CELL_TEXT_DOCUMENT_FILTER,
  };

  extern std::map<DocumentFilterType, std::string> DocumentFilterTypeNames;

  typedef std::variant<
    TextDocumentFilter,
    std::unique_ptr<NotebookCellTextDocumentFilter>
  > DocumentFilter;

  /**
   * A document selector is the combination of one or many document filters.
   *
   * @sample `let sel:DocumentSelector = [{ language: 'typescript' }, { language: 'json', pattern: '**∕tsconfig.json' }]`;
   *
   * The use of a string as a document filter is deprecated @since 3.16.0.
   */
  typedef std::vector<DocumentFilter> DocumentSelector;

  enum class TextDocumentRegistrationOptions_documentSelectorType {
    DOCUMENT_SELECTOR,
    NULL_TYPE,
  };

  extern std::map<TextDocumentRegistrationOptions_documentSelectorType, std::string> TextDocumentRegistrationOptions_documentSelectorTypeNames;

  typedef std::variant<
    DocumentSelector,
    null_t
  > TextDocumentRegistrationOptions_documentSelector;

  /**
   * General text document registration options.
   */
  struct TextDocumentRegistrationOptions
  {
    /**
     * A document selector to identify the scope of the registration. If set to null
     * the document selector provided on the client side will be used.
     */
    TextDocumentRegistrationOptions_documentSelector documentSelector;
  };

  struct DeclarationRegistrationOptions
    : public DeclarationOptions
    , public TextDocumentRegistrationOptions
  {
    /**
     * The id used to register the request. The id can be used to deregister
     * the request again. See also Registration#id.
     */
    std::optional<string_t> id;
  };

  /**
   * Registration options for a {@link HoverRequest}.
   */
  struct HoverRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public HoverOptions
  {
  };

  /**
   * Inlay hint options used during static or dynamic registration.
   *
   * @since 3.17.0
   */
  struct InlayHintRegistrationOptions
    : public InlayHintOptions
    , public TextDocumentRegistrationOptions
  {
    /**
     * The id used to register the request. The id can be used to deregister
     * the request again. See also Registration#id.
     */
    std::optional<string_t> id;
  };

  /**
   * Save registration options.
   */
  struct TextDocumentSaveRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public SaveOptions
  {
  };

  struct SelectionRangeRegistrationOptions
    : public SelectionRangeOptions
    , public TextDocumentRegistrationOptions
  {
    /**
     * The id used to register the request. The id can be used to deregister
     * the request again. See also Registration#id.
     */
    std::optional<string_t> id;
  };

  struct LinkedEditingRangeRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public LinkedEditingRangeOptions
  {
    /**
     * The id used to register the request. The id can be used to deregister
     * the request again. See also Registration#id.
     */
    std::optional<string_t> id;
  };

  struct ImplementationRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public ImplementationOptions
  {
    /**
     * The id used to register the request. The id can be used to deregister
     * the request again. See also Registration#id.
     */
    std::optional<string_t> id;
  };

  /**
   * Registration options for a {@link DefinitionRequest}.
   */
  struct DefinitionRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public DefinitionOptions
  {
  };

  /**
   * Describe options to be used when registered for text document change events.
   */
  struct TextDocumentChangeRegistrationOptions
    : public TextDocumentRegistrationOptions
  {
    /**
     * How documents are synced to the server.
     */
    TextDocumentSyncKind syncKind;
  };

  struct MonikerRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public MonikerOptions
  {
  };

  /**
   * @since 3.16.0
   */
  struct SemanticTokensRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public SemanticTokensOptions
  {
    /**
     * The id used to register the request. The id can be used to deregister
     * the request again. See also Registration#id.
     */
    std::optional<string_t> id;
  };

  /**
   * Registration options for a {@link CodeActionRequest}.
   */
  struct CodeActionRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public CodeActionOptions
  {
  };

  /**
   * Registration options for a {@link CompletionRequest}.
   */
  struct CompletionRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public CompletionOptions
  {
  };

  /**
   * Inline completion options used during static or dynamic registration.
   *
   * @since 3.18.0
   * @proposed
   */
  struct InlineCompletionRegistrationOptions
    : public InlineCompletionOptions
    , public TextDocumentRegistrationOptions
  {
    /**
     * The id used to register the request. The id can be used to deregister
     * the request again. See also Registration#id.
     */
    std::optional<string_t> id;
  };

  /**
   * Type hierarchy options used during static or dynamic registration.
   *
   * @since 3.17.0
   */
  struct TypeHierarchyRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public TypeHierarchyOptions
  {
    /**
     * The id used to register the request. The id can be used to deregister
     * the request again. See also Registration#id.
     */
    std::optional<string_t> id;
  };

  /**
   * Registration options for a {@link DocumentLinkRequest}.
   */
  struct DocumentLinkRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public DocumentLinkOptions
  {
  };

  /**
   * Registration options for a {@link RenameRequest}.
   */
  struct RenameRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public RenameOptions
  {
  };

  /**
   * Registration options for a {@link CodeLensRequest}.
   */
  struct CodeLensRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public CodeLensOptions
  {
  };

  /**
   * Registration options for a {@link DocumentFormattingRequest}.
   */
  struct DocumentFormattingRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public DocumentFormattingOptions
  {
  };

  /**
   * Registration options for a {@link DocumentOnTypeFormattingRequest}.
   */
  struct DocumentOnTypeFormattingRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public DocumentOnTypeFormattingOptions
  {
  };

  /**
   * Call hierarchy options used during static or dynamic registration.
   *
   * @since 3.16.0
   */
  struct CallHierarchyRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public CallHierarchyOptions
  {
    /**
     * The id used to register the request. The id can be used to deregister
     * the request again. See also Registration#id.
     */
    std::optional<string_t> id;
  };

  /**
   * Diagnostic registration options.
   *
   * @since 3.17.0
   */
  struct DiagnosticRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public DiagnosticOptions
  {
    /**
     * The id used to register the request. The id can be used to deregister
     * the request again. See also Registration#id.
     */
    std::optional<string_t> id;
  };

  /**
   * Registration options for a {@link DocumentSymbolRequest}.
   */
  struct DocumentSymbolRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public DocumentSymbolOptions
  {
  };

  /**
   * Registration options for a {@link ReferencesRequest}.
   */
  struct ReferenceRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public ReferenceOptions
  {
  };

  /**
   * Registration options for a {@link DocumentRangeFormattingRequest}.
   */
  struct DocumentRangeFormattingRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public DocumentRangeFormattingOptions
  {
  };

  struct DocumentColorRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public DocumentColorOptions
  {
    /**
     * The id used to register the request. The id can be used to deregister
     * the request again. See also Registration#id.
     */
    std::optional<string_t> id;
  };

  /**
   * Registration options for a {@link SignatureHelpRequest}.
   */
  struct SignatureHelpRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public SignatureHelpOptions
  {
  };

  /**
   * Registration options for a {@link DocumentHighlightRequest}.
   */
  struct DocumentHighlightRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public DocumentHighlightOptions
  {
  };

  struct FoldingRangeRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public FoldingRangeOptions
  {
    /**
     * The id used to register the request. The id can be used to deregister
     * the request again. See also Registration#id.
     */
    std::optional<string_t> id;
  };

  /**
   * Inline value options used during static or dynamic registration.
   *
   * @since 3.17.0
   */
  struct InlineValueRegistrationOptions
    : public InlineValueOptions
    , public TextDocumentRegistrationOptions
  {
    /**
     * The id used to register the request. The id can be used to deregister
     * the request again. See also Registration#id.
     */
    std::optional<string_t> id;
  };

  struct TypeDefinitionRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public TypeDefinitionOptions
  {
    /**
     * The id used to register the request. The id can be used to deregister
     * the request again. See also Registration#id.
     */
    std::optional<string_t> id;
  };

  struct NotebookDocumentSyncOptions_notebookSelector_1_cells
  {
    string_t language;
  };

  enum class NotebookDocumentSyncOptions_notebookSelector_1_notebookType {
    STRING_TYPE,
    NOTEBOOK_DOCUMENT_FILTER,
  };

  extern std::map<NotebookDocumentSyncOptions_notebookSelector_1_notebookType, std::string> NotebookDocumentSyncOptions_notebookSelector_1_notebookTypeNames;

  typedef std::variant<
    string_t,
    NotebookDocumentFilter
  > NotebookDocumentSyncOptions_notebookSelector_1_notebook;

  struct NotebookDocumentSyncOptions_notebookSelector_1
  {
    std::optional<NotebookDocumentSyncOptions_notebookSelector_1_notebook> notebook;
    std::vector<std::unique_ptr<NotebookDocumentSyncOptions_notebookSelector_1_cells>> cells;
  };

  struct NotebookDocumentSyncOptions_notebookSelector_0_cells
  {
    string_t language;
  };

  enum class NotebookDocumentSyncOptions_notebookSelector_0_notebookType {
    STRING_TYPE,
    NOTEBOOK_DOCUMENT_FILTER,
  };

  extern std::map<NotebookDocumentSyncOptions_notebookSelector_0_notebookType, std::string> NotebookDocumentSyncOptions_notebookSelector_0_notebookTypeNames;

  typedef std::variant<
    string_t,
    NotebookDocumentFilter
  > NotebookDocumentSyncOptions_notebookSelector_0_notebook;

  struct NotebookDocumentSyncOptions_notebookSelector_0
  {
    NotebookDocumentSyncOptions_notebookSelector_0_notebook notebook;
    std::optional<std::vector<std::unique_ptr<NotebookDocumentSyncOptions_notebookSelector_0_cells>>> cells;
  };

  enum class NotebookDocumentSyncOptions_notebookSelectorType {
    NOTEBOOK_DOCUMENT_SYNC_OPTIONS_NOTEBOOK_SELECTOR_0,
    NOTEBOOK_DOCUMENT_SYNC_OPTIONS_NOTEBOOK_SELECTOR_1,
  };

  extern std::map<NotebookDocumentSyncOptions_notebookSelectorType, std::string> NotebookDocumentSyncOptions_notebookSelectorTypeNames;

  typedef std::variant<
    std::unique_ptr<NotebookDocumentSyncOptions_notebookSelector_0>,
    std::unique_ptr<NotebookDocumentSyncOptions_notebookSelector_1>
  > NotebookDocumentSyncOptions_notebookSelector;

  /**
   * Options specific to a notebook plus its cells
   * to be synced to the server.
   *
   * If a selector provides a notebook document
   * filter but no cell selector all cells of a
   * matching notebook document will be synced.
   *
   * If a selector provides no notebook document
   * filter but only a cell selector all notebook
   * document that contain at least one matching
   * cell will be synced.
   *
   * @since 3.17.0
   */
  struct NotebookDocumentSyncOptions
  {
    /**
     * The notebooks to be synced
     */
    std::vector<NotebookDocumentSyncOptions_notebookSelector> notebookSelector;
    /**
     * Whether save notification should be forwarded to
     * the server. Will only be honored if mode === `notebook`.
     */
    std::optional<boolean_t> save;
  };

  /**
   * Registration options specific to a notebook.
   *
   * @since 3.17.0
   */
  struct NotebookDocumentSyncRegistrationOptions
    : public NotebookDocumentSyncOptions
  {
    /**
     * The id used to register the request. The id can be used to deregister
     * the request again. See also Registration#id.
     */
    std::optional<string_t> id;
  };

  enum class ServerCapabilities_textDocumentSyncType {
    TEXT_DOCUMENT_SYNC_OPTIONS,
    TEXT_DOCUMENT_SYNC_KIND,
  };

  extern std::map<ServerCapabilities_textDocumentSyncType, std::string> ServerCapabilities_textDocumentSyncTypeNames;

  typedef std::variant<
    std::unique_ptr<TextDocumentSyncOptions>,
    TextDocumentSyncKind
  > ServerCapabilities_textDocumentSync;

  enum class ServerCapabilities_notebookDocumentSyncType {
    NOTEBOOK_DOCUMENT_SYNC_OPTIONS,
    NOTEBOOK_DOCUMENT_SYNC_REGISTRATION_OPTIONS,
  };

  extern std::map<ServerCapabilities_notebookDocumentSyncType, std::string> ServerCapabilities_notebookDocumentSyncTypeNames;

  typedef std::variant<
    std::unique_ptr<NotebookDocumentSyncOptions>,
    std::unique_ptr<NotebookDocumentSyncRegistrationOptions>
  > ServerCapabilities_notebookDocumentSync;

  enum class ServerCapabilities_hoverProviderType {
    BOOLEAN_TYPE,
    HOVER_OPTIONS,
  };

  extern std::map<ServerCapabilities_hoverProviderType, std::string> ServerCapabilities_hoverProviderTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<HoverOptions>
  > ServerCapabilities_hoverProvider;

  enum class ServerCapabilities_declarationProviderType {
    BOOLEAN_TYPE,
    DECLARATION_OPTIONS,
    DECLARATION_REGISTRATION_OPTIONS,
  };

  extern std::map<ServerCapabilities_declarationProviderType, std::string> ServerCapabilities_declarationProviderTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<DeclarationOptions>,
    std::unique_ptr<DeclarationRegistrationOptions>
  > ServerCapabilities_declarationProvider;

  enum class ServerCapabilities_definitionProviderType {
    BOOLEAN_TYPE,
    DEFINITION_OPTIONS,
  };

  extern std::map<ServerCapabilities_definitionProviderType, std::string> ServerCapabilities_definitionProviderTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<DefinitionOptions>
  > ServerCapabilities_definitionProvider;

  enum class ServerCapabilities_typeDefinitionProviderType {
    BOOLEAN_TYPE,
    TYPE_DEFINITION_OPTIONS,
    TYPE_DEFINITION_REGISTRATION_OPTIONS,
  };

  extern std::map<ServerCapabilities_typeDefinitionProviderType, std::string> ServerCapabilities_typeDefinitionProviderTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<TypeDefinitionOptions>,
    std::unique_ptr<TypeDefinitionRegistrationOptions>
  > ServerCapabilities_typeDefinitionProvider;

  enum class ServerCapabilities_implementationProviderType {
    BOOLEAN_TYPE,
    IMPLEMENTATION_OPTIONS,
    IMPLEMENTATION_REGISTRATION_OPTIONS,
  };

  extern std::map<ServerCapabilities_implementationProviderType, std::string> ServerCapabilities_implementationProviderTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<ImplementationOptions>,
    std::unique_ptr<ImplementationRegistrationOptions>
  > ServerCapabilities_implementationProvider;

  enum class ServerCapabilities_referencesProviderType {
    BOOLEAN_TYPE,
    REFERENCE_OPTIONS,
  };

  extern std::map<ServerCapabilities_referencesProviderType, std::string> ServerCapabilities_referencesProviderTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<ReferenceOptions>
  > ServerCapabilities_referencesProvider;

  enum class ServerCapabilities_documentHighlightProviderType {
    BOOLEAN_TYPE,
    DOCUMENT_HIGHLIGHT_OPTIONS,
  };

  extern std::map<ServerCapabilities_documentHighlightProviderType, std::string> ServerCapabilities_documentHighlightProviderTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<DocumentHighlightOptions>
  > ServerCapabilities_documentHighlightProvider;

  enum class ServerCapabilities_documentSymbolProviderType {
    BOOLEAN_TYPE,
    DOCUMENT_SYMBOL_OPTIONS,
  };

  extern std::map<ServerCapabilities_documentSymbolProviderType, std::string> ServerCapabilities_documentSymbolProviderTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<DocumentSymbolOptions>
  > ServerCapabilities_documentSymbolProvider;

  enum class ServerCapabilities_codeActionProviderType {
    BOOLEAN_TYPE,
    CODE_ACTION_OPTIONS,
  };

  extern std::map<ServerCapabilities_codeActionProviderType, std::string> ServerCapabilities_codeActionProviderTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<CodeActionOptions>
  > ServerCapabilities_codeActionProvider;

  enum class ServerCapabilities_colorProviderType {
    BOOLEAN_TYPE,
    DOCUMENT_COLOR_OPTIONS,
    DOCUMENT_COLOR_REGISTRATION_OPTIONS,
  };

  extern std::map<ServerCapabilities_colorProviderType, std::string> ServerCapabilities_colorProviderTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<DocumentColorOptions>,
    std::unique_ptr<DocumentColorRegistrationOptions>
  > ServerCapabilities_colorProvider;

  enum class ServerCapabilities_workspaceSymbolProviderType {
    BOOLEAN_TYPE,
    WORKSPACE_SYMBOL_OPTIONS,
  };

  extern std::map<ServerCapabilities_workspaceSymbolProviderType, std::string> ServerCapabilities_workspaceSymbolProviderTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<WorkspaceSymbolOptions>
  > ServerCapabilities_workspaceSymbolProvider;

  enum class ServerCapabilities_documentFormattingProviderType {
    BOOLEAN_TYPE,
    DOCUMENT_FORMATTING_OPTIONS,
  };

  extern std::map<ServerCapabilities_documentFormattingProviderType, std::string> ServerCapabilities_documentFormattingProviderTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<DocumentFormattingOptions>
  > ServerCapabilities_documentFormattingProvider;

  enum class ServerCapabilities_documentRangeFormattingProviderType {
    BOOLEAN_TYPE,
    DOCUMENT_RANGE_FORMATTING_OPTIONS,
  };

  extern std::map<ServerCapabilities_documentRangeFormattingProviderType, std::string> ServerCapabilities_documentRangeFormattingProviderTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<DocumentRangeFormattingOptions>
  > ServerCapabilities_documentRangeFormattingProvider;

  enum class ServerCapabilities_renameProviderType {
    BOOLEAN_TYPE,
    RENAME_OPTIONS,
  };

  extern std::map<ServerCapabilities_renameProviderType, std::string> ServerCapabilities_renameProviderTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<RenameOptions>
  > ServerCapabilities_renameProvider;

  enum class ServerCapabilities_foldingRangeProviderType {
    BOOLEAN_TYPE,
    FOLDING_RANGE_OPTIONS,
    FOLDING_RANGE_REGISTRATION_OPTIONS,
  };

  extern std::map<ServerCapabilities_foldingRangeProviderType, std::string> ServerCapabilities_foldingRangeProviderTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<FoldingRangeOptions>,
    std::unique_ptr<FoldingRangeRegistrationOptions>
  > ServerCapabilities_foldingRangeProvider;

  enum class ServerCapabilities_selectionRangeProviderType {
    BOOLEAN_TYPE,
    SELECTION_RANGE_OPTIONS,
    SELECTION_RANGE_REGISTRATION_OPTIONS,
  };

  extern std::map<ServerCapabilities_selectionRangeProviderType, std::string> ServerCapabilities_selectionRangeProviderTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<SelectionRangeOptions>,
    std::unique_ptr<SelectionRangeRegistrationOptions>
  > ServerCapabilities_selectionRangeProvider;

  enum class ServerCapabilities_callHierarchyProviderType {
    BOOLEAN_TYPE,
    CALL_HIERARCHY_OPTIONS,
    CALL_HIERARCHY_REGISTRATION_OPTIONS,
  };

  extern std::map<ServerCapabilities_callHierarchyProviderType, std::string> ServerCapabilities_callHierarchyProviderTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<CallHierarchyOptions>,
    std::unique_ptr<CallHierarchyRegistrationOptions>
  > ServerCapabilities_callHierarchyProvider;

  enum class ServerCapabilities_linkedEditingRangeProviderType {
    BOOLEAN_TYPE,
    LINKED_EDITING_RANGE_OPTIONS,
    LINKED_EDITING_RANGE_REGISTRATION_OPTIONS,
  };

  extern std::map<ServerCapabilities_linkedEditingRangeProviderType, std::string> ServerCapabilities_linkedEditingRangeProviderTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<LinkedEditingRangeOptions>,
    std::unique_ptr<LinkedEditingRangeRegistrationOptions>
  > ServerCapabilities_linkedEditingRangeProvider;

  enum class ServerCapabilities_semanticTokensProviderType {
    SEMANTIC_TOKENS_OPTIONS,
    SEMANTIC_TOKENS_REGISTRATION_OPTIONS,
  };

  extern std::map<ServerCapabilities_semanticTokensProviderType, std::string> ServerCapabilities_semanticTokensProviderTypeNames;

  typedef std::variant<
    std::unique_ptr<SemanticTokensOptions>,
    std::unique_ptr<SemanticTokensRegistrationOptions>
  > ServerCapabilities_semanticTokensProvider;

  enum class ServerCapabilities_monikerProviderType {
    BOOLEAN_TYPE,
    MONIKER_OPTIONS,
    MONIKER_REGISTRATION_OPTIONS,
  };

  extern std::map<ServerCapabilities_monikerProviderType, std::string> ServerCapabilities_monikerProviderTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<MonikerOptions>,
    std::unique_ptr<MonikerRegistrationOptions>
  > ServerCapabilities_monikerProvider;

  enum class ServerCapabilities_typeHierarchyProviderType {
    BOOLEAN_TYPE,
    TYPE_HIERARCHY_OPTIONS,
    TYPE_HIERARCHY_REGISTRATION_OPTIONS,
  };

  extern std::map<ServerCapabilities_typeHierarchyProviderType, std::string> ServerCapabilities_typeHierarchyProviderTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<TypeHierarchyOptions>,
    std::unique_ptr<TypeHierarchyRegistrationOptions>
  > ServerCapabilities_typeHierarchyProvider;

  enum class ServerCapabilities_inlineValueProviderType {
    BOOLEAN_TYPE,
    INLINE_VALUE_OPTIONS,
    INLINE_VALUE_REGISTRATION_OPTIONS,
  };

  extern std::map<ServerCapabilities_inlineValueProviderType, std::string> ServerCapabilities_inlineValueProviderTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<InlineValueOptions>,
    std::unique_ptr<InlineValueRegistrationOptions>
  > ServerCapabilities_inlineValueProvider;

  enum class ServerCapabilities_inlayHintProviderType {
    BOOLEAN_TYPE,
    INLAY_HINT_OPTIONS,
    INLAY_HINT_REGISTRATION_OPTIONS,
  };

  extern std::map<ServerCapabilities_inlayHintProviderType, std::string> ServerCapabilities_inlayHintProviderTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<InlayHintOptions>,
    std::unique_ptr<InlayHintRegistrationOptions>
  > ServerCapabilities_inlayHintProvider;

  enum class ServerCapabilities_diagnosticProviderType {
    DIAGNOSTIC_OPTIONS,
    DIAGNOSTIC_REGISTRATION_OPTIONS,
  };

  extern std::map<ServerCapabilities_diagnosticProviderType, std::string> ServerCapabilities_diagnosticProviderTypeNames;

  typedef std::variant<
    std::unique_ptr<DiagnosticOptions>,
    std::unique_ptr<DiagnosticRegistrationOptions>
  > ServerCapabilities_diagnosticProvider;

  enum class ServerCapabilities_inlineCompletionProviderType {
    BOOLEAN_TYPE,
    INLINE_COMPLETION_OPTIONS,
  };

  extern std::map<ServerCapabilities_inlineCompletionProviderType, std::string> ServerCapabilities_inlineCompletionProviderTypeNames;

  typedef std::variant<
    boolean_t,
    std::unique_ptr<InlineCompletionOptions>
  > ServerCapabilities_inlineCompletionProvider;

  struct ServerCapabilities_workspace
  {
    std::optional<std::unique_ptr<WorkspaceFoldersServerCapabilities>> workspaceFolders;
    std::optional<std::unique_ptr<FileOperationOptions>> fileOperations;
  };

  /**
   * Defines the capabilities provided by a language
   * server.
   */
  struct ServerCapabilities
  {
    /**
     * The position encoding the server picked from the encodings offered
     * by the client via the client capability `general.positionEncodings`.
     *
     * If the client didn't provide any position encodings the only valid
     * value that a server can return is 'utf-16'.
     *
     * If omitted it defaults to 'utf-16'.
     *
     * @since 3.17.0
     */
    std::optional<PositionEncodingKind> positionEncoding;
    /**
     * Defines how text documents are synced. Is either a detailed structure
     * defining each notification or for backwards compatibility the
     * TextDocumentSyncKind number.
     */
    std::optional<ServerCapabilities_textDocumentSync> textDocumentSync;
    /**
     * Defines how notebook documents are synced.
     *
     * @since 3.17.0
     */
    std::optional<ServerCapabilities_notebookDocumentSync> notebookDocumentSync;
    /**
     * The server provides completion support.
     */
    std::optional<std::unique_ptr<CompletionOptions>> completionProvider;
    /**
     * The server provides hover support.
     */
    std::optional<ServerCapabilities_hoverProvider> hoverProvider;
    /**
     * The server provides signature help support.
     */
    std::optional<std::unique_ptr<SignatureHelpOptions>> signatureHelpProvider;
    /**
     * The server provides Goto Declaration support.
     */
    std::optional<ServerCapabilities_declarationProvider> declarationProvider;
    /**
     * The server provides goto definition support.
     */
    std::optional<ServerCapabilities_definitionProvider> definitionProvider;
    /**
     * The server provides Goto Type Definition support.
     */
    std::optional<ServerCapabilities_typeDefinitionProvider> typeDefinitionProvider;
    /**
     * The server provides Goto Implementation support.
     */
    std::optional<ServerCapabilities_implementationProvider> implementationProvider;
    /**
     * The server provides find references support.
     */
    std::optional<ServerCapabilities_referencesProvider> referencesProvider;
    /**
     * The server provides document highlight support.
     */
    std::optional<ServerCapabilities_documentHighlightProvider> documentHighlightProvider;
    /**
     * The server provides document symbol support.
     */
    std::optional<ServerCapabilities_documentSymbolProvider> documentSymbolProvider;
    /**
     * The server provides code actions. CodeActionOptions may only be
     * specified if the client states that it supports
     * `codeActionLiteralSupport` in its initial `initialize` request.
     */
    std::optional<ServerCapabilities_codeActionProvider> codeActionProvider;
    /**
     * The server provides code lens.
     */
    std::optional<std::unique_ptr<CodeLensOptions>> codeLensProvider;
    /**
     * The server provides document link support.
     */
    std::optional<std::unique_ptr<DocumentLinkOptions>> documentLinkProvider;
    /**
     * The server provides color provider support.
     */
    std::optional<ServerCapabilities_colorProvider> colorProvider;
    /**
     * The server provides workspace symbol support.
     */
    std::optional<ServerCapabilities_workspaceSymbolProvider> workspaceSymbolProvider;
    /**
     * The server provides document formatting.
     */
    std::optional<ServerCapabilities_documentFormattingProvider> documentFormattingProvider;
    /**
     * The server provides document range formatting.
     */
    std::optional<ServerCapabilities_documentRangeFormattingProvider> documentRangeFormattingProvider;
    /**
     * The server provides document formatting on typing.
     */
    std::optional<std::unique_ptr<DocumentOnTypeFormattingOptions>> documentOnTypeFormattingProvider;
    /**
     * The server provides rename support. RenameOptions may only be
     * specified if the client states that it supports
     * `prepareSupport` in its initial `initialize` request.
     */
    std::optional<ServerCapabilities_renameProvider> renameProvider;
    /**
     * The server provides folding provider support.
     */
    std::optional<ServerCapabilities_foldingRangeProvider> foldingRangeProvider;
    /**
     * The server provides selection range support.
     */
    std::optional<ServerCapabilities_selectionRangeProvider> selectionRangeProvider;
    /**
     * The server provides execute command support.
     */
    std::optional<std::unique_ptr<ExecuteCommandOptions>> executeCommandProvider;
    /**
     * The server provides call hierarchy support.
     *
     * @since 3.16.0
     */
    std::optional<ServerCapabilities_callHierarchyProvider> callHierarchyProvider;
    /**
     * The server provides linked editing range support.
     *
     * @since 3.16.0
     */
    std::optional<ServerCapabilities_linkedEditingRangeProvider> linkedEditingRangeProvider;
    /**
     * The server provides semantic tokens support.
     *
     * @since 3.16.0
     */
    std::optional<ServerCapabilities_semanticTokensProvider> semanticTokensProvider;
    /**
     * The server provides moniker support.
     *
     * @since 3.16.0
     */
    std::optional<ServerCapabilities_monikerProvider> monikerProvider;
    /**
     * The server provides type hierarchy support.
     *
     * @since 3.17.0
     */
    std::optional<ServerCapabilities_typeHierarchyProvider> typeHierarchyProvider;
    /**
     * The server provides inline values.
     *
     * @since 3.17.0
     */
    std::optional<ServerCapabilities_inlineValueProvider> inlineValueProvider;
    /**
     * The server provides inlay hints.
     *
     * @since 3.17.0
     */
    std::optional<ServerCapabilities_inlayHintProvider> inlayHintProvider;
    /**
     * The server has support for pull model diagnostics.
     *
     * @since 3.17.0
     */
    std::optional<ServerCapabilities_diagnosticProvider> diagnosticProvider;
    /**
     * Inline completion options used during static registration.
     *
     * @since 3.18.0
     * @proposed
     */
    std::optional<ServerCapabilities_inlineCompletionProvider> inlineCompletionProvider;
    /**
     * Workspace specific server capabilities.
     */
    std::optional<std::unique_ptr<ServerCapabilities_workspace>> workspace;
    /**
     * Experimental server capabilities.
     */
    std::optional<std::unique_ptr<LSPAny>> experimental;
  };

  struct InitializeResult_serverInfo
  {
    string_t name;
    std::optional<string_t> version;
  };

  /**
   * The result returned from an initialize request.
   */
  struct InitializeResult
  {
    /**
     * The capabilities the language server provides.
     */
    std::unique_ptr<ServerCapabilities> capabilities;
    /**
     * Information about the server.
     *
     * @since 3.15.0
     */
    std::optional<std::unique_ptr<InitializeResult_serverInfo>> serverInfo;
  };

  /**
   * The glob pattern to watch relative to the base path. Glob patterns can have the following syntax:
   * - `*` to match one or more characters in a path segment
   * - `?` to match on one character in a path segment
   * - `**` to match any number of path segments, including none
   * - `{}` to group conditions (e.g. `**​/​*.{ts,js}` matches all TypeScript and JavaScript files)
   * - `[]` to declare a range of characters to match in a path segment (e.g., `example.[0-9]` to match on `example.0`, `example.1`, …)
   * - `[!...]` to negate a range of characters to match in a path segment (e.g., `example.[!0-9]` to match on `example.a`, `example.b`, but not `example.0`)
   *
   * @since 3.17.0
   */
  typedef string_t Pattern;

  enum class RelativePattern_baseUriType {
    WORKSPACE_FOLDER,
    URI,
  };

  extern std::map<RelativePattern_baseUriType, std::string> RelativePattern_baseUriTypeNames;

  typedef std::variant<
    std::unique_ptr<WorkspaceFolder>,
    URI
  > RelativePattern_baseUri;

  /**
   * A relative pattern is a helper to construct glob patterns that are matched
   * relatively to a base URI. The common value for a `baseUri` is a workspace
   * folder root, but it can be another absolute URI as well.
   *
   * @since 3.17.0
   */
  struct RelativePattern
  {
    /**
     * A workspace folder or a base URI to which this pattern will be matched
     * against relatively.
     */
    RelativePattern_baseUri baseUri;
    /**
     * The actual glob pattern;
     */
    Pattern pattern;
  };

  enum class GlobPatternType {
    PATTERN,
    RELATIVE_PATTERN,
  };

  extern std::map<GlobPatternType, std::string> GlobPatternTypeNames;

  typedef std::variant<
    Pattern,
    std::unique_ptr<RelativePattern>
  > GlobPattern;

  struct FileSystemWatcher
  {
    /**
     * The glob pattern to watch. See {@link GlobPattern glob pattern} for more detail.
     *
     * @since 3.17.0 support for relative patterns.
     */
    GlobPattern globPattern;
    /**
     * The kind of events of interest. If omitted it defaults
     * to WatchKind.Create | WatchKind.Change | WatchKind.Delete
     * which is 7.
     */
    std::optional<WatchKind> kind;
  };

  /**
   * Describe options to be used when registered for text document change events.
   */
  struct DidChangeWatchedFilesRegistrationOptions
  {
    /**
     * The watchers to register.
     */
    std::vector<std::unique_ptr<FileSystemWatcher>> watchers;
  };

  enum class TextDocumentImplementationResultType {
    DEFINITION,
    DEFINITION_LINK_ARRAY,
    NULL_TYPE,
    LOCATION_ARRAY,
  };

  extern std::map<TextDocumentImplementationResultType, std::string> TextDocumentImplementationResultTypeNames;

  typedef std::variant<
    Definition,
    std::vector<std::unique_ptr<DefinitionLink>>,
    null_t,
    std::vector<std::unique_ptr<Location>>
  > TextDocumentImplementationResult;

  enum class TextDocumentTypeDefinitionResultType {
    DEFINITION,
    DEFINITION_LINK_ARRAY,
    NULL_TYPE,
    LOCATION_ARRAY,
  };

  extern std::map<TextDocumentTypeDefinitionResultType, std::string> TextDocumentTypeDefinitionResultTypeNames;

  typedef std::variant<
    Definition,
    std::vector<std::unique_ptr<DefinitionLink>>,
    null_t,
    std::vector<std::unique_ptr<Location>>
  > TextDocumentTypeDefinitionResult;

  enum class WorkspaceWorkspaceFoldersResultType {
    WORKSPACE_FOLDER_ARRAY,
    NULL_TYPE,
  };

  extern std::map<WorkspaceWorkspaceFoldersResultType, std::string> WorkspaceWorkspaceFoldersResultTypeNames;

  typedef std::variant<
    std::vector<std::unique_ptr<WorkspaceFolder>>,
    null_t
  > WorkspaceWorkspaceFoldersResult;

  typedef std::vector<std::unique_ptr<LSPAny>> WorkspaceConfigurationResult;

  typedef std::vector<std::unique_ptr<ColorInformation>> TextDocumentDocumentColorResult;

  typedef std::vector<std::unique_ptr<ColorPresentation>> TextDocumentColorPresentationResult;

  enum class TextDocumentFoldingRangeResultType {
    FOLDING_RANGE_ARRAY,
    NULL_TYPE,
  };

  extern std::map<TextDocumentFoldingRangeResultType, std::string> TextDocumentFoldingRangeResultTypeNames;

  typedef std::variant<
    std::vector<std::unique_ptr<FoldingRange>>,
    null_t
  > TextDocumentFoldingRangeResult;

  typedef null_t WorkspaceFoldingRangeRefreshResult;

  enum class TextDocumentDeclarationResultType {
    DECLARATION,
    DECLARATION_LINK_ARRAY,
    NULL_TYPE,
    LOCATION_ARRAY,
  };

  extern std::map<TextDocumentDeclarationResultType, std::string> TextDocumentDeclarationResultTypeNames;

  typedef std::variant<
    Declaration,
    std::vector<std::unique_ptr<DeclarationLink>>,
    null_t,
    std::vector<std::unique_ptr<Location>>
  > TextDocumentDeclarationResult;

  typedef null_t WindowWorkDoneProgressCreateResult;

  enum class TextDocumentPrepareCallHierarchyResultType {
    CALL_HIERARCHY_ITEM_ARRAY,
    NULL_TYPE,
  };

  extern std::map<TextDocumentPrepareCallHierarchyResultType, std::string> TextDocumentPrepareCallHierarchyResultTypeNames;

  typedef std::variant<
    std::vector<std::unique_ptr<CallHierarchyItem>>,
    null_t
  > TextDocumentPrepareCallHierarchyResult;

  enum class CallHierarchyIncomingCallsResultType {
    CALL_HIERARCHY_INCOMING_CALL_ARRAY,
    NULL_TYPE,
  };

  extern std::map<CallHierarchyIncomingCallsResultType, std::string> CallHierarchyIncomingCallsResultTypeNames;

  typedef std::variant<
    std::vector<std::unique_ptr<CallHierarchyIncomingCall>>,
    null_t
  > CallHierarchyIncomingCallsResult;

  enum class CallHierarchyOutgoingCallsResultType {
    CALL_HIERARCHY_OUTGOING_CALL_ARRAY,
    NULL_TYPE,
  };

  extern std::map<CallHierarchyOutgoingCallsResultType, std::string> CallHierarchyOutgoingCallsResultTypeNames;

  typedef std::variant<
    std::vector<std::unique_ptr<CallHierarchyOutgoingCall>>,
    null_t
  > CallHierarchyOutgoingCallsResult;

  enum class TextDocumentSemanticTokensFullResultType {
    SEMANTIC_TOKENS,
    NULL_TYPE,
    SEMANTIC_TOKENS_PARTIAL_RESULT,
  };

  extern std::map<TextDocumentSemanticTokensFullResultType, std::string> TextDocumentSemanticTokensFullResultTypeNames;

  typedef std::variant<
    std::unique_ptr<SemanticTokens>,
    null_t,
    std::unique_ptr<SemanticTokensPartialResult>
  > TextDocumentSemanticTokensFullResult;

  enum class TextDocumentSemanticTokensFullDeltaResultType {
    SEMANTIC_TOKENS,
    SEMANTIC_TOKENS_DELTA,
    NULL_TYPE,
    SEMANTIC_TOKENS_PARTIAL_RESULT,
    SEMANTIC_TOKENS_DELTA_PARTIAL_RESULT,
  };

  extern std::map<TextDocumentSemanticTokensFullDeltaResultType, std::string> TextDocumentSemanticTokensFullDeltaResultTypeNames;

  typedef std::variant<
    std::unique_ptr<SemanticTokens>,
    std::unique_ptr<SemanticTokensDelta>,
    null_t,
    std::unique_ptr<SemanticTokensPartialResult>,
    std::unique_ptr<SemanticTokensDeltaPartialResult>
  > TextDocumentSemanticTokensFullDeltaResult;

  enum class TextDocumentSemanticTokensRangeResultType {
    SEMANTIC_TOKENS,
    NULL_TYPE,
    SEMANTIC_TOKENS_PARTIAL_RESULT,
  };

  extern std::map<TextDocumentSemanticTokensRangeResultType, std::string> TextDocumentSemanticTokensRangeResultTypeNames;

  typedef std::variant<
    std::unique_ptr<SemanticTokens>,
    null_t,
    std::unique_ptr<SemanticTokensPartialResult>
  > TextDocumentSemanticTokensRangeResult;

  typedef null_t WorkspaceSemanticTokensRefreshResult;

  typedef ShowDocumentResult WindowShowDocumentResult;

  enum class TextDocumentLinkedEditingRangeResultType {
    LINKED_EDITING_RANGES,
    NULL_TYPE,
  };

  extern std::map<TextDocumentLinkedEditingRangeResultType, std::string> TextDocumentLinkedEditingRangeResultTypeNames;

  typedef std::variant<
    std::unique_ptr<LinkedEditingRanges>,
    null_t
  > TextDocumentLinkedEditingRangeResult;

  enum class WorkspaceWillCreateFilesResultType {
    WORKSPACE_EDIT,
    NULL_TYPE,
  };

  extern std::map<WorkspaceWillCreateFilesResultType, std::string> WorkspaceWillCreateFilesResultTypeNames;

  typedef std::variant<
    std::unique_ptr<WorkspaceEdit>,
    null_t
  > WorkspaceWillCreateFilesResult;

  enum class WorkspaceWillRenameFilesResultType {
    WORKSPACE_EDIT,
    NULL_TYPE,
  };

  extern std::map<WorkspaceWillRenameFilesResultType, std::string> WorkspaceWillRenameFilesResultTypeNames;

  typedef std::variant<
    std::unique_ptr<WorkspaceEdit>,
    null_t
  > WorkspaceWillRenameFilesResult;

  enum class WorkspaceWillDeleteFilesResultType {
    WORKSPACE_EDIT,
    NULL_TYPE,
  };

  extern std::map<WorkspaceWillDeleteFilesResultType, std::string> WorkspaceWillDeleteFilesResultTypeNames;

  typedef std::variant<
    std::unique_ptr<WorkspaceEdit>,
    null_t
  > WorkspaceWillDeleteFilesResult;

  enum class TextDocumentMonikerResultType {
    MONIKER_ARRAY,
    NULL_TYPE,
  };

  extern std::map<TextDocumentMonikerResultType, std::string> TextDocumentMonikerResultTypeNames;

  typedef std::variant<
    std::vector<std::unique_ptr<Moniker>>,
    null_t
  > TextDocumentMonikerResult;

  enum class TextDocumentPrepareTypeHierarchyResultType {
    TYPE_HIERARCHY_ITEM_ARRAY,
    NULL_TYPE,
  };

  extern std::map<TextDocumentPrepareTypeHierarchyResultType, std::string> TextDocumentPrepareTypeHierarchyResultTypeNames;

  typedef std::variant<
    std::vector<std::unique_ptr<TypeHierarchyItem>>,
    null_t
  > TextDocumentPrepareTypeHierarchyResult;

  enum class TypeHierarchySupertypesResultType {
    TYPE_HIERARCHY_ITEM_ARRAY,
    NULL_TYPE,
  };

  extern std::map<TypeHierarchySupertypesResultType, std::string> TypeHierarchySupertypesResultTypeNames;

  typedef std::variant<
    std::vector<std::unique_ptr<TypeHierarchyItem>>,
    null_t
  > TypeHierarchySupertypesResult;

  enum class TypeHierarchySubtypesResultType {
    TYPE_HIERARCHY_ITEM_ARRAY,
    NULL_TYPE,
  };

  extern std::map<TypeHierarchySubtypesResultType, std::string> TypeHierarchySubtypesResultTypeNames;

  typedef std::variant<
    std::vector<std::unique_ptr<TypeHierarchyItem>>,
    null_t
  > TypeHierarchySubtypesResult;

  enum class TextDocumentInlineValueResultType {
    INLINE_VALUE_ARRAY,
    NULL_TYPE,
  };

  extern std::map<TextDocumentInlineValueResultType, std::string> TextDocumentInlineValueResultTypeNames;

  typedef std::variant<
    std::vector<InlineValue>,
    null_t
  > TextDocumentInlineValueResult;

  typedef null_t WorkspaceInlineValueRefreshResult;

  enum class TextDocumentInlayHintResultType {
    INLAY_HINT_ARRAY,
    NULL_TYPE,
  };

  extern std::map<TextDocumentInlayHintResultType, std::string> TextDocumentInlayHintResultTypeNames;

  typedef std::variant<
    std::vector<std::unique_ptr<InlayHint>>,
    null_t
  > TextDocumentInlayHintResult;

  typedef InlayHint InlayHintResolveResult;

  typedef null_t WorkspaceInlayHintRefreshResult;

  enum class TextDocumentDiagnosticResultType {
    DOCUMENT_DIAGNOSTIC_REPORT,
    DOCUMENT_DIAGNOSTIC_REPORT_PARTIAL_RESULT,
  };

  extern std::map<TextDocumentDiagnosticResultType, std::string> TextDocumentDiagnosticResultTypeNames;

  typedef std::variant<
    DocumentDiagnosticReport,
    std::unique_ptr<DocumentDiagnosticReportPartialResult>
  > TextDocumentDiagnosticResult;

  enum class WorkspaceDiagnosticResultType {
    WORKSPACE_DIAGNOSTIC_REPORT,
    WORKSPACE_DIAGNOSTIC_REPORT_PARTIAL_RESULT,
  };

  extern std::map<WorkspaceDiagnosticResultType, std::string> WorkspaceDiagnosticResultTypeNames;

  typedef std::variant<
    std::unique_ptr<WorkspaceDiagnosticReport>,
    std::unique_ptr<WorkspaceDiagnosticReportPartialResult>
  > WorkspaceDiagnosticResult;

  typedef null_t WorkspaceDiagnosticRefreshResult;

  enum class TextDocumentInlineCompletionResultType {
    INLINE_COMPLETION_LIST,
    INLINE_COMPLETION_ITEM_ARRAY,
    NULL_TYPE,
  };

  extern std::map<TextDocumentInlineCompletionResultType, std::string> TextDocumentInlineCompletionResultTypeNames;

  typedef std::variant<
    std::unique_ptr<InlineCompletionList>,
    std::vector<std::unique_ptr<InlineCompletionItem>>,
    null_t
  > TextDocumentInlineCompletionResult;

  typedef null_t ClientRegisterCapabilityResult;

  typedef null_t ClientUnregisterCapabilityResult;

  typedef null_t ShutdownResult;

  enum class WindowShowMessageRequestResultType {
    MESSAGE_ACTION_ITEM,
    NULL_TYPE,
  };

  extern std::map<WindowShowMessageRequestResultType, std::string> WindowShowMessageRequestResultTypeNames;

  typedef std::variant<
    std::unique_ptr<MessageActionItem>,
    null_t
  > WindowShowMessageRequestResult;

  enum class TextDocumentWillSaveWaitUntilResultType {
    TEXT_EDIT_ARRAY,
    NULL_TYPE,
  };

  extern std::map<TextDocumentWillSaveWaitUntilResultType, std::string> TextDocumentWillSaveWaitUntilResultTypeNames;

  typedef std::variant<
    std::vector<std::unique_ptr<TextEdit>>,
    null_t
  > TextDocumentWillSaveWaitUntilResult;

  enum class TextDocumentCompletionResultType {
    COMPLETION_ITEM_ARRAY,
    COMPLETION_LIST,
    NULL_TYPE,
  };

  extern std::map<TextDocumentCompletionResultType, std::string> TextDocumentCompletionResultTypeNames;

  typedef std::variant<
    std::vector<std::unique_ptr<CompletionItem>>,
    std::unique_ptr<CompletionList>,
    null_t
  > TextDocumentCompletionResult;

  typedef CompletionItem CompletionItemResolveResult;

  enum class TextDocumentHoverResultType {
    HOVER,
    NULL_TYPE,
  };

  extern std::map<TextDocumentHoverResultType, std::string> TextDocumentHoverResultTypeNames;

  typedef std::variant<
    std::unique_ptr<Hover>,
    null_t
  > TextDocumentHoverResult;

  enum class TextDocumentSignatureHelpResultType {
    SIGNATURE_HELP,
    NULL_TYPE,
  };

  extern std::map<TextDocumentSignatureHelpResultType, std::string> TextDocumentSignatureHelpResultTypeNames;

  typedef std::variant<
    std::unique_ptr<SignatureHelp>,
    null_t
  > TextDocumentSignatureHelpResult;

  enum class TextDocumentDefinitionResultType {
    DEFINITION,
    DEFINITION_LINK_ARRAY,
    NULL_TYPE,
    LOCATION_ARRAY,
  };

  extern std::map<TextDocumentDefinitionResultType, std::string> TextDocumentDefinitionResultTypeNames;

  typedef std::variant<
    Definition,
    std::vector<std::unique_ptr<DefinitionLink>>,
    null_t,
    std::vector<std::unique_ptr<Location>>
  > TextDocumentDefinitionResult;

  enum class TextDocumentReferencesResultType {
    LOCATION_ARRAY,
    NULL_TYPE,
  };

  extern std::map<TextDocumentReferencesResultType, std::string> TextDocumentReferencesResultTypeNames;

  typedef std::variant<
    std::vector<std::unique_ptr<Location>>,
    null_t
  > TextDocumentReferencesResult;

  enum class TextDocumentDocumentHighlightResultType {
    DOCUMENT_HIGHLIGHT_ARRAY,
    NULL_TYPE,
  };

  extern std::map<TextDocumentDocumentHighlightResultType, std::string> TextDocumentDocumentHighlightResultTypeNames;

  typedef std::variant<
    std::vector<std::unique_ptr<DocumentHighlight>>,
    null_t
  > TextDocumentDocumentHighlightResult;

  enum class TextDocumentCodeActionResult_0Type {
    COMMAND,
    CODE_ACTION,
  };

  extern std::map<TextDocumentCodeActionResult_0Type, std::string> TextDocumentCodeActionResult_0TypeNames;

  typedef std::variant<
    std::unique_ptr<Command>,
    std::unique_ptr<CodeAction>
  > TextDocumentCodeActionResult_0;

  enum class TextDocumentCodeActionResultType {
    COMMAND_OR_CODE_ACTION_ARRAY,
    NULL_TYPE,
  };

  extern std::map<TextDocumentCodeActionResultType, std::string> TextDocumentCodeActionResultTypeNames;

  typedef std::variant<
    std::vector<TextDocumentCodeActionResult_0>,
    null_t
  > TextDocumentCodeActionResult;

  typedef CodeAction CodeActionResolveResult;

  enum class WorkspaceSymbolResultType {
    SYMBOL_INFORMATION_ARRAY,
    WORKSPACE_SYMBOL_ARRAY,
    NULL_TYPE,
  };

  extern std::map<WorkspaceSymbolResultType, std::string> WorkspaceSymbolResultTypeNames;

  typedef std::variant<
    std::vector<std::unique_ptr<SymbolInformation>>,
    std::vector<std::unique_ptr<WorkspaceSymbol>>,
    null_t
  > WorkspaceSymbolResult;

  typedef WorkspaceSymbol WorkspaceSymbolResolveResult;

  enum class TextDocumentCodeLensResultType {
    CODE_LENS_ARRAY,
    NULL_TYPE,
  };

  extern std::map<TextDocumentCodeLensResultType, std::string> TextDocumentCodeLensResultTypeNames;

  typedef std::variant<
    std::vector<std::unique_ptr<CodeLens>>,
    null_t
  > TextDocumentCodeLensResult;

  typedef CodeLens CodeLensResolveResult;

  typedef null_t WorkspaceCodeLensRefreshResult;

  enum class TextDocumentDocumentLinkResultType {
    DOCUMENT_LINK_ARRAY,
    NULL_TYPE,
  };

  extern std::map<TextDocumentDocumentLinkResultType, std::string> TextDocumentDocumentLinkResultTypeNames;

  typedef std::variant<
    std::vector<std::unique_ptr<DocumentLink>>,
    null_t
  > TextDocumentDocumentLinkResult;

  typedef DocumentLink DocumentLinkResolveResult;

  enum class TextDocumentFormattingResultType {
    TEXT_EDIT_ARRAY,
    NULL_TYPE,
  };

  extern std::map<TextDocumentFormattingResultType, std::string> TextDocumentFormattingResultTypeNames;

  typedef std::variant<
    std::vector<std::unique_ptr<TextEdit>>,
    null_t
  > TextDocumentFormattingResult;

  enum class TextDocumentRangeFormattingResultType {
    TEXT_EDIT_ARRAY,
    NULL_TYPE,
  };

  extern std::map<TextDocumentRangeFormattingResultType, std::string> TextDocumentRangeFormattingResultTypeNames;

  typedef std::variant<
    std::vector<std::unique_ptr<TextEdit>>,
    null_t
  > TextDocumentRangeFormattingResult;

  enum class TextDocumentRangesFormattingResultType {
    TEXT_EDIT_ARRAY,
    NULL_TYPE,
  };

  extern std::map<TextDocumentRangesFormattingResultType, std::string> TextDocumentRangesFormattingResultTypeNames;

  typedef std::variant<
    std::vector<std::unique_ptr<TextEdit>>,
    null_t
  > TextDocumentRangesFormattingResult;

  enum class TextDocumentOnTypeFormattingResultType {
    TEXT_EDIT_ARRAY,
    NULL_TYPE,
  };

  extern std::map<TextDocumentOnTypeFormattingResultType, std::string> TextDocumentOnTypeFormattingResultTypeNames;

  typedef std::variant<
    std::vector<std::unique_ptr<TextEdit>>,
    null_t
  > TextDocumentOnTypeFormattingResult;

  enum class TextDocumentRenameResultType {
    WORKSPACE_EDIT,
    NULL_TYPE,
  };

  extern std::map<TextDocumentRenameResultType, std::string> TextDocumentRenameResultTypeNames;

  typedef std::variant<
    std::unique_ptr<WorkspaceEdit>,
    null_t
  > TextDocumentRenameResult;

  enum class TextDocumentPrepareRenameResultType {
    PREPARE_RENAME_RESULT,
    NULL_TYPE,
  };

  extern std::map<TextDocumentPrepareRenameResultType, std::string> TextDocumentPrepareRenameResultTypeNames;

  typedef std::variant<
    PrepareRenameResult,
    null_t
  > TextDocumentPrepareRenameResult;

  enum class WorkspaceExecuteCommandResultType {
    ANY_TYPE,
    NULL_TYPE,
  };

  extern std::map<WorkspaceExecuteCommandResultType, std::string> WorkspaceExecuteCommandResultTypeNames;

  typedef std::variant<
    std::unique_ptr<LSPAny>,
    null_t
  > WorkspaceExecuteCommandResult;

  typedef ApplyWorkspaceEditResult WorkspaceApplyEditResult;

  enum class IncomingRequest
  {
    CALL_HIERARCHY_INCOMING_CALLS,
    CALL_HIERARCHY_OUTGOING_CALLS,
    CODE_ACTION_RESOLVE,
    CODE_LENS_RESOLVE,
    COMPLETION_ITEM_RESOLVE,
    DOCUMENT_LINK_RESOLVE,
    INITIALIZE,
    INLAY_HINT_RESOLVE,
    SHUTDOWN,
    TEXT_DOCUMENT_CODE_ACTION,
    TEXT_DOCUMENT_CODE_LENS,
    TEXT_DOCUMENT_COLOR_PRESENTATION,
    TEXT_DOCUMENT_COMPLETION,
    TEXT_DOCUMENT_DECLARATION,
    TEXT_DOCUMENT_DEFINITION,
    TEXT_DOCUMENT_DIAGNOSTIC,
    TEXT_DOCUMENT_DOCUMENT_COLOR,
    TEXT_DOCUMENT_DOCUMENT_HIGHLIGHT,
    TEXT_DOCUMENT_DOCUMENT_LINK,
    TEXT_DOCUMENT_DOCUMENT_SYMBOL,
    TEXT_DOCUMENT_FOLDING_RANGE,
    TEXT_DOCUMENT_FORMATTING,
    TEXT_DOCUMENT_HOVER,
    TEXT_DOCUMENT_IMPLEMENTATION,
    TEXT_DOCUMENT_INLAY_HINT,
    TEXT_DOCUMENT_INLINE_COMPLETION,
    TEXT_DOCUMENT_INLINE_VALUE,
    TEXT_DOCUMENT_LINKED_EDITING_RANGE,
    TEXT_DOCUMENT_MONIKER,
    TEXT_DOCUMENT_ON_TYPE_FORMATTING,
    TEXT_DOCUMENT_PREPARE_CALL_HIERARCHY,
    TEXT_DOCUMENT_PREPARE_RENAME,
    TEXT_DOCUMENT_PREPARE_TYPE_HIERARCHY,
    TEXT_DOCUMENT_RANGES_FORMATTING,
    TEXT_DOCUMENT_RANGE_FORMATTING,
    TEXT_DOCUMENT_REFERENCES,
    TEXT_DOCUMENT_RENAME,
    TEXT_DOCUMENT_SELECTION_RANGE,
    TEXT_DOCUMENT_SEMANTIC_TOKENS_FULL,
    TEXT_DOCUMENT_SEMANTIC_TOKENS_FULL_DELTA,
    TEXT_DOCUMENT_SEMANTIC_TOKENS_RANGE,
    TEXT_DOCUMENT_SIGNATURE_HELP,
    TEXT_DOCUMENT_TYPE_DEFINITION,
    TEXT_DOCUMENT_WILL_SAVE_WAIT_UNTIL,
    TYPE_HIERARCHY_SUBTYPES,
    TYPE_HIERARCHY_SUPERTYPES,
    WORKSPACE_DIAGNOSTIC,
    WORKSPACE_EXECUTE_COMMAND,
    WORKSPACE_SYMBOL,
    WORKSPACE_SYMBOL_RESOLVE,
    WORKSPACE_WILL_CREATE_FILES,
    WORKSPACE_WILL_DELETE_FILES,
    WORKSPACE_WILL_RENAME_FILES,
  };

  extern std::map<IncomingRequest, std::string> IncomingRequestNames;
  extern std::map<IncomingRequest, std::string> IncomingRequestValues;

  auto incomingRequestByName(const std::string &name) -> IncomingRequest;
  auto incomingRequestByValue(const std::string &value) -> IncomingRequest;
  auto isIncomingRequest(const std::string &value) -> bool;

  enum class OutgoingRequest
  {
    CLIENT_REGISTER_CAPABILITY,
    CLIENT_UNREGISTER_CAPABILITY,
    WINDOW_SHOW_DOCUMENT,
    WINDOW_SHOW_MESSAGE_REQUEST,
    WINDOW_WORK_DONE_PROGRESS_CREATE,
    WORKSPACE_APPLY_EDIT,
    WORKSPACE_CODE_LENS_REFRESH,
    WORKSPACE_CONFIGURATION,
    WORKSPACE_DIAGNOSTIC_REFRESH,
    WORKSPACE_FOLDING_RANGE_REFRESH,
    WORKSPACE_INLAY_HINT_REFRESH,
    WORKSPACE_INLINE_VALUE_REFRESH,
    WORKSPACE_SEMANTIC_TOKENS_REFRESH,
    WORKSPACE_WORKSPACE_FOLDERS,
  };

  extern std::map<OutgoingRequest, std::string> OutgoingRequestNames;
  extern std::map<OutgoingRequest, std::string> OutgoingRequestValues;

  auto outgoingRequestByName(const std::string &name) -> OutgoingRequest;
  auto outgoingRequestByValue(const std::string &value) -> OutgoingRequest;

  enum class IncomingNotification
  {
    EXIT,
    INITIALIZED,
    NOTEBOOK_DOCUMENT_DID_CHANGE,
    NOTEBOOK_DOCUMENT_DID_CLOSE,
    NOTEBOOK_DOCUMENT_DID_OPEN,
    NOTEBOOK_DOCUMENT_DID_SAVE,
    SET_TRACE,
    TEXT_DOCUMENT_DID_CHANGE,
    TEXT_DOCUMENT_DID_CLOSE,
    TEXT_DOCUMENT_DID_OPEN,
    TEXT_DOCUMENT_DID_SAVE,
    TEXT_DOCUMENT_WILL_SAVE,
    WINDOW_WORK_DONE_PROGRESS_CANCEL,
    WORKSPACE_DID_CHANGE_CONFIGURATION,
    WORKSPACE_DID_CHANGE_WATCHED_FILES,
    WORKSPACE_DID_CHANGE_WORKSPACE_FOLDERS,
    WORKSPACE_DID_CREATE_FILES,
    WORKSPACE_DID_DELETE_FILES,
    WORKSPACE_DID_RENAME_FILES,
  };

  extern std::map<IncomingNotification, std::string> IncomingNotificationNames;
  extern std::map<IncomingNotification, std::string> IncomingNotificationValues;

  auto incomingNotificationByName(const std::string &name) -> IncomingNotification;
  auto incomingNotificationByValue(const std::string &value) -> IncomingNotification;
  auto isIncomingNotification(const std::string &value) -> bool;

  enum class OutgoingNotification
  {
    LOG_TRACE,
    TELEMETRY_EVENT,
    TEXT_DOCUMENT_PUBLISH_DIAGNOSTICS,
    WINDOW_LOG_MESSAGE,
    WINDOW_SHOW_MESSAGE,
  };

  extern std::map<OutgoingNotification, std::string> OutgoingNotificationNames;
  extern std::map<OutgoingNotification, std::string> OutgoingNotificationValues;

  auto outgoingNotificationByName(const std::string &name) -> OutgoingNotification;
  auto outgoingNotificationByValue(const std::string &value) -> OutgoingNotification;

  struct DocumentSymbol;  // forward declaration

  enum class TextDocumentDocumentSymbolResultType {
    SYMBOL_INFORMATION_ARRAY,
    DOCUMENT_SYMBOL_ARRAY,
    NULL_TYPE,
  };

  extern std::map<TextDocumentDocumentSymbolResultType, std::string> TextDocumentDocumentSymbolResultTypeNames;

  typedef std::variant<
    std::vector<std::unique_ptr<SymbolInformation>>,
    std::vector<std::unique_ptr<DocumentSymbol>>,
    null_t
  > TextDocumentDocumentSymbolResult;

  /**
   * Represents programming constructs like variables, classes, interfaces etc.
   * that appear in a document. Document symbols can be hierarchical and they
   * have two ranges: one that encloses its definition and one that points to
   * its most interesting range, e.g. the range of an identifier.
   */
  struct DocumentSymbol
  {
    /**
     * The name of this symbol. Will be displayed in the user interface and therefore must not be
     * an empty string or a string only consisting of white spaces.
     */
    string_t name;
    /**
     * More detail for this symbol, e.g the signature of a function.
     */
    std::optional<string_t> detail;
    /**
     * The kind of this symbol.
     */
    SymbolKind kind;
    /**
     * Tags for this document symbol.
     *
     * @since 3.16.0
     */
    std::optional<std::vector<SymbolTag>> tags;
    /**
     * Indicates if this symbol is deprecated.
     *
     * @deprecated Use tags instead
     */
    std::optional<boolean_t> deprecated;
    /**
     * The range enclosing this symbol not including leading/trailing whitespace but everything else
     * like comments. This information is typically used to determine if the clients cursor is
     * inside the symbol to reveal in the symbol in the UI.
     */
    std::unique_ptr<Range> range;
    /**
     * The range that should be selected and revealed when this symbol is being picked, e.g the name of a function.
     * Must be contained by the `range`.
     */
    std::unique_ptr<Range> selectionRange;
    /**
     * Children of this symbol, e.g. properties of a class.
     */
    std::optional<std::vector<std::unique_ptr<DocumentSymbol>>> children;
  };

  struct SelectionRange;  // forward declaration

  /**
   * A selection range represents a part of a selection hierarchy. A selection range
   * may have a parent selection range that contains it.
   */
  struct SelectionRange
  {
    /**
     * The {@link Range range} of this selection range.
     */
    std::unique_ptr<Range> range;
    /**
     * The parent selection range containing this range. Therefore `parent.range` must contain `this.range`.
     */
    std::optional<std::unique_ptr<SelectionRange>> parent;
  };

  enum class TextDocumentSelectionRangeResultType {
    SELECTION_RANGE_ARRAY,
    NULL_TYPE,
  };

  extern std::map<TextDocumentSelectionRangeResultType, std::string> TextDocumentSelectionRangeResultTypeNames;

  typedef std::variant<
    std::vector<std::unique_ptr<SelectionRange>>,
    null_t
  > TextDocumentSelectionRangeResult;

} // namespace LCompilers::LanguageServerProtocol
