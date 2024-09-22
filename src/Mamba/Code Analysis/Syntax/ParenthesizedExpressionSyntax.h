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

        const SyntaxToken* OpenParenthesisToken;
        const ExpressionSyntax* Expression;
        const SyntaxToken* CloseParenthesisToken;

    private:
        std::size_t ChildrenCount() const noexcept override;
        const SyntaxNode* ChildAt(std::size_t Index) const noexcept override;
    };
} // namespace Mamba