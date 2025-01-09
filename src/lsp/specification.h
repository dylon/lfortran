#ifndef LCOMPILERS_LSP_SPECIFICATION_H
#define LCOMPILERS_LSP_SPECIFICATION_H

#include <cstddef>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

/**
 * Interface definitions from the LSP 3.17 specification.
 * See: https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification
 */
namespace LCompilers::LanguageServiceProvider {

  /**
   * Defines an integer number in the range of -2^31 to 2^31 - 1.
   *
   * export type integer = number;
   */
  typedef int integer;

  /**
   * Defines an unsigned integer number in the range of 0 to 2^31 - 1.
   *
   * export type uinteger = number;
   */
  typedef unsigned int uinteger;

  /**
   * Defines a decimal number. Since decimal numbers are very rare in the
   * language server specification we denote the exact range with every decimal
   * using the mathematics interval notation (e.g. [0, 1] denotes all decimals d
   * with 0 <= d <= 1.
   *
   * export type decimal = number;
   */
  typedef double decimal;

  typedef bool boolean;

  typedef std::nullptr_t null;

  typedef std::string string;

  struct LSPAny;  // Forward declaration

  /**
   * LSP object definition.
   *
   * export type LSPObject = { [key: string]: LSPAny };
   *
   * @since 3.17.0
   */
  typedef std::map<std::string, std::unique_ptr<LSPAny>> LSPObject;

  /**
   * LSP arrays.
   *
   * export type LSPArray = LSPAny[];
   *
   * @since 3.17.0
   */
  typedef std::vector<std::unique_ptr<LSPAny>> LSPArray;

  enum class LSPAnyType {
    LSP_OBJECT,
    LSP_ARRAY,
    LSP_STRING,
    LSP_INTEGER,
    LSP_UINTEGER,
    LSP_DECIMAL,
    LSP_BOOLEAN,
    LSP_NULL,
  };

  /**
   * The LSP any type.
   *
   * export type LSPAny = LSPObject
   *   | LSPArray
   *   | string
   *   | integer
   *   | uinteger
   *   | decimal
   *   | boolean
   *   | null;
   *
   * @since 3.17.0
   */
  struct LSPAny {
    LSPAnyType type;
    union {
      std::unique_ptr<LSPObject> objectValue;
      std::unique_ptr<LSPArray> arrayValue;
      string stringValue;
      integer integerValue;
      uinteger uintegerValue;
      decimal decimalValue;
      null nullValue;
    };
  };

  /**
   * A general message as defined by JSON-RPC. The language server protocol
   * always uses “2.0” as the jsonrpc version.
   *
   * interface Message {
   *   jsonrpc: string;
   * }
   */
  struct Message {
    string jsonrpc;
  };

  enum class RequestIdType {
    LSP_INTEGER,
    LSP_STRING,
  };

  struct RequestId {
    RequestIdType type;
    union {
      integer integervalue;
      string stringvalue;
    };
  };

  enum class RequestParamsType {
    LSP_ARRAY,
    LSP_OBJECT,
  };

  struct RequestParams {
    RequestParamsType type;
    union {
      std::unique_ptr<std::vector<std::unique_ptr<void>>> arrayValue;
      std::unique_ptr<void> objectValue;
    };
  };

  /**
   * A request message to describe a request between the client and the server.
   * Every processed request must send a response back to the sender of the
   * request.
   *
   * interface RequestMessage extends Message {
   *   id: integer | string;
   *   method: string;
   *   params?: array | object;
   * }
   */
  struct RequestMessage : public Message {

    /**
     * The request id.
     *
     * id: integer | string;
     */
    std::unique_ptr<RequestId> id;

    /**
    * The method to be invoked.
     *
     * method: string;
    */
    string method;

    /**
     * The method's params.
     *
     * params?: array | object;
     */
    std::optional<std::unique_ptr<RequestParams>> params;
  };

  /**
   * export namespace ErrorCodes {
   *   export const ParseError: integer = -32700;
   *   export const InvalidRequest: integer = -32600;
   *   export const MethodNotFound: integer = -32601;
   *   export const InvalidParams: integer = -32602;
   *   export const InternalError: integer = -32603;
   *   export const jsonrpcReservedErrorRangeStart: integer = -32099;
   *   export const serverErrorStart: integer = jsonrpcReservedErrorRangeStart;
   *   export const ServerNotInitialized: integer = -32002;
   *   export const UnknownErrorCode: integer = -32001;
   *   export const jsonrpcReservedErrorRangeEnd = -32000;
   *   export const serverErrorEnd: integer = jsonrpcReservedErrorRangeEnd;
   *   export const lspReservedErrorRangeStart: integer = -32899;
   *   export const RequestFailed: integer = -32803;
   *   export const ServerCancelled: integer = -32802;
   *   export const ContentModified: integer = -32801;
   *   export const RequestCancelled: integer = -32800;
   *   export const lspReservedErrorRangeEnd: integer = -32800;
   * }
   */
  enum class ErrorCodes {

    /**
     * Defined by JSON-RPC.
     *
     * export const ParseError: integer = -32700;
     */
    ParseError = -32700,

    /**
     * Defined by JSON-RPC.
     *
     * export const InvalidRequest: integer = -32600;
     */
    InvalidRequest = -32600,

    /**
     * Defined by JSON-RPC.
     *
     * export const MethodNotFound: integer = -32601;
     */
    MethodNotFound = -32601,

    /**
     * Defined by JSON-RPC.
     *
     * export const InvalidParams: integer = -32602;
     */
    InvalidParams = -32602,

    /**
     * Defined by JSON-RPC.
     *
     * export const InternalError: integer = -32603;
     */
    InternalError = -32603,

    /**
     * This is the start range of JSON-RPC reserved error codes.
     * It doesn't denote a real error code. No LSP error codes should
     * be defined between the start and end range. For backwards
     * compatibility the `ServerNotInitialized` and the `UnknownErrorCode`
     * are left in the range.
     *
     * export const jsonrpcReservedErrorRangeStart: integer = -32099;
     *
     * @since 3.16.0
     */
    jsonrpcReservedErrorRangeStart = -32099,

    /**
     * @deprecated use jsonrpcReservedErrorRangeStart
     *
     * export const serverErrorStart: integer = jsonrpcReservedErrorRangeStart;
     */
    serverErrorStart = jsonrpcReservedErrorRangeStart,

    /**
     * Error code indicating that a server received a notification or
     * request before the server has received the `initialize` request.
     *
     * export const ServerNotInitialized: integer = -32002;
     */
    ServerNotInitialized = -32002,

    /**
     * export const UnknownErrorCode: integer = -32001;
     */
    UnknownErrorCode = -32001,

    /**
     * This is the end range of JSON-RPC reserved error codes.
     * It doesn't denote a real error code.
     *
     * export const jsonrpcReservedErrorRangeEnd = -32000;
     *
     * @since 3.16.0
     */
    jsonrpcReservedErrorRangeEnd = -32000,

    /**
     * @deprecated use jsonrpcReservedErrorRangeEnd
     *
     * export const serverErrorEnd: integer = jsonrpcReservedErrorRangeEnd;
     */
    serverErrorEnd = jsonrpcReservedErrorRangeEnd,

    /**
     * This is the start range of LSP reserved error codes.
     * It doesn't denote a real error code.
     *
     * @since 3.16.0
     *
     * export const lspReservedErrorRangeStart: integer = -32899;
     */
    lspReservedErrorRangeStart = -32899,

    /**
     * A request failed but it was syntactically correct, e.g the
     * method name was known and the parameters were valid. The error
     * message should contain human readable information about why
     * the request failed.
     *
     * export const RequestFailed: integer = -32803;
     *
     * @since 3.17.0
     */
    RequestFailed = -32803,

    /**
     * The server cancelled the request. This error code should
     * only be used for requests that explicitly support being
     * server cancellable.
     *
     * export const ServerCancelled: integer = -32802;
     *
     * @since 3.17.0
     */
    ServerCancelled = -32802,

    /**
     * The server detected that the content of a document got
     * modified outside normal conditions. A server should
     * NOT send this error code if it detects a content change
     * in it unprocessed messages. The result even computed
     * on an older state might still be useful for the client.
     *
     * If a client decides that a result is not of any use anymore
     * the client should cancel the request.
     *
     * export const ContentModified: integer = -32801;
     */
    ContentModified = -32801,

    /**
     * The client has canceled a request and a server has detected
     * the cancel.
     *
     * export const RequestCancelled: integer = -32800;
     */
    RequestCancelled = -32800,

    /**
     * This is the end range of LSP reserved error codes.
     * It doesn't denote a real error code.
     *
     * export const lspReservedErrorRangeEnd: integer = -32800;
     *
     * @since 3.16.0
     */
    lspReservedErrorRangeEnd = -32800,
  };

  /**
   * interface ResponseError {
   *   code: integer;
   *   message: string;
   *   data?: LSPAny;
   * }
   */
  struct ResponseError {

    /**
     * A number indicating the error type that occurred.
     *
     * code: integer;
     */
    integer code;

    /**
     * A string providing a short description of the error.
     *
     * message: string;
     */
    string message;

    /**
     * A primitive or structured value that contains additional
     * information about the error. Can be omitted.
     *
     * data?: LSPAny;
     */
    std::optional<std::unique_ptr<LSPAny>> data;
  };

  enum class ResponseIdType {
    LSP_INTEGER,
    LSP_STRING,
    LSP_NULL,
  };

  struct ResponseId {
    ResponseIdType type;
    union {
      integer integerValue;
      string stringValue;
      null nullValue;
    };
  };

  /**
   * A Response Message sent as a result of a request. If a request doesn’t
   * provide a result value the receiver of a request still needs to return a
   * response message to conform to the JSON-RPC specification. The result
   * property of the ResponseMessage should be set to null in this case to
   * signal a successful request.
   */
  struct ResponseMessage : public Message {

    /**
     * The request id.
     *
     * id: integer | string | null;
     */
    std::unique_ptr<ResponseId> id;

    /**
     * The result of a request. This member is REQUIRED on success. This member
     * MUST NOT exist if there was an error invoking the method.
     *
     * result?: LSPAny;
     */
    std::optional<std::unique_ptr<LSPAny>> result;

    /**
     * The error object in case a request fails.
     *
     * error?: ResponseError;
     */
    std::optional<std::unique_ptr<ResponseError>> error;
  };

  enum class NotificationParamsType {
    LSP_ARRAY,
    LSP_OBJECT,
  };

  struct NotificationParams {
    NotificationParamsType type;
    union {
      std::unique_ptr<std::vector<std::unique_ptr<void>>> arrayValue;
      std::unique_ptr<void> objectValue;
    };
  };

  /**
   * A notification message. A processed notification message must not send a
   * response back. They work like events.
   *
   * interface NotificationMessage extends Message {
   *   method: string;
   *   params?: array | object;
   * }
   */
  struct NotificationMessage : public Message {

    /**
     * The method to be invoked.
     *
     * method: string;
     */
    std::string method;

    /**
     * The notification's params.
     *
     * params?: array | object;
     */
    std::optional<std::unique_ptr<NotificationParams>> params;
  };

  // ---------------------------------------------------------------------------
  // $ Notifications and Requests
  // ---------------------------------------------------------------------------
  // Notification and requests whose methods start with ‘$/’ are messages which
  // are protocol implementation dependent and might not be implementable in all
  // clients or servers. For example if the server implementation uses a single
  // threaded synchronous programming language then there is little a server can
  // do to react to a $/cancelRequest notification. If a server or client
  // receives notifications starting with ‘$/’ it is free to ignore the
  // notification. If a server or client receives a request starting with ‘$/’
  // it must error the request with error code MethodNotFound (e.g. -32601).
  // ---------------------------------------------------------------------------

  /**
   * The base protocol offers support for request cancellation. To cancel a request, a notification message with the following properties is sent:
   *
   *  Notification:
   *    method: ‘$/cancelRequest’
   *    params: CancelParams defined as follows:
   *
   * A request that got canceled still needs to return from the server and send
   * a response back. It can not be left open / hanging. This is in line with
   * the JSON-RPC protocol that requires that every request sends a response
   * back. In addition it allows for returning partial results on cancel. If the
   * request returns an error response on cancellation it is advised to set the
   * error code to ErrorCodes.RequestCancelled.
   *
   *  interface CancelParams {
   *    id: integer | string;
   *  }
   */
  struct CancelParams {

    /**
     * The request id to cancel.
     *
     * id: integer | string;
     */
    std::unique_ptr<RequestId> id;
  };

  enum class ProgressTokenType {
    LSP_INTEGER,
    LSP_STRING,
  };

  struct ProgressToken {
    ProgressTokenType type;
    union {
      integer integerValue;
      string stringValue;
    };
  };

  /**
   * The base protocol offers also support to report progress in a generic
   * fashion. This mechanism can be used to report any kind of progress
   * including work done progress (usually used to report progress in the user
   * interface using a progress bar) and partial result progress to support
   * streaming of results.
   *
   * A progress notification has the following properties:
   *
   * Notification:
   *   method: ‘$/progress’
   *   params: ProgressParams defined as follows:
   *
   * Progress is reported against a token. The token is different than the
   * request ID which allows to report progress out of band and also for
   * notification.
   *
   * interface ProgressParams<T> {
   *   token: ProgressToken;
   *   value: T;
   * }
   */
  struct ProgressParams {

    /**
     * The progress token provided by the client or server.
     *
     * token: ProgressToken;
     */
    std::unique_ptr<ProgressToken> token;

    /**
     * The progress data.
     *
     * value: T;
     */
    std::unique_ptr<void> value;
  };

  /**
   * Position in a text document expressed as zero-based line and zero-based
   * character offset. A position is between two characters like an ‘insert’
   * cursor in an editor. Special values like for example -1 to denote the end
   * of a line are not supported.
   *
   * interface Position {
   *   line: uinteger;
   *   character: uinteger;
   * }
   */
  struct Position {

    /**
     * Line position in a document (zero-based).
     *
     * line: uinteger;
     */
    uinteger line;

    /**
     * Character offset on a line in a document (zero-based). The meaning of this
     * offset is determined by the negotiated `PositionEncodingKind`.
     *
     * If the character value is greater than the line length it defaults back
     * to the line length.
     *
     * character: uinteger;
     */
    uinteger character;
  };

  /**
   * A type indicating how positions are encoded, specifically what column
   * offsets mean.
   *
   * export type PositionEncodingKind = string;
   *
   * @since 3.17.0
   */
  enum class PositionEncodingKind {

    /**
     * Character offsets count UTF-8 code units (e.g bytes).
     *
     * export const UTF8: PositionEncodingKind = 'utf-8';
     */
    UTF8,

    /**
     * Character offsets count UTF-16 code units.
     *
     * This is the default and must always be supported
     * by servers
     *
     * export const UTF16: PositionEncodingKind = 'utf-16';
     */
    UTF16,

    /**
     * Character offsets count UTF-32 code units.
     *
     * Implementation note: these are the same as Unicode code points,
     * so this `PositionEncodingKind` may also be used for an
     * encoding-agnostic representation of character offsets.
     *
     * export const UTF32: PositionEncodingKind = 'utf-32';
     */
    UTF32,
  };

  extern std::map<PositionEncodingKind, std::string> PositionEncodingKindNames;

  auto positionEncodingKind(const std::string &name) -> PositionEncodingKind;

  /**
   * A range in a text document expressed as (zero-based) start and end
   * positions. A range is comparable to a selection in an editor. Therefore,
   * the end position is exclusive. If you want to specify a range that contains
   * a line including the line ending character(s) then use an end position
   * denoting the start of the next line.
   *
   * interface Range {
   *   start: Position;
   *   end: Position;
   * }
   */
  struct Range {

    /**
     * The range's start position.
     *
     * start: Position;
     */
    std::unique_ptr<Position> start;

    /**
     * The range's end position.
     *
     * end: Position;
     */
    std::unique_ptr<Position> end;
  };

  /**
   * A URI specifying the path to a document. This should begin with "file://"
   * but might begin with "file:" or the protocol (e.g. "file://") may be
   * excluded. By specification, it should begin with the "file://" protocol.
   *
   * type DocumentUri = string;
   */
  typedef string DocumentUri;

  /**
   * A tagging interface for normal non document URIs.
   *
   * type URI = string;
   */
  typedef string URI;

  /**
   * An item to transfer a text document from the client to the server.
   *
   * Text documents have a language identifier to identify a document on the
   * server side when it handles more than one language to avoid re-interpreting
   * the file extension. If a document refers to one of the programming
   * languages listed below it is recommended that clients use those ids.
   *
   * --------------------+-----------------------------------
   * Language            | Identifier
   * --------------------+-----------------------------------
   * ABAP                | abap
   * Windows Bat         | bat
   * BibTeX              | bibtex
   * Clojure             | clojure
   * Coffeescript        | coffeescript
   * C                   | c
   * C++                 | cpp
   * C#                  | csharp
   * CSS                 | css
   * Diff                | diff
   * Dart                | dart
   * Dockerfile          | dockerfile
   * Elixir              | elixir
   * Erlang              | erlang
   * F#                  | fsharp
   * Git                 | git-commit
   * Git                 | git-rebase
   * Go                  | go
   * Groovy              | groovy
   * Handlebars          | handlebars
   * HTML                | html
   * Ini                 | ini
   * Java                | java
   * JavaScript          | javascript
   * JavaScript React    | javascriptreact
   * JSON                | json
   * LaTeX               | latex
   * Less                | less
   * Lua                 | lua
   * Makefile            | makefile
   * Markdown            | markdown
   * Objective-C         | objective-c
   * Objective-C++       | objective-cpp
   * Perl                | perl
   * Perl 6              | perl6
   * PHP                 | php
   * Powershell          | powershell
   * Pug                 | jade
   * Python              | python
   * R                   | r
   * Razor (cshtml)      | razor
   * Ruby                | ruby
   * Rust                | rust
   * SCSS                | scss (syntax using curly brackets)
   * SCSS                | sass (indented syntax)
   * Scala               | scala
   * ShaderLab           | shaderlab
   * Shell Script (Bash) | shellscript
   * SQL                 | sql
   * Swift               | swift
   * TypeScript          | typescript
   * TypeScript React    | typescriptreact
   * TeX                 | tex
   * Visual Basic        | vb
   * XML                 | xml
   * XSL                 | xsl
   * YAML                | yaml
   * --------------------+-----------------------------------
   *
   * interface TextDocumentItem {
   *   uri: DocumentUri;
   *   languageId: string;
   *   version: integer;
   *   text: string;
   * }
   */
  struct TextDocumentItem {

    /**
     * The text document's URI.
     *
     * uri: DocumentUri;
     */
    DocumentUri uri;

    /**
     * The text document's language identifier.
     *
     * languageId: string;
     */
    string languageId;

    /**
     * The version number of this document (it will increase after each change,
     * including undo/redo).
     *
     * version: integer;
     */
    integer version;

    /**
     * The content of the opened text document.
     *
     * text: string;
     */
    string text;
  };

  /**
   * Text documents are identified using a URI. On the protocol level, URIs are
   * passed as strings.
   *
   * interface TextDocumentIdentifier {
   *   uri: DocumentUri;
   * }
   */
  struct TextDocumentIdentifier {

    /**
     * The text document's URI.
     *
     * uri: DocumentUri;
     */
    DocumentUri uri;
  };

  /**
   * An identifier to denote a specific version of a text document. This
   * information usually flows from the client to the server.
   *
   * interface VersionedTextDocumentIdentifier extends TextDocumentIdentifier {
   *   version: integer;
   * }
   */
  struct VersionedTextDocumentIdentifier : public TextDocumentIdentifier {

    /**
     * The version number of this document.
     *
     * The version number of a document will increase after each change,
     * including undo/redo. The number doesn't need to be consecutive.
     *
     * version: integer;
     */
    integer version;
  };

  /**
   * An identifier which optionally denotes a specific version of a text
   * document. This information usually flows from the server to the client.
   *
   * interface OptionalVersionedTextDocumentIdentifier extends TextDocumentIdentifier {
   *   version: integer | null;
   * }
   */
  struct OptionalVersionedTextDocumentIdentifier : public TextDocumentIdentifier {

    /**
     * The version number of this document. If an optional versioned text document
     * identifier is sent from the server to the client and the file is not
     * open in the editor (the server has not received an open notification
     * before) the server can send `null` to indicate that the version is
     * known and the content on disk is the master (as specified with document
     * content ownership).
     *
     * The version number of a document will increase after each change,
     * including undo/redo. The number doesn't need to be consecutive.
     *
     * version: integer | null;
     */
    std::optional<integer> version;
  };

  /**
   * Was TextDocumentPosition in 1.0 with inlined parameters.
   *
   * A parameter literal used in requests to pass a text document and a position
   * inside that document. It is up to the client to decide how a selection is
   * converted into a position when issuing a request for a text document. The
   * client can for example honor or ignore the selection direction to make LSP
   * request consistent with features implemented internally.
   *
   * interface TextDocumentPositionParams {
   *   textDocument: TextDocumentIdentifier;
   *   position: Position;
   * }
   */
  struct TextDocumentPositionParams {

    /**
     * The text document.
     *
     * textDocument: TextDocumentIdentifier;
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;

    /**
     * The position inside the text document.
     *
     * position: Position;
     */
    std::unique_ptr<Position> position;
  };

  /**
   * A document filter denotes a document through properties like language,
   * scheme or pattern. An example is a filter that applies to TypeScript files
   * on disk. Another example is a filter that applies to JSON files with name
   * package.json.
   *
   * Please note that for a document filter to be valid at least one of the
   * properties for language, scheme, or pattern must be set. To keep the type
   * definition simple all properties are marked as optional.
   *
   * export interface DocumentFilter {
   *   language?: string;
   *   scheme?: string;
   *   pattern?: string;
   * }
   */
  struct DocumentFilter {

    /**
     * A language id, like `typescript`.
     *
     * language?: string;
     */
    std::optional<string> language;

    /**
     * A Uri scheme, like `file` or `untitled`.
     *
     * scheme?: string;
     */
    std::optional<string> scheme;

