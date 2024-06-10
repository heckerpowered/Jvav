#include "MambaCore.h"

MAMBA_NAMESPACE_BEGIN

enum class SyntaxKind
{
    BadToken,

    // Tokens
    EndOfFileToken,
    NumberToken,
    StringToken,
    PlusToken,
    PlusEqualsToken,
    MinusToken,
    MinusEqualsToken,
    StarToken,
    StarEqualsToken,
    SlashToken,
    SlashEqualsToken,
    BangToken,
    EqualsToken,
    TildeToken,
    HatToken,
    HatEqualsToken,
    AmpersandToken,
    AmpersandAmpersandToken,
    AmpersandEqualsToken,
    PipeToken,
    PipeEqualsToken,
    PipePipeToken,
    EqualsEqualsToken,
    BangEqualsToken,
    LessToken,
    LessOrEqualsToken,
    GreaterToken,
    GreaterOrEqualsToken,
    OpenParenthesisToken,
    CloseParenthesisToken,
    OpenBraceToken,
    CloseBraceToken,
    ColonToken,
    CommaToken,
    IdentifierToken,

    // Keywords
    BreakKeyword,
    ContinueKeyword,
    ElseKeyword,
    FalseKeyword,
    ForKeyword,
    FunctionKeyword,
    IfKeyword,
    LetKeyword,
    VarKeyword,
    ReturnKeyword,
    ToKeyword,
    TrueKeyword,
    WhileKeyword,
    DoKeyword,

    CompilationUnit
};

MAMBA_NAMESPACE_END