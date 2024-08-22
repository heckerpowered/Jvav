#pragma once

#include "ExpressionSyntax.h"
#include "SyntaxToken.h"
#include "SyntaxTree.h"

namespace Mamba
{
    class ParenthesizedExpressionSyntax : public ExpressionSyntax
    {
    public:
        using Super = ExpressionSyntax;

        [[nodiscard]] ParenthesizedExpressionSyntax(
            const class SyntaxTree* SyntaxTree,
            const SyntaxToken* OpenParenthesisToken,
            const ExpressionSyntax* Expression,
            const SyntaxToken* CloseParenthesisToken
        ) noexcept;

        ~ParenthesizedExpressionSyntax() noexcept override;

        SyntaxKind Kind() const noexcept override;
        std::vector<const SyntaxNode*> Children() const noexcept override;

        const SyntaxToken* OpenParenthesisToken;
        const ExpressionSyntax* Expression;
        const SyntaxToken* CloseParenthesisToken;
    };
} // namespace Mamba