    /**
     * A glob pattern, like `*.{ts,js}`.
     *
     * Glob patterns can have the following syntax:
     * - `*` to match one or more characters in a path segment.
     * - `?` to match on one character in a path segment.
     * - `**` to match any number of path segments, including none.
     * - `{}` to group sub patterns into an OR expression.
     * - `[]` to declare a range of characters to match in a path segment.
     * - `[!...]` to negate a range of characters to match in a path segment.
     *
     * pattern?: string;
     */
    std::optional<string> pattern;
  };

  /**
   * A document selector is the combination of one or more document filters.
   *
   * export type DocumentSelector = DocumentFilter[];
   */
  typedef std::vector<std::unique_ptr<DocumentFilter>> DocumentSelector;

  /**
   * A textual edit applicable to a text document.
   *
   * interface TextEdit {
   *   range: Range;
   *   newText: string;
   * }
   */
  struct TextEdit {

    /**
     * The range of the text document to be manipulated. To insert
     * text into a document create a range where start === end.
     *
     * range: Range;
     */
    std::unique_ptr<Range> range;

    /**
     * The string to be inserted. For delete operations use an
     * empty string.
     *
     * newText: string;
     */
    string newText;
  };

  /**
   * Since 3.16.0 there is also the concept of an annotated text edit which
   * supports to add an annotation to a text edit. The annotation can add
   * information describing the change to the text edit.
   *
   * Usually clients provide options to group the changes along the annotations
   * they are associated with. To support this in the protocol an edit or
   * resource operation refers to a change annotation using an identifier and
   * not the change annotation literal directly. This allows servers to use the
   * identical annotation across multiple edits or resource operations which
   * then allows clients to group the operations under that change annotation.
   * The actual change annotations together with their identifiers are managed
   * by the workspace edit via the new property changeAnnotations.
   *
   * export interface ChangeAnnotation {
   *   label: string;
   *   needsConfirmation?: boolean;
   *   description?: string;
   * }
   */
  struct ChangeAnnotation {

    /**
     * A human-readable string describing the actual change. The string
     * is rendered prominent in the user interface.
     *
     * label: string;
     */
    string label;

    /**
     * A flag which indicates that user confirmation is needed
     * before applying the change.
     *
     * needsConfirmation?: boolean;
     */
    std::optional<boolean> needsConfirmation;

    /**
     * A human-readable string which is rendered less prominent in
     * the user interface.
     *
     * description?: string;
     */
    std::optional<string> description;
  };

  /**
   * An identifier referring to a change annotation managed by a workspace
   * edit.
   *
   * export type ChangeAnnotationIdentifier = string;
   *
   * @since 3.16.0.
   */
  typedef string ChangeAnnotationIdentifier;

  /**
   * A special text edit with an additional change annotation.
   *
   * export interface AnnotatedTextEdit extends TextEdit {
   *   annotationId: ChangeAnnotationIdentifier;
   * }
   *
   * @since 3.16.0.
   */
  struct AnnotatedTextEdit : public TextEdit {

    /**
     * The actual annotation identifier.
     *
     * annotationId: ChangeAnnotationIdentifier;
     */
    ChangeAnnotationIdentifier annotationId;
  };

  enum class OptionalAnnotatedTextEditType {
    NOT_ANNOTATED,
    ANNOTATED,
  };

  struct OptionalAnnotatedTextEdit {
    OptionalAnnotatedTextEditType type;
    union {
      std::unique_ptr<TextEdit> textEdit;
      std::unique_ptr<AnnotatedTextEdit> annotatedTextEdit;
    };
  };

  /**
   * New in version 3.16: support for AnnotatedTextEdit. The support is guarded
   * by the client capability workspace.workspaceEdit.changeAnnotationSupport.
   * If a client doesn’t signal the capability, servers shouldn’t send
   * AnnotatedTextEdit literals back to the client.
   *
   * Describes textual changes on a single text document. The text document is
   * referred to as a OptionalVersionedTextDocumentIdentifier to allow clients
   * to check the text document version before an edit is applied. A
   * TextDocumentEdit describes all changes on a version Si and after they are
   * applied move the document to version Si+1. So the creator of a
   * TextDocumentEdit doesn’t need to sort the array of edits or do any kind of
   * ordering. However the edits must be non overlapping.
   *
   * export interface TextDocumentEdit {
   *   textDocument: OptionalVersionedTextDocumentIdentifier;
   *   edits: (TextEdit | AnnotatedTextEdit)[];
   * }
   */
  struct TextDocumentEdit {

    /**
     * The text document to change.
     *
     * textDocument: OptionalVersionedTextDocumentIdentifier;
     */
    std::unique_ptr<OptionalVersionedTextDocumentIdentifier> textDocument;

    /**
     * The edits to be applied.
     *
     * @since 3.16.0 - support for AnnotatedTextEdit. This is guarded by the
     * client capability `workspace.workspaceEdit.changeAnnotationSupport`
     *
     * edits: (TextEdit | AnnotatedTextEdit)[];
     */
    std::unique_ptr<std::vector<std::unique_ptr<OptionalAnnotatedTextEdit>>> edits;
  };

  /**
   * Represents a location inside a resource, such as a line inside a text file.
   *
   * interface Location {
   *   uri: DocumentUri;
   *   range: Range;
   * }
   */
  struct Location {

    DocumentUri uri;

    Range range;
  };

  /**
   * Represents a link between a source and a target location.
   *
   * interface LocationLink {
   *   originSelectionRange?: Range;
   *   targetUri: DocumentUri;
   *   targetRange: Range;
   *   targetSelectionRange: Range;
   * }
   */
  struct LocationLink {

    /**
     * Span of the origin of this link.
     *
     * Used as the underlined span for mouse interaction. Defaults to the word
     * range at the mouse position.
     *
     * originSelectionRange?: Range;
     */
    std::optional<std::unique_ptr<Range>> originSelectionRange;

    /**
     * The target resource identifier of this link.
     *
     * targetUri: DocumentUri;
     */
    DocumentUri targetUri;

    /**
     * The full target range of this link. If the target for example is a symbol
     * then target range is the range enclosing this symbol not including
     * leading/trailing whitespace but everything else like comments. This
     * information is typically used to highlight the range in the editor.
     *
     * targetRange: Range;
     */
    std::unique_ptr<Range> targetRange;

    /**
     * The range that should be selected and revealed when this link is being
     * followed, e.g the name of a function. Must be contained by the
     * `targetRange`. See also `DocumentSymbol#range`
     *
     * targetSelectionRange: Range;
     */
    std::unique_ptr<Range> targetSelectionRange;
  };

  /**
   * The protocol currently supports the following diagnostic severities and
   * tags.
   *
   * export namespace DiagnosticSeverity {
   *   export const Error: 1 = 1;
   *   export const Warning: 2 = 2;
   *   export const Information: 3 = 3;
   *   export const Hint: 4 = 4;
   * }
   *
   * export type DiagnosticSeverity = 1 | 2 | 3 | 4;
   */
  enum class DiagnosticSeverity {

    /**
     * Reports an error.
     *
     * export const Error: 1 = 1;
     */
    Error = 1,

    /**
     * Reports a warning.
     *
     * export const Warning: 2 = 2;
     */
    Warning = 2,

    /**
     * Reports an information.
     *
     * export const Information: 3 = 3;
     */
    Information = 3,

    /**
     * Reports a hint.
     *
     * export const Hint: 4 = 4;
     */
    Hint = 4,
  };

  extern std::map<DiagnosticSeverity, std::string> DiagnosticSeverityNames;

  auto diagnosticSeverity(const std::string &name) -> DiagnosticSeverity;

  /**
   * The diagnostic tags.
   *
   * export namespace DiagnosticTag {
   *   export const Unnecessary: 1 = 1;
   *   export const Deprecated: 2 = 2;
   * }
   *
   * export type DiagnosticTag = 1 | 2;
   *
   * @since 3.15.0
   */
  enum class DiagnosticTag {

    /**
     * Unused or unnecessary code.
     *
     * Clients are allowed to render diagnostics with this tag faded out
     * instead of having an error squiggle.
     *
     * export const Unnecessary: 1 = 1;
     */
    Unnecessary = 1,

    /**
     * Deprecated or obsolete code.
     *
     * Clients are allowed to rendered diagnostics with this tag strike through.
     *
     * export const Deprecated: 2 = 2;
     */
    Deprecated = 2,
  };

  extern std::map<DiagnosticTag, std::string> DiagnosticTagNames;

  auto diagnosticTag(const std::string &name) -> DiagnosticTag;

  /**
   * Represents a related message and source code location for a diagnostic.
   * This should be used to point to code locations that cause or are related to
   * a diagnostics, e.g when duplicating a symbol in a scope.
   *
   * export interface DiagnosticRelatedInformation {
   *   location: Location;
   *   message: string;
   * }
   */
  struct DiagnosticRelatedInformation {

    /**
     * The location of this related diagnostic information.
     *
     * location: Location;
     */
    std::unique_ptr<Location> location;

    /**
     * The message of this related diagnostic information.
     *
     * message: string;
     */
    string message;
  };

  /**
   * Structure to capture a description for an error code.
   *
   * export interface CodeDescription {
   *   href: URI;
   * }
   *
   * @since 3.16.0
   */
  struct CodeDescription {

    /**
     * An URI to open with more information about the diagnostic error.
     *
     * href: URI;
     */
    URI href;
  };

  enum class DiagnosticCodeType {
    LSP_INTEGER,
    LSP_STRING,
  };

  struct DiagnosticCode {
    DiagnosticCodeType type;
    union {
      integer integerValue;
      string stringValue;
    };
  };

  /**
   * Represents a diagnostic, such as a compiler error or warning. Diagnostic
   * objects are only valid in the scope of a resource.
   *
   * export interface Diagnostic {
   *   range: Range;
   *   severity?: DiagnosticSeverity;
   *   code?: integer | string;
   *   codeDescription?: CodeDescription;
   *   source?: string;
   *   message: string;
   *   tags?: DiagnosticTag[];
   *   relatedInformation?: DiagnosticRelatedInformation[];
   *   data?: LSPAny;
   * }
   */
  struct Diagnostic {

    /**
     * The range at which the message applies.
     *
     * range: Range;
     */
    std::unique_ptr<Range> range;

    /**
     * The diagnostic's severity. To avoid interpretation mismatches when a
     * server is used with different clients it is highly recommended that
     * servers always provide a severity value. If omitted, it’s recommended for
     * the client to interpret it as an Error severity.
     *
     * severity?: DiagnosticSeverity;
     */
    std::optional<DiagnosticSeverity> severity;

    /**
     * The diagnostic's code, which might appear in the user interface.
     *
     * code?: integer | string;
     */
    std::optional<std::unique_ptr<DiagnosticCode>> code;

    /**
     * An optional property to describe the error code.
     *
     * codeDescription?: CodeDescription;
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<CodeDescription>> codeDescription;

    /**
     * A human-readable string describing the source of this
     * diagnostic, e.g. 'typescript' or 'super lint'.
     *
     * source?: string;
     */
    std::optional<string> source;

    /**
     * The diagnostic's message.
     *
     * message: string;
     */
    string message;

    /**
     * Additional metadata about the diagnostic.
     *
     * tags?: DiagnosticTag[];
     *
     * @since 3.15.0
     */
    std::optional<std::unique_ptr<std::vector<std::unique_ptr<DiagnosticTag>>>> tags;

    /**
     * An array of related diagnostic information, e.g. when symbol-names within
     * a scope collide all definitions can be marked via this property.
     *
     * relatedInformation?: DiagnosticRelatedInformation[];
     */
    std::optional<std::unique_ptr<std::vector<std::unique_ptr<DiagnosticRelatedInformation>>>> relatedInformation;

    /**
     * A data entry field that is preserved between a
     * `textDocument/publishDiagnostics` notification and
     * `textDocument/codeAction` request.
     *
     * data?: LSPAny;
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<LSPAny>> data;
  };

  /**
   * Represents a reference to a command. Provides a title which will be used to
   * represent a command in the UI. Commands are identified by a string
   * identifier. The recommended way to handle commands is to implement their
   * execution on the server side if the client and server provides the
   * corresponding capabilities. Alternatively the tool extension code could
   * handle the command. The protocol currently doesn’t specify a set of
   * well-known commands.
   *
   * interface Command {
   *   title: string;
   *   command: string;
   *   arguments?: LSPAny[];
   * }
   */
  struct Command {

    /**
     * Title of the command, like `save`.
     *
     * title: string;
     */
    string title;

    /**
     * The identifier of the actual command handler.
     *
     * command: string;
     */
    string command;

    /**
     * Arguments that the command handler should be invoked with.
     *
     * arguments?: LSPAny[];
     */
    std::optional<std::unique_ptr<std::vector<std::unique_ptr<LSPAny>>>> arguments;
  };

  /**
   * Describes the content type that a client supports in various result
   * literals like `Hover`, `ParameterInfo` or `CompletionItem`.
   *
   * Please note that `MarkupKinds` must not start with a `$`. This kinds are
   * reserved for internal usage.
   *
   * export namespace MarkupKind {
   *   export const PlainText: 'plaintext' = 'plaintext';
   *   export const Markdown: 'markdown' = 'markdown';
   * }
   * export type MarkupKind = 'plaintext' | 'markdown';
   */
  enum class MarkupKind {

    /**
     * Plain text is supported as a content format
     *
     * export const PlainText: 'plaintext' = 'plaintext';
     */
    PlainText,

    /**
     * Markdown is supported as a content format
     *
     * export const Markdown: 'markdown' = 'markdown';
     */
    Markdown,
  };

  extern std::map<MarkupKind, std::string> MarkupKindNames;

  auto markupKind(const std::string &name) -> MarkupKind;

  /**
   * A `MarkupContent` literal represents a string value which content is
   * interpreted base on its kind flag. Currently the protocol supports
   * `plaintext` and `markdown` as markup kinds.
   *
   * If the kind is `markdown` then the value can contain fenced code blocks
   * like in GitHub issues.
   *
   * *Please Note* that clients might sanitize the return markdown. A client
   * could decide to remove HTML from the markdown to avoid script execution.
   *
   * export interface MarkupContent {
   *   kind: MarkupKind;
   *   value: string;
   * }
   */
  struct MarkupContent {

    /**
     * The type of the Markup
     *
     * kind: MarkupKind;
     */
    MarkupKind kind;

    /**
     * The content itself
     *
     * value: string;
     */
    string value;
  };

  /**
   * Client capabilities specific to the used markdown parser.
   *
   * export interface MarkdownClientCapabilities {
   *   parser: string;
   *   version?: string;
   *   allowedTags?: string[];
   * }
   *
   * @since 3.16.0
   */
  struct MarkdownClientCapabilities {

    /**
     * The name of the parser.
     *
     * parser: string;
     */
    string parser;

    /**
     * The version of the parser.
     *
     * version?: string;
     */
    std::optional<string> version;

    /**
     * A list of HTML tags that the client allows / supports in
     * Markdown.
     *
     * allowedTags?: string[];
     *
     * @since 3.17.0
     */
    std::optional<std::vector<std::string>> allowedTags;
  };

  // --------------------------------------------------------------------------
  // New in version 3.13. Since version 3.16 file resource changes can carry an
  // additional property changeAnnotation to describe the actual change in more
  // detail. Whether a client has support for change annotations is guarded by
  // the client capability workspace.workspaceEdit.changeAnnotationSupport.
  //
  // File resource changes allow servers to create, rename and delete files and
  // folders via the client. Note that the names talk about files but the
  // operations are supposed to work on files and folders. This is in line with
  // other naming in the Language Server Protocol (see file watchers which can
  // watch files and folders).
  // --------------------------------------------------------------------------

  /**
   * Options to create a file.
   *
   * export interface CreateFileOptions {
   *   overwrite?: boolean;
   *   ignoreIfExists?: boolean;
   * }
   */
  struct CreateFileOptions {

    /**
     * Overwrite existing file. Overwrite wins over `ignoreIfExists`
     *
     * overwrite?: boolean;
     */
    std::optional<boolean> overwrite;

    /**
     * Ignore if exists.
     *
     * ignoreIfExists?: boolean;
     */
    std::optional<boolean> ignoreIfExists;
  };

  /**
   * Create file operation.
   *
   * export interface CreateFile {
   *   kind: 'create';
   *   uri: DocumentUri;
   *   options?: CreateFileOptions;
   *   annotationId?: ChangeAnnotationIdentifier;
   * }
   */
  struct CreateFile {

    /**
     * A create
     *
     * kind: 'create';
     */
    const std::string kind = "create";

    /**
     * The resource to create.
     *
     * uri: DocumentUri;
     */
    DocumentUri uri;

    /**
     * Additional options
     *
     * options?: CreateFileOptions;
     */
    std::optional<std::unique_ptr<CreateFileOptions>> options;

    /**
     * An optional annotation identifier describing the operation.
     *
     * annotationId?: ChangeAnnotationIdentifier;
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<ChangeAnnotationIdentifier>> annotationId;
  };

  /**
   * Rename file options.
   *
   * export interface RenameFileOptions {
   *   overwrite?: boolean;
   *   ignoreIfExists?: boolean;
   * }
   */
  struct RenameFileOptions {

    /**
     * Overwrite target if existing. Overwrite wins over `ignoreIfExists`
     *
     * overwrite?: boolean;
     */
    std::optional<boolean> overwrite;

    /**
     * Ignores if target exists.
     *
     * ignoreIfExists?: boolean;
     */
    std::optional<boolean> ignoreIfExists;
  };

  /**
   * Rename file operation.
   *
   * export interface RenameFile {
   *   kind: 'rename';
   *   oldUri: DocumentUri;
   *   newUri: DocumentUri;
   *   options?: RenameFileOptions;
   *   annotationId?: ChangeAnnotationIdentifier;
   * }
   */
  struct RenameFile {

    /**
     * A rename.
     *
     * kind: 'rename';
     */
    const std::string kind = "rename";

    /**
     * The old (existing) location.
     *
     * oldUri: DocumentUri;
     */
    DocumentUri oldUri;

    /**
     * The new location.
     *
     * newUri: DocumentUri;
     */
    DocumentUri newUri;

    /**
     * Rename options.
     *
     * options?: RenameFileOptions;
     */
    std::optional<std::unique_ptr<RenameFileOptions>> options;

    /**
     * An optional annotation identifier describing the operation.
     *
     * annotationId?: ChangeAnnotationIdentifier;
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<ChangeAnnotationIdentifier>> annotationId;
  };

  /**
   * Delete file options.
   *
   * export interface DeleteFileOptions {
   *   recursive?: boolean;
   *   ignoreIfNotExists?: boolean;
   * }
   */
  struct DeleteFileOptions {

    /**
     * Delete the content recursively if a folder is denoted.
     *
     * recursive?: boolean;
     */
    std::optional<boolean> recursive;

    /**
     * Ignore the operation if the file doesn't exist.
     *
     * ignoreIfNotExists?: boolean;
     */
    std::optional<boolean> ignoreIfNotExists;
  };

  /**
   * Delete file operation.
   *
   * export interface DeleteFile {
   *   kind: 'delete';
   *   uri: DocumentUri;
   *   options?: DeleteFileOptions;
   *   annotationId?: ChangeAnnotationIdentifier;
   * }
   */
  struct DeleteFile {

    /**
     * A delete.
     *
     * kind: 'delete';
     */
    const std::string kind = "delete";

    /**
     * The file to delete.
     *
     * uri: DocumentUri;
     */
    DocumentUri uri;

    /**
     * Delete options.
     *
     * options?: DeleteFileOptions;
     */
    std::optional<std::unique_ptr<DeleteFileOptions>> options;

    /**
     * An optional annotation identifier describing the operation.
     *
     * annotationId?: ChangeAnnotationIdentifier;
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<ChangeAnnotationIdentifier>> annotationId;
  };

  enum class DocumentChangeType {
    TEXT_DOCUMENT_EDIT,
    CREATE_FILE,
    RENAME_FILE,
    DELETE_FILE,
  };

  struct DocumentChange {
    DocumentChangeType type;
    union {
      std::unique_ptr<TextDocumentEdit> textDocumentEdit;
      std::unique_ptr<CreateFile> createFile;
      std::unique_ptr<RenameFile> renameFile;
      std::unique_ptr<DeleteFile> deleteFile;
    };
  };

  /**
   * A workspace edit represents changes to many resources managed in the
   * workspace. The edit should either provide changes or documentChanges. If
   * the client can handle versioned document edits and if documentChanges are
   * present, the latter are preferred over changes.
   *
   * Since version 3.13.0 a workspace edit can contain resource operations
   * (create, delete or rename files and folders) as well. If resource
   * operations are present clients need to execute the operations in the order
   * in which they are provided. So a workspace edit for example can consist of
   * the following two changes: (1) create file a.txt and (2) a text document
   * edit which insert text into file a.txt. An invalid sequence (e.g. (1)
   * delete file a.txt and (2) insert text into file a.txt) will cause failure
   * of the operation. How the client recovers from the failure is described by
   * the client capability: workspace.workspaceEdit.failureHandling
   *
   * export interface WorkspaceEdit {
   *   changes?: { [uri: DocumentUri]: TextEdit[]; };
   *   documentChanges?: (
   *     TextDocumentEdit[] |
   *     (TextDocumentEdit | CreateFile | RenameFile | DeleteFile)[]
   *   );
   *   changeAnnotations?: {
   *     [id: string]: ChangeAnnotation;
   *   };
   * }
   */
  struct WorkspaceEdit {

    /**
     * Holds changes to existing resources.
     *
     * changes?: { [uri: DocumentUri]: TextEdit[]; };
     */
    std::optional<std::unique_ptr<std::map<DocumentUri, std::unique_ptr<std::vector<TextEdit>>>>> changes;

