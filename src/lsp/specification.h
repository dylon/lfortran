#ifndef LCOMPILERS_LSP_SPECIFICATION_H
#define LCOMPILERS_LSP_SPECIFICATION_H

#include <cstddef>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

/**
 * Interface definitions from the LSP 3.17 specification.
 * See: https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification
 */
namespace LCompilers::LanguageServerProtocol {

  template <typename T>
  using ptr_vector = std::vector<std::unique_ptr<T>>;

  template <typename T>
  using optional_ptr = std::optional<std::unique_ptr<T>>;

  template <typename T>
  using optional_vector = std::optional<std::vector<T>>;

  template <typename T>
  using optional_ptr_vector = std::optional<ptr_vector<T>>;

  template <typename T>
  using optional_ptr_vector = std::optional<ptr_vector<T>>;

  template <typename T>
  struct ValueSet {
    ptr_vector<T> valueSet;
  };

  template <typename T>
  struct OptionalValueSet {
    optional_ptr_vector<T> valueSet;
  };

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
    std::variant<
      std::unique_ptr<LSPObject>,
      std::unique_ptr<LSPArray>,
      string,
      integer,
      uinteger,
      decimal,
      boolean,
      null
    > value;
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
    std::variant<
      integer,
      string
    > value;
  };

  enum class RequestParamsType {
    LSP_ARRAY,
    LSP_OBJECT,
  };

  struct RequestParams {
    RequestParamsType type;
    std::variant<
      std::unique_ptr<LSPArray>,
      std::unique_ptr<LSPObject>
    > value;
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
    optional_ptr<RequestParams> params;
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

  extern std::map<ErrorCodes, std::string> ErrorCodeNames;

  auto errorCodeByName(const std::string &name) -> ErrorCodes;

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
    optional_ptr<LSPAny> data;
  };

  enum class ResponseIdType {
    LSP_INTEGER,
    LSP_STRING,
    LSP_NULL,
  };

  struct ResponseId {
    ResponseIdType type;
    std::variant<
      integer,
      string,
      null
    > value;
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
    optional_ptr<LSPAny> result;

    /**
     * The error object in case a request fails.
     *
     * error?: ResponseError;
     */
    optional_ptr<ResponseError> error;
  };

  enum class NotificationParamsType {
    LSP_ARRAY,
    LSP_OBJECT,
  };

  struct NotificationParams {
    NotificationParamsType type;
    std::variant<
      std::unique_ptr<LSPArray>,
      std::unique_ptr<LSPObject>
    > value;
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
    optional_ptr<NotificationParams> params;
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
    std::variant<
      integer,
      string
    > value;
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

  auto positionEncodingKindByName(const std::string &name) -> PositionEncodingKind;

  extern std::map<PositionEncodingKind, std::string> PositionEncodingKindValues;

  auto positionEncodingKindByValue(const std::string &value) -> PositionEncodingKind;

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
   * Client capabilities specific to regular expressions.
   *
   * The following client capability is used to announce a client’s regular
   * expression engine
   * - property path (optional): general.regularExpressions
   * - property type: RegularExpressionsClientCapabilities defined as follows:
   *
   * export interface RegularExpressionsClientCapabilities {
   *   engine: string;
   *   version?: string;
   * }
   *
   * The following table lists the well known engine values. Please note that
   * the table should be driven by the community which integrates LSP into
   * existing clients. It is not the goal of the spec to list all available
   * regular expression engines.
   *
   * +------------+---------+-----------------------+
   * | Engine     | Version | Documentation         |
   * +------------+---------+-----------------------+
   * | ECMAScript | ES2020  | ECMAScript 2020 & MDN |
   * +------------+---------+-----------------------+
   *
   * Regular Expression Subset:
   *
   * The following features from the ECMAScript 2020 regular expression
   * specification are NOT mandatory for a client:
   * - Assertions: Lookahead assertion, Negative lookahead assertion, lookbehind
   *   assertion, negative lookbehind assertion.
   * - Character classes: matching control characters using caret notation and
   *   matching UTF-16 code units.
   * - Group and ranges: named capturing groups.
   * - Unicode property escapes: none of the features needs to be supported.
   *
   * The only regular expression flag that a client needs to support is ‘i’ to
   * specify a case insensitive search.
   */
  struct RegularExpressionsClientCapabilities {

    /**
     * The engine's name.
     *
     * engine: string;
     */
    string engine;

    /**
     * The engine's version.
     *
     * version?: string;
     */
    std::optional<string> version;
  };

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
    std::variant<
      std::unique_ptr<TextEdit>,
      std::unique_ptr<AnnotatedTextEdit>
    > value;
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
    ptr_vector<OptionalAnnotatedTextEdit> edits;
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
    optional_ptr<Range> originSelectionRange;

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

  auto diagnosticSeverityByName(const std::string &name) -> DiagnosticSeverity;

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

  auto diagnosticTagByName(const std::string &name) -> DiagnosticTag;

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
    std::variant<
      integer,
      string
    > value;
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
    optional_ptr<DiagnosticCode> code;

    /**
     * An optional property to describe the error code.
     *
     * codeDescription?: CodeDescription;
     *
     * @since 3.16.0
     */
    optional_ptr<CodeDescription> codeDescription;

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
    optional_ptr_vector<DiagnosticTag> tags;

    /**
     * An array of related diagnostic information, e.g. when symbol-names within
     * a scope collide all definitions can be marked via this property.
     *
     * relatedInformation?: DiagnosticRelatedInformation[];
     */
    optional_ptr_vector<DiagnosticRelatedInformation> relatedInformation;

    /**
     * A data entry field that is preserved between a
     * `textDocument/publishDiagnostics` notification and
     * `textDocument/codeAction` request.
     *
     * data?: LSPAny;
     *
     * @since 3.16.0
     */
    optional_ptr<LSPAny> data;
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
    optional_ptr_vector<LSPAny> arguments;
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

  auto markupKindByName(const std::string &name) -> MarkupKind;

  extern std::map<MarkupKind, std::string> MarkupKindValues;

  auto markupKindByValue(const std::string &value) -> MarkupKind;

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
    optional_ptr<CreateFileOptions> options;

    /**
     * An optional annotation identifier describing the operation.
     *
     * annotationId?: ChangeAnnotationIdentifier;
     *
     * @since 3.16.0
     */
    optional_ptr<ChangeAnnotationIdentifier> annotationId;
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
    optional_ptr<RenameFileOptions> options;

    /**
     * An optional annotation identifier describing the operation.
     *
     * annotationId?: ChangeAnnotationIdentifier;
     *
     * @since 3.16.0
     */
    optional_ptr<ChangeAnnotationIdentifier> annotationId;
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
    optional_ptr<DeleteFileOptions> options;

    /**
     * An optional annotation identifier describing the operation.
     *
     * annotationId?: ChangeAnnotationIdentifier;
     *
     * @since 3.16.0
     */
    optional_ptr<ChangeAnnotationIdentifier> annotationId;
  };

  enum class DocumentChangeType {
    TEXT_DOCUMENT_EDIT,
    CREATE_FILE,
    RENAME_FILE,
    DELETE_FILE,
  };

  struct DocumentChange {
    DocumentChangeType type;
    std::variant<
      std::unique_ptr<TextDocumentEdit>,
      std::unique_ptr<CreateFile>,
      std::unique_ptr<RenameFile>,
      std::unique_ptr<DeleteFile>
    > value;
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
    optional_ptr<std::map<DocumentUri, ptr_vector<TextEdit>>> changes;

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
    optional_ptr_vector<DocumentChange> documentChanges;

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
    optional_ptr<std::map<string, std::unique_ptr<ChangeAnnotation>>> changeAnnotations;
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

  auto resourceOperationKindByName(const std::string &name) -> ResourceOperationKind;

  extern std::map<ResourceOperationKind, std::string> ResourceOperationKindValues;

  auto resourceOperationKindByValue(const std::string &value) -> ResourceOperationKind;

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

  auto failureHandlingKindByName(const std::string &name) -> FailureHandlingKind;

  extern std::map<FailureHandlingKind, std::string> FailureHandlingKindValues;

  auto failureHandlingKindByValue(const std::string &value) -> FailureHandlingKind;

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
    optional_ptr_vector<ResourceOperationKind> resourceOperations;

    /**
     * The failure handling strategy of a client if applying the workspace edit
     * fails.
     *
     * failureHandling?: FailureHandlingKind;
     *
     * @since 3.13.0
     */
    optional_ptr<FailureHandlingKind> failureHandling;

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
    optional_ptr<ChangeAnnotationSupport> changeAnnotationSupport;
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
    optional_ptr<ProgressToken> workDoneToken;
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
    optional_ptr<ProgressToken> partialResultToken;
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

  auto traceValueByName(const std::string &name) -> TraceValue;

  extern std::map<TraceValue, std::string> TraceValueValues;

  auto traceValueByValue(const std::string &value) -> TraceValue;

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
    optional_ptr<DocumentSelector> documentSelector;
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
   * A symbol kind.
   *
   *  export namespace SymbolKind {
   *    export const File = 1;
   *    export const Module = 2;
   *    export const Namespace = 3;
   *    export const Package = 4;
   *    export const Class = 5;
   *    export const Method = 6;
   *    export const Property = 7;
   *    export const Field = 8;
   *    export const Constructor = 9;
   *    export const Enum = 10;
   *    export const Interface = 11;
   *    export const Function = 12;
   *    export const Variable = 13;
   *    export const Constant = 14;
   *    export const String = 15;
   *    export const Number = 16;
   *    export const Boolean = 17;
   *    export const Array = 18;
   *    export const Object = 19;
   *    export const Key = 20;
   *    export const Null = 21;
   *    export const EnumMember = 22;
   *    export const Struct = 23;
   *    export const Event = 24;
   *    export const Operator = 25;
   *    export const TypeParameter = 26;
   *  }
   *  export type SymbolKind = 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
   *    9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 |
   *    20 | 21 | 22 | 23 | 24 | 25 | 26;
   */
  enum class SymbolKind {
    File = 1,
    Module = 2,
    Namespace = 3,
    Package = 4,
    Class = 5,
    Method = 6,
    Property = 7,
    Field = 8,
    Constructor = 9,
    Enum = 10,
    Interface = 11,
    Function = 12,
    Variable = 13,
    Constant = 14,
    String = 15,
    Number = 16,
    Boolean = 17,
    Array = 18,
    Object = 19,
    Key = 20,
    Null = 21,
    EnumMember = 22,
    Struct = 23,
    Event = 24,
    Operator = 25,
    TypeParameter = 26,
  };

  extern std::map<SymbolKind, std::string> SymbolKindNames;

  auto symbolKindByName(const std::string &name) -> SymbolKind;

  /**
   * Symbol tags are extra annotations that tweak the rendering of a symbol.
   *
   * export namespace SymbolTag {
   *   export const Deprecated: 1 = 1;
   * }
   * export type SymbolTag = 1;
   *
   * @since 3.16
   */
  enum class SymbolTag {

    /**
     * Render a symbol as obsolete, usually using a strike-out.
     *
     * export const Deprecated: 1 = 1;
     */
    Deprecated = 1,
  };

  extern std::map<SymbolTag, std::string> SymbolTagNames;

  auto symbolTagByName(const std::string &name) -> SymbolTag;

  struct DocumentSymbol;  // Forward reference for cyclic reference.

  /**
   * Represents programming constructs like variables, classes, interfaces etc.
   * that appear in a document. Document symbols can be hierarchical and they
   * have two ranges: one that encloses its definition and one that points to its
   * most interesting range, e.g. the range of an identifier.
   *
   * export interface DocumentSymbol {
   *   name: string;
   *   detail?: string;
   *   kind: SymbolKind;
   *   tags?: SymbolTag[];
   *   deprecated?: boolean;
   *   range: Range;
   *   selectionRange: Range;
   *   children?: DocumentSymbol[];
   * }
   */
  struct DocumentSymbol {

    /**
     * The name of this symbol. Will be displayed in the user interface and
     * therefore must not be an empty string or a string only consisting of
     * white spaces.
     *
     * name: string;
     */
    string name;

    /**
     * More detail for this symbol, e.g the signature of a function.
     *
     * detail?: string;
     */
    std::optional<string> detail;

    /**
     * The kind of this symbol.
     *
     * kind: SymbolKind;
     */
    SymbolKind kind;

    /**
     * Tags for this document symbol.
     *
     * tags?: SymbolTag[];
     *
     * @since 3.16.0
     */
    optional_ptr<std::vector<SymbolTag>> tags;

    /**
     * Indicates if this symbol is deprecated.
     *
     * deprecated?: boolean;
     *
     * @deprecated Use tags instead
     */
    std::optional<boolean> deprecated;

    /**
     * The range enclosing this symbol not including leading/trailing whitespace
     * but everything else like comments. This information is typically used to
     * determine if the clients cursor is inside the symbol to reveal in the
     * symbol in the UI.
     *
     * range: Range;
     */
    std::unique_ptr<Range> range;

    /**
     * The range that should be selected and revealed when this symbol is being
     * picked, e.g. the name of a function. Must be contained by the `range`.
     *
     * selectionRange: Range;
     */
    std::unique_ptr<Range> selectionRange;

    /**
     * Children of this symbol, e.g. properties of a class.
     *
     * children?: DocumentSymbol[];
     */
    optional_ptr<ptr_vector<DocumentSymbol>> children;
  };

  /**
   * Represents information about programming constructs like variables, classes,
   * interfaces etc.
   *
   * export interface SymbolInformation {
   *   name: string;
   *   kind: SymbolKind;
   *   tags?: SymbolTag[];
   *   deprecated?: boolean;
   *   location: Location;
   *   containerName?: string;
   * }
   *
   * @deprecated use DocumentSymbol or WorkspaceSymbol instead.
   */
  struct SymbolInformation {

    /**
     * The name of this symbol.
     *
     * name: string;
     */
    string name;

    /**
     * The kind of this symbol.
     *
     * kind: SymbolKind;
     */
    SymbolKind kind;

    /**
     * Tags for this symbol.
     *
     * tags?: SymbolTag[];
     *
     * @since 3.16.0
     */
    optional_ptr<std::vector<SymbolTag>> tags;

    /**
     * Indicates if this symbol is deprecated.
     *
     * deprecated?: boolean;
     *
     * @deprecated Use tags instead
     */
    std::optional<boolean> deprecated;

    /**
     * The location of this symbol. The location's range is used by a tool
     * to reveal the location in the editor. If the symbol is selected in the
     * tool the range's start information is used to position the cursor. So
     * the range usually spans more then the actual symbol's name and does
     * normally include things like visibility modifiers.
     *
     * The range doesn't have to denote a node range in the sense of an abstract
     * syntax tree. It can therefore not be used to re-construct a hierarchy of
     * the symbols.
     *
     * location: Location;
     */
    std::unique_ptr<Location> location;

    /**
     * The name of the symbol containing this symbol. This information is for
     * user interface purposes (e.g. to render a qualifier in the user interface
     * if necessary). It can't be used to re-infer a hierarchy for the document
     * symbols.
     *
     * containerName?: string;
     */
    std::optional<string> containerName;
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
    optional_ptr_vector<SymbolTag> tags;

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
    optional_ptr<LSPAny> data;
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
    ptr_vector<Range> fromRanges;
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
    ptr_vector<Range> fromRanges;
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
    optional_ptr_vector<SymbolTag> tags;

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
    optional_ptr<LSPAny> data;
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
   * The document highlight request is sent from the client to the server to
   * resolve document highlights for a given text document position. For
   * programming languages this usually highlights all references to the symbol
   * scoped to this file. However, we kept ‘textDocument/documentHighlight’ and
   * ‘textDocument/references’ separate requests since the first one is allowed
   * to be more fuzzy. Symbol matches usually have a DocumentHighlightKind of
   * Read or Write whereas fuzzy or textual matches use Text as the kind.
   *
   * Client Capability:
   * - property name (optional): textDocument.documentHighlight
   * - property type: DocumentHighlightClientCapabilities defined as follows:
   *
   * export interface DocumentHighlightClientCapabilities {
   *   dynamicRegistration?: boolean;
   * }
   */
  struct DocumentHighlightClientCapabilities {

    /**
     * Whether document highlight supports dynamic registration.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;
  };

  /**
   * Server Capability:
   * - property name (optional): documentHighlightProvider
   * - property type: boolean | DocumentHighlightOptions where
   *   DocumentHighlightOptions is defined as follows:
   *
   * export interface DocumentHighlightOptions extends WorkDoneProgressOptions {
   * }
   */
  struct DocumentHighlightOptions : public WorkDoneProgressOptions {
    // empty
  };

  /**
   * Registration Options: DocumentHighlightRegistrationOptions defined as
   * follows:
   *
   * export interface DocumentHighlightRegistrationOptions extends
   *   TextDocumentRegistrationOptions, DocumentHighlightOptions {
   * }
   */
  struct DocumentHighlightRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public DocumentHighlightOptions {
    // empty
  };

  /**
   * Request:
   * - method: textDocument/documentHighlight
   * - params: DocumentHighlightParams defined as follows:
   *
   * export interface DocumentHighlightParams extends TextDocumentPositionParams,
   *   WorkDoneProgressParams, PartialResultParams {
   * }
   */
  struct DocumentHighlightParams
    : public TextDocumentPositionParams
    , public WorkDoneProgressParams
    , public PartialResultParams {
    // empty
  };

  /**
   * A document highlight kind.
   *
   * export namespace DocumentHighlightKind {
   *   export const Text = 1;
   *   export const Read = 2;
   *   export const Write = 3;
   * }
   * export type DocumentHighlightKind = 1 | 2 | 3;
   */
  enum class DocumentHighlightKind {

    /**
     * A textual occurrence.
     *
     * export const Text = 1;
     */
    Text = 1,

    /**
     * Read-access of a symbol, like reading a variable.
     *
     * export const Read = 2;
     */
    Read = 2,

    /**
     * Write-access of a symbol, like writing to a variable.
     *
     * export const Write = 3;
     */
    Write = 3,
  };

  extern std::map<DocumentHighlightKind, std::string> DocumentHighlightKindNames;

  auto documentHighlightKindByName(const std::string &name) -> DocumentHighlightKind;

  /**
   * A document highlight is a range inside a text document which deserves
   * special attention. Usually a document highlight is visualized by changing
   * the background color of its range.
   *
   * Response:
   * - result: DocumentHighlight[] | null defined as follows:
   *
   * export interface DocumentHighlight {
   *   range: Range;
   *   kind?: DocumentHighlightKind;
   * }
   *
   * - partial result: DocumentHighlight[]
   * - error: code and message set in case an exception happens during the
   *   document highlight request.
   */
  struct DocumentHighlight {

    /**
     * The range this highlight applies to.
     *
     * range: Range;
     */
    std::unique_ptr<Range> range;

    /**
     * The highlight kind, default is DocumentHighlightKind.Text.
     *
     * kind?: DocumentHighlightKind;
     */
    std::optional<DocumentHighlightKind> kind;
  };

  /**
   * The document links request is sent from the client to the server to request
   * the location of links in a document.
   *
   * Client Capability:
   * - property name (optional): textDocument.documentLink
   * - property type: DocumentLinkClientCapabilities defined as follows:
   *
   * export interface DocumentLinkClientCapabilities {
   *   dynamicRegistration?: boolean;
   *   tooltipSupport?: boolean;
   * }
   */
  struct DocumentLinkClientCapabilities {

    /**
     * Whether document link supports dynamic registration.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;

    /**
     * Whether the client supports the `tooltip` property on `DocumentLink`.
     *
     * tooltipSupport?: boolean;
     *
     * @since 3.15.0
     */
    std::optional<boolean> tooltipSupport;
  };

  /**
   * Server Capability:
   * - property name (optional): documentLinkProvider
   * - property type: DocumentLinkOptions defined as follows:
   *
   * export interface DocumentLinkOptions extends WorkDoneProgressOptions {
   *   resolveProvider?: boolean;
   * }
   */
  struct DocumentLinkOptions : public WorkDoneProgressOptions {

    /**
     * Document links have a resolve provider as well.
     *
     * resolveProvider?: boolean;
     */
    std::optional<boolean> resolveProvider;
  };

  /**
   * Registration Options: DocumentLinkRegistrationOptions defined as follows:
   *
   * export interface DocumentLinkRegistrationOptions extends
   *   TextDocumentRegistrationOptions, DocumentLinkOptions {
   * }
   */
  struct DocumentLinkRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public DocumentLinkOptions {
    // empty
  };

  /**
   * Request:
   * - method: textDocument/documentLink
   * - params: DocumentLinkParams defined as follows:
   *
   * interface DocumentLinkParams extends WorkDoneProgressParams,
   *   PartialResultParams {
   *   textDocument: TextDocumentIdentifier;
   * }
   */
  struct DocumentLinkParams
    : public WorkDoneProgressParams
    , public PartialResultParams {

    /**
     * The document to provide document links for.
     *
     * textDocument: TextDocumentIdentifier;
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
  };

  /**
   * A document link is a range in a text document that links to an internal or
   * external resource, like another text document or a web site.
   *
   * Response:
   * - result: DocumentLink[] | null.
   * - partial result: DocumentLink[]
   * - error: code and message set in case an exception happens during the
   *   document link request.
   *
   * interface DocumentLink {
   *   range: Range;
   *   target?: URI;
   *   tooltip?: string;
   *   data?: LSPAny;
   * }
   */
  struct DocumentLink {

    /**
     * The range this link applies to.
     *
     * range: Range;
     */
    std::unique_ptr<Range> range;

    /**
     * The uri this link points to. If missing a resolve request is sent later.
     *
     * target?: URI;
     */
    std::optional<URI> target;

    /**
     * The tooltip text when you hover over this link.
     *
     * If a tooltip is provided, is will be displayed in a string that includes
     * instructions on how to trigger the link, such as `{0} (ctrl + click)`.
     * The specific instructions vary depending on OS, user settings, and
     * localization.
     *
     * tooltip?: string;
     *
     * @since 3.15.0
     */
    std::optional<string> tooltip;

    /**
     * A data entry field that is preserved on a document link between a
     * DocumentLinkRequest and a DocumentLinkResolveRequest.
     *
     * data?: LSPAny;
     */
    optional_ptr<LSPAny> data;
  };

  // --------------------------------------------------------------------------
  // Document Link Resolve Request
  // --------------------------------------------------------------------------
  // The document link resolve request is sent from the client to the server to
  // resolve the target of a given document link.
  //
  // Request:
  // - method: documentLink/resolve
  // - params: DocumentLink
  //
  // Response:
  // - result: DocumentLink
  // - error: code and message set in case an exception happens during the
  //   document link resolve request.
  // --------------------------------------------------------------------------

  /**
   * The hover request is sent from the client to the server to request hover
   * information at a given text document position.
   *
   * Client Capability:
   * - property name (optional): textDocument.hover
   * - property type: HoverClientCapabilities defined as follows:
   *
   * export interface HoverClientCapabilities {
   *   dynamicRegistration?: boolean;
   *   contentFormat?: MarkupKind[];
   * }
   */
  struct HoverClientCapabilities {

    /**
     * Whether hover supports dynamic registration.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;

    /**
     * Client supports the follow content formats if the content
     * property refers to a `literal of type MarkupContent`.
     * The order describes the preferred format of the client.
     *
     * contentFormat?: MarkupKind[];
     */
    optional_ptr_vector<MarkupKind> contentFormat;
  };

  /**
   * Server Capability:
   * - property name (optional): hoverProvider
   * - property type: boolean | HoverOptions where HoverOptions is defined as
   *   follows:
   *
   * export interface HoverOptions extends WorkDoneProgressOptions {
   * }
   */
  struct HoverOptions : public WorkDoneProgressOptions {
    // empty
  };

  /**
   * Registration Options: HoverRegistrationOptions defined as follows:
   *
   * export interface HoverRegistrationOptions
   *   extends TextDocumentRegistrationOptions, HoverOptions {
   * }
   */
  struct HoverRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public HoverOptions {
    // empty
  };

  /**
   * Request:
   * - method: textDocument/hover
   * - params: HoverParams defined as follows:
   *
   * export interface HoverParams extends TextDocumentPositionParams,
   *   WorkDoneProgressParams {
   * }
   */
  struct HoverParams
    : public TextDocumentPositionParams
    , public WorkDoneProgressParams {
    // empty
  };

  struct MarkedStringWithLanguage {

    /**
     * language: string;
     */
    string language;

    /**
     * value: string;
     */
    string value;
  };

  enum class MarkedStringType {
    LITERAL,
    WITH_LANGUAGE,
  };

  /**
   * MarkedString can be used to render human readable text. It is either a
   * markdown string or a code-block that provides a language and a code snippet.
   * The language identifier is semantically equal to the optional language
   * identifier in fenced code blocks in GitHub issues.
   *
   * The pair of a language and a value is an equivalent to markdown:
   * ```${language}
   * ${value}
   * ```
   *
   * Note that markdown strings will be sanitized - that means html will be
   * escaped.
   *
   * type MarkedString = string | { language: string; value: string };
   *
   * @deprecated use MarkupContent instead.
   */
  struct MarkedString {
    MarkedStringType type;
    std::variant<
      string,
      std::unique_ptr<MarkedStringWithLanguage>
    > value;
  };

  enum class HoverContentsType {
    MARKED_STRING,
    MARKED_STRING_ARRAY,
    MARKUP_CONTENT,
  };

  struct HoverContents {
    HoverContentsType type;
    std::variant<
      std::unique_ptr<MarkedString>,
      ptr_vector<MarkedString>,
      std::unique_ptr<MarkupContent>
    > value;
  };

  /**
   * The result of a hover request.
   *
   * Response:
   * - result: Hover | null defined as follows:
   *
   * export interface Hover {
   *   contents: MarkedString | MarkedString[] | MarkupContent;
   *   range?: Range;
   * }
   *
   * - error: code and message set in case an exception happens during the hover
   *   request.
   */
  struct Hover {

    /**
     * The hover's content
     *
     * contents: MarkedString | MarkedString[] | MarkupContent;
     */
    std::unique_ptr<HoverContents> contents;

    /**
     * An optional range is a range inside a text document
     * that is used to visualize a hover, e.g. by changing the background color.
     *
     * range?: Range;
     */
    optional_ptr<Range> range;
  };

  /**
   * The code lens request is sent from the client to the server to compute code
   * lenses for a given text document.
   *
   * Client Capability:
   * - property name (optional): textDocument.codeLens
   * - property type: CodeLensClientCapabilities defined as follows:
   *
   * export interface CodeLensClientCapabilities {
   *   dynamicRegistration?: boolean;
   * }
   */
  struct CodeLensClientCapabilities {

    /**
     * Whether code lens supports dynamic registration.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;
  };

  /**
   * Server Capability:
   * - property name (optional): codeLensProvider
   * - property type: CodeLensOptions defined as follows:
   *
   * export interface CodeLensOptions extends WorkDoneProgressOptions {
   *   resolveProvider?: boolean;
   * }
   */
  struct CodeLensOptions : public WorkDoneProgressOptions {

    /**
     * Code lens has a resolve provider as well.
     *
     * resolveProvider?: boolean;
     */
    std::optional<boolean> resolveProvider;
  };

  /**
   * Registration Options: CodeLensRegistrationOptions defined as follows:
   *
   * export interface CodeLensRegistrationOptions extends
   *   TextDocumentRegistrationOptions, CodeLensOptions {
   * }
   */
  struct CodeLensRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public CodeLensOptions {
    // empty
  };

  /**
   * Request:
   * - method: textDocument/codeLens
   * - params: CodeLensParams defined as follows:
   *
   * interface CodeLensParams extends WorkDoneProgressParams, PartialResultParams {
   *   textDocument: TextDocumentIdentifier;
   * }
   */
  struct CodeLensParams
    : public WorkDoneProgressParams
    , public PartialResultParams {

    /**
     * The document to request code lens for.
     *
     * textDocument: TextDocumentIdentifier;
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
  };

  /**
   * A code lens represents a command that should be shown along with
   * source text, like the number of references, a way to run tests, etc.
   *
   * A code lens is _unresolved_ when no command is associated to it. For
   * performance reasons the creation of a code lens and resolving should be done
   * in two stages.
   *
   * Response:
   * - result: CodeLens[] | null defined as follows:
   *
   * interface CodeLens {
   *   range: Range;
   *   command?: Command;
   *   data?: LSPAny;
   * }
   *
   * - partial result: CodeLens[]
   * - error: code and message set in case an exception happens during the code
   *   lens request.
   */
  struct CodeLens {

    /**
     * The range in which this code lens is valid. Should only span a single
     * line.
     *
     * range: Range;
     */
    std::unique_ptr<Range> range;

    /**
     * The command this code lens represents.
     *
     * command?: Command;
     */
    optional_ptr<Command> command;

    /**
     * A data entry field that is preserved on a code lens item between
     * a code lens and a code lens resolve request.
     *
     * data?: LSPAny;
     */
    optional_ptr<LSPAny> data;
  };

  // --------------------------------------------------------------------------
  // Code Lens Resolve Request
  // --------------------------------------------------------------------------
  // The code lens resolve request is sent from the client to the server to
  // resolve the command for a given code lens item.
  //
  // Request:
  // - method: codeLens/resolve
  // - params: CodeLens
  //
  // Response:
  // - result: CodeLens
  // - error: code and message set in case an exception happens during the code
  //   lens resolve request.
  // --------------------------------------------------------------------------

  /**
   * The workspace/codeLens/refresh request is sent from the server to the
   * client. Servers can use it to ask clients to refresh the code lenses
   * currently shown in editors. As a result the client should ask the server to
   * recompute the code lenses for these editors. This is useful if a server
   * detects a configuration change which requires a re-calculation of all code
   * lenses. Note that the client still has the freedom to delay the
   * re-calculation of the code lenses if for example an editor is currently not
   * visible.
   *
   * Client Capability:
   * - property name (optional): workspace.codeLens
   * - property type: CodeLensWorkspaceClientCapabilities defined as follows:
   *
   * export interface CodeLensWorkspaceClientCapabilities {
   *   refreshSupport?: boolean;
   * }
   *
   * Request:
   * - method: workspace/codeLens/refresh
   * - params: none
   *
   * Response:
   * - result: void
   * - error: code and message set in case an exception happens during the
   *   ‘workspace/codeLens/refresh’ request
   */
  struct CodeLensWorkspaceClientCapabilities {

    /**
     * Whether the client implementation supports a refresh request sent from the
     * server to the client.
     *
     * Note that this event is global and will force the client to refresh all
     * code lenses currently shown. It should be used with absolute care and is
     * useful for situation where a server for example detect a project wide
     * change that requires such a calculation.
     *
     * refreshSupport?: boolean;
     */
    std::optional<boolean> refreshSupport;
  };

  /**
   * A set of predefined range kinds.
   *
   * export namespace FoldingRangeKind {
   *   export const Comment = 'comment';
   *   export const Imports = 'imports';
   *   export const Region = 'region';
   * }
   * export type FoldingRangeKind = string;
   */
  enum class FoldingRangeKind {

    /**
     * Folding range for a comment
     *
     * export const Comment = 'comment';
     */
    Comment,

    /**
     * Folding range for imports or includes
     *
     * export const Imports = 'imports';
     */
    Imports,

    /**
     * Folding range for a region (e.g. `#region`)
     *
     * export const Region = 'region';
     */
    Region,
  };

  extern std::map<FoldingRangeKind, std::string> FoldingRangeKindNames;

  auto foldingRangeKindByName(const std::string &name) -> FoldingRangeKind;

  extern std::map<FoldingRangeKind, std::string> FoldingRangeKindValues;

  auto foldingRangeKindByValue(const std::string &value) -> FoldingRangeKind;

  /**
   * Represents a folding range. To be valid, start and end line must be bigger
   * than zero and smaller than the number of lines in the document. Clients are
   * free to ignore invalid ranges.
   *
   * Response:
   * - result: FoldingRange[] | null defined as follows:
   *
   * export interface FoldingRange {
   *   startLine: uinteger;
   *   startCharacter?: uinteger;
   *   endLine: uinteger;
   *   endCharacter?: uinteger;
   *   kind?: FoldingRangeKind;
   *   collapsedText?: string;
   * }
   *
   * - partial result: FoldingRange[]
   * - error: code and message set in case an exception happens during the
   *   ‘textDocument/foldingRange’ request
   */
  struct FoldingRange {

    /**
     * The zero-based start line of the range to fold. The folded area starts
     * after the line's last character. To be valid, the end must be zero or
     * larger and smaller than the number of lines in the document.
     *
     * startLine: uinteger;
     */
    uinteger startLine;

    /**
     * The zero-based character offset from where the folded range starts. If
     * not defined, defaults to the length of the start line.
     *
     * startCharacter?: uinteger;
     */
    std::optional<uinteger> startCharacter;

    /**
     * The zero-based end line of the range to fold. The folded area ends with
     * the line's last character. To be valid, the end must be zero or larger
     * and smaller than the number of lines in the document.
     *
     * endLine: uinteger;
     */
    uinteger endLine;

    /**
     * The zero-based character offset before the folded range ends. If not
     * defined, defaults to the length of the end line.
     *
     * endCharacter?: uinteger;
     */
    std::optional<uinteger> endCharacter;

    /**
     * Describes the kind of the folding range such as `comment` or `region`.
     * The kind is used to categorize folding ranges and used by commands like
     * 'Fold all comments'. See [FoldingRangeKind](#FoldingRangeKind) for an
     * enumeration of standardized kinds.
     *
     * kind?: FoldingRangeKind;
     */
    std::optional<FoldingRangeKind> kind;

    /**
     * The text that the client should show when the specified range is
     * collapsed. If not defined or not supported by the client, a default
     * will be chosen by the client.
     *
     * collapsedText?: string;
     *
     * @since 3.17.0 - proposed
     */
    std::optional<string> collapsedText;
  };

  /**
   * The selection range request is sent from the client to the server to return
   * suggested selection ranges at an array of given positions. A selection
   * range is a range around the cursor position which the user might be
   * interested in selecting.
   *
   * A selection range in the return array is for the position in the provided
   * parameters at the same index. Therefore positions[i] must be contained in
   * result[i].range. To allow for results where some positions have selection
   * ranges and others do not, result[i].range is allowed to be the empty range
   * at positions[i].
   *
   * Typically, but not necessary, selection ranges correspond to the nodes of
   * the syntax tree.
   *
   * Client Capability:
   * - property name (optional): textDocument.selectionRange
   * - property type: SelectionRangeClientCapabilities defined as follows:
   *
   * export interface SelectionRangeClientCapabilities {
   *   dynamicRegistration?: boolean;
   * }
   */
  struct SelectionRangeClientCapabilities {

    /**
     * Whether implementation supports dynamic registration for selection range
     * providers. If this is set to `true` the client supports the new
     * `SelectionRangeRegistrationOptions` return value for the corresponding
     * server capability as well.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;
  };

  /**
   * Server Capability:
   * - property name (optional): selectionRangeProvider
   * - property type: boolean | SelectionRangeOptions |
   *   SelectionRangeRegistrationOptions where SelectionRangeOptions is defined
   *   as follows:
   *
   * export interface SelectionRangeOptions extends WorkDoneProgressOptions {
   * }
   */
  struct SelectionRangeOptions : public WorkDoneProgressOptions {
    // empty
  };

  /**
   * Registration Options: SelectionRangeRegistrationOptions defined as follows:
   *
   * export interface SelectionRangeRegistrationOptions extends
   *   SelectionRangeOptions, TextDocumentRegistrationOptions,
   *   StaticRegistrationOptions {
   * }
   */
  struct SelectionRangeRegistrationOptions
    : public SelectionRangeOptions
    , public TextDocumentRegistrationOptions
    , public StaticRegistrationOptions {
    // empty
  };

  /**
   * Request:
   * - method: textDocument/selectionRange
   * - params: SelectionRangeParams defined as follows:
   *
   * export interface SelectionRangeParams extends WorkDoneProgressParams,
   *   PartialResultParams {
   *   textDocument: TextDocumentIdentifier;
   *   positions: Position[];
   * }
   */
  struct SelectionRangeParams
    : public WorkDoneProgressParams
    , public PartialResultParams {

    /**
     * The text document.
     *
     * textDocument: TextDocumentIdentifier;
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;

    /**
     * The positions inside the text document.
     *
     * positions: Position[];
     */
    ptr_vector<Position> positions;
  };

  /**
   * Response:
   * - result: SelectionRange[] | null defined as follows:
   *
   * export interface SelectionRange {
   *   range: Range;
   *   parent?: SelectionRange;
   * }
   *
   * - partial result: SelectionRange[]
   * - error: code and message set in case an exception happens during the
   *   ‘textDocument/selectionRange’ request
   */
  struct SelectionRange {

    /**
     * The [range](#Range) of this selection range.
     *
     * range: Range;
     */
    std::unique_ptr<Range> range;

    /**
     * The parent selection range containing this range. Therefore
     * `parent.range` must contain `this.range`.
     *
     * parent?: SelectionRange;
     */
    optional_ptr<SelectionRange> parent;
  };

  struct FoldingRangeCollapsedText {

    /**
     * If set, the client signals that it supports setting collapsedText on
     * folding ranges to display custom labels instead of the default text.
     *
     * collapsedText?: boolean;
     *
     * @since 3.17.0
     */
    std::optional<boolean> collapsedText;
  };

  /**
   * The folding range request is sent from the client to the server to return
   * all folding ranges found in a given text document.
   *
   * Client Capability:
   * - property name (optional): textDocument.foldingRange
   * - property type: FoldingRangeClientCapabilities defined as follows:
   *
   * export interface FoldingRangeClientCapabilities {
   *   dynamicRegistration?: boolean;
   *   rangeLimit?: uinteger;
   *   lineFoldingOnly?: boolean;
   *   foldingRangeKind? : {
   *     valueSet?: FoldingRangeKind[];
   *   };
   *   foldingRange?: {
   *     collapsedText?: boolean;
   *   };
   * }
   */
  struct FoldingRangeClientCapabilities {

    /**
     * Whether implementation supports dynamic registration for folding range
     * providers. If this is set to `true` the client supports the new
     * `FoldingRangeRegistrationOptions` return value for the corresponding
     * server capability as well.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;

    /**
     * The maximum number of folding ranges that the client prefers to receive
     * per document. The value serves as a hint, servers are free to follow the
     * limit.
     *
     * rangeLimit?: uinteger;
     */
    std::optional<uinteger> rangeLimit;

    /**
     * If set, the client signals that it only supports folding complete lines.
     * If set, client will ignore specified `startCharacter` and `endCharacter`
     * properties in a FoldingRange.
     *
     * lineFoldingOnly?: boolean;
     */
    std::optional<boolean> lineFoldingOnly;

    /**
     * Specific options for the folding range kind.
     *
     * foldingRangeKind? : {
     *   // The folding range kind values the client supports. When this
     *   // property exists the client also guarantees that it will handle
     *   // values outside its set gracefully and falls back to a default value
     *   // when unknown.
     *   valueSet?: FoldingRangeKind[];
     * };
     *
     * @since 3.17.0
     */
    optional_ptr<OptionalValueSet<FoldingRangeKind>> foldingRangeKind;

    /**
     * Specific options for the folding range.
     *
     * foldingRange?: {
     *   collapsedText?: boolean;
     * };
     *
     * @since 3.17.0
     */
    optional_ptr<FoldingRangeCollapsedText> foldingRange;
  };

  /**
   * Server Capability:
   * - property name (optional): foldingRangeProvider
   * - property type: boolean | FoldingRangeOptions |
   *   FoldingRangeRegistrationOptions where FoldingRangeOptions is defined as
   *   follows:
   *
   * export interface FoldingRangeOptions extends WorkDoneProgressOptions {
   * }
   */
  struct FoldingRangeOptions : public WorkDoneProgressOptions {
    // empty
  };

  /**
   * Registration Options: FoldingRangeRegistrationOptions defined as follows:
   *
   * export interface FoldingRangeRegistrationOptions extends
   *   TextDocumentRegistrationOptions, FoldingRangeOptions,
   *   StaticRegistrationOptions {
   * }
   */
  struct FoldingRangeRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public FoldingRangeOptions
    , public StaticRegistrationOptions {
    // empty
  };

  /**
   * Request:
   * - method: textDocument/foldingRange
   * - params: FoldingRangeParams defined as follows:
   *
   * export interface FoldingRangeParams extends WorkDoneProgressParams,
   *   PartialResultParams {
   *   textDocument: TextDocumentIdentifier;
   * }
   */
  struct FoldingRangeParams
    : public WorkDoneProgressParams
    , public PartialResultParams {

    /**
     * The text document.
     *
     * textDocument: TextDocumentIdentifier;
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
  };

  /**
   * The document symbol request is sent from the client to the server. The
   * returned result is either:
   * - SymbolInformation[] which is a flat list of all symbols found in a given
   *   text document. Then neither the symbol’s location range nor the symbol’s
   *   container name should be used to infer a hierarchy.
   * - DocumentSymbol[] which is a hierarchy of symbols found in a given text
   *   document.
   *
   * Servers should whenever possible return DocumentSymbol since it is the
   * richer data structure.
   *
   * Client Capability:
   * - property name (optional): textDocument.documentSymbol
   * - property type: DocumentSymbolClientCapabilities defined as follows:
   *
   * export interface DocumentSymbolClientCapabilities {
   *   dynamicRegistration?: boolean;
   *   symbolKind?: {
   *     valueSet?: SymbolKind[];
   *   };
   *   hierarchicalDocumentSymbolSupport?: boolean;
   *   tagSupport?: {
   *     valueSet: SymbolTag[];
   *   };
   *   labelSupport?: boolean;
   * }
   */
  struct DocumentSymbolClientCapabilities {

    /**
     * Whether document symbol supports dynamic registration.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;

    /**
     * Specific capabilities for the `SymbolKind` in the
     * `textDocument/documentSymbol` request.
     *
     * symbolKind?: {
     *   // The symbol kind values the client supports. When this property
     *   // exists the client also guarantees that it will handle values outside
     *   // its set gracefully and falls back to a default value when unknown.
     *   //
     *   // If this property is not present the client only supports the symbol
     *   // kinds from `File` to `Array` as defined in the initial version of
     *   // the protocol.
     *   valueSet?: SymbolKind[];
     * };
     */
    optional_ptr<OptionalValueSet<SymbolKind>> symbolKind;

    /**
     * The client supports hierarchical document symbols.
     *
     * hierarchicalDocumentSymbolSupport?: boolean;
     */
    std::optional<boolean> hierarchicalDocumentSymbolSupport;

    /**
     * The client supports tags on `SymbolInformation`. Tags are supported on
     * `DocumentSymbol` if `hierarchicalDocumentSymbolSupport` is set to true.
     * Clients supporting tags have to handle unknown tags gracefully.
     *
     * tagSupport?: {
     *   // The tags supported by the client.
     *   valueSet: SymbolTag[];
     * };
     *
     * @since 3.16.0
     */
    optional_ptr<ValueSet<SymbolTag>> tagSupport;

    /**
     * The client supports an additional label presented in the UI when
     * registering a document symbol provider.
     *
     * labelSupport?: boolean;
     *
     * @since 3.16.0
     */
    std::optional<boolean> labelSupport;
  };

  /**
   * Server Capability:
   * - property name (optional): documentSymbolProvider
   * - property type: boolean | DocumentSymbolOptions where
   *   DocumentSymbolOptions is defined as follows:
   *
   * export interface DocumentSymbolOptions extends WorkDoneProgressOptions {
   *   label?: string;
   * }
   */
  struct DocumentSymbolOptions : public WorkDoneProgressOptions {

    /**
     * A human-readable string that is shown when multiple outlines trees
     * are shown for the same document.
     *
     * label?: string;
     *
     * @since 3.16.0
     */
    std::optional<string> label;
  };

  /**
   * Registration Options: DocumentSymbolRegistrationOptions defined as follows:
   *
   * export interface DocumentSymbolRegistrationOptions extends
   *   TextDocumentRegistrationOptions, DocumentSymbolOptions {
   * }
   */
  struct DocumentSymbolRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public DocumentSymbolOptions {
    // empty
  };

  /**
   * Request:
   * - method: textDocument/documentSymbol
   * - params: DocumentSymbolParams defined as follows:
   *
   * export interface DocumentSymbolParams extends WorkDoneProgressParams,
   *   PartialResultParams {
   *   textDocument: TextDocumentIdentifier;
   * }
   *
   * Response:
   * - result: DocumentSymbol[] | SymbolInformation[] | null
   * - partial result: DocumentSymbol[] | SymbolInformation[]. DocumentSymbol[]
   *   and SymbolInformation[] can not be mixed. That means the first chunk
   *   defines the type of all the other chunks.
   * - error: code and message set in case an exception happens during the
   *   document symbol request.
   */
  struct DocumentSymbolParams
    : public WorkDoneProgressParams
    , public PartialResultParams {

    /**
     * The text document.
     *
     * textDocument: TextDocumentIdentifier;
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
  };

  /**
   * The request is sent from the client to the server to resolve semantic
   * tokens for a given file. Semantic tokens are used to add additional color
   * information to a file that depends on language specific symbol information.
   * A semantic token request usually produces a large result. The protocol
   * therefore supports encoding tokens with numbers. In addition optional
   * support for deltas is available.
   *
   * General Concepts
   *
   * Tokens are represented using one token type combined with n token
   * modifiers. A token type is something like class or function and token
   * modifiers are like static or async. The protocol defines a set of token
   * types and modifiers but clients are allowed to extend these and announce
   * the values they support in the corresponding client capability. The
   * predefined values are:
   *
   * export enum SemanticTokenTypes {
   *   namespace = 'namespace',
   *   type = 'type',
   *   class = 'class',
   *   enum = 'enum',
   *   interface = 'interface',
   *   struct = 'struct',
   *   typeParameter = 'typeParameter',
   *   parameter = 'parameter',
   *   variable = 'variable',
   *   property = 'property',
   *   enumMember = 'enumMember',
   *   event = 'event',
   *   function = 'function',
   *   method = 'method',
   *   macro = 'macro',
   *   keyword = 'keyword',
   *   modifier = 'modifier',
   *   comment = 'comment',
   *   string = 'string',
   *   number = 'number',
   *   regexp = 'regexp',
   *   operator = 'operator',
   *   decorator = 'decorator'
   * }
   */
  enum class SemanticTokenTypes {
    Namespace,
    Type,
    Class,
    Enum,
    Interface,
    Struct,
    TypeParameter,
    Parameter,
    Variable,
    Property,
    EnumMember,
    Event,
    Function,
    Method,
    Macro,
    Keyword,
    Modifier,
    Comment,
    String,
    Number,
    Regexp,
    Operator,
    Decorator,
  };

  extern std::map<SemanticTokenTypes, std::string> SemanticTokenTypeNames;

  auto semanticTokenTypesByName(const std::string &name) -> SemanticTokenTypes;

  extern std::map<SemanticTokenTypes, std::string> SemanticTokenTypeValues;

  auto semanticTokenTypesByValue(const std::string &value) -> SemanticTokenTypes;

  /**
   * export enum SemanticTokenModifiers {
   *   declaration = 'declaration',
   *   definition = 'definition',
   *   readonly = 'readonly',
   *   static = 'static',
   *   deprecated = 'deprecated',
   *   abstract = 'abstract',
   *   async = 'async',
   *   modification = 'modification',
   *   documentation = 'documentation',
   *   defaultLibrary = 'defaultLibrary'
   * }
   */
  enum class SemanticTokenModifiers {
    Declaration,
    Definition,
    Readonly,
    Static,
    Deprecated,
    Abstract,
    Async,
    Modification,
    Documentation,
    DefaultLibrary,
  };

  extern std::map<SemanticTokenModifiers, std::string> SemanticTokenModifierNames;

  auto semanticTokenModifiersByName(const std::string &name) -> SemanticTokenModifiers;

  extern std::map<SemanticTokenModifiers, std::string> SemanticTokenModifierValues;

  auto semanticTokenModifiersByValue(const std::string &value) -> SemanticTokenModifiers;

  /**
   * The protocol defines an additional token format capability to allow future
   * extensions of the format. The only format that is currently specified is
   * relative expressing that the tokens are described using relative positions.
   *
   * export namespace TokenFormat {
   *   export const Relative: 'relative' = 'relative';
   * }
   * export type TokenFormat = 'relative';
   */
  enum class TokenFormat {
    Relative,
  };

  extern std::map<TokenFormat, std::string> TokenFormatNames;

  auto tokenFormatByName(const std::string &name) -> TokenFormat;

  extern std::map<TokenFormat, std::string> TokenFormatValues;

  auto tokenFormatByValue(const std::string &value) -> TokenFormat;

  /**
   * On the capability level types and modifiers are defined using strings.
   * However the real encoding happens using numbers. The server therefore needs
   * to let the client know which numbers it is using for which types and
   * modifiers. They do so using a legend, which is defined as follows:
   *
   * export interface SemanticTokensLegend {
   *   tokenTypes: string[];
   *   tokenModifiers: string[];
   * }
   */
  struct SemanticTokensLegend {

    /**
     * The token types a server uses.
     *
     * tokenTypes: string[];
     */
    ptr_vector<string> tokenTypes;

    /**
     * The token modifiers a server uses.
     *
     * tokenModifiers: string[];
     */
    ptr_vector<string> tokenModifiers;
  };

  struct RangeCapabilities {
    // empty
  };

  enum class OptionalRangeCapabilitiesType {
    BOOLEAN,
    RANGE_CAPABILITIES,
  };

  struct OptionalRangeCapabilities {
    OptionalRangeCapabilitiesType type;
    std::variant<
      boolean,
      std::unique_ptr<RangeCapabilities>
    > value;
  };

  struct FullCapabilities {
    std::optional<boolean> delta;
  };

  enum class OptionalFullCapabilitiesType {
    BOOLEAN,
    FULL_CAPABILITIES,
  };

  struct OptionalFullCapabilities {
    OptionalFullCapabilitiesType type;
    std::variant<
      boolean,
      std::unique_ptr<FullCapabilities>
    > value;
  };

  struct SemanticTokensClientRequestCapabilities {

    /**
     * The client will send the `textDocument/semanticTokens/range` request
     * if the server provides a corresponding handler.
     *
     * range?: boolean | {
     * };
     */
    optional_ptr<OptionalRangeCapabilities> range;

    /**
     * The client will send the `textDocument/semanticTokens/full` request if
     * the server provides a corresponding handler.
     *
     * full?: boolean | {
     *   delta?: boolean;
     * };
     */
    optional_ptr<OptionalFullCapabilities> full;
  };

  /**
   * The following client capabilities are defined for semantic token requests
   * sent from the client to the server:
   * - property name (optional): textDocument.semanticTokens
   * - property type: SemanticTokensClientCapabilities defined as follows:
   *
   * interface SemanticTokensClientCapabilities {
   *   dynamicRegistration?: boolean;
   *   requests: {
   *     range?: boolean | {
   *     };
   *     full?: boolean | {
   *       delta?: boolean;
   *     };
   *   };
   *   tokenTypes: string[];
   *   tokenModifiers: string[];
   *   formats: TokenFormat[];
   *   overlappingTokenSupport?: boolean;
   *   multilineTokenSupport?: boolean;
   *   serverCancelSupport?: boolean;
   *   augmentsSyntaxTokens?: boolean;
   * }
   */
  struct SemanticTokensClientCapabilities {

    /**
     * Whether implementation supports dynamic registration. If this is set to
     * `true` the client supports the new `(TextDocumentRegistrationOptions &
     * StaticRegistrationOptions)` return value for the corresponding server
     * capability as well.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;

    /**
     * Which requests the client supports and might send to the server depending
     * on the server's capability. Please note that clients might not show
     * semantic tokens or degrade some of the user experience if a range or full
     * request is advertised by the client but not provided by the server. If
     * for example the client capability `requests.full` and `request.range` are
     * both set to true but the server only provides a range provider the client
     * might not render a minimap correctly or might even decide to not show any
     * semantic tokens at all.
     *
     * requests: {
     *   range?: boolean | {
     *   };
     *   full?: boolean | {
     *     delta?: boolean;
     *   };
     * };
     */
    std::unique_ptr<SemanticTokensClientRequestCapabilities> requests;

    /**
     * The token types that the client supports.
     *
     * tokenTypes: string[];
     */
    ptr_vector<string> tokenTypes;

    /**
     * The token modifiers that the client supports.
     *
     * tokenModifiers: string[];
     */
    std::vector<string> tokenModifiers;

    /**
     * The formats the clients supports.
     *
     * formats: TokenFormat[];
     */
    std::vector<TokenFormat> formats;

    /**
     * Whether the client supports tokens that can overlap each other.
     *
     * overlappingTokenSupport?: boolean;
     */
    std::optional<boolean> overlappingTokenSupport;

    /**
     * Whether the client supports tokens that can span multiple lines.
     *
     * multilineTokenSupport?: boolean;
     */
    std::optional<boolean> multilineTokenSupport;

    /**
     * Whether the client allows the server to actively cancel a
     * semantic token request, e.g. supports returning
     * ErrorCodes.ServerCancelled. If a server does the client
     * needs to retrigger the request.
     *
     * serverCancelSupport?: boolean;
     *
     * @since 3.17.0
     */
    std::optional<boolean> serverCancelSupport;

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
     * augmentsSyntaxTokens?: boolean;
     *
     * @since 3.17.0
     */
    std::optional<boolean> augmentsSyntaxTokens;
  };

  /**
   * The following server capabilities are defined for semantic tokens:
   * - property name (optional): semanticTokensProvider
   * - property type: SemanticTokensOptions | SemanticTokensRegistrationOptions
   *   where SemanticTokensOptions is defined as follows:
   *
   * export interface SemanticTokensOptions extends WorkDoneProgressOptions {
   *   legend: SemanticTokensLegend;
   *   range?: boolean | {
   *   };
   *   full?: boolean | {
   *     delta?: boolean;
   *   };
   * }
   */
  struct SemanticTokensOptions : public WorkDoneProgressOptions {

    /**
     * The legend used by the server
     *
     * legend: SemanticTokensLegend;
     */
    std::unique_ptr<SemanticTokensLegend> legend;

    /**
     * Server supports providing semantic tokens for a specific range of a
     * document.
     *
     * range?: boolean | {
     * };
     */
    optional_ptr<OptionalRangeCapabilities> range;

    /**
     * Server supports providing semantic tokens for a full document.
     *
     * full?: boolean | {
     *   delta?: boolean;
     * };
     */
    optional_ptr<OptionalFullCapabilities> full;
  };

  /**
   * Registration Options: SemanticTokensRegistrationOptions defined as follows:
   *
   * export interface SemanticTokensRegistrationOptions extends
   *   TextDocumentRegistrationOptions, SemanticTokensOptions,
   *   StaticRegistrationOptions {
   * }
   */
  struct SemanticTokensRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public SemanticTokensOptions
    , public StaticRegistrationOptions {
    // empty
  };

  /**
   * Request:
   * - method: textDocument/semanticTokens/full
   * - params: SemanticTokensParams defined as follows:
   *
   * export interface SemanticTokensParams extends WorkDoneProgressParams,
   *   PartialResultParams {
   *   textDocument: TextDocumentIdentifier;
   * }
   */
  struct SemanticTokensParams
    : public WorkDoneProgressParams
    , public PartialResultParams {

    /**
     * The text document.
     *
     * textDocument: TextDocumentIdentifier;
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
  };

  /**
   * Response:
   * - result: SemanticTokens | null where SemanticTokens is defined as follows:
   *
   * export interface SemanticTokens {
   *   resultId?: string;
   *   data: uinteger[];
   * }
   *
   * - partial result: SemanticTokensPartialResult defines as follows:
   *
   * export interface SemanticTokensPartialResult {
   *   data: uinteger[];
   * }
   *
   * - error: code and message set in case an exception happens during the
   *   ‘textDocument/semanticTokens/full’ request
   */
  struct SemanticTokens {

    /**
     * An optional result id. If provided and clients support delta updating
     * the client will include the result id in the next semantic token request.
     * A server can then instead of computing all semantic tokens again simply
     * send a delta.
     *
     * resultId?: string;
     */
    std::optional<string> resultId;

    /**
     * The actual tokens.
     *
     * data: uinteger[];
     */
    std::vector<uinteger> data;
  };

  /**
   * export interface SemanticTokensPartialResult {
   *   data: uinteger[];
   * }
   */
  struct SemanticTokensPartialResult {

    /**
     * data: uinteger[];
     */
    std::vector<uinteger> data;
  };

  /**
   * Request:
   * - method: textDocument/semanticTokens/full/delta
   * - params: SemanticTokensDeltaParams defined as follows:
   *
   * export interface SemanticTokensDeltaParams extends WorkDoneProgressParams,
   *   PartialResultParams {
   *   textDocument: TextDocumentIdentifier;
   *   previousResultId: string;
   * }
   */
  struct SemanticTokensDeltaParams
    : public WorkDoneProgressParams
    , public PartialResultParams {

    /**
     * The text document.
     *
     * textDocument: TextDocumentIdentifier;
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;

    /**
     * The result id of a previous response. The result Id can either point to
     * a full response or a delta response depending on what was received last.
     *
     * previousResultId: string;
     */
    string previousResultId;
  };

  /**
   * Response:
   * - result: SemanticTokens | SemanticTokensDelta | null where
   *   SemanticTokensDelta is defined as follows:
   *
   *  export interface SemanticTokensDelta {
   *    readonly resultId?: string;
   *    edits: SemanticTokensEdit[];
   *  }
   *
   * - partial result: SemanticTokensDeltaPartialResult defines as follows:
   *
   * export interface SemanticTokensDeltaPartialResult {
   *   edits: SemanticTokensEdit[];
   * }
   *
   * - error: code and message set in case an exception happens during the
   *   ‘textDocument/semanticTokens/full/delta’ request
   */
  struct SemanticTokensEdit {

    /**
     * The start offset of the edit.
     *
     * start: uinteger;
     */
    uinteger start;

    /**
     * The count of elements to remove.
     *
     * deleteCount: uinteger;
     */
    uinteger deleteCount;

    /**
     * The elements to insert.
     *
     * data?: uinteger[];
     */
    optional_vector<uinteger> data;
  };

  struct SemanticTokensDelta {

    /**
     * readonly resultId?: string;
     */
    std::optional<string> resultId;

    /**
     * The semantic token edits to transform a previous result into a new
     * result.
     *
     * edits: SemanticTokensEdit[];
     */
    ptr_vector<SemanticTokensEdit> edits;
  };

  struct SemanticTokensDeltaPartialResult {

    /**
     * edits: SemanticTokensEdit[];
     */
    ptr_vector<SemanticTokensEdit> edits;
  };

  /**
   * There are two uses cases where it can be beneficial to only compute
   * semantic tokens for a visible range:
   * - for faster rendering of the tokens in the user interface when a user
   *   opens a file. In this use cases servers should also implement the
   *   textDocument/semanticTokens/full request as well to allow for flicker
   *   free scrolling and semantic coloring of a minimap.
   * - if computing semantic tokens for a full document is too expensive servers
   *   can only provide a range call. In this case the client might not render a
   *   minimap correctly or might even decide to not show any semantic tokens at
   *   all.
   *
   * A server is allowed to compute the semantic tokens for a broader range than
   * requested by the client. However if the server does the semantic tokens for
   * the broader range must be complete and correct. If a token at the beginning
   * or end only partially overlaps with the requested range the server should
   * include those tokens in the response.
   *
   * Request:
   * - method: textDocument/semanticTokens/range
   * - params: SemanticTokensRangeParams defined as follows:
   *
   * export interface SemanticTokensRangeParams extends WorkDoneProgressParams,
   *   PartialResultParams {
   *   textDocument: TextDocumentIdentifier;
   *   range: Range;
   * }
   *
   * Response:
   * - result: SemanticTokens | null
   * - partial result: SemanticTokensPartialResult
   * - error: code and message set in case an exception happens during the
   *   ‘textDocument/semanticTokens/range’ request
   */
  struct SemanticTokensRangeParams
    : public WorkDoneProgressParams
    , public PartialResultParams {

    /**
     * The text document.
     *
     * textDocument: TextDocumentIdentifier;
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;

    /**
     * The range the semantic tokens are requested for.
     *
     * range: Range;
     */
    std::unique_ptr<Range> range;
  };

  /**
   * The workspace/semanticTokens/refresh request is sent from the server to the
   * client. Servers can use it to ask clients to refresh the editors for which
   * this server provides semantic tokens. As a result the client should ask the
   * server to recompute the semantic tokens for these editors. This is useful
   * if a server detects a project wide configuration change which requires a
   * re-calculation of all semantic tokens. Note that the client still has the
   * freedom to delay the re-calculation of the semantic tokens if for example
   * an editor is currently not visible.
   *
   * Client Capability:
   * - property name (optional): workspace.semanticTokens
   * - property type: SemanticTokensWorkspaceClientCapabilities defined as
   *   follows:
   *
   * export interface SemanticTokensWorkspaceClientCapabilities {
   *   refreshSupport?: boolean;
   * }
   *
   * Request:
   * - method: workspace/semanticTokens/refresh
   * - params: none
   *
   * Response:
   * - result: void
   * - error: code and message set in case an exception happens during the
   *   ‘workspace/semanticTokens/refresh’ request
   */
  struct SemanticTokensWorkspaceClientCapabilities {

    /**
     * Whether the client implementation supports a refresh request sent from
     * the server to the client.
     *
     * Note that this event is global and will force the client to refresh all
     * semantic tokens currently shown. It should be used with absolute care
     * and is useful for situation where a server for example detect a project
     * wide change that requires such a calculation.
     *
     * refreshSupport?: boolean;
     */
    std::optional<boolean> refreshSupport;
  };

  struct ResolveSupportCapabilities {

    /**
     * The properties that a client can resolve lazily.
     *
     * properties: string[];
     */
    std::vector<string> properties;
  };

  /**
   * Inlay hint client capabilities.
   *
   * The inlay hints request is sent from the client to the server to compute
   * inlay hints for a given [text document, range] tuple that may be rendered
   * in the editor in place with other text.
   *
   * Client Capability:
   * - property name (optional): textDocument.inlayHint
   * - property type: InlayHintClientCapabilities defined as follows:
   *
   * export interface InlayHintClientCapabilities {
   *   dynamicRegistration?: boolean;
   *   resolveSupport?: {
   *     properties: string[];
   *   };
   * }
   *
   * @since 3.17.0
   */
  struct InlayHintClientCapabilities {

    /**
     * Whether inlay hints support dynamic registration.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;

    /**
     * Indicates which properties a client can resolve lazily on an inlay hint.
     *
     * resolveSupport?: {
     *   properties: string[];
     * };
     */
    optional_ptr<ResolveSupportCapabilities> resolveSupport;
  };

  /**
   * Inlay hint options used during static registration.
   *
   * Server Capability:
   * - property name (optional): inlayHintProvider
   * - property type: InlayHintOptions defined as follows:
   *
   * export interface InlayHintOptions extends WorkDoneProgressOptions {
   *   resolveProvider?: boolean;
   * }
   *
   * @since 3.17.0
   */
  struct InlayHintOptions : public WorkDoneProgressOptions {

    /**
     * The server provides support to resolve additional
     * information for an inlay hint item.
     *
     * resolveProvider?: boolean;
     */
    std::optional<boolean> resolveProvider;
  };

  /**
   * Inlay hint options used during static or dynamic registration.
   *
   * Registration Options: InlayHintRegistrationOptions defined as follows:
   *
   * export interface InlayHintRegistrationOptions extends InlayHintOptions,
   *   TextDocumentRegistrationOptions, StaticRegistrationOptions {
   * }
   *
   * @since 3.17.0
   */
  struct InlayHintRegistrationOptions
    : public InlayHintOptions
    , public TextDocumentRegistrationOptions
    , public StaticRegistrationOptions {
    // empty
  };

  /**
   * A parameter literal used in inlay hint requests.
   *
   * Request:
   * - method: textDocument/inlayHint
   * - params: InlayHintParams defined as follows:
   *
   * export interface InlayHintParams extends WorkDoneProgressParams {
   *   textDocument: TextDocumentIdentifier;
   *   range: Range;
   * }
   *
   * @since 3.17.0
   */
  struct InlayHintParams : public WorkDoneProgressParams {

    /**
     * The text document.
     *
     * textDocument: TextDocumentIdentifier;
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;

    /**
     * The visible document range for which inlay hints should be computed.
     *
     * range: Range;
     */
    std::unique_ptr<Range> range;
  };

  enum class StringOrMarkupContentType {
    STRING,
    MARKUP_CONTENT,
  };

  struct StringOrMarkupContent {
    StringOrMarkupContentType type;
    std::variant<
      string,
      std::unique_ptr<MarkupContent>
    > value;
  };

  /**
   * An inlay hint label part allows for interactive and composite labels
   * of inlay hints.
   *
   * @since 3.17.0
   */
  struct InlayHintLabelPart {

    /**
     * The value of this label part.
     *
     * value: string;
     */
    string value;

    /**
     * The tooltip text when you hover over this label part. Depending on the
     * client capability `inlayHint.resolveSupport` clients might resolve this
     * property late using the resolve request.
     *
     * tooltip?: string | MarkupContent;
     */
    optional_ptr<StringOrMarkupContent> tooltip;

    /**
     * An optional source code location that represents this label part.
     *
     * The editor will use this location for the hover and for code navigation
     * features: This part will become a clickable link that resolves to the
     * definition of the symbol at the given location (not necessarily the
     * location itself), it shows the hover that shows at the given location,
     * and it shows a context menu with further code navigation commands.
     *
     * Depending on the client capability `inlayHint.resolveSupport` clients
     * might resolve this property late using the resolve request.
     *
     * location?: Location;
     */
    optional_ptr<Location> location;

    /**
     * An optional command for this label part.
     *
     * Depending on the client capability `inlayHint.resolveSupport` clients
     * might resolve this property late using the resolve request.
     *
     * command?: Command;
     */
    optional_ptr<Command> command;
  };

  /**
   * Inlay hint kinds.
   *
   * export namespace InlayHintKind {
   *   export const Type = 1;
   *   export const Parameter = 2;
   * }
   * export type InlayHintKind = 1 | 2;
   *
   * @since 3.17.0
   */
  enum class InlayHintKind {

    /**
     * An inlay hint that for a type annotation.
     *
     * export const Type = 1;
     */
    Type = 1,

    /**
     * An inlay hint that is for a parameter.
     *
     * export const Parameter = 2;
     */
    Parameter = 2,
  };

  extern std::map<InlayHintKind, std::string> InlayHintKindNames;

  auto inlayHintKindByName(const std::string &name) -> InlayHintKind;

  enum class StringOrInlayHintLabelPartsType {
    STRING,
    INLAY_HINT_LABEL_PART,
  };

  struct StringOrInlayHintLabelParts {
    StringOrInlayHintLabelPartsType type;
    std::variant<
      string,
      ptr_vector<InlayHintLabelPart>
    > value;
  };

  /**
   * Inlay hint information.
   *
   * Response:
   * - result: InlayHint[] | null defined as follows:
   *
   * export interface InlayHint {
   *   position: Position;
   *   label: string | InlayHintLabelPart[];
   *   kind?: InlayHintKind;
   *   textEdits?: TextEdit[];
   *   tooltip?: string | MarkupContent;
   *   paddingLeft?: boolean;
   *   paddingRight?: boolean;
   *   data?: LSPAny;
   * }
   *
   * - error: code and message set in case an exception happens during the inlay
   *   hint request.
   *
   * @since 3.17.0
   */
  struct InlayHint {

    /**
     * The position of this hint.
     *
     * If multiple hints have the same position, they will be shown in the order
     * they appear in the response.
     *
     * position: Position;
     */
    std::unique_ptr<Position> position;

    /**
     * The label of this hint. A human readable string or an array of
     * InlayHintLabelPart label parts.
     *
     * *Note* that neither the string nor the label part can be empty.
     *
     * label: string | InlayHintLabelPart[];
     */
    std::unique_ptr<StringOrInlayHintLabelParts> label;

    /**
     * The kind of this hint. Can be omitted in which case the client
     * should fall back to a reasonable default.
     *
     * kind?: InlayHintKind;
     */
    std::optional<InlayHintKind> kind;

    /**
     * Optional text edits that are performed when accepting this inlay hint.
     *
     * *Note* that edits are expected to change the document so that the inlay
     * hint (or its nearest variant) is now part of the document and the inlay
     * hint itself is now obsolete.
     *
     * Depending on the client capability `inlayHint.resolveSupport` clients
     * might resolve this property late using the resolve request.
     *
     * textEdits?: TextEdit[];
     */
    optional_ptr_vector<TextEdit> textEdits;

    /**
     * The tooltip text when you hover over this item.
     *
     * Depending on the client capability `inlayHint.resolveSupport` clients
     * might resolve this property late using the resolve request.
     *
     * tooltip?: string | MarkupContent;
     */
    optional_ptr<StringOrMarkupContent> tooltip;

    /**
     * Render padding before the hint.
     *
     * Note: Padding should use the editor's background color, not the
     * background color of the hint itself. That means padding can be used to
     * visually align/separate an inlay hint.
     *
     * paddingLeft?: boolean;
     */
    std::optional<boolean> paddingLeft;

    /**
     * Render padding after the hint.
     *
     * Note: Padding should use the editor's background color, not the
     * background color of the hint itself. That means padding can be used to
     * visually align/separate an inlay hint.
     *
     * paddingRight?: boolean;
     */
    std::optional<boolean> paddingRight;

    /**
     * A data entry field that is preserved on an inlay hint between a
     * `textDocument/inlayHint` and a `inlayHint/resolve` request.
     *
     * data?: LSPAny;
     */
    optional_ptr<LSPAny> data;
  };

  // ---------------------------------------------------------------------------
  // Inlay Hint Resolve Request
  // ---------------------------------------------------------------------------
  // Since version 3.17.0
  //
  // The request is sent from the client to the server to resolve additional
  // information for a given inlay hint. This is usually used to compute the
  // tooltip, location or command properties of an inlay hint’s label part to
  // avoid its unnecessary computation during the textDocument/inlayHint
  // request.
  //
  // Consider the clients announces the label.location property as a property
  // that can be resolved lazy using the client capability
  //
  // textDocument.inlayHint.resolveSupport = { properties: ['label.location'] };
  //
  // then an inlay hint with a label part without a location needs to be
  // resolved using the inlayHint/resolve request before it can be used.
  //
  // Client Capability:
  // - property name (optional): textDocument.inlayHint.resolveSupport
  // - property type: { properties: string[]; }
  //
  // Request:
  // - method: inlayHint/resolve
  // - params: InlayHint
  //
  // Response:
  // - result: InlayHint
  // - error: code and message set in case an exception happens during the
  // - completion resolve request.
  // ---------------------------------------------------------------------------

  /**
   * Client workspace capabilities specific to inlay hints.
   *
   * The workspace/inlayHint/refresh request is sent from the server to the
   * client. Servers can use it to ask clients to refresh the inlay hints
   * currently shown in editors. As a result the client should ask the server to
   * recompute the inlay hints for these editors. This is useful if a server
   * detects a configuration change which requires a re-calculation of all inlay
   * hints. Note that the client still has the freedom to delay the
   * re-calculation of the inlay hints if for example an editor is currently not
   * visible.
   *
   * Client Capability:
   * - property name (optional): workspace.inlayHint
   * - property type: InlayHintWorkspaceClientCapabilities defined as follows:
   *
   * export interface InlayHintWorkspaceClientCapabilities {
   *   refreshSupport?: boolean;
   * }
   *
   * Request:
   * - method: workspace/inlayHint/refresh
   * - params: none
   *
   * Response:
   * - result: void
   * - error: code and message set in case an exception happens during the
   *   ‘workspace/inlayHint/refresh’ request
   *
   * @since 3.17.0
   */
  struct InlayHintWorkspaceClientCapabilities {

    /**
     * Whether the client implementation supports a refresh request sent from
     * the server to the client.
     *
     * Note that this event is global and will force the client to refresh all
     * inlay hints currently shown. It should be used with absolute care and
     * is useful for situation where a server for example detects a project wide
     * change that requires such a calculation.
     *
     * refreshSupport?: boolean;
     */
    std::optional<boolean> refreshSupport;
  };

  /**
   * Client capabilities specific to inline values.
   *
   * The inline value request is sent from the client to the server to compute
   * inline values for a given text document that may be rendered in the editor
   * at the end of lines.
   *
   * Client Capability:
   * - property name (optional): textDocument.inlineValue
   * - property type: InlineValueClientCapabilities defined as follows:
   *
   * export interface InlineValueClientCapabilities {
   *   dynamicRegistration?: boolean;
   * }
   *
   * @since 3.17.0
   */
  struct InlineValueClientCapabilities {

    /**
     * Whether implementation supports dynamic registration for inline
     * value providers.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;
  };

  /**
   * Inline value options used during static registration.
   *
   * Server Capability:
   * - property name (optional): inlineValueProvider
   * - property type: InlineValueOptions defined as follows:
   *
   * export interface InlineValueOptions extends WorkDoneProgressOptions {
   * }
   *
   * @since 3.17.0
   */
  struct InlineValueOptions : public WorkDoneProgressOptions {
    // empty
  };

  /**
   * Inline value options used during static or dynamic registration.
   *
   * Registration Options: InlineValueRegistrationOptions defined as follows:
   *
   * export interface InlineValueRegistrationOptions extends InlineValueOptions,
   *   TextDocumentRegistrationOptions, StaticRegistrationOptions {
   * }
   *
   * @since 3.17.0
   */
  struct InlineValueRegistrationOptions
    : public InlineValueOptions
    , public TextDocumentRegistrationOptions
    , public StaticRegistrationOptions {
    // empty
  };

  /**
   * export interface InlineValueContext {
   *   frameId: integer;
   *   stoppedLocation: Range;
   * }
   *
   * @since 3.17.0
   */
  struct InlineValueContext {

    /**
     * The stack frame (as a DAP Id) where the execution has stopped.
     *
     * frameId: integer;
     */
    integer frameId;

    /**
     * The document range where execution has stopped. Typically the end
     * position of the range denotes the line where the inline values are shown.
     *
     * stoppedLocation: Range;
     */
    std::unique_ptr<Range> stoppedLocation;
  };

  /**
   * A parameter literal used in inline value requests.
   *
   * Request:
   * - method: textDocument/inlineValue
   * - params: InlineValueParams defined as follows:
   *
   * export interface InlineValueParams extends WorkDoneProgressParams {
   *   textDocument: TextDocumentIdentifier;
   *   range: Range;
   *   context: InlineValueContext;
   * }
   *
   * @since 3.17.0
   */
  struct InlineValueParams : public WorkDoneProgressParams {

    /**
     * The text document.
     *
     * textDocument: TextDocumentIdentifier;
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;

    /**
     * The document range for which inline values should be computed.
     *
     * range: Range;
     */
    std::unique_ptr<Range> range;

    /**
     * Additional information about the context in which inline values were
     * requested.
     *
     * context: InlineValueContext;
     */
    std::unique_ptr<InlineValueContext> context;
  };

  /**
   * Provide inline value as text.
   *
   * export interface InlineValueText {
   *   range: Range;
   *   text: string;
   * }
   *
   * @since 3.17.0
   */
  struct InlineValueText {

    /**
     * The document range for which the inline value applies.
     *
     * range: Range;
     */
    std::unique_ptr<Range> range;

    /**
     * The text of the inline value.
     *
     * text: string;
     */
    string text;
  };

  /**
   * Provide inline value through a variable lookup.
   *
   * If only a range is specified, the variable name will be extracted from the
   * underlying document.
   *
   * An optional variable name can be used to override the extracted name.
   *
   * export interface InlineValueVariableLookup {
   *   range: Range;
   *   variableName?: string;
   *   caseSensitiveLookup: boolean;
   * }
   *
   * @since 3.17.0
   */
  struct InlineValueVariableLookup {

    /**
     * The document range for which the inline value applies. The range is used
     * to extract the variable name from the underlying document.
     *
     * range: Range;
     */
    std::unique_ptr<Range> range;

    /**
     * If specified the name of the variable to look up.
     *
     * variableName?: string;
     */
    std::optional<string> variableName;

    /**
     * How to perform the lookup.
     *
     * caseSensitiveLookup: boolean;
     */
    boolean caseSensitiveLookup;
  };

  /**
   * Provide an inline value through an expression evaluation.
   *
   * If only a range is specified, the expression will be extracted from the
   * underlying document.
   *
   * An optional expression can be used to override the extracted expression.
   *
   * export interface InlineValueEvaluatableExpression {
   *   range: Range;
   *   expression?: string;
   * }
   *
   * @since 3.17.0
   */
  struct InlineValueEvaluatableExpression {

    /**
     * The document range for which the inline value applies. The range is used
     * to extract the evaluatable expression from the underlying document.
     *
     * range: Range;
     */
    std::unique_ptr<Range> range;

    /**
     * If specified the expression overrides the extracted expression.
     *
     * expression?: string;
     */
    std::optional<string> expression;
  };

  enum class InlineValueType {
    INLINE_VALUE_TEXT,
    INLINE_VALUE_VARIABLE_LOOKUP,
    INLINE_VALUE_EVALUATABLE_EXPRESSION,
  };

  /**
   * Inline value information can be provided by different means:
   * - directly as a text value (class InlineValueText).
   * - as a name to use for a variable lookup (class InlineValueVariableLookup)
   * - as an evaluatable expression (class InlineValueEvaluatableExpression)
   * The InlineValue types combines all inline value types into one type.
   *
   * Response:
   * - result: InlineValue[] | null defined as follows:
   * - error: code and message set in case an exception happens during the
   *   inline values request.
   *
   * @since 3.17.0
   */
  struct InlineValue {
    InlineValueType type;
    std::variant<
      std::unique_ptr<InlineValueText>,
      std::unique_ptr<InlineValueVariableLookup>,
      std::unique_ptr<InlineValueEvaluatableExpression>
    > value;
  };

  /**
   * Client workspace capabilities specific to inline values.
   *
   * The workspace/inlineValue/refresh request is sent from the server to the
   * client. Servers can use it to ask clients to refresh the inline values
   * currently shown in editors. As a result the client should ask the server to
   * recompute the inline values for these editors. This is useful if a server
   * detects a configuration change which requires a re-calculation of all
   * inline values. Note that the client still has the freedom to delay the
   * re-calculation of the inline values if for example an editor is currently
   * not visible.
   *
   * Client Capability:
   * - property name (optional): workspace.inlineValue
   * - property type: InlineValueWorkspaceClientCapabilities defined as follows:
   *
   * export interface InlineValueWorkspaceClientCapabilities {
   *   refreshSupport?: boolean;
   * }
   *
   * Request:
   * - method: workspace/inlineValue/refresh
   * - params: none
   *
   * Response:
   * - result: void
   * - error: code and message set in case an exception happens during the
   *   ‘workspace/inlineValue/refresh’ request
   *
   * @since 3.17.0
   */
  struct InlineValueWorkspaceClientCapabilities {

    /**
     * Whether the client implementation supports a refresh request sent from
     * the server to the client.
     *
     * Note that this event is global and will force the client to refresh all
     * inline values currently shown. It should be used with absolute care and
     * is useful for situation where a server for example detect a project wide
     * change that requires such a calculation.
     *
     * refreshSupport?: boolean;
     */
    std::optional<boolean> refreshSupport;
  };

  /**
   * Language Server Index Format (LSIF) introduced the concept of symbol
   * monikers to help associate symbols across different indexes. This request
   * adds capability for LSP server implementations to provide the same symbol
   * moniker information given a text document position. Clients can utilize
   * this method to get the moniker at the current location in a file user is
   * editing and do further code navigation queries in other services that rely
   * on LSIF indexes and link symbols together.
   *
   * The textDocument/moniker request is sent from the client to the server to
   * get the symbol monikers for a given text document position. An array of
   * Moniker types is returned as response to indicate possible monikers at the
   * given location. If no monikers can be calculated, an empty array or null
   * should be returned.
   *
   * Client Capabilities:
   * - property name (optional): textDocument.moniker
   * - property type: MonikerClientCapabilities defined as follows:
   *
   * interface MonikerClientCapabilities {
   *   dynamicRegistration?: boolean;
   * }
   */
  struct MonikerClientCapabilities {

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
   * - property name (optional): monikerProvider
   * - property type: boolean | MonikerOptions | MonikerRegistrationOptions is
   *   defined as follows:
   *
   * export interface MonikerOptions extends WorkDoneProgressOptions {
   * }
   */
  struct MonikerOptions : public WorkDoneProgressOptions {
    // empty
  };

  /**
   * Registration Options: MonikerRegistrationOptions defined as follows:
   *
   * export interface MonikerRegistrationOptions extends
   *   TextDocumentRegistrationOptions, MonikerOptions {
   * }
   */
  struct MonikerRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public MonikerOptions {
    // empty
  };

  /**
   * Request:
   * - method: textDocument/moniker
   * - params: MonikerParams defined as follows:
   *
   * export interface MonikerParams extends TextDocumentPositionParams,
   *   WorkDoneProgressParams, PartialResultParams {
   * }
   */
  struct MonikerParams
    : public TextDocumentPositionParams
    , public WorkDoneProgressParams
    , public PartialResultParams {
    // empty
  };

  /**
   * Moniker uniqueness level to define scope of the moniker.
   */
  enum class UniquenessLevel {

    /**
     * The moniker is only unique inside a document
     *
     * document = 'document',
     */
    Document,

    /**
     * The moniker is unique inside a project for which a dump got created
     *
     * project = 'project',
     */
    Project,

    /**
     * The moniker is unique inside the group to which a project belongs
     *
     * group = 'group',
     */
    Group,

    /**
     * The moniker is unique inside the moniker scheme.
     *
     * scheme = 'scheme',
     */
    Scheme,

    /**
     * The moniker is globally unique
     *
     * global = 'global'
     */
    Global,
  };

  extern std::map<UniquenessLevel, std::string> UniquenessLevelNames;

  auto uniquenessLevelByName(const std::string &name) -> UniquenessLevel;

  extern std::map<UniquenessLevel, std::string> UniquenessLevelValues;

  auto uniquenessLevelByValue(const std::string &value) -> UniquenessLevel;

  /**
   * The moniker kind.
   */
  enum class MonikerKind {

    /**
     * The moniker represent a symbol that is imported into a project
     *
     * import = 'import',
     */
    Import,

    /**
     * The moniker represents a symbol that is exported from a project
     *
     * export = 'export',
     */
    Export,

    /**
     * The moniker represents a symbol that is local to a project (e.g. a local
     * variable of a function, a class not visible outside the project, ...)
     *
     * local = 'local'
     */
    Local,
  };

  extern std::map<MonikerKind, std::string> MonikerKindNames;

  auto monikerKindByName(const std::string &name) -> MonikerKind;

  extern std::map<MonikerKind, std::string> MonikerKindValues;

  auto monikerKindByValue(const std::string &value) -> MonikerKind;

  /**
   * Moniker definition to match LSIF 0.5 moniker definition.
   *
   * Response:
   * - result: Moniker[] | null
   * - partial result: Moniker[]
   * - error: code and message set in case an exception happens during the
   *   ‘textDocument/moniker’ request
   *
   * Moniker is defined as follows:
   *
   * export interface Moniker {
   *   scheme: string;
   *   identifier: string;
   *   unique: UniquenessLevel;
   *   kind?: MonikerKind;
   * }
   */
  struct Moniker {

    /**
     * The scheme of the moniker. For example tsc or .Net
     *
     * scheme: string;
     */
    string scheme;

    /**
     * The identifier of the moniker. The value is opaque in LSIF however
     * schema owners are allowed to define the structure if they want.
     *
     * identifier: string;
     */
    string identifier;

    /**
     * The scope in which the moniker is unique
     *
     * unique: UniquenessLevel;
     */
    UniquenessLevel unique;

    /**
     * The moniker kind if known.
     *
     * kind?: MonikerKind;
     */
    std::optional<MonikerKind> kind;
  };

  /**
   * Completion item tags are extra annotations that tweak the rendering of a
   * completion item.
   *
   * export namespace CompletionItemTag {
   *   export const Deprecated = 1;
   * }
   * export type CompletionItemTag = 1;
   *
   * @since 3.15.0
   */
  enum class CompletionItemTag {

    /**
     * Render a completion as obsolete, usually using a strike-out.
     *
     * export const Deprecated = 1;
     */
    Deprecated = 1,
  };

  extern std::map<CompletionItemTag, std::string> CompletionItemTagNames;

  auto completionItemTagByName(const std::string &name) -> CompletionItemTag;

  /**
   * How whitespace and indentation is handled during completion item insertion.
   *
   * export namespace InsertTextMode {
   *   export const asIs: 1 = 1;
   *   export const adjustIndentation: 2 = 2;
   * }
   * export type InsertTextMode = 1 | 2;
   *
   * @since 3.16.0
   */
  enum class InsertTextMode {

    /**
     * The insertion or replace strings is taken as it is. If the value is multi
     * line the lines below the cursor will be inserted using the indentation
     * defined in the string value. The client will not apply any kind of
     * adjustments to the string.
     *
     * export const asIs: 1 = 1;
     */
    asIs = 1,

    /**
     * The editor adjusts leading whitespace of new lines so that they match the
     * indentation up to the cursor of the line for which the item is accepted.
     *
     * Consider a line like this: <2tabs><cursor><3tabs>foo. Accepting a multi
     * line completion item is indented using 2 tabs and all following lines
     * inserted will be indented using 2 tabs as well.
     *
     * export const adjustIndentation: 2 = 2;
     */
    adjustIndentation = 2,
  };

  extern std::map<InsertTextMode, std::string> InsertTextModeNames;

  auto insertTextModeByName(const std::string &name) -> InsertTextMode;

  struct CompletionItemCapabilities {

    /**
     * Client supports snippets as insert text.
     *
     * A snippet can define tab stops and placeholders with `$1`, `$2` and
     * `${3:foo}`. `$0` defines the final tab stop, it defaults to the end of
     * the snippet. Placeholders with equal identifiers are linked, that is
     * typing in one will update others too.
     *
     * snippetSupport?: boolean;
     */
    std::optional<boolean> snippetSupport;

    /**
     * Client supports commit characters on a completion item.
     *
     * commitCharactersSupport?: boolean;
     */
    std::optional<boolean> commitCharactersSupport;

    /**
     * Client supports the follow content formats for the documentation
     * property. The order describes the preferred format of the client.
     *
     * documentationFormat?: MarkupKind[];
     */
    optional_vector<MarkupKind> documentationFormat;

    /**
     * Client supports the deprecated property on a completion item.
     *
     * deprecatedSupport?: boolean;
     */
    std::optional<boolean> deprecatedSupport;

    /**
     * Client supports the preselect property on a completion item.
     *
     * preselectSupport?: boolean;
     */
    std::optional<boolean> preselectSupport;

    /**
     * Client supports the tag property on a completion item. Clients supporting
     * tags have to handle unknown tags gracefully. Clients especially need to
     * preserve unknown tags when sending a completion item back to the server
     * in a resolve call.
     *
     * tagSupport?: {
     *   valueSet: CompletionItemTag[];
     * };
     *
     * @since 3.15.0
     */
    optional_ptr<ValueSet<CompletionItemTag>> tagSupport;

    /**
     * Client supports insert replace edit to control different behavior if
     * a completion item is inserted in the text or should replace text.
     *
     * insertReplaceSupport?: boolean;
     *
     * @since 3.16.0
     */
    std::optional<boolean> insertReplaceSupport;

    /**
     * Indicates which properties a client can resolve lazily on a completion
     * item. Before version 3.16.0 only the predefined properties
     * `documentation` and `detail` could be resolved lazily.
     *
     * resolveSupport?: {
     *   properties: string[];
     * };
     *
     * @since 3.16.0
     */
    optional_ptr<ResolveSupportCapabilities> resolveSupport;

    /**
     * The client supports the `insertTextMode` property on a completion item to
     * override the whitespace handling mode as defined by the client (see
     * `insertTextMode`).
     *
     * insertTextModeSupport?: {
     *   valueSet: InsertTextMode[];
     * };
     *
     * @since 3.16.0
     */
    optional_ptr<ValueSet<InsertTextMode>> insertTextModeSupport;

    /**
     * The client has support for completion item label
     * details (see also `CompletionItemLabelDetails`).
     *
     * labelDetailsSupport?: boolean;
     *
     * @since 3.17.0
     */
    std::optional<boolean> labelDetailsSupport;
  };

  struct CompletionListCapabilities {

    /**
     * The client supports the following itemDefaults on a completion list.
     *
     * The value lists the supported property names of the
     * `CompletionList.itemDefaults` object. If omitted no properties are
     * supported.
     *
     * itemDefaults?: string[];
     *
     * @since 3.17.0
     */
    optional_vector<string> itemDefaults;
  };

  /**
   * The kind of a completion entry.
   *
   * export namespace CompletionItemKind {
   *   export const Text = 1;
   *   export const Method = 2;
   *   export const Function = 3;
   *   export const Constructor = 4;
   *   export const Field = 5;
   *   export const Variable = 6;
   *   export const Class = 7;
   *   export const Interface = 8;
   *   export const Module = 9;
   *   export const Property = 10;
   *   export const Unit = 11;
   *   export const Value = 12;
   *   export const Enum = 13;
   *   export const Keyword = 14;
   *   export const Snippet = 15;
   *   export const Color = 16;
   *   export const File = 17;
   *   export const Reference = 18;
   *   export const Folder = 19;
   *   export const EnumMember = 20;
   *   export const Constant = 21;
   *   export const Struct = 22;
   *   export const Event = 23;
   *   export const Operator = 24;
   *   export const TypeParameter = 25;
   * }
   * export type CompletionItemKind = 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 |
   *   11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23 | 24 | 25;
   */
  enum class CompletionItemKind {
    Text = 1,
    Method = 2,
    Function = 3,
    Constructor = 4,
    Field = 5,
    Variable = 6,
    Class = 7,
    Interface = 8,
    Module = 9,
    Property = 10,
    Unit = 11,
    Value = 12,
    Enum = 13,
    Keyword = 14,
    Snippet = 15,
    Color = 16,
    File = 17,
    Reference = 18,
    Folder = 19,
    EnumMember = 20,
    Constant = 21,
    Struct = 22,
    Event = 23,
    Operator = 24,
    TypeParameter = 25,
  };

  extern std::map<CompletionItemKind, std::string> CompletionItemKindNames;

  auto completionItemKindByName(const std::string &name) -> CompletionItemKind;

  /**
   * The Completion request is sent from the client to the server to compute
   * completion items at a given cursor position. Completion items are presented
   * in the IntelliSense user interface. If computing full completion items is
   * expensive, servers can additionally provide a handler for the completion
   * item resolve request (‘completionItem/resolve’). This request is sent when
   * a completion item is selected in the user interface. A typical use case is
   * for example: the textDocument/completion request doesn’t fill in the
   * documentation property for returned completion items since it is expensive
   * to compute. When the item is selected in the user interface then a
   * ‘completionItem/resolve’ request is sent with the selected completion item
   * as a parameter. The returned completion item should have the documentation
   * property filled in. By default the request can only delay the computation
   * of the detail and documentation properties. Since 3.16.0 the client can
   * signal that it can resolve more properties lazily. This is done using the
   * completionItem#resolveSupport client capability which lists all properties
   * that can be filled in during a ‘completionItem/resolve’ request. All other
   * properties (usually sortText, filterText, insertText and textEdit) must be
   * provided in the textDocument/completion response and must not be changed
   * during resolve.
   *
   * The language server protocol uses the following model around completions:
   * - to achieve consistency across languages and to honor different clients
   *   usually the client is responsible for filtering and sorting. This has
   *   also the advantage that client can experiment with different filter and
   *   sorting models. However servers can enforce different behavior by setting
   *   a filterText / sortText
   * - for speed clients should be able to filter an already received completion
   *   list if the user continues typing. Servers can opt out of this using a
   *   CompletionList and mark it as isIncomplete.
   *
   * A completion item provides additional means to influence filtering and
   * sorting. They are expressed by either creating a CompletionItem with a
   * insertText or with a textEdit. The two modes differ as follows:
   * - Completion item provides an insertText / label without a text edit: in
   *   the model the client should filter against what the user has already
   *   typed using the word boundary rules of the language (e.g. resolving the
   *   word under the cursor position). The reason for this mode is that it
   *   makes it extremely easy for a server to implement a basic completion list
   *   and get it filtered on the client.
   * - Completion Item with text edits: in this mode the server tells the client
   *   that it actually knows what it is doing. If you create a completion item
   *   with a text edit at the current cursor position no word guessing takes
   *   place and no automatic filtering (like with an insertText) should happen.
   *   This mode can be combined with a sort text and filter text to customize
   *   two things. If the text edit is a replace edit then the range denotes the
   *   word used for filtering. If the replace changes the text it most likely
   *   makes sense to specify a filter text to be used.
   *
   * Client Capability:
   * - property name (optional): textDocument.completion
   * - property type: CompletionClientCapabilities defined as follows:
   *
   * export interface CompletionClientCapabilities {
   *   dynamicRegistration?: boolean;
   *   completionItem?: {
   *     snippetSupport?: boolean;
   *     commitCharactersSupport?: boolean;
   *     documentationFormat?: MarkupKind[];
   *     deprecatedSupport?: boolean;
   *     preselectSupport?: boolean;
   *     tagSupport?: {
   *       valueSet: CompletionItemTag[];
   *     };
   *     insertReplaceSupport?: boolean;
   *     resolveSupport?: {
   *       properties: string[];
   *     };
   *     insertTextModeSupport?: {
   *       valueSet: InsertTextMode[];
   *     };
   *     labelDetailsSupport?: boolean;
   *   };
   *   completionItemKind?: {
   *     valueSet?: CompletionItemKind[];
   *   };
   *   contextSupport?: boolean;
   *   insertTextMode?: InsertTextMode;
   *   completionList?: {
   *     itemDefaults?: string[];
   *   }
   * }
   */
  struct CompletionClientCapabilities {

    /**
     * Whether completion supports dynamic registration.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;

    /**
     * The client supports the following `CompletionItem` specific capabilities.
     *
     * completionItem?: {
     *   snippetSupport?: boolean;
     *   commitCharactersSupport?: boolean;
     *   documentationFormat?: MarkupKind[];
     *   deprecatedSupport?: boolean;
     *   preselectSupport?: boolean;
     *   tagSupport?: {
     *     valueSet: CompletionItemTag[];
     *   };
     *   insertReplaceSupport?: boolean;
     *   resolveSupport?: {
     *     properties: string[];
     *   };
     *   insertTextModeSupport?: {
     *     valueSet: InsertTextMode[];
     *   };
     *   labelDetailsSupport?: boolean;
     * };
     */
    optional_ptr<CompletionItemCapabilities> completionItem;

    /**
     * completionItemKind?: {
     *   // The completion item kind values the client supports. When this
     *   // property exists the client also guarantees that it will handle
     *   // values outside its set gracefully and falls back to a default value
     *   // when unknown.
     *   //
     *   // If this property is not present the client only supports the
     *   // completion items kinds from `Text` to `Reference` as defined in the
     *   // initial version of the protocol.
     *   valueSet?: CompletionItemKind[];
     * };
     */
    optional_ptr<OptionalValueSet<CompletionItemKind>> completionItemKind;

    /**
     * The client supports to send additional context information for a
     * `textDocument/completion` request.
     *
     * contextSupport?: boolean;
     */
    std::optional<boolean> contextSupport;

    /**
     * The client's default when the completion item doesn't provide a
     * `insertTextMode` property.
     *
     * insertTextMode?: InsertTextMode;
     *
     * @since 3.17.0
     */
    std::optional<InsertTextMode> insertTextMode;

    /**
     * The client supports the following `CompletionList` specific
     * capabilities.
     * completionList?: {
     *   itemDefaults?: string[];
     * }
     *
     * @since 3.17.0
     */
    optional_ptr<CompletionListCapabilities> completionList;
  };

  struct CompletionItemOptions {

    /**
     * The server has support for completion item label
     * details (see also `CompletionItemLabelDetails`) when receiving
     * a completion item in a resolve call.
     *
     * labelDetailsSupport?: boolean;
     *
     * @since 3.17.0
     */
    std::optional<boolean> labelDetailsSupport;
  };

  /**
   * Completion options.
   *
   * Server Capability:
   * - property name (optional): completionProvider
   * - property type: CompletionOptions defined as follows:
   *
   * export interface CompletionOptions extends WorkDoneProgressOptions {
   *   triggerCharacters?: string[];
   *   allCommitCharacters?: string[];
   *   resolveProvider?: boolean;
   *   completionItem?: {
   *     labelDetailsSupport?: boolean;
   *   }
   * }
   */
  struct CompletionOptions : public WorkDoneProgressOptions {

    /**
     * The additional characters, beyond the defaults provided by the client
     * (typically [a-zA-Z]), that should automatically trigger a completion
     * request. For example `.` in JavaScript represents the beginning of an
     * object property or method and is thus a good candidate for triggering a
     * completion request.
     *
     * Most tools trigger a completion request automatically without explicitly
     * requesting it using a keyboard shortcut (e.g. Ctrl+Space). Typically they
     * do so when the user starts to type an identifier. For example if the user
     * types `c` in a JavaScript file code complete will automatically pop up
     * present `console` besides others as a completion item. Characters that
     * make up identifiers don't need to be listed here.
     *
     * triggerCharacters?: string[];
     */
    optional_vector<string> triggerCharacters;

    /**
     * The list of all possible characters that commit a completion. This field
     * can be used if clients don't support individual commit characters per
     * completion item. See client capability
     * `completion.completionItem.commitCharactersSupport`.
     *
     * If a server provides both `allCommitCharacters` and commit characters on
     * an individual completion item the ones on the completion item win.
     *
     * allCommitCharacters?: string[];
     *
     * @since 3.2.0
     */
    optional_vector<string> allCommitCharacters;

    /**
     * The server provides support to resolve additional information for a
     * completion item.
     *
     * resolveProvider?: boolean;
     */
    std::optional<boolean> resolveProvider;

    /**
     * The server supports the following `CompletionItem` specific capabilities.
     *
     * completionItem?: {
     *   labelDetailsSupport?: boolean;
     * }
     *
     * @since 3.17.0
     */
    optional_ptr<CompletionItemOptions> completionItem;
  };

  /**
   * Registration Options: CompletionRegistrationOptions options defined as
   * follows:
   *
   * export interface CompletionRegistrationOptions
   *   extends TextDocumentRegistrationOptions, CompletionOptions {
   * }
   */
  struct CompletionRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public CompletionOptions {
    // empty
  };

  /**
   * How a completion was triggered.
   *
   * export namespace CompletionTriggerKind {
   *   export const Invoked: 1 = 1;
   *   export const TriggerCharacter: 2 = 2;
   *   export const TriggerForIncompleteCompletions: 3 = 3;
   * }
   * export type CompletionTriggerKind = 1 | 2 | 3;
   */
  enum class CompletionTriggerKind {
    Invoked = 1,
    TriggerCharacter = 2,
    TriggerForIncompleteCompletions = 3,
  };

  extern std::map<CompletionTriggerKind, std::string> CompletionTriggerKindNames;

  auto completionTriggerKindByName(const std::string &name) -> CompletionTriggerKind;

  /**
   * Contains additional information about the context in which a completion
   * request is triggered.
   *
   * export interface CompletionContext {
   *   triggerKind: CompletionTriggerKind;
   *   triggerCharacter?: string;
   * }
   */
  struct CompletionContext {

    /**
     * How the completion was triggered.
     *
     * triggerKind: CompletionTriggerKind;
     */
    CompletionTriggerKind triggerKind;

    /**
     * The trigger character (a single character) that has trigger code
     * complete. Is undefined if
     * `triggerKind !== CompletionTriggerKind.TriggerCharacter`
     *
     * triggerCharacter?: string;
     */
    std::optional<string> triggerCharacter;
  };

  /**
   * Request:
   * - method: textDocument/completion
   * - params: CompletionParams defined as follows:
   *
   * export interface CompletionParams extends TextDocumentPositionParams,
   *   WorkDoneProgressParams, PartialResultParams {
   *   context?: CompletionContext;
   * }
   */
  struct CompletionParams
    : public TextDocumentPositionParams
    , public WorkDoneProgressParams
    , public PartialResultParams {

    /**
     * The completion context. This is only available if the client specifies to
     * send this using the client capability `completion.contextSupport ===
     * true`
     *
     * context?: CompletionContext;
     */
    optional_ptr<CompletionContext> context;
  };

  /**
   * Defines whether the insert text in a completion item should be interpreted
   * as plain text or a snippet.
   *
   * export namespace InsertTextFormat {
   *   export const PlainText = 1;
   *   export const Snippet = 2;
   * }
   * export type InsertTextFormat = 1 | 2;
   */
  enum class InsertTextFormat {

    /**
     * The primary text to be inserted is treated as a plain string.
     *
     * export const PlainText = 1;
     */
    PlainText = 1,

    /**
     * The primary text to be inserted is treated as a snippet.
     *
     * A snippet can define tab stops and placeholders with `$1`, `$2` and
     * `${3:foo}`. `$0` defines the final tab stop, it defaults to the end of
     * the snippet. Placeholders with equal identifiers are linked, that is
     * typing in one will update others too.
     *
     * export const Snippet = 2;
     */
    Snippet = 2,
  };

  extern std::map<InsertTextFormat, std::string> InsertTextFormatNames;

  auto insertTextFormatByName(const std::string &name) -> InsertTextFormat;

  /**
   * A special text edit to provide an insert and a replace operation.
   *
   * export interface InsertReplaceEdit {
   *   newText: string;
   *   insert: Range;
   *   replace: Range;
   * }
   *
   * @since 3.16.0
   */
  struct InsertReplaceEdit {

    /**
     * The string to be inserted.
     *
     * newText: string;
     */
    string newText;

    /**
     * The range if the insert is requested
     *
     * insert: Range;
     */
    std::unique_ptr<Range> insert;

    /**
     * The range if the replace is requested.
     *
     * replace: Range;
     */
    std::unique_ptr<Range> replace;
  };

  /**
   * Additional details for a completion item label.
   *
   * export interface CompletionItemLabelDetails {
   *   detail?: string;
   *   description?: string;
   * }
   *
   * @since 3.17.0
   */
  struct CompletionItemLabelDetails {

    /**
     * An optional string which is rendered less prominently directly after
     * {@link CompletionItem.label label}, without any spacing. Should be used
     * for function signatures or type annotations.
     *
     * detail?: string;
     */
    std::optional<string> detail;

    /**
     * An optional string which is rendered less prominently after {@link
     * CompletionItemLabelDetails.detail}. Should be used for fully qualified
     * names or file path.
     *
     * description?: string;
     */
    std::optional<string> description;
  };

  enum class TextEditOrInsertReplaceEditType {
    TEXT_EDIT,
    INSERT_REPLACE_EDIT,
  };

  struct TextEditOrInsertReplaceEdit {
    TextEditOrInsertReplaceEditType type;
    std::variant<
      std::unique_ptr<TextEdit>,
      std::unique_ptr<InsertReplaceEdit>
    > value;
  };

  struct CompletionItem {

    /**
     * The label of this completion item.
     *
     * The label property is also by default the text that is inserted when
     * selecting this completion.
     *
     * If label details are provided the label itself should be an unqualified
     * name of the completion item.
     *
     * label: string;
     */
    string label;

    /**
     * Additional details for the label
     *
     * labelDetails?: CompletionItemLabelDetails;
     *
     * @since 3.17.0
     */
    optional_ptr<CompletionItemLabelDetails> labelDetails;

    /**
     * The kind of this completion item. Based of the kind an icon is chosen by
     * the editor. The standardized set of available values is defined in
     * `CompletionItemKind`.
     *
     * kind?: CompletionItemKind;
     */
    std::optional<CompletionItemKind> kind;

    /**
     * Tags for this completion item.
     *
     * tags?: CompletionItemTag[];
     *
     * @since 3.15.0
     */
    optional_vector<CompletionItemTag> tags;

    /**
     * A human-readable string with additional information about this item, like
     * type or symbol information.
     *
     * detail?: string;
     */
    std::optional<string> detail;

    /**
     * A human-readable string that represents a doc-comment.
     *
     * documentation?: string | MarkupContent;
     */
    optional_ptr<StringOrMarkupContent> documentation;

    /**
     * Indicates if this item is deprecated.
     *
     * deprecated?: boolean;
     *
     * @deprecated Use `tags` instead if supported.
     */
    std::optional<boolean> deprecated;

    /**
     * Select this item when showing.
     *
     * *Note* that only one completion item can be selected and that the tool /
     * client decides which item that is. The rule is that the *first* item of
     * those that match best is selected.
     *
     * preselect?: boolean;
     */
    std::optional<boolean> preselect;

    /**
     * A string that should be used when comparing this item with other items.
     * When omitted the label is used as the sort text for this item.
     *
     * sortText?: string;
     */
    std::optional<string> sortText;

    /**
     * A string that should be used when filtering a set of completion items.
     * When omitted the label is used as the filter text for this item.
     *
     * filterText?: string;
     */
    std::optional<string> filterText;

    /**
     * A string that should be inserted into a document when selecting this
     * completion. When omitted the label is used as the insert text for this
     * item.
     *
     * The `insertText` is subject to interpretation by the client side. Some
     * tools might not take the string literally. For example VS Code when code
     * complete is requested in this example `con<cursor position>` and a
     * completion item with an `insertText` of `console` is provided it will
     * only insert `sole`. Therefore it is recommended to use `textEdit` instead
     * since it avoids additional client side interpretation.
     *
     * insertText?: string;
     */
    std::optional<string> insertText;

    /**
     * The format of the insert text. The format applies to both the
     * `insertText` property and the `newText` property of a provided
     * `textEdit`. If omitted defaults to `InsertTextFormat.PlainText`.
     *
     * Please note that the insertTextFormat doesn't apply to
     * `additionalTextEdits`.
     *
     * insertTextFormat?: InsertTextFormat;
     */
    std::optional<InsertTextFormat> insertTextFormat;

    /**
     * How whitespace and indentation is handled during completion item
     * insertion. If not provided the client's default value depends on the
     * `textDocument.completion.insertTextMode` client capability.
     *
     * insertTextMode?: InsertTextMode;
     *
     * @since 3.16.0
     * @since 3.17.0 - support for `textDocument.completion.insertTextMode`
     */
    std::optional<InsertTextMode> insertTextMode;

    /**
     * An edit which is applied to a document when selecting this completion.
     * When an edit is provided the value of `insertText` is ignored.
     *
     * *Note:* The range of the edit must be a single line range and it must
     * contain the position at which completion has been requested.
     *
     * Most editors support two different operations when accepting a completion
     * item. One is to insert a completion text and the other is to replace an
     * existing text with a completion text. Since this can usually not be
     * predetermined by a server it can report both ranges. Clients need to
     * signal support for `InsertReplaceEdit`s via the
     * `textDocument.completion.completionItem.insertReplaceSupport` client
     * capability property.
     *
     * *Note 1:* The text edit's range as well as both ranges from an insert
     * replace edit must be a [single line] and they must contain the position
     * at which completion has been requested. *Note 2:* If an
     * `InsertReplaceEdit` is returned the edit's insert range must be a prefix
     * of the edit's replace range, that means it must be contained and starting
     * at the same position.
     *
     * textEdit?: TextEdit | InsertReplaceEdit;
     *
     * @since 3.16.0 additional type `InsertReplaceEdit`
     */
    optional_ptr<TextEditOrInsertReplaceEdit> textEdit;

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
     * textEditText?: string;
     *
     * @since 3.17.0
     */
    std::optional<string> textEditText;

    /**
     * An optional array of additional text edits that are applied when
     * selecting this completion. Edits must not overlap (including the same
     * insert position) with the main edit nor with themselves.
     *
     * Additional text edits should be used to change text unrelated to the
     * current cursor position (for example adding an import statement at the
     * top of the file if the completion item will insert an unqualified type).
     *
     * additionalTextEdits?: TextEdit[];
     */
    optional_ptr_vector<TextEdit> additionalTextEdits;

    /**
     * An optional set of characters that when pressed while this completion is
     * active will accept it first and then type that character. *Note* that all
     * commit characters should have `length=1` and that superfluous characters
     * will be ignored.
     *
     * commitCharacters?: string[];
     */
    optional_vector<string> commitCharacters;

    /**
     * An optional command that is executed *after* inserting this completion.
     * *Note* that additional modifications to the current document should be
     * described with the additionalTextEdits-property.
     *
     * command?: Command;
     */
    optional_ptr<Command> command;

    /**
     * A data entry field that is preserved on a completion item between a
     * completion and a completion resolve request.
     *
     * data?: LSPAny;
     */
    optional_ptr<LSPAny> data;
  };

  struct InsertReplace {
    std::unique_ptr<Range> insert;
    std::unique_ptr<Range> replace;
  };

  enum class RangeOrInsertReplaceType {
    RANGE,
    INSERT_REPLACE,
  };

  struct RangeOrInsertReplace {
    RangeOrInsertReplaceType type;
    std::variant<
      std::unique_ptr<Range>,
      std::unique_ptr<InsertReplace>
    > value;
  };

  struct CompletionListItemDefaults {

    /**
     * A default commit character set.
     *
     * commitCharacters?: string[];
     *
     * @since 3.17.0
     */
    optional_vector<string> commitCharacters;

    /**
     * A default edit range
     *
     * editRange?: Range | {
     *   insert: Range;
     *   replace: Range;
     * };
     *
     * @since 3.17.0
     */
    optional_ptr<RangeOrInsertReplace> editRange;

    /**
     * A default insert text format
     *
     * insertTextFormat?: InsertTextFormat;
     *
     * @since 3.17.0
     */
    std::optional<InsertTextFormat> insertTextFormat;

    /**
     * A default insert text mode
     *
     * insertTextMode?: InsertTextMode;
     *
     * @since 3.17.0
     */
    std::optional<InsertTextMode> insertTextMode;

    /**
     * A default data value.
     *
     * data?: LSPAny;
     *
     * @since 3.17.0
     */
    optional_ptr<LSPAny> data;
  };

  /**
   * Represents a collection of [completion items](#CompletionItem) to be
   * presented in the editor.
   *
   * Response:
   * - result: CompletionItem[] | CompletionList | null. If a CompletionItem[]
   *   is provided it is interpreted to be complete. So it is the same as {
   *   isIncomplete: false, items }
   * - partial result: CompletionItem[] or CompletionList followed by
   *   CompletionItem[]. If the first provided result item is of type
   *   CompletionList subsequent partial results of CompletionItem[] add to the
   *   items property of the CompletionList.
   * - error: code and message set in case an exception happens during the
   *   completion request.
   *
   * export interface CompletionList {
   *   isIncomplete: boolean;
   *   itemDefaults?: {
   *     commitCharacters?: string[];
   *     editRange?: Range | {
   *       insert: Range;
   *       replace: Range;
   *     };
   *     insertTextFormat?: InsertTextFormat;
   *     insertTextMode?: InsertTextMode;
   *     data?: LSPAny;
   *   }
   *   items: CompletionItem[];
   * }
   */
  struct CompletionList {

    /**
     * This list is not complete. Further typing should result in recomputing
     * this list.
     *
     * Recomputed lists have all their items replaced (not appended) in the
     * incomplete completion sessions.
     *
     * isIncomplete: boolean;
     */
    boolean isIncomplete;

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
     * itemDefaults?: {
     *   commitCharacters?: string[];
     *   editRange?: Range | {
     *     insert: Range;
     *     replace: Range;
     *   };
     *   insertTextFormat?: InsertTextFormat;
     *   insertTextMode?: InsertTextMode;
     *   data?: LSPAny;
     * }
     *
     * @since 3.17.0
     */
    optional_ptr<CompletionListItemDefaults> itemDefaults;

    /**
     * The completion items.
     *
     * items: CompletionItem[];
     */
    ptr_vector<CompletionItem> items;
  };

  // -----------------------------------------------------------------------
  // Completion Item Resolve Request (:leftwards_arrow_with_hook:)
  // -----------------------------------------------------------------------
  // The request is sent from the client to the server to resolve additional
  // information for a given completion item.
  //
  // Request:
  // - method: completionItem/resolve
  // - params: CompletionItem
  //
  // Response:
  // - result: CompletionItem
  // - error: code and message set in case an exception happens during the
  //   completion resolve request.
  // -----------------------------------------------------------------------

  /**
   * Diagnostics notifications are sent from the server to the client to signal
   * results of validation runs.
   *
   * Diagnostics are “owned” by the server so it is the server’s responsibility
   * to clear them if necessary. The following rule is used for VS Code servers
   * that generate diagnostics:
   * - if a language is single file only (for example HTML) then diagnostics are
   *   cleared by the server when the file is closed. Please note that open /
   *   close events don’t necessarily reflect what the user sees in the user
   *   interface. These events are ownership events. So with the current version
   *   of the specification it is possible that problems are not cleared
   *   although the file is not visible in the user interface since the client
   *   has not closed the file yet.
   * - if a language has a project system (for example C#) diagnostics are not
   *   cleared when a file closes. When a project is opened all diagnostics for
   *   all files are recomputed (or read from a cache).
   *
   * When a file changes it is the server’s responsibility to re-compute
   * diagnostics and push them to the client. If the computed set is empty it
   * has to push the empty array to clear former diagnostics. Newly pushed
   * diagnostics always replace previously pushed diagnostics. There is no
   * merging that happens on the client side.
   *
   * See also the Diagnostic section.
   *
   * Client Capability:
   * - property name (optional): textDocument.publishDiagnostics
   * - property type: PublishDiagnosticsClientCapabilities defined as follows:
   *
   * export interface PublishDiagnosticsClientCapabilities {
   *   relatedInformation?: boolean;
   *   tagSupport?: {
   *     valueSet: DiagnosticTag[];
   *   };
   *   versionSupport?: boolean;
   *   codeDescriptionSupport?: boolean;
   *   dataSupport?: boolean;
   * }
   */
  struct PublishDiagnosticsClientCapabilities {

    /**
     * Whether the clients accepts diagnostics with related information.
     *
     * relatedInformation?: boolean;
     */
    std::optional<boolean> relatedInformation;

    /**
     * Client supports the tag property to provide meta data about a diagnostic.
     * Clients supporting tags have to handle unknown tags gracefully.
     *
     * tagSupport?: {
     *   // The tags supported by the client.
     *   valueSet: DiagnosticTag[];
     * };
     *
     * @since 3.15.0
     */
    optional_ptr<ValueSet<DiagnosticTag>> tagSupport;

    /**
     * Whether the client interprets the version property of the
     * `textDocument/publishDiagnostics` notification's parameter.
     *
     * versionSupport?: boolean;
     *
     * @since 3.15.0
     */
    std::optional<boolean> versionSupport;

    /**
     * Client supports a codeDescription property
     *
     * codeDescriptionSupport?: boolean;
     *
     * @since 3.16.0
     */
    std::optional<boolean> codeDescriptionSupport;

    /**
     * Whether code action supports the `data` property which is
     * preserved between a `textDocument/publishDiagnostics` and
     * `textDocument/codeAction` request.
     *
     * dataSupport?: boolean;
     *
     * @since 3.16.0
     */
    std::optional<boolean> dataSupport;
  };

  /**
   * Notification:
   * - method: textDocument/publishDiagnostics
   * - params: PublishDiagnosticsParams defined as follows:
   *
   * interface PublishDiagnosticsParams {
   *   uri: DocumentUri;
   *   version?: integer;
   *   diagnostics: Diagnostic[];
   * }
   */
  struct PublishDiagnosticsParams {

    /**
     * The URI for which diagnostic information is reported.
     *
     * uri: DocumentUri;
     */
    DocumentUri uri;

    /**
     * Optional the version number of the document the diagnostics are published
     * for.
     *
     * version?: integer;
     *
     * @since 3.15.0
     */
    std::optional<integer> version;

    /**
     * An array of diagnostic information items.
     *
     * diagnostics: Diagnostic[];
     */
    ptr_vector<Diagnostic> diagnostics;
  };

  /**
   * Client capabilities specific to diagnostic pull requests.
   *
   * Diagnostics are currently published by the server to the client using a
   * notification. This model has the advantage that for workspace wide
   * diagnostics the server has the freedom to compute them at a server
   * preferred point in time. On the other hand the approach has the
   * disadvantage that the server can’t prioritize the computation for the file
   * in which the user types or which are visible in the editor. Inferring the
   * client’s UI state from the textDocument/didOpen and textDocument/didChange
   * notifications might lead to false positives since these notifications are
   * ownership transfer notifications.
   *
   * The specification therefore introduces the concept of diagnostic pull
   * requests to give a client more control over the documents for which
   * diagnostics should be computed and at which point in time.
   *
   * Client Capability:
   * - property name (optional): textDocument.diagnostic
   * - property type: DiagnosticClientCapabilities defined as follows:
   *
   * export interface DiagnosticClientCapabilities {
   *   dynamicRegistration?: boolean;
   *   relatedDocumentSupport?: boolean;
   * }
   *
   * @since 3.17.0
   */
  struct DiagnosticClientCapabilities {

    /**
     * Whether implementation supports dynamic registration. If this is set to
     * `true` the client supports the new `(TextDocumentRegistrationOptions &
     * StaticRegistrationOptions)` return value for the corresponding server
     * capability as well.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;

    /**
     * Whether the clients supports related documents for document diagnostic
     * pulls.
     *
     * relatedDocumentSupport?: boolean;
     */
    std::optional<boolean> relatedDocumentSupport;
  };

  /**
   * Diagnostic options.
   *
   * Server Capability:
   * - property name (optional): diagnosticProvider
   * - property type: DiagnosticOptions defined as follows:
   *
   * export interface DiagnosticOptions extends WorkDoneProgressOptions {
   *   identifier?: string;
   *   interFileDependencies: boolean;
   *   workspaceDiagnostics: boolean;
   * }
   *
   * @since 3.17.0
   */
  struct DiagnosticOptions : public WorkDoneProgressOptions {

    /**
     * An optional identifier under which the diagnostics are managed by the
     * client.
     *
     * identifier?: string;
     */
    std::optional<string> identifier;

    /**
     * Whether the language has inter file dependencies meaning that editing
     * code in one file can result in a different diagnostic set in another
     * file. Inter file dependencies are common for most programming languages
     * and typically uncommon for linters.
     *
     * interFileDependencies: boolean;
     */
    boolean interFileDependencies;

    /**
     * The server provides support for workspace diagnostics as well.
     *
     * workspaceDiagnostics: boolean;
     */
    boolean workspaceDiagnostics;
  };

  /**
   * Diagnostic registration options.
   *
   * Registration Options: DiagnosticRegistrationOptions options defined as
   * follows:
   *
   * export interface DiagnosticRegistrationOptions extends
   *   TextDocumentRegistrationOptions, DiagnosticOptions,
   *   StaticRegistrationOptions {
   * }
   *
   * @since 3.17.0
   */
  struct DiagnosticRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public DiagnosticOptions
    , public StaticRegistrationOptions {
    // empty
  };

  /**
   * Parameters of the document diagnostic request.
   *
   * The text document diagnostic request is sent from the client to the server
   * to ask the server to compute the diagnostics for a given document. As with
   * other pull requests the server is asked to compute the diagnostics for the
   * currently synced version of the document.
   *
   * Request:
   * - method: textDocument/diagnostic.
   * - params: DocumentDiagnosticParams defined as follows:
   *
   * export interface DocumentDiagnosticParams extends WorkDoneProgressParams,
   *   PartialResultParams {
   *   textDocument: TextDocumentIdentifier;
   *   identifier?: string;
   *   previousResultId?: string;
   * }
   *
   * @since 3.17.0
   */
  struct DocumentDiagnosticParams
    : public WorkDoneProgressParams
    , public PartialResultParams {

    /**
     * The text document.
     *
     * textDocument: TextDocumentIdentifier;
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;

    /**
     * The additional identifier  provided during registration.
     *
     * identifier?: string;
     */
    std::optional<string> identifier;

    /**
     * The result id of a previous response if provided.
     *
     * previousResultId?: string;
     */
    std::optional<string> previousResultId;
  };

  enum class DocumentDiagnosticReportType {
    RELATED_FULL_DOCUMENT_DIAGNOSTIC_REPORT,
    RELATED_UNCHANGED_DOCUMENT_DIAGNOSTIC_REPORT,
  };

  /**
   * The document diagnostic report kinds.
   *
   * export namespace DocumentDiagnosticReportKind {
   *   export const Full = 'full';
   *   export const Unchanged = 'unchanged';
   * }
   * export type DocumentDiagnosticReportKind = 'full' | 'unchanged';
   *
   * @since 3.17.0
   */
  enum class DocumentDiagnosticReportKind {

    /**
     * A diagnostic report with a full set of problems.
     *
     * export const Full = 'full';
     */
    Full,

    /**
     * A report indicating that the last returned report is still accurate.
     *
     * export const Unchanged = 'unchanged';
     */
    Unchanged,
  };

  extern std::map<DocumentDiagnosticReportKind, std::string> DocumentDiagnosticReportKindNames;

  auto documentDiagnosticReportKindByName(const std::string &name) -> DocumentDiagnosticReportKind;

  extern std::map<DocumentDiagnosticReportKind, std::string> DocumentDiagnosticReportKindValues;

  auto documentDiagnosticReportKindByValue(const std::string &value) -> DocumentDiagnosticReportKind;

  /**
   * A diagnostic report with a full set of problems.
   *
   * export interface FullDocumentDiagnosticReport {
   *   kind: DocumentDiagnosticReportKind.Full;
   *   resultId?: string;
   *   items: Diagnostic[];
   * }
   *
   * @since 3.17.0
   */
  struct FullDocumentDiagnosticReport {

    /**
     * A full document diagnostic report.
     *
     * kind: DocumentDiagnosticReportKind.Full;
     */
    const DocumentDiagnosticReportKind kind = DocumentDiagnosticReportKind::Full;

    /**
     * An optional result id. If provided it will be sent on the next diagnostic
     * request for the same document.
     *
     * resultId?: string;
     */
    std::optional<string> resultId;

    /**
     * The actual items.
     *
     * items: Diagnostic[];
     */
    ptr_vector<Diagnostic> items;
  };

  /**
   * A diagnostic report indicating that the last returned
   * report is still accurate.
   *
   * export interface UnchangedDocumentDiagnosticReport {
   *   kind: DocumentDiagnosticReportKind.Unchanged;
   *   resultId: string;
   * }
   *
   * @since 3.17.0
   */
  struct UnchangedDocumentDiagnosticReport {

    /**
     * A document diagnostic report indicating no changes to the last result. A
     * server can only return `unchanged` if result ids are provided.
     *
     * kind: DocumentDiagnosticReportKind.Unchanged;
     */
    const DocumentDiagnosticReportKind kind = DocumentDiagnosticReportKind::Unchanged;

    /**
     * A result id which will be sent on the next diagnostic request for the
     * same document.
     *
     * resultId: string;
     */
    string resultId;
  };

  enum class FullOrUnchangedDocumentDiagnosticReportType {
    FULL_DOCUMENT_DIAGNOSTIC_REPORT,
    UNCHANGED_DOCUMENT_DIAGNOSTIC_REPORT,
  };

  struct FullOrUnchangedDocumentDiagnosticReport {
    FullOrUnchangedDocumentDiagnosticReportType type;
    std::variant<
      std::unique_ptr<FullDocumentDiagnosticReport>,
      std::unique_ptr<UnchangedDocumentDiagnosticReport>
    > value;
  };

  /**
   * A full diagnostic report with a set of related documents.
   *
   * export interface RelatedFullDocumentDiagnosticReport extends
   *   FullDocumentDiagnosticReport {
   *   relatedDocuments?: {
   *     [uri: string]:
   *       FullDocumentDiagnosticReport | UnchangedDocumentDiagnosticReport;
   *   };
   * }
   *
   * @since 3.17.0
   */
  struct RelatedFullDocumentDiagnosticReport : public FullDocumentDiagnosticReport {

    /**
     * Diagnostics of related documents. This information is useful
     * in programming languages where code in a file A can generate
     * diagnostics in a file B which A depends on. An example of
     * such a language is C/C++ where macro definitions in a file
     * a.cpp and result in errors in a header file b.hpp.
     *
     * relatedDocuments?: {
     *   [uri: string]:  // DocumentUri
     *     FullDocumentDiagnosticReport | UnchangedDocumentDiagnosticReport;
     * };
     *
     * @since 3.17.0
     */
    optional_ptr<std::map<string, FullOrUnchangedDocumentDiagnosticReport>> relatedDocuments;
  };

  /**
   * An unchanged diagnostic report with a set of related documents.
   *
   * export interface RelatedUnchangedDocumentDiagnosticReport extends
   *   UnchangedDocumentDiagnosticReport {
   *   relatedDocuments?: {
   *     [uri: string]:
   *       FullDocumentDiagnosticReport | UnchangedDocumentDiagnosticReport;
   *   };
   * }
   *
   * @since 3.17.0
   */
  struct RelatedUnchangedDocumentDiagnosticReport : public UnchangedDocumentDiagnosticReport {

    /**
     * Diagnostics of related documents. This information is useful
     * in programming languages where code in a file A can generate
     * diagnostics in a file B which A depends on. An example of
     * such a language is C/C++ where macro definitions in a file
     * a.cpp and result in errors in a header file b.hpp.
     *
     * relatedDocuments?: {
     *   [uri: string]:  // DocumentUri
     *     FullDocumentDiagnosticReport | UnchangedDocumentDiagnosticReport;
     * };
     *
     * @since 3.17.0
     */
    optional_ptr<std::map<string, FullOrUnchangedDocumentDiagnosticReport>> relatedDocuments;
  };

  /**
   * The result of a document diagnostic pull request. A report can either be a
   * full report containing all diagnostics for the requested document or a
   * unchanged report indicating that nothing has changed in terms of
   * diagnostics in comparison to the last pull request.
   *
   * Response:
   * - result: DocumentDiagnosticReport defined as follows:
   *
   * export type DocumentDiagnosticReport = RelatedFullDocumentDiagnosticReport
   *   | RelatedUnchangedDocumentDiagnosticReport;
   *
   * - partial result: The first literal send need to be a
   *   DocumentDiagnosticReport followed by n
   *   DocumentDiagnosticReportPartialResult literals defined as follows:
   *
   * export interface DocumentDiagnosticReportPartialResult {
   *   relatedDocuments: {
   *     [uri: string]:
   *       FullDocumentDiagnosticReport | UnchangedDocumentDiagnosticReport;
   *   };
   * }
   *
   * - error: code and message set in case an exception happens during the
   *   diagnostic request. A server is also allowed to return an error with code
   *   ServerCancelled indicating that the server can’t compute the result right
   *   now. A server can return a DiagnosticServerCancellationData data to
   *   indicate whether the client should re-trigger the request. If no data is
   *   provided it defaults to { retriggerRequest: true }:
   *
   * export interface DiagnosticServerCancellationData {
   *   retriggerRequest: boolean;
   * }
   *
   * @since 3.17.0
   */
  struct DocumentDiagnosticReport {
    DocumentDiagnosticReportType type;
    std::variant<
      std::unique_ptr<RelatedFullDocumentDiagnosticReport>,
      std::unique_ptr<RelatedUnchangedDocumentDiagnosticReport>
    > value;
  };

  /**
   * A partial result for a document diagnostic report.
   *
   * export interface DocumentDiagnosticReportPartialResult {
   *   relatedDocuments: {
   *     [uri: string]:
   *       FullDocumentDiagnosticReport | UnchangedDocumentDiagnosticReport;
   *   };
   * }
   *
   * @since 3.17.0
   */
  struct DocumentDiagnosticReportPartialResult {

    /**
     * relatedDocuments: {
     *   [uri: string]:  // DocumentUri
     *     FullDocumentDiagnosticReport | UnchangedDocumentDiagnosticReport;
     * };
     */
    std::unique_ptr<std::map<string, FullOrUnchangedDocumentDiagnosticReport>> relatedDocuments;
  };

  /**
   * Cancellation data returned from a diagnostic request.
   *
   * export interface DiagnosticServerCancellationData {
   *   retriggerRequest: boolean;
   * }
   *
   * @since 3.17.0
   */
  struct DiagnosticServerCancellationData {

    /**
     * retriggerRequest: boolean;
     */
    boolean retriggerRequest;
  };

  /**
   * A previous result id in a workspace pull request.
   *
   * export interface PreviousResultId {
   *   uri: DocumentUri;
   *   value: string;
   * }
   *
   * @since 3.17.0
   */
  struct PreviousResultId {

    /**
     * The URI for which the client knows a result id.
     *
     * uri: DocumentUri;
     */
    DocumentUri uri;

    /**
     * The value of the previous result id.
     *
     * value: string;
     */
    string value;
  };

  /**
   * A full document diagnostic report for a workspace diagnostic result.
   *
   * export interface WorkspaceFullDocumentDiagnosticReport extends
   *   FullDocumentDiagnosticReport {
   *   uri: DocumentUri;
   *   version: integer | null;
   * }
   *
   * @since 3.17.0
   */
  struct WorkspaceFullDocumentDiagnosticReport
    : public FullDocumentDiagnosticReport {

    /**
     * The URI for which diagnostic information is reported.
     *
     * uri: DocumentUri;
     */
    DocumentUri uri;

    /**
     * The version number for which the diagnostics are reported. If the
     * document is not marked as open `null` can be provided.
     *
     * version: integer | null;
     */
    std::optional<integer> version;
  };

  /**
   * An unchanged document diagnostic report for a workspace diagnostic result.
   *
   * export interface WorkspaceUnchangedDocumentDiagnosticReport extends
   *   UnchangedDocumentDiagnosticReport {
   *   uri: DocumentUri;
   *   version: integer | null;
   * };
   *
   * @since 3.17.0
   */
  struct WorkspaceUnchangedDocumentDiagnosticReport
    : public UnchangedDocumentDiagnosticReport {

    /**
     * The URI for which diagnostic information is reported.
     *
     * uri: DocumentUri;
     */
    DocumentUri uri;

    /**
     * The version number for which the diagnostics are reported. If the
     * document is not marked as open `null` can be provided.
     *
     * version: integer | null;
     */
    std::optional<integer> version;
  };

  enum class WorkspaceDocumentDiagnosticReportType {
    WORKSPACE_FULL_DOCUMENT_DIAGNOSTIC_REPORT,
    WORKSPACE_UNCHANGED_DOCUMENT_DIAGNOSTIC_REPORT,
  };

  /**
   * A workspace diagnostic document report.
   *
   * export type WorkspaceDocumentDiagnosticReport =
   *   WorkspaceFullDocumentDiagnosticReport
   *   | WorkspaceUnchangedDocumentDiagnosticReport;
   *
   * @since 3.17.0
   */
  struct WorkspaceDocumentDiagnosticReport {
    WorkspaceDocumentDiagnosticReportType type;
    std::variant<
      std::unique_ptr<WorkspaceFullDocumentDiagnosticReport>,
      std::unique_ptr<WorkspaceUnchangedDocumentDiagnosticReport>
    > value;
  };

  /**
   * A workspace diagnostic report.
   *
   * Response:
   * - result: WorkspaceDiagnosticReport defined as follows:
   *
   * export interface WorkspaceDiagnosticReport {
   *   items: WorkspaceDocumentDiagnosticReport[];
   * }
   *
   * - partial result: The first literal send need to be a
   *   WorkspaceDiagnosticReport followed by n
   *   WorkspaceDiagnosticReportPartialResult literals.
   * - error: code and message set in case an exception happens during the
   *   diagnostic request. A server is also allowed to return and error with
   *   code ServerCancelled indicating that the server can’t compute the result
   *   right now. A server can return a DiagnosticServerCancellationData data to
   *   indicate whether the client should re-trigger the request. If no data is
   *   provided it defaults to { retriggerRequest: true }
   *
   * @since 3.17.0
   */
  struct WorkspaceDiagnosticReport {

    /**
     * items: WorkspaceDocumentDiagnosticReport[];
     */
    ptr_vector<WorkspaceDocumentDiagnosticReport> items;
  };

  /**
   * A partial result for a workspace diagnostic report.
   *
   * export interface WorkspaceDiagnosticReportPartialResult {
   *   items: WorkspaceDocumentDiagnosticReport[];
   * }
   *
   * @since 3.17.0
   */
  struct WorkspaceDiagnosticReportPartialResult {

    /**
     * items: WorkspaceDocumentDiagnosticReport[];
     */
    ptr_vector<WorkspaceDocumentDiagnosticReport> items;
  };

  /**
   * Parameters of the workspace diagnostic request.
   *
   * The workspace diagnostic request is sent from the client to the server to
   * ask the server to compute workspace wide diagnostics which previously where
   * pushed from the server to the client. In contrast to the document
   * diagnostic request the workspace request can be long running and is not
   * bound to a specific workspace or document state. If the client supports
   * streaming for the workspace diagnostic pull it is legal to provide a
   * document diagnostic report multiple times for the same document URI. The
   * last one reported will win over previous reports.
   *
   * If a client receives a diagnostic report for a document in a workspace
   * diagnostic request for which the client also issues individual document
   * diagnostic pull requests the client needs to decide which diagnostics win
   * and should be presented. In general:
   * - diagnostics for a higher document version should win over those from a
   *   lower document version (e.g. note that document versions are steadily
   *   increasing)
   * - diagnostics from a document pull should win over diagnostics from a
   *   workspace pull.
   *
   * Request:
   * - method: workspace/diagnostic.
   * - params: WorkspaceDiagnosticParams defined as follows:
   *
   * export interface WorkspaceDiagnosticParams extends WorkDoneProgressParams,
   *   PartialResultParams {
   *   identifier?: string;
   *   previousResultIds: PreviousResultId[];
   * }
   *
   * @since 3.17.0
   */
  struct WorkspaceDiagnosticParams
    : public WorkDoneProgressParams
    , public PartialResultParams {

    /**
     * The additional identifier provided during registration.
     *
     * identifier?: string;
     */
    std::optional<string> identifier;

    /**
     * The currently known diagnostic reports with their previous result ids.
     *
     * previousResultIds: PreviousResultId[];
     */
    ptr_vector<PreviousResultId> previousResultIds;
  };

  /**
   * Workspace client capabilities specific to diagnostic pull requests.
   *
   * The workspace/diagnostic/refresh request is sent from the server to the
   * client. Servers can use it to ask clients to refresh all needed document
   * and workspace diagnostics. This is useful if a server detects a project
   * wide configuration change which requires a re-calculation of all
   * diagnostics.
   *
   * Client Capability:
   * - property name (optional): workspace.diagnostics
   * - property type: DiagnosticWorkspaceClientCapabilities defined as follows:
   *
   * export interface DiagnosticWorkspaceClientCapabilities {
   *   refreshSupport?: boolean;
   * }
   *
   * Request:
   * - method: workspace/diagnostic/refresh
   * - params: none
   *
   * Response:
   * - result: void
   * - error: code and message set in case an exception happens during the
   *   ‘workspace/diagnostic/refresh’ request
   *
   * Implementation Considerations
   * -----------------------------
   *
   * Generally the language server specification doesn’t enforce any specific
   * client implementation since those usually depend on how the client UI
   * behaves. However since diagnostics can be provided on a document and
   * workspace level here are some tips:
   * - a client should pull actively for the document the users types in.
   * - if the server signals inter file dependencies a client should also pull
   *   for visible documents to ensure accurate diagnostics. However the pull
   *   should happen less frequently.
   * - if the server signals workspace pull support a client should also pull
   *   for workspace diagnostics. It is recommended for clients to implement
   *   partial result progress for the workspace pull to allow servers to keep
   *   the request open for a long time. If a server closes a workspace
   *   diagnostic pull request the client should re-trigger the request.
   *
   * @since 3.17.0
   */
  struct DiagnosticWorkspaceClientCapabilities {

    /**
     * Whether the client implementation supports a refresh request sent from
     * the server to the client.
     *
     * Note that this event is global and will force the client to refresh all
     * pulled diagnostics currently shown. It should be used with absolute care
     * and is useful for situation where a server for example detects a project
     * wide change that requires such a calculation.
     *
     * refreshSupport?: boolean;
     */
    std::optional<boolean> refreshSupport;
  };

  struct ParameterInformationCapabilities {

    /**
     * The client supports processing label offsets instead of a
     * simple label string.
     *
     * labelOffsetSupport?: boolean;
     *
     * @since 3.14.0
     */
    std::optional<boolean> labelOffsetSupport;
  };

  struct SignatureInformationCapabilities {

    /**
     * Client supports the follow content formats for the documentation
     * property. The order describes the preferred format of the client.
     *
     * documentationFormat?: MarkupKind[];
     */
    optional_vector<MarkupKind> documentationFormat;

    /**
     * Client capabilities specific to parameter information.
     *
     * parameterInformation?: {
     *   labelOffsetSupport?: boolean;
     * };
     */
    optional_ptr<ParameterInformationCapabilities> parameterInformation;

    /**
     * The client supports the `activeParameter` property on
     * `SignatureInformation` literal.
     *
     * activeParameterSupport?: boolean;
     *
     * @since 3.16.0
     */
    std::optional<boolean> activeParameterSupport;
  };

  /**
   * The signature help request is sent from the client to the server to request
   * signature information at a given cursor position.
   *
   * Client Capability:
   * - property name (optional): textDocument.signatureHelp
   * - property type: SignatureHelpClientCapabilities defined as follows:
   *
   * export interface SignatureHelpClientCapabilities {
   *   dynamicRegistration?: boolean;
   *   signatureInformation?: {
   *     documentationFormat?: MarkupKind[];
   *     parameterInformation?: {
   *       labelOffsetSupport?: boolean;
   *     };
   *     activeParameterSupport?: boolean;
   *   };
   *   contextSupport?: boolean;
   * }
   */
  struct SignatureHelpClientCapabilities {

    /**
     * Whether signature help supports dynamic registration.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;

    /**
     * The client supports the following `SignatureInformation`
     * specific properties.
     *
     * signatureInformation?: {
     *   documentationFormat?: MarkupKind[];
     *   parameterInformation?: {
     *     labelOffsetSupport?: boolean;
     *   };
     *   activeParameterSupport?: boolean;
     * };
     */
    optional_ptr<SignatureInformationCapabilities> signatureInformation;

    /**
     * The client supports to send additional context information for a
     * `textDocument/signatureHelp` request. A client that opts into
     * contextSupport will also support the `retriggerCharacters` on
     * `SignatureHelpOptions`.
     *
     * contextSupport?: boolean;
     *
     * @since 3.15.0
     */
    std::optional<boolean> contextSupport;
  };

  /**
   * Server Capability:
   * - property name (optional): signatureHelpProvider
   * - property type: SignatureHelpOptions defined as follows:
   *
   * export interface SignatureHelpOptions extends WorkDoneProgressOptions {
   *   triggerCharacters?: string[];
   *   retriggerCharacters?: string[];
   * }
   */
  struct SignatureHelpOptions : public WorkDoneProgressOptions {

    /**
     * The characters that trigger signature help automatically.
     *
     * triggerCharacters?: string[];
     */
    optional_vector<string> triggerCharacters;

    /**
     * List of characters that re-trigger signature help.
     *
     * These trigger characters are only active when signature help is already
     * showing. All trigger characters are also counted as re-trigger
     * characters.
     *
     * retriggerCharacters?: string[];
     *
     * @since 3.15.0
     */
    optional_vector<string> retriggerCharacters;
  };

  /**
   * Registration Options: SignatureHelpRegistrationOptions defined as follows:
   *
   * export interface SignatureHelpRegistrationOptions
   *   extends TextDocumentRegistrationOptions, SignatureHelpOptions {
   * }
   */
  struct SignatureHelpRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public SignatureHelpOptions {
    // empty
  };

  /**
   * How a signature help was triggered.
   *
   * export namespace SignatureHelpTriggerKind {
   *   export const Invoked: 1 = 1;
   *   export const TriggerCharacter: 2 = 2;
   *   export const ContentChange: 3 = 3;
   * }
   * export type SignatureHelpTriggerKind = 1 | 2 | 3;
   *
   * @since 3.15.0
   */
  enum class SignatureHelpTriggerKind {

    /**
     * Signature help was invoked manually by the user or by a command.
     *
     * export const Invoked: 1 = 1;
     */
    Invoked = 1,

    /**
     * Signature help was triggered by a trigger character.
     *
     * export const TriggerCharacter: 2 = 2;
     */
    TriggerCharacter = 2,

    /**
     * Signature help was triggered by the cursor moving or by the document
     * content changing.
     *
     * export const ContentChange: 3 = 3;
     */
    ContentChange = 3,
  };

  extern std::map<SignatureHelpTriggerKind, std::string> SignatureHelpTriggerKindNames;

  auto signatureHelpTriggerKindByName(const std::string &name) -> SignatureHelpTriggerKind;

  enum class StringOrUintegerPairType {
    STRING,
    UINTEGER_PAIR,
  };

  struct StringOrUintegerPair {
    StringOrUintegerPairType type;
    std::variant<
      string,
      std::unique_ptr<std::pair<uinteger, uinteger>>
    > value;
  };

  /**
   * Represents a parameter of a callable-signature. A parameter can have a
   * label and a doc-comment.
   *
   * export interface ParameterInformation {
   *   label: string | [uinteger, uinteger];
   *   documentation?: string | MarkupContent;
   * }
   */
  struct ParameterInformation {

    /**
     * The label of this parameter information.
     *
     * Either a string or an inclusive start and exclusive end offsets within
     * its containing signature label. (see SignatureInformation.label). The
     * offsets are based on a UTF-16 string representation as `Position` and
     * `Range` does.
     *
     * *Note*: a label of type string should be a substring of its containing
     * signature label. Its intended use case is to highlight the parameter
     * label part in the `SignatureInformation.label`.
     *
     * label: string | [uinteger, uinteger];
     */
    std::unique_ptr<StringOrUintegerPair> label;

    /**
     * The human-readable doc-comment of this parameter. Will be shown in the UI
     * but can be omitted.
     *
     * documentation?: string | MarkupContent;
     */
    optional_ptr<StringOrMarkupContent> documentation;
  };

  /**
   * Represents the signature of something callable. A signature can have a
   * label, like a function-name, a doc-comment, and a set of parameters.
   *
   * export interface SignatureInformation {
   *   label: string;
   *   documentation?: string | MarkupContent;
   *   parameters?: ParameterInformation[];
   *   activeParameter?: uinteger;
   * }
   */
  struct SignatureInformation {

    /**
     * The label of this signature. Will be shown in the UI.
     *
     * label: string;
     */
    string label;

    /**
     * The human-readable doc-comment of this signature. Will be shown in the UI
     * but can be omitted.
     *
     * documentation?: string | MarkupContent;
     */
    optional_ptr<StringOrMarkupContent> documentation;

    /**
     * The parameters of this signature.
     *
     * parameters?: ParameterInformation[];
     */
    optional_ptr_vector<ParameterInformation> parameters;

    /**
     * The index of the active parameter.
     *
     * If provided, this is used in place of `SignatureHelp.activeParameter`.
     *
     * activeParameter?: uinteger;
     *
     * @since 3.16.0
     */
    std::optional<uinteger> activeParameter;
  };

  /**
   * Signature help represents the signature of something callable. There can be
   * multiple signature but only one active and only one active parameter.
   *
   * Response:
   * - result: SignatureHelp | null defined as follows:
   *
   * export interface SignatureHelp {
   *   signatures: SignatureInformation[];
   *   activeSignature?: uinteger;
   *   activeParameter?: uinteger;
   * }
   *
   * - error: code and message set in case an exception happens during the
   *   signature help request.
   */
  struct SignatureHelp {

    /**
     * One or more signatures. If no signatures are available the signature help
     * request should return `null`.
     *
     * signatures: SignatureInformation[];
     */
    ptr_vector<SignatureInformation> signatures;

    /**
     * The active signature. If omitted or the value lies outside the range of
     * `signatures` the value defaults to zero or is ignore if the
     * `SignatureHelp` as no signatures.
     *
     * Whenever possible implementors should make an active decision about the
     * active signature and shouldn't rely on a default value.
     *
     * In future version of the protocol this property might become mandatory to
     * better express this.
     *
     * activeSignature?: uinteger;
     */
    std::optional<uinteger> activeSignature;

    /**
     * The active parameter of the active signature. If omitted or the value
     * lies outside the range of `signatures[activeSignature].parameters`
     * defaults to 0 if the active signature has parameters. If the active
     * signature has no parameters it is ignored. In future version of the
     * protocol this property might become mandatory to better express the
     * active parameter if the active signature does have any.
     *
     * activeParameter?: uinteger;
     */
    std::optional<uinteger> activeParameter;
  };

  /**
   * Additional information about the context in which a signature help request
   * was triggered.
   *
   * export interface SignatureHelpContext {
   *   triggerKind: SignatureHelpTriggerKind;
   *   triggerCharacter?: string;
   *   isRetrigger: boolean;
   *   activeSignatureHelp?: SignatureHelp;
   * }
   *
   * @since 3.15.0
   */
  struct SignatureHelpContext {

    /**
     * Action that caused signature help to be triggered.
     *
     * triggerKind: SignatureHelpTriggerKind;
     */
    SignatureHelpTriggerKind triggerKind;

    /**
     * Character that caused signature help to be triggered.
     *
     * This is undefined when triggerKind !==
     * SignatureHelpTriggerKind.TriggerCharacter
     *
     * triggerCharacter?: string;
     */
    std::optional<string> triggerCharacter;

    /**
     * `true` if signature help was already showing when it was triggered.
     *
     * Retriggers occur when the signature help is already active and can be
     * caused by actions such as typing a trigger character, a cursor move, or
     * document content changes.
     *
     * isRetrigger: boolean;
     */
    boolean isRetrigger;

    /**
     * The currently active `SignatureHelp`.
     *
     * The `activeSignatureHelp` has its `SignatureHelp.activeSignature` field
     * updated based on the user navigating through available signatures.
     *
     * activeSignatureHelp?: SignatureHelp;
     */
    optional_ptr<SignatureHelp> activeSignatureHelp;
  };

  /**
   * Request:
   * - method: textDocument/signatureHelp
   * - params: SignatureHelpParams defined as follows:
   *
   * export interface SignatureHelpParams extends TextDocumentPositionParams,
   *   WorkDoneProgressParams {
   *   context?: SignatureHelpContext;
   * }
   */
  struct SignatureHelpParams
    : public TextDocumentPositionParams
    , public WorkDoneProgressParams {

    /**
     * The signature help context. This is only available if the client
     * specifies to send this using the client capability
     * `textDocument.signatureHelp.contextSupport === true`
     *
     * context?: SignatureHelpContext;
     *
     * @since 3.15.0
     */
    optional_ptr<SignatureHelpContext> context;
  };

  /**
   * The kind of a code action.
   *
   * Kinds are a hierarchical list of identifiers separated by `.`, e.g.
   * `"refactor.extract.function"`.
   *
   * The set of kinds is open and client needs to announce the kinds it supports
   * to the server during initialization.
   *
   * export type CodeActionKind = string;
   * export namespace CodeActionKind {
   *   export const Empty: CodeActionKind = '';
   *   export const QuickFix: CodeActionKind = 'quickfix';
   *   export const Refactor: CodeActionKind = 'refactor';
   *   export const RefactorExtract: CodeActionKind = 'refactor.extract';
   *   export const RefactorInline: CodeActionKind = 'refactor.inline';
   *   export const RefactorRewrite: CodeActionKind = 'refactor.rewrite';
   *   export const Source: CodeActionKind = 'source';
   *   export const SourceOrganizeImports: CodeActionKind =
   *     'source.organizeImports';
   *   export const SourceFixAll: CodeActionKind = 'source.fixAll';
   * }
   */
  enum class CodeActionKind {

    /**
     * Empty kind.
     *
     * export const Empty: CodeActionKind = '';
     */
    Empty,

    /**
     * Base kind for quickfix actions: 'quickfix'.
     *
     * export const QuickFix: CodeActionKind = 'quickfix';
     */
    QuickFix,

    /**
     * Base kind for refactoring actions: 'refactor'.
     *
     * export const Refactor: CodeActionKind = 'refactor';
     */
    Refactor,

    /**
     * Base kind for refactoring extraction actions: 'refactor.extract'.
     *
     * Example extract actions:
     *
     * - Extract method
     * - Extract function
     * - Extract variable
     * - Extract interface from class
     * - ...
     *
     * export const RefactorExtract: CodeActionKind = 'refactor.extract';
     */
    RefactorExtract,

    /**
     * Base kind for refactoring inline actions: 'refactor.inline'.
     *
     * Example inline actions:
     *
     * - Inline function
     * - Inline variable
     * - Inline constant
     * - ...
     *
     * export const RefactorInline: CodeActionKind = 'refactor.inline';
     */
    RefactorInline,

    /**
     * Base kind for refactoring rewrite actions: 'refactor.rewrite'.
     *
     * Example rewrite actions:
     *
     * - Convert JavaScript function to class
     * - Add or remove parameter
     * - Encapsulate field
     * - Make method static
     * - Move method to base class
     * - ...
     *
     * export const RefactorRewrite: CodeActionKind = 'refactor.rewrite';
     */
    RefactorRewrite,

    /**
     * Base kind for source actions: `source`.
     *
     * Source code actions apply to the entire file.
     *
     * export const Source: CodeActionKind = 'source';
     */
    Source,

    /**
     * Base kind for an organize imports source action:
     * `source.organizeImports`.
     *
     * export const SourceOrganizeImports: CodeActionKind =
     *   'source.organizeImports';
     */
    SourceOrganizeImports,

    /**
     * Base kind for a 'fix all' source action: `source.fixAll`.
     *
     * 'Fix all' actions automatically fix errors that have a clear fix that do
     * not require user input. They should not suppress errors or perform unsafe
     * fixes such as generating new types or classes.
     *
     * export const SourceFixAll: CodeActionKind = 'source.fixAll';
     *
     * @since 3.17.0
     */
    SourceFixAll,
  };

  extern std::map<CodeActionKind, std::string> CodeActionKindNames;

  auto codeActionKindByName(const std::string &name) -> CodeActionKind;

  extern std::map<CodeActionKind, std::string> CodeActionKindValues;

  auto codeActionKindByValue(const std::string &value) -> CodeActionKind;

  struct CodeActionLiteralSupportCapabilities {

    /**
     * The code action kind is supported with the following value
     * set.
     *
     * codeActionKind: {
     *   // The code action kind values the client supports. When this
     *   // property exists the client also guarantees that it will
     *   // handle values outside its set gracefully and falls back
     *   // to a default value when unknown.
     *   valueSet: CodeActionKind[];
     * };
     */
    std::unique_ptr<ValueSet<CodeActionKind>> codeActionKind;
  };

  /**
   * The code action request is sent from the client to the server to compute
   * commands for a given text document and range. These commands are typically
   * code fixes to either fix problems or to beautify/refactor code. The result
   * of a textDocument/codeAction request is an array of Command literals which
   * are typically presented in the user interface. To ensure that a server is
   * useful in many clients the commands specified in a code actions should be
   * handled by the server and not by the client (see workspace/executeCommand
   * and ServerCapabilities.executeCommandProvider). If the client supports
   * providing edits with a code action then that mode should be used.
   *
   * Since version 3.16.0: a client can offer a server to delay the computation
   * of code action properties during a ‘textDocument/codeAction’ request:
   *
   * This is useful for cases where it is expensive to compute the value of a
   * property (for example the edit property). Clients signal this through the
   * codeAction.resolveSupport capability which lists all properties a client
   * can resolve lazily. The server capability
   * codeActionProvider.resolveProvider signals that a server will offer a
   * codeAction/resolve route. To help servers to uniquely identify a code
   * action in the resolve request, a code action literal can optional carry a
   * data property. This is also guarded by an additional client capability
   * codeAction.dataSupport. In general, a client should offer data support if
   * it offers resolve support. It should also be noted that servers shouldn’t
   * alter existing attributes of a code action in a codeAction/resolve request.
   *
   * Since version 3.8.0: support for CodeAction literals to enable the
   * following scenarios:
   * - the ability to directly return a workspace edit from the code action
   *   request. This avoids having another server roundtrip to execute an actual
   *   code action. However server providers should be aware that if the code
   *   action is expensive to compute or the edits are huge it might still be
   *   beneficial if the result is simply a command and the actual edit is only
   *   computed when needed.
   * - the ability to group code actions using a kind. Clients are allowed to
   *   ignore that information. However it allows them to better group code
   *   action for example into corresponding menus (e.g. all refactor code
   *   actions into a refactor menu).
   *
   * Clients need to announce their support for code action literals (e.g.
   * literals of type CodeAction) and code action kinds via the corresponding
   * client capability codeAction.codeActionLiteralSupport.
   *
   * Client Capability:
   * - property name (optional): textDocument.codeAction
   * - property type: CodeActionClientCapabilities defined as follows:
   *
   * export interface CodeActionClientCapabilities {
   *   dynamicRegistration?: boolean;
   *   codeActionLiteralSupport?: {
   *     codeActionKind: {
   *       valueSet: CodeActionKind[];
   *     };
   *   };
   *   isPreferredSupport?: boolean;
   *   disabledSupport?: boolean;
   *   dataSupport?: boolean;
   *   resolveSupport?: {
   *     properties: string[];
   *   };
   *   honorsChangeAnnotations?: boolean;
   * }
   */
  struct CodeActionClientCapabilities {

    /**
     * Whether code action supports dynamic registration.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;

    /**
     * The client supports code action literals as a valid
     * response of the `textDocument/codeAction` request.
     *
     * codeActionLiteralSupport?: {
     *   codeActionKind: {
     *     valueSet: CodeActionKind[];
     *   };
     * };
     *
     * @since 3.8.0
     */
    optional_ptr<CodeActionLiteralSupportCapabilities> codeActionLiteralSupport;

    /**
     * Whether code action supports the `isPreferred` property.
     *
     * isPreferredSupport?: boolean;
     *
     * @since 3.15.0
     */
    std::optional<boolean> isPreferredSupport;

    /**
     * Whether code action supports the `disabled` property.
     *
     * disabledSupport?: boolean;
     *
     * @since 3.16.0
     */
    std::optional<boolean> disabledSupport;

    /**
     * Whether code action supports the `data` property which is preserved
     * between a `textDocument/codeAction` and a `codeAction/resolve` request.
     *
     * dataSupport?: boolean;
     *
     * @since 3.16.0
     */
    std::optional<boolean> dataSupport;

    /**
     * Whether the client supports resolving additional code action
     * properties via a separate `codeAction/resolve` request.
     *
     * resolveSupport?: {
     *   properties: string[];
     * };
     *
     * @since 3.16.0
     */
    optional_ptr<ResolveSupportCapabilities> resolveSupport;

    /**
     * Whether the client honors the change annotations in text edits and
     * resource operations returned via the `CodeAction#edit` property by for
     * example presenting the workspace edit in the user interface and asking
     * for confirmation.
     *
     * honorsChangeAnnotations?: boolean;
     *
     * @since 3.16.0
     */
    std::optional<boolean> honorsChangeAnnotations;
  };

  /**
   * Server Capability:
   * - property name (optional): codeActionProvider
   * - property type: boolean | CodeActionOptions where CodeActionOptions is
   *   defined as follows:
   *
   * export interface CodeActionOptions extends WorkDoneProgressOptions {
   *   codeActionKinds?: CodeActionKind[];
   *   resolveProvider?: boolean;
   * }
   */
  struct CodeActionOptions : public WorkDoneProgressOptions {

    /**
     * CodeActionKinds that this server may return.
     *
     * The list of kinds may be generic, such as `CodeActionKind.Refactor`, or
     * the server may list out every specific kind they provide.
     *
     * codeActionKinds?: CodeActionKind[];
     */
    optional_vector<CodeActionKind> codeActionKinds;

    /**
     * The server provides support to resolve additional information for a code
     * action.
     *
     * resolveProvider?: boolean;
     *
     * @since 3.16.0
     */
    std::optional<boolean> resolveProvider;
  };

  /**
   * Registration Options: CodeActionRegistrationOptions defined as follows:
   *
   * export interface CodeActionRegistrationOptions extends
   *   TextDocumentRegistrationOptions, CodeActionOptions {
   * }
   */
  struct CodeActionRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public CodeActionOptions {
    // empty
  };

  /**
   * The reason why code actions were requested.
   *
   * export namespace CodeActionTriggerKind {
   *   export const Invoked: 1 = 1;
   *   export const Automatic: 2 = 2;
   * }
   * export type CodeActionTriggerKind = 1 | 2;
   *
   * @since 3.17.0
   */
  enum class CodeActionTriggerKind {

    /**
     * Code actions were explicitly requested by the user or by an extension.
     *
     * export const Invoked: 1 = 1;
     */
    Invoked = 1,

    /**
     * Code actions were requested automatically.
     *
     * This typically happens when current selection in a file changes, but can
     * also be triggered when file content changes.
     *
     * export const Automatic: 2 = 2;
     */
    Automatic = 2,
  };

  extern std::map<CodeActionTriggerKind, std::string> CodeActionTriggerKindNames;

  auto codeActionTriggerKindByName(const std::string &name) -> CodeActionTriggerKind;

  /**
   * Contains additional diagnostic information about the context in which a
   * code action is run.
   *
   * export interface CodeActionContext {
   *   diagnostics: Diagnostic[];
   *   only?: CodeActionKind[];
   *   triggerKind?: CodeActionTriggerKind;
   * }
   */
  struct CodeActionContext {

    /**
     * An array of diagnostics known on the client side overlapping the range
     * provided to the `textDocument/codeAction` request. They are provided so
     * that the server knows which errors are currently presented to the user
     * for the given range. There is no guarantee that these accurately reflect
     * the error state of the resource. The primary parameter to compute code
     * actions is the provided range.
     *
     * diagnostics: Diagnostic[];
     */
    ptr_vector<Diagnostic> diagnostics;

    /**
     * Requested kind of actions to return.
     *
     * Actions not of this kind are filtered out by the client before being
     * shown. So servers can omit computing them.
     *
     * only?: CodeActionKind[];
     */
    optional_vector<CodeActionKind> only;

    /**
     * The reason why code actions were requested.
     *
     * triggerKind?: CodeActionTriggerKind;
     *
     * @since 3.17.0
     */
    std::optional<CodeActionTriggerKind> triggerKind;
  };

  /**
   * Params for the CodeActionRequest.
   *
   * Request:
   * - method: textDocument/codeAction
   * - params: CodeActionParams defined as follows:
   *
   * export interface CodeActionParams extends WorkDoneProgressParams,
   *   PartialResultParams {
   *   textDocument: TextDocumentIdentifier;
   *   range: Range;
   *   context: CodeActionContext;
   * }
   */
  struct CodeActionParams
    : public WorkDoneProgressParams
    , public PartialResultParams {

    /**
     * The document in which the command was invoked.
     *
     * textDocument: TextDocumentIdentifier;
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;

    /**
     * The range for which the command was invoked.
     *
     * range: Range;
     */
    std::unique_ptr<Range> range;

    /**
     * Context carrying additional information.
     *
     * context: CodeActionContext;
     */
    std::unique_ptr<CodeActionContext> context;
  };

  struct Reason {

    /**
     * Human readable description of why the code action is currently disabled.
     *
     * This is displayed in the code actions UI.
     *
     * reason: string;
     */
    string reason;
  };

  /**
   * A code action represents a change that can be performed in code, e.g. to
   * fix a problem or to refactor code.
   *
   * A CodeAction must set either `edit` and/or a `command`. If both are
   * supplied, the `edit` is applied first, then the `command` is executed.
   *
   * Response:
   * - result: (Command | CodeAction)[] | null where CodeAction is defined as
   *   follows:
   *
   * export interface CodeAction {
   *   title: string;
   *   kind?: CodeActionKind;
   *   diagnostics?: Diagnostic[];
   *   isPreferred?: boolean;
   *   disabled?: {
   *     reason: string;
   *   };
   *   edit?: WorkspaceEdit;
   *   command?: Command;
   *   data?: LSPAny;
   * }
   *
   * - partial result: (Command | CodeAction)[]
   * - error: code and message set in case an exception happens during the code
   *   action request.
   */
  struct CodeAction {

    /**
     * A short, human-readable, title for this code action.
     *
     * title: string;
     */
    string title;

    /**
     * The kind of the code action.
     *
     * Used to filter code actions.
     *
     * kind?: CodeActionKind;
     */
    std::optional<CodeActionKind> kind;

    /**
     * The diagnostics that this code action resolves.
     *
     * diagnostics?: Diagnostic[];
     */
    ptr_vector<Diagnostic> diagnostics;

    /**
     * Marks this as a preferred action. Preferred actions are used by the `auto
     * fix` command and can be targeted by keybindings.
     *
     * A quick fix should be marked preferred if it properly addresses the
     * underlying error. A refactoring should be marked preferred if it is the
     * most reasonable choice of actions to take.
     *
     * isPreferred?: boolean;
     *
     * @since 3.15.0
     */
    std::optional<boolean> isPreferred;

    /**
     * Marks that the code action cannot currently be applied.
     *
     * Clients should follow the following guidelines regarding disabled code
     * actions:
     *
     * - Disabled code actions are not shown in automatic lightbulbs code
     *   action menus.
     *
     * - Disabled actions are shown as faded out in the code action menu when
     *   the user request a more specific type of code action, such as
     *   refactorings.
     *
     * - If the user has a keybinding that auto applies a code action and only
     *   a disabled code actions are returned, the client should show the user
     *   an error message with `reason` in the editor.
     *
     * disabled?: {
     *   reason: string;
     * };
     *
     * @since 3.16.0
     */
    optional_ptr<Reason> disabled;

    /**
     * The workspace edit this code action performs.
     *
     * edit?: WorkspaceEdit;
     */
    optional_ptr<WorkspaceEdit> edit;

    /**
     * A command this code action executes. If a code action provides an edit
     * and a command, first the edit is executed and then the command.
     *
     * command?: Command;
     */
    optional_ptr<Command> command;

    /**
     * A data entry field that is preserved on a code action between
     * a `textDocument/codeAction` and a `codeAction/resolve` request.
     *
     * data?: LSPAny;
     *
     * @since 3.16.0
     */
    optional_ptr<LSPAny> data;
  };

  // --------------------------------------------------------------------------
  // Code Action Resolve Request
  // --------------------------------------------------------------------------
  // Since version 3.16.0
  //
  // The request is sent from the client to the server to resolve additional
  // information for a given code action. This is usually used to compute the
  // edit property of a code action to avoid its unnecessary computation during
  // the textDocument/codeAction request.
  //
  // Client Capability:
  // - property name (optional): textDocument.codeAction.resolveSupport
  // - property type: { properties: string[]; }
  //
  // Request:
  // - method: codeAction/resolve
  // - params: CodeAction
  //
  // Response:
  // - result: CodeAction
  // - error: code and message set in case an exception happens during the code
  // - action resolve request.
  // --------------------------------------------------------------------------

  /**
   * The document color request is sent from the client to the server to list
   * all color references found in a given text document. Along with the range,
   * a color value in RGB is returned.
   *
   * Clients can use the result to decorate color references in an editor. For
   * example:
   * - Color boxes showing the actual color next to the reference
   * - Show a color picker when a color reference is edited
   *
   * Client Capability:
   * - property name (optional): textDocument.colorProvider
   * - property type: DocumentColorClientCapabilities defined as follows:
   *
   * export interface DocumentColorClientCapabilities {
   *   dynamicRegistration?: boolean;
   * }
   */
  struct DocumentColorClientCapabilities {

    /**
     * Whether document color supports dynamic registration.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;
  };

  /**
   * Server Capability:
   * - property name (optional): colorProvider
   * - property type: boolean | DocumentColorOptions |
   *   DocumentColorRegistrationOptions where DocumentColorOptions is defined as
   *   follows:
   *
   * export interface DocumentColorOptions extends WorkDoneProgressOptions {
   * }
   */
  struct DocumentColorOptions : public WorkDoneProgressOptions {
    // empty
  };

  /**
   * Registration Options: DocumentColorRegistrationOptions defined as follows:
   *
   * export interface DocumentColorRegistrationOptions extends
   *   TextDocumentRegistrationOptions, StaticRegistrationOptions,
   *   DocumentColorOptions {
   * }
   */
  struct DocumentColorRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public StaticRegistrationOptions
    , public DocumentColorOptions {
    // empty
  };

  /**
   * Request:
   * - method: textDocument/documentColor
   * - params: DocumentColorParams defined as follows:
   *
   * interface DocumentColorParams extends WorkDoneProgressParams,
   *   PartialResultParams {
   *   textDocument: TextDocumentIdentifier;
   * }
   */
  struct DocumentColorParams
    : public WorkDoneProgressParams
    , public PartialResultParams {

    /**
     * The text document.
     *
     * textDocument: TextDocumentIdentifier;
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;
  };

  /**
   * Represents a color in RGBA space.
   *
   * interface Color {
   *   readonly red: decimal;
   *   readonly green: decimal;
   *   readonly blue: decimal;
   *   readonly alpha: decimal;
   * }
   */
  struct Color {

    /**
     * The red component of this color in the range [0-1].
     *
     * readonly red: decimal;
     */
    decimal red;

    /**
     * The green component of this color in the range [0-1].
     *
     * readonly green: decimal;
     */
    decimal green;

    /**
     * The blue component of this color in the range [0-1].
     *
     * readonly blue: decimal;
     */
    decimal blue;

    /**
     * The alpha component of this color in the range [0-1].
     *
     * readonly alpha: decimal;
     */
    decimal alpha;
  };

  /**
   * Response:
   * - result: ColorInformation[] defined as follows:
   *
   * interface ColorInformation {
   *   range: Range;
   *   color: Color;
   * }
   *
   * - partial result: ColorInformation[]
   * - error: code and message set in case an exception happens during the
   *   ‘textDocument/documentColor’ request
   */
  struct ColorInformation {

    /**
     * The range in the document where this color appears.
     *
     * range: Range;
     */
    std::unique_ptr<Range> range;

    /**
     * The actual color value for this color range.
     *
     * color: Color;
     */
    std::unique_ptr<Color> color;
  };

  /**
   * The color presentation request is sent from the client to the server to
   * obtain a list of presentations for a color value at a given location.
   * Clients can use the result to
   * - modify a color reference.
   * - show in a color picker and let users pick one of the presentations
   *
   * This request has no special capabilities and registration options since it
   * is send as a resolve request for the textDocument/documentColor request.
   *
   * Request:
   * - method: textDocument/colorPresentation
   * - params: ColorPresentationParams defined as follows:
   *
   * interface ColorPresentationParams extends WorkDoneProgressParams,
   *   PartialResultParams {
   *   textDocument: TextDocumentIdentifier;
   *   color: Color;
   *   range: Range;
   * }
   */
  struct ColorPresentationParams
    : public WorkDoneProgressParams
    , public PartialResultParams {

    /**
     * The text document.
     *
     * textDocument: TextDocumentIdentifier;
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;

    /**
     * The color information to request presentations for.
     *
     * color: Color;
     */
    std::unique_ptr<Color> color;

    /**
     * The range where the color would be inserted. Serves as a context.
     *
     * range: Range;
     */
    std::unique_ptr<Range> range;
  };

  /**
   * Response:
   * - result: ColorPresentation[] defined as follows:
   *
   * interface ColorPresentation {
   *   label: string;
   *   textEdit?: TextEdit;
   *   additionalTextEdits?: TextEdit[];
   * }
   *
   * - partial result: ColorPresentation[]
   * - error: code and message set in case an exception happens during the
   *   ‘textDocument/colorPresentation’ request
   */
  struct ColorPresentation {

    /**
     * The label of this color presentation. It will be shown on the color
     * picker header. By default this is also the text that is inserted when
     * selecting this color presentation.
     *
     * label: string;
     */
    string label;

    /**
     * An [edit](#TextEdit) which is applied to a document when selecting this
     * presentation for the color. When omitted the
     * [label](#ColorPresentation.label) is used.
     *
     * textEdit?: TextEdit;
     */
    optional_ptr<TextEdit> textEdit;

    /**
     * An optional array of additional [text edits](#TextEdit) that are applied
     * when selecting this color presentation. Edits must not overlap with the
     * main [edit](#ColorPresentation.textEdit) nor with themselves.
     *
     * additionalTextEdits?: TextEdit[];
     */
    optional_ptr_vector<TextEdit> additionalTextEdits;
  };

  /**
   * Client Capability:
   * - property name (optional): textDocument.formatting
   * - property type: DocumentFormattingClientCapabilities defined as follows:
   *
   * export interface DocumentFormattingClientCapabilities {
   *   dynamicRegistration?: boolean;
   * }
   */
  struct DocumentFormattingClientCapabilities {

    /**
     * Whether formatting supports dynamic registration.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;
  };

  /**
   * Server Capability:
   * - property name (optional): documentFormattingProvider
   * - property type: boolean | DocumentFormattingOptions where
   *   DocumentFormattingOptions is defined as follows:
   *
   * export interface DocumentFormattingOptions extends WorkDoneProgressOptions {
   * }
   */
  struct DocumentFormattingOptions : public WorkDoneProgressOptions {
    // empty
  };

  /**
   * Registration Options: DocumentFormattingRegistrationOptions defined as
   * follows:
   *
   * export interface DocumentFormattingRegistrationOptions extends
   *   TextDocumentRegistrationOptions, DocumentFormattingOptions {
   * }
   */
  struct DocumentFormattingRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public DocumentFormattingOptions {
    // empty
  };

  enum class BooleanOrIntegerOrStringType {
    BOOLEAN,
    INTEGER,
    STRING,
  };

  struct BooleanOrIntegerOrString {
    BooleanOrIntegerOrStringType type;
    std::variant<
      boolean,
      integer,
      string
    > value;
  };

  /**
   * Value-object describing what options formatting should use.
   *
   * interface FormattingOptions {
   *   tabSize: uinteger;
   *   insertSpaces: boolean;
   *   trimTrailingWhitespace?: boolean;
   *   insertFinalNewline?: boolean;
   *   trimFinalNewlines?: boolean;
   *   [key: string]: boolean | integer | string;
   * }
   */
  struct FormattingOptions {

    /**
     * Size of a tab in spaces.
     *
     * tabSize: uinteger;
     */
    uinteger tabSize;

    /**
     * Prefer spaces over tabs.
     *
     * insertSpaces: boolean;
     */
    boolean insertSpaces;

    /**
     * Trim trailing whitespace on a line.
     *
     * trimTrailingWhitespace?: boolean;
     *
     * @since 3.15.0
     */
    boolean trimTrailingWhitespace;

    /**
     * Insert a newline character at the end of the file if one does not exist.
     *
     * insertFinalNewline?: boolean;
     *
     * @since 3.15.0
     */
    std::optional<boolean> insertFinalNewline;

    /**
     * Trim all newlines after the final newline at the end of the file.
     *
     * trimFinalNewlines?: boolean;
     *
     * @since 3.15.0
     */
    std::optional<boolean> trimFinalNewlines;

    /**
     * Signature for further properties.
     *
     * [key: string]: boolean | integer | string;
     */
    std::optional<std::map<string, BooleanOrIntegerOrString>> additionalProperties;
  };

  /**
   * Request:
   * - method: textDocument/formatting
   * - params: DocumentFormattingParams defined as follows:
   *
   * interface DocumentFormattingParams extends WorkDoneProgressParams {
   *   textDocument: TextDocumentIdentifier;
   *   options: FormattingOptions;
   * }
   *
   * Response:
   * - result: TextEdit[] | null describing the modification to the document to
   *   be formatted.
   * - error: code and message set in case an exception happens during the
   *   formatting request.
   */
  struct DocumentFormattingParams : public WorkDoneProgressParams {

    /**
     * The document to format.
     *
     * textDocument: TextDocumentIdentifier;
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;

    /**
     * The format options.
     *
     * options: FormattingOptions;
     */
    std::unique_ptr<FormattingOptions> options;
  };

  /**
   * The document range formatting request is sent from the client to the server
   * to format a given range in a document.
   *
   * Client Capability:
   * - property name (optional): textDocument.rangeFormatting
   * - property type: DocumentRangeFormattingClientCapabilities defined as
   *   follows:
   *
   * export interface DocumentRangeFormattingClientCapabilities {
   *   dynamicRegistration?: boolean;
   * }
   */
  struct DocumentRangeFormattingClientCapabilities {

    /**
     * Whether formatting supports dynamic registration.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;
  };

  /**
   * Server Capability:
   * - property name (optional): documentRangeFormattingProvider
   * - property type: boolean | DocumentRangeFormattingOptions where
   *   DocumentRangeFormattingOptions is defined as follows:
   *
   * export interface DocumentRangeFormattingOptions extends
   *   WorkDoneProgressOptions {
   * }
   */
  struct DocumentRangeFormattingOptions : public WorkDoneProgressOptions {
    // empty
  };

  /**
   * Registration Options: DocumentFormattingRegistrationOptions defined as
   * follows:
   *
   * export interface DocumentRangeFormattingRegistrationOptions extends
   *   TextDocumentRegistrationOptions, DocumentRangeFormattingOptions {
   * }
   */
  struct DocumentRangeFormattingRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public DocumentRangeFormattingOptions {
    // empty
  };

  /**
   * Request:
   * - method: textDocument/rangeFormatting,
   * - params: DocumentRangeFormattingParams defined as follows:
   *
   * interface DocumentRangeFormattingParams extends WorkDoneProgressParams {
   *   textDocument: TextDocumentIdentifier;
   *   range: Range;
   *   options: FormattingOptions;
   * }
   *
   * Response:
   * - result: TextEdit[] | null describing the modification to the document to
   *   be formatted.
   * - error: code and message set in case an exception happens during the range
   *   formatting request.
   */
  struct DocumentRangeFormattingParams : public WorkDoneProgressParams {

    /**
     * The document to format.
     *
     * textDocument: TextDocumentIdentifier;
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;

    /**
     * The range to format
     *
     * range: Range;
     */
    std::unique_ptr<Range> range;

    /**
     * The format options
     *
     * options: FormattingOptions;
     */
    std::unique_ptr<FormattingOptions> options;
  };

  /**
   * The document on type formatting request is sent from the client to the
   * server to format parts of the document during typing.
   *
   * Client Capability:
   * - property name (optional): textDocument.onTypeFormatting
   * - property type: DocumentOnTypeFormattingClientCapabilities defined as
   *   follows:
   *
   * export interface DocumentOnTypeFormattingClientCapabilities {
   *   dynamicRegistration?: boolean;
   * }
   */
  struct DocumentOnTypeFormattingClientCapabilities {

    /**
     * Whether on type formatting supports dynamic registration.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;
  };

  /**
   * Server Capability:
   * - property name (optional): documentOnTypeFormattingProvider
   * - property type: DocumentOnTypeFormattingOptions defined as follows:
   *
   * export interface DocumentOnTypeFormattingOptions {
   *   firstTriggerCharacter: string;
   *   moreTriggerCharacter?: string[];
   * }
   */
  struct DocumentOnTypeFormattingOptions {

    /**
     * A character on which formatting should be triggered, like `{`.
     *
     * firstTriggerCharacter: string;
     */
    string firstTriggerCharacter;

    /**
     * More trigger characters.
     *
     * moreTriggerCharacter?: string[];
     */
    optional_vector<string> moreTriggerCharacter;
  };

  /**
   * Registration Options: DocumentOnTypeFormattingRegistrationOptions defined
   * as follows:
   *
   * export interface DocumentOnTypeFormattingRegistrationOptions extends
   *   TextDocumentRegistrationOptions, DocumentOnTypeFormattingOptions {
   * }
   */
  struct DocumentOnTypeFormattingRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public DocumentOnTypeFormattingOptions {
    // empty
  };

  /**
   * Request:
   * - method: textDocument/onTypeFormatting
   * - params: DocumentOnTypeFormattingParams defined as follows:
   *
   * interface DocumentOnTypeFormattingParams {
   *   textDocument: TextDocumentIdentifier;
   *   position: Position;
   *   ch: string;
   *   options: FormattingOptions;
   * }
   *
   * Response:
   * - result: TextEdit[] | null describing the modification to the document.
   * - error: code and message set in case an exception happens during the range
   *   formatting request.
   */
  struct DocumentOnTypeFormattingParams {

    /**
     * The document to format.
     *
     * textDocument: TextDocumentIdentifier;
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;

    /**
     * The position around which the on type formatting should happen. This is
     * not necessarily the exact position where the character denoted by the
     * property `ch` got typed.
     *
     * position: Position;
     */
    std::unique_ptr<Position> position;

    /**
     * The character that has been typed that triggered the formatting on type
     * request. That is not necessarily the last character that got inserted
     * into the document since the client could auto insert characters as well
     * (e.g. like automatic brace completion).
     *
     * ch: string;
     */
    string ch;

    /**
     * The formatting options.
     *
     * options: FormattingOptions;
     */
    std::unique_ptr<FormattingOptions> options;
  };

  /**
   * export namespace PrepareSupportDefaultBehavior {
   *   export const Identifier: 1 = 1;
   * }
   * export type PrepareSupportDefaultBehavior = 1;
   */
  enum class PrepareSupportDefaultBehavior {

    /**
     * The client's default behavior is to select the identifier
     * according to the language's syntax rule.
     *
     * export const Identifier: 1 = 1;
     */
    Identifier = 1,
  };

  extern std::map<PrepareSupportDefaultBehavior, std::string> PrepareSupportDefaultBehaviorNames;

  auto prepareSupportDefaultBehaviorByName(const std::string &name) -> PrepareSupportDefaultBehavior;

  /**
   * The rename request is sent from the client to the server to ask the server
   * to compute a workspace change so that the client can perform a
   * workspace-wide rename of a symbol.
   *
   * Client Capability:
   * - property name (optional): textDocument.rename
   * - property type: RenameClientCapabilities defined as follows:
   *
   * export interface RenameClientCapabilities {
   *   dynamicRegistration?: boolean;
   *   prepareSupport?: boolean;
   *   prepareSupportDefaultBehavior?: PrepareSupportDefaultBehavior;
   *   honorsChangeAnnotations?: boolean;
   * }
   */
  struct RenameClientCapabilities {

    /**
     * Whether rename supports dynamic registration.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;

    /**
     * Client supports testing for validity of rename operations
     * before execution.
     *
     * prepareSupport?: boolean;
     *
     * @since version 3.12.0
     */
    std::optional<boolean> prepareSupport;

    /**
     * Client supports the default behavior result
     * (`{ defaultBehavior: boolean }`).
     *
     * The value indicates the default behavior used by the
     * client.
     *
     * prepareSupportDefaultBehavior?: PrepareSupportDefaultBehavior;
     *
     * @since version 3.16.0
     */
    std::optional<PrepareSupportDefaultBehavior> prepareSupportDefaultBehavior;

    /**
     * Whether the client honors the change annotations in text edits and
     * resource operations returned via the rename request's workspace edit by
     * for example presenting the workspace edit in the user interface and
     * asking for confirmation.
     *
     * honorsChangeAnnotations?: boolean;
     *
     * @since 3.16.0
     */
    std::optional<boolean> honorsChangeAnnotations;
  };

  /**
   * Server Capability:
   * - property name (optional): renameProvider
   * - property type: boolean | RenameOptions where RenameOptions is defined as
   *   follows:
   *
   * export interface RenameOptions extends WorkDoneProgressOptions {
   *   prepareProvider?: boolean;
   * }
   *
   * RenameOptions may only be specified if the client states that it supports
   * prepareSupport in its initial initialize request.
   */
  struct RenameOptions : public WorkDoneProgressOptions {

    /**
     * Renames should be checked and tested before being executed.
     *
     * prepareProvider?: boolean;
     */
    std::optional<boolean> prepareProvider;
  };

  /**
   * Registration Options: RenameRegistrationOptions defined as follows:
   *
   * export interface RenameRegistrationOptions extends
   *   TextDocumentRegistrationOptions, RenameOptions {
   * }
   */
  struct RenameRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public RenameOptions {
    // empty
  };

  /**
   * Request:
   * - method: textDocument/rename
   * - params: RenameParams defined as follows:
   *
   * interface RenameParams extends TextDocumentPositionParams,
   *   WorkDoneProgressParams {
   *   newName: string;
   * }
   *
   * Response:
   * - result: WorkspaceEdit | null describing the modification to the
   *   workspace. null should be treated the same was as WorkspaceEdit with no
   *   changes (no change was required).
   * - error: code and message set in case when rename could not be performed
   *   for any reason. Examples include: there is nothing at given position to
   *   rename (like a space), given symbol does not support renaming by the
   *   server or the code is invalid (e.g. does not compile).
   */
  struct RenameParams
    : public TextDocumentPositionParams
    , public WorkDoneProgressParams {

    /**
     * The new name of the symbol. If the given name is not valid the request
     * must return a [ResponseError](#ResponseError) with an appropriate message
     * set.
     *
     * newName: string;
     */
    string newName;
  };

  /**
   * The prepare rename request is sent from the client to the server to setup
   * and test the validity of a rename operation at a given location.
   *
   * Request:
   * - method: textDocument/prepareRename
   * - params: PrepareRenameParams defined as follows:
   *
   * export interface PrepareRenameParams extends TextDocumentPositionParams, WorkDoneProgressParams {
   * }
   *
   * Response:
   * - result: Range | { range: Range, placeholder: string } | {
   *   defaultBehavior: boolean } | null describing a Range of the string to
   *   rename and optionally a placeholder text of the string content to be
   *   renamed. If { defaultBehavior: boolean } is returned (since 3.16) the
   *   rename position is valid and the client should use its default behavior
   *   to compute the rename range. If null is returned then it is deemed that a
   *   ‘textDocument/rename’ request is not valid at the given position.
   * - error: code and message set in case the element can’t be renamed. Clients
   *   should show the information in their user interface.
   */
  struct PrepareRenameParams
    : public TextDocumentPositionParams
    , public WorkDoneProgressParams {
    // empty
  };

  /**
   * The linked editing request is sent from the client to the server to return
   * for a given position in a document the range of the symbol at the position
   * and all ranges that have the same content. Optionally a word pattern can be
   * returned to describe valid contents. A rename to one of the ranges can be
   * applied to all other ranges if the new content is valid. If no
   * result-specific word pattern is provided, the word pattern from the
   * client’s language configuration is used.
   *
   * Client Capabilities:
   * - property name (optional): textDocument.linkedEditingRange
   * - property type: LinkedEditingRangeClientCapabilities defined as follows:
   *
   * export interface LinkedEditingRangeClientCapabilities {
   *   dynamicRegistration?: boolean;
   * }
   */
  struct LinkedEditingRangeClientCapabilities {

    /**
     * Whether the implementation supports dynamic registration.
     * If this is set to `true` the client supports the new
     * `(TextDocumentRegistrationOptions & StaticRegistrationOptions)`
     * return value for the corresponding server capability as well.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;
  };

  /**
   * Server Capability:
   * - property name (optional): linkedEditingRangeProvider
   * - property type: boolean | LinkedEditingRangeOptions |
   *   LinkedEditingRangeRegistrationOptions defined as follows:
   *
   * export interface LinkedEditingRangeOptions extends WorkDoneProgressOptions {
   * }
   */
  struct LinkedEditingRangeOptions : public WorkDoneProgressOptions {
    // empty
  };

  /**
   * Registration Options: LinkedEditingRangeRegistrationOptions defined as
   * follows:
   *
   * export interface LinkedEditingRangeRegistrationOptions extends
   *   TextDocumentRegistrationOptions, LinkedEditingRangeOptions,
   *   StaticRegistrationOptions {
   * }
   */
  struct LinkedEditingRangeRegistrationOptions
    : public TextDocumentRegistrationOptions
    , public LinkedEditingRangeOptions
    , public StaticRegistrationOptions {
    // empty
  };

  /**
   * Request:
   * - method: textDocument/linkedEditingRange
   * - params: LinkedEditingRangeParams defined as follows:
   *
   * export interface LinkedEditingRangeParams extends TextDocumentPositionParams,
   *   WorkDoneProgressParams {
   * }
   */
  struct LinkedEditingRangeParams
    : public TextDocumentPositionParams
    , public WorkDoneProgressParams {
    // empty
  };

  /**
   * Response:
   * - result: LinkedEditingRanges | null defined as follows:
   *
   * export interface LinkedEditingRanges {
   *   ranges: Range[];
   *   wordPattern?: string;
   * }
   *
   * - error: code and message set in case an exception happens during the
   *   ‘textDocument/linkedEditingRange’ request
   */
  struct LinkedEditingRanges {

    /**
     * A list of ranges that can be renamed together. The ranges must have
     * identical length and contain identical text content. The ranges cannot
     * overlap.
     *
     * ranges: Range[];
     */
    ptr_vector<Range> ranges;

    /**
     * An optional word pattern (regular expression) that describes valid
     * contents for the given ranges. If no pattern is provided, the client
     * configuration's word pattern will be used.
     *
     *
     wordPattern?: string;
     */
    std::optional<string> wordPattern;
  };

  /**
   * The workspace symbol request is sent from the client to the server to list
   * project-wide symbols matching the query string. Since 3.17.0 servers can
   * also provide a handler for workspaceSymbol/resolve requests. This allows
   * servers to return workspace symbols without a range for a workspace/symbol
   * request. Clients then need to resolve the range when necessary using the
   * workspaceSymbol/resolve request. Servers can only use this new model if
   * clients advertise support for it via the workspace.symbol.resolveSupport
   * capability.
   *
   * Client Capability:
   * - property path (optional): workspace.symbol
   * - property type: WorkspaceSymbolClientCapabilities defined as follows:
   *
   * interface WorkspaceSymbolClientCapabilities {
   *   dynamicRegistration?: boolean;
   *   symbolKind?: {
   *     valueSet?: SymbolKind[];
   *   };
   *   tagSupport?: {
   *     valueSet: SymbolTag[];
   *   };
   *   resolveSupport?: {
   *     properties: string[];
   *   };
   * }
   */
  struct WorkspaceSymbolClientCapabilities {

    /**
     * Symbol request supports dynamic registration.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;

    /**
     * Specific capabilities for the `SymbolKind` in the `workspace/symbol`
     * request.
     *
     * symbolKind?: {
     *   // The symbol kind values the client supports. When this
     *   // property exists the client also guarantees that it will
     *   // handle values outside its set gracefully and falls back
     *   // to a default value when unknown.
     *   //
     *   // If this property is not present the client only supports
     *   // the symbol kinds from `File` to `Array` as defined in
     *   // the initial version of the protocol.
     *   valueSet?: SymbolKind[];
     * };
     */
    optional_ptr<OptionalValueSet<SymbolKind>> symbolKind;

    /**
     * The client supports tags on `SymbolInformation` and `WorkspaceSymbol`.
     * Clients supporting tags have to handle unknown tags gracefully.
     *
     * tagSupport?: {
     *   // The tags supported by the client.
     *   valueSet: SymbolTag[];
     * };
     *
     * @since 3.16.0
     */
    optional_ptr<ValueSet<SymbolTag>> tagSupport;

    /**
     * The client support partial workspace symbols. The client will send the
     * request `workspaceSymbol/resolve` to the server to resolve additional
     * properties.
     *
     * resolveSupport?: {
     *   // The properties that a client can resolve lazily. Usually
     *   // `location.range`
     *   properties: string[];
     * };
     *
     * @since 3.17.0 - proposedState
     */
    optional_ptr<ResolveSupportCapabilities> resolveSupport;
  };

  /**
   * Server Capability:
   * - property path (optional): workspaceSymbolProvider
   * - property type: boolean | WorkspaceSymbolOptions where
   *   WorkspaceSymbolOptions is defined as follows:
   *
   * export interface WorkspaceSymbolOptions extends WorkDoneProgressOptions {
   *   resolveProvider?: boolean;
   * }
   */
  struct WorkspaceSymbolOptions : public WorkDoneProgressOptions {

    /**
     * The server provides support to resolve additional
     * information for a workspace symbol.
     *
     * resolveProvider?: boolean;
     *
     * @since 3.17.0
     */
    std::optional<boolean> resolveProvider;
  };

  /**
   * Registration Options: WorkspaceSymbolRegistrationOptions defined as
   * follows:
   *
   * export interface WorkspaceSymbolRegistrationOptions
   *   extends WorkspaceSymbolOptions {
   * }
   */
  struct WorkspaceSymbolRegistrationOptions : public WorkspaceSymbolOptions {
    // empty
  };

  /**
   * The parameters of a Workspace Symbol Request.
   *
   * Request:
   * - method: ‘workspace/symbol’
   * - params: WorkspaceSymbolParams defined as follows:
   *
   * interface WorkspaceSymbolParams extends WorkDoneProgressParams,
   *   PartialResultParams {
   *   query: string;
   * }
   */
  struct WorkspaceSymbolParams
    : public WorkDoneProgressParams
    , public PartialResultParams {

    /**
     * A query string to filter symbols by. Clients may send an empty string
     * here to request all symbols.
     *
     * query: string;
     */
    string query;
  };

  struct DocumentUriLocation {
    DocumentUri uri;
  };

  enum class LocationOrDocumentUriType {
    LOCATION,
    DOCUMENT_URI,
  };

  struct LocationOrDocumentUri {
    LocationOrDocumentUriType type;
    std::variant<
      std::unique_ptr<Location>,
      std::unique_ptr<DocumentUriLocation>
    > value;
  };

  /**
   * A special workspace symbol that supports locations without a range
   *
   * Response:
   * - result: SymbolInformation[] | WorkspaceSymbol[] | null. See above for the
   *   definition of SymbolInformation. It is recommended that you use the new
   *   WorkspaceSymbol. However whether the workspace symbol can return a
   *   location without a range depends on the client capability
   *   workspace.symbol.resolveSupport. WorkspaceSymbolwhich is defined as
   *   follows:
   *
   * export interface WorkspaceSymbol {
   *   name: string;
   *   kind: SymbolKind;
   *   tags?: SymbolTag[];
   *   containerName?: string;
   *   location: Location | { uri: DocumentUri };
   *   data?: LSPAny;
   * }
   *
   * - partial result: SymbolInformation[] | WorkspaceSymbol[] as defined above.
   * - error: code and message set in case an exception happens during the
   *   workspace symbol request.
   *
   * @since 3.17.0
   */
  struct WorkspaceSymbol {

    /**
     * The name of this symbol.
     *
     * name: string;
     */
    string name;

    /**
     * The kind of this symbol.
     *
     * kind: SymbolKind;
     */
    SymbolKind kind;

    /**
     * Tags for this completion item.
     *
     * tags?: SymbolTag[];
     */
    optional_vector<SymbolTag> tags;

    /**
     * The name of the symbol containing this symbol. This information is for
     * user interface purposes (e.g. to render a qualifier in the user interface
     * if necessary). It can't be used to re-infer a hierarchy for the document
     * symbols.
     *
     * containerName?: string;
     */
    std::optional<string> containerName;

    /**
     * The location of this symbol. Whether a server is allowed to return a
     * location without a range depends on the client capability
     * `workspace.symbol.resolveSupport`.
     *
     * location: Location | { uri: DocumentUri };
     *
     * See also `SymbolInformation.location`.
     */
    std::unique_ptr<LocationOrDocumentUri> location;

    /**
     * A data entry field that is preserved on a workspace symbol between a
     * workspace symbol request and a workspace symbol resolve request.
     *
     * data?: LSPAny;
     */
    optional_ptr<LSPAny> data;
  };

  // The request is sent from the client to the server to resolve additional
  // information for a given workspace symbol.
  //
  // Request:
  // - method: ‘workspaceSymbol/resolve’
  // - params: WorkspaceSymbol
  //
  // Response:
  // - result: WorkspaceSymbol
  // - error: code and message set in case an exception happens during the
  //   workspace symbol resolve request.

  /**
   * export interface ConfigurationItem {
   *   scopeUri?: URI;
   *   section?: string;
   * }
   */
  struct ConfigurationItem {

    /**
     * The scope to get the configuration section for.
     *
     * scopeUri?: URI;
     */
    std::optional<URI> scopeUri;

    /**
     * The configuration section asked for.
     *
     * section?: string;
     */
    std::optional<string> section;
  };

  /**
   * The workspace/configuration request is sent from the server to the client
   * to fetch configuration settings from the client. The request can fetch
   * several configuration settings in one roundtrip. The order of the returned
   * configuration settings correspond to the order of the passed
   * ConfigurationItems (e.g. the first item in the response is the result for
   * the first configuration item in the params).
   *
   * A ConfigurationItem consists of the configuration section to ask for and an
   * additional scope URI. The configuration section asked for is defined by the
   * server and doesn’t necessarily need to correspond to the configuration
   * store used by the client. So a server might ask for a configuration
   * cpp.formatterOptions but the client stores the configuration in an XML
   * store layout differently. It is up to the client to do the necessary
   * conversion. If a scope URI is provided the client should return the setting
   * scoped to the provided resource. If the client for example uses
   * EditorConfig to manage its settings the configuration should be returned
   * for the passed resource URI. If the client can’t provide a configuration
   * setting for a given scope then null needs to be present in the returned
   * array.
   *
   * This pull model replaces the old push model were the client signaled
   * configuration change via an event. If the server still needs to react to
   * configuration changes (since the server caches the result of
   * workspace/configuration requests) the server should register for an empty
   * configuration change using the following registration pattern:
   *
   * connection.client.register(DidChangeConfigurationNotification.type, undefined);
   *
   * Client Capability:
   * - property path (optional): workspace.configuration
   * - property type: boolean
   *
   * Request:
   * - method: ‘workspace/configuration’
   * - params: ConfigurationParams defined as follows:
   *
   * export interface ConfigurationParams {
   *   items: ConfigurationItem[];
   * }
   *
   * Response:
   * - result: LSPAny[]
   * - error: code and message set in case an exception happens during the
   *   ‘workspace/configuration’ request
   */
  struct ConfigurationParams {

    /**
     * items: ConfigurationItem[];
     */
    ptr_vector<ConfigurationItem> items;
  };

  /**
   * A notification sent from the client to the server to signal the change of
   * configuration settings.
   *
   * Client Capability:
   * - property path (optional): workspace.didChangeConfiguration
   * - property type: DidChangeConfigurationClientCapabilities defined as
   *   follows:
   *
   * export interface DidChangeConfigurationClientCapabilities {
   *   dynamicRegistration?: boolean;
   * }
   */
  struct DidChangeConfigurationClientCapabilities {

    /**
     * Did change configuration notification supports dynamic registration.
     *
     * dynamicRegistration?: boolean;
     *
     * @since 3.6.0 to support the new pull model.
     */
    std::optional<boolean> dynamicRegistration;
  };

  /**
   * Notification:
   * - method: ‘workspace/didChangeConfiguration’,
   * - params: DidChangeConfigurationParams defined as follows:
   *
   * interface DidChangeConfigurationParams {
   *   settings: LSPAny;
   * }
   */
  struct DidChangeConfigurationParams {

    /**
     * The actual changed settings
     *
     * settings: LSPAny;
     */
    std::unique_ptr<LSPAny> settings;
  };

  enum class StringOrBooleanType {
    STRING,
    BOOLEAN,
  };

  struct StringOrBoolean {
    StringOrBooleanType type;
    std::variant<
      string,
      boolean
    > value;
  };

  /**
   * Many tools support more than one root folder per workspace. Examples for
   * this are VS Code’s multi-root support, Atom’s project folder support or
   * Sublime’s project support. If a client workspace consists of multiple roots
   * then a server typically needs to know about this. The protocol up to now
   * assumes one root folder which is announced to the server by the rootUri
   * property of the InitializeParams. If the client supports workspace folders
   * and announces them via the corresponding workspaceFolders client
   * capability, the InitializeParams contain an additional property
   * workspaceFolders with the configured workspace folders when the server
   * starts.
   *
   * The workspace/workspaceFolders request is sent from the server to the
   * client to fetch the current open list of workspace folders. Returns null in
   * the response if only a single file is open in the tool. Returns an empty
   * array if a workspace is open but no folders are configured.
   *
   * Client Capability:
   * - property path (optional): workspace.workspaceFolders
   * - property type: boolean
   *
   * Server Capability:
   * - property path (optional): workspace.workspaceFolders
   * - property type: WorkspaceFoldersServerCapabilities defined as follows:
   *
   * export interface WorkspaceFoldersServerCapabilities {
   *   supported?: boolean;
   *   changeNotifications?: string | boolean;
   * }
   */
  struct WorkspaceFoldersServerCapabilities {

    /**
     * The server has support for workspace folders
     *
     * supported?: boolean;
     */
    std::optional<boolean> supported;

    /**
     * Whether the server wants to receive workspace folder change
     * notifications.
     *
     * If a string is provided, the string is treated as an ID under which the
     * notification is registered on the client side. The ID can be used to
     * unregister for these events using the `client/unregisterCapability`
     * request.
     *
     * changeNotifications?: string | boolean;
     */
    optional_ptr<StringOrBoolean> changeNotifications;
  };

  /**
   * Request:
   * - method: workspace/workspaceFolders
   * - params: none
   *
   * Response:
   * - result: WorkspaceFolder[] | null defined as follows:
   *
   * export interface WorkspaceFolder {
   *   uri: URI;
   *   name: string;
   * }
   *
   * - error: code and message set in case an exception happens during the
   *   ‘workspace/workspaceFolders’ request
   */
  struct WorkspaceFolder {

    /**
     * The associated URI for this workspace folder.
     *
     * uri: URI;
     */
    URI uri;

    /**
     * The name of the workspace folder. Used to refer to this
     * workspace folder in the user interface.
     *
     * name: string;
     */
    string name;
  };

  /**
   * The workspace folder change event.
   *
   * export interface WorkspaceFoldersChangeEvent {
   *   added: WorkspaceFolder[];
   *   removed: WorkspaceFolder[];
   * }
   */
  struct WorkspaceFoldersChangeEvent {

    /**
     * The array of added workspace folders
     *
     * added: WorkspaceFolder[];
     */
    ptr_vector<WorkspaceFolder> added;

    /**
     * The array of the removed workspace folders
     *
     * removed: WorkspaceFolder[];
     */
    ptr_vector<WorkspaceFolder> removed;
  };

  /**
   * The workspace/didChangeWorkspaceFolders notification is sent from the
   * client to the server to inform the server about workspace folder
   * configuration changes. A server can register for this notification by using
   * either the server capability workspace.workspaceFolders.changeNotifications
   * or by using the dynamic capability registration mechanism. To dynamically
   * register for the workspace/didChangeWorkspaceFolders send a
   * client/registerCapability request from the server to the client. The
   * registration parameter must have a registrations item of the following
   * form, where id is a unique id used to unregister the capability (the
   * example uses a UUID):
   *
   * {
   *   id: "28c6150c-bd7b-11e7-abc4-cec278b6b50a",
   *   method: "workspace/didChangeWorkspaceFolders"
   * }
   *
   * Notification:
   * - method: ‘workspace/didChangeWorkspaceFolders’
   * - params: DidChangeWorkspaceFoldersParams defined as follows:
   *
   * export interface DidChangeWorkspaceFoldersParams {
   *   event: WorkspaceFoldersChangeEvent;
   * }
   */
  struct DidChangeWorkspaceFoldersParams {

    /**
     * The actual workspace folder change event.
     *
     * event: WorkspaceFoldersChangeEvent;
     */
    std::unique_ptr<WorkspaceFoldersChangeEvent> event;
  };

  /**
   * A pattern kind describing if a glob pattern matches a file a folder or
   * both.
   *
   * export namespace FileOperationPatternKind {
   *   export const file: 'file' = 'file';
   *   export const folder: 'folder' = 'folder';
   * }
   * export type FileOperationPatternKind = 'file' | 'folder';
   *
   * @since 3.16.0
   */
  enum class FileOperationPatternKind {

    /**
     * The pattern matches a file only.
     *
     * export const file: 'file' = 'file';
     */
    File,

    /**
     * The pattern matches a folder only.
     *
     * export const folder: 'folder' = 'folder';
     */
    Folder,
  };

  extern std::map<FileOperationPatternKind, std::string> FileOperationPatternKindNames;

  auto fileOperationPatternKindByName(const std::string &name) -> FileOperationPatternKind;

  extern std::map<FileOperationPatternKind, std::string> FileOperationPatternKindValues;

  auto fileOperationPatternKindByValue(const std::string &value) -> FileOperationPatternKind;

  /**
   * Matching options for the file operation pattern.
   *
   * export interface FileOperationPatternOptions {
   *   ignoreCase?: boolean;
   * }
   *
   * @since 3.16.0
   */
  struct FileOperationPatternOptions {

    /**
     * The pattern should be matched ignoring casing.
     *
     * ignoreCase?: boolean;
     */
    std::optional<boolean> ignoreCase;
  };

  /**
   * A pattern to describe in which file operation requests or notifications the
   * server is interested in.
   *
   * interface FileOperationPattern {
   *   glob: string;
   *   matches?: FileOperationPatternKind;
   *   options?: FileOperationPatternOptions;
   * }
   *
   * @since 3.16.0
   */
  struct FileOperationPattern {

    /**
     * The glob pattern to match. Glob patterns can have the following syntax:
     * - `*` to match one or more characters in a path segment
     * - `?` to match on one character in a path segment
     * - `**` to match any number of path segments, including none
     * - `{}` to group sub patterns into an OR expression.
     * - `[]` to declare a range of characters to match in a path segment.
     * - `[!...]` to negate a range of characters to match in a path segment
     *
     * glob: string;
     */
    string glob;

    /**
     * Whether to match files or folders with this pattern.
     *
     * Matches both if undefined.
     *
     * matches?: FileOperationPatternKind;
     */
    std::optional<FileOperationPatternKind> matches;

    /**
     * Additional options used during matching.
     *
     * options?: FileOperationPatternOptions;
     */
    optional_ptr<FileOperationPatternOptions> options;
  };

  /**
   * A filter to describe in which file operation requests or notifications
   * the server is interested in.
   *
   * export interface FileOperationFilter {
   *   scheme?: string;
   *   pattern: FileOperationPattern;
   * }
   *
   * @since 3.16.0
   */
  struct FileOperationFilter {

    /**
     * A Uri like `file` or `untitled`.
     *
     * scheme?: string;
     */
    std::optional<string> scheme;

    /**
     * The actual file operation pattern.
     *
     * pattern: FileOperationPattern;
     */
    std::unique_ptr<FileOperationPattern> pattern;
  };

  /**
   * The options to register for file operations.
   *
   * The will create files request is sent from the client to the server before
   * files are actually created as long as the creation is triggered from within
   * the client either by a user action or by applying a workspace edit. The
   * request can return a WorkspaceEdit which will be applied to workspace
   * before the files are created. Hence the WorkspaceEdit can not manipulate
   * the content of the files to be created. Please note that clients might drop
   * results if computing the edit took too long or if a server constantly fails
   * on this request. This is done to keep creates fast and reliable.
   *
   * Client Capability:
   * - property name (optional): workspace.fileOperations.willCreate
   * - property type: boolean
   *
   * The capability indicates that the client supports sending
   * workspace/willCreateFiles requests.
   *
   * Server Capability:
   * - property name (optional): workspace.fileOperations.willCreate
   * - property type: FileOperationRegistrationOptions where
   *   FileOperationRegistrationOptions is defined as follows:
   *
   * interface FileOperationRegistrationOptions {
   *   filters: FileOperationFilter[];
   * }
   *
   * @since 3.16.0
   */
  struct FileOperationRegistrationOptions {

    /**
     * The actual filters.
     *
     * filters: FileOperationFilter[];
     */
    ptr_vector<FileOperationFilter> filters;
  };

  /**
   * Represents information on a file/folder create.
   *
   * export interface FileCreate {
   *   uri: string;
   * }
   *
   * @since 3.16.0
   */
  struct FileCreate {

    /**
     * A file:// URI for the location of the file/folder being created.
     *
     * uri: string;
     */
    string uri;
  };

  /**
   * The parameters sent in notifications/requests for user-initiated creation
   * of files.
   *
   * The capability indicates that the server is interested in receiving workspace/willCreateFiles requests.
   *
   * Registration Options: none
   *
   * Request:
   * - method: ‘workspace/willCreateFiles’
   * - params: CreateFilesParams defined as follows:
   *
   * export interface CreateFilesParams {
   *   files: FileCreate[];
   * }
   *
   * Response:
   * - result:WorkspaceEdit | null
   * - error: code and message set in case an exception happens during the
   *   willCreateFiles request.
   *
   * @since 3.16.0
   */
  struct CreateFilesParams {

    /**
     * An array of all files/folders created in this operation.
     *
     * files: FileCreate[];
     */
    ptr_vector<FileCreate> files;
  };

  // -----------------------------------------------------------------------
  // DidCreateFiles Notification
  // -----------------------------------------------------------------------
  // The did create files notification is sent from the client to the server
  // when files were created from within the client.
  //
  // Client Capability:
  // - property name (optional): workspace.fileOperations.didCreate
  // - property type: boolean
  //
  // The capability indicates that the client supports sending
  // workspace/didCreateFiles notifications.
  //
  // Server Capability:
  // - property name (optional): workspace.fileOperations.didCreate
  // - property type: FileOperationRegistrationOptions
  //
  // The capability indicates that the server is interested in receiving
  // workspace/didCreateFiles notifications.
  //
  // Notification:
  // - method: ‘workspace/didCreateFiles’
  // - params: CreateFilesParams
  // -----------------------------------------------------------------------

  /**
   * Represents information on a file/folder rename.
   *
   * @since 3.16.0
   */
  struct FileRename {

    /**
     * A file:// URI for the original location of the file/folder being renamed.
     *
     * oldUri: string;
     */
    string oldUri;

    /**
     * A file:// URI for the new location of the file/folder being renamed.
     *
     * newUri: string;
     */
    string newUri;
  };

  /**
   * The parameters sent in notifications/requests for user-initiated renames of
   * files.
   *
   * The will rename files request is sent from the client to the server before
   * files are actually renamed as long as the rename is triggered from within
   * the client either by a user action or by applying a workspace edit. The
   * request can return a WorkspaceEdit which will be applied to workspace
   * before the files are renamed. Please note that clients might drop results
   * if computing the edit took too long or if a server constantly fails on this
   * request. This is done to keep renames fast and reliable.
   *
   * Client Capability:
   * - property name (optional): workspace.fileOperations.willRename
   * - property type: boolean
   *
   * The capability indicates that the client supports sending
   * workspace/willRenameFiles requests.
   *
   * Server Capability:
   * - property name (optional): workspace.fileOperations.willRename
   * - property type: FileOperationRegistrationOptions
   *
   * The capability indicates that the server is interested in receiving
   * workspace/willRenameFiles requests.
   *
   * Registration Options: none
   *
   * Request:
   * - method: ‘workspace/willRenameFiles’
   * - params: RenameFilesParams defined as follows:
   *
   * export interface RenameFilesParams {
   *   files: FileRename[];
   * }
   *
   * Response:
   * - result:WorkspaceEdit | null
   * - error: code and message set in case an exception happens during the
   *   workspace/willRenameFiles request.
   *
   * @since 3.16.0
   */
  struct RenameFilesParams {

    /**
     * An array of all files/folders renamed in this operation. When a folder
     * is renamed, only the folder will be included, and not its children.
     *
     * files: FileRename[];
     */
    ptr_vector<FileRename> files;
  };

  // -----------------------------------------------------------------------
  // DidRenameFiles Notification
  // -----------------------------------------------------------------------
  // The did rename files notification is sent from the client to the server
  // when files were renamed from within the client.
  //
  // Client Capability:
  // - property name (optional): workspace.fileOperations.didRename
  // - property type: boolean
  //
  // The capability indicates that the client supports sending
  // workspace/didRenameFiles notifications.
  //
  // Server Capability:
  // - property name (optional): workspace.fileOperations.didRename
  // - property type: FileOperationRegistrationOptions
  //
  // The capability indicates that the server is interested in receiving
  // workspace/didRenameFiles notifications.
  //
  // Notification:
  // - method: ‘workspace/didRenameFiles’
  // - params: RenameFilesParams
  // -----------------------------------------------------------------------

  /**
   * Represents information on a file/folder delete.
   *
   * export interface FileDelete {
   *   uri: string;
   * }
   *
   * @since 3.16.0
   */
  struct FileDelete {

    /**
     * A file:// URI for the location of the file/folder being deleted.
     *
     * uri: string;
     */
    string uri;
  };

  /**
   * The parameters sent in notifications/requests for user-initiated deletes of
   * files.
   *
   * The will delete files request is sent from the client to the server before
   * files are actually deleted as long as the deletion is triggered from within
   * the client either by a user action or by applying a workspace edit. The
   * request can return a WorkspaceEdit which will be applied to workspace
   * before the files are deleted. Please note that clients might drop results
   * if computing the edit took too long or if a server constantly fails on this
   * request. This is done to keep deletes fast and reliable.
   *
   * Client Capability:
   * - property name (optional): workspace.fileOperations.willDelete
   * - property type: boolean
   *
   * The capability indicates that the client supports sending
   * workspace/willDeleteFiles requests.
   *
   * Server Capability:
   * - property name (optional): workspace.fileOperations.willDelete
   * - property type: FileOperationRegistrationOptions
   *
   * The capability indicates that the server is interested in receiving
   * workspace/willDeleteFiles requests.
   *
   * Registration Options: none
   *
   * Request:
   * - method: workspace/willDeleteFiles
   * - params: DeleteFilesParams defined as follows:
   *
   * export interface DeleteFilesParams {
   *   files: FileDelete[];
   * }
   *
   * - result:WorkspaceEdit | null
   * - error: code and message set in case an exception happens during the
   *   workspace/willDeleteFiles request.
   *
   * @since 3.16.0
   */
  struct DeleteFilesParams {

    /**
     * An array of all files/folders deleted in this operation.
     *
     * files: FileDelete[];
     */
    ptr_vector<FileDelete> files;
  };

  // -----------------------------------------------------------------------
  // DidDeleteFiles Notification
  // -----------------------------------------------------------------------
  // The did delete files notification is sent from the client to the server
  // when files were deleted from within the client.
  //
  // Client Capability:
  // - property name (optional): workspace.fileOperations.didDelete
  // - property type: boolean
  //
  // The capability indicates that the client supports sending
  // workspace/didDeleteFiles notifications.
  //
  // Server Capability:
  // - property name (optional): workspace.fileOperations.didDelete
  // - property type: FileOperationRegistrationOptions
  //
  // The capability indicates that the server is interested in receiving
  // workspace/didDeleteFiles notifications.
  //
  // Notification:
  // - method: ‘workspace/didDeleteFiles’
  // - params: DeleteFilesParams
  // -----------------------------------------------------------------------

  /**
   * The watched files notification is sent from the client to the server when
   * the client detects changes to files and folders watched by the language
   * client (note although the name suggest that only file events are sent it is
   * about file system events which include folders as well). It is recommended
   * that servers register for these file system events using the registration
   * mechanism. In former implementations clients pushed file events without the
   * server actively asking for it.
   *
   * Servers are allowed to run their own file system watching mechanism and not
   * rely on clients to provide file system events. However this is not
   * recommended due to the following reasons:
   * - to our experience getting file system watching on disk right is
   *   challenging, especially if it needs to be supported across multiple OSes.
   * - file system watching is not for free especially if the implementation
   *   uses some sort of polling and keeps a file system tree in memory to
   *   compare time stamps (as for example some node modules do)
   * - a client usually starts more than one server. If every server runs its
   *   own file system watching it can become a CPU or memory problem.
   * - in general there are more server than client implementations. So this
   *   problem is better solved on the client side.
   *
   * Client Capability:
   * - property path (optional): workspace.didChangeWatchedFiles
   * - property type: DidChangeWatchedFilesClientCapabilities defined as follows:
   *
   * export interface DidChangeWatchedFilesClientCapabilities {
   *   dynamicRegistration?: boolean;
   *   relativePatternSupport?: boolean;
   * }
   */
  struct DidChangeWatchedFilesClientCapabilities {

    /**
     * Did change watched files notification supports dynamic registration.
     * Please note that the current protocol doesn't support static
     * configuration for file changes from the server side.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;

    /**
     * Whether the client has support for relative patterns
     * or not.
     *
     * relativePatternSupport?: boolean;
     *
     * @since 3.17.0
     */
    std::optional<boolean> relativePatternSupport;
  };

  /**
   * The glob pattern to watch relative to the base path. Glob patterns can have
   * the following syntax:
   * - `*` to match one or more characters in a path segment
   * - `?` to match on one character in a path segment
   * - `**` to match any number of path segments, including none
   * - `{}` to group conditions
   * - `[]` to declare a range of characters to match in a path segment
   * - `[!...]` to negate a range of characters to match in a path segment
   *
   * export type Pattern = string;
   *
   * @since 3.17.0
   */
  typedef string Pattern;

  enum class WorkspaceFolderOrUriType {
    WORKSPACE_FOLDER,
    URI,
  };

  struct WorkspaceFolderOrUri {
    WorkspaceFolderOrUriType type;
    std::variant<
      std::unique_ptr<WorkspaceFolder>,
      URI
    > value;
  };

  /**
   * A relative pattern is a helper to construct glob patterns that are matched
   * relatively to a base URI. The common value for a `baseUri` is a workspace
   * folder root, but it can be another absolute URI as well.
   *
   * export interface RelativePattern {
   *   baseUri: WorkspaceFolder | URI;
   *   pattern: Pattern;
   * }
   *
   * @since 3.17.0
   */
  struct RelativePattern {

    /**
     * A workspace folder or a base URI to which this pattern will be matched
     * against relatively.
     *
     * baseUri: WorkspaceFolder | URI;
     */
    std::unique_ptr<WorkspaceFolderOrUri> baseUri;

    /**
     * The actual glob pattern;
     *
     * pattern: Pattern;
     */
    Pattern pattern;
  };

  /**
   * export namespace WatchKind {
   *   export const Create = 1;
   *   export const Change = 2;
   *   export const Delete = 4;
   * }
   * export type WatchKind = uinteger;
   */
  enum class WatchKind {

    /**
     * Interested in create events.
     *
     * export const Create = 1;
     */
    Create = 1,

    /**
     * Interested in change events
     *
     * export const Change = 2;
     */
    Change = 2,

    /**
     * Interested in delete events
     *
     * export const Delete = 4;
     */
    Delete = 4,
  };

  extern std::map<WatchKind, std::string> WatchKindNames;

  auto watchKindByName(const std::string &name) -> WatchKind;

  enum class GlobPatternType {
    PATTERN,
    RELATIVE_PATTERN,
  };

  /**
   * The glob pattern. Either a string pattern or a relative pattern.
   *
   * export type GlobPattern = Pattern | RelativePattern;
   *
   * @since 3.17.0
   */
  struct GlobPattern {
    GlobPatternType type;
    std::variant<
      Pattern,
      std::unique_ptr<RelativePattern>
    > value;
  };

  /**
   * export interface FileSystemWatcher {
   *   globPattern: GlobPattern;
   *   kind?: WatchKind;
   * }
   */
  struct FileSystemWatcher {

    /**
     * The glob pattern to watch. See {@link GlobPattern glob pattern}
     * for more detail.
     *
     * globPattern: GlobPattern;
     *
     * @since 3.17.0 support for relative patterns.
     */
    std::unique_ptr<GlobPattern> globPattern;

    /**
     * The kind of events of interest. If omitted it defaults to
     * WatchKind.Create | WatchKind.Change | WatchKind.Delete which is 7.
     *
     * kind?: WatchKind;
     */
    std::optional<WatchKind> kind;
  };

  /**
   * Describe options to be used when registering for file system change events.
   *
   * Registration Options: DidChangeWatchedFilesRegistrationOptions defined as
   * follows:
   *
   * export interface DidChangeWatchedFilesRegistrationOptions {
   *   watchers: FileSystemWatcher[];
   * }
   */
  struct DidChangeWatchedFilesRegistrationOptions {

    /**
     * The watchers to register.
     *
     * watchers: FileSystemWatcher[];
     */
    ptr_vector<FileSystemWatcher> watchers;
  };

  /**
   * The file event type.
   *
   * export namespace FileChangeType {
   *   export const Created = 1;
   *   export const Changed = 2;
   *   export const Deleted = 3;
   * }
   * export type FileChangeType = 1 | 2 | 3;
   */
  enum class FileChangeType {

    /**
     * The file got created.
     *
     * export const Created = 1;
     */
    Created = 1,

    /**
     * The file got changed.
     *
     * export const Changed = 2;
     */
    Changed = 2,

    /**
     * The file got deleted.
     *
     * export const Deleted = 3;
     */
    Deleted = 3,
  };

  extern std::map<FileChangeType, std::string> FileChangeTypeNames;

  auto fileChangeTypeByName(const std::string &name) -> FileChangeType;

  /**
   * An event describing a file change.
   *
   * interface FileEvent {
   *   uri: DocumentUri;
   *   type: FileChangeType;
   * }
   */
  struct FileEvent {

    /**
     * The file's URI.
     *
     * uri: DocumentUri;
     */
    DocumentUri uri;

    /**
     * The change type.
     *
     * type: FileChangeType;
     */
    FileChangeType type;
  };

  /**
   * Notification:
   * - method: ‘workspace/didChangeWatchedFiles’
   * - params: DidChangeWatchedFilesParams defined as follows:
   *
   * interface DidChangeWatchedFilesParams {
   *   changes: FileEvent[];
   * }
   */
  struct DidChangeWatchedFilesParams {

    /**
     * The actual file events.
     *
     * changes: FileEvent[];
     */
    ptr_vector<FileEvent> changes;
  };

  /**
   * The workspace/executeCommand request is sent from the client to the server
   * to trigger command execution on the server. In most cases the server
   * creates a WorkspaceEdit structure and applies the changes to the workspace
   * using the request workspace/applyEdit which is sent from the server to the
   * client.
   *
   * Client Capability:
   * - property path (optional): workspace.executeCommand
   * - property type: ExecuteCommandClientCapabilities defined as follows:
   *
   * export interface ExecuteCommandClientCapabilities {
   *   dynamicRegistration?: boolean;
   * }
   */
  struct ExecuteCommandClientCapabilities {

    /**
     * Execute command supports dynamic registration.
     *
     * dynamicRegistration?: boolean;
     */
    std::optional<boolean> dynamicRegistration;
  };

  /**
   * Server Capability:
   * - property path (optional): executeCommandProvider
   * - property type: ExecuteCommandOptions defined as follows:
   *
   * export interface ExecuteCommandOptions extends WorkDoneProgressOptions {
   *   commands: string[];
   * }
   */
  struct ExecuteCommandOptions : public WorkDoneProgressOptions {

    /**
     * The commands to be executed on the server
     *
     * commands: string[];
     */
    std::vector<string> commands;
  };

  /**
   * Execute command registration options.
   *
   * Registration Options: ExecuteCommandRegistrationOptions defined as follows:
   *
   * export interface ExecuteCommandRegistrationOptions
   *   extends ExecuteCommandOptions {
   * }
   */
  struct ExecuteCommandRegistrationOptions : public ExecuteCommandOptions {
    // empty
  };

  /**
   * Request:
   * - method: ‘workspace/executeCommand’
   * - params: ExecuteCommandParams defined as follows:
   *
   * export interface ExecuteCommandParams extends WorkDoneProgressParams {
   *   command: string;
   *   arguments?: LSPAny[];
   * }
   *
   * The arguments are typically specified when a command is returned from the
   * server to the client. Example requests that return a command are
   * textDocument/codeAction or textDocument/codeLens.
   *
   * Response:
   * - result: LSPAny
   * - error: code and message set in case an exception happens during the
   *   request.
   */
  struct ExecuteCommandParams : public WorkDoneProgressParams {

    /**
     * The identifier of the actual command handler.
     *
     * command: string;
     */
    string command;

    /**
     * Arguments that the command should be invoked with.
     *
     * arguments?: LSPAny[];
     */
    optional_ptr_vector<LSPAny> arguments;
  };

  /**
   * The workspace/applyEdit request is sent from the server to the client to
   * modify resource on the client side.
   *
   * Client Capability:
   * - property path (optional): workspace.applyEdit
   * - property type: boolean
   *
   * See also the WorkspaceEditClientCapabilities for the supported capabilities
   * of a workspace edit.
   *
   * Request:
   * - method: ‘workspace/applyEdit’
   * - params: ApplyWorkspaceEditParams defined as follows:
   *
   * export interface ApplyWorkspaceEditParams {
   *   label?: string;
   *   edit: WorkspaceEdit;
   * }
   */
  struct ApplyWorkspaceEditParams {

    /**
     * An optional label of the workspace edit. This label is
     * presented in the user interface for example on an undo
     * stack to undo the workspace edit.
     *
     * label?: string;
     */
    std::optional<string> label;

    /**
     * The edits to apply.
     *
     * edit: WorkspaceEdit;
     */
    std::unique_ptr<WorkspaceEdit> edit;
  };

  /**
   * Response:
   * - result: ApplyWorkspaceEditResult defined as follows:
   *
   * export interface ApplyWorkspaceEditResult {
   *   applied: boolean;
   *   failureReason?: string;
   *   failedChange?: uinteger;
   * }
   *
   * - error: code and message set in case an exception happens during the
   *   request.
   */
  struct ApplyWorkspaceEditResult {

    /**
     * Indicates whether the edit was applied or not.
     *
     * applied: boolean;
     */
    boolean applied;

    /**
     * An optional textual description for why the edit was not applied. This
     * may be used by the server for diagnostic logging or to provide a suitable
     * error for a request that triggered the edit.
     *
     * failureReason?: string;
     */
    std::optional<string> failureReason;

    /**
     * Depending on the client's failure handling strategy `failedChange` might
     * contain the index of the change that failed. This property is only
     * available if the client signals a `failureHandling` strategy in its
     * client capabilities.
     *
     * failedChange?: uinteger;
     */
    std::optional<uinteger> failedChange;
  };

  /**
   * export namespace MessageType {
   *   export const Error = 1;
   *   export const Warning = 2;
   *   export const Info = 3;
   *   export const Log = 4;
   *   export const Debug = 5;
   * }
   * export type MessageType = 1 | 2 | 3 | 4 | 5;
   */
  enum class MessageType {

    /**
     * An error message.
     *
     * export const Error = 1;
     */
    Error = 1,

    /**
     * A warning message.
     *
     * export const Warning = 2;
     */
    Warning = 2,

    /**
     * An information message.
     *
     * export const Info = 3;
     */
    Info = 3,

    /**
     * A log message.
     *
     * export const Log = 4;
     */
    Log = 4,

    /**
     * A debug message.
     *
     * export const Debug = 5;
     *
     * @since 3.18.0
     * @proposed
     */
    Debug = 5,
  };

  extern std::map<MessageType, std::string> MessageTypeNames;

  auto messageTypeByName(const std::string &name) -> MessageType;

  /**
   * The show message notification is sent from a server to a client to ask the
   * client to display a particular message in the user interface.
   *
   * Notification:
   * - method: ‘window/showMessage’
   * - params: ShowMessageParams defined as follows:
   *
   * interface ShowMessageParams {
   *   type: MessageType;
   *   message: string;
   * }
   */
  struct ShowMessageParams {

    /**
     * The message type. See {@link MessageType}.
     *
     * type: MessageType;
     */
    MessageType type;

    /**
     * The actual message.
     *
     * message: string;
     */
    string message;
  };

  struct MessageActionItemCapabilities {

    /**
     * Whether the client supports additional attributes which are preserved and
     * sent back to the server in the request's response.
     *
     * additionalPropertiesSupport?: boolean;
     */
    std::optional<boolean> additionalPropertiesSupport;
  };

  /**
   * Show message request client capabilities.
   *
   * The show message request is sent from a server to a client to ask the
   * client to display a particular message in the user interface. In addition
   * to the show message notification the request allows to pass actions and to
   * wait for an answer from the client.
   *
   * Client Capability:
   * - property path (optional): window.showMessage
   * - property type: ShowMessageRequestClientCapabilities defined as follows:
   *
   * export interface ShowMessageRequestClientCapabilities {
   *   messageActionItem?: {
   *     additionalPropertiesSupport?: boolean;
   *   };
   * }
   */
  struct ShowMessageRequestClientCapabilities {

    /**
     * Capabilities specific to the `MessageActionItem` type.
     *
     * messageActionItem?: {
     *   additionalPropertiesSupport?: boolean;
     * };
     */
    optional_ptr<MessageActionItemCapabilities> messageActionItem;
  };

  /**
   * interface MessageActionItem {
   *   title: string;
   * }
   */
  struct MessageActionItem {

    /**
     * A short title like 'Retry', 'Open Log' etc.
     *
     * title: string;
     */
    string title;
  };

  /**
   * Request:
   * - method: ‘window/showMessageRequest’
   * - params: ShowMessageRequestParams defined as follows:
   *
   * interface ShowMessageRequestParams {
   *   type: MessageType;
   *   message: string;
   *   actions?: MessageActionItem[];
   * }
   *
   * Response:
   * - result: the selected MessageActionItem | null if none got selected.
   * - error: code and message set in case an exception happens during showing a
   *   message.
   */
  struct ShowMessageRequestParams {

    /**
     * The message type. See {@link MessageType}
     *
     * type: MessageType;
     */
    MessageType type;

    /**
     * The actual message
     *
     * message: string;
     */
    string message;

    /**
     * The message action items to present.
     *
     * actions?: MessageActionItem[];
     */
    optional_ptr_vector<MessageActionItem> actions;
  };

  /**
   * Client capabilities for the show document request.
   *
   * The show document request is sent from a server to a client to ask the
   * client to display a particular resource referenced by a URI in the user
   * interface.
   *
   * Client Capability:
   * - property path (optional): window.showDocument
   * - property type: ShowDocumentClientCapabilities defined as follows:
   *
   * export interface ShowDocumentClientCapabilities {
   *   support: boolean;
   * }
   *
   * @since 3.16.0
   */
  struct ShowDocumentClientCapabilities {

    /**
     * The client has support for the show document
     * request.
     *
     * support: boolean;
     */
    boolean support;
  };

  /**
   * Params to show a resource.
   *
   * Request:
   * - method: ‘window/showDocument’
   * - params: ShowDocumentParams defined as follows:
   *
   * export interface ShowDocumentParams {
   *   uri: URI;
   *   external?: boolean;
   *   takeFocus?: boolean;
   *   selection?: Range;
   * }
   *
   * @since 3.16.0
   */
  struct ShowDocumentParams {

    /**
     * The uri to show.
     *
     * uri: URI;
     */
    URI uri;

    /**
     * Indicates to show the resource in an external program. To show, for
     * example, `https://code.visualstudio.com/` in the default WEB browser set
     * `external` to `true`.
     *
     * external?: boolean;
     */
    std::optional<boolean> external;

    /**
     * An optional property to indicate whether the editor showing the document
     * should take focus or not. Clients might ignore this property if an
     * external program is started.
     *
     * takeFocus?: boolean;
     */
    std::optional<boolean> takeFocus;

    /**
     * An optional selection range if the document is a text document. Clients
     * might ignore the property if an external program is started or the file
     * is not a text file.
     *
     * selection?: Range;
     */
    optional_ptr<Range> selection;
  };

  /**
   * The result of an show document request.
   *
   * Response:
   * - result: ShowDocumentResult defined as follows:
   *
   * export interface ShowDocumentResult {
   *   success: boolean;
   * }
   *
   * - error: code and message set in case an exception happens during showing a
   *   document.
   *
   * @since 3.16.0
   */
  struct ShowDocumentResult {

    /**
     * A boolean indicating if the show was successful.
     *
     * success: boolean;
     */
    boolean success;
  };

  /**
   * The log message notification is sent from the server to the client to ask
   * the client to log a particular message.
   *
   * Notification:
   * - method: ‘window/logMessage’
   * - params: LogMessageParams defined as follows:
   *
   * interface LogMessageParams {
   *   type: MessageType;
   *   message: string;
   * }
   */
  struct LogMessageParams {

    /**
     * The message type. See {@link MessageType}
     *
     * type: MessageType;
     */
    MessageType type;

    /**
     * The actual message
     *
     * message: string;
     */
    string message;
  };

  /**
   * The window/workDoneProgress/create request is sent from the server to the
   * client to ask the client to create a work done progress.
   *
   * Client Capability:
   * - property name (optional): window.workDoneProgress
   * - property type: boolean
   *
   * Request:
   * - method: ‘window/workDoneProgress/create’
   * - params: WorkDoneProgressCreateParams defined as follows:
   *
   * export interface WorkDoneProgressCreateParams {
   *   token: ProgressToken;
   * }
   *
   * Response:
   * - result: void
   * - error: code and message set in case an exception happens during the
   *   ‘window/workDoneProgress/create’ request. In case an error occurs a
   *   server must not send any progress notification using the token provided
   *   in the WorkDoneProgressCreateParams.
   */
  struct WorkDoneProgressCreateParams {

    /**
     * The token to be used to report progress.
     *
     * token: ProgressToken;
     */
    std::unique_ptr<ProgressToken> token;
  };

  /**
   * The window/workDoneProgress/cancel notification is sent from the client to
   * the server to cancel a progress initiated on the server side using the
   * window/workDoneProgress/create. The progress need not be marked as
   * cancellable to be cancelled and a client may cancel a progress for any
   * number of reasons: in case of error, reloading a workspace etc.
   *
   * Notification:
   * - method: ‘window/workDoneProgress/cancel’
   * - params: WorkDoneProgressCancelParams defined as follows:
   *
   * export interface WorkDoneProgressCancelParams {
   *   token: ProgressToken;
   * }
   */
  struct WorkDoneProgressCancelParams {

    /**
     * The token to be used to report progress.
     *
     * token: ProgressToken;
     */
    std::unique_ptr<ProgressToken> token;
  };

  // ---------------------------------------------------------------------------
  // Telemetry Notification
  // ---------------------------------------------------------------------------
  // The telemetry notification is sent from the server to the client to ask the
  // client to log a telemetry event. The protocol doesn’t specify the payload
  // since no interpretation of the data happens in the protocol. Most clients
  // even don’t handle the event directly but forward them to the extensions
  // owing the corresponding server issuing the event.
  //
  // Notification:
  // - method: ‘telemetry/event’
  // - params: ‘object’ | ‘array’;
  // ---------------------------------------------------------------------------

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
    optional_ptr<TextDocumentSyncClientCapabilities> synchronization;

    /**
     * Capabilities specific to the `textDocument/completion` request.
     *
     * completion?: CompletionClientCapabilities;
     */
    optional_ptr<CompletionClientCapabilities> completion;

    /**
     * Capabilities specific to the `textDocument/hover` request.
     *
     * hover?: HoverClientCapabilities;
     */
    optional_ptr<HoverClientCapabilities> hover;

    /**
     * Capabilities specific to the `textDocument/signatureHelp` request.
     *
     * signatureHelp?: SignatureHelpClientCapabilities;
     */
    optional_ptr<SignatureHelpClientCapabilities> signatureHelp;

    /**
     * Capabilities specific to the `textDocument/declaration` request.
     *
     * declaration?: DeclarationClientCapabilities;
     *
     * @since 3.14.0
     */
    std::optional<std::unique_ptr<DeclarationClientCapabilities>> declaration;

    /**
     * Capabilities specific to the `textDocument/definition` request.
     *
     * definition?: DefinitionClientCapabilities;
     */
    optional_ptr<DefinitionClientCapabilities> definition;

    /**
     * Capabilities specific to the `textDocument/typeDefinition` request.
     *
     * typeDefinition?: TypeDefinitionClientCapabilities;
     *
     * @since 3.6.0
     */
    optional_ptr<TypeDefinitionClientCapabilities> typeDefinition;

    /**
     * Capabilities specific to the `textDocument/implementation` request.
     *
     * implementation?: ImplementationClientCapabilities;
     *
     * @since 3.6.0
     */
    optional_ptr<ImplementationClientCapabilities> implementation;

    /**
     * Capabilities specific to the `textDocument/references` request.
     *
     * references?: ReferenceClientCapabilities;
     */
    optional_ptr<ReferenceClientCapabilities> references;

    /**
     * Capabilities specific to the `textDocument/documentHighlight` request.
     *
     * documentHighlight?: DocumentHighlightClientCapabilities;
     */
    optional_ptr<DocumentHighlightClientCapabilities> documentHighlight;

    /**
     * Capabilities specific to the `textDocument/documentSymbol` request.
     *
     * documentSymbol?: DocumentSymbolClientCapabilities;
     */
    optional_ptr<DocumentSymbolClientCapabilities> documentSymbol;

    /**
     * Capabilities specific to the `textDocument/codeAction` request.
     *
     * codeAction?: CodeActionClientCapabilities;
     */
    optional_ptr<CodeActionClientCapabilities> codeAction;

    /**
     * Capabilities specific to the `textDocument/codeLens` request.
     *
     * codeLens?: CodeLensClientCapabilities;
     */
    optional_ptr<CodeLensClientCapabilities> codeLens;

    /**
     * Capabilities specific to the `textDocument/documentLink` request.
     *
     * documentLink?: DocumentLinkClientCapabilities;
     */
    optional_ptr<DocumentLinkClientCapabilities> documentLink;

    /**
     * Capabilities specific to the `textDocument/documentColor` and the
     * `textDocument/colorPresentation` request.
     *
     * colorProvider?: DocumentColorClientCapabilities;
     *
     * @since 3.6.0
     */
    optional_ptr<DocumentColorClientCapabilities> colorProvider;

    /**
     * Capabilities specific to the `textDocument/formatting` request.
     *
     * formatting?: DocumentFormattingClientCapabilities;
     */
    optional_ptr<DocumentFormattingClientCapabilities> formatting;

    /**
     * Capabilities specific to the `textDocument/rangeFormatting` request.
     *
     * rangeFormatting?: DocumentRangeFormattingClientCapabilities;
     */
    optional_ptr<DocumentRangeFormattingClientCapabilities> rangeFormatting;

    /**
     * Capabilities specific to the `textDocument/onTypeFormatting` request.
     *
     * onTypeFormatting?: DocumentOnTypeFormattingClientCapabilities;
     */
    optional_ptr<DocumentOnTypeFormattingClientCapabilities> onTypeFormatting;

    /**
     * Capabilities specific to the `textDocument/rename` request.
     *
     * rename?: RenameClientCapabilities;
     */
    optional_ptr<RenameClientCapabilities> rename;

    /**
     * Capabilities specific to the `textDocument/publishDiagnostics`
     * notification.
     *
     * publishDiagnostics?: PublishDiagnosticsClientCapabilities;
     */
    optional_ptr<PublishDiagnosticsClientCapabilities> publishDiagnostics;

    /**
     * Capabilities specific to the `textDocument/foldingRange` request.
     *
     * foldingRange?: FoldingRangeClientCapabilities;
     *
     * @since 3.10.0
     */
    optional_ptr<FoldingRangeClientCapabilities> foldingRange;

    /**
     * Capabilities specific to the `textDocument/selectionRange` request.
     *
     * selectionRange?: SelectionRangeClientCapabilities;
     *
     * @since 3.15.0
     */
    optional_ptr<SelectionRangeClientCapabilities> selectionRange;

    /**
     * Capabilities specific to the `textDocument/linkedEditingRange` request.
     *
     * linkedEditingRange?: LinkedEditingRangeClientCapabilities;
     *
     * @since 3.16.0
     */
    optional_ptr<LinkedEditingRangeClientCapabilities> linkedEditingRange;

    /**
     * Capabilities specific to the various call hierarchy requests.
     *
     * callHierarchy?: CallHierarchyClientCapabilities;
     *
     * @since 3.16.0
     */
    optional_ptr<CallHierarchyClientCapabilities> callHierarchy;

    /**
     * Capabilities specific to the various semantic token requests.
     *
     * semanticTokens?: SemanticTokensClientCapabilities;
     *
     * @since 3.16.0
     */
    optional_ptr<SemanticTokensClientCapabilities> semanticTokens;

    /**
     * Capabilities specific to the `textDocument/moniker` request.
     *
     * moniker?: MonikerClientCapabilities;
     *
     * @since 3.16.0
     */
    optional_ptr<MonikerClientCapabilities> moniker;

    /**
     * Capabilities specific to the various type hierarchy requests.
     *
     * typeHierarchy?: TypeHierarchyClientCapabilities;
     *
     * @since 3.17.0
     */
    optional_ptr<TypeHierarchyClientCapabilities> typeHierarchy;

    /**
     * Capabilities specific to the `textDocument/inlineValue` request.
     *
     * inlineValue?: InlineValueClientCapabilities;
     *
     * @since 3.17.0
     */
    optional_ptr<InlineValueClientCapabilities> inlineValue;

    /**
     * Capabilities specific to the `textDocument/inlayHint` request.
     *
     * inlayHint?: InlayHintClientCapabilities;
     *
     * @since 3.17.0
     */
    optional_ptr<InlayHintClientCapabilities> inlayHint;

    /**
     * Capabilities specific to the diagnostic pull model.
     *
     * diagnostic?: DiagnosticClientCapabilities;
     *
     * @since 3.17.0
     */
    optional_ptr<DiagnosticClientCapabilities> diagnostic;
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

  auto notebookCellKindByName(const std::string &name) -> NotebookCellKind;

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
    optional_ptr<LSPObject> metadata;

    /**
     * Additional execution summary information if supported by the client.
     *
     * executionSummary?: ExecutionSummary;
     */
    optional_ptr<ExecutionSummary> executionSummary;
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
    optional_ptr<LSPObject> metadata;

    /**
     * The cells of a notebook.
     *
     * cells: NotebookCell[];
     */
    ptr_vector<NotebookCell> cells;
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
    std::variant<
      std::unique_ptr<NotebookTypeDocumentFilter>,
      std::unique_ptr<NotebookSchemeDocumentFilter>,
      std::unique_ptr<NotebookPatternDocumentFilter>
    > value;
  };

  enum class StringOrNotebookDocumentFilterType {
    STRING,
    NOTEBOOK_DOCUMENT_FILTER,
  };

  struct StringOrNotebookDocumentFilter {
    StringOrNotebookDocumentFilterType type;
    std::variant<
      string,
      std::unique_ptr<NotebookDocumentFilter>
    > value;
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
    optional_ptr<WorkspaceEditClientCapabilities> workspaceEdit;

    /**
     * Capabilities specific to the `workspace/didChangeConfiguration`
     * notification.
     *
     * didChangeConfiguration?: DidChangeConfigurationClientCapabilities;
     */
    optional_ptr<DidChangeConfigurationClientCapabilities> didChangeConfiguration;

    /**
     * Capabilities specific to the `workspace/didChangeWatchedFiles`
     * notification.
     *
     * didChangeWatchedFiles?: DidChangeWatchedFilesClientCapabilities;
     */
    optional_ptr<DidChangeWatchedFilesClientCapabilities> didChangeWatchedFiles;

    /**
     * Capabilities specific to the `workspace/symbol` request.
     *
     * symbol?: WorkspaceSymbolClientCapabilities;
     */
    optional_ptr<WorkspaceSymbolClientCapabilities> symbol;

    /**
     * Capabilities specific to the `workspace/executeCommand` request.
     *
     * executeCommand?: ExecuteCommandClientCapabilities;
     */
    optional_ptr<ExecuteCommandClientCapabilities> executeCommand;

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
    optional_ptr<SemanticTokensWorkspaceClientCapabilities> semanticTokens;

    /**
     * Capabilities specific to the code lens requests scoped to the
     * workspace.
     *
     * codeLens?: CodeLensWorkspaceClientCapabilities;
     *
     * @since 3.16.0
     */
    optional_ptr<CodeLensWorkspaceClientCapabilities> codeLens;

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
    optional_ptr<FileCapabilities> fileOperations;

    /**
     * Client workspace capabilities specific to inline values.
     *
     * inlineValue?: InlineValueWorkspaceClientCapabilities;
     *
     * @since 3.17.0
     */
    optional_ptr<InlineValueWorkspaceClientCapabilities> inlineValue;

    /**
     * Client workspace capabilities specific to inlay hints.
     *
     * inlayHint?: InlayHintWorkspaceClientCapabilities;
     *
     * @since 3.17.0
     */
    optional_ptr<InlayHintWorkspaceClientCapabilities> inlayHint;

    /**
     * Client workspace capabilities specific to diagnostics.
     *
     * diagnostics?: DiagnosticWorkspaceClientCapabilities;
     *
     * @since 3.17.0.
     */
    optional_ptr<DiagnosticWorkspaceClientCapabilities> diagnostics;
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
    optional_ptr<ShowMessageRequestClientCapabilities> showMessage;

    /**
     * Client capabilities for the show document request.
     *
     * showDocument?: ShowDocumentClientCapabilities;
     *
     * @since 3.16.0
     */
    optional_ptr<ShowDocumentClientCapabilities> showDocument;
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
    ptr_vector<string> retryOnContentModified;
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
    optional_ptr<StaleRequestSupport> staleRequestSupport;

    /**
     * Client capabilities specific to regular expressions.
     *
     * regularExpressions?: RegularExpressionsClientCapabilities;
     *
     * @since 3.16.0
     */
    optional_ptr<RegularExpressionsClientCapabilities> regularExpressions;

    /**
     * Client capabilities specific to the client's markdown parser.
     *
     * markdown?: MarkdownClientCapabilities;
     *
     * @since 3.16.0
     */
    optional_ptr<MarkdownClientCapabilities> markdown;

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
    optional_ptr_vector<PositionEncodingKind> positionEncodings;
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
    optional_ptr<WorkspaceCapabilities> workspace;

    /**
     * Text document specific client capabilities.
     *
     * textDocument?: TextDocumentClientCapabilities;
     */
    optional_ptr<TextDocumentClientCapabilities> textDocument;

    /**
     * Capabilities specific to the notebook document support.
     *
     * notebookDocument?: NotebookDocumentClientCapabilities;
     *
     * @since 3.17.0
     */
    optional_ptr<NotebookDocumentClientCapabilities> notebookDocument;

    /**
     * Window specific client capabilities.
     *
     * window?: {
     *   workDoneProgress?: boolean;
     *   showMessage?: ShowMessageRequestClientCapabilities;
     *   showDocument?: ShowDocumentClientCapabilities;
     * };
     */
    optional_ptr<WindowCapabilities> window;

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
    optional_ptr<GeneralCapabilities> general;

    /**
     * Experimental client capabilities.
     *
     * experimental?: LSPAny;
     */
    optional_ptr<LSPAny> experimental;
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

  auto textDocumentSyncKindByName(const std::string &name) -> TextDocumentSyncKind;

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
    std::variant<
      boolean,
      std::unique_ptr<SaveOptions>
    > value;
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
    optional_ptr<SaveOrOptions> save;
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
    std::variant<
      std::unique_ptr<TextDocumentSyncOptions>,
      TextDocumentSyncKind
    > value;
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
    std::variant<
      std::unique_ptr<PartialTextDocumentContentChangeEvent>,
      std::unique_ptr<WholeTextDocumentContentChangeEvent>
    > value;
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
    optional_ptr_vector<NotebookSelectorCell> cells;
  };

  struct CellsRequiredNotebookSelector {

    /**
     * The notebook to be synced. If a string value is provided it matches
     * against the notebook type. '*' matches every notebook.
     *
     * notebook: string | NotebookDocumentFilter;
     */
    optional_ptr<StringOrNotebookDocumentFilter> notebook;

    /**
     * The cells of the matching notebook to be synced.
     *
     * cells?: { language: string }[];
     */
    ptr_vector<NotebookSelectorCell> cells;
  };

  enum class NotebookSelectorType {
    NOTEBOOK_REQUIRED,
    CELLS_REQUIRED,
  };

  struct NotebookSelector {
    NotebookSelectorType type;
    std::variant<
      std::unique_ptr<NotebookRequiredNotebookSelector>,
      std::unique_ptr<CellsRequiredNotebookSelector>
    > value;
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
    ptr_vector<NotebookSelector> notebookSelector;

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
    ptr_vector<TextDocumentItem> cellTextDocuments;
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
    optional_ptr_vector<NotebookCell> cells;
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
    optional_ptr_vector<TextDocumentItem> didOpen;

    /**
     * Additional closed cell text documents.
     *
     * didClose?: TextDocumentIdentifier[];
     */
    optional_ptr_vector<TextDocumentIdentifier> didClose;
  };

  struct NotebookDocumentChangeEventCellsTextContent {

    /**
     * document: VersionedTextDocumentIdentifier;
     */
    std::unique_ptr<VersionedTextDocumentIdentifier> document;

    /**
     * changes: TextDocumentContentChangeEvent[];
     */
    ptr_vector<TextDocumentContentChangeEvent> changes;
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
    optional_ptr<NotebookDocumentChangeEventCellsStructure> structure;

    /**
     * Changes to notebook cells properties like its
     * kind, execution summary or metadata.
     *
     * data?: NotebookCell[];
     */
    optional_ptr_vector<NotebookCell> data;

    /**
     * Changes to the text content of notebook cells.
     *
     * textContent?: {
     *   document: VersionedTextDocumentIdentifier;
     *   changes: TextDocumentContentChangeEvent[];
     * }[];
     */
    optional_ptr_vector<NotebookDocumentChangeEventCellsTextContent> textContent;
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
    optional_ptr<LSPObject> metadata;

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
    optional_ptr<NotebookDocumentChangeEventCells> cells;
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
    ptr_vector<TextDocumentIdentifier> cellTextDocuments;
  };

  enum class NotebookDocumentSyncType {
    NOTEBOOK_DOCUMENT_SYNC_OPTIONS,
    NOTEBOOK_DOCUMENT_SYNC_REGISTRATION_OPTIONS,
  };

  struct NotebookDocumentSync {
    NotebookDocumentSyncType type;
    std::variant<
      std::unique_ptr<NotebookDocumentSyncOptions>,
      std::unique_ptr<NotebookDocumentSyncRegistrationOptions>
    > value;
  };

  enum class HoverProviderType {
    BOOLEAN,
    HOVER_OPTIONS,
  };

  struct HoverProvider {
    HoverProviderType type;
    std::variant<
      boolean,
      std::unique_ptr<HoverOptions>
    > value;
  };

  enum class DeclarationProviderType {
    BOOLEAN,
    DECLARATION_OPTIONS,
    DECLARATION_REGISTRATION_OPTIONS,
  };

  struct DeclarationProvider {
    DeclarationProviderType type;
    std::variant<
      boolean,
      std::unique_ptr<DeclarationOptions>,
      std::unique_ptr<DeclarationRegistrationOptions>
    > value;
  };

  enum class DefinitionProviderType {
    BOOLEAN,
    DEFINITION_OPTIONS,
  };

  struct DefinitionProvider {
    DefinitionProviderType type;
    std::variant<
      boolean,
      std::unique_ptr<DefinitionOptions>
    > value;
  };

  enum class TypeDefinitionProviderType {
    BOOLEAN,
    TYPE_DEFINITION_OPTIONS,
    TYPE_DEFINITION_REGISTRATION_OPTIONS,
  };

  struct TypeDefinitionProvider {
    TypeDefinitionProviderType type;
    std::variant<
      boolean,
      std::unique_ptr<TypeDefinitionOptions>,
      std::unique_ptr<TypeDefinitionRegistrationOptions>
    > value;
  };

  enum class ImplementationProviderType {
    BOOLEAN,
    IMPLEMENTATION_OPTIONS,
    IMPLEMENTATION_REGISTRATION_OPTIONS,
  };

  struct ImplementationProvider {
    ImplementationProviderType type;
    std::variant<
      boolean,
      std::unique_ptr<ImplementationOptions>,
      std::unique_ptr<ImplementationRegistrationOptions>
    > value;
  };

  enum class ReferencesProviderType {
    BOOLEAN,
    REFERENCE_OPTIONS,
  };

  struct ReferencesProvider {
    ReferencesProviderType type;
    std::variant<
      boolean,
      std::unique_ptr<ReferenceOptions>
    > value;
  };

  enum class DocumentHighlightProviderType {
    BOOLEAN,
    DOCUMENT_HIGHLIGHT_OPTIONS,
  };

  struct DocumentHighlightProvider {
    DocumentHighlightProviderType type;
    std::variant<
      boolean,
      std::unique_ptr<DocumentHighlightOptions>
    > value;
  };

  enum class DocumentSymbolProviderType {
    BOOLEAN,
    DOCUMENT_SYMBOL_OPTIONS,
  };

  struct DocumentSymbolProvider {
    DocumentSymbolProviderType type;
    std::variant<
      boolean,
      std::unique_ptr<DocumentSymbolOptions>
    > value;
  };

  enum class CodeActionProviderType {
    BOOLEAN,
    CODE_ACTION_OPTIONS,
  };

  struct CodeActionProvider {
    CodeActionProviderType type;
    std::variant<
      boolean,
      std::unique_ptr<CodeActionOptions>
    > value;
  };

  enum class ColorProviderType {
    BOOLEAN,
    DOCUMENT_COLOR_OPTIONS,
    DOCUMENT_COLOR_REGISTRATION_OPTIONS,
  };

  struct ColorProvider {
    ColorProviderType type;
    std::variant<
      boolean,
      std::unique_ptr<DocumentColorOptions>,
      std::unique_ptr<DocumentColorRegistrationOptions>
    > value;
  };

  enum class DocumentFormattingProviderType {
    BOOLEAN,
    DOCUMENT_FORMATTING_OPTIONS,
  };

  struct DocumentFormattingProvider {
    DocumentFormattingProviderType type;
    std::variant<
      boolean,
      std::unique_ptr<DocumentFormattingOptions>
    > value;
  };

  enum class DocumentRangeFormattingProviderType {
    BOOLEAN,
    DOCUMENT_RANGE_FORMATTING_OPTIONS,
  };

  struct DocumentRangeFormattingProvider {
    DocumentRangeFormattingProviderType type;
    std::variant<
      boolean,
      std::unique_ptr<DocumentRangeFormattingOptions>
    > value;
  };

  enum class RenameProviderType {
    BOOLEAN,
    RENAME_OPTIONS,
  };

  struct RenameProvider {
    RenameProviderType type;
    std::variant<
      boolean,
      std::unique_ptr<RenameOptions>
    > value;
  };

  enum class FoldingRangeProviderType {
    BOOLEAN,
    FOLDING_RANGE_OPTIONS,
    FOLDING_RANGE_REGISTRATION_OPTIONS,
  };

  struct FoldingRangeProvider {
    FoldingRangeProviderType type;
    std::variant<
      boolean,
      std::unique_ptr<FoldingRangeOptions>,
      std::unique_ptr<FoldingRangeRegistrationOptions>
    > value;
  };

  enum class SelectionRangeProviderType {
    BOOLEAN,
    SELECTION_RANGE_OPTIONS,
    SELECTION_RANGE_REGISTRATION_OPTIONS,
  };

  struct SelectionRangeProvider {
    SelectionRangeProviderType type;
    std::variant<
      boolean,
      std::unique_ptr<SelectionRangeOptions>,
      std::unique_ptr<SelectionRangeRegistrationOptions>
    > value;
  };

  enum class LinkedEditingRangeProviderType {
    BOOLEAN,
    LINKED_EDITING_RANGE_OPTIONS,
    LINKED_EDITING_RANGE_REGISTRATION_OPTIONS,
  };

  struct LinkedEditingRangeProvider {
    LinkedEditingRangeProviderType type;
    std::variant<
      boolean,
      std::unique_ptr<LinkedEditingRangeOptions>,
      std::unique_ptr<LinkedEditingRangeRegistrationOptions>
    > value;
  };

  enum class CallHierarchyProviderType {
    BOOLEAN,
    CALL_HIERARCHY_OPTIONS,
    CALL_HIERARCHY_REGISTRATION_OPTIONS,
  };

  struct CallHierarchyProvider {
    CallHierarchyProviderType type;
    std::variant<
      boolean,
      std::unique_ptr<CallHierarchyOptions>,
      std::unique_ptr<CallHierarchyRegistrationOptions>
    > value;
  };

  enum class SemanticTokensProviderType {
    SEMANTIC_TOKENS_OPTIONS,
    SEMANTIC_TOKENS_REGISTRATION_OPTIONS,
  };

  struct SemanticTokensProvider {
    SemanticTokensProviderType type;
    std::variant<
      std::unique_ptr<SemanticTokensOptions>,
      std::unique_ptr<SemanticTokensRegistrationOptions>
    > value;
  };

  enum class MonikerProviderType {
    BOOLEAN,
    MONIKER_OPTIONS,
    MONIKER_REGISTRATION_OPTIONS,
  };

  struct MonikerProvider {
    MonikerProviderType type;
    std::variant<
      boolean,
      std::unique_ptr<MonikerOptions>,
      std::unique_ptr<MonikerRegistrationOptions>
    > value;
  };

  enum class TypeHierarchyProviderType {
    BOOLEAN,
    TYPE_HIERARCHY_OPTIONS,
    TYPE_HIERARCHY_REGISTRATION_OPTIONS,
  };

  struct TypeHierarchyProvider {
    TypeHierarchyProviderType type;
    std::variant<
      boolean,
      std::unique_ptr<TypeHierarchyOptions>,
      std::unique_ptr<TypeHierarchyRegistrationOptions>
    > value;
  };

  enum class InlineValueProviderType {
    BOOLEAN,
    INLINE_VALUE_OPTIONS,
    INLINE_VALUE_REGISTRATION_OPTIONS,
  };

  struct InlineValueProvider {
    InlineValueProviderType type;
    std::variant<
      boolean,
      std::unique_ptr<InlineValueOptions>,
      std::unique_ptr<InlineValueRegistrationOptions>
    > value;
  };

  enum class InlayHintProviderType {
    BOOLEAN,
    INLAY_HINT_OPTIONS,
    INLAY_HINT_REGISTRATION_OPTIONS,
  };

  struct InlayHintProvider {
    InlayHintProviderType type;
    std::variant<
      boolean,
      std::unique_ptr<InlayHintOptions>,
      std::unique_ptr<InlayHintRegistrationOptions>
    > value;
  };

  enum class DiagnosticProviderType {
    DIAGNOSTIC_OPTIONS,
    DIAGNOSTIC_REGISTRATION_OPTIONS,
  };

  struct DiagnosticProvider {
    DiagnosticProviderType type;
    std::variant<
      std::unique_ptr<DiagnosticOptions>,
      std::unique_ptr<DiagnosticRegistrationOptions>
    > value;
  };

  enum class WorkspaceSymbolProviderType {
    BOOLEAN,
    WORKSPACE_SYMBOL_OPTIONS,
  };

  struct WorkspaceSymbolProvider {
    WorkspaceSymbolProviderType type;
    std::variant<
      boolean,
      std::unique_ptr<WorkspaceSymbolOptions>
    > value;
  };

  struct FileOperations {

    /**
     * The server is interested in receiving didCreateFiles
     * notifications.
     *
     * didCreate?: FileOperationRegistrationOptions;
     */
    optional_ptr<FileOperationRegistrationOptions> didCreate;

    /**
     * The server is interested in receiving willCreateFiles requests.
     *
     * willCreate?: FileOperationRegistrationOptions;
     */
    optional_ptr<FileOperationRegistrationOptions> willCreate;

    /**
     * The server is interested in receiving didRenameFiles
     * notifications.
     *
     * didRename?: FileOperationRegistrationOptions;
     */
    optional_ptr<FileOperationRegistrationOptions> didRename;

    /**
     * The server is interested in receiving willRenameFiles requests.
     *
     * willRename?: FileOperationRegistrationOptions;
     */
    optional_ptr<FileOperationRegistrationOptions> willRename;

    /**
     * The server is interested in receiving didDeleteFiles file
     * notifications.
     *
     * didDelete?: FileOperationRegistrationOptions;
     */
    optional_ptr<FileOperationRegistrationOptions> didDelete;

    /**
     * The server is interested in receiving willDeleteFiles file
     * requests.
     *
     * willDelete?: FileOperationRegistrationOptions;
     */
    optional_ptr<FileOperationRegistrationOptions> willDelete;
  };

  struct WorkspaceProvider {

    /**
     * The server supports workspace folder.
     *
     * workspaceFolders?: WorkspaceFoldersServerCapabilities;
     *
     * @since 3.6.0
     */
    optional_ptr<WorkspaceFoldersServerCapabilities> workspaceFolders;

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
    optional_ptr<FileOperations> fileOperations;
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
    optional_ptr<PositionEncodingKind> positionEncoding;

    /**
     * Defines how text documents are synced. Is either a detailed structure
     * defining each notification or for backwards compatibility the
     * TextDocumentSyncKind number. If omitted it defaults to
     * `TextDocumentSyncKind.None`.
     *
     * textDocumentSync?: TextDocumentSyncOptions | TextDocumentSyncKind;
     */
    optional_ptr<TextDocumentSync> textDocumentSync;

    /**
     * Defines how notebook documents are synced.
     *
     * notebookDocumentSync?: NotebookDocumentSyncOptions
     *   | NotebookDocumentSyncRegistrationOptions;
     *
     * @since 3.17.0
     */
    optional_ptr<NotebookDocumentSync> notebookDocumentSync;

    /**
     * The server provides completion support.
     *
     * completionProvider?: CompletionOptions;
     */
    optional_ptr<CompletionOptions> completionProvider;

    /**
     * The server provides hover support.
     *
     * hoverProvider?: boolean | HoverOptions;
     */
    optional_ptr<HoverProvider> hoverProvider;

    /**
     * The server provides signature help support.
     *
     * signatureHelpProvider?: SignatureHelpOptions;
     */
    optional_ptr<SignatureHelpOptions> signatureHelpProvider;

    /**
     * The server provides go to declaration support.
     *
     * declarationProvider?: boolean | DeclarationOptions
     *   | DeclarationRegistrationOptions;
     *
     * @since 3.14.0
     */
    optional_ptr<DeclarationProvider> declarationProvider;

    /**
     * The server provides goto definition support.
     *
     * definitionProvider?: boolean | DefinitionOptions;
     */
    optional_ptr<DefinitionProvider> definitionProvider;

    /**
     * The server provides goto type definition support.
     *
     * typeDefinitionProvider?: boolean | TypeDefinitionOptions
     *   | TypeDefinitionRegistrationOptions;
     *
     * @since 3.6.0
     */
    optional_ptr<TypeDefinitionProvider> typeDefinitionProvider;

    /**
     * The server provides goto implementation support.
     *
     * implementationProvider?: boolean | ImplementationOptions
     *   | ImplementationRegistrationOptions;
     *
     * @since 3.6.0
     */
    optional_ptr<ImplementationProvider> implementationProvider;

    /**
     * The server provides find references support.
     *
     * referencesProvider?: boolean | ReferenceOptions;
     */
    optional_ptr<ReferencesProvider> referencesProvider;

    /**
     * The server provides document highlight support.
     *
     * documentHighlightProvider?: boolean | DocumentHighlightOptions;
     */
    optional_ptr<DocumentHighlightProvider> documentHighlightProvider;

    /**
     * The server provides document symbol support.
     *
     * documentSymbolProvider?: boolean | DocumentSymbolOptions;
     */
    optional_ptr<DocumentSymbolProvider> documentSymbolProvider;

    /**
     * The server provides code actions. The `CodeActionOptions` return type is
     * only valid if the client signals code action literal support via the
     * property `textDocument.codeAction.codeActionLiteralSupport`.
     *
     * codeActionProvider?: boolean | CodeActionOptions;
     */
    optional_ptr<CodeActionProvider> codeActionProvider;

    /**
     * The server provides code lens.
     *
     * codeLensProvider?: CodeLensOptions;
     */
    optional_ptr<CodeLensOptions> codeLensProvider;

    /**
     * The server provides document link support.
     *
     * documentLinkProvider?: DocumentLinkOptions;
     */
    optional_ptr<DocumentLinkOptions> documentLinkProvider;

    /**
     * The server provides color provider support.
     *
     * colorProvider?: boolean | DocumentColorOptions
     *   | DocumentColorRegistrationOptions;
     *
     * @since 3.6.0
     */
    optional_ptr<ColorProvider> colorProvider;

    /**
     * The server provides document formatting.
     *
     * documentFormattingProvider?: boolean | DocumentFormattingOptions;
     */
    optional_ptr<DocumentFormattingProvider> documentFormattingProvider;

    /**
     * The server provides document range formatting.
     *
     * documentRangeFormattingProvider?: boolean | DocumentRangeFormattingOptions;
     */
    optional_ptr<DocumentRangeFormattingProvider> documentRangeFormattingProvider;

    /**
     * The server provides document formatting on typing.
     *
     * documentOnTypeFormattingProvider?: DocumentOnTypeFormattingOptions;
     */
    optional_ptr<DocumentOnTypeFormattingOptions> documentOnTypeFormattingProvider;

    /**
     * The server provides rename support. RenameOptions may only be specified
     * if the client states that it supports `prepareSupport` in its initial
     * `initialize` request.
     *
     * renameProvider?: boolean | RenameOptions;
     */
    optional_ptr<RenameProvider> renameProvider;

    /**
     * The server provides folding provider support.
     *
     * foldingRangeProvider?: boolean | FoldingRangeOptions
     *   | FoldingRangeRegistrationOptions;
     *
     * @since 3.10.0
     */
    optional_ptr<FoldingRangeProvider> foldingRangeProvider;

    /**
     * The server provides execute command support.
     *
     * executeCommandProvider?: ExecuteCommandOptions;
     */
    optional_ptr<ExecuteCommandOptions> executeCommandProvider;

    /**
     * The server provides selection range support.
     *
     * selectionRangeProvider?: boolean | SelectionRangeOptions
     *   | SelectionRangeRegistrationOptions;
     *
     * @since 3.15.0
     */
    optional_ptr<SelectionRangeProvider> selectionRangeProvider;

    /**
     * The server provides linked editing range support.
     *
     * linkedEditingRangeProvider?: boolean | LinkedEditingRangeOptions
     *   | LinkedEditingRangeRegistrationOptions;
     *
     * @since 3.16.0
     */
    optional_ptr<LinkedEditingRangeProvider> linkedEditingRangeProvider;

    /**
     * The server provides call hierarchy support.
     *
     * callHierarchyProvider?: boolean | CallHierarchyOptions
     *   | CallHierarchyRegistrationOptions;
     *
     * @since 3.16.0
     */
    optional_ptr<CallHierarchyProvider> callHierarchyProvider;

    /**
     * The server provides semantic tokens support.
     *
     * semanticTokensProvider?: SemanticTokensOptions
     *   | SemanticTokensRegistrationOptions;
     *
     * @since 3.16.0
     */
    optional_ptr<SemanticTokensProvider> semanticTokensProvider;

    /**
     * Whether server provides moniker support.
     *
     * monikerProvider?: boolean | MonikerOptions | MonikerRegistrationOptions;
     *
     * @since 3.16.0
     */
    optional_ptr<MonikerProvider> monikerProvider;

    /**
     * The server provides type hierarchy support.
     *
     * typeHierarchyProvider?: boolean | TypeHierarchyOptions
     *   | TypeHierarchyRegistrationOptions;
     *
     * @since 3.17.0
     */
    optional_ptr<TypeHierarchyProvider> typeHierarchyProvider;

    /**
     * The server provides inline values.
     *
     * inlineValueProvider?: boolean | InlineValueOptions
     *   | InlineValueRegistrationOptions;
     *
     * @since 3.17.0
     */
    optional_ptr<InlineValueProvider> inlineValueProvider;

    /**
     * The server provides inlay hints.
     *
     * inlayHintProvider?: boolean | InlayHintOptions
     *   | InlayHintRegistrationOptions;
     *
     * @since 3.17.0
     */
    optional_ptr<InlayHintProvider> inlayHintProvider;

    /**
     * The server has support for pull model diagnostics.
     *
     * diagnosticProvider?: DiagnosticOptions | DiagnosticRegistrationOptions;
     *
     * @since 3.17.0
     */
    optional_ptr<DiagnosticProvider> diagnosticProvider;

    /**
     * The server provides workspace symbol support.
     *
     * workspaceSymbolProvider?: boolean | WorkspaceSymbolOptions;
     */
    optional_ptr<WorkspaceSymbolProvider> workspaceSymbolProvider;

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
    optional_ptr<WorkspaceProvider> workspace;

    /**
     * Experimental server capabilities.
     *
     * experimental?: LSPAny;
     */
    optional_ptr<LSPAny> experimental;
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
    optional_ptr<LSPAny> registerOptions;
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
    ptr_vector<Registration> registrations;
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
    ptr_vector<Unregistration> unregisterations;  //<- Typo is part of spec!
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
    optional_ptr<ServerInfo> serverInfo;
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

  auto initializeErrorCodesByName(const std::string &name) -> InitializeErrorCodes;

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
    optional_ptr<ClientInfo> clientInfo;

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
    optional_ptr<LSPAny> initializationOptions;

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
    optional_ptr_vector<WorkspaceFolder> workspaceFolders;
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
    ptr_vector<TextDocumentContentChangeEvent> contentChanges;
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

  auto textDocumentSaveReasonByName(const std::string &name) -> TextDocumentSaveReason;

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

  enum class RequestMethod {
    INITIALIZE,
  };

  extern std::map<RequestMethod, std::string> RequestMethodNames;

  extern std::map<RequestMethod, std::string> RequestMethodValues;

  auto requestMethodByName(const std::string &name) -> RequestMethod;

  auto requestMethodByValue(const std::string &value) -> RequestMethod;

  enum class NotificationMethod {
    INITIALIZED,
    DID_OPEN_TEXT_DOCUMENT,
    DID_CHANGE_TEXT_DOCUMENT,
    DID_SAVE_TEXT_DOCUMENT,
    DID_CLOSE_TEXT_DOCUMENT,
  };

  extern std::map<NotificationMethod, std::string> NotificationMethodNames;

  extern std::map<NotificationMethod, std::string> NotificationMethodValues;

  auto notificationMethodByName(const std::string &name) -> NotificationMethod;

  auto notificationMethodByValue(const std::string &value) -> NotificationMethod;

  // Implementation Considerations
  // Language servers usually run in a separate process and clients communicate
  // with them in an asynchronous fashion. Additionally clients usually allow
  // users to interact with the source code even if request results are pending.
  // We recommend the following implementation pattern to avoid that clients
  // apply outdated response results:
  // - if a client sends a request to the server and the client state changes in
  //   a way that it invalidates the response it should do the following:
  //   - cancel the server request and ignore the result if the result is not
  //     useful for the client anymore. If necessary the client should resend
  //     the request.
  //   - keep the request running if the client can still make use of the result
  //     by, for example, transforming it to a new result by applying the state
  //     change to the result.
  // - servers should therefore not decide by themselves to cancel requests
  //   simply due to that fact that a state change notification is detected in
  //   the queue. As said the result could still be useful for the client.
  // - if a server detects an internal state change (for example, a project
  //   context changed) that invalidates the result of a request in execution
  //   the server can error these requests with ContentModified. If clients
  //   receive a ContentModified error, it generally should not show it in the
  //   UI for the end-user. Clients can resend the request if they know how to
  //   do so. It should be noted that for all position based requests it might
  //   be especially hard for clients to re-craft a request.
  // - a client should not send resolve requests for out of date objects (for
  //   example, code lenses, …). If a server receives a resolve request for an
  //   out of date object the server can error these requests with
  //   ContentModified.
  // - if a client notices that a server exits unexpectedly, it should try to
  //   restart the server. However clients should be careful not to restart a
  //   crashing server endlessly. VS Code, for example, doesn’t restart a server
  //   which has crashed 5 times in the last 180 seconds.
  //
  // Servers usually support different communication channels (e.g. stdio,
  // pipes, …). To ease the usage of servers in different clients it is highly
  // recommended that a server implementation supports the following command
  // line arguments to pick the communication channel:
  // - stdio: uses stdio as the communication channel.
  // - pipe: use pipes (Windows) or socket files (Linux, Mac) as the
  //   communication channel. The pipe / socket file name is passed as the next
  //   arg or with --pipe=.
  // - socket: uses a socket as the communication channel. The port is passed as
  //   next arg or with --port=.
  // - node-ipc: use node IPC communication between the client and the server.
  //   This is only supported if both client and server run under node.
  //
  // To support the case that the editor starting a server crashes an editor
  // should also pass its process id to the server. This allows the server to
  // monitor the editor process and to shutdown itself if the editor process
  // dies. The process id passed on the command line should be the same as the
  // one passed in the initialize parameters. The command line argument to use
  // is --clientProcessId.

} // namespace LCompilers::LanguageServerProtocol

#endif // LCOMPILERS_LSP_SPECIFICATION_H
