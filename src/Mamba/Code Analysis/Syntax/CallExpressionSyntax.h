#pragma once

#include "ExpressionSyntax.h"
#include "SeperatedSyntaxList.h"
#include "SyntaxNode.h"
#include "SyntaxToken.h"
#include <memory>

namespace Mamba
{
    class CallExpressionSyntax : public ExpressionSyntax
    {
    public:
        using Super = ExpressionSyntax;

        [[nodiscard]] CallExpressionSyntax(
            const std::shared_ptr<const class SyntaxTree> SyntaxTree,
            const std::shared_ptr<const class SyntaxToken> Identifier,
            const std::shared_ptr<const class SyntaxToken> OpenParenthesisToken,
            const std::shared_ptr<const SeperatedSyntaxList<const std::shared_ptr<const class SyntaxNode>>> Arguments,
            const std::shared_ptr<const class SyntaxToken> CloseParenthesisToken) noexcept;

        virtual SyntaxKind Kind() const noexcept override;
        virtual std::vector<std::shared_ptr<const class SyntaxNode>> Children() const noexcept override;

        const std::shared_ptr<const class SyntaxToken> Identifier;
        const std::shared_ptr<const class SyntaxToken> OpenParenthesisToken;
        const std::shared_ptr<const SeperatedSyntaxList<const std::shared_ptr<const class SyntaxNode>>> Arguments;
        const std::shared_ptr<const class SyntaxToken> CloseParenthesisToken;
    };
} // namespace Mamba