    /**
     * Depending on the client capability
     * `workspace.workspaceEdit.resourceOperations` document changes are either
     * an array of `TextDocumentEdit`s to express changes to n different text
     * documents where each text document edit addresses a specific version of a
     * text document. Or it can contain above `TextDocumentEdit`s mixed with
     * create, rename and delete file / folder operations.
     *
     * Whether a client supports versioned document edits is expressed via
     * `workspace.workspaceEdit.documentChanges` client capability.
     *
     * If a client neither supports `documentChanges` nor
     * `workspace.workspaceEdit.resourceOperations` then only plain `TextEdit`s
     * using the `changes` property are supported.
     *
     * documentChanges?: (
     *   TextDocumentEdit[] |
     *   (TextDocumentEdit | CreateFile | RenameFile | DeleteFile)[]
     * );
     */
    std::optional<std::unique_ptr<std::vector<std::unique_ptr<DocumentChange>>>> documentChanges;

    /**
     * A map of change annotations that can be referenced in
     * `AnnotatedTextEdit`s or create, rename and delete file / folder
     * operations.
     *
     * Whether clients honor this property depends on the client capability
     * `workspace.changeAnnotationSupport`.
     *
     * changeAnnotations?: {
     *   [id: string]: ChangeAnnotation;
     * };
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<std::map<string, std::unique_ptr<ChangeAnnotation>>>> changeAnnotations;
  };

  /**
   * The kind of resource operations supported by the client.
   *
   * export type ResourceOperationKind = 'create' | 'rename' | 'delete';
   * export namespace ResourceOperationKind {
   *   export const Create: ResourceOperationKind = 'create';
   *   export const Rename: ResourceOperationKind = 'rename';
   *   export const Delete: ResourceOperationKind = 'delete';
   * }
   */
  enum class ResourceOperationKind {

    /**
     * Supports creating new files and folders.
     *
     * export const Create: ResourceOperationKind = 'create';
     */
    Create,

    /**
     * Supports renaming existing files and folders.
     *
     * export const Rename: ResourceOperationKind = 'rename';
     */
    Rename,

    /**
     * Supports deleting existing files and folders.
     *
     * export const Delete: ResourceOperationKind = 'delete';
     */
    Delete,
  };

  extern std::map<ResourceOperationKind, std::string> ResourceOperationKindNames;

  auto resourceOperationKind(const std::string &name) -> ResourceOperationKind;

  /**
   * export type FailureHandlingKind = 'abort' | 'transactional' | 'undo'
   *   | 'textOnlyTransactional';
   * export namespace FailureHandlingKind {
   *   export const Abort: FailureHandlingKind = 'abort';
   *   export const Transactional: FailureHandlingKind = 'transactional';
   *   export const TextOnlyTransactional: FailureHandlingKind
   *     = 'textOnlyTransactional';
   *   export const Undo: FailureHandlingKind = 'undo';
   * }
   */
  enum class FailureHandlingKind {

    /**
     * Applying the workspace change is simply aborted if one of the changes
     * provided fails. All operations executed before the failing operation stay
     * executed.
     *
     * export const Abort: FailureHandlingKind = 'abort';
     */
    Abort,

    /**
     * All operations are executed transactional. That means they either all
     * succeed or no changes at all are applied to the workspace.
     *
     * export const Transactional: FailureHandlingKind = 'transactional';
     */
    Transactional,

    /**
     * If the workspace edit contains only textual file changes they are
     * executed transactional. If resource changes (create, rename or delete
     * file) are part of the change the failure handling strategy is abort.
     *
     * export const TextOnlyTransactional: FailureHandlingKind
     *   = 'textOnlyTransactional';
     */
    TextOnlyTransactional,

    /**
     * The client tries to undo the operations already executed. But there is no
     * guarantee that this is succeeding.
     *
     * export const Undo: FailureHandlingKind = 'undo';
     */
    Undo,
  };

  extern std::map<FailureHandlingKind, std::string> FailureHandlingKindNames;

  auto failureHandlingKind(const std::string &name) -> FailureHandlingKind;

  struct ChangeAnnotationSupport {

    /**
     * Whether the client groups edits with equal labels into tree nodes,
     * for instance all edits labelled with "Changes in Strings" would
     * be a tree node.
     *
     * groupsOnLabel?: boolean;
     */
    std::optional<boolean> groupsOnLabel;
  };

  /**
   * New in version 3.13: ResourceOperationKind and FailureHandlingKind and the
   * client capability workspace.workspaceEdit.resourceOperations as well as
   * workspace.workspaceEdit.failureHandling.
   *
   * The capabilities of a workspace edit has evolved over the time. Clients can
   * describe their support using the following client capability:
   *
   * Client Capability:
   *   property path (optional): workspace.workspaceEdit
   *   property type: WorkspaceEditClientCapabilities
   *
   * export interface WorkspaceEditClientCapabilities {
   *   documentChanges?: boolean;
   *   resourceOperations?: ResourceOperationKind[];
   *   failureHandling?: FailureHandlingKind;
   *   normalizesLineEndings?: boolean;
   *   changeAnnotationSupport?: {
   *     groupsOnLabel?: boolean;
   *   };
   * }
   */
  struct WorkspaceEditClientCapabilities {

    /**
     * The client supports versioned document changes in `WorkspaceEdit`s
     *
     * documentChanges?: boolean;
     */
    std::optional<boolean> documentChanges;

    /**
     * The resource operations the client supports. Clients should at least
     * support 'create', 'rename' and 'delete' files and folders.
     *
     * resourceOperations?: ResourceOperationKind[];
     *
     * @since 3.13.0
     */
    std::optional<std::unique_ptr<std::vector<std::unique_ptr<ResourceOperationKind>>>> resourceOperations;

    /**
     * The failure handling strategy of a client if applying the workspace edit
     * fails.
     *
     * failureHandling?: FailureHandlingKind;
     *
     * @since 3.13.0
     */
    std::optional<std::unique_ptr<FailureHandlingKind>> failureHandling;

    /**
     * Whether the client normalizes line endings to the client specific
     * setting. If set to `true` the client will normalize line ending
     * characters in a workspace edit to the client specific new line
     * character(s).
     *
     * normalizesLineEndings?: boolean;
     *
     * @since 3.16.0
     */
    std::optional<boolean> normalizesLineEndings;

    /**
     * Whether the client in general supports change annotations on text edits,
     * create file, rename file and delete file changes.
     *
     * changeAnnotationSupport?: {
     *   groupsOnLabel?: boolean;
     * };
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<ChangeAnnotationSupport>> changeAnnotationSupport;
  };

  // Work done progress is reported using the generic $/progress notification.
  // The value payload of a work done progress notification can be of three
  // different forms.

  /**
   * To start progress reporting a $/progress notification with the following
   * payload must be sent:
   *
   * export interface WorkDoneProgressBegin {
   *   kind: 'begin';
   *   title: string;
   *   cancellable?: boolean;
   *   message?: string;
   *   percentage?: uinteger;
   * }
   */
  struct WorkDoneProgressBegin {

    /**
     * kind: 'begin';
     */
    const std::string kind = "begin";

    /**
     * Mandatory title of the progress operation. Used to briefly inform about
     * the kind of operation being performed.
     *
     * Examples: "Indexing" or "Linking dependencies".
     *
     * title: string;
     */
    string title;

    /**
     * Controls if a cancel button should show to allow the user to cancel the
     * long running operation. Clients that don't support cancellation are
     * allowed to ignore the setting.
     *
     * cancellable?: boolean;
     */
    std::optional<boolean> cancellable;

    /**
     * Optional, more detailed associated progress message. Contains
     * complementary information to the `title`.
     *
     * Examples: "3/25 files", "project/src/module2", "node_modules/some_dep".
     * If unset, the previous progress message (if any) is still valid.
     *
     * message?: string;
     */
    std::optional<string> message;

    /**
     * Optional progress percentage to display (value 100 is considered 100%).
     * If not provided infinite progress is assumed and clients are allowed to
     * ignore the `percentage` value in subsequent report notifications.
     *
     * The value should be steadily rising. Clients are free to ignore values
     * that are not following this rule. The value range is [0, 100].
     *
     * percentage?: uinteger;
     */
    std::optional<uinteger> percentage;
  };

  /**
   * Reporting progress is done using the following payload.
   *
   * export interface WorkDoneProgressReport {
   *   kind: 'report';
   *   cancellable?: boolean;
   *   message?: string;
   *   percentage?: uinteger;
   * }
   */
  struct WorkDoneProgressReport {

    /**
     * kind: 'report';
     */
    const std::string kind = "report";

    /**
     * Controls enablement state of a cancel button. This property is only valid
     * if a cancel button got requested in the `WorkDoneProgressBegin` payload.
     *
     * Clients that don't support cancellation or don't support control the
     * button's enablement state are allowed to ignore the setting.
     *
     * cancellable?: boolean;
     */
    std::optional<boolean> cancellable;

    /**
     * Optional, more detailed associated progress message. Contains
     * complementary information to the `title`.
     *
     * Examples: "3/25 files", "project/src/module2", "node_modules/some_dep".
     * If unset, the previous progress message (if any) is still valid.
     *
     * message?: string;
     */
    std::optional<string> message;

    /**
     * Optional progress percentage to display (value 100 is considered 100%).
     * If not provided infinite progress is assumed and clients are allowed
     * to ignore the `percentage` value in subsequent report notifications.
     *
     * The value should be steadily rising. Clients are free to ignore values
     * that are not following this rule. The value range is [0, 100].
     *
     * percentage?: uinteger;
     */
    std::optional<uinteger> percentage;
  };

  /**
   * Signaling the end of a progress reporting is done using the following payload.
   *
   * export interface WorkDoneProgressEnd {
   *   kind: 'end';
   *   message?: string;
   * }
   */
  struct WorkDoneProgressEnd {

    /**
     * kind: 'end';
     */
    const std::string kind = "end";

    /**
     * Optional, a final message indicating to for example indicate the outcome
     * of the operation.
     *
     * message?: string;
     */
    std::optional<string> message;
  };

  // ---------------------------------------------------------------------------
  // Work Done progress can be initiated in two different ways:
  // 1. By the sender of a request (mostly clients) using the predefined
  // workDoneToken property in the requests parameter literal. The document will
  // refer to this kind of progress as client initiated progress.
  // 2. By a server using the request window/workDoneProgress/create. The
  // document will refer to this kind of progress as server initiated progress.
  // ---------------------------------------------------------------------------

  /**
   * Consider a client sending a textDocument/reference request to a server and
   * the client accepts work done progress reporting on that request. To signal
   * this to the server the client would add a workDoneToken property to the
   * reference request parameters.
   *
   * The corresponding type definition for the parameter property looks like this:
   *
   * export interface WorkDoneProgressParams {
   *   workDoneToken?: ProgressToken;
   * }
   */
  struct WorkDoneProgressParams {

    /**
     * An optional token that a server can use to report work done progress.
     *
     * workDoneToken?: ProgressToken;
     */
    std::optional<std::unique_ptr<ProgressToken>> workDoneToken;
  };

  /**
   * A server uses the workDoneToken to report progress for the specific
   * textDocument/reference.
   *
   * The token received via the workDoneToken property in a request’s param
   * literal is only valid as long as the request has not send a response back.
   * Canceling work done progress is done by simply canceling the corresponding
   * request.
   *
   * There is no specific client capability signaling whether a client will send
   * a progress token per request. The reason for this is that this is in many
   * clients not a static aspect and might even change for every request
   * instance for the same request type. So the capability is signal on every
   * request instance by the presence of a workDoneToken property.
   *
   * To avoid that clients set up a progress monitor user interface before
   * sending a request but the server doesn’t actually report any progress a
   * server needs to signal general work done progress reporting support in the
   * corresponding server capability. For the above find references example a
   * server would signal such a support by setting the referencesProvider
   * property in the server capabilities
   *
   * The corresponding type definition for the server capability looks like
   * this:
   *
   * export interface WorkDoneProgressOptions {
   *   workDoneProgress?: boolean;
   * }
   */
  struct WorkDoneProgressOptions {
    std::optional<boolean> workDoneProgress;
  };

  /**
   * A parameter literal used to pass a partial result token.
   *
   * export interface PartialResultParams {
   *   partialResultToken?: ProgressToken;
   * }
   */
  struct PartialResultParams {

    /**
     * An optional token that a server can use to report partial results (e.g.
     * streaming) to the client.
     *
     * partialResultToken?: ProgressToken;
     */
    std::optional<std::unique_ptr<ProgressToken>> partialResultToken;
  };

  /**
   * A TraceValue represents the level of verbosity with which the server
   * systematically reports its execution trace using $/logTrace notifications.
   * The initial trace value is set by the client at initialization and can be
   * modified later using the $/setTrace notification.
   */
  enum class TraceValue {
    OFF,
    MESSAGES,
    VERBOSE,
  };

  extern std::map<TraceValue, std::string> TraceValueNames;

  auto traceValue(const std::string &name) -> TraceValue;

  /**
   * Information about the client
   *
   * @since 3.15.0
   */
  struct ClientInfo {

    /**
     * The name of the client as defined by the client.
     *
     * name: string;
     */
    string name;

    /**
     * The client's version as defined by the client.
     *
     * version?: string;
     */
    std::optional<string> version;
  };

  /**
   * Document renames should be signaled to a server sending a document close
   * notification with the document’s old name followed by an open notification
   * using the document’s new name. Major reason is that besides the name other
   * attributes can change as well like the language that is associated with the
   * document. In addition the new document could not be of interest for the
   * server anymore.
   *
   * Servers can participate in a document rename by subscribing for the
   * workspace/didRenameFiles notification or the workspace/willRenameFiles
   * request.
   *
   * The final structure of the TextDocumentSyncClientCapabilities and the
   * TextDocumentSyncOptions server options look like this:
   *
   * export interface TextDocumentSyncClientCapabilities {
   *   dynamicRegistration?: boolean;
   *   willSave?: boolean;
   *   willSaveWaitUntil?: boolean;
   *   didSave?: boolean;
   * }
   */
  struct TextDocumentSyncClientCapabilities {

    /**
     * Whether text document synchronization supports dynamic registration.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;

    /**
     * The client supports sending will save notifications.
     *
     * willSave?: boolean;
     */
    std::optional<boolean> willSave;

    /**
     * The client supports sending a will save request and waits for a response
     * providing text edits which will be applied to the document before it is
     * saved.
     *
     * willSaveWaitUntil?: boolean;
     */
    std::optional<boolean> willSaveWaitUntil;

    /**
     * The client supports did save notifications.
     *
     * didSave?: boolean;
     */
    std::optional<boolean> didSave;
  };

  // Language Features provide the actual smarts in the language server
  // protocol. They are usually executed on a [text document, position] tuple.
  // The main language feature categories are:
  // - code comprehension features like Hover or Goto Definition.
  // - coding features like diagnostics, code complete or code actions.
  // The language features should be computed on the synchronized state of the
  // document.

  /**
   * The go to declaration request is sent from the client to the server to
   * resolve the declaration location of a symbol at a given text document
   * position.
   *
   * The result type LocationLink[] got introduced with version 3.14.0 and
   * depends on the corresponding client capability
   * textDocument.declaration.linkSupport.
   *
   * Client Capability:
   *   property name (optional): textDocument.declaration
   *   property type: DeclarationClientCapabilities defined as follows:
   *
   * export interface DeclarationClientCapabilities {
   *   dynamicRegistration?: boolean;
   *   linkSupport?: boolean;
   * }
   */
  struct DeclarationClientCapabilities {

    /**
     * Whether declaration supports dynamic registration. If this is set to
     * `true` the client supports the new `DeclarationRegistrationOptions`
     * return value for the corresponding server capability as well.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;

    /**
     * The client supports additional metadata in the form of declaration links.
     *
     * linkSupport?: boolean;
     */
    std::optional<boolean> linkSupport;
  };

  /**
   * Server Capability:
   * - property name (optional): declarationProvider
   * - property type: boolean | DeclarationOptions |
   *   DeclarationRegistrationOptions where DeclarationOptions is defined as
   *   follows:
   *
   * export interface DeclarationOptions extends WorkDoneProgressOptions {
   * }
   */
  struct DeclarationOptions : public WorkDoneProgressOptions {
    // empty
  };

  /**
   * Registration Options: DeclarationRegistrationOptions defined as follows:
   *
   * export interface DeclarationRegistrationOptions extends DeclarationOptions,
   *   TextDocumentRegistrationOptions, StaticRegistrationOptions {
   * }
   */
  struct DeclarationRegistrationOptions
    : public DeclarationOptions
    , public TextDocumentRegistrationOptions
    , public StaticRegistrationOptions {
    // empty
  };

  /**
   * Request:
   * - method: textDocument/declaration
   * - params: DeclarationParams defined as follows:
   *
   * export interface DeclarationParams extends TextDocumentPositionParams,
   *   WorkDoneProgressParams, PartialResultParams {
   * }
   *
   * Response:
   * - result: Location | Location[] | LocationLink[] |null
   * - partial result: Location[] | LocationLink[]
   * - error: code and message set in case an exception happens during the
   *   declaration request.
   */
  struct DeclarationParams
    : public TextDocumentPositionParams
    , public WorkDoneProgressParams
    , public PartialResultParams {
    // empty
  };

  /**
   * The go to definition request is sent from the client to the server to
   * resolve the definition location of a symbol at a given text document
   * position.
   *
   * The result type LocationLink[] got introduced with version 3.14.0 and
   * depends on the corresponding client capability
   * textDocument.definition.linkSupport.
   *
   * Client Capability:
   * - property name (optional): textDocument.definition
   * - property type: DefinitionClientCapabilities defined as follows:
   *
   * export interface DefinitionClientCapabilities {
   *   dynamicRegistration?: boolean;
   *   linkSupport?: boolean;
   * }
   */
  struct DefinitionClientCapabilities {

    /**
     * Whether definition supports dynamic registration.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;

    /**
     * The client supports additional metadata in the form of definition links.
     *
     * linkSupport?: boolean;
     *
     * @since 3.14.0
     */
    std::optional<boolean> linkSupport;
  };

  /**
   * Server Capability:
   * - property name (optional): definitionProvider
   * - property type: boolean | DefinitionOptions where DefinitionOptions is
   *   defined as follows:
   *
   * export interface DefinitionOptions extends WorkDoneProgressOptions {
   * }
   */
  struct DefinitionOptions : public WorkDoneProgressOptions {
    // empty
  };

  /**
   * Registration Options: DefinitionRegistrationOptions defined as follows:
   *
   * export interface DefinitionRegistrationOptions extends
   *   TextDocumentRegistrationOptions, DefinitionOptions {
   * }
   */
  struct DefinitionRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public DefinitionOptions {
    //empty
  };

  /**
   * Request:
   * - method: textDocument/definition
   * - params: DefinitionParams defined as follows:
   *
   * export interface DefinitionParams extends TextDocumentPositionParams,
   *   WorkDoneProgressParams, PartialResultParams {
   * }
   *
   * Response:
   * - result: Location | Location[] | LocationLink[] | null
   * - partial result: Location[] | LocationLink[]
   * - error: code and message set in case an exception happens during the
   *   definition request.
   */
  struct DefinitionParams
    : public TextDocumentPositionParams
    , public WorkDoneProgressParams
    , public PartialResultParams {
    // empty
  };

  /**
   * The go to type definition request is sent from the client to the server to
   * resolve the type definition location of a symbol at a given text document
   * position.
   *
   * The result type LocationLink[] got introduced with version 3.14.0 and
   * depends on the corresponding client capability
   * textDocument.typeDefinition.linkSupport.
   *
   * Client Capability:
   * - property name (optional): textDocument.typeDefinition
   * - property type: TypeDefinitionClientCapabilities defined as follows:
   *
   * export interface TypeDefinitionClientCapabilities {
   *   dynamicRegistration?: boolean;
   *   linkSupport?: boolean;
   * }
   */
  struct TypeDefinitionClientCapabilities {

    /**
     * Whether implementation supports dynamic registration. If this is set to
     * `true` the client supports the new `TypeDefinitionRegistrationOptions`
     * return value for the corresponding server capability as well.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;

    /**
     * The client supports additional metadata in the form of definition links.
     *
     * linkSupport?: boolean;
     *
     * @since 3.14.0
     */
    std::optional<boolean> linkSupport;
  };

  /**
   * Server Capability:
   * - property name (optional): typeDefinitionProvider
   * - property type: boolean | TypeDefinitionOptions |
   *   TypeDefinitionRegistrationOptions where TypeDefinitionOptions is defined
   *   as follows:
   *
   * export interface TypeDefinitionOptions extends WorkDoneProgressOptions {
   * }
   */
  struct TypeDefinitionOptions : public WorkDoneProgressOptions {
    // empty
  };

  /**
   * Registration Options: TypeDefinitionRegistrationOptions defined as follows:
   *
   * export interface TypeDefinitionRegistrationOptions extends
   *   TextDocumentRegistrationOptions, TypeDefinitionOptions,
   *   StaticRegistrationOptions {
   * }
   */
  struct TypeDefinitionRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public TypeDefinitionOptions
    , public StaticRegistrationOptions {
    // empty
  };

  /**
   * Request:
   * - method: textDocument/typeDefinition
   * - params: TypeDefinitionParams defined as follows:
   *
   * export interface TypeDefinitionParams extends TextDocumentPositionParams,
   *   WorkDoneProgressParams, PartialResultParams {
   * }
   *
   * Response:
   * - result: Location | Location[] | LocationLink[] | null
   * - partial result: Location[] | LocationLink[]
   * - error: code and message set in case an exception happens during the
   *   definition request.
   */
  struct TypeDefinitionParams
    : public TextDocumentPositionParams
    , public WorkDoneProgressParams
    , public PartialResultParams {
    // empty
  };

  /**
   * The go to implementation request is sent from the client to the server to
   * resolve the implementation location of a symbol at a given text document
   * position.
   *
   * The result type LocationLink[] got introduced with version 3.14.0 and
   * depends on the corresponding client capability
   * textDocument.implementation.linkSupport.
   *
   * Client Capability:
   * - property name (optional): textDocument.implementation
   * - property type: ImplementationClientCapabilities defined as follows:
   *
   * export interface ImplementationClientCapabilities {
   *   dynamicRegistration?: boolean;
   *   linkSupport?: boolean;
   * }
   */
  struct ImplementationClientCapabilities {

