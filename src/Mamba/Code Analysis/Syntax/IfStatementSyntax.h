#pragma once

#include "ElseClauseSyntax.h"
#include "ExpressionSyntax.h"
#include "StatementSyntax.h"
#include "SyntaxToken.h"

namespace Mamba
{
    class IfStatementSyntax : public StatementSyntax
    {
    public:
        using Super = StatementSyntax;

        [[nodiscard]] IfStatementSyntax(
            const class SyntaxTree* SyntaxTree,
            const SyntaxToken* IfKeyword,
            const ExpressionSyntax* Condition,
            const StatementSyntax* ThenStatement,
            NullablePointer<const ElseClauseSyntax> ElseClause
        ) noexcept;

        ~IfStatementSyntax() noexcept override;

        SyntaxKind Kind() const noexcept override;

        const SyntaxToken* IfKeyword;
        const ExpressionSyntax* Condition;
        const StatementSyntax* ThenStatement;
        NullablePointer<const ElseClauseSyntax> ElseClause;

    private:
        std::size_t ChildrenCount() const noexcept override;
        const SyntaxNode* ChildAt(std::size_t Index) const noexcept override;
    };
} // namespace Mamba