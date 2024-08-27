#pragma once

#include "ExpressionSyntax.h"
#include "SyntaxToken.h"
#include "SyntaxTree.h"

namespace Mamba
{
    class UnaryExpressionSyntax : public ExpressionSyntax
    {
    public:
        using Super = ExpressionSyntax;

        [[nodiscard]] UnaryExpressionSyntax(
            const class SyntaxTree* SyntaxTree,
            const SyntaxToken* OperatorToken,
            const ExpressionSyntax* Operand
        ) noexcept;

        ~UnaryExpressionSyntax() noexcept override;

        SyntaxKind Kind() const noexcept override;

        const SyntaxToken* OperatorToken;
        const ExpressionSyntax* Operand;

    private:
        std::size_t ChildrenCount() const noexcept override;
        const SyntaxNode* ChildAt(std::size_t Index) const noexcept override;
    };
} // namespace Mamba