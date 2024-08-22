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
        std::vector<const SyntaxNode*> Children() const noexcept override;

        const SyntaxToken* OperatorToken;
        const ExpressionSyntax* Operand;
    };
} // namespace Mamba