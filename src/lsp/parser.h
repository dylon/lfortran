#ifndef LCOMPILERS_LSP_AST_PARSER_H
#define LCOMPILERS_LSP_AST_PARSER_H

#include <memory>
#include <optional>
#include <vector>

#include <lsp/lexer.h>
#include <lsp/parser_declarations.h>

namespace LCompilers::LanguageServiceProvider::AST {

  template <typename T>
  struct DelimitedListElement {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<T> value;
  };

  template <typename T>
  struct DelimitedList {
    std::unique_ptr<T> first;
    std::vector<std::unique_ptr<DelimitedListElement<T>>> rest;
  };

  template <typename T>
  struct OptionallyDelimitedListElement {
    std::optional<std::unique_ptr<Token>> delimiterToken;  // ","
    std::unique_ptr<T> value;
  };

  template <typename T>
  struct OptionallyDelimitedList {
    std::unique_ptr<T> first;
    std::vector<std::unique_ptr<OptionallyDelimitedListElement<T>>> rest;
  };

  struct Label {
    // Constraint: At least one digit in a label must be nonzero.
    std::unique_ptr<Token> token;  // ( digit ){1,5}
  };

  struct Sign {
    std::unique_ptr<Token> token;  // ("+" | "-")
  };

  struct DigitString {
    std::unique_ptr<Token> token;  // ( digit )+
  };

  struct SignedDigitString {
    std::optional<std::unique_ptr<Sign>> sign;
    std::unique_ptr<DigitString> digitString;
  };

  struct ScalarIntConstantName {
    std::unique_ptr<Token> token;  // [a-zA-Z][a-zA-Z_0-9]*
  };

  enum class KindParamType {
    DIGIT_STRING,
    SCALAR_INT_CONSTANT_NAME,
  };

  struct KindParam {
    // Constraint: The value of KindParam must be nonnegative.
    // -----------------------------------------------------------------------
    // Constraint: The value of KindParam must specify a representation method
    // that exists on the processor.
    // -----------------------------------------------------------------------
    KindParamType type;
    union {
      std::unique_ptr<DigitString> digitString;
      std::unique_ptr<ScalarIntConstantName> scalarIntConstantName;
    };
  };

  struct Underscore {
    std::unique_ptr<Token> token;  // "_"
  };

  struct LeadingKind {
    std::unique_ptr<KindParam> kindParam;
    std::unique_ptr<Underscore> underscore;
  };

  struct TrailingKind {
    std::unique_ptr<Underscore> underscore;
    std::unique_ptr<KindParam> kindParam;
  };

  struct IntLiteralConstant {
    std::unique_ptr<DigitString> digitString;
    std::optional<std::unique_ptr<TrailingKind>> kind;
  };

  struct SignedIntLiteralConstant {
    std::optional<std::unique_ptr<Sign>> sign;
    std::unique_ptr<IntLiteralConstant> intLiteralConstant;
  };

  struct BinaryConstant {
    std::unique_ptr<Token> token;  // "B" ( "'" [01]+ "'" | '"' [01]+ '"' )
  };

  struct OctalConstant {
    std::unique_ptr<Token> token;  // "O" ( "'" [0-7]+ "'" | '"' [0-7]+ '"' )
  };

  struct HexConstant {
    std::unique_ptr<Token> token;  // "Z" ( "'" [0-9a-fA-F]+ "'" | '"' [0-9a-fA-F]+ '"' )
  };

  enum class BozLiteralConstantType {
    BINARY_CONSTANT,
    OCTAL_CONSTANT,
    HEX_CONSTANT,
  };

  struct BozLiteralConstant {
    BozLiteralConstantType type;
    union {
      std::unique_ptr<BinaryConstant> binaryConstant;
      std::unique_ptr<OctalConstant> octalConstant;
      std::unique_ptr<HexConstant> hexConstant;
    };
  };

  struct Exponent {
    std::unique_ptr<SignedDigitString> signedDigitString;
  };

  struct ExponentLetter {
    std::unique_ptr<Token> token;  // ( "+" | "-" )
  };

  struct RightPartialSignificand {
    std::unique_ptr<DigitString> integralPart;
    std::unique_ptr<Token> decimalPoint;  // "."
    std::optional<std::unique_ptr<DigitString>> fractionalPart;
  };

  struct LeftPartialSignificand {
    std::unique_ptr<Token>  decimalPoint;  // "."
    std::unique_ptr<DigitString> fractionalPart;
  };

  enum class SignificandType {
    RIGHT_PARTIAL,
    LEFT_PARTIAL,
  };

  struct Significand {
    SignificandType type;
    union {
      std::unique_ptr<RightPartialSignificand> rightPartialSignificand;
      std::unique_ptr<LeftPartialSignificand> leftPartialSignificand;
    };
  };

  struct TrailingExponent {
    std::unique_ptr<ExponentLetter> exponentLetter;
    std::unique_ptr<Exponent> exponent;
  };

  struct RealLiteralWithSignificand {
    std::unique_ptr<Significand> significand;
    std::optional<std::unique_ptr<TrailingExponent>> exponent;
    std::optional<std::unique_ptr<TrailingKind>> kind;
  };

  struct RealLiteralWithoutSignificand {
    std::unique_ptr<DigitString> digitString;
    std::unique_ptr<ExponentLetter> exponentLetter;
    std::unique_ptr<Exponent> exponent;
    std::optional<std::unique_ptr<TrailingKind>> kind;
  };

  enum class RealLiteralType {
    WITH_SIGNIFICAND,
    WITHOUT_SIGNIFICAND,
  };

  struct RealLiteralConstant {
    // Constraint: If both KindParam and ExponentLetter are present, the
    // ExponentLetter must be "E".
    // -----------------------------------------------------------------------
    // Constraint: The value of KindParam must specify an approximation method
    // that exists on the processor.
    // -----------------------------------------------------------------------
    RealLiteralType type;
    union {
      std::unique_ptr<RealLiteralWithSignificand> withSignificand;
      std::unique_ptr<RealLiteralWithoutSignificand> withoutSignificand;
    };
  };

  struct SignedRealLiteralConstant {
    std::optional<std::unique_ptr<Sign>> sign;
    std::unique_ptr<RealLiteralConstant> realLiteralConstant;
  };

  enum class RealPartType {
    INTEGER,
    REAL,
  };

  struct RealPart {
    RealPartType type;
    union {
      std::unique_ptr<SignedIntLiteralConstant> signedIntLiteralConstant;
      std::unique_ptr<SignedRealLiteralConstant> signedRealLiteralConstant;
    };
  };

  enum class ImagPartType {
    INTEGER,
    REAL,
  };

  struct ImagPart {
    ImagPartType type;
    union {
      std::unique_ptr<SignedIntLiteralConstant> signedIntLiteralConstant;
      std::unique_ptr<SignedRealLiteralConstant> signedRealLiteralConstant;
    };
  };

  struct ComplexLiteralConstant {
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<RealPart> realPart;
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<ImagPart> imagPart;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  struct RepCharSeq {
    std::unique_ptr<Token> token;  // ( rep-char )*
  };

  struct CharLiteralConstant {
    // Constraint: The value of KindParam must specify a representation method
    // that exists on the processor.
    // -----------------------------------------------------------------------
    // Constraint: If string begins with '"', it must end with '"'.
    // -----------------------------------------------------------------------
    // Constraint: If string begins with "'", it must end with "'".
    // -----------------------------------------------------------------------
    std::optional<std::unique_ptr<LeadingKind>> kind;
    std::unique_ptr<Token> leftQuote;  // ( '"' | "'" )
    std::unique_ptr<RepCharSeq> string;
    std::unique_ptr<Token> rightQuote;  // ( '"' | "'" )
  };

  struct LogicalLiteralConstant {
    std::unique_ptr<Token> token;  // ( ".TRUE." | ".FALSE." )
    std::optional<std::unique_ptr<TrailingKind>> kind;
  };

  struct PrivateSequenceStmt {
    std::unique_ptr<Token> token;  // ( "PRIVATE" | "SEQUENCE" )
  };

  struct TypeName {
    std::unique_ptr<Token> token;  // ( name )
  };

  struct ScalarIntVariable {
  };

  struct ScalarIntInitializationExpr {
  };

  struct ScalarIntExpr {
  };

  struct ExprAndDefinedBinaryOp {
    std::unique_ptr<Expr> expr;
    std::unique_ptr<DefinedBinaryOp> definedBinaryOp;
  };

  struct Expr {
    std::optional<std::unique_ptr<ExprAndDefinedBinaryOp>> exprAndDefinedBinaryOp;
    std::unique_ptr<Level5Expr> level5Expr;
  };

  using ExprList = DelimitedList<Expr>;

  struct StructureConstructor {
    std::unique_ptr<TypeName> typeName;
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<ExprList> exprList;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  struct ACDoVariable {
    // Constraint: ACDoVariable must be a named variable.
    std::unique_ptr<ScalarIntVariable> scalarIntVariable;
  };

  struct ACImpliedDoControlThirdExpr {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<ScalarIntExpr> expr;
  };

  struct ACImpliedDoControl {
    std::unique_ptr<ACDoVariable> acDoVariable;
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<ScalarIntExpr> firstExpr;
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<ScalarIntExpr> secondExpr;
    std::optional<std::unique_ptr<ACImpliedDoControlThirdExpr>> thirdExpr;
  };

  struct ACImpliedDo {
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<ACValueList> acValueList;
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<ACImpliedDoControl> acImpliedDoControl;
    std::unique_ptr<Token> closingParenToken;  // "("
  };

  enum class ACValueType {
    EXPR,
    AC_IMPLIED_DO,
  };

  struct ACValue {
    ACValueType type;
    union {
      std::unique_ptr<Expr> expr;
      std::unique_ptr<ACImpliedDo> acImpliedDo;
    };
  };

  using ACValueList = DelimitedList<ACValue>;

  struct ArrayConstructor {
    // Constraint: Each ACValue expression in the ArrayConstructor must have the
    // same type and type parameters.
    // -------------------------------------------------------------------------
    std::unique_ptr<Token> beginArrayToken;  // "(/"
    std::unique_ptr<ACValueList> acValueList;
    std::unique_ptr<Token> endArrayToken;  // "/)"
  };

  struct ExplicitShapeUpperBound {
    std::unique_ptr<SpecificationExpr> specificationExpr;
  };

  struct ExplicitShapeLowerBound {
    std::unique_ptr<SpecificationExpr> specificationExpr;
  };

  struct ExplicitShapeLowerBoundTo {
    std::unique_ptr<ExplicitShapeLowerBound> lowerBound;
    std::unique_ptr<Token> toToken;  // ":"
  };

  struct ExplicitShapeSpec {
    // Constraint: An explicit-shape array whose bounds depend on the values of
    // nonconstant expressions must be a dummy argument, a function result, or
    // an automatic array of a procedure.
    // ------------------------------------------------------------------------
    std::optional<std::unique_ptr<ExplicitShapeLowerBoundTo>> explicitShapeLowerRange;
    std::unique_ptr<ExplicitShapeUpperBound> explicitShapeUpperBound;
  };

  struct DelimitedExplicitShapeSpec {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<ExplicitShapeSpec> explicitShapeSpec;
  };

  struct ExplicitShapeSpecList {
    std::unique_ptr<ExplicitShapeSpec> firstExplicitShapeSpec;
    std::vector<std::unique_ptr<DelimitedExplicitShapeSpec>> restExplicitShapeSpecs;
  };

  struct AssumedShapeSpec {
    std::optional<std::unique_ptr<ExplicitShapeLowerBound>> lowerBound;
    std::unique_ptr<Token> toToken;  // ":"
  };

  struct DelimitedAssumedShapeSpec {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<AssumedShapeSpec> assumedShapeSpec;
  };

  struct AssumedShapeSpecList {
    std::unique_ptr<AssumedShapeSpec> firstAssumedShapeSpec;
    std::vector<std::unique_ptr<DelimitedAssumedShapeSpec>> restAssumedShapeSpecs;
  };

  struct DeferredShapeSpec {
    std::unique_ptr<Token> rangeToken;  // ":"
  };

  struct DelimitedDeferredShapeSpec {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<DeferredShapeSpec> deferredShapeSpec;
  };

  struct DeferredShapeSpecList {
    std::unique_ptr<DeferredShapeSpec> firstDeferredShapeSpec;
    std::vector<std::unique_ptr<DelimitedDeferredShapeSpec>> restDeferredShapeSpecs;
  };

  struct ExplicitShapeSpecListDelimited {
    std::unique_ptr<ExplicitShapeSpecList> explicitShapeSpecList;
    std::unique_ptr<Token> delimiterToken;  // ","
  };

  struct AssumedSizeSpec {
    // Constraint: The function name of an array-valued function must not be
    // declared as an assumed-size array.
    // ---------------------------------------------------------------------
    std::optional<std::unique_ptr<ExplicitShapeSpecListDelimited>> explicitShapeSpecListDelimited;
    std::optional<std::unique_ptr<ExplicitShapeLowerBoundTo>> lowerBoundRanged;
    std::unique_ptr<Token> starToken;  // "*"
  };

  enum class ArraySpecType {
    EXPLICIT_SHAPE_SPEC_LIST,
    ASSUMED_SHAPE_SPEC_LIST,
    DEFERRED_SHAPE_SPEC_LIST,
    ASSUMED_SIZE_SPEC,
  };

  struct ArraySpec {
    // Constraint: The maximum rank is seven.
    ArraySpecType type;
    union {
      std::unique_ptr<ExplicitShapeSpecList> explicitShapeSpecList;
      std::unique_ptr<AssumedShapeSpecList> assumedShapeSpecList;
      std::unique_ptr<DeferredShapeSpecList> deferredShapeSpecList;
      std::unique_ptr<AssumedSizeSpec> assumedSizeSpec;
    };
  };

  struct ArraySpecDecl {
    std::unique_ptr<Token> dimensionToken;  // "DIMENSION"
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<ArraySpec> arraySpec;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  struct AccessSpec {
    // Constraint: An AccessSpec attribute may appear only in the scoping unit
    // of a module.
    // -----------------------------------------------------------------------
    std::unique_ptr<Token> token;  // ( "PUBLIC" | "PRIVATE" )
  };

  struct DerivedTypeStmtAccessSpec {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<AccessSpec> accessSpec;
  };

  struct DerivedTypeStmtDecl {
    std::optional<std::unique_ptr<DerivedTypeStmtAccessSpec>> derivedTypeStmtAccessSpec;
    std::unique_ptr<Token> define;  // "::"
  };

  struct DerivedTypeStmt {
    std::unique_ptr<Token> typeToken;  // "TYPE"
    std::optional<std::unique_ptr<DerivedTypeStmtDecl>> derivedTypeStmtDecl;
    std::unique_ptr<TypeName> typeName;
  };

  struct EndTypeStmt {
    // Constrain: If "END TYPE" is followed by a TypeName, the TypeName must be
    // the same as that in the corresponding DerivedTypeStmt.
    // ------------------------------------------------------------------------
    std::unique_ptr<Token> endToken;  // "END"
    std::unique_ptr<Token> typeToken;  // "TYPE"
    std::optional<std::unique_ptr<TypeName>> typeName;
  };

  struct IntentSpec {
    // Constraint: The INTENT attribute must not be specified for a dummy
    // argument that is a dummy procedure or a dummy pointer.
    // ------------------------------------------------------------------
    std::unique_ptr<Token> token;  // ( "IN" | "OUT" | "INOUT" )
  };

  struct IntentSpecDecl {
    std::unique_ptr<Token> intentToken;  // "INTENT"
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<IntentSpec> intentSpec;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  enum class AttrSpecType {
    PARAMETER,
    ACCESS,
    ALLOCATABLE,
    DIMENSION,
    EXTERNAL,
    INTENT,
    INTRINSIC,
    OPTIONAL,
    POINTER,
    SAVE,
    TARGET,
  };

  struct AttrSpec {
    AttrSpecType type;
    union {
      std::unique_ptr<Token> parameterToken;  // "PARAMETER"
      std::unique_ptr<AccessSpec> accessSpec;
      std::unique_ptr<Token> allocatableToken;  // "ALLOCATABLE"
      std::unique_ptr<ArraySpecDecl> arraySpecDecl;
      std::unique_ptr<Token> externalToken;  // "EXTERNAL"
      std::unique_ptr<IntentSpecDecl> intentSpecDecl;
      std::unique_ptr<Token> intrinsicToken;  // "INTRINSIC"
      std::unique_ptr<Token> optionalToken;  // "OPTIONAL"
      std::unique_ptr<Token> pointerToken;  // "POINTER"
      std::unique_ptr<Token> saveToken;  // "SAVE"
      std::unique_ptr<Token> targetToken;  // "TARGET"
    };
  };

  struct KindAssignment {
    std::unique_ptr<Token> kindToken;  // "KIND"
    std::unique_ptr<Token> assignToken;  // "="
  };

  struct KindSelector {
    std::unique_ptr<Token> openingParenToken;  // "("
    std::optional<std::unique_ptr<KindAssignment>> kindAssignment;
    std::unique_ptr<ScalarIntInitializationExpr> scalarIntInitializationExpr;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  struct IntegerSelector {
    std::unique_ptr<Token> integerToken;  // "INTEGER"
    std::optional<std::unique_ptr<KindSelector>> kindSelector;
  };

  struct RealSelector {
    std::unique_ptr<Token> realToken;  // "REAL"
    std::optional<std::unique_ptr<KindSelector>> kindSelector;
  };

  struct DoublePrecision {
    std::unique_ptr<Token> doubleToken;  // "DOUBLE"
    std::unique_ptr<Token> precisionToken;  // "PRECISION"
  };

  struct ComplexSelector {
    std::unique_ptr<Token> complexToken;  // "COMPLEX"
    std::optional<std::unique_ptr<KindSelector>> kindSelector;
  };

