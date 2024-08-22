#pragma once

#include "ExpressionSyntax.h"
#include "StatementSyntax.h"
#include "SyntaxToken.h"

namespace Mamba
{
    class WhileStatementSyntax : public StatementSyntax
    {
    public:
        using Super = StatementSyntax;

        [[nodiscard]] WhileStatementSyntax(
            const class SyntaxTree* SyntaxTree,
            const SyntaxToken* WhileKeyword,
            const ExpressionSyntax* Condition,
            const StatementSyntax* Body
        ) noexcept;

        ~WhileStatementSyntax() noexcept override;

        SyntaxKind Kind() const noexcept override;
        std::vector<const SyntaxNode*> Children() const noexcept override;

        const SyntaxToken* WhileKeyword;
        const ExpressionSyntax* Condition;
        const StatementSyntax* Body;
    };
} // namespace Mamba