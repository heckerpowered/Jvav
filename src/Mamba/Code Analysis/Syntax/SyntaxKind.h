#pragma once

namespace Mamba
{
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
        WhitespaceToken,

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

        // Nodes
        CompilationUnit,
        FunctionDeclaration,
        GlobalStatement,
        Parameter,
        TypeClause,
        ElseClause,

        // Statements
        BlockStatement,
        VariableDeclaration,
        IfStatement,
        WhileStatement,
        DoWhileStatement,
        ForStatement,
        ReturnStatement,
        ExpressionStatement,
        ContinueStatement,
        BreakStatement,

        // Expressions
        LiteralExpression,
        NameExpression,
        UnaryExpression,
        BinaryExpression,
        CompoundAssignmentExpression,
        ParenthesizedExpression,
        AssignmentExpression,
        CallExpression,
    };
} // namespace Mamba