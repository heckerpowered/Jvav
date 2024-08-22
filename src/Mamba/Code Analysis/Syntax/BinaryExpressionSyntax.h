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
        std::vector<const SyntaxNode*> Children() const noexcept override;

        const ExpressionSyntax* Left;
        const SyntaxToken* OperatorToken;
        const ExpressionSyntax* Right;
    };
} // namespace Mamba
