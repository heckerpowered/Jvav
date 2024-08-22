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
        std::vector<const SyntaxNode*> Children() const noexcept override;

        const SyntaxToken* Identifier;
        const SyntaxToken* OpenParenthesisToken;
        SeperatedSyntaxList<const ExpressionSyntax*> Arguments;
        const SyntaxToken* CloseParenthesisToken;
    };
} // namespace Mamba