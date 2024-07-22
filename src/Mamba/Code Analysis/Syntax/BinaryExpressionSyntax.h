#pragma once

#include "ExpressionSyntax.h"
#include "SyntaxKind.h"
#include "SyntaxToken.h"
#include <memory>

namespace Mamba
{
    class BinaryExpressionSyntax : public ExpressionSyntax
    {
    public:
        using Super = ExpressionSyntax;

        [[nodiscard]] BinaryExpressionSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                             const std::shared_ptr<const class ExpressionSyntax> Left,
                                             const std::shared_ptr<const class SyntaxToken> OperatorToken,
                                             const std::shared_ptr<const class ExpressionSyntax> Right) noexcept;

        SyntaxKind Kind() const noexcept override;
        std::vector<std::shared_ptr<const class SyntaxNode>> Children() const noexcept override;

        std::shared_ptr<const class ExpressionSyntax> Left;
        std::shared_ptr<const class SyntaxToken> OperatorToken;
        std::shared_ptr<const class ExpressionSyntax> Right;
    };
} // namespace Mamba
