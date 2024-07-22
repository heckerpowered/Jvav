#pragma once

#include "ExpressionSyntax.h"
#include "SyntaxToken.h"
#include "SyntaxTree.h"
#include <memory>

namespace Mamba
{
    class UnaryExpressionSyntax : public ExpressionSyntax
    {
    public:
        using Super = ExpressionSyntax;

        [[nodiscard]] UnaryExpressionSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                            const std::shared_ptr<const class SyntaxToken> OperatorToken,
                                            const std::shared_ptr<const class ExpressionSyntax> Operand) noexcept;

        SyntaxKind Kind() const noexcept override;
        std::vector<std::shared_ptr<const class SyntaxNode>> Children() const noexcept override;

        const std::shared_ptr<const class SyntaxToken> OperatorToken;
        const std::shared_ptr<const class ExpressionSyntax> Operand;
    };
} // namespace Mamba