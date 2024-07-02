#pragma once

#include "ExpressionSyntax.h"
#include "SyntaxToken.h"
#include "SyntaxTree.h"
#include <memory>

namespace Mamba
{
    class ParenthesizedExpressionSyntax : public ExpressionSyntax
    {
    public:
        using Super = ExpressionSyntax;

        [[nodiscard]] ParenthesizedExpressionSyntax(
            const std::shared_ptr<const class SyntaxTree> SyntaxTree,
            const std::shared_ptr<const class SyntaxToken> OpenParenthesisToken,
            const std::shared_ptr<const class ExpressionSyntax> Expression,
            const std::shared_ptr<const class SyntaxToken> CloseParenthesisToken) noexcept;

        virtual SyntaxKind Kind() const noexcept override;
        virtual std::vector<std::shared_ptr<const class SyntaxNode>> Children() const noexcept override;

        const std::shared_ptr<const class SyntaxToken> OpenParenthesisToken;
        const std::shared_ptr<const class ExpressionSyntax> Expression;
        const std::shared_ptr<const class SyntaxToken> CloseParenthesisToken;
    };
} // namespace Mamba