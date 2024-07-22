#pragma once

#include "StatementSyntax.h"

#include <memory>

namespace Mamba
{
    class IfStatementSyntax : public StatementSyntax
    {
    public:
        using Super = StatementSyntax;

        [[nodiscard]] IfStatementSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                        const std::shared_ptr<const class SyntaxToken> IfKeyword,
                                        const NullableSharedPtr<const class SyntaxToken> OpenParenthesisToken,
                                        const std::shared_ptr<const class ExpressionSyntax> Condition,
                                        const NullableSharedPtr<const class SyntaxToken> CloseParenthesisToken,
                                        const std::shared_ptr<const class StatementSyntax> ThenStatement,
                                        const NullableSharedPtr<const class ElseClauseSyntax> ElseClause) noexcept;

        SyntaxKind Kind() const noexcept override;
        std::vector<std::shared_ptr<const class SyntaxNode>> Children() const noexcept override;

        const std::shared_ptr<const class SyntaxToken> IfKeyword;
        const NullableSharedPtr<const class SyntaxToken> OpenParenthesisToken;
        const std::shared_ptr<const class ExpressionSyntax> Condition;
        const NullableSharedPtr<const class SyntaxToken> CloseParenthesisToken;
        const std::shared_ptr<const class StatementSyntax> ThenStatement;
        const NullableSharedPtr<const class ElseClauseSyntax> ElseClause;
    };
} // namespace Mamba