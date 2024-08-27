#pragma once

#include "ExpressionSyntax.h"
#include "StatementSyntax.h"
#include "SyntaxKind.h"
#include "SyntaxToken.h"

namespace Mamba
{
    class DoWhileStatementSyntax : public StatementSyntax
    {
    public:
        using Super = StatementSyntax;

        [[nodiscard]] DoWhileStatementSyntax(
            const class SyntaxTree* SyntaxTree,
            const SyntaxToken* DoKeyword,
            const StatementSyntax* Body,
            const SyntaxToken* WhileKeyword,
            const ExpressionSyntax* Condition
        ) noexcept;

        ~DoWhileStatementSyntax() noexcept override;

        SyntaxKind Kind() const noexcept override;

        const SyntaxToken* DoKeyword;
        const StatementSyntax* Body;
        const SyntaxToken* WhileKeyword;
        const ExpressionSyntax* Condition;

    private:
        std::size_t ChildrenCount() const noexcept override;
        const SyntaxNode* ChildAt(std::size_t Index) const noexcept override;
    };
} // namespace Mamba