  struct LengthAssignment {
    std::unique_ptr<Token> lenToken;  // "LEN"
    std::unique_ptr<Token> assignToken;  // "="
  };

  enum class TypeParamValueType {
    SPECIFICATION_EXPR,
    STAR,
  };

  struct TypeParamValue {
    // Constraint: A function name must not be declarated with an asterisk
    // TypeParamValue if the function is an internal or module function,
    // array-valued, pointer-valued, or recursive.
    // -------------------------------------------------------------------
    TypeParamValueType type;
    union {
      std::unique_ptr<SpecificationExpr> specificationExpr;
      std::unique_ptr<Token> starToken;  // "*"
    };
  };

  struct LengthWithTypeParamValue {
    std::unique_ptr<Token> openingParenToken;  // "("
    std::optional<std::unique_ptr<LengthAssignment>> lengthAssignment;
    std::unique_ptr<TypeParamValue> typeParamValue;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  struct StarCharLengthDelimited {
    std::unique_ptr<StarCharLength> starCharLength;
    std::optional<std::unique_ptr<Token>> delimiterToken;  // ","
  };

  enum class LengthSelectorType {
    TYPE_PARAM_VALUE,
    CHAR_LENGTH,
  };

  struct LengthSelector {
    LengthSelectorType type;
    union {
      std::unique_ptr<LengthWithTypeParamValue> lengthWithTypeParamValue;
      std::unique_ptr<StarCharLengthDelimited> starCharLengthDelimited;
    };
  };

  struct LengthWithKind {
  };

  struct TypeParamWithScalarIntInitExpr {
  };

  struct KindWithOptLength {
  };

  enum class CharSelectorType {
    LENGTH_SELECTOR,
    LENGTH_WITH_KIND,
    TYPE_PARAM_WITH_SCALAR_INT_INIT_EXPR,
    KIND_WITH_OPT_LENTH,
  };

  struct CharSelector {
    // Constraint: The optional comma in a LengthSelector is permitted only in a
    // TypeSpec in a TypeDeclarationStmt.
    // -------------------------------------------------------------------------
    // Constraint: The optional comma in a LengthSelector is permitted only if
    // no double colon separator appears in the TypeDeclarationStmt.
    // -------------------------------------------------------------------------
    // Constraint: The value of ScalarIntInitializationExpr must be nonnegative
    // and must specify a representation method that exists on the processor.
    // -------------------------------------------------------------------------
    // Constraint: The ScalarIntLiteralConstant must not include a KindParam.
    // -------------------------------------------------------------------------
    CharSelectorType type;
    union {
      std::unique_ptr<LengthSelector> lengthSelector;
      std::unique_ptr<LengthWithKind> lengthWithKind;
      std::unique_ptr<TypeParamWithScalarIntInitExpr> typeParamWithScalarIntInitExpr;
      std::unique_ptr<KindWithOptLength> kindWithOptLength;
    };
  };

  struct CharacterSelector {
    std::unique_ptr<Token> characterToken;  // "CHARACTER"
    std::optional<std::unique_ptr<CharSelector>> charSelector;
  };

  struct LogicalSelector {
    std::unique_ptr<Token> logicalToken;  // "LOGICAL"
    std::optional<std::unique_ptr<KindSelector>> kindSelector;
  };

  struct TypeSelector {
    std::unique_ptr<Token> typeToken;  // "TYPE"
    std::unique_ptr<Token> openingParenToken;  // "("
    std::optional<std::unique_ptr<TypeName>> typeName;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  enum class TypeSpecType {
    INTEGER,
    REAL,
    DOUBLE_PRECISION,
    COMPLEX,
    CHARACTER,
    LOGICAL,
    TYPE,
  };

  struct TypeSpec {
    TypeSpecType type;
    union {
      std::unique_ptr<IntegerSelector> integerSelector;
      std::unique_ptr<RealSelector> realSelector;
      std::unique_ptr<DoublePrecision> doublePrecision;
      std::unique_ptr<ComplexSelector> complexSelector;
      std::unique_ptr<CharacterSelector> characterSelector;
      std::unique_ptr<LogicalSelector> logicalSelector;
      std::unique_ptr<TypeSelector> typeSelector;
    };
  };

  enum class ComponentArraySpecType {
    EXPLICIT,
    DEFERRED,
  };

  struct ComponentArraySpec {
    ComponentArraySpecType type;
    union {
      std::unique_ptr<ExplicitShapeSpecList> explicitShapeSpecList;
      std::unique_ptr<DeferredShapeSpecList> deferredShapeSpecList;
    };
  };

  struct ComponentAttrSpecArraySpec {
    std::unique_ptr<Token> dimensionToken;  // "DIMENSION"
    std::unique_ptr<ComponentArraySpec> componentArraySpec;
  };

  enum class ComponentAttrSpecType {
    POINTER,
    DIMENSION,
  };

  struct ComponentAttrSpec {
    ComponentAttrSpecType type;
    union {
      std::unique_ptr<Token> pointerToken;  // "POINTER"
      std::unique_ptr<ComponentAttrSpecArraySpec> componentAttrSpecArraySpec;
    };
  };

  struct ComponentAttrSpecList {
  };

  struct ComponentName {
  };

