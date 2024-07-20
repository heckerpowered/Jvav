#pragma once

#include <memory>
#include <vector>

#include "DiagnosticBag.h"
#include "SeperatedSyntaxList.h"

namespace Mamba
{
    class Parser
    {
        const std::shared_ptr<const class SyntaxTree> SyntaxTree;
        const std::shared_ptr<const class SourceText> Text;
        std::vector<std::shared_ptr<const class SyntaxToken>> Tokens;
        std::size_t Position;

    public:
        DiagnosticBag Diagnostics;

        [[nodiscard]] Parser(const std::shared_ptr<const class SyntaxTree> SyntaxTree) noexcept;

    private:
        [[nodiscard]] std::shared_ptr<const class SyntaxToken> Peek(const std::size_t Offset) noexcept;
        [[nodiscard]] std::shared_ptr<const class SyntaxToken> Current() noexcept;
        std::shared_ptr<const class SyntaxToken> NextToken() noexcept;
        [[nodiscard]] std::shared_ptr<const class SyntaxToken> MatchToken(const SyntaxKind Kind) noexcept;

    public:
        [[nodiscard]] std::shared_ptr<const class CompilationUnitSyntax> ParseCompilationUnit() noexcept;

    private:
        [[nodiscard]] std::vector<std::shared_ptr<const class MemberSyntax>> ParseMembers() noexcept;
        [[nodiscard]] std::shared_ptr<const class MemberSyntax> ParseMember() noexcept;
        [[nodiscard]] std::shared_ptr<const class FunctionDeclarationSyntax> ParseFunctionDeclaration() noexcept;
        [[nodiscard]] std::shared_ptr<const SeperatedSyntaxList<std::shared_ptr<const class SyntaxNode>>>
            ParseParameterList() noexcept;
        [[nodiscard]] std::shared_ptr<const class ParameterSyntax> ParseParameter() noexcept;
        [[nodiscard]] std::shared_ptr<const class MemberSyntax> ParseGlobalStatement() noexcept;
        [[nodiscard]] std::shared_ptr<const class StatementSyntax> ParseStatement() noexcept;
        [[nodiscard]] std::shared_ptr<const class BlockStatementSyntax> ParseBlockStatement() noexcept;
        [[nodiscard]] std::shared_ptr<const class StatementSyntax> ParseVariableDeclaration() noexcept;
        [[nodiscard]] NullableSharedPtr<const class TypeClauseSyntax> ParseOptionalTypeClause() noexcept;
        [[nodiscard]] std::shared_ptr<const class TypeClauseSyntax> ParseTypeClause() noexcept;
        [[nodiscard]] std::shared_ptr<const class StatementSyntax> ParseIfStatement() noexcept;
        [[nodiscard]] NullableSharedPtr<const class ElseClauseSyntax> ParseOptionalElseClause() noexcept;
        [[nodiscard]] std::shared_ptr<const class StatementSyntax> ParseWhileStatement() noexcept;
        [[nodiscard]] std::shared_ptr<const class StatementSyntax> ParseDoWhileStatement() noexcept;
        [[nodiscard]] std::shared_ptr<const class StatementSyntax> ParseForStatement() noexcept;
        [[nodiscard]] std::shared_ptr<const class StatementSyntax> ParseBreakStatement() noexcept;
        [[nodiscard]] std::shared_ptr<const class StatementSyntax> ParseContinueStatement() noexcept;
        [[nodiscard]] std::shared_ptr<const class StatementSyntax> ParseReturnStatement() noexcept;
        [[nodiscard]] std::shared_ptr<const class ExpressionStatementSyntax> ParseExpressionStatement() noexcept;
        [[nodiscard]] std::shared_ptr<const class ExpressionSyntax> ParseExpression() noexcept;
        [[nodiscard]] std::shared_ptr<const class ExpressionSyntax> ParseAssignmentExpression() noexcept;
        [[nodiscard]] std::shared_ptr<const class ExpressionSyntax>
            ParseBinaryExpression(const std::size_t ParentPrecedence = 0) noexcept;
        [[nodiscard]] std::shared_ptr<const class ExpressionSyntax> ParsePrimaryExpression() noexcept;
        [[nodiscard]] std::shared_ptr<const class ExpressionSyntax> ParseParenthesizedExpression() noexcept;
        [[nodiscard]] std::shared_ptr<const class ExpressionSyntax> ParseBooleanLiteral() noexcept;
        [[nodiscard]] std::shared_ptr<const class ExpressionSyntax> ParseNumericLiteral() noexcept;
        [[nodiscard]] std::shared_ptr<const class ExpressionSyntax> ParseStringLiteral() noexcept;
        [[nodiscard]] std::shared_ptr<const class ExpressionSyntax> ParseNameOrCallExpression() noexcept;
        [[nodiscard]] std::shared_ptr<const class ExpressionSyntax> ParseCallExpression() noexcept;
        [[nodiscard]] std::shared_ptr<const SeperatedSyntaxList<std::shared_ptr<const class SyntaxNode>>>
            ParseArguments() noexcept;
        [[nodiscard]] std::shared_ptr<const class ExpressionSyntax> ParseNameExpression() noexcept;
    };

} // namespace Mamba