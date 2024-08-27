#pragma once

#include "ExpressionSyntax.h"
#include "SyntaxToken.h"

namespace Mamba
{
    class LiteralExpressionSyntax : public ExpressionSyntax
    {
    public:
        using Super = ExpressionSyntax;

        [[nodiscard]] LiteralExpressionSyntax(const class SyntaxTree* SyntaxTree, const SyntaxToken* LiteralToken) noexcept;
        [[nodiscard]] LiteralExpressionSyntax(const class SyntaxTree* SyntaxTree, const SyntaxToken* LiteralToken, Literal Value) noexcept;

        SyntaxKind Kind() const noexcept override;

        const SyntaxToken* LiteralToken;
        Literal Value;

    private:
        std::size_t ChildrenCount() const noexcept override;
        const SyntaxNode* ChildAt(std::size_t Index) const noexcept override;
    };
} // namespace Mamba