#pragma once

#include "ExpressionSyntax.h"
#include "SyntaxToken.h"

namespace Mamba
{
    class LiteralExpressionSyntax : public ExpressionSyntax
    {
    public:
        using Super = ExpressionSyntax;

        [[nodiscard]] LiteralExpressionSyntax(
            const class SyntaxTree* SyntaxTree,
            const SyntaxToken* LiteralToken
        ) noexcept;

        [[nodiscard]] LiteralExpressionSyntax(
            const class SyntaxTree* SyntaxTree,
            const SyntaxToken* LiteralToken,
            const Literal Value
        ) noexcept;

        SyntaxKind Kind() const noexcept override;
        std::vector<const SyntaxNode*> Children() const noexcept override;

        const SyntaxToken* LiteralToken;
        Literal Value;
    };
} // namespace Mamba