    /**
     * Whether implementation supports dynamic registration. If this is set to
     * `true` the client supports the new `ImplementationRegistrationOptions`
     * return value for the corresponding server capability as well.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;

    /**
     * The client supports additional metadata in the form of definition links.
     *
     * linkSupport?: boolean;
     *
     * @since 3.14.0
     */
    std::optional<boolean> linkSupport;
  };

  /**
   * Server Capability:
   * - property name (optional): implementationProvider
   * - property type: boolean | ImplementationOptions |
   *   ImplementationRegistrationOptions where ImplementationOptions is defined
   *   as follows:
   *
   * export interface ImplementationOptions extends WorkDoneProgressOptions {
   * }
   */
  struct ImplementationOptions : public WorkDoneProgressOptions {
    // empty
  };

  /**
   * Registration Options: ImplementationRegistrationOptions defined as follows:
   *
   * export interface ImplementationRegistrationOptions extends
   *   TextDocumentRegistrationOptions, ImplementationOptions,
   *   StaticRegistrationOptions {
   * }
   */
  struct ImplementationRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public ImplementationOptions
    , public StaticRegistrationOptions {
    // empty
  };

  /**
   * Request:
   * - method: textDocument/implementation
   * - params: ImplementationParams defined as follows:
   *
   * export interface ImplementationParams extends TextDocumentPositionParams,
   *   WorkDoneProgressParams, PartialResultParams {
   * }
   *
   * Response:
   * - result: Location | Location[] | LocationLink[] | null
   * - partial result: Location[] | LocationLink[]
   * - error: code and message set in case an exception happens during the
   *   definition request.
   */
  struct ImplementationParams
    : public TextDocumentPositionParams
    , public WorkDoneProgressParams
    , public PartialResultParams {
    // empty
  };

  /**
   * The references request is sent from the client to the server to resolve
   * project-wide references for the symbol denoted by the given text document
   * position.
   *
   * Client Capability:
   * - property name (optional): textDocument.references
   * - property type: ReferenceClientCapabilities defined as follows:
   *
   * export interface ReferenceClientCapabilities {
   *   dynamicRegistration?: boolean;
   * }
   */
  struct ReferenceClientCapabilities {

    /**
     * Whether references supports dynamic registration.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;
  };

  /**
   * Server Capability:
   * - property name (optional): referencesProvider
   * - property type: boolean | ReferenceOptions where ReferenceOptions is
   *   defined as follows:
   *
   * export interface ReferenceOptions extends WorkDoneProgressOptions {
   * }
   */
  struct ReferenceOptions : public WorkDoneProgressOptions {
    // empty
  };

  /**
   * Registration Options: ReferenceRegistrationOptions defined as follows:
   *
   * export interface ReferenceRegistrationOptions extends
   *   TextDocumentRegistrationOptions, ReferenceOptions {
   * }
   */
  struct ReferenceRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public ReferenceOptions {
    // empty
  };

  /**
   * export interface ReferenceContext {
   *   includeDeclaration: boolean;
   * }
   */
  struct ReferenceContext {

    /**
     * Include the declaration of the current symbol.
     *
     * includeDeclaration: boolean;
     */
    boolean includeDeclaration;
  };

  /**
   * Request:
   * - method: textDocument/references
   * - params: ReferenceParams defined as follows:
   *
   * export interface ReferenceParams extends TextDocumentPositionParams,
   *   WorkDoneProgressParams, PartialResultParams {
   *   context: ReferenceContext;
   * }
   *
   * Response:
   * - result: Location[] | null
   * - partial result: Location[]
   * - error: code and message set in case an exception happens during the
   *   reference request.
   */
  struct ReferenceParams
    : public TextDocumentPositionParams
    , public WorkDoneProgressParams
    , public PartialResultParams {

    /**
     * context: ReferenceContext;
     */
    std::unique_ptr<ReferenceContext> context;
  };

  /**
   * The call hierarchy request is sent from the client to the server to return
   * a call hierarchy for the language element of given text document positions.
   * The call hierarchy requests are executed in two steps:
   * 1. first a call hierarchy item is resolved for the given text document
   *    position
   * 2. for a call hierarchy item the incoming or outgoing call hierarchy items
   *    are resolved.
   *
   * Client Capability:
   * - property name (optional): textDocument.callHierarchy
   * - property type: CallHierarchyClientCapabilities defined as follows:
   *
   * interface CallHierarchyClientCapabilities {
   *   dynamicRegistration?: boolean;
   * }
   */
  struct CallHierarchyClientCapabilities {

    /**
     * Whether implementation supports dynamic registration. If this is set to
     * `true` the client supports the new `(TextDocumentRegistrationOptions &
     * StaticRegistrationOptions)` return value for the corresponding server
     * capability as well.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;
  };

  /**
   * Server Capability:
   * - property name (optional): callHierarchyProvider
   * - property type: boolean | CallHierarchyOptions |
   *   CallHierarchyRegistrationOptions where CallHierarchyOptions is defined as
   *   follows:
   *
   * export interface CallHierarchyOptions extends WorkDoneProgressOptions {
   * }
   */
  struct CallHierarchyOptions : public WorkDoneProgressOptions {
    // empty
  };

  /**
   * Registration Options: CallHierarchyRegistrationOptions defined as follows:
   *
   * export interface CallHierarchyRegistrationOptions extends
   *   TextDocumentRegistrationOptions, CallHierarchyOptions,
   *   StaticRegistrationOptions {
   * }
   */
  struct CallHierarchyRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public CallHierarchyOptions
    , public StaticRegistrationOptions {
    // empty
  };

  /**
   * Request:
   * - method: textDocument/prepareCallHierarchy
   * - params: CallHierarchyPrepareParams defined as follows:
   *
   * export interface CallHierarchyPrepareParams extends TextDocumentPositionParams,
   *   WorkDoneProgressParams {
   * }
   *
   * Response:
   * - result: CallHierarchyItem[] | null defined as follows:
   */
  struct CallHierarchyPrepareParams
    : public TextDocumentPositionParams
    , public WorkDoneProgressParams {
    // empty
  };

  /**
   * export interface CallHierarchyItem {
   *   name: string;
   *   kind: SymbolKind;
   *   tags?: SymbolTag[];
   *   detail?: string;
   *   uri: DocumentUri;
   *   range: Range;
   *   selectionRange: Range;
   *   data?: LSPAny;
   * }
   */
  struct CallHierarchyItem {

    /**
     * The name of this item.
     *
     * name: string;
     */
    string name;

    /**
     * The kind of this item.
     *
     * kind: SymbolKind;
     */
    SymbolKind kind;

    /**
     * Tags for this item.
     *
     * tags?: SymbolTag[];
     */
    std::optional<std::unique_ptr<std::vector<std::unique_ptr<SymbolTag>>>> tags;

    /**
     * More detail for this item, e.g. the signature of a function.
     *
     * detail?: string;
     */
    std::optional<string> detail;

    /**
     * The resource identifier of this item.
     *
     * uri: DocumentUri;
     */
    DocumentUri uri;

    /**
     * The range enclosing this symbol not including leading/trailing whitespace
     * but everything else, e.g. comments and code.
     *
     * range: Range;
     */
    std::unique_ptr<Range> range;

    /**
     * The range that should be selected and revealed when this symbol is being
     * picked, e.g. the name of a function. Must be contained by the
     * [`range`](#CallHierarchyItem.range).
     *
     * selectionRange: Range;
     */
    std::unique_ptr<Range> selectionRange;

    /**
     * A data entry field that is preserved between a call hierarchy prepare and
     * incoming calls or outgoing calls requests.
     *
     * data?: LSPAny;
     */
    std::optional<std::unique_ptr<LSPAny>> data;
  };

  /**
   * The request is sent from the client to the server to resolve incoming calls
   * for a given call hierarchy item. The request doesn’t define its own client
   * and server capabilities. It is only issued if a server registers for the
   * textDocument/prepareCallHierarchy request.
   *
   * Request:
   * - method: callHierarchy/incomingCalls
   * - params: CallHierarchyIncomingCallsParams defined as follows:
   *
   * export interface CallHierarchyIncomingCallsParams extends
   *   WorkDoneProgressParams, PartialResultParams {
   *   item: CallHierarchyItem;
   * }
   */
  struct CallHierarchyIncomingCallsParams
    : public WorkDoneProgressParams
    , public PartialResultParams {

    /**
     * item: CallHierarchyItem;
     */
    std::unique_ptr<CallHierarchyItem> item;
  };

  /**
   * Response:
   * - result: CallHierarchyIncomingCall[] | null defined as follows:
   *
   * export interface CallHierarchyIncomingCall {
   *   from: CallHierarchyItem;
   *   fromRanges: Range[];
   * }
   *
   * - partial result: CallHierarchyIncomingCall[]
   * - error: code and message set in case an exception happens during the
   *   ‘callHierarchy/incomingCalls’ request
   */
  struct CallHierarchyIncomingCall {

    /**
     * The item that makes the call.
     *
     * from: CallHierarchyItem;
     */
    std::unique_ptr<CallHierarchyItem> from;

    /**
     * The ranges at which the calls appear. This is relative to the caller
     * denoted by [`this.from`](#CallHierarchyIncomingCall.from).
     *
     * fromRanges: Range[];
     */
    std::unique_ptr<std::vector<std::unique_ptr<Range>>> fromRanges;
  };

  /**
   * The request is sent from the client to the server to resolve outgoing calls
   * for a given call hierarchy item. The request doesn’t define its own client
   * and server capabilities. It is only issued if a server registers for the
   * textDocument/prepareCallHierarchy request.
   *
   * Request:
   * - method: callHierarchy/outgoingCalls
   * - params: CallHierarchyOutgoingCallsParams defined as follows:
   *
   * export interface CallHierarchyOutgoingCallsParams extends
   *   WorkDoneProgressParams, PartialResultParams {
   *   item: CallHierarchyItem;
   * }
   */
  struct CallHierarchyOutgoingCallsParams
    : public WorkDoneProgressParams
    , public PartialResultParams {

    /**
     * item: CallHierarchyItem;
     */
    std::unique_ptr<CallHierarchyItem> item;
  };

  /**
   * Response:
   * - result: CallHierarchyOutgoingCall[] | null defined as follows:
   *
   * export interface CallHierarchyOutgoingCall {
   *   to: CallHierarchyItem;
   *   fromRanges: Range[];
   * }
   *
   * - partial result: CallHierarchyOutgoingCall[]
   * - error: code and message set in case an exception happens during the
   *   ‘callHierarchy/outgoingCalls’ request
   */
  struct CallHierarchyOutgoingCall {

    /**
     * The item that is called.
     *
     * to: CallHierarchyItem;
     */
    std::unique_ptr<CallHierarchyItem> to;

    /**
     * The range at which this item is called. This is the range relative to the
     * caller, e.g the item passed to `callHierarchy/outgoingCalls` request.
     *
     * fromRanges: Range[];
     */
    std::unique_ptr<std::vector<std::unique_ptr<Range>>> fromRanges;
  };

  /**
   * The type hierarchy request is sent from the client to the server to return
   * a type hierarchy for the language element of given text document positions.
   * Will return null if the server couldn’t infer a valid type from the
   * position. The type hierarchy requests are executed in two steps:
   * 1. first a type hierarchy item is prepared for the given text document position.
   * 2. for a type hierarchy item the supertype or subtype type hierarchy items
   *    are resolved.
   *
   * Client Capability:
   * - property name (optional): textDocument.typeHierarchy
   * - property type: TypeHierarchyClientCapabilities defined as follows:
   *
   * type TypeHierarchyClientCapabilities = {
   *   dynamicRegistration?: boolean;
   * };
   */
  struct TypeHierarchyClientCapabilities {

    /**
     * Whether implementation supports dynamic registration. If this is set to
     * `true` the client supports the new `(TextDocumentRegistrationOptions &
     * StaticRegistrationOptions)` return value for the corresponding server
     * capability as well.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;
  };

  /**
   * Server Capability:
   * - property name (optional): typeHierarchyProvider
   * - property type: boolean | TypeHierarchyOptions |
   *   TypeHierarchyRegistrationOptions where TypeHierarchyOptions is defined as
   *   follows:
   *
   * export interface TypeHierarchyOptions extends WorkDoneProgressOptions {
   * }
   */
  struct TypeHierarchyOptions : public WorkDoneProgressOptions {
    // empty
  };

  /**
   * Registration Options: TypeHierarchyRegistrationOptions defined as follows:
   *
   * export interface TypeHierarchyRegistrationOptions extends
   *   TextDocumentRegistrationOptions, TypeHierarchyOptions,
   *   StaticRegistrationOptions {
   * }
   */
  struct TypeHierarchyRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public TypeHierarchyOptions
    , public StaticRegistrationOptions {
    // empty
  };

  /**
   * Request:
   * - method: ‘textDocument/prepareTypeHierarchy’
   * - params: TypeHierarchyPrepareParams defined as follows:
   *
   * export interface TypeHierarchyPrepareParams extends TextDocumentPositionParams,
   *   WorkDoneProgressParams {
   * }
   */
  struct TypeHierarchyPrepareParams
    : public TextDocumentPositionParams
    , public WorkDoneProgressParams {
    // empty
  };

  /**
   * Response:
   * - result: TypeHierarchyItem[] | null defined as follows:
   *
   * export interface TypeHierarchyItem {
   *   name: string;
   *   kind: SymbolKind;
   *   tags?: SymbolTag[];
   *   detail?: string;
   *   uri: DocumentUri;
   *   range: Range;
   *   selectionRange: Range;
   *   data?: LSPAny;
   * }
   *
   * - error: code and message set in case an exception happens during the
   *   ‘textDocument/prepareTypeHierarchy’ request
   */
  struct TypeHierarchyItem {

    /**
     * The name of this item.
     *
     * name: string;
     */
    string name;

    /**
     * The kind of this item.
     *
     * kind: SymbolKind;
     */
    SymbolKind kind;

    /**
     * Tags for this item.
     *
     * tags?: SymbolTag[];
     */
    std::optional<std::unique_ptr<std::vector<std::unique_ptr<SymbolTag>>>> tags;

    /**
     * More detail for this item, e.g. the signature of a function.
     *
     * detail?: string;
     */
    std::optional<string> detail;

    /**
     * The resource identifier of this item.
     *
     * uri: DocumentUri;
     */
    DocumentUri uri;

    /**
     * The range enclosing this symbol not including leading/trailing whitespace
     * but everything else, e.g. comments and code.
     *
     * range: Range;
     */
    std::unique_ptr<Range> range;

    /**
     * The range that should be selected and revealed when this symbol is being
     * picked, e.g. the name of a function. Must be contained by the
     * [`range`](#TypeHierarchyItem.range).
     *
     * selectionRange: Range;
     */
    std::unique_ptr<Range> selectionRange;

    /**
     * A data entry field that is preserved between a type hierarchy prepare and
     * supertypes or subtypes requests. It could also be used to identify the
     * type hierarchy in the server, helping improve the performance on
     * resolving supertypes and subtypes.
     *
     * data?: LSPAny;
     */
    std::optional<std::unique_ptr<LSPAny>> data;
  };

  /**
   * The request is sent from the client to the server to resolve the supertypes
   * for a given type hierarchy item. Will return null if the server couldn’t
   * infer a valid type from item in the params. The request doesn’t define its
   * own client and server capabilities. It is only issued if a server registers
   * for the textDocument/prepareTypeHierarchy request.
   *
   * Request:
   * - method: ‘typeHierarchy/supertypes’
   * - params: TypeHierarchySupertypesParams defined as follows:
   *
   * export interface TypeHierarchySupertypesParams extends
   *   WorkDoneProgressParams, PartialResultParams {
   *   item: TypeHierarchyItem;
   * }
   *
   * Response:
   * - result: TypeHierarchyItem[] | null
   * - partial result: TypeHierarchyItem[]
   * - error: code and message set in case an exception happens during the
   *   ‘typeHierarchy/supertypes’ request
   */
  struct TypeHierarchySupertypesParams
    : public WorkDoneProgressParams
    , public PartialResultParams {

    /**
     * item: TypeHierarchyItem;
     */
    std::unique_ptr<TypeHierarchyItem> item;
  };

  /**
   * The request is sent from the client to the server to resolve the subtypes
   * for a given type hierarchy item. Will return null if the server couldn’t
   * infer a valid type from item in the params. The request doesn’t define its
   * own client and server capabilities. It is only issued if a server registers
   * for the textDocument/prepareTypeHierarchy request.
   *
   * Request:
   * - method: ‘typeHierarchy/subtypes’
   * - params: TypeHierarchySubtypesParams defined as follows:
   *
   * export interface TypeHierarchySubtypesParams extends
   *   WorkDoneProgressParams, PartialResultParams {
   *   item: TypeHierarchyItem;
   * }
   *
   * Response:
   * - result: TypeHierarchyItem[] | null
   * - partial result: TypeHierarchyItem[]
   * - error: code and message set in case an exception happens during the
   *   ‘typeHierarchy/subtypes’ request
   */
  struct TypeHierarchySubtypesParams
    : public WorkDoneProgressParams
    , public PartialResultParams {

    /**
     * item: TypeHierarchyItem;
     */
    std::unique_ptr<TypeHierarchyItem> item;
  };

  /**
   * Text document specific client capabilities.
   *
   * export interface TextDocumentClientCapabilities {
   *   synchronization?: TextDocumentSyncClientCapabilities;
   *   completion?: CompletionClientCapabilities;
   *   hover?: HoverClientCapabilities;
   *   signatureHelp?: SignatureHelpClientCapabilities;
   *   declaration?: DeclarationClientCapabilities;
   *   definition?: DefinitionClientCapabilities;
   *   typeDefinition?: TypeDefinitionClientCapabilities;
   *   implementation?: ImplementationClientCapabilities;
   *   references?: ReferenceClientCapabilities;
   *   documentHighlight?: DocumentHighlightClientCapabilities;
   *   documentSymbol?: DocumentSymbolClientCapabilities;
   *   codeAction?: CodeActionClientCapabilities;
   *   codeLens?: CodeLensClientCapabilities;
   *   documentLink?: DocumentLinkClientCapabilities;
   *   colorProvider?: DocumentColorClientCapabilities;
   *   formatting?: DocumentFormattingClientCapabilities;
   *   rangeFormatting?: DocumentRangeFormattingClientCapabilities;
   *   onTypeFormatting?: DocumentOnTypeFormattingClientCapabilities;
   *   rename?: RenameClientCapabilities;
   *   publishDiagnostics?: PublishDiagnosticsClientCapabilities;
   *   foldingRange?: FoldingRangeClientCapabilities;
   *   selectionRange?: SelectionRangeClientCapabilities;
   *   linkedEditingRange?: LinkedEditingRangeClientCapabilities;
   *   callHierarchy?: CallHierarchyClientCapabilities;
   *   semanticTokens?: SemanticTokensClientCapabilities;
   *   moniker?: MonikerClientCapabilities;
   *   typeHierarchy?: TypeHierarchyClientCapabilities;
   *   inlineValue?: InlineValueClientCapabilities;
   *   inlayHint?: InlayHintClientCapabilities;
   *   diagnostic?: DiagnosticClientCapabilities;
   * }
   */
  struct TextDocumentClientCapabilities {

    /**
     * synchronization?: TextDocumentSyncClientCapabilities;
     */
    std::optional<std::unique_ptr<TextDocumentSyncClientCapabilities>> synchronization;

    /**
     * Capabilities specific to the `textDocument/completion` request.
     *
     * completion?: CompletionClientCapabilities;
     */
    std::optional<std::unique_ptr<CompletionClientCapabilities>> completion;

    /**
     * Capabilities specific to the `textDocument/hover` request.
     *
     * hover?: HoverClientCapabilities;
     */
    std::optional<std::unique_ptr<HoverClientCapabilities>> hover;

    /**
     * Capabilities specific to the `textDocument/signatureHelp` request.
     *
     * signatureHelp?: SignatureHelpClientCapabilities;
     */
    std::optional<std::unique_ptr<SignatureHelpClientCapabilities>> signatureHelp;

    /**
     * Capabilities specific to the `textDocument/declaration` request.
     *
     * declaration?: DeclarationClientCapabilities;
     *
     * @since 3.14.0
     */
    std::optional<std::unqiue_ptr<DeclarationClientCapabilities>> declaration;

    /**
     * Capabilities specific to the `textDocument/definition` request.
     *
     * definition?: DefinitionClientCapabilities;
     */
    std::optional<std::unique_ptr<DefinitionClientCapabilities>> definition;

    /**
     * Capabilities specific to the `textDocument/typeDefinition` request.
     *
     * typeDefinition?: TypeDefinitionClientCapabilities;
     *
     * @since 3.6.0
     */
    std::optional<std::unique_ptr<TypeDefinitionClientCapabilities>> typeDefinition;

    /**
     * Capabilities specific to the `textDocument/implementation` request.
     *
     * implementation?: ImplementationClientCapabilities;
     *
     * @since 3.6.0
     */
    std::optional<std::unique_ptr<ImplementationClientCapabilities>> implementation;

    /**
     * Capabilities specific to the `textDocument/references` request.
     *
     * references?: ReferenceClientCapabilities;
     */
    std::optional<std::unique_ptr<ReferenceClientCapabilities>> references;

    /**
     * Capabilities specific to the `textDocument/documentHighlight` request.
     *
     * documentHighlight?: DocumentHighlightClientCapabilities;
     */
    std::optional<std::unique_ptr<DocumentHighlightClientCapabilities>> documentHighlight;

    /**
     * Capabilities specific to the `textDocument/documentSymbol` request.
     *
     * documentSymbol?: DocumentSymbolClientCapabilities;
     */
    std::optional<std::unique_ptr<DocumentSymbolClientCapabilities>> documentSymbol;

    /**
     * Capabilities specific to the `textDocument/codeAction` request.
     *
     * codeAction?: CodeActionClientCapabilities;
     */
    std::optional<std::unique_ptr<CodeActionClientCapabilities>> codeAction;

    /**
     * Capabilities specific to the `textDocument/codeLens` request.
     *
     * codeLens?: CodeLensClientCapabilities;
     */
    std::optional<std::unique_ptr<CodeLensClientCapabilities>> codeLens;

