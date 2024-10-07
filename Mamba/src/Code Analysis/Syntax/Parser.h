#pragma once

#include <vector>

#include "BreakStatementSyntax.h"
#include "CallExpressionSyntax.h"
#include "CompilationUnitSyntax.h"
#include "ContinueStatementSyntax.h"
#include "DiagnosticBag.h"
#include "DoWhileStatementSyntax.h"
#include "ElseClauseSyntax.h"
#include "ExpressionStatementSyntax.h"
#include "ForStatementSyntax.h"
#include "FunctionDeclarationSyntax.h"
#include "IfStatementSyntax.h"
#include "LiteralExpressionSyntax.h"
#include "NameExpressionSyntax.h"
#include "ParameterSyntax.h"
#include "ParenthesizedExpressionSyntax.h"
#include "ReturnStatementSyntax.h"
#include "SeperatedSyntaxList.h"
#include "SyntaxToken.h"
#include "VariableDeclarationSyntax.h"
#include "WhileStatementSyntax.h"

namespace Mamba
{
    class Parser
    {
        const class SyntaxTree* SyntaxTree;
        std::vector<SyntaxToken> Tokens;
        std::size_t Position;

    public:
        DiagnosticBag Diagnostics;

        [[nodiscard]] Parser(const class SyntaxTree* SyntaxTree, std::vector<SyntaxToken>&& Tokens) noexcept;
        Parser(const Parser&) = delete;
        Parser(Parser&& Other) noexcept;

    private:
        [[nodiscard]] const SyntaxToken* Peek(std::size_t Offset) noexcept;
        [[nodiscard]] const SyntaxToken* Current() noexcept;
        const SyntaxToken* NextToken() noexcept;
        [[nodiscard]] const SyntaxToken* MatchToken(SyntaxKind Kind) noexcept;

    public:
        [[nodiscard]] CompilationUnitSyntax* ParseCompilationUnit() noexcept;

    private:
        [[nodiscard]] ExpressionSyntax* ParseUnaryOrPrimaryExpression(std::size_t ParentPrecedence = 0) noexcept;
        [[nodiscard]] ExpressionSyntax* ParseBinaryExpression(std::size_t ParentPrecedence = 0) noexcept;
        [[nodiscard]] ParenthesizedExpressionSyntax* ParseParenthesizedExpression() noexcept;
        [[nodiscard]] SeperatedSyntaxList<ParameterSyntax*> ParseParameterList() noexcept;
        [[nodiscard]] NullablePointer<TypeClauseSyntax> ParseOptionalTypeClause() noexcept;
        [[nodiscard]] NullablePointer<ElseClauseSyntax> ParseOptionalElseClause() noexcept;
        [[nodiscard]] SeperatedSyntaxList<ExpressionSyntax*> ParseArguments() noexcept;
        [[nodiscard]] FunctionDeclarationSyntax* ParseFunctionDeclaration() noexcept;
        [[nodiscard]] VariableDeclarationSyntax* ParseVariableDeclaration() noexcept;
        [[nodiscard]] ExpressionStatementSyntax* ParseExpressionStatement() noexcept;
        [[nodiscard]] ContinueStatementSyntax* ParseContinueStatement() noexcept;
        [[nodiscard]] DoWhileStatementSyntax* ParseDoWhileStatement() noexcept;
        [[nodiscard]] LiteralExpressionSyntax* ParseNumericLiteral() noexcept;
        [[nodiscard]] LiteralExpressionSyntax* ParseBooleanLiteral() noexcept;
        [[nodiscard]] ExpressionSyntax* ParseAssignmentExpression() noexcept;
        [[nodiscard]] ExpressionSyntax* ParseNameOrCallExpression() noexcept;
        [[nodiscard]] LiteralExpressionSyntax* ParseStringLiteral() noexcept;
        [[nodiscard]] ReturnStatementSyntax* ParseReturnStatement() noexcept;
        [[nodiscard]] CallExpressionSyntax* ParseCallExpression() noexcept;
        [[nodiscard]] BlockStatementSyntax* ParseBlockStatement() noexcept;
        [[nodiscard]] WhileStatementSyntax* ParseWhileStatement() noexcept;
        [[nodiscard]] BreakStatementSyntax* ParseBreakStatement() noexcept;
        [[nodiscard]] NameExpressionSyntax* ParseNameExpression() noexcept;
        [[nodiscard]] ExpressionSyntax* ParsePrimaryExpression() noexcept;
        [[nodiscard]] std::vector<MemberSyntax*> ParseMembers() noexcept;
        [[nodiscard]] ForStatementSyntax* ParseForStatement() noexcept;
        [[nodiscard]] IfStatementSyntax* ParseIfStatement() noexcept;
        [[nodiscard]] MemberSyntax* ParseGlobalStatement() noexcept;
        [[nodiscard]] ExpressionSyntax* ParseExpression() noexcept;
        [[nodiscard]] TypeClauseSyntax* ParseTypeClause() noexcept;
        [[nodiscard]] ParameterSyntax* ParseParameter() noexcept;
        [[nodiscard]] StatementSyntax* ParseStatement() noexcept;
        [[nodiscard]] MemberSyntax* ParseMember() noexcept;
    };

} // namespace Mamba