#pragma once

#include "ExpressionSyntax.h"
#include "SyntaxKind.h"
#include "SyntaxToken.h"

namespace Mamba
{
    class BinaryExpressionSyntax : public ExpressionSyntax
    {
    public:
        using Super = ExpressionSyntax;

        [[nodiscard]] BinaryExpressionSyntax(
            const class SyntaxTree* SyntaxTree,
            const ExpressionSyntax* Left,
            const SyntaxToken* OperatorToken,
            const ExpressionSyntax* Right
        ) noexcept;

        ~BinaryExpressionSyntax() noexcept override;

        SyntaxKind Kind() const noexcept override;

        const ExpressionSyntax* Left;
        const SyntaxToken* OperatorToken;
        const ExpressionSyntax* Right;

    private:
        std::size_t ChildrenCount() const noexcept override;
        const SyntaxNode* ChildAt(std::size_t Index) const noexcept override;
    };
} // namespace Mamba
