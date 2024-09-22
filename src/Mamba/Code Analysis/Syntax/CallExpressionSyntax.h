#pragma once

#include "ExpressionSyntax.h"
#include "SeperatedSyntaxList.h"
#include "SyntaxNode.h"
#include "SyntaxToken.h"

namespace Mamba
{
    class CallExpressionSyntax : public ExpressionSyntax
    {
    public:
        using Super = ExpressionSyntax;

        [[nodiscard]] CallExpressionSyntax(
            const class SyntaxTree* SyntaxTree,
            const SyntaxToken* Identifier,
            const SyntaxToken* OpenParenthesisToken,
            SeperatedSyntaxList<const ExpressionSyntax*> Arguments,
            const SyntaxToken* CloseParenthesisToken
        ) noexcept;

        ~CallExpressionSyntax() noexcept override;

        SyntaxKind Kind() const noexcept override;

        const SyntaxToken* Identifier;
        const SyntaxToken* OpenParenthesisToken;
        SeperatedSyntaxList<const ExpressionSyntax*> Arguments;
        const SyntaxToken* CloseParenthesisToken;

    private:
        std::size_t ChildrenCount() const noexcept override;
        const SyntaxNode* ChildAt(std::size_t Index) const noexcept override;
    };
} // namespace Mamba