    /**
     * Capabilities specific to the `textDocument/documentLink` request.
     *
     * documentLink?: DocumentLinkClientCapabilities;
     */
    std::optional<std::unique_ptr<DocumentLinkClientCapabilities>> documentLink;

    /**
     * Capabilities specific to the `textDocument/documentColor` and the
     * `textDocument/colorPresentation` request.
     *
     * colorProvider?: DocumentColorClientCapabilities;
     *
     * @since 3.6.0
     */
    std::optional<std::unique_ptr<DocumentColorClientCapabilities>> colorProvider;

    /**
     * Capabilities specific to the `textDocument/formatting` request.
     *
     * formatting?: DocumentFormattingClientCapabilities;
     */
    std::optional<std::unique_ptr<DocumentFormattingClientCapabilities>> formatting;

    /**
     * Capabilities specific to the `textDocument/rangeFormatting` request.
     *
     * rangeFormatting?: DocumentRangeFormattingClientCapabilities;
     */
    std::optional<std::unique_ptr<DocumentRangeFormattingClientCapabilities>> rangeFormatting;

    /**
     * Capabilities specific to the `textDocument/onTypeFormatting` request.
     *
     * onTypeFormatting?: DocumentOnTypeFormattingClientCapabilities;
     */
    std::optional<std::unique_ptr<DocumentOnTypeFormattingClientCapabilities>> onTypeFormatting;

    /**
     * Capabilities specific to the `textDocument/rename` request.
     *
     * rename?: RenameClientCapabilities;
     */
    std::optional<std::unique_ptr<RenameClientCapabilities>> rename;

    /**
     * Capabilities specific to the `textDocument/publishDiagnostics`
     * notification.
     *
     * publishDiagnostics?: PublishDiagnosticsClientCapabilities;
     */
    std::optional<std::unique_ptr<PublishDiagnosticsClientCapabilities>> publishDiagnostics;

    /**
     * Capabilities specific to the `textDocument/foldingRange` request.
     *
     * foldingRange?: FoldingRangeClientCapabilities;
     *
     * @since 3.10.0
     */
    std::optional<std::unique_ptr<FoldingRangeClientCapabilities>> foldingRange;

    /**
     * Capabilities specific to the `textDocument/selectionRange` request.
     *
     * selectionRange?: SelectionRangeClientCapabilities;
     *
     * @since 3.15.0
     */
    std::optional<std::unique_ptr<SelectionRangeClientCapabilities>> selectionRange;

    /**
     * Capabilities specific to the `textDocument/linkedEditingRange` request.
     *
     * linkedEditingRange?: LinkedEditingRangeClientCapabilities;
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<LinkedEditingRangeClientCapabilities>> linkedEditingRange;

    /**
     * Capabilities specific to the various call hierarchy requests.
     *
     * callHierarchy?: CallHierarchyClientCapabilities;
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<CallHierarchyClientCapabilities>> callHierarchy;

    /**
     * Capabilities specific to the various semantic token requests.
     *
     * semanticTokens?: SemanticTokensClientCapabilities;
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<SemanticTokensClientCapabilities>> semanticTokens;

    /**
     * Capabilities specific to the `textDocument/moniker` request.
     *
     * moniker?: MonikerClientCapabilities;
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<MonikerClientCapabilities>> moniker;

    /**
     * Capabilities specific to the various type hierarchy requests.
     *
     * typeHierarchy?: TypeHierarchyClientCapabilities;
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<TypeHierarchyClientCapabilities>> typeHierarchy;

    /**
     * Capabilities specific to the `textDocument/inlineValue` request.
     *
     * inlineValue?: InlineValueClientCapabilities;
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<InlineValueClientCapabilities>> inlineValue;

    /**
     * Capabilities specific to the `textDocument/inlayHint` request.
     *
     * inlayHint?: InlayHintClientCapabilities;
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<InlayHintClientCapabilities>> inlayHint;

    /**
     * Capabilities specific to the diagnostic pull model.
     *
     * diagnostic?: DiagnosticClientCapabilities;
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<DiagnosticClientCapabilities>> diagnostic;
  };

  // Notebooks are becoming more and more popular. Adding support for them to
  // the language server protocol allows notebook editors to reuse language
  // smarts provided by the server inside a notebook or a notebook cell,
  // respectively. To reuse protocol parts and therefore server implementations
  // notebooks are modeled in the following way in LSP:
  // - notebook document: a collection of notebook cells typically stored in a
  //   file on disk. A notebook document has a type and can be uniquely
  //   identified using a resource URI.
  // - notebook cell: holds the actual text content. Cells have a kind (either
  //   code or markdown). The actual text content of the cell is stored in a
  //   text document which can be synced to the server like all other text
  //   documents. Cell text documents have an URI however servers should not
  //   rely on any format for this URI since it is up to the client on how it
  //   will create these URIs. The URIs must be unique across ALL notebook cells
  //   and can therefore be used to uniquely identify a notebook cell or the
  //   cell’s text document.

  /**
   * A notebook cell kind.
   *
   * export namespace NotebookCellKind {
   *   export const Markup: 1 = 1;
   *   export const Code: 2 = 2;
   * }
   *
   * @since 3.17.0
   */
  enum class NotebookCellKind {

    /**
     * A markup-cell is formatted source that is used for display.
     *
     * export const Markup: 1 = 1;
     */
    Markup = 1,

    /**
     * A code-cell is source code.
     *
     * export const Code: 2 = 2;
     */
    Code = 2,
  };

  extern std::map<NotebookCellKind, std::string> NotebookCellKindNames;

  auto notebookCellKind(const std::string &name) -> NotebookCellKind;

  /**
   * export interface ExecutionSummary {
   *   executionOrder: uinteger;
   *   success?: boolean;
   * }
   */
  struct ExecutionSummary {

    /**
     * A strict monotonically increasing value indicating the execution order of
     * a cell inside a notebook.
     *
     * executionOrder: uinteger;
     */
    uinteger executionOrder;

    /**
     * Whether the execution was successful or not if known by the client.
     *
     * success?: boolean;
     */
    std::optional<boolean> success;
  };

  /**
   * A notebook cell.
   *
   * A cell's document URI must be unique across ALL notebook cells and can
   * therefore be used to uniquely identify a notebook cell or the cell's text
   * document.
   *
   * export interface NotebookCell {
   *   kind: NotebookCellKind;
   *   document: DocumentUri;
   *   metadata?: LSPObject;
   *   executionSummary?: ExecutionSummary;
   * }
   *
   * @since 3.17.0
   */
  struct NotebookCell {

    /**
     * The cell's kind
     *
     * kind: NotebookCellKind;
     */
    NotebookCellKind kind;

    /**
     * The URI of the cell's text document content.
     *
     * document: DocumentUri;
     */
    DocumentUri document;

    /**
     * Additional metadata stored with the cell.
     *
     * metadata?: LSPObject;
     */
    std::optional<std::unique_ptr<LSPObject>> metadata;

    /**
     * Additional execution summary information if supported by the client.
     *
     * executionSummary?: ExecutionSummary;
     */
    std::optional<std::unique_ptr<ExecutionSummary>> executionSummary;
  };

  /**
   * A notebook document.
   *
   * export interface NotebookDocument {
   *   uri: URI;
   *   notebookType: string;
   *   version: integer;
   *   metadata?: LSPObject;
   *   cells: NotebookCell[];
   * }
   *
   * @since 3.17.0
   */
  struct NotebookDocument {

    /**
     * The notebook document's URI.
     *
     * uri: URI;
     */
    URI uri;

    /**
     * The type of the notebook.
     *
     * notebookType: string;
     */
    string notebookType;

    /**
     * The version number of this document (it will increase after each change,
     * including undo/redo).
     *
     * version: integer;
     */
    integer version;

    /**
     * Additional metadata stored with the notebook document.
     *
     * metadata?: LSPObject;
     */
    std::optional<std::unique_ptr<LSPObject>> metadata;

    /**
     * The cells of a notebook.
     *
     * cells: NotebookCell[];
     */
    std::unique_ptr<std::vector<std::unique_ptr<NotebookCell>>> cells;
  };

  enum class NotebookDocumentFilterType {
    NOTEBOOK_TYPE_REQUIRED,
    SCHEME_REQUIRED,
    PATTERN_REQUIRED,
  };

  struct NotebookTypeDocumentFilter {

    /**
     * The type of the enclosing notebook.
     *
     * notebookType: string;
     */
    string notebookType;

    /**
     * A Uri scheme, like `file` or `untitled`.
     *
     * scheme?: string;
     */
    std::optional<string> scheme;

    /**
     * A glob pattern.
     *
     * pattern?: string;
     */
    std::optional<string> pattern;
  };

  struct NotebookSchemeDocumentFilter {

    /**
     * The type of the enclosing notebook.
     *
     * notebookType: string;
     */
    std::optional<string> notebookType;

    /**
     * A Uri scheme, like `file` or `untitled`.
     *
     * scheme?: string;
     */
    string scheme;

    /**
     * A glob pattern.
     *
     * pattern?: string;
     */
    std::optional<string> pattern;
  };

  struct NotebookPatternDocumentFilter {

    /**
     * The type of the enclosing notebook.
     *
     * notebookType: string;
     */
    std::optional<string> notebookType;

    /**
     * A Uri scheme, like `file` or `untitled`.
     *
     * scheme?: string;
     */
    std::optional<string> scheme;

    /**
     * A glob pattern.
     *
     * pattern?: string;
     */
    string pattern;
  };

  struct NotebookDocumentFilter {
    NotebookDocumentFilterType type;
    union {
      std::unique_ptr<NotebookTypeDocumentFilter> notebookTypeDocumentFilter;
      std::unique_ptr<NotebookSchemeDocumentFilter> notebookSchemeDocumentFilter;
      std::unique_ptr<NotebookPatternDocumentFilter> notebookPatternDocumentFilter;
    };
  };

  enum class StringOrNotebookDocumentFilterType {
    STRING,
    NOTEBOOK_DOCUMENT_FILTER,
  };

  struct StringOrNotebookDocumentFilter {
    StringOrNotebookDocumentFilterType type;
    union {
      string stringValue;
      std::unique_ptr<NotebookDocumentFilter> notebookDocumentFilter;
    };
  };

  /**
   * A notebook cell text document filter denotes a cell text
   * document by different properties.
   *
   * export interface NotebookCellTextDocumentFilter {
   *   notebook: string | NotebookDocumentFilter;
   *   language?: string;
   * }
   *
   * @since 3.17.0
   */
  struct NotebookCellTextDocumentFilter {

    /**
     * A filter that matches against the notebook containing the notebook cell.
     * If a string value is provided it matches against the notebook type. '*'
     * matches every notebook.
     *
     * notebook: string | NotebookDocumentFilter;
     */
    std::unique_ptr<StringOrNotebookDocumentFilterType> notebook;

    /**
     * A language id like `python`.
     *
     * Will be matched against the language id of the notebook cell document.
     * '*' matches every language.
     *
     * language?: string;
     */
    std::optional<string> language;
  };

  /**
   * Notebook specific client capabilities.
   *
   * export interface NotebookDocumentSyncClientCapabilities {
   *   dynamicRegistration?: boolean;
   *   executionSummarySupport?: boolean;
   * }
   *
   * @since 3.17.0
   */
  struct NotebookDocumentSyncClientCapabilities {

    /**
     * Whether implementation supports dynamic registration. If this is set to
     * `true` the client supports the new
     * `(NotebookDocumentSyncRegistrationOptions & NotebookDocumentSyncOptions)`
     * return value for the corresponding server capability as well.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;

    /**
     * The client supports sending execution summary data per cell.
     *
     * executionSummarySupport?: boolean;
     */
    std::optional<boolean> executionSummarySupport;
  };

  /**
   * Capabilities specific to the notebook document support.
   *
   * export interface NotebookDocumentClientCapabilities {
   *   synchronization: NotebookDocumentSyncClientCapabilities;
   * }
   *
   * @since 3.17.0
   */
  struct NotebookDocumentClientCapabilities {

    /**
     * Capabilities specific to notebook document synchronization
     *
     * synchronization: NotebookDocumentSyncClientCapabilities;
     *
     * @since 3.17.0
     */
    std::unique_ptr<NotebookDocumentSyncClientCapabilities> synchronization;
  };

  /**
   * The client has support for file requests/notifications.
   *
   * @since 3.16.0
   */
  struct FileCapabilities {

    /**
     * Whether the client supports dynamic registration for file
     * requests/notifications.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;

    /**
     * The client has support for sending didCreateFiles notifications.
     *
     * didCreate?: boolean;
     */
    std::optional<boolean> didCreate;

    /**
     * The client has support for sending willCreateFiles requests.
     *
     * willCreate?: boolean;
     */
    std::optional<boolean> willCreate;

    /**
     * The client has support for sending didRenameFiles notifications.
     *
     * didRename?: boolean;
     */
    std::optional<boolean> didRename;

    /**
     * The client has support for sending willRenameFiles requests.
     *
     * willRename?: boolean;
     */
    std::optional<boolean> willRename;

    /**
     * The client has support for sending didDeleteFiles notifications.
     *
     * didDelete?: boolean;
     */
    std::optional<boolean> didDelete;

    /**
     * The client has support for sending willDeleteFiles requests.
     *
     * willDelete?: boolean;
     */
    std::optional<boolean> willDelete;
  };

  /**
   * Workspace specific client capabilities.
   */
  struct WorkspaceCapabilities {

    /**
     * The client supports applying batch edits to the workspace by supporting
     * the request 'workspace/applyEdit'
     *
     * applyEdit?: boolean;
     */
    std::optional<boolean> applyEdit;

    /**
     * Capabilities specific to `WorkspaceEdit`s
     *
     * workspaceEdit?: WorkspaceEditClientCapabilities;
     */
    std::optional<std::unique_ptr<WorkspaceEditClientCapabilities>> workspaceEdit;

    /**
     * Capabilities specific to the `workspace/didChangeConfiguration`
     * notification.
     *
     * didChangeConfiguration?: DidChangeConfigurationClientCapabilities;
     */
    std::optional<std::unique_ptr<DidChangeConfigurationClientCapabilities>> didChangeConfiguration;

    /**
     * Capabilities specific to the `workspace/didChangeWatchedFiles`
     * notification.
     *
     * didChangeWatchedFiles?: DidChangeWatchedFilesClientCapabilities;
     */
    std::optional<std::unique_ptr<DidChangeWatchedFilesClientCapabilities>> didChangeWatchedFiles;

    /**
     * Capabilities specific to the `workspace/symbol` request.
     *
     * symbol?: WorkspaceSymbolClientCapabilities;
     */
    std::optional<std::unique_ptr<WorkspaceSymbolClientCapabilities>> symbol;

    /**
     * Capabilities specific to the `workspace/executeCommand` request.
     *
     * executeCommand?: ExecuteCommandClientCapabilities;
     */
    std::optional<std::unique_ptr<ExecuteCommandClientCapabilities>> executeCommand;

    /**
     * The client has support for workspace folders.
     *
     * workspaceFolders?: boolean;
     *
     * @since 3.6.0
     */
    std::optional<boolean> workspaceFolders;

    /**
     * The client supports `workspace/configuration` requests.
     *
     * configuration?: boolean;
     *
     * @since 3.6.0
     */
    std::optional<boolean> configuration;

    /**
     * Capabilities specific to the semantic token requests scoped to the
     * workspace.
     *
     * semanticTokens?: SemanticTokensWorkspaceClientCapabilities;
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<SemanticTokensWorkspaceClientCapabilities>> semanticTokens;

    /**
     * Capabilities specific to the code lens requests scoped to the
     * workspace.
     *
     * codeLens?: CodeLensWorkspaceClientCapabilities;
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<CodeLensWorkspaceClientCapabilities>> codeLens;

    /**
     * The client has support for file requests/notifications.
     *
     * fileOperations?: {
     *   dynamicRegistration?: boolean;
     *   didCreate?: boolean;
     *   willCreate?: boolean;
     *   didRename?: boolean;
     *   willRename?: boolean;
     *   didDelete?: boolean;
     *   willDelete?: boolean;
     * };
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<FileCapabilities>> fileOperations;

    /**
     * Client workspace capabilities specific to inline values.
     *
     * inlineValue?: InlineValueWorkspaceClientCapabilities;
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<InlineValueWorkspaceClientCapabilities>> inlineValue;

    /**
     * Client workspace capabilities specific to inlay hints.
     *
     * inlayHint?: InlayHintWorkspaceClientCapabilities;
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<InlayHintWorkspaceClientCapabilities>> inlayHint;

    /**
     * Client workspace capabilities specific to diagnostics.
     *
     * diagnostics?: DiagnosticWorkspaceClientCapabilities;
     *
     * @since 3.17.0.
     */
    std::optional<std::unique_ptr<DiagnosticWorkspaceClientCapabilities>> diagnostics;
  };

  /**
   * Window specific client capabilities.
   */
  struct WindowCapabilities {

    /**
     * It indicates whether the client supports server initiated
     * progress using the `window/workDoneProgress/create` request.
     *
     * The capability also controls Whether client supports handling
     * of progress notifications. If set servers are allowed to report a
     * `workDoneProgress` property in the request specific server
     * capabilities.
     *
     * workDoneProgress?: boolean;
     *
     * @since 3.15.0
     */
    std::optional<boolean> workDoneProgress;

    /**
     * Capabilities specific to the showMessage request
     *
     * showMessage?: ShowMessageRequestClientCapabilities;
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<ShowMessageRequestClientCapabilities>> showMessage;

    /**
     * Client capabilities for the show document request.
     *
     * showDocument?: ShowDocumentClientCapabilities;
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<ShowDocumentClientCapabilities>> showDocument;
  };

  /**
   * Client capability that signals how the client handles stale requests (e.g.
   * a request for which the client will not process the response anymore since
   * the information is outdated).
   *
   * @since 3.17.0
   */
  struct StaleRequestSupport {

    /**
     * The client will actively cancel the request.
     *
     * cancel: boolean;
     */
    boolean cancel;

    /**
     * The list of requests for which the client will retry the request if it
     * receives a response with error code `ContentModified``
     *
     * retryOnContentModified: string[];
     */
    std::unique_ptr<std::vector<string>> retryOnContentModified;
  };

  /**
   * General client capabilities.
   *
   * @since 3.16.0
   */
  struct GeneralCapabilities {

    /**
     * Client capability that signals how the client
     * handles stale requests (e.g. a request
     * for which the client will not process the response
     * anymore since the information is outdated).
     *
     * staleRequestSupport?: {
     *   cancel: boolean;
     *   retryOnContentModified: string[];
     * }
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<StaleRequestSupport>> staleRequestSupport;

    /**
     * Client capabilities specific to regular expressions.
     *
     * regularExpressions?: RegularExpressionsClientCapabilities;
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<RegularExpressionsClientCapabilities>> regularExpressions;

    /**
     * Client capabilities specific to the client's markdown parser.
     *
     * markdown?: MarkdownClientCapabilities;
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<MarkdownClientCapabilities>> markdown;

    /**
     * The position encodings supported by the client. Client and server
     * have to agree on the same position encoding to ensure that offsets
     * (e.g. character position in a line) are interpreted the same on both
     * side.
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
     * positionEncodings?: PositionEncodingKind[];
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<std::vector<std::unique_ptr<PositionEncodingKind>>>> positionEncodings;
  };

  /**
   * ClientCapabilities define capabilities for dynamic registration, workspace
   * and text document features the client supports. The experimental can be
   * used to pass experimental capabilities under development. For future
   * compatibility a ClientCapabilities object literal can have more properties
   * set than currently defined. Servers receiving a ClientCapabilities object
   * literal with unknown properties should ignore these properties. A missing
   * property should be interpreted as an absence of the capability. If a
   * missing property normally defines sub properties, all missing sub
   * properties should be interpreted as an absence of the corresponding
   * capability.
   *
   * Client capabilities got introduced with version 3.0 of the protocol. They
   * therefore only describe capabilities that got introduced in 3.x or later.
   * Capabilities that existed in the 2.x version of the protocol are still
   * mandatory for clients. Clients cannot opt out of providing them. So even if
   * a client omits the ClientCapabilities.textDocument.synchronization it is
   * still required that the client provides text document synchronization (e.g.
   * open, changed and close notifications).
   *
   * interface ClientCapabilities {
   *   workspace?: {
   *     applyEdit?: boolean;
   *     workspaceEdit?: WorkspaceEditClientCapabilities;
   *     didChangeConfiguration?: DidChangeConfigurationClientCapabilities;
   *     didChangeWatchedFiles?: DidChangeWatchedFilesClientCapabilities;
   *     symbol?: WorkspaceSymbolClientCapabilities;
   *     executeCommand?: ExecuteCommandClientCapabilities;
   *     workspaceFolders?: boolean;
   *     configuration?: boolean;
   *     semanticTokens?: SemanticTokensWorkspaceClientCapabilities;
   *     codeLens?: CodeLensWorkspaceClientCapabilities;
   *     fileOperations?: {
   *       dynamicRegistration?: boolean;
   *       didCreate?: boolean;
   *       willCreate?: boolean;
   *       didRename?: boolean;
   *       willRename?: boolean;
   *       didDelete?: boolean;
   *       willDelete?: boolean;
   *     };
   *     inlineValue?: InlineValueWorkspaceClientCapabilities;
   *     inlayHint?: InlayHintWorkspaceClientCapabilities;
   *     diagnostics?: DiagnosticWorkspaceClientCapabilities;
   *   };
   *   textDocument?: TextDocumentClientCapabilities;
   *   notebookDocument?: NotebookDocumentClientCapabilities;
   *   window?: {
   *     workDoneProgress?: boolean;
   *     showMessage?: ShowMessageRequestClientCapabilities;
   *     showDocument?: ShowDocumentClientCapabilities;
   *   };
   *   general?: {
   *     staleRequestSupport?: {
   *       cancel: boolean;
   *       retryOnContentModified: string[];
   *     }
   *     regularExpressions?: RegularExpressionsClientCapabilities;
   *     markdown?: MarkdownClientCapabilities;
   *     positionEncodings?: PositionEncodingKind[];
   *   };
   *   experimental?: LSPAny;
   * }
   */
  struct ClientCapabilities {

