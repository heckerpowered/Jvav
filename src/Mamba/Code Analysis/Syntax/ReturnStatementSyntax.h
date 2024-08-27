#pragma once

#include "ExpressionSyntax.h"
#include "StatementSyntax.h"
#include "SyntaxToken.h"

namespace Mamba
{
    class ReturnStatementSyntax : public StatementSyntax
    {
    public:
        using Super = StatementSyntax;

        [[nodiscard]] ReturnStatementSyntax(
            const class SyntaxTree* SyntaxTree,
            const SyntaxToken* ReturnKeyword,
            NullablePointer<const ExpressionSyntax> Expression
        ) noexcept;

        ~ReturnStatementSyntax() noexcept override;

        SyntaxKind Kind() const noexcept override;

        const SyntaxToken* ReturnKeyword;
        NullablePointer<const ExpressionSyntax> Expression;

    private:
        std::size_t ChildrenCount() const noexcept override;
        const SyntaxNode* ChildAt(std::size_t Index) const noexcept override;
    };
} // namespace Mamba