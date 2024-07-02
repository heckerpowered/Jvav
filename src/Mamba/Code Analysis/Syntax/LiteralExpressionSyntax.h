#pragma once

#include "ExpressionSyntax.h"

#include <memory>

namespace Mamba
{
    // A expression that represents a literal value, empty values are disallowed
    class LiteralExpressionSyntax : public ExpressionSyntax
    {
    public:
        using Super = ExpressionSyntax;

        [[nodiscard]] LiteralExpressionSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                              const std::shared_ptr<const class SyntaxToken> LiteralToken) noexcept;

        [[nodiscard]] LiteralExpressionSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                              const std::shared_ptr<const class SyntaxToken> LiteralToken,
                                              const std::shared_ptr<const class Literal> Value) noexcept;

        virtual SyntaxKind Kind() const noexcept override;
        virtual std::vector<std::shared_ptr<const class SyntaxNode>> Children() const noexcept override;

        const std::shared_ptr<const class SyntaxToken> LiteralToken;
        const std::shared_ptr<const class Literal> Value;
    };
} // namespace Mamba