    /**
     * Workspace specific client capabilities.
     *
     * workspace?: {
     *   applyEdit?: boolean;
     *   workspaceEdit?: WorkspaceEditClientCapabilities;
     *   didChangeConfiguration?: DidChangeConfigurationClientCapabilities;
     *   didChangeWatchedFiles?: DidChangeWatchedFilesClientCapabilities;
     *   symbol?: WorkspaceSymbolClientCapabilities;
     *   executeCommand?: ExecuteCommandClientCapabilities;
     *   workspaceFolders?: boolean;
     *   configuration?: boolean;
     *   semanticTokens?: SemanticTokensWorkspaceClientCapabilities;
     *   codeLens?: CodeLensWorkspaceClientCapabilities;
     *   fileOperations?: {
     *     dynamicRegistration?: boolean;
     *     didCreate?: boolean;
     *     willCreate?: boolean;
     *     didRename?: boolean;
     *     willRename?: boolean;
     *     didDelete?: boolean;
     *     willDelete?: boolean;
     *   };
     *   inlineValue?: InlineValueWorkspaceClientCapabilities;
     *   inlayHint?: InlayHintWorkspaceClientCapabilities;
     *   diagnostics?: DiagnosticWorkspaceClientCapabilities;
     * };
     */
    std::optional<std::unique_ptr<WorkspaceCapabilities>> workspace;

    /**
     * Text document specific client capabilities.
     *
     * textDocument?: TextDocumentClientCapabilities;
     */
    std::optional<std::unique_ptr<TextDocumentClientCapabilities>> textDocument;

    /**
     * Capabilities specific to the notebook document support.
     *
     * notebookDocument?: NotebookDocumentClientCapabilities;
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<NotebookDocumentClientCapabilities>> notebookDocument;

    /**
     * Window specific client capabilities.
     *
     * window?: {
     *   workDoneProgress?: boolean;
     *   showMessage?: ShowMessageRequestClientCapabilities;
     *   showDocument?: ShowDocumentClientCapabilities;
     * };
     */
    std::optional<std::unique_ptr<WindowCapabilities>> window;

    /**
     * General client capabilities.
     *
     * general?: {
     *   staleRequestSupport?: {
     *     cancel: boolean;
     *     retryOnContentModified: string[];
     *   }
     *   regularExpressions?: RegularExpressionsClientCapabilities;
     *   markdown?: MarkdownClientCapabilities;
     *   positionEncodings?: PositionEncodingKind[];
     * };
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<GeneralCapabilities>> general;

    /**
     * Experimental client capabilities.
     *
     * experimental?: LSPAny;
     */
    std::optional<std::unique_ptr<LSPAny>> experimental;
  };

  /**
   * Information about the server.
   */
  struct ServerInfo {

    /**
     * The name of the server as defined by the server.
     *
     * name: string;
     */
    string name;

    /**
     * The server's version as defined by the server.
     *
     * version?: string;
     */
    std::optional<string> version;
  };

  /**
   * Defines how the host (editor) should sync document changes to the language
   * server.
   *
   * Client support for textDocument/didOpen, textDocument/didChange and
   * textDocument/didClose notifications is mandatory in the protocol and
   * clients can not opt out supporting them. This includes both full and
   * incremental synchronization in the textDocument/didChange notification. In
   * addition a server must either implement all three of them or none. Their
   * capabilities are therefore controlled via a combined client and server
   * capability. Opting out of text document synchronization makes only sense if
   * the documents shown by the client are read only. Otherwise the server might
   * receive request for documents, for which the content is managed in the
   * client (e.g. they might have changed).
   *
   * Client Capability:
   *   property path (optional): textDocument.synchronization.dynamicRegistration
   *   property type: boolean
   *
   * Controls whether text document synchronization supports dynamic registration.
   *
   * Server Capability:
   *   property path (optional): textDocumentSync
   *   property type: TextDocumentSyncKind | TextDocumentSyncOptions. The below
   *   definition of the TextDocumentSyncOptions only covers the properties
   *   specific to the open, change and close notifications. A complete
   *   definition covering all properties can be found here:
   *
   * export namespace TextDocumentSyncKind {
   *   export const None = 0;
   *   export const Full = 1;
   *   export const Incremental = 2;
   * }
   * export type TextDocumentSyncKind = 0 | 1 | 2;
   */
  enum class TextDocumentSyncKind {

    /**
     * Documents should not be synced at all.
     *
     * export const None = 0;
     */
    None = 0,

    /**
     * Documents are synced by always sending the full content of the document.
     *
     * export const Full = 1;
     */
    Full = 1,

    /**
     * Documents are synced by sending the full content on open. After that only
     * incremental updates to the document are sent.
     *
     * export const Incremental = 2;
     */
    Incremental = 2,
  };

  extern std::map<TextDocumentSyncKind, std::string> TextDocumentSyncKindNames;

  auto textDocumentSyncKind(const std::string &name) -> TextDocumentSyncKind;

  /**
   * The document save notification is sent from the client to the server when
   * the document was saved in the client.
   *
   * Client Capability:
   *   property name (optional): textDocument.synchronization.didSave
   *   property type: boolean
   *
   * The capability indicates that the client supports textDocument/didSave
   * notifications.
   *
   * Server Capability:
   *   property name (optional): textDocumentSync.save
   *   property type: boolean | SaveOptions where SaveOptions is defined as follows:
   *
   * export interface SaveOptions {
   *   includeText?: boolean;
   * }
   */
  struct SaveOptions {

    /**
     * The client is supposed to include the content on save.
     *
     * includeText?: boolean;
     */
    std::optional<boolean> includeText;
  };

  enum class SaveOrOptionsType {
    BOOLEAN,
    SAVE_OPTIONS,
  };

  struct SaveOrOptions {
    SaveOrOptionsType type;
    union {
      boolean booleanValue;
      std::unique_ptr<SaveOptions> saveOptions;
    };
  };

  /**
   * export interface TextDocumentSyncOptions {
   *   openClose?: boolean;
   *   change?: TextDocumentSyncKind;
   *   willSave?: boolean;
   *   willSaveWaitUntil?: boolean;
   *   save?: boolean | SaveOptions;
   * }
   */
  struct TextDocumentSyncOptions {

    /**
     * Open and close notifications are sent to the server. If omitted open
     * close notifications should not be sent.
     *
     * openClose?: boolean;
     */
    std::optional<boolean> openClose;

    /**
     * Change notifications are sent to the server. See
     * TextDocumentSyncKind.None, TextDocumentSyncKind.Full and
     * TextDocumentSyncKind.Incremental. If omitted it defaults to
     * TextDocumentSyncKind.None.
     *
     * change?: TextDocumentSyncKind;
     */
    std::optional<TextDocumentSyncKind> change;

    /**
     * If present will save notifications are sent to the server. If omitted
     * the notification should not be sent.
     *
     * willSave?: boolean;
     */
    std::optional<boolean> willSave;

    /**
     * If present will save wait until requests are sent to the server. If
     * omitted the request should not be sent.
     *
     * willSaveWaitUntil?: boolean;
     */
    std::optional<boolean> willSaveWaitUntil;

    /**
     * If present save notifications are sent to the server. If omitted the
     * notification should not be sent.
     *
     * save?: boolean | SaveOptions;
     */
    std::optional<std::unique_ptr<SaveOrOptions>> save;
  };

  enum class TextDocumentSyncType {
    TEXT_DOCUMENT_SYNC_OPTIONS,
    TEXT_DOCUMENT_SYNC_KIND,
  };

  /**
   * Defines how text documents are synced. Is either a detailed structure
   * defining each notification or for backwards compatibility the
   * TextDocumentSyncKind number. If omitted it defaults to
   * `TextDocumentSyncKind.None`.
   */
  struct TextDocumentSync {
    TextDocumentSyncType type;
    union {
      std::unique_ptr<TextDocumentSyncOptions> textDocumentSyncOptions;
      std::unique_ptr<TextDocumentSyncKind> textDocumentSyncKind;
    };
  };

  struct NotebookSelectorCell {
    string language;
  };

  struct NotebookRequiredNotebookSelector {

    /**
     * The notebook to be synced. If a string value is provided it matches
     * against the notebook type. '*' matches every notebook.
     *
     * notebook: string | NotebookDocumentFilter;
     */
    std::unique_ptr<StringOrNotebookDocumentFilter> notebook;

    /**
     * The cells of the matching notebook to be synced.
     *
     * cells?: { language: string }[];
     */
    std::optional<std::unique_ptr<std::vector<std::unique_ptr<NotebookSelectorCell>>>> cells;
  };

  struct CellsRequiredNotebookSelector {

    /**
     * The notebook to be synced. If a string value is provided it matches
     * against the notebook type. '*' matches every notebook.
     *
     * notebook: string | NotebookDocumentFilter;
     */
    std::optional<std::unique_ptr<StringOrNotebookDocumentFilter>> notebook;

    /**
     * The cells of the matching notebook to be synced.
     *
     * cells?: { language: string }[];
     */
    std::unique_ptr<std::vector<std::unique_ptr<NotebookSelectorCell>>> cells;
  };

  enum class NotebookSelectorType {
    NOTEBOOK_REQUIRED,
    CELLS_REQUIRED,
  };

  struct NotebookSelector {
    NotebookSelectorType type;
    union {
      std::unique_ptr<NotebookRequiredNotebookSelector> notebookRequiredNotebookSelector;
      std::unique_ptr<CellsRequiredNotebookSelector> cellsRequiredNotebookSelector;
    };
  };

  /**
   * Options specific to a notebook plus its cells to be synced to the server.
   *
   * If a selector provides a notebook document filter but no cell selector all
   * cells of a matching notebook document will be synced.
   *
   * If a selector provides no notebook document filter but only a cell selector
   * all notebook documents that contain at least one matching cell will be
   * synced.
   *
   * export interface NotebookDocumentSyncOptions {
   *   notebookSelector: ({
   *     notebook: string | NotebookDocumentFilter;
   *     cells?: { language: string }[];
   *   } | {
   *     notebook?: string | NotebookDocumentFilter;
   *     cells: { language: string }[];
   *   })[];
   *   save?: boolean;
   * }
   *
   * @since 3.17.0
   */
  struct NotebookDocumentSyncOptions {

    /**
     * The notebooks to be synced.
     *
     * notebookSelector: ({
     *   notebook: string | NotebookDocumentFilter;
     *   cells?: { language: string }[];
     * } | {
     *   notebook?: string | NotebookDocumentFilter;
     *   cells: { language: string }[];
     * })[];
     */
    std::unique_ptr<std::vector<std::unique_ptr<NotebookSelector>>> notebookSelector;

    /**
     * Whether save notification should be forwarded to the server. Will only be
     * honored if mode === `notebook`.
     *
     * save?: boolean;
     */
    std::optional<boolean> save;
  };

  /**
   * Registration options specific to a notebook.
   *
   * export interface NotebookDocumentSyncRegistrationOptions extends
   *   NotebookDocumentSyncOptions, StaticRegistrationOptions {
   * }
   *
   * @since 3.17.0
   */
  struct NotebookDocumentSyncRegistrationOptions
    : public NotebookDocumentSyncOptions
    , public StaticRegistrationOptions {
  };

  /**
   * The params sent in an open notebook document notification.
   *
   * The open notification is sent from the client to the server when a notebook
   * document is opened. It is only sent by a client if the server requested the
   * synchronization mode notebook in its notebookDocumentSync capability.
   *
   * Notification:
   *   method: notebookDocument/didOpen
   *   params: DidOpenNotebookDocumentParams defined as follows:
   *
   * export interface DidOpenNotebookDocumentParams {
   *   notebookDocument: NotebookDocument;
   *   cellTextDocuments: TextDocumentItem[];
   * }
   *
   * @since 3.17.0
   */
  struct DidOpenNotebookDocumentParams {

    /**
     * The notebook document that got opened.
     *
     * notebookDocument: NotebookDocument;
     */
    std::unique_ptr<NotebookDocument> notebookDocument;

    /**
     * The text documents that represent the content of a notebook cell.
     *
     * cellTextDocuments: TextDocumentItem[];
     */
    std::unique_ptr<std::vector<std::unique_ptr<TextDocumentItem>>> cellTextDocuments;
  };

  /**
   * A literal to identify a notebook document in the client.
   *
   * export interface NotebookDocumentIdentifier {
   *   uri: URI;
   * }
   *
   * @since 3.17.0
   */
  struct NotebookDocumentIdentifier {

    /**
     * The notebook document's URI.
     *
     * uri: URI;
     */
    URI uri;
  };

  /**
   * A versioned notebook document identifier.
   *
   * export interface VersionedNotebookDocumentIdentifier {
   *   version: integer;
   *   uri: URI;
   * }
   *
   * @since 3.17.0
   */
  struct VersionedNotebookDocumentIdentifier {

    /**
     * The version number of this notebook document.
     *
     * version: integer;
     */
    integer version;

    /**
     * The notebook document's URI.
     *
     * uri: URI;
     */
    URI uri;
  };

  /**
   * A change describing how to move a `NotebookCell` array from state S to S'.
   *
   * export interface NotebookCellArrayChange {
   *   start: uinteger;
   *   deleteCount: uinteger;
   *   cells?: NotebookCell[];
   * }
   *
   * @since 3.17.0
   */
  struct NotebookCellArrayChange {

    /**
     * The start offset of the cell that changed.
     *
     * start: uinteger;
     */
    uinteger start;

    /**
     * The deleted cells
     *
     * deleteCount: uinteger;
     */
    uinteger deleteCount;

    /**
     * The new cells, if any
     *
     * cells?: NotebookCell[];
     */
    std::optional<std::unique_ptr<std::vector<std::unique_ptr<NotebookCell>>>> cells;
  };

  struct NotebookDocumentChangeEventCellsStructure {

    /**
     * The change to the cell array.
     *
     * array: NotebookCellArrayChange;
     */
    std::unique_ptr<NotebookCellArrayChange> array;

    /**
     * Additional opened cell text documents.
     *
     * didOpen?: TextDocumentItem[];
     */
    std::optional<std::unique_ptr<std::vector<std::unique_ptr<TextDocumentItem>>>> didOpen;

    /**
     * Additional closed cell text documents.
     *
     * didClose?: TextDocumentIdentifier[];
     */
    std::optional<std::unique_ptr<std::vector<std::unique_ptr<TextDocumentIdentifier>>>> didClose;
  };

  struct NotebookDocumentChangeEventCellsTextContent {

    /**
     * document: VersionedTextDocumentIdentifier;
     */
    std::unique_ptr<VersionedTextDocumentIdentifier> document;

    /**
     * changes: TextDocumentContentChangeEvent[];
     */
    std::unique_ptr<std::vector<std::unique_ptr<TextDocumentContentChangeEvent>>> changes;
  };

  struct NotebookDocumentChangeEventCells {

    /**
     * Changes to the cell structure to add or remove cells.
     *
     * structure?: {
     *   array: NotebookCellArrayChange;
     *   didOpen?: TextDocumentItem[];
     *   didClose?: TextDocumentIdentifier[];
     * };
     */
    std::optional<std::unique_ptr<NotebookDocumentChangeEventCellsStructure>> structure;

    /**
     * Changes to notebook cells properties like its
     * kind, execution summary or metadata.
     *
     * data?: NotebookCell[];
     */
    std::optional<std::unique_ptr<std::vector<std::unique_ptr<NotebookCell>>>> data;

    /**
     * Changes to the text content of notebook cells.
     *
     * textContent?: {
     *   document: VersionedTextDocumentIdentifier;
     *   changes: TextDocumentContentChangeEvent[];
     * }[];
     */
    std::optional<std::unique_ptr<std::vector<std::unique_ptr<NotebookDocumentChangeEventCellsTextContent>>>> textContent;
  };

  /**
   * A change event for a notebook document.
   *
   * export interface NotebookDocumentChangeEvent {
   *   metadata?: LSPObject;
   *   cells?: {
   *      structure?: {
   *       array: NotebookCellArrayChange;
   *       didOpen?: TextDocumentItem[];
   *       didClose?: TextDocumentIdentifier[];
   *     };
   *     data?: NotebookCell[];
   *     textContent?: {
   *       document: VersionedTextDocumentIdentifier;
   *       changes: TextDocumentContentChangeEvent[];
   *     }[];
   *   };
   * }
   *
   * @since 3.17.0
   */
  struct NotebookDocumentChangeEvent {

    /**
     * The changed meta data if any.
     *
     * metadata?: LSPObject;
     */
    std::optional<std::unique_ptr<LSPObject>> metadata;

    /**
     * Changes to cells
     *
     * cells?: {
     *   structure?: {
     *     array: NotebookCellArrayChange;
     *     didOpen?: TextDocumentItem[];
     *     didClose?: TextDocumentIdentifier[];
     *   };
     *   data?: NotebookCell[];
     *   textContent?: {
     *     document: VersionedTextDocumentIdentifier;
     *     changes: TextDocumentContentChangeEvent[];
     *   }[];
     * };
     */
    std::optional<std::unique_ptr<NotebookDocumentChangeEventCells>> cells;
  };

  /**
   * The params sent in a change notebook document notification.
   *
   * The change notification is sent from the client to the server when a
   * notebook document changes. It is only sent by a client if the server
   * requested the synchronization mode notebook in its notebookDocumentSync
   * capability.
   *
   * Notification:
   *   method: notebookDocument/didChange
   *   params: DidChangeNotebookDocumentParams defined as follows:
   *
   * export interface DidChangeNotebookDocumentParams {
   *   notebookDocument: VersionedNotebookDocumentIdentifier;
   *   change: NotebookDocumentChangeEvent;
   * }
   *
   * @since 3.17.0
   */
  struct DidChangeNotebookDocumentParams {

    /**
     * The notebook document that did change. The version number points to the
     * version after all provided changes have been applied.
     *
     * notebookDocument: VersionedNotebookDocumentIdentifier;
     */
    std::unique_ptr<VersionedNotebookDocumentIdentifier> notebookDocument;

    /**
     * The actual changes to the notebook document.
     *
     * The change describes single state change to the notebook document. So it
     * moves a notebook document, its cells and its cell text document contents
     * from state S to S'.
     *
     * To mirror the content of a notebook using change events use the following
     * approach:
     * - start with the same initial content
     * - apply the 'notebookDocument/didChange' notifications in the order you
     *   receive them.
     *
     * change: NotebookDocumentChangeEvent;
     */
    std::unique_ptr<NotebookDocumentChangeEvent> change;
  };

  /**
   * The params sent in a save notebook document notification.
   *
   * The save notification is sent from the client to the server when a notebook
   * document is saved. It is only sent by a client if the server requested the
   * synchronization mode notebook in its notebookDocumentSync capability.
   *
   * Notification:
   *   method: notebookDocument/didSave
   *   params: DidSaveNotebookDocumentParams defined as follows:
   *
   * export interface DidSaveNotebookDocumentParams {
   *   notebookDocument: NotebookDocumentIdentifier;
   * }
   *
   * @since 3.17.0
   */
  struct DidSaveNotebookDocumentParams {

    /**
     * The notebook document that got saved.
     *
     * notebookDocument: NotebookDocumentIdentifier;
     */
    std::unique_ptr<NotebookDocumentIdentifier> notebookDocument;
  };

  /**
   * The params sent in a close notebook document notification.
   *
   * The close notification is sent from the client to the server when a
   * notebook document is closed. It is only sent by a client if the server
   * requested the synchronization mode notebook in its notebookDocumentSync
   * capability.
   *
   * Notification:
   *   method: notebookDocument/didClose
   *   params: DidCloseNotebookDocumentParams defined as follows:
   *
   * export interface DidCloseNotebookDocumentParams {
   *   notebookDocument: NotebookDocumentIdentifier;
   *   cellTextDocuments: TextDocumentIdentifier[];
   * }
   *
   * @since 3.17.0
   */
  struct DidCloseNotebookDocumentParams {

    /**
     * The notebook document that got closed.
     *
     * notebookDocument: NotebookDocumentIdentifier;
     */
    std::unique_ptr<NotebookDocumentIdentifier> notebookDocument;

    /**
     * The text documents that represent the content
     * of a notebook cell that got closed.
     *
     * cellTextDocuments: TextDocumentIdentifier[];
     */
    std::unique_ptr<std::vector<std::unique_ptr<TextDocumentIdentifier>>> cellTextDocuments;
  };

  enum class NotebookDocumentSyncType {
    NOTEBOOK_DOCUMENT_SYNC_OPTIONS,
    NOTEBOOK_DOCUMENT_SYNC_REGISTRATION_OPTIONS,
  };

  struct NotebookDocumentSync {
    NotebookDocumentSyncType type;
    union {
      std::unique_ptr<NotebookDocumentSyncOptions> notebookDocumentSyncOptions;
      std::unique_ptr<NotebookDocumentSyncRegistrationOptions> notebookDocumentSyncRegistrationOptions;
    };
  };

  enum class HoverProviderType {
    BOOLEAN,
    HOVER_OPTIONS,
  };

  struct HoverProvider {
    HoverProviderType type;
    union {
      boolean booleanValue;
      std::unique_ptr<HoverOptions> hoverOptions;
    };
  };

  enum class DeclarationProviderType {
    BOOLEAN,
    DECLARATION_OPTIONS,
    DECLARATION_REGISTRATION_OPTIONS,
  };

  struct DeclarationProvider {
    DeclarationProviderType type;
    union {
      boolean booleanValue;
      std::unique_ptr<DeclarationOptions> declarationOptions;
      std::unique_ptr<DeclarationRegistrationOptions> declarationRegistrationOptions;
    };
  };

  enum class DefinitionProviderType {
    BOOLEAN,
    DEFINITION_OPTIONS,
  };

  struct DefinitionProvider {
    DefinitionProviderType type;
    union {
      boolean booleanValue;
      std::unique_ptr<DefinitionOptions> definitionOptions;
    };
  };

  enum class TypeDefinitionProviderType {
    BOOLEAN,
    TYPE_DEFINITION_OPTIONS,
    TYPE_DEFINITION_REGISTRATION_OPTIONS,
  };