  struct ComponentDeclArraySpec {
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<ComponentArraySpec> componentArraySpec;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  struct ParenthesizedTypeParamValue {
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<TypeParamValue> typeParamValue;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  struct ScalarIntLiteralConstant {
  };

  enum class CharLengthType {
    TYPE_PARAM_VALUE,
    SCALAR_INT_LITERAL_CONSTANT,
  };

  struct CharLength {
    CharLengthType type;
    union {
      std::unique_ptr<ParenthesizedTypeParamValue> parenthesizedTypeParamValue;
      std::unique_ptr<ScalarIntLiteralConstant> scalarIntLiteralConstant;
    };
  };

  struct StarCharLength {
    std::unique_ptr<Token> asterisk;  // "*"
    std::unique_ptr<CharLength> charLength;
  };

  struct ComponentDecl {
    // Constraint: If the POINTER attribute is not specified, each
    // ComponentArraySpec must be an ExplicitShapeSpecList.
    // ----------------------------------------------------------------------
    // Constraint: If the POINTER attribute is specified, each
    // ComponentArraySpec must be a DeferredShapeSpecList.
    // ----------------------------------------------------------------------
    // Constraint: The [ * char-length ] option is permitted only if the type
    // specified is character.
    // ----------------------------------------------------------------------
    // Constraint: The character length specified by the CharLength in a
    // ComponentDecl or the CharSelector in a TypeSpec must be a constant
    // specification expression.
    // ----------------------------------------------------------------------
    // Constraint: Each bound in the ExplicitShapeSpec must be a constant
    // specification expression.
    // ----------------------------------------------------------------------
    std::unique_ptr<ComponentName> componentName;
    std::optional<std::unique_ptr<ComponentDeclArraySpec>> componentDeclArraySpec;
    std::optional<std::unique_ptr<StarCharLength>> starCharLength;
  };

  struct ComponentDeclList {
  };

  struct ComponentDefStmtAccessSpec {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<ComponentAttrSpecList> componentAttrSpecList;
  };

  struct ComponentDefStmtDecl {
    std::optional<std::unique_ptr<ComponentDefStmtAccessSpec>> componentDefStmtAccessSpec;
    std::unique_ptr<Token> define;  // "::"
  };

  struct ComponentDefStmt {
    // Constraint: No ComponentAttrSpec may appear more than once in a given
    // ComponentDefStmt.
    // ------------------------------------------------------------------------
    // Constraint: If the POINTER attribute is not specified for a component, a
    // TypeSpec in the ComponentDefStmt must specify an intrinsic type or a
    // previously defined derived type.
    // ------------------------------------------------------------------------
    // Constraint: If the POINTER attribute is specified for a component, a
    // TypeSpec in the ComponentDefStmt must specify an intrinsic type or any
    // accessible derived type including the type being defined.
    // ------------------------------------------------------------------------
    std::unique_ptr<TypeSpec> typeSpec;
    std::optional<std::unique_ptr<ComponentDefStmtDecl>> componentDefStmtDecl;
    std::unique_ptr<ComponentDeclList> componentDeclList;
  };

  struct UseStmt {
  };

  struct RangeUpperLetter {
    std::unique_ptr<Token> rangeToken;  // "-"
    std::unique_ptr<Token> upperLetterToken;  // ( "A" | "B" | ... | "Z" )
  };

  struct LetterSpec {
    std::unique_ptr<Token> lowerLetterToken;  // ( "A" | "B" | ... | "Z" )
    std::optional<std::unique_ptr<RangeUpperLetter>> rangeUpperLetter;
  };

  struct DelimitedLetterSpec {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<LetterSpec> letterSpec;
  };

  struct LetterSpecList {
    std::unique_ptr<LetterSpec> firstLetterSpec;
    std::vector<std::unique_ptr<DelimitedLetterSpec>> restLetterSpecs;
  };

  struct ImplicitSpec {
    std::unique_ptr<TypeSpec> typeSpec;
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<LetterSpecList> letterSpecs;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  struct DelimitedImplicitSpec {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<ImplicitSpec> implicitSpec;
  };

  struct ImplicitSpecList {
    std::unique_ptr<ImplicitSpec> firstImplicitSpec;
    std::vector<std::unique_ptr<DelimitedImplicitSpec>> restImplicitSpecs;
  };

  struct DeclaredImplicitSpecList {
    std::unique_ptr<Token> implicitToken;  // "IMPLICIT"
    std::unique_ptr<ImplicitSpecList> implicitSpecs;
  };

  struct DeclaredImplicitNone {
    std::unique_ptr<Token> implicitToken;  // "IMPLICIT"
    std::unique_ptr<Token> noneToken;  // "NONE"
  };

  enum class ImplicitStmtType {
    IMPLICIT_SPEC_LIST,
    NONE,
  };

  struct ImplicitStmt {
    // Constraint: If "IMPLICIT NONE" is specified in a scoping unit, it must
    // precede any PARAMETER statements that appear in the scoping unit and
    // there must be no other IMPLICIT statement in the scoping unit.
    // -------------------------------------------------------------------------
    // Constraint: If the minus and second letter appear, the second letter must
    // follow the first letter alphabetically.
    // -------------------------------------------------------------------------
    ImplicitStmtType type;
    union {
      std::unique_ptr<DeclaredImplicitSpecList> implicitSpecs;
      std::unique_ptr<DeclaredImplicitNone> implicitNone;
    };
  };

  struct Name {
    std::unique_ptr<Token> token;  // ( name )
  };

  struct NamedConstant {
    std::unique_ptr<Name> name;
  };

  struct NamedConstantDef {
    std::unique_ptr<NamedConstant> namedConstant;
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<InitializationExpr> initializationExpr;
  };

  struct DelimitedNamedConstantDef {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<NamedConstantDef> namedConstantDef;
  };

  struct NamedConstantDefList {
    std::unique_ptr<NamedConstantDef> firstNamedConstantDef;
    std::vector<std::unique_ptr<DelimitedNamedConstantDef>> restNamedConstantDefs;
  };

  struct ParameterStmt {
    std::unique_ptr<Token> parameterToken;  // "PARAMETER"
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<NamedConstantDefList> namedConstantDefs;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  struct FormatStmt {
  };

  struct EntryStmt {
  };

  enum class ImplicitPartStmtType {
    IMPLICIT_STMT,
    PARAMETER_STMT,
    FORMAT_STMT,
    ENTRY_STMT,
  };

  struct ImplicitPartStmt {
    ImplicitPartStmtType type;
    union {
      std::unique_ptr<ImplicitStmt> implicitStmt;
      std::unique_ptr<ParameterStmt> parameterStmt;
      std::unique_ptr<FormatStmt> formatStmt;
      std::unique_ptr<EntryStmt> entryStmt;
    };
  };

  struct ImplicitPart {
    std::vector<std::unique_ptr<ImplicitPartStmt>> implicitPartStmts;
    std::unique_ptr<ImplicitStmt> implicitStmt;
  };

  struct DerivedTypeDef {
    // Constraint: The same PrivateSequenceStmt must not appear more than once
    // in a given DerivedtypeDef.
    // -------------------------------------------------------------------------
    // Constraint: If SEQUENCE is present, all derived types specified in
    // component definitions must be sequence types.
    // -------------------------------------------------------------------------
    // Constraint: An AccessSpec or a PRIVATE statement within the definition is
    // permitted only if the type definition is within the specification part of
    // a module.
    // -------------------------------------------------------------------------
    // Constraint: If a component of a derived type is of a type declared to be
    // private, either the derived type definition must contain the PRIVATE
    // statement or the derived type must be private.
    // -------------------------------------------------------------------------
    // Constraint: A derived type TypeName must not be the same as the name of
    // any intrinsic type nor the same as any other accessible derived TypeName.
    // -------------------------------------------------------------------------
    std::unique_ptr<DerivedTypeStmt> derivedTypeStmt;
    std::vector<std::unique_ptr<PrivateSequenceStmt>> privateSequenceStmts;
    std::unique_ptr<ComponentDefStmt> firstComponentDefStmt;
    std::vector<std::unique_ptr<ComponentDefStmt>> restComponentDefStmts;
  };

  struct InterfaceBlock {
  };

  struct ObjectName {
    std::unique_ptr<Token> token;  // ( name )
  };

  struct ParenthesizedArraySpec {
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<ArraySpec> arraySpec;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  struct AssignedInitializationExpr {
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<InitializationExpr> initializationExpr;
  };

  struct ObjectEntityDecl {
    std::unique_ptr<ObjectName> objectName;
    std::optional<std::unique_ptr<ParenthesizedArraySpec>> parenthesizedArraySpec;
    std::optional<std::unique_ptr<StarCharLength>> starCharLength;
    std::optional<std::unique_ptr<AssignedInitializationExpr>> assignedInitializationExpr;
  };

  struct FunctionName {
    std::unique_ptr<Token> token;  // ( name )
  };

  struct FunctionEntityDecl {
    std::unique_ptr<FunctionName> functionName;
    std::optional<std::unique_ptr<StarCharLength>> starCharLength;
  };

  enum class EntityDeclType {
    OBJECT,
    FUNCTION,
  };

  struct EntityDecl {
    EntityDeclType type;
    union {
      std::unique_ptr<ObjectEntityDecl> objectEntityDecl;
      std::unique_ptr<FunctionEntityDecl> functionEntityDecl;
    };
  };

  struct DelimitedEntityDecl {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<EntityDecl> entityDecl;
  };

  struct EntityDeclList {
    std::unique_ptr<EntityDecl> firstEntityDecl;
    std::vector<std::unique_ptr<DelimitedEntityDecl>> restEntityDecls;
  };

  struct DelimitedAttrSpec {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<AttrSpec> attrSpec;
  };

  struct TypeDecl {
    std::vector<std::unique_ptr<DelimitedAttrSpec>> separatedAttrSpecs;
    std::unique_ptr<Token> defineToken;  // "::"
  };

  struct TypeDeclarationStmt {
    // Constraint: The same AttrSpec must not appear more than once in a given
    // TypeDeclarationStmt.
    // -------------------------------------------------------------------------
    // Constraint: The FunctionName must be the name of an external function, an
    // intrinsic function, a function dummy procedure, or a statement function.
    // -------------------------------------------------------------------------
    // Constraint: The AssignedInitializationExpr must appear if the statement
    // contains a PARAMETER attribute.
    // -------------------------------------------------------------------------
    // Constraint: If AssignedInitializationExpr appears, a double colon
    // separator must appear before the EntityDeclList.
    // -------------------------------------------------------------------------
    // Constraint: The AssignedInitializationExpr must not appear if ObjectName
    // is a dummy argument, a function result, an object in a named common block
    // unless the type declaration is in a block data program unit, an object in
    // blank common, an allocatable array, a pointer, an external name, an
    // intrinsic name, or an automatic object.
    // -------------------------------------------------------------------------
    // Constraint: The StarCharLength option is permitted only if the type
    // specified is character.
    // -------------------------------------------------------------------------
    // Constraint: The ALLOCATABLE attribute may be used only when declaring an
    // array that is not a dummy argument or a function result.
    // -------------------------------------------------------------------------
    // Constraint: An array declared with a POINTER or an ALLOCATABLE attribute
    // must be specified with an ArraySpec that is a DeferredShapeSpecList.
    // -------------------------------------------------------------------------
    // Constraint: An ArraySpec for an ObjectName that is a function result that
    // does not have the POINTER attribute must be an ExplicitShapeSpecList.
    // -------------------------------------------------------------------------
    // Constraint: If the POINTER attribute is specified, the TARGET, INTENT,
    // EXTERNAL, or INTRINSIC attribute must not be specified.
    // -------------------------------------------------------------------------
    // Constraint: If the TARGET attribute is specified, the POINTER, EXTERNAL,
    // INTRINSIC, or PARAMETER attribute must not be specified.
    // -------------------------------------------------------------------------
    // Constraint: The PARAMETER attribute must not be specified for dummy
    // arguments, pointers, allocatable arrays, functions, or objects in a
    // common block.
    // -------------------------------------------------------------------------
    // Constraint: The INTENT and OPTIONAL attributes may be specified only for
    // dummy arguments.
    // -------------------------------------------------------------------------
    // Constraint: An entity must not have the PUBLIC attribute if its type has
    // the PRIVATE attribute.
    // -------------------------------------------------------------------------
    // Constraint: An entity in a TypeDeclarationStmt must not have the EXTERNAL
    // or INTRINSIC attribute specified unless it is a function.
    // -------------------------------------------------------------------------
    // Constraint: An array must not have both the ALLOCATABLE attribute and the
    // POINTER attribute.
    // -------------------------------------------------------------------------
    // Constraint: An entity must not be given explicitly any attribute more
    // than once in a scoping unit.
    // -------------------------------------------------------------------------
    // Constraint: The value of ScalarIntInitializationExpr must be nonnegative
    // and must specify a representation method that exists on the processor.
    // -------------------------------------------------------------------------
    std::unique_ptr<TypeSpec> typeSpec;
    std::optional<std::unique_ptr<TypeDecl>> typeDecl;
    std::unique_ptr<EntityDeclList> entityDeclList;
  };

  struct UseName {
  };

  struct GenericSpec {
  };

  enum class AccessIdType {
    USE_NAME,
    GENERIC_SPEC,
  };

  struct AccessId {
    AccessIdType type;
    union {
      std::unique_ptr<UseName> useName;
      std::unique_ptr<GenericSpec> genericSpec;
    };
  };

  struct DelimitedAccessId {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<AccessId> accessId;
  };

  struct AccessIdList {
    std::unique_ptr<AccessId> firstAccessId;
    std::vector<std::unique_ptr<DelimitedAccessId>> restAccessIds;
  };

  struct DefinedAccessIdList {
    std::optional<std::unique_ptr<Token>> defineToken;  // "::"
    std::unique_ptr<AccessIdList> accessIdList;
  };

  struct AccessStmt {
    // Constraint: An AccessStmt may appear only in the scoping unit of a
    // module. Only one accessibility statement with an omitted AccessIdList is
    // permitted in the scoping unit of a module.
    // -------------------------------------------------------------------------
    // Constraint: Each UseName must be the name of a named variable, procedure,
    // derived type, named constant, or namelist group.
    // -------------------------------------------------------------------------
    // Constraint: A module procedure that has a dummy argument or function
    // result of a type that has PRIVATE accessibility must have PRIVATE
    // accessibility and must not have a generic identifier that has PUBLIC
    // accessibility.
    // -------------------------------------------------------------------------
    std::unique_ptr<AccessSpec> accessSpec;
    std::optional<std::unique_ptr<DefinedAccessIdList>> definedAccessIdList;
  };

  struct ArrayName {
    std::unique_ptr<Token> token;  // ( name )
  };

  struct ParenthesizedDeferredShapeSpecList {
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<DeferredShapeSpecList> deferredShapeSpecList;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  struct ArrayNameAndDeferredShapeSpecList {
    std::unique_ptr<ArrayName> arrayName;
    std::optional<std::unique_ptr<ParenthesizedDeferredShapeSpecList>> parenthesizedDeferredShapeSpecList;
  };

  struct DelimitedArrayNameAndDeferredShapeSpecList {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<ArrayNameAndDeferredShapeSpecList> arrayNameAndDeferredShapeSpecList;
  };

  struct ArrayNameAndDeferredShapeSpecLists {
    std::unique_ptr<ArrayNameAndDeferredShapeSpecList> firstArrayNameAndDeferredShapeSpecList;
    std::vector<std::unique_ptr<DelimitedArrayNameAndDeferredShapeSpecList>> restArrayNameAndDeferredShapeSpecList;
  };

  struct AllocatableStmt {
    // Constraint: The ArrayName must not be a dummy argument or function
    // result.
    // --------------------------------------------------------------------
    // Constraint: If the DIMENSION attribute for an ArrayName is specified
    // elsewhere in the scoping unit, the ArraySpec must be a
    // DeferredShapeSpecList.
    // --------------------------------------------------------------------
    std::unique_ptr<Token> allocatableToken;  // "ALLOCATABLE"
    std::optional<std::unique_ptr<Token>> defineToken;  // "::"
    std::unique_ptr<ArrayNameAndDeferredShapeSpecLists> arrayNameAndDeferredShapeSpecLists;
  };

  struct VariableName {
    // Constraint: If a VariableName appears with an ExplicitShapeSpecList, it
    // must not have the POINTER attribute.
    // -----------------------------------------------------------------------
    std::unique_ptr<Token> token;  // ( name )
  };

  struct ParenthesizedExplicitShapeSpecList {
    // Constraint: Each bound in the ExplicitShapeSpec must be a constant
    // specification expression.
    // ------------------------------------------------------------------
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<ExplicitShapeSpecList> explicitShapeSpecList;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  struct CommonBlockName {
    std::unique_ptr<Token> token;  // ( name )
  };

  struct EmphasizedCommonBlockName {
    std::unique_ptr<Token> beginCommonBlockNameToken;  // "/"
    std::unique_ptr<CommonBlockName> commonBlockName;
    std::unique_ptr<Token> EndCommonBlockNameToken;  // "/"
  };

  struct CommonBlockObject {
    // Constraint: A CommonBlockObject must not be a dummy argument, an
    // allocatable array, an automatic object, a function name, an entry name,
    // or a result name.
    // -----------------------------------------------------------------------
    // Constraint: If a CommonBlockObject is of a derived type, it must be a
    // sequence type.
    // -----------------------------------------------------------------------
    std::unique_ptr<VariableName> variableName;
    std::optional<std::unique_ptr<ParenthesizedExplicitShapeSpecList>> parenthesizedExplicitShapeSpecList;
  };

  struct DelimitedCommonBlockObject {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<CommonBlockObject> commonBlockObject;
  };

  struct CommonBlockObjectList {
    // Constraint: Only one appearance of a given VariableName is permitted in
    // all CommonBlockObjectLists within a scoping unit.
    // -----------------------------------------------------------------------
    std::unique_ptr<CommonBlockObject> firstCommonBlockObject;
    std::vector<std::unique_ptr<DelimitedCommonBlockObject>> restCommonBlockObjects;
  };

  struct EmphasizedCommonBlockNameAndObjectList {
    std::unique_ptr<EmphasizedCommonBlockName> emphasizedCommonBlockName;
    std::unique_ptr<CommonBlockObjectList> commonBlockObjectList;
  };

  struct DelimitedEmphasizedCommonBlockNameAndObjectList {
    std::optional<std::unique_ptr<Token>> delimiterToken;  // ","
    std::unique_ptr<EmphasizedCommonBlockNameAndObjectList> emphasizedCommonBlockNameAndObjectList;
  };

  struct EmphasizedCommonBlockNameAndObjectLists {
    std::unique_ptr<EmphasizedCommonBlockNameAndObjectList> firstEmphasizedCommonBlockNameAndObjectList;
    std::vector<std::unique_ptr<DelimitedEmphasizedCommonBlockNameAndObjectList>> restEmphasizedCommonBlockNameAndObjectLists;
  };

  struct CommonStmt {
    std::unique_ptr<Token> commonToken;  // "COMMON"
    std::unique_ptr<EmphasizedCommonBlockNameAndObjectLists> emphasizedCommonBlockNameAndObjectLists;
  };

  struct ArrayNameSpec {
    std::unique_ptr<ArrayName> arrayName;
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<ArraySpec> arraySpec;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  struct DelimitedArrayNameSpec {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<ArrayNameSpec> arrayNameSpec;
  };

  struct ArrayNameSpecList {
    std::unique_ptr<ArrayNameSpec> firstArrayNameSpec;
    std::vector<std::unique_ptr<DelimitedArrayNameSpec>> restArrayNameSpecs;
  };

  struct DimensionStmt {
    std::unique_ptr<Token> dimensionToken;  // "DIMENSION"
    std::optional<std::unique_ptr<Token>> defineToken;  // "::"
    std::unique_ptr<ArrayNameSpecList> arrayNameSpecList;
  };

  struct Subscript {
    std::unique_ptr<ScalarIntExpr> scalarIntExpr;
  };

  struct Stride {
    std::unique_ptr<ScalarIntExpr> scalarIntExpr;
  };

  struct ByStride {
    std::unique_ptr<Token> byToken;  // ":"
    std::unique_ptr<Stride> stride;
  };

  struct SubscriptTriplet {
    // Constraint: The second Subscript must not be omitted from a
    // SubscriptTriplet in the last dimension of an assumed-size array.
    // ----------------------------------------------------------------
    std::optional<std::unique_ptr<Subscript>> lowerBound;
    std::unique_ptr<Token> rangeToken;  // ":"
    std::optional<std::unique_ptr<Subscript>> upperBound;
    std::optional<std::unique_ptr<ByStride>> byStride;
  };

  struct VectorSubscript {
    // Constraint: A VectorSubscript must be an integer array expression or rank
    // one.
    // -------------------------------------------------------------------------
    std::unique_ptr<IntExpr> intExpr;
  };

  enum class SectionSubscriptType {
    SUBSCRIPT,
    SUBSCRIPT_TRIPLET,
    VECTOR_SUBSCRIPT,
  };

  struct SectionSubscript {
    SectionSubscriptType type;
    union {
      std::unique_ptr<Subscript> subscript;
      std::unique_ptr<SubscriptTriplet> subscriptTriplet;
      std::unique_ptr<VectorSubscript> vectorSubscript;
    };
  };

  struct DelimitedSectionSubscript {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<SectionSubscript> sectionSubscript;
  };

  struct SectionSubscriptList {
    std::unique_ptr<SectionSubscript> firstSectionSubscript;
    std::vector<std::unique_ptr<DelimitedSectionSubscript>> restSectionSubscripts;
  };

  struct ParenthesizedSectionSubscriptList {
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<SectionSubscriptList> sectionSubscriptList;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  struct PartName {
    std::unique_ptr<Token> token;  // ( name )
  };

  struct PartRef {
    // Constrain: In a PartRef containing a SectionSubscriptList, the number of
    // SectionSubscripts must equal the rank of PartName.
    // ------------------------------------------------------------------------
    std::unique_ptr<PartName> partName;
    std::optional<std::unique_ptr<ParenthesizedSectionSubscriptList>> parenthesizedSectionSubscriptList;
  };

  struct AccessorPartRef {
    std::unique_ptr<Token> accessorToken;  // "%"
    std::unique_ptr<PartRef> partRef;
  };

  struct DataRef {
    // Constraint: In a DataRef, each PartName except the rightmost must be of
    // derived type.
    // -------------------------------------------------------------------------
    // Constraint: In a DataRef, each PartName except the leftmost must be the
    // name of a component of the derived type definition of the type of the
    // preceding PartName.
    // -------------------------------------------------------------------------
    // Constraint: In a DataRef, there must not be more than one PartRef with
    // nonzero rank. A PartName to the right of a PartRef with nonzero rank must
    // not have the POINTER attribute.
    // -------------------------------------------------------------------------
    std::unique_ptr<PartRef> firstPartRef;
    std::vector<std::unique_ptr<AccessorPartRef>> restPartRefs;
  };

  struct ArrayElement {
    // Constraint: In an ArrayElement or a ScalarStructureComponent that is a
    // DataIDoObject, any subscript must be an expression whose primaries are
    // either constants or DO variables of the containing DataImpliedDos, and
    // each operation must be intrinsic.
    // -------------------------------------------------------------------------
    // Constraint: In an ArrayElement, every PartRef must have rank zero and the
    // last PartRef must contain a SubscriptList.
    // -------------------------------------------------------------------------
    std::unique_ptr<DataRef> dataRef;
  };

  struct ScalarVariableName {
    std::unique_ptr<Token> token;  // ( name )
  };

  struct ArrayVariableName {
    std::unique_ptr<Token> token;  // ( name )
  };

  struct ScalarStructureComponent {
  };

  struct ScalarConstant {
  };

  enum class ParentStringType {
    SCALAR_VARIABLE_NAME,
    ARRAY_ELEMENT,
    SCALAR_STRUCTURE_COMPONENT,
    SCALAR_CONSTANT,
  };

  struct ParentString {
    // Constraint: ParentString must be of type character.
    ParentStringType type;
    union {
      std::unique_ptr<ScalarVariableName> scalarVariableName;
      std::unique_ptr<ArrayElement> arrayElement;
      std::unique_ptr<ScalarStructureComponent> scalarStructureComponent;
      std::unique_ptr<ScalarConstant> scalarConstant;
    };
  };

  struct SubstringRange {
    std::optional<std::unique_ptr<ScalarIntExpr>> lowerBound;
    std::unique_ptr<Token> rangeToken;  // ":"
    std::optional<std::unique_ptr<ScalarIntExpr>> upperBound;
  };

  struct ParenthesizedSubstringRange {
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<SubstringRange> substringRange;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  struct Substring {
    std::unique_ptr<ParentString> parentString;
    std::unique_ptr<ParenthesizedSubstringRange> parenthesizedSubstringRange;
  };

  enum class EquivalenceObjectType {
    VARIABLE_NAME,
    ARRAY_ELEMENT,
    SUBSTRING,
  };

  struct EquivalenceObject {
    // Constraint: An EquivalenceObject must not be a dummy argument, a pointer,
    // an allocatable array, an object of a nonsequence derived type or of a
    // sequence derived type containing a pointer at any level of component
    // selection, an automatic object, a function name, an entry name, a result
    // name, a named constant, a structure component, or a subobject of any of
    // the preceding objects.
    // -------------------------------------------------------------------------
    // Constraint: Each subscript or substring range expression in an
    // EquivalenceObject must be an integer initialization expression.
    // -------------------------------------------------------------------------
    // Constraint: If an EquivalenceObject is of type default integer, default
    // real, double precision real, default complex, default logical, or numeric
    // sequence type, all of the objects in the equivalence set must be of these
    // types.
    // -------------------------------------------------------------------------
    // Constraint: If an EquivalenceObject is of type default character or
    // character sequence type, all of the objects in the equivalence set must
    // be of these types.
    // -------------------------------------------------------------------------
    // Constraint: If an EquivalenceObject is of a derived type that is not a
    // numeric sequence or character sequence type, all of the objects in the
    // equivalence set must be of the same type.
    // -------------------------------------------------------------------------
    // Constraint: If an EquivalenceObject is of an intrinsic type other than
    // default integer, default real, double precision real, default complex,
    // default logical, or default character, all of the objects in the
    // equivalence set must be of the same type with the same kind type
    // parameter value.
    // -------------------------------------------------------------------------
    EquivalenceObjectType type;
    union {
      std::unique_ptr<VariableName> variableName;
      std::unique_ptr<ArrayElement> arrayElement;
      std::unique_ptr<Substring> substring;
    };
  };

  struct DelimitedEquivalenceObject {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<EquivalenceObject> equivalenceObject;
  };

  struct EquivalenceSet {
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<EquivalenceObject> firstEquivalenceObject;
    std::vector<std::unique_ptr<DelimitedEquivalenceObject>> restEquivalenceObjects;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  struct DelimitedEquivalenceSet {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<EquivalenceSet> equivalenceSet;
  };

  struct EquivalenceSetList {
    std::unique_ptr<EquivalenceSet> firstEquivalenceSet;
    std::vector<std::unique_ptr<DelimitedEquivalenceSet>> restEquivalenceSets;
  };

  struct EquivalenceStmt {
    std::unique_ptr<Token> equivalenceToken;  // "EQUIVALENCE"
    std::unique_ptr<EquivalenceSetList> equivalentSets;
  };

  struct ExternalStmt {
  };

  struct DummyArgName {
    std::unique_ptr<Token> token;  // ( name )
  };

  struct DelimitedDummyArgName {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<DummyArgName> dummyArgName;
  };

  struct DummyArgNameList {
    std::unique_ptr<DummyArgName> firstDummyArgName;
    std::vector<std::unique_ptr<DelimitedDummyArgName>> restDummyArgNames;
  };

  struct IntentStmt {
    // Constraint: An IntentStmt may appear only in the SpecificationPart of a
    // subprogram or an interface body.
    // -----------------------------------------------------------------------
    // Constraint: DummyArgName must not be the name of a dummy procedure or a
    // dummy pointer.
    std::unique_ptr<Token> intentToken;  // "INTENT"
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<IntentSpec> intentSpec;
    std::unique_ptr<Token> closingParenToken;  // ")"
    std::optional<std::unique_ptr<Token>> defineToken;  // "::"
    std::unique_ptr<DummyArgNameList> dummyArgNameList;
  };

  struct IntrinsicStmt {
  };

  struct NamelistGroupObject {
    // Constraint: A NamelistGroupObject must not be an array dummy argument
    // with a nonconstant bound, a variable with nonconstant character length,
    // an automatic object, a pointer, a variable of a type that has an ultimate
    // component that is a pointer, or an allocatable array.
    // -------------------------------------------------------------------------
    std::unique_ptr<VariableName> variableName;
  };

  struct DelimitedNamelistGroupObject {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<NamelistGroupObject> namelistGroupObject;
  };

  struct NamelistGroupObjectList {
    std::unique_ptr<NamelistGroupObject> firstNamelistGroupObject;
    std::vector<std::unique_ptr<DelimitedNamelistGroupObject>> restNamelistGroupObjects;
  };

  struct NamelistGroupName {
    std::unique_ptr<Token> token;  // ( name )
  };

  struct NamelistGroupNameAndObjectList {
    // Constraint: If a NamelistGroupName has the PUBLIC attribute, no item in
    // the NamelistGroupObjectList may have the PRIVATE attribute.
    // -----------------------------------------------------------------------
    std::unique_ptr<Token> openingSlashToken;  // "/"
    std::unique_ptr<NamelistGroupName> namelistGroupName;
    std::unique_ptr<Token> closingSlashToken;  // "/"
    std::unique_ptr<NamelistGroupObjectList> namelistGroupObjects;
  };

  struct DelimitedNamelistGroupNameAndObjectList {
    std::optional<std::unique_ptr<Token>> delimiterToken;  // ","
    std::unique_ptr<NamelistGroupNameAndObjectList> namelistGroupNameAndObjectList;
  };

  struct NamelistGroupNameAndObjectLists {
    std::unique_ptr<NamelistGroupNameAndObjectList> firstNamelistGroupNameAndObjectList;
    std::vector<std::unique_ptr<DelimitedNamelistGroupNameAndObjectList>> restNamelistGroupNameAndObjectLists;
  };

  struct NamelistStmt {
    std::unique_ptr<Token> namelistToken;  // "NAMELIST"
    std::unique_ptr<NamelistGroupNameAndObjectLists> namelistGroupNameAndObjectLists;
  };

  struct OptionalStmt {
    std::unique_ptr<Token> optionalToken;  // "OPTIONAL"
    std::optional<std::unique_ptr<Token>> defineToken;  // "::"
    std::unique_ptr<DummyArgNameList> dummyArgNameList;
  };

  struct ObjectNameAndDeferredShapeSpecList {
    std::unique_ptr<ObjectName> objectName;
    std::optional<std::unique_ptr<ParenthesizedDeferredShapeSpecList>> parenthesizedDeferredShapeSpecList;
  };

  struct DelimitedObjectNameAndDeferredShapeSpecList {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<ObjectNameAndDeferredShapeSpecList> objectNameAndDeferredShapeSpecList;
  };

  struct ObjectNameAndDeferredShapeSpecLists {
    std::unique_ptr<ObjectNameAndDeferredShapeSpecList> firstObjectNameAndDeferredShapeSpecList;
    std::vector<std::unique_ptr<DelimitedObjectNameAndDeferredShapeSpecList>> restObjectNameAndDeferredShapeSpecLists;
  };

  struct PointerStmt {
    // Constraint: The INTENT attribute must not be specified for an ObjectName.
    // ----------------------------------------------------------------------------
    // Constraint: If the DIMENSION attribute for an ObjectName is specified
    // elsewhere in the scoping unit, the ArraySpec must be a
    // DeferredShapeSpecList.
    // ----------------------------------------------------------------------------
    // Constraint: The PARAMETER attribute must not be specified for an ObjectName.
    // ----------------------------------------------------------------------------
    std::unique_ptr<Token> pointerToken;  // "POINTER"
    std::unique_ptr<ObjectNameAndDeferredShapeSpecLists> objectNameAndDeferredShapeSpecLists;
  };

  enum class SavedEntityType {
    OBJECT_NAME,
    COMMON_BLOCK_NAME,
  };

  struct SavedEntity {
    // Constraint: An ObjectName must not be a dummy argument name, a procedure
    // name, a function result name, an automatic data object name, or the name
    // of an entity in a common block.
    // ------------------------------------------------------------------------
    // Constraint: If a SAVE statement with an omitted saved entity list occurs
    // in a scoping unit, no other explicit occurrence of the SAVE attribute or
    // SAVE statement is permitted in the same scoping unit.
    // ------------------------------------------------------------------------
    SavedEntityType type;
    union {
      std::unique_ptr<ObjectName> objectName;
      std::unique_ptr<EmphasizedCommonBlockName> emphasizedCommonBlockName;
    };
  };

  struct DelimitedSavedEntity {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<SavedEntity> savedEntity;
  };

  struct SavedEntityList {
    std::unique_ptr<SavedEntity> firstSavedEntity;
    std::vector<std::unique_ptr<DelimitedSavedEntity>> restSavedEntities;
  };

  struct DefinedSavedEntityList {
    std::optional<std::unique_ptr<Token>> defineToken;  // "::"
    std::unique_ptr<SavedEntityList> savedEntityList;
  };

  struct SaveStmt {
    std::unique_ptr<Token> saveToken;  // "SAVE"
    std::optional<std::unique_ptr<DefinedSavedEntityList>> definedSavedEntityList;
  };

  struct ObjectNameAndParenthesizedArraySpec {
    std::unique_ptr<ObjectName> objectName;
    std::optional<std::unique_ptr<ParenthesizedArraySpec>> parenthesizedArraySpec;
  };

  struct DelimitedObjectNameAndParenthesizedArraySpec {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<ObjectNameAndParenthesizedArraySpec> objectNameAndParenthesizedArraySpec;
  };

  struct ObjectNameAndParenthesizedArraySpecList {
    std::unique_ptr<ObjectNameAndParenthesizedArraySpec> firstObjectNameAndParenthesizedArraySpec;
    std::vector<std::unique_ptr<DelimitedObjectNameAndParenthesizedArraySpec>> restObjectNameAndParenthesizedArraySpecs;
  };

  struct TargetStmt {
    std::unique_ptr<Token> targetToken;  // "TARGET"
    std::optional<std::unique_ptr<Token>> defineToken;  // "::"
    std::unique_ptr<ObjectNameAndParenthesizedArraySpecList> objectNameAndParenthesizedArraySpecList;
  };

  struct ArraySection {
    // Constraint: In an ArraySection, exactly one PartRef must have nonzero
    // rank, and either the final PartRef has a SectionSubscriptList with
    // nonzero rank or another PartRef has nonzero rank.
    // ---------------------------------------------------------------------
    // Constraint: In an ArraySection with a SubstringRange, the rightmost
    // PartName must be of type character.
    // ---------------------------------------------------------------------
    std::unique_ptr<DataRef> dataRef;
    std::optional<std::unique_ptr<ParenthesizedArraySpec>> parenthesizedArraySpec;
  };

  struct StructureComponent {
    // Constraint: In a StructureComponent, there must be more than one PartRef
    // and the rightmost PartRef must be of the form PartName.
    // ------------------------------------------------------------------------
    std::unique_ptr<DataRef> dataRef;
  };

  enum class SubobjectType {
    ARRAY_ELEMENT,
    ARRAY_SECTION,
    STRUCTURE_COMPONENT,
    SUBSTRING,
  };

  struct Subobject {
    SubobjectType type;
    union {
      std::unique_ptr<ArrayElement> arrayElement;
      std::unique_ptr<ArraySection> arraySection;
      std::unique_ptr<StructureComponent> structureComponent;
      std::unique_ptr<Substring> substring;
    };
  };

  enum class VariableType {
    SCALAR_VARIABLE_NAME,
    ARRAY_VARIABLE_NAME,
    SUBOBJECT,
  };

  struct Variable {
    // Constraint: In a Variable that is a DAtaStmtObject, any subscript,
    // section subscript, substring starting point, and substring ending point
    // must be an initialization expression.
    // -------------------------------------------------------------------------
    // Constraint: A Variable whose name or designator is included in a
    // DataStmtObjectList or a DataIDoObjectList must not be: a dummy argument,
    // made accessible by use association or host association, in a named common
    // block unless the DATA statement is in a block data program unit, in a
    // blank common block, a function name, a function result name, an automatic
    // object, a pointer, or an allocatable array.
    // -------------------------------------------------------------------------
    // Constraint: ArrayVariableName must be the name of a data object that is
    // an array.
    // -------------------------------------------------------------------------
    // Constraint: ArrayVariableName must not have the PARAMETER attribute.
    // -------------------------------------------------------------------------
    // Constraint: ScalarVariableName must not have the PARAMETER attribute.
    // -------------------------------------------------------------------------
    // Constraint: Subobject must not be a subobject designator (for example, a
    // substring) whose parent is a constant.
    // -------------------------------------------------------------------------
    VariableType type;
    union {
      std::unique_ptr<ScalarVariableName> scalarVariableName;
      std::unique_ptr<ArrayVariableName> arrayVariableName;
      std::unique_ptr<Subobject> subobject;
    };
  };

  struct LogicalVariable {
    // Constraint: LogicalVariable must be of type logical.
    // ----------------------------------------------------
    std::unique_ptr<Variable> variable;
  };

  struct DefaultLogicalVariable {
    // Constraint: DefaultLogicalVariable must be of type default logical.
    // -------------------------------------------------------------------
    std::unique_ptr<Variable> variable;
  };

  struct CharVariable {
    // Constraint: CharVariable must be of type character.
    std::unique_ptr<Variable> variable;
  };

  struct DefaultCharVariable {
    // Constraint: DefaultCharVariable must be of type default character.
    std::unique_ptr<Variable> variable;
  };

  struct IntVariable {
    // Constraint: IntVariable must be of type integer.
    std::unique_ptr<Variable> variable;
  };

  struct DefaultIntVariable {
    // Constraint: DefaultIntVariable must be of type default integer.
    std::unique_ptr<Variable> variable;
  };

  struct ScalarIntConstant {
  };

  struct DelimitedScalarIntExpr {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<ScalarIntExpr> scalarIntExpr;
  };

  struct DataIDoVariable {
    // Constraint: DataIDoVariable must be a named variable.
    std::unique_ptr<ScalarIntVariable> scalarIntVariable;
  };

  enum class DataIDoObjectType {
    ARRAY_ELEMENT,
    SCALAR_STRUCTURE_COMPONENT,
    DATA_IMPLIED_DO,
  };

  struct DataIDoObject {
    DataIDoObjectType type;
    union {
      std::unique_ptr<ArrayElement> arrayElement;
      std::unique_ptr<ScalarStructureComponent> scalarStructureComponent;
      std::unique_ptr<DataImpliedDo> dataImpliedDo;
    };
  };

  struct DelimitedDataIDoObject {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<DataIDoObject> dataIDoObject;
  };

  struct DataIDoObjectList {
    std::unique_ptr<DataIDoObject> firstDataIDoObject;
    std::vector<std::unique_ptr<DelimitedDataIDoObject>> restDataIDoObjects;
  };

  struct DataImpliedDo {
    // Constraint: A ScalarIntExpr of a DataImpliedDo must involve as primaries
    // only constants or DO variables of the containing DataImpliedDos, and each
    // operation must be intrinsic.
    // -------------------------------------------------------------------------
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<DataIDoObjectList> dataIDoObjects;
    std::unique_ptr<Token> firstDelimiterToken;  // ","
    std::unique_ptr<DataIDoVariable> dataIDoVariable;
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<ScalarIntExpr> firstScalarIntExpr;
    std::unique_ptr<Token> secondDelimiterToken;  // ","
    std::unique_ptr<ScalarIntExpr> secondScalarIntExpr;
    std::unique_ptr<DelimitedScalarIntExpr> thirdScalarIntExpr;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  enum class DataStmtObjectType {
    VARIABLE,
    DATA_IMPLIED_DO,
  };

  struct DataStmtObject {
    DataStmtObjectType type;
    union {
      std::unique_ptr<Variable> variable;
      std::unique_ptr<DataImpliedDo> dataImpliedDo;
    };
  };

  struct DelimitedDataStmtObject {
    std::unique_ptr<Token> delimiterToken;  // ","
  };

  struct DataStmtObjectList {
    std::unique_ptr<DataStmtObject> firstDataStmtObject;
    std::vector<std::unique_ptr<DelimitedDataStmtObject>> restDataStmtObjects;
  };

  struct DataStmtRepeat {
    // Constraint: The DATA statement repeat factor must be positive or zero. If
    // the DATA statement repeat factor is a named constant, it must have been
    // declared previously in the scoping unit or made accessible by use
    // association or host association.
    // -------------------------------------------------------------------------
    std::unique_ptr<ScalarIntConstant> scalarIntConstant;
  };

  struct DataStmtRepeatStar {
    std::unique_ptr<DataStmtRepeat> dataStmtRepeat;
    std::unique_ptr<Token> starToken;  // "*"
  };

  enum class DataStmtConstantType {
    SCALAR_CONSTANT,
    SIGNED_INT_LITERAL_CONSTANT,
    SIGNED_REAL_LITERAL_CONSTANT,
    STRUCTURE_CONSTRUCTOR,
    BOZ_LITERAL_CONSTANT,
  };

  struct DataStmtConstant {
    // Constraint: If a DataStmtConstant is a StructureConstructor, each
    // component must be an initialization expression.
    // -----------------------------------------------------------------
    DataStmtConstantType type;
    union {
      std::unique_ptr<ScalarConstant> scalarConstant;
      std::unique_ptr<SignedIntLiteralConstant> signedIntLiteralConstant;
      std::unique_ptr<SignedRealLiteralConstant> signedRealLiteralConstant;
      std::unique_ptr<StructureConstructor> structureConstructor;
      std::unique_ptr<BozLiteralConstant> bozLiteralConstant;
    };
  };

  struct DataStmtValue {
    std::optional<std::unique_ptr<DataStmtRepeatStar>> dataStmtRepeatStar;
    std::unique_ptr<DataStmtConstant> dataStmtConstant;
  };

  struct DelimitedDataStmtValue {
    std::unique_ptr<Token> delimiterToken;  // ","
  };

  struct DataStmtValueList {
    std::unique_ptr<DataStmtValue> firstDataStmtValue;
    std::vector<std::unique_ptr<DelimitedDataStmtValue>> restDataStmtValues;
  };

  struct DataStmtSet {
    // Constraint: The ArrayElement must not have a constant parent.
    // -------------------------------------------------------------------------
    // Constraint: The ScalarStructureComponent must not have a constant parent.
    // -------------------------------------------------------------------------
    std::unique_ptr<DataStmtObjectList> dataStmtObjectList;
    std::unique_ptr<Token> openingSlash;  // "/"
    std::unique_ptr<DataStmtValueList> dataStmtValueList;
    std::unique_ptr<Token> closingSlash;  // "/"
  };

  struct DelimitedDataStmtSet {
    std::optional<std::unique_ptr<Token>> delimiterToken;  // ","
    std::unique_ptr<DataStmtSet> dataStmtSet;
  };

  struct DataStmtSets {
    std::unique_ptr<DataStmtSet> firstDataStmtSet;
    std::vector<std::unique_ptr<DelimitedDataStmtSet>> restDataStmtSets;
  };

  struct DataStmt {
    std::unique_ptr<Token> dataToken;  // "DATA"
    std::unique_ptr<DataStmtSets> dataStmtSets;
  };

  enum class SpecificationStmtType {
    ACCESS_STMT,
    ALLOCATABLE_STMT,
    COMMON_STMT,
    DATA_STMT,
    DIMENSION_STMT,
    EQUIVALENCE_STMT,
    EXTERNAL_STMT,
    INTENT_STMT,
    INTRINSIC_STMT,
    NAMELIST_STMT,
    OPTIONAL_STMT,
    POINTER_STMT,
    SAVE_STMT,
    TARGET_STMT,
  };

  struct SpecificationStmt {
    SpecificationStmtType type;
    union {
      std::unique_ptr<AccessStmt> accessStmt;
      std::unique_ptr<AllocatableStmt> allocatableStmt;
      std::unique_ptr<CommonStmt> commonStmt;
      std::unique_ptr<DataStmt> dataStmt;
      std::unique_ptr<DimensionStmt> dimensionStmt;
      std::unique_ptr<EquivalenceStmt> equivalenceStmt;
      std::unique_ptr<ExternalStmt> externalStmt;
      std::unique_ptr<IntentStmt> intentStmt;
      std::unique_ptr<IntrinsicStmt> intrinsicStmt;
      std::unique_ptr<NamelistStmt> namelistStmt;
      std::unique_ptr<OptionalStmt> optionalStmt;
      std::unique_ptr<PointerStmt> pointerStmt;
      std::unique_ptr<SaveStmt> saveStmt;
      std::unique_ptr<TargetStmt> targetStmt;
    };
  };

  struct StmtFunctionStmt {
  };

  enum class DeclarationConstructType {
    DERIVED_TYPE_DEF,
    INTERFACE_BLOCK,
    TYPE_DECLARATION_STMT,
    SPECIFICATION_STMT,
    PARAMETER_STMT,
    FORMAT_STMT,
    ENTRY_STMT,
    STMT_FUNCTION_STMT,
  };

  struct DeclarationConstruct {
    DeclarationConstructType type;
    union {
      std::unique_ptr<DerivedTypeDef> derivedTypeDef;
      std::unique_ptr<InterfaceBlock> interfaceBlock;
      std::unique_ptr<TypeDeclarationStmt> typeDeclarationStmt;
      std::unique_ptr<SpecificationStmt> specificationStmt;
      std::unique_ptr<ParameterStmt> parameterStmt;
      std::unique_ptr<FormatStmt> formatStmt;
      std::unique_ptr<EntryStmt> entryStmt;
      std::unique_ptr<StmtFunctionStmt> stmtFunctionStmt;
    };
  };

  struct SpecificationPart {
    std::vector<std::unique_ptr<UseStmt>> useStmts;
    std::optional<std::unique_ptr<ImplicitPart>> implicitPart;
    std::vector<std::unique_ptr<DeclarationConstruct>> declarationConstructs;
  };

  struct StatVariable {
    std::unique_ptr<ScalarIntVariable> scalarIntVariable;
  };

  struct AllocateUpperBound {
    std::unique_ptr<ScalarIntExpr> scalarIntExpr;
  };

  struct AllocateLowerBound {
    std::unique_ptr<ScalarIntExpr> scalarIntExpr;
  };

  struct AllocateLowerBoundTo {
    std::unique_ptr<AllocateLowerBound> allocateLowerBound;
    std::unique_ptr<Token> toToken;  // ":"
  };

  struct AllocateShapeSpec {
    std::optional<std::unique_ptr<AllocateLowerBoundTo>> allocateLowerBoundTo;
    std::unique_ptr<AllocateUpperBound> allocateUpperBound;
  };

  struct DelimitedAllocateShapeSpec {
    std::unique_ptr<Token> delimiterToken;
    std::unique_ptr<AllocateShapeSpec> allocateShapeSpec;
  };

  struct AllocateShapeSpecList {
    // Constraint: The number of AllocateShapeSpecs in an AllocateShapeSpecList
    // must be the same as the rank of the pointer or allocatable array.
    // ------------------------------------------------------------------------
    std::unique_ptr<AllocateShapeSpec> firstAllocateShapeSpec;
    std::vector<std::unique_ptr<DelimitedAllocateShapeSpec>> restAllocateShapeSpecs;
  };

  struct ParenthesizedAllocateShapeSpecList {
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<AllocateShapeSpecList> allocateShapeSpecs;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  enum class AllocateObjectType {
    VARIABLE_NAME,
    STRUCTURE_COMPONENT,
  };

  struct AllocateObject {
    // Constraint: Each AllocateObject must be a pointer or an allocatable
    // array.
    // -------------------------------------------------------------------
    AllocateObjectType type;
    union {
      std::unique_ptr<VariableName> variableName;
      std::unique_ptr<StructureComponent> structureComponent;
    };
  };

  using AllocateObjectList = DelimitedList<AllocateObject>;

  struct Allocation {
    std::unique_ptr<AllocateObject> allocateObject;
    std::optional<std::unique_ptr<ParenthesizedAllocateShapeSpecList>> parenthesizedAllocateShapeSpecList;
  };

  using AllocationList = DelimitedList<Allocation>;

  struct DelimitedStatIsStatVariable {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<Token> statToken;  // "STAT"
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<StatVariable> statVariable;
  };

  struct AllocateStmt {
    std::unique_ptr<Token> allocateToken;  // "ALLOCATE"
    std::unique_ptr<Token> openingParenToken;  // "("
    std::optional<std::unique_ptr<DelimitedStatIsStatVariable>> delimitedStatIsStatVariable;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  struct AssignmentStmt {
    // Constraint: A Variable in an AssignmentStmt must not be an assumed-size
    // array.
    // -----------------------------------------------------------------------
    std::unique_ptr<Variable> variable;
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<Expr> expr;
  };

  struct UnitPositionSpec {
    std::unique_ptr<UnitAssignment> unitAssignment;
    std::unique_ptr<ExternalFileUnit> externalFileUnit;
  };

  struct IostatPositionSpec {
    std::unique_ptr<Token> iostatToken;  // "IOSTAT"
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<ScalarDefaultIntVariable> scalarDefaultIntVariable;
  };

  struct ErrPositionSpec {
    std::unique_ptr<Token> errToken;  // "ERR"
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<Label> label;
  };

  enum class PositionSpecType {
    UNIT,
    IOSTAT,
    ERR,
  };

  struct PositionSpec {
    // Constraint: The Label in the "ERR=" specifier must be the statement label
    // of a branch target statement that appears in the same scoping unit as the
    // file positioning statement.
    // -------------------------------------------------------------------------
    // Constraint: If the optional characters "UNIT=" are omitted from the unit
    // specifier, the unit specifier must be the first item in the
    // PositionSpecList.
    // -------------------------------------------------------------------------
    // Constraint: A PositionSpecList must contain exactly one ExternalFileUnit
    // and may contain at most one of each of the other specifiers.
    // -------------------------------------------------------------------------
    PositionSpecType type;
    union {
      std::unique_ptr<UnitPositionSpec> unitPositionSpec;
      std::unique_ptr<IostatPositionSpec> iostatPositionSpec;
      std::unique_ptr<ErrPositionSpec> errPositionSpec;
    };
  };

  using PositionSpecList = DelimitedList<PositionSpec>;

  struct BackspaceExternalFileUnit {
    std::unique_ptr<Token> backspaceToken;  // "BACKSPACE"
    std::unique_ptr<ExternalFileUnit> externalFileUnit;
  };

  struct BackspacePositionSpecList {
    std::unique_ptr<Token> backspaceToken;  // "BACKSPACE"
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<PositionSpecList> positionSpecList;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  struct EndfileExternalFileUnit {
    std::unique_ptr<Token> endfileToken;  // "ENDFILE"
    std::unique_ptr<ExternalFileUnit> externalFileUnit;
  };

  struct EndfilePositionSpecList {
    std::unique_ptr<Token> endfileToken;  // "ENDFILE"
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<PositionSpecList> positionSpecList;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  struct RewindExternalFileUnit {
    std::unique_ptr<Token> rewindToken;  // "REWIND"
    std::unique_ptr<ExternalFileUnit> externalFileUnit;
  };

  struct RewindPositionSpecList {
    std::unique_ptr<Token> rewindToken;  // "REWIND"
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<PositionSpecList> positionSpecList;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  enum class FilePositioningType {
    EXTERNAL_FILE_UNIT,
    POSITION_SPEC_LIST,
  };

  struct BackspaceStmt {
    FilePositioningType type;
    union {
      std::unique_ptr<BackspaceExternalFileUnit> backspaceExternalFileUnit;
      std::unique_ptr<BackspacePositionSpecList> backspacePositionSpecList;
    };
  };

  struct CallStmt {
  };

  enum class CloseSpecType {
    UNIT,
    IOSTAT,
    ERR,
    STATUS,
  };

  struct CloseSpec {
    CloseSpecType type;
    union {
      std::unique_ptr<UnitConnectSpec> unitConnectSpec;
      std::unique_ptr<IostatConnectSpec> iostatConnectSpec;
      std::unique_ptr<ErrConnectSpec> errConnectSpec;
      std::unique_ptr<StatusConnectSpec> statusConnectSpec;
    };
  };

  struct DelimitedCloseSpec {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<CloseSpec> closeSpec;
  };

  struct CloseSpecList {
    std::unique_ptr<CloseSpec> firstCloseSpec;
    std::vector<std::unique_ptr<DelimitedCloseSpec>> restCloseSpecs;
  };

  struct CloseStmt {
    // Constraint: If the optional characters "UNIT =" are omitted from the unit
    // specifier, the unit specifier must be the first item in the
    // CloseSpecList.
    // -------------------------------------------------------------------------
    // Constraint: Each specifier must not appear more than once in a given
    // CloseStmt; an ExternalFileUnit must be specified.
    // -------------------------------------------------------------------------
    // Constraint: The Label used in the "ERR =" specifier must be the statement
    // label of a branch target statement that appears in the same scoping unit
    // as the CLOSE statement.
    // -------------------------------------------------------------------------
    std::unique_ptr<Token> closeToken;  // "CLOSE"
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<CloseSpecList> closeSpecList;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  struct DelimitedLabel {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<Label> label;
  };

  struct LabelList {
    std::unique_ptr<Label> firstLabel;
    std::vector<std::unique_ptr<DelimitedLabel>> restLabels;
  };

  struct ComputedGotoStmt {
    // Constraint: Each Label in a LabelList must be the statement label of a
    // branch target statement that appears in the same scoping unit as the
    // ComputedGotoStmt.
    // ----------------------------------------------------------------------
    std::unique_ptr<Token> goToken;  // "GO"
    std::unique_ptr<Token> toLabel;  // "TO"
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<LabelList> labelList;
    std::unique_ptr<Token> closingParenToken;  // ")"
    std::optional<std::unique_ptr<Token>> delimiterToken;  // ","
    std::unique_ptr<ScalarIntExpr> scalarIntExpr;
  };

  struct ContinueStmt {
    std::unique_ptr<Token> continueToken;  // "CONTINUE"
  };

  struct CycleStmt {
    // Constraint: If a CycleStmt refers to a DoConstructName, it must be within
    // the range of that DoConstruct; otherwise, it must be within the range of
    // at least one DoConstruct.
    // -------------------------------------------------------------------------
    std::unique_ptr<Token> cycleToken;  // "CYCLE"
    std::optional<std::unique_ptr<DoConstructName>> doConstructName;
  };

  struct DeallocateStmt {
    // Constraint: Each AllocateObject must be a pointer or an allocatable
    // array.
    // -------------------------------------------------------------------
    std::unique_ptr<Token> deallocateToken;  // "DEALLOCATE"
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<AllocateObjectList> allocateObjectList;
    std::optional<std::unique_ptr<DelimitedStatIsStatVariable>> delimitedStatIsStatVariable;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  struct EndfileStmt {
  };

  struct EndFunctionStmt {
  };

  struct EndProgramStmt {
  };

  struct EndSubroutineStmt {
  };

  struct ExitStmt {
    // Constraint: If an ExitStmt refers to a DoConstructName, it must be within
    // the range of that DoConstruct; otherwise, it must be within the range of
    // at least one DoConstruct.
    // -------------------------------------------------------------------------
    std::unique_ptr<Token> exitToken;  // "EXIT"
    std::optional<std::unique_ptr<DoConstructName>> doConstructName;
  };

  struct GotoStmt {
    // Constraint: The Label must be the statement label of a branch target
    // statement that appears in the same scoping unit as the GotoStmt.
    // --------------------------------------------------------------------
    std::unique_ptr<Token> goToken;  // "GO"
    std::unique_ptr<Token> toLabel;  // "TO"
    std::unique_ptr<Label> label;
  };

  struct IfStmt {
    // Constraint: The ActionStmt in the IfStmt must not be an IfStmt,
    // EndProgramStmt, EndFunctionStmt, or EndSubroutineStmt.
    // ---------------------------------------------------------------
    std::unique_ptr<Token> ifToken;  // "IF"
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<ScalarLogicalExpr> scalarLogicalExpr;
    std::unique_ptr<Token> closingParenToken;  // ")"
    std::unique_ptr<ActionStmt> actionStmt;
  };

  struct UnitInquireSpec {
    std::unique_ptr<UnitAssignment> unitAssignment;
    std::unique_ptr<ExternalFileUnit> externalFileUnit;
  };

  struct FileInquireSpec {
    std::unique_ptr<Token> fileToken;  // "FILE"
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<FileNameExpr> fileNameExpr;
  };

  struct IostatInquireSpec {
    std::unique_ptr<Token> iostatToken;  // "IOSTAT"
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<ScalarDefaultIntVariable> scalarDefaultIntVariable;
  };

  struct ErrInquireSpec {
  };

  struct ExistInquireSpec {
  };

  struct OpenedInquireSpec {
  };

  struct NumberInquireSpec {
  };

  struct NamedInquireSpec {
  };

  struct NameInquireSpec {
  };

  struct AccessInquireSpec {
  };

  struct SequentialInquireSpec {
  };

  struct DirectInquireSpec {
  };

  struct FormInquireSpec {
  };

  struct FormattedInquireSpec {
  };

  struct UnformattedInquireSpec {
  };

  struct ReclInquireSpec {
  };

  struct NextrecInquireSpec {
  };

  struct BlankInquireSpec {
  };

  struct PositionInquireSpec {
  };

  struct ActionInquireSpec {
  };

  struct ReadInquireSpec {
  };

  struct WriteInquireSpec {
  };

  struct ReadwriteInquireSpec {
  };

  struct DelimInquireSpec {
  };

  struct PadInquireSpec {
  };

  enum class InquireSpecType {
    UNIT,
    FILE,
    IOSTAT,
    ERR,
    EXIST,
    OPENED,
    NUMBER,
    NAMED,
    NAME,
    ACCESS,
    SEQUENTIAL,
    DIRECT,
    FORM,
    FORMATTED,
    UNFORMATTED,
    RECL,
    NEXTREC,
    BLANK,
    POSITION,
    ACTION,
    READ,
    WRITE,
    READWRITE,
    DELIM,
    PAD,
  };

  struct InquireSpec {
    InquireSpecType type;
    union {
      std::unique_ptr<UnitInquireSpec> unitInquireSpec;
      std::unique_ptr<FileInquireSpec> fileInquireSpec;
      std::unique_ptr<IostatInquireSpec> iostatInquireSpec;
      std::unique_ptr<ErrInquireSpec> errInquireSpec;
      std::unique_ptr<ExistInquireSpec> existInquireSpec;
      std::unique_ptr<OpenedInquireSpec> openedInquireSpec;
      std::unique_ptr<NumberInquireSpec> numberInquireSpec;
      std::unique_ptr<NamedInquireSpec> namedInquireSpec;
      std::unique_ptr<NameInquireSpec> nameInquireSpec;
      std::unique_ptr<AccessInquireSpec> accessInquireSpec;
      std::unique_ptr<SequentialInquireSpec> sequentialInquireSpec;
      std::unique_ptr<DirectInquireSpec> directInquireSpec;
      std::unique_ptr<FormInquireSpec> formInquireSpec;
      std::unique_ptr<FormattedInquireSpec> formattedInquireSpec;
      std::unique_ptr<UnformattedInquireSpec> unformattedInquireSpec;
      std::unique_ptr<ReclInquireSpec> reclInquireSpec;
      std::unique_ptr<NextrecInquireSpec> nextrecInquireSpec;
      std::unique_ptr<BlankInquireSpec> blankInquireSpec;
      std::unique_ptr<PositionInquireSpec> positionInquireSpec;
      std::unique_ptr<ActionInquireSpec> actionInquireSpec;
      std::unique_ptr<ReadInquireSpec> readInquireSpec;
      std::unique_ptr<WriteInquireSpec> writeInquireSpec;
      std::unique_ptr<ReadwriteInquireSpec> readwriteInquireSpec;
      std::unique_ptr<DelimInquireSpec> delimInquireSpec;
      std::unique_ptr<PadInquireSpec> padInquireSpec;
    };
  };

  using InquireSpecList = DelimitedList<InquireSpec>;

  struct InquireStmtWithInquireSpecList {
    std::unique_ptr<Token> inquireToken;  // "INQUIRE"
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<Token> iolengthToken;  // "IOLENGTH"
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<ScalarDefaultIntVariable> scalarDefaultIntVariable;
    std::unique_ptr<Token> closingParenToken;  // ")"
    std::unique_ptr<OutputItemList> outputItemList;
  };

  struct InquireStmtWithOutputItemList {
    std::unique_ptr<Token> inquireToken;  // "INQUIRE"
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<InquireSpecList> inquireSpecList;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  enum class InquireStmtType {
    INQUIRE_SPEC_LIST,
    OTUPUT_ITEM_LIST,
  };

  struct InquireStmt {
    InquireStmtType type;
    union {
      std::unique_ptr<InquireStmtWithInquireSpecList> inquireStmtWithInquireSpecList;
      std::unique_ptr<InquireStmtWithOutputItemList> inquireStmtWithOutputItemList;
    };
  };

  struct PointerObject {
    // Constraint: Each PointerObject must have the POINTER attribute.
    // ---------------------------------------------------------------
  };

  struct DelimitedPointerObject {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<PointerObject> pointerObject;
  };

  struct PointerObjectList {
    std::unique_ptr<PointerObject> firstPointerObject;
    std::vector<std::unique_ptr<DelimitedPointerObject>> restPointerObjects;
  };

  struct NullifyStmt {
    std::unique_ptr<Token> nullifyToken;  // "NULLIFY"
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<PointerObjectList> pointerObjects;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  struct UnitAssignment {
    std::unique_ptr<Token> unitToken;  // "UNIT"
    std::unique_ptr<Token> assignToken;  // "="
  };

  struct UnitConnectSpec {
    std::optional<std::unique_ptr<UnitAssignment>> unitAssignment;
    std::unique_ptr<ExternalFileUnit> externalFileUnit;
  };

  struct ScalarDefaultIntVariable {
  };

  struct IostatConnectSpec {
    std::unique_ptr<Token> iostatToken;  // "IOSTAT"
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<ScalarDefaultIntVariable> scalarDefaultIntVariable;
  };

  struct ErrConnectSpec {
    std::unique_ptr<Token> errToken;  // "ERR"
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<Label> label;
  };

  struct ScalarDefaultCharExpr {
  };

  struct FileNameExpr {
    std::unique_ptr<ScalarDefaultCharExpr> scalarDefaultCharExpr;
  };

  struct FileConnectSpec {
    std::unique_ptr<Token> fileToken;  // "FILE"
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<FileNameExpr> fileNameExpr;
  };

  struct StatusConnectSpec {
    std::unique_ptr<Token> statusToken;  // "STATUS"
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<ScalarDefaultCharExpr> scalarDefaultCharExpr;
  };

  struct AccessConnectSpec {
    std::unique_ptr<Token> accessToken;  // "ACCESS"
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<ScalarDefaultCharExpr> scalarDefaultCharExpr;
  };

  struct FormConnectSpec {
    std::unique_ptr<Token> formToken;  // "FORM"
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<ScalarDefaultCharExpr> scalarDefaultCharExpr;
  };

  struct ReclConnectSpec {
    std::unique_ptr<Token> reclToken;  // "RECL"
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<ScalarIntExpr> scalarIntExpr;
  };

  struct BlankConnectSpec {
    std::unique_ptr<Token> blankToken;  // "BLANK"
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<ScalarDefaultCharExpr> scalarDefaultCharExpr;
  };

  struct PositionConnectSpec {
    std::unique_ptr<Token> positionToken;  // "POSITION"
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<ScalarDefaultCharExpr> scalarDefaultCharExpr;
  };

  struct ActionConnectSpec {
    std::unique_ptr<Token> actionToken;  // "ACTION"
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<ScalarDefaultCharExpr> scalarDefaultCharExpr;
  };

  struct DelimConnectSpec {
    std::unique_ptr<Token> delimToken;  // "DELIM"
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<ScalarDefaultCharExpr> scalarDefaultCharExpr;
  };

  struct PadConnectSpec {
    std::unique_ptr<Token> padToken;  // "PAD"
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<ScalarDefaultCharExpr> scalarDefaultCharExpr;
  };

  enum class ConnectSpecType {
    UNIT,
    IOSTAT,
    ERR,
    FILE,
    STATUS,
    ACCESS,
    FORM,
    RECL,
    BLANK,
    POSITION,
    ACTION,
    DELIM,
    PAD,
  };

  struct ConnectSpec {
    ConnectSpecType type;
    union {
      std::unique_ptr<UnitConnectSpec> unitConnectSpec;
      std::unique_ptr<IostatConnectSpec> iostatConnectSpec;
      std::unique_ptr<ErrConnectSpec> errConnectSpec;
      std::unique_ptr<FileConnectSpec> fileConnectSpec;
      std::unique_ptr<StatusConnectSpec> statusConnectSpec;
      std::unique_ptr<AccessConnectSpec> accessConnectSpec;
      std::unique_ptr<FormConnectSpec> formConnectSpec;
      std::unique_ptr<ReclConnectSpec> reclConnectSpec;
      std::unique_ptr<BlankConnectSpec> blankConnectSpec;
      std::unique_ptr<PositionConnectSpec> positionConnectSpec;
      std::unique_ptr<ActionConnectSpec> actionConnectSpec;
      std::unique_ptr<DelimConnectSpec> delimConnectSpec;
      std::unique_ptr<PadConnectSpec> padConnectSpec;
    };
  };

  struct DelimitedConnectSpec {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<ConnectSpec> connectSpec;
  };

  struct ConnectSpecList {
    std::unique_ptr<ConnectSpec> firstConnectSpec;
    std::vector<std::unique_ptr<DelimitedConnectSpec>> restConnectSpecs;
  };

  struct OpenStmt {
    // Constraint: If the optional characters "UNIT =" are omitted from the unit
    // specifier, the unit specifier must be the first item in the
    // ConnectSpecList.
    // -------------------------------------------------------------------------
    // Constraint: Each specifier must not appear more than once in a given
    // OpenStmt; an ExternalFileUnit must be specified.
    // -------------------------------------------------------------------------
    // Constraint: The Label used in the "ERR =" specifier must be the statement
    // label of a branch target statement that appears in the same scoping unit
    // as the OPEN statement.
    // -------------------------------------------------------------------------
    std::unique_ptr<Token> openToken;  // "OPEN"
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<ConnectSpecList> connectSpecList;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  enum class TargetType {
    VARIABLE,
    EXPR,
  };

  struct Target {
    TargetType type;
    union {
      std::unique_ptr<Variable> variable;
      std::unique_ptr<Expr> expr;
    };
  };

  struct PointerAssignmentStmt {
    // Constraint: The PointerObject must have the POINTER attribute.
    // -------------------------------------------------------------------------
    // Constraint: The Variable must have the TARGET attribute or be a subobject
    // of an object with the TARGET attribute, or it must have the POINTER
    // attribute.
    // -------------------------------------------------------------------------
    // Constraint: The Target must be of the same type, type parameters, and
    // rank as the pointer.
    // -------------------------------------------------------------------------
    // Constraint: The Target must not be an array section with a vector
    // subscript.
    // -------------------------------------------------------------------------
    // Constraint: The Expr must deliver a pointer result.
    // -------------------------------------------------------------------------
    std::unique_ptr<PointerObject> pointerObject;
    std::unique_ptr<Token> mapsToToken;  // "=>"
    std::unique_ptr<Target> target;
  };

  enum class FormatType {
    DEFAULT_CHAR_EXPR,
    LABEL,
    STAR,
    SCALAR_DEFAULT_INT_VARIABLE,
  };

  struct Format {
    // Constraint: The Label must be the label of a FORMAT statement that
    // appears in the same scoping unit as the statement containing the format
    // specifier.
    // -----------------------------------------------------------------------
    FormatType type;
    union {
      std::unique_ptr<DefaultCharExpr> defaultCharExpr;
      std::unique_ptr<Label> label;
      std::unique_ptr<Token> starToken;  // "*"
      std::unique_ptr<ScalarDefaultIntVariable> scalarDefaultIntVariable;
    };
  };

  struct UnitIoSpec {
    std::optional<std::unique_ptr<UnitAssignment>> unitAssignment;
    std::unique_ptr<IoUnit> ioUnit;
  };

  struct FormatAssignment {
    std::unique_ptr<Token> formatToken;  // "FMT"
    std::unique_ptr<Token> assignToken;  // "="
  };

  struct FormatIoSpec {
    std::optional<std::unique_ptr<FormatAssignment>> formatAssignment;
    std::unique_ptr<Format> format;
  };

  struct NamelistAssignment {
    std::unique_ptr<Token> namelistToken;  // "NML"
    std::unique_ptr<Token> assignToken;  // "="
  };

  struct NamelistIoSpec {
    std::optional<std::unique_ptr<NamelistAssignment>> namelistAssignment;
    std::unique_ptr<NamelistGroupName> namelistGroupName;
  };

  struct RecIoSpec {
    std::unique_ptr<Token> recToken;  // "REC"
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<ScalarIntExpr> scalarIntExpr;
  };

  struct IostatIoSpec {
    std::unique_ptr<Token> iostatToken;  // "IOSTAT"
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<ScalarDefaultIntVariable> scalarDefaultIntVariable;
  };

  struct ErrIoSpec {
    std::unique_ptr<Token> errToken;  // "ERR"
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<Label> label;
  };

  struct EndIoSpec {
    std::unique_ptr<Token> endToken;  // "END"
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<Label> label;
  };

  struct AdvanceIoSpec {
    std::unique_ptr<Token> advanceToken;  // "ADVANCE"
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<ScalarDefaultCharVariable> scalarDefaultCharVariable;
  };

  struct SizeIoSpec {
    std::unique_ptr<Token> sizeToken;  // "SIZE"
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<ScalarDefaultIntVariable> scalarDefaultIntVariable;
  };

  struct EorIoSpec {
    std::unique_ptr<Token> eorToken;  // "EOR"
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<Label> label;
  };

  enum class IoControlSpecType {
    UNIT,
    FMT,
    NML,
    REC,
    IOSTAT,
    ERR,
    END,
    ADVANCE,
    SIZE,
    EOR,
  };

  struct IoControlSpec {
    // Constraint: The Label in the "ERR=", "EOR=", or "END=" specifier must be
    // the statement label of a branch target statement that appears in the same
    // scoping unit as the data transfer statement.
    // -------------------------------------------------------------------------
    // Constraint: A NamelistGroupName must not be present if an InputItemList
    // or an OutputItemList is present in the data transfer statement.
    // -------------------------------------------------------------------------
    // Constraint: An IoControlSpecList must not contain both a Format and a
    // NamelistGroupName.
    // -------------------------------------------------------------------------
    // Constraint: If the optional characters "UNIT=" are omitted from the unit
    // specifier, the unit specifier must be the first item in the control
    // information list.
    // -------------------------------------------------------------------------
    // Constraint: If the optional characters "FMT=" are omitted from the format
    // specifier, the format specifier must be the second item in the control
    // information list and the first item must be the unit specifier without
    // the optional characters "UNIT=".
    // -------------------------------------------------------------------------
    // Constraint: If the optional characters "NML=" are omitted from the
    // namelist specifier, the namelist specifier must be the second item in the
    // control information list and the first item must be the unit specifier
    // wihtout the optional characters "UNIT=".
    // -------------------------------------------------------------------------
    // Constraint: If the unit specifier specifies an internal file, the
    // IoControlSpecList must not contain a "REC=" specifier or a
    // NamelistGroupName.
    // -------------------------------------------------------------------------
    // Constraint: If the "REC=" specifier is present, an "END=" specifier must
    // not appear, a NamelistGroupName must not appear, and the Format, if any,
    // must not be an asterisk specifying list-directed input/output.
    // -------------------------------------------------------------------------
    // Constraint: An "ADVANCE=" specifier may be present only in a formatted
    // sequential input/output statement with explicit format specification
    // whose control information list does not contain an internal file unit
    // specifier.
    // -------------------------------------------------------------------------
    // Constraint: If an "EOR=" specifier is present, an "ADVANCE=" specifier
    // also must appear.
    // -------------------------------------------------------------------------
    // Constraint: If a "SIZE=" specifier is present, an "ADVANCE=" specifier
    // also must appear.
    // -------------------------------------------------------------------------
    IoControlSpecType type;
    union {
      std::unique_ptr<UnitIoSpec> unitIoSpec;
      std::unique_ptr<FormatIoSpec> formatIoSpec;
      std::unique_ptr<NamelistIoSpec> namelistIoSpec;
      std::unique_ptr<RecIoSpec> recIoSpec;
      std::unique_ptr<IostatIoSpec> iostatIoSpec;
      std::unique_ptr<ErrIoSpec> errIoSpec;
      std::unique_ptr<EndIoSpec> endIoSpec;
      std::unique_ptr<AdvanceIoSpec> advanceIoSpec;
      std::unique_ptr<SizeIoSpec> sizeIoSpec;
      std::unique_ptr<EorIoSpec> eorIoSpec;
    };
  };

  struct DelimitedIoControlSpec {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<IoControlSpec> ioControlSpec;
  };

  struct IoControlSpecList {
    // Constraint: An IoControlSpecList must contain exactly one IoUnit and may
    // contain at most one of each of the other specifiers.
    // ------------------------------------------------------------------------
    std::unique_ptr<IoControlSpec> firstIoControlSpec;
    std::vector<std::unique_ptr<DelimitedIoControlSpec>> restIoControlSpecs;
  };

  enum class IoImpliedDoObjectType {
    INPUT_ITEM,
    OUTPUT_ITEM,
  };

  struct IoImpliedDoObject {
    // Constraint: In an InputItemList, and IoImpliedDoObject must be an
    // InputItem. In an OutputItemList, an IoImpliedDoObject must be an
    // OutputItem.
    // -----------------------------------------------------------------
    IoImpliedDoObjectType type;
    union {
      std::unique_ptr<InputItem> inputItem;
      std::unique_ptr<OutputItem> outputItem;
    };
  };

  struct DelimitedIoImpliedDoObject {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<IoImpliedDoObject> ioImpliedDoObject;
  };

  struct IoImpliedDoObjectList {
    std::unique_ptr<IoImpliedDoObject> firstIoImpliedDoObject;
    std::vector<std::unique_ptr<DelimitedIoImpliedDoObject>> restIoImpliedDoObjects;
  };

  struct IoImpliedDoControl {
    // Constraint: The DoVariable must be a named scalar variable of type
    // integer, default real, or double precision real.
    // ----------------------------------------------------------------------
    // Constraint: Each ScalarNumericExpr in an IoImpliedDoControl must be of
    // type integer, default real, or double precision real.
    // ----------------------------------------------------------------------
    std::unique_ptr<DoVariable> doVariable;
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<ScalarNumericExpr> fromScalarNumericExpr;
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<ScalarNumericExpr> toScalarNumericExpr;
    std::optional<std::unique_ptr<LoopControlStepSize>> loopControlStepSize;
  };

  struct IoImpliedDo {
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<IoImpliedDoObjectList> ioImpliedDoObjectList;
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<IoImpliedDoControl> ioImpliedDoControl;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  enum class InputItemType {
    VARIABLE,
    IO_IMPLIED_DO,
  };

  struct InputItem {
    // Constraint: A Variable that is an InputItem must not be an assumed-size
    // array.
    // -----------------------------------------------------------------------
    InputItemType type;
    union {
      std::unique_ptr<Variable> variable;
      std::unique_ptr<IoImpliedDo> ioImpliedDo;
    };
  };

  enum class OutputItemType {
    EXPR,
    IO_IMPLIED_DO,
  };

  struct OutputItem {
    OutputItemType type;
    union {
      std::unique_ptr<Expr> expr;
      std::unique_ptr<IoImpliedDo> ioImpliedDo;
    };
  };

  using OutputItemList = DelimitedList<OutputItem>;

  struct DelimitedOutputItemList {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<OutputItemList> outputItemList;
  };

  struct PrintStmt {
    std::unique_ptr<Token> printToken;  // "PRINT"
    std::unique_ptr<Format> format;
    std::optional<std::unique_ptr<DelimitedOutputItemList>> delimitedOutputItemList;
  };

  struct ReadStmtWithSpecList {
    std::unique_ptr<Token> readToken;  // "READ"
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<IoControlSpecList> ioControlSpecList;
    std::unique_ptr<Token> closingParenToken;  // ")"
    std::optional<std::unique_ptr<OutputItemList>> outputItemList;
  };

  struct ReadStmtWithFormat {
    std::unique_ptr<Token> readToken;  // "READ"
    std::unique_ptr<Format> format;
    std::optional<std::unique_ptr<DelimitedOutputItemList>> delimitedOutputItemList;
  };

  enum class ReadStmtType {
    SPEC_LIST,
    FORMAT,
  };

  struct ReadStmt {
    ReadStmtType type;
    union {
    };
  };

  struct ReturnStmt {
  };

  struct RewindStmt {
  };

  enum class StopCodeType {
    SCALAR_CHAR_CONSTANT,
    DIGIT_STRING,
  };

  struct StopCode {
    // Constraint: ScalarCharConstant must be of type default character.
    // -----------------------------------------------------------------
    StopCodeType type;
    union {
      std::unique_ptr<ScalarCharConstant> scalarCharConstant;
      std::unique_ptr<Token> digitString;  // ( digit ){1,5}
    };
  };

  struct StopStmt {
    std::unique_ptr<Token> stopToken;  // "STOP"
    std::optional<std::unique_ptr<StopCode>> stopCode;
  };

  struct MaskExpr {
    std::unique_ptr<LogicalExpr> logicalExpr;
  };

  struct WhereStmt {
    std::unique_ptr<Token> whereToken;  // "WHERE"
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<MaskExpr> maskExpr;
    std::unique_ptr<Token> closingParenToken;  // ")"
    std::unique_ptr<AssignmentStmt> assignmentStmt;
  };

  struct WriteStmt {
    // Constraint: An "END=", "EOR=", or "SIZE=" specifier must not appear in a
    // WriteStmt.
    // ------------------------------------------------------------------------
    std::unique_ptr<Token> writeToken;  // "WRITE"
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<IoControlSpecList> ioControlSpecList;
    std::unique_ptr<Token> closingParenToken;  // ")"
    std::optional<std::unique_ptr<OutputItemList>> outputItemList;
  };

  struct ArithmeticIfStmt {
    // Constraint: Each Label must be the label of a branch target statement
    // that appears in the same scoping unit as the ArithmeticIfStmt.
    // ---------------------------------------------------------------------
    // Constraint: The ScalarNumericExpr must not be of type complex.
    // ---------------------------------------------------------------------
    std::unique_ptr<Token> ifToken;  // "IF"
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<ScalarNumericExpr> scalarNumericExpr;
    std::unique_ptr<Token> closingParenToken;  // ")"
    std::unique_ptr<Label> firstLabel;
    std::unique_ptr<Token> firstDelimiterToken;  // ","
    std::unique_ptr<Label> secondLabel;
    std::unique_ptr<Token> secondDelimiterToken;  // ","
    std::unique_ptr<Label> thirdLabel;
  };

  struct AssignStmt {
    // Constraint: The Label must be the statement label of a branch target
    // statement or FormatStmt that appears in the same scoping unit as the
    // AssignStmt.
    // ------------------------------------------------------------------------
    // Constraint: ScalarIntVariable must be named and of type default integer.
    // ------------------------------------------------------------------------
    std::unique_ptr<Token> assignToken;  // "ASSIGN"
    std::unique_ptr<Label> label;
    std::unique_ptr<Token> toToken;  // "TO"
    std::unique_ptr<ScalarIntVariable> scalarIntVariable;
  };

  struct DelimitedParenthesizedLabelList {
    std::optional<std::unique_ptr<Token>> delimiterToken;  // ","
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<LabelList> labelList;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  struct AssignedGotoStmt {
    // Constraint: Each Label in a LabelList must be the statement label of a
    // branch target statement that appears in the same coping unit as the
    // AssignedGotoStmt.
    // ------------------------------------------------------------------------
    // Constraint: ScalarIntVariable must be named and of type default integer.
    // ------------------------------------------------------------------------
    std::unique_ptr<Token> goToken;  // "GO"
    std::unique_ptr<Token> toToken;  // "TO"
    std::unique_ptr<ScalarIntVariable> scalarIntVariable;
    std::optional<std::unique_ptr<DelimitedParenthesizedLabelList>> labels;
  };

  struct PauseStmt {
    std::unique_ptr<Token> pauseToken;  // "PAUSE"
    std::optional<std::unique_ptr<StopCode>> stopCode;
  };

  enum class ActionStmtType {
    ALLOCATE_STMT,
    ASSIGNMENT_STMT,
    BACKSPACE_STMT,
    CALL_STMT,
    CLOSE_STMT,
    COMPUTED_GOTO_STMT,
    CONTINUE_STMT,
    CYCLE_STMT,
    DEALLOCATE_STMT,
    ENDFILE_STMT,
    END_FUNCTION_STMT,
    END_PROGRAM_STMT,
    END_SUBROUTINE_STMT,
    EXIT_STMT,
    GOTO_STMT,
    IF_STMT,
    INQUIRE_STMT,
    NULLIFY_STMT,
    OPEN_STMT,
    POINTER_ASSIGNMENT_STMT,
    PRINT_STMT,
    READ_STMT,
    RETURN_STMT,
    REWIND_STMT,
    STOP_STMT,
    WHERE_STMT,
    WRITE_STMT,
    ARITHMETIC_IF_STMT,
    ASSIGN_STMT,
    ASSIGNED_GOTO_STMT,
    PAUSE_STMT,
  };

  struct ActionStmt {
    ActionStmtType type;
    union {
      std::unique_ptr<AllocateStmt> allocateStmt;
      std::unique_ptr<AssignmentStmt> assignmentStmt;
      std::unique_ptr<BackspaceStmt> backspaceStmt;
      std::unique_ptr<CallStmt> callStmt;
      std::unique_ptr<CloseStmt> closeStmt;
      std::unique_ptr<ComputedGotoStmt> computedGotoStmt;
      std::unique_ptr<ContinueStmt> continueStmt;
      std::unique_ptr<CycleStmt> cycleStmt;
      std::unique_ptr<DeallocateStmt> deallocateStmt;
      std::unique_ptr<EndfileStmt> endfileStmt;
      std::unique_ptr<EndFunctionStmt> endFunctionStmt;
      std::unique_ptr<EndProgramStmt> endProgramStmt;
      std::unique_ptr<EndSubroutineStmt> endSubroutineStmt;
      std::unique_ptr<ExitStmt> exitStmt;
      std::unique_ptr<GotoStmt> gotoStmt;
      std::unique_ptr<IfStmt> ifStmt;
      std::unique_ptr<InquireStmt> inquireStmt;
      std::unique_ptr<NullifyStmt> nullifyStmt;
      std::unique_ptr<OpenStmt> openStmt;
      std::unique_ptr<PointerAssignmentStmt> pointerAssignmentStmt;
      std::unique_ptr<PrintStmt> printStmt;
      std::unique_ptr<ReadStmt> readStmt;
      std::unique_ptr<ReturnStmt> returnStmt;
      std::unique_ptr<RewindStmt> rewindStmt;
      std::unique_ptr<StopStmt> stopStmt;
      std::unique_ptr<WhereStmt> whereStmt;
      std::unique_ptr<WriteStmt> writeStmt;
      std::unique_ptr<ArithmeticIfStmt> arithmeticIfStmt;
      std::unique_ptr<AssignStmt> assignStmt;
      std::unique_ptr<AssignedGotoStmt> assignedGotoStmt;
      std::unique_ptr<PauseStmt> pauseStmt;
    };
  };

  struct CaseConstructName {
    std::unique_ptr<Token> token;  // ( name )
  };

  struct CaseConstructNameDefinition {
    std::unique_ptr<CaseConstructName> caseConstructName;
    std::unique_ptr<Token> defineToken;  // ":"
  };

  enum class CaseExprType {
    SCALAR_INT_EXPR,
    SCALAR_CHAR_EXPR,
    SCALAR_LOGICAL_EXPR,
  };

  struct CaseExpr {
    CaseExprType type;
    union {
      std::unique_ptr<ScalarIntExpr> scalarIntExpr;
      std::unique_ptr<ScalarCharExpr> scalarCharExpr;
      std::unique_ptr<ScalarLogicalExpr> scalarLogicalExpr;
    };
  };

  struct SelectCaseStmt {
    std::optional<std::unique_ptr<CaseConstructNameDefinition>> caseConstructNameDefinition;
    std::unique_ptr<Token> selectToken;  // "SELECT"
    std::unique_ptr<Token> caseToken;  // "CASE"
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<CaseExpr> caseExpr;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  enum class CaseValueType {
    INT,
    CHAR,
    LOGICAL,
  };

  struct CaseValue {
    CaseValueType type;
    union {
      std::unique_ptr<ScalarIntInitializationExpr> scalarIntInitializationExpr;
      std::unique_ptr<ScalarCharInitializationExpr> scalarCharInitializationExpr;
      std::unique_ptr<ScalarLogicalInitializationExpr> scalarLogicalInitializationExpr;
    };
  };

  struct CaseValueTo {
    std::unique_ptr<CaseValue> caseValue;
    std::unique_ptr<Token> toToken;  // ":"
  };

  struct ToCaseValue {
    std::unique_ptr<Token> toToken;  // ":"
    std::unique_ptr<CaseValue> caseValue;
  };

  struct CaseValueToCaseValue {
    std::unique_ptr<CaseValue> fromCaseValue;
    std::unique_ptr<Token> toToken;  // ":"
    std::unique_ptr<CaseValue> toCaseValue;
  };

  enum class CaseValueRangeType {
    CASE_VALUE,
    CASE_VALUE_TO,
    TO_CASE_VALUE,
    CASE_VALUE_TO_CASE_VALUE,
  };

  struct CaseValueRange {
    CaseValueRangeType type;
    union {
      std::unique_ptr<CaseValue> caseValue;
      std::unique_ptr<CaseValueTo> caseValueTo;
      std::unique_ptr<ToCaseValue> toCaseValue;
      std::unique_ptr<CaseValueToCaseValue> caseValueToCaseValue;
    };
  };

  struct DelimitedCaseValueRange {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<CaseValueRange> caseValueRange;
  };

  struct CaseValueRangeList {
    std::unique_ptr<CaseValueRange> firstCaseValueRange;
    std::vector<std::unique_ptr<DelimitedCaseValueRange>> restCaseValueRanges;
  };

  struct ParenthesizedCaseValueRangeList {
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<CaseValueRangeList> caseValueRangeList;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  enum class CaseSelectorType {
    CASE_VALUE_RANGE_LIST,
    DEFAULT,
  };

  struct CaseSelector {
    CaseSelectorType type;
    union {
      std::unique_ptr<ParenthesizedCaseValueRangeList> parenthesizedCaseValueRangeList;
      std::unique_ptr<Token> defaultToken;  // "DEFAULT"
    };
  };

  struct CaseStmt {
    // Constraint: No more than one of the selectors of one of the CASE
    // statements may be DEFAULT.
    // ----------------------------------------------------------------
    std::unique_ptr<Token> caseToken;  // "CASE"
    std::unique_ptr<CaseSelector> caseSelector;
    std::optional<std::unique_ptr<CaseConstructName>> caseConstructName;
  };

  struct CaseStmtAndBlock {
    std::unique_ptr<CaseStmt> caseStmt;
    std::unique_ptr<Block> block;
  };

  struct EndSelectStmt {
    std::unique_ptr<Token> endToken;  // "END"
    std::unique_ptr<Token> selectToken;  // "SELECT"
    std::optional<std::unique_ptr<CaseConstructName>> caseConstructName;
  };

  struct CaseConstruct {
    // Constraint: If the SelectCaseStmt of a CaseConstruct is identified by a
    // CaseConstructName, the corresponding EndSelectStmt must specify the same
    // CaseConstructName. If the SelectCaseStmt of a CaseConstruct is not
    // identified by a CaseConstructName, the corresponding EndSelectStmt must
    // not specify a CaseConstructName. If a CaseStmt is identified by a
    // CaseConstructName, the corresponding SelectCaseStmt must specify the same
    // CaseConstructName.
    // -------------------------------------------------------------------------
    // Constraint: For a given CaseConstruct, each CaseValue must be of the same
    // type as CaseExpr. For character type, length differences are allowed, but
    // the kind type parameters must be the same.
    // -------------------------------------------------------------------------
    // Construct: A CaseValueRange using a colon must not be used if CaseExpr is
    // of type logical.
    // -------------------------------------------------------------------------
    // Constraint: For a given CaseConstruct, the CaseValueRanges must not
    // overlap; that is, there must be no possible value of the CaseExpr that
    // matches more than one CaseValueRange.
    // -------------------------------------------------------------------------
    std::unique_ptr<SelectCaseStmt> selectCaseStmt;
    std::vector<std::unique_ptr<CaseStmtAndBlock>> caseStmtsAndBlocks;
    std::unique_ptr<EndSelectStmt> endSelectStmt;
  };

  struct DoConstructName {
    std::unique_ptr<Token> token;  // ( name )
  };

  struct DoConstructNameDefinition {
    std::unique_ptr<DoConstructName> doConstructName;
    std::unique_ptr<Token> defineToken;  // ":"
  };

  struct ScalarVariable {
  };

  struct DoVariable {
    // Constraint: The DoVariable must be a named scalar variable of type
    // integer, default real, or double precision real.
    // ------------------------------------------------------------------
    std::unique_ptr<ScalarVariable> scalarVariable;
  };

  struct ScalarNumericExpr {
  };

  struct LoopControlStepSize {
    std::unique_ptr<Token> delimiterToken;  // ","
    std::unique_ptr<ScalarNumericExpr> scalarNumericExpr;
  };

  struct EnumeratedLoopControl {
    std::optional<std::unique_ptr<Token>> firstDelimiterToken;  // ","
    std::unique_ptr<DoVariable> doVariable;
    std::unique_ptr<Token> assignToken;  // "="
    std::unique_ptr<ScalarNumericExpr> fromScalarNumericExpr;
    std::unique_ptr<Token> secondDelimiterToken;  // ","
    std::unique_ptr<ScalarNumericExpr> toScalarNumericExpr;
    std::optional<std::unique_ptr<LoopControlStepSize>> loopControlStepSize;
  };

  struct ConditionalLoopControl {
    std::optional<std::unique_ptr<Token>> firstDelimiterToken;  // ","
    std::unique_ptr<Token> whileToken;  // "WHILE"
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<ScalarLogicalExpr> scalarLogicalExpr;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  enum class LoopControlType {
    ENUMERATED,
    CONDITIONAL,
  };

  struct LoopControl {
    // Constraint: Each ScalarNumericExpr in LoopControl must be of type
    // integer, default real, or double precision real.
    // -----------------------------------------------------------------
    LoopControlType type;
    union {
      std::unique_ptr<EnumeratedLoopControl> enumeratedLoopControl;
      std::unique_ptr<ConditionalLoopControl> conditionalLoopControl;
    };
  };

  struct LabelDoStmt {
    std::optional<std::unique_ptr<DoConstructNameDefinition>> doConstructNameDefinition;
    std::unique_ptr<Token> doToken;  // "DO"
    std::unique_ptr<Label> label;
    std::optional<std::unique_ptr<LoopControl>> loopControl;
  };

  struct NonlabelDoStmt {
    std::optional<std::unique_ptr<DoConstructNameDefinition>> doConstructNameDefinition;
    std::unique_ptr<Token> doToken;  // "DO"
    std::optional<std::unique_ptr<LoopControl>> loopControl;
  };

  enum class DoStmtType {
    LABEL,
    NONLABEL,
  };

  struct DoStmt {
    DoStmtType type;
    union {
      std::unique_ptr<LabelDoStmt> labelDoStmt;
      std::unique_ptr<NonlabelDoStmt> nonlabelDoStmt;
    };
  };

  struct DoBlock {
    std::unique_ptr<Block> block;
  };

  struct EndDoStmt {
    std::unique_ptr<Token> endToken;  // "END"
    std::unique_ptr<Token> doToken;  // "DO"
    std::optional<std::unique_ptr<DoConstructName>> doConstructName;
  };

  enum class EndDoType {
  };

  struct EndDo {
    EndDoType type;
    union {
      std::unique_ptr<EndDoStmt> endDoStmt;
      std::unique_ptr<ContinueStmt> continueStmt;
    };
  };

  struct BlockDoConstruct {
    // Constraint: If the DoStmt of a BlockDoConstruct is identified by a
    // DoConstructName, the corresponding EndDo must be an EndDoStmt specifying
    // the same DoConstructName. If the DoStmt of a BlockDoConstruct is not
    // identified by a DoConstructName, the corresponding EndDo must not specify
    // a DoConstructName.
    // -------------------------------------------------------------------------
    // Constraint: If the DoStmt is a NonlabelDoStmt, the corresponding EndDo
    // must be an EndDoStmt.
    // -------------------------------------------------------------------------
    // Constraint: If the DoStmt is a LabelDoStmt, the corresponding EndDo must
    // be identified with the same Label.
    // -------------------------------------------------------------------------
    std::unique_ptr<DoStmt> doStmt;
    std::unique_ptr<DoBlock> doBlock;
    std::unique_ptr<EndDo> endDo;
  };

  struct DoBody {
    std::vector<std::unique_ptr<ExecutionPartConstruct>> executionPartConstructs;
  };

  struct DoTermActionStmt {
    // Constraint: A DoTermActionStmt must not be a ContinueStmt, a GotoStmt, a
    // ReturnStmt, a StopStmt, an ExitStmt, a CycleStmt, an EndFunctionStmt, and
    // EndSubroutineStmt, and EndProgramStmt, an ArithmeticIfStmt, or an
    // AssignedGotoStmt.
    // -------------------------------------------------------------------------
    // Constraint: The DoTermActionStmt must be identified with a label and the
    // corresponding LabelDoStmt must refer to the same label.
    // -------------------------------------------------------------------------
    std::unique_ptr<ActionStmt> actionStmt;
  };

  struct ActionTermDoConstruct {
    std::unique_ptr<LabelDoStmt> labelDoStmt;
    std::unique_ptr<DoBody> doBody;
    std::unique_ptr<DoTermActionStmt> doTermActionStmt;
  };

  struct OuterSharedDoConstruct {
  };

  struct DoTermSharedStmt {
    // Constraint: A DoTermSharedStmt must not be a GotoStmt, a ReturnStmt, a
    // StopStmt, an ExitStmt, a CycleStmt, and EndFunctionStmt, an
    // EndSubroutineStmt, and EndProgramStmt, an ArithmeticIfStmt, or an
    // AssignedGotoStmt.
    // ------------------------------------------------------------------------
    // Constraint: The DoTermSharedStmt must be identified with a label and all
    // of the LabelDoStmts of the SharedTermDoConstruct must refer to the same
    // label.
    // ------------------------------------------------------------------------
    std::unique_ptr<ActionStmt> actionStmt;
  };

  struct InnerSharedDoConstruct {
    std::unique_ptr<LabelDoStmt> labelDoStmt;
    std::unique_ptr<DoBody> doBody;
    std::unique_ptr<DoTermSharedStmt> doTermSharedStmt;
  };

  enum class SharedTermDoConstructType {
    OUTER,
    INNER,
  };

  struct SharedTermDoConstruct {
    SharedTermDoConstructType type;
    union {
      std::unique_ptr<OuterSharedDoConstruct> outerSharedDoConstruct;
      std::unique_ptr<InnerSharedDoConstruct> innerSharedDoConstruct;
    };
  };

  struct OuterSharedDoConstruct {
    std::unique_ptr<LabelDoStmt> labelDoStmt;
    std::unique_ptr<DoBody> doBody;
    std::unique_ptr<SharedTermDoConstruct> sharedTermDoConstruct;
  };

  enum class NonblockDoConstructType {
    ACTION_TERM,
    OUTER_SHARED,
  };

  struct NonblockDoConstruct {
    NonblockDoConstructType type;
    union {
      std::unique_ptr<ActionTermDoConstruct> actionTermDoConstruct;
      std::unique_ptr<OuterSharedDoConstruct> outerSharedDoConstruct;
    };
  };

  enum class DoConstructType {
    BLOCK,
    NONBLOCK,
  };

  struct DoConstruct {
    DoConstructType type;
    union {
      std::unique_ptr<BlockDoConstruct> blockDoConstruct;
      std::unique_ptr<NonblockDoConstruct> nonblockDoConstruct;
    };
  };

  struct ScalarLogicalExpr {
  };

  struct IfConstructName {
    std::unique_ptr<Token> token;  // ( name )
  };

  struct IfConstructNameDefinition {
    std::unique_ptr<IfConstructName> ifConstructName;
    std::unique_ptr<Token> defineToken;  // ":"
  };

  struct IfThenStmt {
    std::optional<std::unique_ptr<IfConstructNameDefinition>> ifConstructNameDefinition;
    std::unique_ptr<Token> ifToken;  // "IF"
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<ScalarLogicalExpr> scalarLogicalExpr;
    std::unique_ptr<Token> closingParenToken;  // ")"
    std::unique_ptr<Token> thenToken;  // "THEN"
  };

  struct ElseIfStmt {
    std::unique_ptr<Token> elseToken;  // "ELSE"
    std::unique_ptr<Token> ifToken;  // "IF"
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<ScalarLogicalExpr> scalarLogicalExpr;
    std::unique_ptr<Token> closingParenToken;  // ")"
    std::unique_ptr<Token> thenToken;  // "THEN"
    std::optional<std::unique_ptr<IfConstructName>> ifConstructName;
  };

  struct ElseIfStmtAndBlock {
    std::unique_ptr<ElseIfStmt> elseIfStmt;
    std::unique_ptr<Block> block;
  };

  struct ElseStmt {
    std::unique_ptr<Token> elseToken;  // "ELSE"
    std::optional<std::unique_ptr<IfConstructName>> ifConstructName;
  };

  struct ElseStmtAndBlock {
    std::unique_ptr<ElseStmt> elseStmt;
    std::unique_ptr<Block> block;
  };

  struct EndIfStmt {
    std::unique_ptr<Token> endToken;  // "END"
    std::unique_ptr<Token> ifToken;  // "IF"
    std::optional<std::unique_ptr<IfConstructName>> ifConstructName;
  };

  struct IfConstruct {
    // Constraint: If the IfThenStmt of an IfConstruct is identified by an
    // IfConstructName, the corresponding EndIfStmt must specify the same
    // IfConstructName. If the IfThenStmt of an IfConstruct is not identified by
    // an IfConstructName, the corresponding EndIfStmt must not specify an
    // IfConstructName. If an ElseIfStmt or ElseStmt is identified by an
    // IfConstructName, the corresponding IfThenStmt must specify the same
    // IfConstructName.
    // -------------------------------------------------------------------------
    std::unique_ptr<IfThenStmt> ifThenStmt;
    std::unique_ptr<Block> block;
    std::vector<std::unique_ptr<ElseIfStmtAndBlock>> elseIfStmtsAndBlocks;
    std::optional<std::unique_ptr<ElseStmtAndBlock>> elseStmtAndBlock;
    std::unique_ptr<EndIfStmt> endIfStmt;
  };

  struct ElsewhereStmt {
    std::unique_ptr<Token> elsewhereToken;  // "ELSEWHERE"
  };

  struct EndWhereStmt {
    std::unique_ptr<Token> endToken;  // "END"
    std::unique_ptr<Token> whereToken;  // "WHERE"
  };

  struct ElsewhereStmtAndAssignmentStmts {
    std::unique_ptr<ElsewhereStmt> elsewhereStmt;
    std::vector<std::unique_ptr<AssignmentStmt>> assignmentStmts;
  };

  struct WhereConstructStmt {
    std::unique_ptr<Token> whereToken;  // "WHERE"
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<MaskExpr> maskExpr;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  struct WhereConstruct {
    // Constraint: The AssignmentStmt must not be a defined assignment.
    std::unique_ptr<WhereConstructStmt> whereConstructStmt;
    std::vector<std::unique_ptr<AssignmentStmt>> assignmentStmts;
    std::optional<std::unique_ptr<ElsewhereStmtAndAssignmentStmts>> elsewhereStmtAndAssignmentStmts;
    std::unique_ptr<EndWhereStmt> endWhereStmt;
  };

  enum class ExecutableConstructType {
    ACTION_STMT,
    CASE_CONSTRUCT,
    DO_CONSTRUCT,
    IF_CONSTRUCT,
    WHERE_CONSTRUCT,
  };

  struct ExecutableConstruct {
    ExecutableConstructType type;
    union {
      std::unique_ptr<ActionStmt> actionStmt;
      std::unique_ptr<CaseConstruct> caseConstruct;
      std::unique_ptr<DoConstruct> doConstruct;
      std::unique_ptr<IfConstruct> ifConstruct;
      std::unique_ptr<WhereConstruct> whereConstruct;
    };
  };

  enum class ExecutionPartConstructType {
    EXECUTABLE_CONSTRUCT,
    FORMAT_STMT,
    DATA_STMT,
    ENTRY_STMT,
  };

  struct ExecutionPartConstruct {
    ExecutionPartConstructType executionPartConstructType;
    union {
      std::unique_ptr<ExecutableConstruct> executableConstruct;
      std::unique_ptr<FormatStmt> formatStmt;
      std::unique_ptr<DataStmt> dataStmt;
      std::unique_ptr<EntryStmt> entryStmt;
    };
  };

  struct ExecutionPart {
    // Constraint: An ExecutionPart must not contain an EndFunctionStmt,
    // EndProgramStmt, or EndSubroutineStmt.
    // -----------------------------------------------------------------
    std::unique_ptr<ExecutableConstruct> executableConstruct;
    std::vector<std::unique_ptr<ExecutionPartConstruct>> executionPartConstructs;
  };

  struct ContainsStmt {
  };

  enum class InternalSubprogramType {
    FUNCTION_SUBPROGRAM,
    SUBROUTINE_SUBPROGRAM,
  };

  struct InternalSubprogram {
    InternalSubprogramType type;
    union {
      std::unique_ptr<FunctionSubprogram> functionSubprogram;
      std::unique_ptr<SubroutineSubprogram> subroutineSubprogram;
    };
  };

  struct InternalSubprogramPart {
    std::unique_ptr<ContainsStmt> containsStmt;
    std::unique_ptr<InternalSubprogram> firstInternalSubprogram;
    std::vector<std::unique_ptr<InternalSubprogram>> restInternalSubprograms;
  };

  struct FunctionStmt {
  };

  struct FunctionSubprogram {
    std::unique_ptr<FunctionStmt> functionStmt;
    std::optional<std::unique_ptr<SpecificationPart>> specificationPart;
    std::optional<std::unique_ptr<ExecutionPart>> executionPart;
    std::optional<std::unique_ptr<InternalSubprogramPart>> internalSubprogramPart;
    std::unique_ptr<EndFunctionStmt> endFunctionStmt;
  };

  struct SubroutineStmt {
  };

  struct SubroutineSubprogram {
    std::unique_ptr<SubroutineStmt> subroutineStmt;
    std::optional<std::unique_ptr<SpecificationPart>> specificationPart;
    std::optional<std::unique_ptr<ExecutionPart>> executionPart;
    std::optional<std::unique_ptr<InternalSubprogramPart>> internalSubprogramPart;
    std::unique_ptr<EndSubroutineStmt> endSubroutineStmt;
  };

  struct ProgramStmt {
  };

  struct MainProgram {
    std::optional<std::unique_ptr<ProgramStmt>> programStmt;
    std::optional<std::unique_ptr<SpecificationPart>> specificationPart;
    std::optional<std::unique_ptr<ExecutionPart>> executionPart;
    std::optional<std::unique_ptr<InternalSubprogramPart>> internalSubprogramPart;
    std::unique_ptr<EndProgramStmt> endProgramStmt;
  };

  enum class ExternalSubprogramType {
    FUNCTION_SUBPROGRAM,
    SUBROUTINE_SUBPROGRAM,
  };

  struct ExternalSubprogram {
    ExternalSubprogramType type;
    union {
      std::unique_ptr<FunctionSubprogram> functionSubprogram;
      std::unique_ptr<SubroutineSubprogram> subroutineSubprogram;
    };
  };

  enum class ModuleSubprogramType {
    FUNCTION_SUBPROGRAM,
    SUBROUTINE_SUBPROGRAM,
  };

  struct ModuleSubprogram {
    ModuleSubprogramType type;
    union {
      std::unique_ptr<FunctionSubprogram> functionSubprogram;
      std::unique_ptr<SubroutineSubprogram> subroutineSubprogram;
    };
  };

  struct ModuleSubprogramPart {
    std::unique_ptr<ContainsStmt> containsStmt;
    std::unique_ptr<ModuleSubprogram> firstModuleSubprogram;
    std::vector<std::unique_ptr<ModuleSubprogram>> restModuleSubprograms;
  };

  struct ModuleStmt {
  };

  struct EndModuleStmt {
  };

  struct Module {
    std::unique_ptr<ModuleStmt> moduleStmt;
    std::optional<std::unique_ptr<SpecificationPart>> specificationPart;
    std::optional<std::unique_ptr<ModuleSubprogramPart>> moduleSubprogramPart;
    std::unique_ptr<EndModuleStmt> endModuleStmt;
  };

  struct BlockDataStmt {
  };

  struct EndBlockDataStmt {
  };

  struct BlockData {
    std::unique_ptr<BlockDataStmt> blockDataStmt;
    std::optional<std::unique_ptr<SpecificationPart>> specificationPart;
    std::unique_ptr<EndBlockDataStmt> endBlockDataStmt;
  };

  enum class ProgramUnitType {
    MAIN_PROGRAM,
    EXTERNAL_SUBPROGRAM,
    MODULE,
    BLOCK_DATA,
  };

  struct ProgramUnit {
    ProgramUnitType type;
    union {
      std::unique_ptr<MainProgram> mainProgram;
      std::unique_ptr<ExternalSubprogram> externalSubprogram;
      std::unique_ptr<Module> module;
      std::unique_ptr<BlockData> blockData;
    };
  };

  struct ExecutableProgram {
    // TODO: break into first and rest
    std::vector<std::unique_ptr<ProgramUnit>> programUnits;
  };

  enum class LiteralConstantType {
    INT_LITERAL_CONSTANT,
    REAL_LITERAL_CONSTANT,
    COMPLEX_LITERAL_CONSTANT,
    LOGICAL_LITERAL_CONSTANT,
    CHAR_LITERAL_CONSTANT,
    BOZ_LITERAL_CONSTANT,
  };

  struct LiteralConstant {
    LiteralConstantType type;
    union {
      std::unique_ptr<IntLiteralConstant> intLiteralConstant;
      std::unique_ptr<RealLiteralConstant> realLiteralConstant;
      std::unique_ptr<ComplexLiteralConstant> complexLiteralConstant;
      std::unique_ptr<LogicalLiteralConstant> logicalLiteralConstant;
      std::unique_ptr<CharLiteralConstant> charLiteralConstant;
      std::unique_ptr<BozLiteralConstant> bozLiteralConstant;
    };
  };

  enum class ConstantType {
    LITERAL_CONSTANT,
    NAMED_CONSTANT,
  };

  struct Constant {
    ConstantType type;
    union {
      std::unique_ptr<LiteralConstant> literalConstant;
      std::unique_ptr<NamedConstant> namedConstant;
    };
  };

  struct IntConstant {
    // Constraint: IntConstant must be of type integer.
    std::unique_ptr<Constant> constant;
  };

  struct CharConstant {
    // Constraint: CharConstant must be of type character.
    std::unique_ptr<Constant> constant;
  };

  struct PowerOp {
    std::unique_ptr<Token> token;  // "**"
  };

  struct MultOp {
    std::unique_ptr<Token> token;  // ( "*" | "/" )
  };

  struct AddOp {
    std::unique_ptr<Token> token;  // ( "+" | "-" )
  };

  struct ConcatOp {
    std::unique_ptr<Token> token;  // "//"
  };

  struct RelOp {
    // ( ".EQ." | ".NE." | ".LT." | ".LE." | ".GT." | ".GE." | "==" | "/=" | "<" | "<=" | ">" | ">" )
    std::unique_ptr<Token> token;
  };

  struct NotOp {
    std::unique_ptr<Token> token;  // ".NOT."
  };

  struct AndOp {
    std::unique_ptr<Token> token;  // ".AND."
  };

  struct OrOp {
    std::unique_ptr<Token> token;  // ".OR."
  };

  struct EquivOp {
    std::unique_ptr<Token> token;  // ( ".EQV." | ".NEQV." )
  };

  enum class IntrinsicOperatorType {
    POWER_OP,
    MULT_OP,
    ADD_OP,
    CONCAT_OP,
    REL_OP,
    NOT_OP,
    AND_OP,
    OR_OP,
    EQUIV_OP,
  };

  struct IntrinsicOperator {
    IntrinsicOperatorType type;
    union {
      std::unique_ptr<PowerOp> powerOp;
      std::unique_ptr<MultOp> multOp;
      std::unique_ptr<AddOp> addOp;
      std::unique_ptr<ConcatOp> concatOp;
      std::unique_ptr<RelOp> relOp;
      std::unique_ptr<NotOp> notOp;
      std::unique_ptr<AndOp> andOp;
      std::unique_ptr<OrOp> orOp;
      std::unique_ptr<EquivOp> equivOp;
    };
  };

  struct DefinedUnaryOp {
    // Constraint: A DefinedUnaryOp must not contain more than 31 letters and
    // must not be the same as any intrinsicOperator or LogicalLiteralConstant.
    // ------------------------------------------------------------------------
    std::unique_ptr<Token> openingDotToken;  // "."
    std::unique_ptr<Token> nameToken;  // ( name )
    std::unique_ptr<Token> closingDotToken;  // "."
  };

  struct DefinedBinaryOp {
    // Constraint: A DefinedBinaryOp must not contain more than 31 letters and
    // must not be the same as any IntrinsicOperator or LogicalLiteralConstant.
    // ------------------------------------------------------------------------
    std::unique_ptr<Token> openingDotToken;  // "."
    std::unique_ptr<Token> nameToken;  // ( name )
    std::unique_ptr<Token> closingDotToken;  // "."
  };

  struct ExtendedIntrinsicOp {
    IntrinsicOperator intrinsicOperator;
  };

  enum class DefinedOperatorType {
    DEFINED_UNARY_OP,
    DEFINED_BINARY_OP,
    EXTENDED_INTRINSIC_OP,
  };

  struct DefinedOperator {
    DefinedOperatorType type;
    union {
      std::unique_ptr<DefinedUnaryOp> definedUnaryOp;
      std::unique_ptr<DefinedBinaryOp> definedBinaryOp;
      std::unique_ptr<ExtendedIntrinsicOp> extendedIntrinsicOp;
    };
  };

  struct ConstantSubobject {
    // Constraint: Subobject must be a subobject designator whose parent is a
    // constant.
    // ----------------------------------------------------------------------
    std::unique_ptr<Subobject> subobject;
  };

  struct FunctionReference {
  };

  struct ParenthesizedExpr {
    std::unique_ptr<Token> openingParenToken;  // "("
    std::unique_ptr<Expr> expr;
    std::unique_ptr<Token> closingParenToken;  // ")"
  };

  enum class PrimaryType {
    CONSTANT,
    CONSTANT_SUBOBJECT,
    VARIABLE,
    ARRAY_CONSTRUCTOR,
    STRUCTURE_CONSTRUCTOR,
    FUNCTION_REFERENCE,
    EXPR,
  };

  struct Primary {
    // Constraint: A Variable that is a Primary must not be an assumed-size
    // array.
    // --------------------------------------------------------------------
    PrimaryType type;
    union {
      std::unique_ptr<Constant> constant;
      std::unique_ptr<ConstantSubobject> constantSubobject;
      std::unique_ptr<Variable> variable;
      std::unique_ptr<ArrayConstructor> arrayConstructor;
      std::unique_ptr<StructureConstructor> structureConstructor;
      std::unique_ptr<FunctionReference> functionReference;
      std::unique_ptr<ParenthesizedExpr> expr;
    };
  };

  struct Level1Expr {
    std::optional<std::unique_ptr<DefinedUnaryOp>> definedUnaryOp;
    std::unique_ptr<Primary> primary;
  };

  struct PowerOpAndMultOperand {
    std::unique_ptr<PowerOp> powerOp;
    std::unique_ptr<MultOperand> multOperand;
  };

  struct MultOperand {
    std::unique_ptr<Level1Expr> level1Expr;
    std::optional<std::unique_ptr<PowerOpAndMultOperand>> powerOpAndMultOperand;
  };

  struct AddOperandAndMultOp {
    std::unique_ptr<AddOperand> addOperand;
    std::unique_ptr<MultOp> multOp;
  };

  struct AddOperand {
    std::optional<std::unique_ptr<AddOperandAndMultOp>> addOperandAndMultOp;
    std::unique_ptr<MultOperand> multOperand;
  };

  struct Level2ExprAndAddOp {
    std::optional<std::unique_ptr<Level2Expr>> level2Expr;
    std::unique_ptr<AddOp> addOp;
  };

  struct Level2Expr {
    std::optional<std::unique_ptr<Level2ExprAndAddOp>> level2ExprAndAddOp;
    std::unique_ptr<AddOperand> addOperand;
  };

  struct Level3ExprAndConcatOp {
    std::unique_ptr<Level3Expr> level3Expr;
    std::unique_ptr<ConcatOp> concatOp;
  };

  struct Level3Expr {
    std::optional<std::unique_ptr<Level3ExprAndConcatOp>> level3ExprAndConcatOp;
    std::unique_ptr<Level2Expr> level2Expr;
  };

  struct Level3ExprAndRelOp {
    std::unique_ptr<Level3Expr> level3Expr;
    std::unique_ptr<RelOp> relOp;
  };

  struct Level4Expr {
    std::optional<std::unique_ptr<Level3ExprAndRelOp>> level3ExprAndRelOp;
    std::unique_ptr<Level3Expr> level3Expr;
  };

  struct AndOperand {
    std::optional<std::unique_ptr<NotOp>> notOp;
    std::unique_ptr<Level4Expr> level4Expr;
  };

  struct OrOperandAndAndOp {
    std::unique_ptr<OrOperand> orOperand;
    std::unique_ptr<AndOp> andOp;
  };

  struct OrOperand {
    std::optional<std::unique_ptr<OrOperandAndAndOp>> orOperandAndAndOp;
    std::unique_ptr<AndOperand> andOperand;
  };

  struct EquivOperandAndOrOp {
    std::unique_ptr<EquivOperand> equivOperand;
    std::unique_ptr<OrOp> orOp;
  };

  struct EquivOperand {
    std::optional<std::unique_ptr<EquivOperandAndOrOp>> equivOperandAndOrOp;
    std::unique_ptr<OrOperand> orOperand;
  };

  struct Level5ExprAndEquivOp {
    std::unique_ptr<Level5Expr> level5Expr;
    std::unique_ptr<EquivOp> equivOp;
  };

  struct Level5Expr {
    std::optional<std::unique_ptr<Level5ExprAndEquivOp>> level5ExprAndEquivOp;
    std::unique_ptr<EquivOperand> equivOperand;
  };

  struct LogicalExpr {
    // Constraint: LogicalExpr must be of type logical.
    // ------------------------------------------------
    std::unique_ptr<Expr> expr;
  };

  struct CharExpr {
    // Constraint: CharExpr must be of type character.
    // -----------------------------------------------
    std::unique_ptr<Expr> expr;
  };

  struct DefaultCharExpr {
    // Constraint: DefaultCharExpr must be of type default character.
    // --------------------------------------------------------------
    std::unique_ptr<Expr> expr;
  };

  struct IntExpr {
    // Constraint: IntExpr must be of type integer.
    // --------------------------------------------
    std::unique_ptr<Expr> expr;
  };

  struct NumericExpr {
    // Constraint: NumericExpr must be of type integer, real, or complex.
    // ------------------------------------------------------------------
    std::unique_ptr<Expr> expr;
  };

  struct InitializationExpr {
    // Constraint: InitializationExpr must be an initialization expression.
    // --------------------------------------------------------------------
    std::unique_ptr<Expr> expr;
  };

  struct CharInitializationExpr {
    // Constraint: A CharInitializationExpr must be an initialization
    // expression.
    // ------------------------------------------------------------------------
    std::unique_ptr<CharExpr> charExpr;
  };

  struct IntInitializationExpr {
    // Constraint: An IntInitializationExpr must be an initialization
    // expression.
    // -----------------------------------------------------------------------
    std::unique_ptr<IntExpr> intExpr;
  };

  struct LogicalInitializationExpr {
    // Constraint: A LogicalInitializationExpr must be an initialization
    // expression.
    // -----------------------------------------------------------------
    std::unique_ptr<LogicalExpr> logicalExpr;
  };

  struct IntitializationExpr {
    // Constraint: An InitializationExpr must be an initialization expression.
    // -----------------------------------------------------------------------
    std::unique_ptr<Expr> expr;
  };

  struct SpecificationExpr {
    // Constraint: The ScalarIntExpr must be a restricted expression.
    // --------------------------------------------------------------
    std::unique_ptr<ScalarIntExpr> scalarIntExpr;
  };

  struct Block {
    std::vector<std::unique_ptr<ExecutionPartConstruct>> executionPartConstructs;
  };

  struct ExternalFileUnit {
    std::unique_ptr<ScalarIntExpr> scalarIntExpr;
  };

  struct InternalFileUnit {
    // Constraint: The DefaultCharVariable must not be an array section with a
    // vector subscript.
    std::unique_ptr<DefaultCharVariable> defaultCharVariable;
  };

  enum class IoUnitType {
    EXTERNAL,
    STAR,
    INTERNAL,
  };

  struct IoUnit {
    IoUnitType type;
    union {
      std::unique_ptr<ExternalFileUnit> externalFileUnit;
      std::unique_ptr<Token> starToken;  // "*"
      std::unique_ptr<InternalFileUnit> internalFileUnit;
    };
  };

  class Parser {
  };

} // namespace LCompilers::LanguageServiceProvider::AST

#endif // LCOMPILERS_LSP_AST_PARSER_H
