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

        const SyntaxToken* WhileKeyword;
        const ExpressionSyntax* Condition;
        const StatementSyntax* Body;

    private:
        std::size_t ChildrenCount() const noexcept override;
        const SyntaxNode* ChildAt(std::size_t Index) const noexcept override;
    };
} // namespace Mamba