  struct TypeDefinitionProvider {
    TypeDefinitionProviderType type;
    union {
      boolean booleanValue;
      std::unique_ptr<TypeDefinitionOptions> typeDefinitionOptions;
      std::unique_ptr<TypeDefinitionRegistrationOptions> typeDefinitionRegistrationOptions;
    };
  };

  enum class ImplementationProviderType {
    BOOLEAN,
    IMPLEMENTATION_OPTIONS,
    IMPLEMENTATION_REGISTRATION_OPTIONS,
  };

  struct ImplementationProvider {
    ImplementationProviderType type;
    union {
      boolean booleanValue;
      std::unique_ptr<ImplementationOptions> implementationOptions;
      std::unique_ptr<ImplementationRegistrationOptions> implementationRegistrationOptions;
    };
  };

  enum class ReferencesProviderType {
    BOOLEAN,
    REFERENCE_OPTIONS,
  };

  struct ReferencesProvider {
    ReferencesProviderType type;
    union {
      boolean booleanValue;
      std::unique_ptr<ReferenceOptions> referenceOptions;
    };
  };

  enum class DocumentHighlightProviderType {
    BOOLEAN,
    DOCUMENT_HIGHLIGHT_OPTIONS,
  };

  struct DocumentHighlightProvider {
    DocumentHighlightProviderType type;
    union {
      boolean booleanValue;
      std::unique_ptr<DocumentHighlightOptions> documentHighlightOptions;
    };
  };

  enum class DocumentSymbolProviderType {
    BOOLEAN,
    DOCUMENT_SYMBOL_OPTIONS,
  };

  struct DocumentSymbolProvider {
    DocumentSymbolProviderType type;
    union {
      boolean booleanValue;
      std::unique_ptr<DocumentSymbolOptions> documentSymbolOptions;
    };
  };

  enum class CodeActionProviderType {
    BOOLEAN,
    CODE_ACTION_OPTIONS,
  };

  struct CodeActionProvider {
    CodeActionProviderType type;
    union {
      boolean booleanValue;
      std::unique_ptr<CodeActionOptions> codeActionOptions;
    };
  };

  enum class ColorProviderType {
    BOOLEAN,
    DOCUMENT_COLOR_OPTIONS,
    DOCUMENT_COLOR_REGISTRATION_OPTIONS,
  };

  struct ColorProvider {
    ColorProviderType type;
    union {
      boolean booleanValue;
      std::unique_ptr<DocumentColorOptions> documentColorOptions;
      std::unique_ptr<DocumentColorRegistrationOptions> documentColorRegistrationOptions;
    };
  };

  enum class DocumentFormattingProviderType {
    BOOLEAN,
    DOCUMENT_FORMATTING_OPTIONS,
  };

  struct DocumentFormattingProvider {
    DocumentFormattingProviderType type;
    union {
      boolean booleanValue;
      std::unique_ptr<DocumentFormattingOptions> documentFormattingOptions;
    };
  };

  enum class DocumentRangeFormattingProviderType {
    BOOLEAN,
    DOCUMENT_RANGE_FORMATTING_OPTIONS,
  };

  struct DocumentRangeFormattingProvider {
    DocumentRangeFormattingProviderType type;
    union {
      boolean booleanValue;
      std::unique_ptr<DocumentRangeFormattingOptions> documentRangeFormattingOptions;
    };
  };

  enum class RenameProviderType {
    BOOLEAN,
    RENAME_OPTIONS,
  };

  struct RenameProvider {
    RenameProviderType type;
    union {
      boolean booleanValue;
      std::unique_ptr<RenameOptions> renameOptions;
    };
  };

  enum class FoldingRangeProviderType {
    BOOLEAN,
    FOLDING_RANGE_OPTIONS,
    FOLDING_RANGE_REGISTRATION_OPTIONS,
  };

  struct FoldingRangeProvider {
    FoldingRangeProviderType type;
    union {
      boolean booleanValue;
      std::unique_ptr<FoldingRangeOptions> foldingRangeOptions;
      std::unique_ptr<FoldingRangeRegistrationOptions> foldingRangeRegistrationOptions;
    };
  };

  enum class SelectionRangeProviderType {
    BOOLEAN,
    SELECTION_RANGE_OPTIONS,
    SELECTION_RANGE_REGISTRATION_OPTIONS,
  };

  struct SelectionRangeProvider {
    SelectionRangeProviderType type;
    union {
      boolean booleanValue;
      std::unique_ptr<SelectionRangeOptions> selectionRangeOptions;
      std::unique_ptr<SelectionRangeRegistrationOptions> selectionRangeRegistrationOptions;
    };
  };

  enum class LinkedEditingRangeProviderType {
    BOOLEAN,
    LINKED_EDITING_RANGE_OPTIONS,
    LINKED_EDITING_RANGE_REGISTRATION_OPTIONS,
  };

  struct LinkedEditingRangeProvider {
    LinkedEditingRangeProviderType type;
    union {
      boolean booleanValue;
      std::unique_ptr<LinkedEditingRangeOptions> linkedEditingRangeOptions;
      std::unique_ptr<LinkedEditingRangeRegistrationOptions> linkedEditingRangeRegistrationOptions;
    };
  };

  enum class CallHierarchyProviderType {
    BOOLEAN,
    CALL_HIERARCHY_OPTIONS,
    CALL_HIERARCHY_REGISTRATION_OPTIONS,
  };

  struct CallHierarchyProvider {
    CallHierarchyProviderType type;
    union {
      boolean booleanValue;
      std::unique_ptr<CallHierarchyOptions> callHierarchyOptions;
      std::unique_ptr<CallHierarchyRegistrationOptions> callHierarchyRegistrationOptions;
    };
  };

  enum class SemanticTokensProviderType {
    SEMANTIC_TOKENS_OPTIONS,
    SEMANTIC_TOKENS_REGISTRATION_OPTIONS,
  };

  struct SemanticTokensProvider {
    SemanticTokensProviderType type;
    union {
      std::unique_ptr<SemanticTokensOptions> semanticTokensOptions;
      std::unique_ptr<SemanticTokensRegistrationOptions> semanticTokensRegistrationOptions;
    };
  };

  enum class MonikerProviderType {
    BOOLEAN,
    MONIKER_OPTIONS,
    MONIKER_REGISTRATION_OPTIONS,
  };

  struct MonikerProvider {
    MonikerProviderType type;
    union {
      boolean booleanValue;
      std::unique_ptr<MonikerOptions> monikerOptions;
      std::unique_ptr<MonikerRegistrationOptions> monikerRegistrationOptions;
    };
  };

  enum class TypeHierarchyProviderType {
    BOOLEAN,
    TYPE_HIERARCHY_OPTIONS,
    TYPE_HIERARCHY_REGISTRATION_OPTIONS,
  };

  struct TypeHierarchyProvider {
    TypeHierarchyProviderType type;
    union {
      boolean booleanValue;
      std::unique_ptr<TypeHierarchyOptions> typeHierarchyOptions;
      std::unique_ptr<TypeHierarchyRegistrationOptions> typeHierarchyRegistrationOptions;
    };
  };

  enum class InlineValueProviderType {
    BOOLEAN,
    INLINE_VALUE_OPTIONS,
    INLINE_VALUE_REGISTRATION_OPTIONS,
  };

  struct InlineValueProvider {
    InlineValueProviderType type;
    union {
      boolean booleanValue;
      std::unique_ptr<InlineValueOptions> inlineValueOptions;
      std::unique_ptr<InlineValueRegistrationOptions> inlineValueRegistrationOptions;
    };
  };

  enum class InlayHintProviderType {
    BOOLEAN,
    INLAY_HINT_OPTIONS,
    INLAY_HINT_REGISTRATION_OPTIONS,
  };

  struct InlayHintProvider {
    InlayHintProviderType type;
    union {
      boolean booleanValue;
      std::unique_ptr<InlayHintOptions> inlayHintOptions;
      std::unique_ptr<InlayHintRegistrationOptions> inlayHintRegistrationOptions;
    };
  };

  enum class DiagnosticProviderType {
    DIAGNOSTIC_OPTIONS,
    DIAGNOSTIC_REGISTRATION_OPTIONS,
  };

  struct DiagnosticProvider {
    DiagnosticProviderType type;
    union {
      std::unique_ptr<DiagnosticOptions> diagnosticOptions;
      std::unique_ptr<DiagnosticRegistrationOptions> diagnosticRegistrationOptions;
    };
  };

  enum class WorkspaceSymbolProviderType {
    BOOLEAN,
    WORKSPACE_SYMBOL_OPTIONS,
  };

  struct WorkspaceSymbolProvider {
    WorkspaceSymbolProviderType type;
    union {
      boolean booleanValue;
      std::unique_ptr<WorkspaceSymbolOptions> workspaceSymbolOptions;
    };
  };

  struct FileOperations {

    /**
     * The server is interested in receiving didCreateFiles
     * notifications.
     *
     * didCreate?: FileOperationRegistrationOptions;
     */
    std::optional<std::unique_ptr<FileOperationRegistrationOptions>> didCreate;

    /**
     * The server is interested in receiving willCreateFiles requests.
     *
     * willCreate?: FileOperationRegistrationOptions;
     */
    std::optional<std::unique_ptr<FileOperationRegistrationOptions>> willCreate;

    /**
     * The server is interested in receiving didRenameFiles
     * notifications.
     *
     * didRename?: FileOperationRegistrationOptions;
     */
    std::optional<std::unique_ptr<FileOperationRegistrationOptions>> didRename;

    /**
     * The server is interested in receiving willRenameFiles requests.
     *
     * willRename?: FileOperationRegistrationOptions;
     */
    std::optional<std::unique_ptr<FileOperationRegistrationOptions>> willRename;

    /**
     * The server is interested in receiving didDeleteFiles file
     * notifications.
     *
     * didDelete?: FileOperationRegistrationOptions;
     */
    std::optional<std::unique_ptr<FileOperationRegistrationOptions>> didDelete;

    /**
     * The server is interested in receiving willDeleteFiles file
     * requests.
     *
     * willDelete?: FileOperationRegistrationOptions;
     */
    std::optional<std::unique_ptr<FileOperationRegistrationOptions>> willDelete;
  };

  struct WorkspaceProvider {

    /**
     * The server supports workspace folder.
     *
     * workspaceFolders?: WorkspaceFoldersServerCapabilities;
     *
     * @since 3.6.0
     */
    std::optional<std::unique_ptr<WorkspaceFoldersServerCapabilities>> workspaceFolders;

    /**
     * The server is interested in file notifications/requests.
     *
     * fileOperations?: {
     *   didCreate?: FileOperationRegistrationOptions;
     *   willCreate?: FileOperationRegistrationOptions;
     *   didRename?: FileOperationRegistrationOptions;
     *   willRename?: FileOperationRegistrationOptions;
     *   didDelete?: FileOperationRegistrationOptions;
     *   willDelete?: FileOperationRegistrationOptions;
     * };
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<FileOperations>> fileOperations;
  };

  /**
   * The server can signal the following capabilities:
   *
   * interface ServerCapabilities {
   *   positionEncoding?: PositionEncodingKind;
   *   textDocumentSync?: TextDocumentSyncOptions | TextDocumentSyncKind;
   *   notebookDocumentSync?: NotebookDocumentSyncOptions
   *     | NotebookDocumentSyncRegistrationOptions;
   *   completionProvider?: CompletionOptions;
   *   hoverProvider?: boolean | HoverOptions;
   *   signatureHelpProvider?: SignatureHelpOptions;
   *   declarationProvider?: boolean | DeclarationOptions
   *     | DeclarationRegistrationOptions;
   *   definitionProvider?: boolean | DefinitionOptions;
   *   typeDefinitionProvider?: boolean | TypeDefinitionOptions
   *     | TypeDefinitionRegistrationOptions;
   *   implementationProvider?: boolean | ImplementationOptions
   *     | ImplementationRegistrationOptions;
   *   referencesProvider?: boolean | ReferenceOptions;
   *   documentHighlightProvider?: boolean | DocumentHighlightOptions;
   *   documentSymbolProvider?: boolean | DocumentSymbolOptions;
   *   codeActionProvider?: boolean | CodeActionOptions;
   *   codeLensProvider?: CodeLensOptions;
   *   documentLinkProvider?: DocumentLinkOptions;
   *   colorProvider?: boolean | DocumentColorOptions
   *     | DocumentColorRegistrationOptions;
   *   documentFormattingProvider?: boolean | DocumentFormattingOptions;
   *   documentRangeFormattingProvider?: boolean | DocumentRangeFormattingOptions;
   *   documentOnTypeFormattingProvider?: DocumentOnTypeFormattingOptions;
   *   renameProvider?: boolean | RenameOptions;
   *   foldingRangeProvider?: boolean | FoldingRangeOptions
   *     | FoldingRangeRegistrationOptions;
   *   executeCommandProvider?: ExecuteCommandOptions;
   *   selectionRangeProvider?: boolean | SelectionRangeOptions
   *     | SelectionRangeRegistrationOptions;
   *   linkedEditingRangeProvider?: boolean | LinkedEditingRangeOptions
   *     | LinkedEditingRangeRegistrationOptions;
   *   callHierarchyProvider?: boolean | CallHierarchyOptions
   *     | CallHierarchyRegistrationOptions;
   *   semanticTokensProvider?: SemanticTokensOptions
   *     | SemanticTokensRegistrationOptions;
   *   monikerProvider?: boolean | MonikerOptions | MonikerRegistrationOptions;
   *   typeHierarchyProvider?: boolean | TypeHierarchyOptions
   *     | TypeHierarchyRegistrationOptions;
   *   inlineValueProvider?: boolean | InlineValueOptions
   *     | InlineValueRegistrationOptions;
   *   inlayHintProvider?: boolean | InlayHintOptions
   *     | InlayHintRegistrationOptions;
   *   diagnosticProvider?: DiagnosticOptions | DiagnosticRegistrationOptions;
   *   workspaceSymbolProvider?: boolean | WorkspaceSymbolOptions;
   *   workspace?: {
   *     workspaceFolders?: WorkspaceFoldersServerCapabilities;
   *     fileOperations?: {
   *       didCreate?: FileOperationRegistrationOptions;
   *       willCreate?: FileOperationRegistrationOptions;
   *       didRename?: FileOperationRegistrationOptions;
   *       willRename?: FileOperationRegistrationOptions;
   *       didDelete?: FileOperationRegistrationOptions;
   *       willDelete?: FileOperationRegistrationOptions;
   *     };
   *   };
   *   experimental?: LSPAny;
   * }
   */
  struct ServerCapabilities {

    /**
     * The position encoding the server picked from the encodings offered
     * by the client via the client capability `general.positionEncodings`.
     *
     * If the client didn't provide any position encodings the only valid
     * value that a server can return is 'utf-16'.
     *
     * If omitted it defaults to 'utf-16'.
     *
     * positionEncoding?: PositionEncodingKind;
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<PositionEncodingKind>> positionEncoding;

    /**
     * Defines how text documents are synced. Is either a detailed structure
     * defining each notification or for backwards compatibility the
     * TextDocumentSyncKind number. If omitted it defaults to
     * `TextDocumentSyncKind.None`.
     *
     * textDocumentSync?: TextDocumentSyncOptions | TextDocumentSyncKind;
     */
    std::optional<std::unique_ptr<TextDocumentSync>> textDocumentSync;

    /**
     * Defines how notebook documents are synced.
     *
     * notebookDocumentSync?: NotebookDocumentSyncOptions
     *   | NotebookDocumentSyncRegistrationOptions;
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<NotebookDocumentSync>> notebookDocumentSync;

    /**
     * The server provides completion support.
     *
     * completionProvider?: CompletionOptions;
     */
    std::optional<std::unique_ptr<CompletionOptions>> completionProvider;

    /**
     * The server provides hover support.
     *
     * hoverProvider?: boolean | HoverOptions;
     */
    std::optional<std::unique_ptr<HoverProvider>> hoverProvider;

    /**
     * The server provides signature help support.
     *
     * signatureHelpProvider?: SignatureHelpOptions;
     */
    std::optional<std::unique_ptr<SignatureHelpOptions>> signatureHelpProvider;

    /**
     * The server provides go to declaration support.
     *
     * declarationProvider?: boolean | DeclarationOptions
     *   | DeclarationRegistrationOptions;
     *
     * @since 3.14.0
     */
    std::optional<std::unique_ptr<DeclarationProvider>> declarationProvider;

    /**
     * The server provides goto definition support.
     *
     * definitionProvider?: boolean | DefinitionOptions;
     */
    std::optional<std::unique_ptr<DefinitionProvider>> definitionProvider;

    /**
     * The server provides goto type definition support.
     *
     * typeDefinitionProvider?: boolean | TypeDefinitionOptions
     *   | TypeDefinitionRegistrationOptions;
     *
     * @since 3.6.0
     */
    std::optional<std::unique_ptr<TypeDefinitionProvider>> typeDefinitionProvider;

    /**
     * The server provides goto implementation support.
     *
     * implementationProvider?: boolean | ImplementationOptions
     *   | ImplementationRegistrationOptions;
     *
     * @since 3.6.0
     */
    std::optional<std::unique_ptr<ImplementationProvider>> implementationProvider;

    /**
     * The server provides find references support.
     *
     * referencesProvider?: boolean | ReferenceOptions;
     */
    std::optional<std::unique_ptr<ReferencesProvider>> referencesProvider;

    /**
     * The server provides document highlight support.
     *
     * documentHighlightProvider?: boolean | DocumentHighlightOptions;
     */
    std::optional<std::unique_ptr<DocumentHighlightProvider>> documentHighlightProvider;

    /**
     * The server provides document symbol support.
     *
     * documentSymbolProvider?: boolean | DocumentSymbolOptions;
     */
    std::optional<std::unique_ptr<DocumentSymbolProvider>> documentSymbolProvider;

    /**
     * The server provides code actions. The `CodeActionOptions` return type is
     * only valid if the client signals code action literal support via the
     * property `textDocument.codeAction.codeActionLiteralSupport`.
     *
     * codeActionProvider?: boolean | CodeActionOptions;
     */
    std::optional<std::unique_ptr<CodeActionProvider>> codeActionProvider;

    /**
     * The server provides code lens.
     *
     * codeLensProvider?: CodeLensOptions;
     */
    std::optional<std::unique_ptr<CodeLensOptions>> codeLensProvider;

    /**
     * The server provides document link support.
     *
     * documentLinkProvider?: DocumentLinkOptions;
     */
    std::optional<std::unique_ptr<DocumentLinkOptions>> documentLinkProvider;

    /**
     * The server provides color provider support.
     *
     * colorProvider?: boolean | DocumentColorOptions
     *   | DocumentColorRegistrationOptions;
     *
     * @since 3.6.0
     */
    std::optional<std::unique_ptr<ColorProvider>> colorProvider;

    /**
     * The server provides document formatting.
     *
     * documentFormattingProvider?: boolean | DocumentFormattingOptions;
     */
    std::optional<std::unique_ptr<DocumentFormattingProvider>> documentFormattingProvider;

    /**
     * The server provides document range formatting.
     *
     * documentRangeFormattingProvider?: boolean | DocumentRangeFormattingOptions;
     */
    std::optional<std::unique_ptr<DocumentRangeFormattingProvider>> documentRangeFormattingProvider;

    /**
     * The server provides document formatting on typing.
     *
     * documentOnTypeFormattingProvider?: DocumentOnTypeFormattingOptions;
     */
    std::optional<std::unique_ptr<DocumentOnTypeFormattingOptions>> documentOnTypeFormattingProvider;

    /**
     * The server provides rename support. RenameOptions may only be specified
     * if the client states that it supports `prepareSupport` in its initial
     * `initialize` request.
     *
     * renameProvider?: boolean | RenameOptions;
     */
    std::optional<std::unique_ptr<RenameProvider>> renameProvider;

    /**
     * The server provides folding provider support.
     *
     * foldingRangeProvider?: boolean | FoldingRangeOptions
     *   | FoldingRangeRegistrationOptions;
     *
     * @since 3.10.0
     */
    std::optional<std::unique_ptr<FoldingRangeProvider>> foldingRangeProvider;

    /**
     * The server provides execute command support.
     *
     * executeCommandProvider?: ExecuteCommandOptions;
     */
    std::optional<std::unique_ptr<ExecuteCommandOptions>> executeCommandProvider;

    /**
     * The server provides selection range support.
     *
     * selectionRangeProvider?: boolean | SelectionRangeOptions
     *   | SelectionRangeRegistrationOptions;
     *
     * @since 3.15.0
     */
    std::optional<std::unique_ptr<SelectionRangeProvider>> selectionRangeProvider;

    /**
     * The server provides linked editing range support.
     *
     * linkedEditingRangeProvider?: boolean | LinkedEditingRangeOptions
     *   | LinkedEditingRangeRegistrationOptions;
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<LinkedEditingRangeProvider>> linkedEditingRangeProvider;

    /**
     * The server provides call hierarchy support.
     *
     * callHierarchyProvider?: boolean | CallHierarchyOptions
     *   | CallHierarchyRegistrationOptions;
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<CallHierarchyProvider>> callHierarchyProvider;

    /**
     * The server provides semantic tokens support.
     *
     * semanticTokensProvider?: SemanticTokensOptions
     *   | SemanticTokensRegistrationOptions;
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<SemanticTokensProvider>> semanticTokensProvider;

    /**
     * Whether server provides moniker support.
     *
     * monikerProvider?: boolean | MonikerOptions | MonikerRegistrationOptions;
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<MonikerProvider>> monikerProvider;

    /**
     * The server provides type hierarchy support.
     *
     * typeHierarchyProvider?: boolean | TypeHierarchyOptions
     *   | TypeHierarchyRegistrationOptions;
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<TypeHierarchyProvider>> typeHierarchyProvider;

    /**
     * The server provides inline values.
     *
     * inlineValueProvider?: boolean | InlineValueOptions
     *   | InlineValueRegistrationOptions;
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<InlineValueProvider>> inlineValueProvider;

    /**
     * The server provides inlay hints.
     *
     * inlayHintProvider?: boolean | InlayHintOptions
     *   | InlayHintRegistrationOptions;
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<InlayHintProvider>> inlayHintProvider;

    /**
     * The server has support for pull model diagnostics.
     *
     * diagnosticProvider?: DiagnosticOptions | DiagnosticRegistrationOptions;
     *
     * @since 3.17.0
     */
    std::optional<std::unique_ptr<DiagnosticProvider>> diagnosticProvider;

