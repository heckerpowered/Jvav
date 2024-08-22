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
        std::vector<const SyntaxNode*> Children() const noexcept override;

        const SyntaxToken* DoKeyword;
        const StatementSyntax* Body;
        const SyntaxToken* WhileKeyword;
        const ExpressionSyntax* Condition;
    };
} // namespace Mamba