    /**
     * The server provides workspace symbol support.
     *
     * workspaceSymbolProvider?: boolean | WorkspaceSymbolOptions;
     */
    std::optional<std::unique_ptr<WorkspaceSymbolProvider>> workspaceSymbolProvider;

    /**
     * Workspace specific server capabilities
     *
     * workspace?: {
     *   workspaceFolders?: WorkspaceFoldersServerCapabilities;
     *   fileOperations?: {
     *     didCreate?: FileOperationRegistrationOptions;
     *     willCreate?: FileOperationRegistrationOptions;
     *     didRename?: FileOperationRegistrationOptions;
     *     willRename?: FileOperationRegistrationOptions;
     *     didDelete?: FileOperationRegistrationOptions;
     *     willDelete?: FileOperationRegistrationOptions;
     *   };
     * };
     */
    std::optional<std::unique_ptr<WorkspaceProvider>> workspace;

    /**
     * Experimental server capabilities.
     *
     * experimental?: LSPAny;
     */
    std::optional<std::unique_ptr<LSPAny>> experimental;
  };

  /**
   * The initialized notification is sent from the client to the server after
   * the client received the result of the initialize request but before the
   * client is sending any other request or notification to the server. The
   * server can use the initialized notification, for example, to dynamically
   * register capabilities. The initialized notification may only be sent once.
   *
   * Notification:
   *   method: ‘initialized’
   *   params: InitializedParams defined as follows:
   *
   * interface InitializedParams {
   * }
   */
  struct InitializedParams {
    // empty
  };

  /**
   * The client/registerCapability request is sent from the server to the client
   * to register for a new capability on the client side. Not all clients need
   * to support dynamic capability registration. A client opts in via the
   * dynamicRegistration property on the specific client capabilities. A client
   * can even provide dynamic registration for capability A but not for
   * capability B (see TextDocumentClientCapabilities as an example).
   *
   * Server must not register the same capability both statically through the
   * initialize result and dynamically for the same document selector. If a
   * server wants to support both static and dynamic registration it needs to
   * check the client capability in the initialize request and only register the
   * capability statically if the client doesn’t support dynamic registration
   * for that capability.
   *
   * Request:
   *   method: ‘client/registerCapability’
   *   params: RegistrationParams
   *
   * Where RegistrationParams are defined as follows:
   *
   * export interface Registration {
   *   id: string;
   *   method: string;
   *   registerOptions?: LSPAny;
   * }
   */
  struct Registration {

    /**
     * The id used to register the request. The id can be used to deregister
     * the request again.
     *
     * id: string;
     */
    string id;

    /**
     * The method / capability to register for.
     *
     * method: string;
     */
    string method;

    /**
     * Options necessary for the registration.
     *
     * registerOptions?: LSPAny;
     */
    std::optional<std::unique_ptr<LSPAny>> registerOptions;
  };

  /**
   * export interface RegistrationParams {
   *   registrations: Registration[];
   * }
   */
  struct RegistrationParams {

    /**
     * registrations: Registration[];
     */
    std::unique_ptr<std::vector<std::unique_ptr<Registration>>> registrations;
  };

  /**
   * Since most of the registration options require to specify a document
   * selector there is a base interface that can be used. See
   * TextDocumentRegistrationOptions.
   *
   * Response:
   *   result: void.
   *   error: code and message set in case an exception happens during the request.
   *
   * StaticRegistrationOptions can be used to register a feature in the
   * initialize result with a given server control ID to be able to un-register
   * the feature later on.
   *
   * export interface StaticRegistrationOptions {
   *   id?: string;
   * }
   */
  struct StaticRegistrationOptions {

    /**
     * The id used to register the request. The id can be used to deregister
     * the request again. See also Registration#id.
     *
     * id?: string;
     */
    std::optional<string> id;
  };

  /**
   * TextDocumentRegistrationOptions can be used to dynamically register for
   * requests for a set of text documents.
   *
   * export interface TextDocumentRegistrationOptions {
   *   documentSelector: DocumentSelector | null;
   * }
   */
  struct TextDocumentRegistrationOptions {

    /**
     * A document selector to identify the scope of the registration. If set to
     * null the document selector provided on the client side will be used.
     *
     * documentSelector: DocumentSelector | null;
     */
    std::optional<std::unique_ptr<DocumentSelector>> documentSelector;
  };

  /**
   * The client/unregisterCapability request is sent from the server to the
   * client to unregister a previously registered capability.
   *
   * Request:
   *   method: ‘client/unregisterCapability’
   *   params: UnregistrationParams
   *
   * Where UnregistrationParams are defined as follows:
   *
   * export interface Unregistration {
   *   id: string;
   *   method: string;
   * }
   */
  struct Unregistration {

    /**
     * The id used to unregister the request or notification. Usually an id
     * provided during the register request.
     *
     * id: string;
     */
    string id;

    /**
     * The method / capability to unregister for.
     *
     * method: string;
     */
    string method;
  };

  /**
   * export interface UnregistrationParams {
   *   unregisterations: Unregistration[];
   * }
   */
  struct UnregistrationParams {

    /**
     * This should correctly be named `unregistrations`. However changing this
     * is a breaking change and needs to wait until we deliver a 4.x version of
     * the specification.
     *
     * unregisterations: Unregistration[];
     */
    std::unique_ptr<std::vector<std::unique_ptr<Unregistration>>> unregisterations;  //<- Typo is part of spec!
  };

  /**
   * A notification that should be used by the client to modify the trace setting of the server.
   *
   * Notification:
   *   method: ‘$/setTrace’
   *   params: SetTraceParams defined as follows:
   *
   * interface SetTraceParams {
   *   value: TraceValue;
   * }
   */
  struct SetTraceParams {

    /**
     * The new value that should be assigned to the trace setting.
     *
     * value: TraceValue;
     */
    TraceValue value;
  };

  /**
   * A notification to log the trace of the server’s execution. The amount and
   * content of these notifications depends on the current trace configuration.
   * If trace is 'off', the server should not send any logTrace notification. If
   * trace is 'messages', the server should not add the 'verbose' field in the
   * LogTraceParams.
   *
   * $/logTrace should be used for systematic trace reporting. For single
   * debugging messages, the server should send window/logMessage notifications.
   *
   * Notification:
   *   method: ‘$/logTrace’
   *   params: LogTraceParams defined as follows:
   *
   * interface LogTraceParams {
   *   message: string;
   *   verbose?: string;
   * }
   */
  struct LogTraceParams {

    /**
     * The message to be logged.
     *
     * message: string;
     */
    string message;

    /**
     * Additional information that can be computed if the `trace` configuration
     * is set to `'verbose'`
     *
     * verbose?: string;
     */
    std::optional<string> verbose;
  };

  /**
   * Response:
   *   result: InitializeResult defined as follows:
   *
   * interface InitializeResult {
   *   capabilities: ServerCapabilities;
   *   serverInfo?: {
   *     name: string;
   *     version?: string;
   *   };
   * }
   */
  struct InitializeResult {

    /**
     * The capabilities the language server provides.
     *
     * capabilities: ServerCapabilities;
     */
    std::unique_ptr<ServerCapabilities> capabilities;

    /**
     * Information about the server.
     *
     * serverInfo?: {
     *   name: string;
     *   version?: string;
     * };
     *
     * @since 3.15.0
     */
    std::optional<std::unique_ptr<ServerInfo>> serverInfo;
  };

  /**
   * error.code
   *
   * Known error codes for an `InitializeErrorCodes`;
   *
   * export namespace InitializeErrorCodes {
   *   export const unknownProtocolVersion: 1 = 1;
   * }
   * export type InitializeErrorCodes = 1;
   */
  enum class InitializeErrorCodes {

    /**
     * If the protocol version provided by the client can't be handled by
     * the server.
     *
     * export const unknownProtocolVersion: 1 = 1;
     *
     * @deprecated This initialize error got replaced by client capabilities.
     * There is no version handshake in version 3.0x
     */
    unknownProtocolVersion = 1,
  };

  extern std::map<InitializeErrorCodes, std::string> InitializeErrorCodesNames;

  auto initializeErrorCodes(const std::string &name) -> InitializeErrorCodes;

  /**
   * error.data
   *
   * interface InitializeError {
   *   retry: boolean;
   * }
   */
  struct InitializeError {

    /**
     * Indicates whether the client execute the following retry logic:
     * (1) show the message provided by the ResponseError to the user
     * (2) user selects retry or cancel
     * (3) if user selected retry the initialize method is sent again.
     *
     * retry: boolean;
     */
    boolean retry;
  };

  /**
   * The initialize request is sent as the first request from the client to the
   * server. If the server receives a request or notification before the
   * initialize request it should act as follows:
   * - For a request the response should be an error with code: -32002. The
   *   message can be picked by the server.
   * - Notifications should be dropped, except for the exit notification. This
   *   will allow the exit of a server without an initialize request.
   *
   * Until the server has responded to the initialize request with an
   * InitializeResult, the client must not send any additional requests or
   * notifications to the server. In addition the server is not allowed to send
   * any requests or notifications to the client until it has responded with an
   * InitializeResult, with the exception that during the initialize request the
   * server is allowed to send the notifications window/showMessage,
   * window/logMessage and telemetry/event as well as the
   * window/showMessageRequest request to the client. In case the client sets up
   * a progress token in the initialize params (e.g. property workDoneToken) the
   * server is also allowed to use that token (and only that token) using the
   * $/progress notification sent from the server to the client.
   *
   * The initialize request may only be sent once.
   *
   * Request:
   *   method: ‘initialize’
   *   params: InitializeParams defined as follows:
   *
   * interface InitializeParams extends WorkDoneProgressParams {
   *   processId: integer | null;
   *   clientInfo?: {
   *     name: string;
   *     version?: string;
   *   };
   *   locale?: string;
   *   rootPath?: string | null;
   *   rootUri: DocumentUri | null;
   *   initializationOptions?: LSPAny;
   *   capabilities: ClientCapabilities;
   *   trace?: TraceValue;
   *   workspaceFolders?: WorkspaceFolder[] | null;
   * }
   */
  struct InitializeParams : public WorkDoneProgressParams {

    /**
     * The process Id of the parent process that started the server. Is null if
     * the process has not been started by another process. If the parent
     * process is not alive then the server should exit (see exit notification)
     * its process.
     *
     * processId: integer | null;
     */
    std::optional<integer> processId;

    /**
     * Information about the client
     *
     * clientInfo?: {
     *   name: string;
     *   version?: string;
     * };
     *
     * @since 3.15.0
     */
    std::optional<std::unique_ptr<ClientInfo>> clientInfo;

    /**
     * The locale the client is currently showing the user interface
     * in. This must not necessarily be the locale of the operating
     * system.
     *
     * Uses IETF language tags as the value's syntax
     * (See https://en.wikipedia.org/wiki/IETF_language_tag)
     *
     * locale?: string;
     *
     * @since 3.16.0
     */
    std::optional<string> locale;

    /**
     * The rootPath of the workspace. Is null
     * if no folder is open.
     *
     * rootPath?: string | null;
     *
     * @deprecated in favour of `rootUri`.
     */
    std::optional<string> rootPath;

    /**
     * The rootUri of the workspace. Is null if no
     * folder is open. If both `rootPath` and `rootUri` are set
     * `rootUri` wins.
     *
     * rootUri: DocumentUri | null;
     *
     * @deprecated in favour of `workspaceFolders`
     */
    std::optional<DocumentUri> rootUri;

    /**
     * User provided initialization options.
     *
     * initializationOptions?: LSPAny;
     */
    std::optional<std::unique_ptr<LSPAny>> initializationOptions;

    /**
     * The capabilities provided by the client (editor or tool)
     *
     * capabilities: ClientCapabilities;
     */
    std::unique_ptr<ClientCapabilities> capabilities;

    /**
     * The initial trace setting. If omitted trace is disabled ('off').
     *
     * trace?: TraceValue;
     */
    std::optional<TraceValue> trace;

    /**
     * The workspace folders configured in the client when the server starts.
     * This property is only available if the client supports workspace folders.
     * It can be `null` if the client supports workspace folders but none are
     * configured.
     *
     * workspaceFolders?: WorkspaceFolder[] | null;
     *
     * @since 3.6.0
     */
    std::optional<std::unique_ptr<std::vector<std::unique_ptr<WorkspaceFolder>>>> workspaceFolders;
  };

  /**
   * The document open notification is sent from the client to the server to
   * signal newly opened text documents. The document’s content is now managed
   * by the client and the server must not try to read the document’s content
   * using the document’s Uri. Open in this sense means it is managed by the
   * client. It doesn’t necessarily mean that its content is presented in an
   * editor. An open notification must not be sent more than once without a
   * corresponding close notification send before. This means open and close
   * notification must be balanced and the max open count for a particular
   * textDocument is one. Note that a server’s ability to fulfill requests is
   * independent of whether a text document is open or closed.
   *
   * The DidOpenTextDocumentParams contain the language id the document is
   * associated with. If the language id of a document changes, the client needs
   * to send a textDocument/didClose to the server followed by a
   * textDocument/didOpen with the new language id if the server handles the new
   * language id as well.
   *
   * Client Capability: See general synchronization client capabilities.
   *
   * Server Capability: See general synchronization server capabilities.
   *
   * Registration Options: TextDocumentRegistrationOptions
   *
   * Notification:
   *   method: ‘textDocument/didOpen’
   *   params: DidOpenTextDocumentParams defined as follows:
   *
   * interface DidOpenTextDocumentParams {
   *   textDocument: TextDocumentItem;
   * }
   */
  struct DidOpenTextDocumentParams {

    /**
     * The document that was opened.
     *
     * textDocument: TextDocumentItem;
     */
    std::unique_ptr<TextDocumentItem> textDocument;
  };

  /**
   * Describe options to be used when registering for text document change
   * events.
   *
   * The document change notification is sent from the client to the server to
   * signal changes to a text document. Before a client can change a text
   * document it must claim ownership of its content using the
   * textDocument/didOpen notification. In 2.0 the shape of the params has
   * changed to include proper version numbers.
   *
   * Client Capability: See general synchronization client capabilities.
   *
   * Server Capability: See general synchronization server capabilities.
   *
   * Registration Options: TextDocumentChangeRegistrationOptions defined as
   * follows:
   *
   * export interface TextDocumentChangeRegistrationOptions
   *     extends TextDocumentRegistrationOptions {
   *   syncKind: TextDocumentSyncKind;
   * }
   */
  struct TextDocumentChangeRegistrationOptions : public TextDocumentRegistrationOptions {

    /**
     * How documents are synced to the server. See TextDocumentSyncKind.Full and
     * TextDocumentSyncKind.Incremental.
     *
     * syncKind: TextDocumentSyncKind;
     */
    TextDocumentSyncKind syncKind;
  };

  struct PartialTextDocumentContentChangeEvent {

    /**
     * The range of the document that changed.
     *
     * range: Range;
     */
    std::unique_ptr<Range> range;

    /**
     * The optional length of the range that got replaced.
     *
     * rangeLength?: uinteger;
     *
     * @deprecated use range instead.
     */
    std::optional<uinteger> rangeLength;

    /**
     * The new text for the provided range.
     *
     * text: string;
     */
    string text;
  };

  struct WholeTextDocumentContentChangeEvent {

    /**
     * The new text of the whole document.
     *
     * text: string;
     */
    string text;
  };

  enum class TextDocumentContentChangeEventType {
    PARTIAL_TEXT_DOCUMENT,
    WHOLE_TEXT_DOCUMENT,
  };

  /**
   * An event describing a change to a text document. If only a text is provided
   * it is considered to be the full content of the document.
   */
  struct TextDocumentContentChangeEvent {
    TextDocumentContentChangeEventType type;
    union {
      std::unique_ptr<PartialTextDocumentContentChangeEvent> partialTextDocumentContentChangeEvent;
      std::unique_ptr<WholeTextDocumentContentChangeEvent> wholeTextDocumentContentChangeEvent;
    };
  };

  /**
   * Notification:
   *   method: textDocument/didChange
   *   params: DidChangeTextDocumentParams defined as follows:
   *
   * interface DidChangeTextDocumentParams {
   *   textDocument: VersionedTextDocumentIdentifier;
   *   contentChanges: TextDocumentContentChangeEvent[];
   * }
   */
  struct DidChangeTextDocumentParams {

    /**
     * The document that did change. The version number points
     * to the version after all provided content changes have
     * been applied.
     *
     * textDocument: VersionedTextDocumentIdentifier;
     */
    std::unique_ptr<VersionedTextDocumentIdentifier> textDocument;

    /**
     * The actual content changes. The content changes describe single state
     * changes to the document. So if there are two content changes c1 (at array
     * index 0) and c2 (at array index 1) for a document in state S then c1
     * moves the document from S to S' and c2 from S' to S''. So c1 is computed
     * on the state S and c2 is computed on the state S'.
     *
     * To mirror the content of a document using change events use the following
     * approach:
     * - start with the same initial content
     * - apply the 'textDocument/didChange' notifications in the order you
     *   receive them.
     * - apply the `TextDocumentContentChangeEvent`s in a single notification in
     *   the order you receive them.
     *
     * contentChanges: TextDocumentContentChangeEvent[];
     */
    std::unique_ptr<std::vector<std::unique_ptr<TextDocumentContentChangeEvent>>> contentChanges;
  };

  /**
   * Represents reasons why a text document is saved.
   *
   * export namespace TextDocumentSaveReason {
   *   export const Manual = 1;
   *   export const AfterDelay = 2;
   *   export const FocusOut = 3;
   * }
   * export type TextDocumentSaveReason = 1 | 2 | 3;
   */
  enum class TextDocumentSaveReason {

    /**
     * Manually triggered, e.g. by the user pressing save, by starting
     * debugging, or by an API call.
     *
     * export const Manual = 1;
     */
    Manual = 1,

    /**
     * Automatic after a delay.
     *
     * export const AfterDelay = 2;
     */
    AfterDelay = 2,

    /**
     * When the editor lost focus.
     *
     * export const FocusOut = 3;
     */
    FocusOut = 3,
  };

  extern std::map<TextDocumentSaveReason, std::string> TextDocumentSaveReasonNames;

  auto textDocumentSaveReason(const std::string &name) -> TextDocumentSaveReason;

  /**
   * The document will save notification is sent from the client to the server
   * before the document is actually saved. If a server has registered for open
   * / close events clients should ensure that the document is open before a
   * willSave notification is sent since clients can’t change the content of a
   * file without ownership transferal.
   *
   * Client Capability:
   *   property name (optional): textDocument.synchronization.willSave
   *   property type: boolean
   *
   * The capability indicates that the client supports textDocument/willSave
   * notifications.
   *
   * Server Capability:
   *   property name (optional): textDocumentSync.willSave
   *   property type: boolean
   *
   * The capability indicates that the server is interested in
   * textDocument/willSave notifications.
   *
   * Registration Options: TextDocumentRegistrationOptions
   *
   * Notification:
   *   method: ‘textDocument/willSave’
   *   params: WillSaveTextDocumentParams defined as follows:
   *
   * export interface WillSaveTextDocumentParams {
   *   textDocument: TextDocumentIdentifier;
   *   reason: TextDocumentSaveReason;
   * }
   */
  struct WillSaveTextDocumentParams {

    /**
     * The document that will be saved.
     *
     * textDocument: TextDocumentIdentifier;
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;

    /**
     * The 'TextDocumentSaveReason'.
     *
     * reason: TextDocumentSaveReason;
     */
    std::unique_ptr<TextDocumentSaveReason> reason;
  };

  /**
   * The capability indicates that the server is interested in
   * textDocument/didSave notifications.
   *
   * Registration Options: TextDocumentSaveRegistrationOptions defined as
   * follows:
   *
   * export interface TextDocumentSaveRegistrationOptions
   *     extends TextDocumentRegistrationOptions {
   *   includeText?: boolean;
   * }
   */
  struct TextDocumentSaveRegistrationOptions : public TextDocumentRegistrationOptions {

    /**
     * The client is supposed to include the content on save.
     *
     * includeText?: boolean;
     */
    std::optional<boolean> includeText;
  };

  /**
   * Notification:
   *   method: textDocument/didSave
   *   params: DidSaveTextDocumentParams defined as follows:
   *
   * interface DidSaveTextDocumentParams {
   *   textDocument: TextDocumentIdentifier;
   *   text?: string;
   * }
   */
  struct DidSaveTextDocumentParams {

    /**
     * The document that was saved.
     *
     * textDocument: TextDocumentIdentifier;
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;

    /**
     * Optional the content when saved. Depends on the includeText value when
     * the save notification was requested.
     *
     * text?: string;
     */
    std::optional<string> text;
  };

  /**
   * The document close notification is sent from the client to the server when
   * the document got closed in the client. The document’s master now exists
   * where the document’s Uri points to (e.g. if the document’s Uri is a file
   * Uri the master now exists on disk). As with the open notification the close
   * notification is about managing the document’s content. Receiving a close
   * notification doesn’t mean that the document was open in an editor before. A
   * close notification requires a previous open notification to be sent. Note
   * that a server’s ability to fulfill requests is independent of whether a
   * text document is open or closed.
   *
   * Client Capability: See general synchronization client capabilities.
   *
   * Server Capability: See general synchronization server capabilities.
   *
   * Registration Options: TextDocumentRegistrationOptions
   *
   * Notification:
   *   method: textDocument/didClose
   *   params: DidCloseTextDocumentParams defined as follows:
   *
   * interface DidCloseTextDocumentParams {
   *   textDocument: TextDocumentIdentifier;
   * }
   */
  struct DidCloseTextDocumentParams {

    /**
     * The document that was closed.
     *
     * textDocument: TextDocumentIdentifier;
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
  };

} // namespace LCompilers::LanguageServiceProvider

#endif // LCOMPILERS_LSP_SPECIFICATION_H
