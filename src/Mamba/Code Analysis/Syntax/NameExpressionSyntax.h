#pragma once

#include "ExpressionSyntax.h"
#include "SyntaxToken.h"
#include "SyntaxTree.h"
#include <memory>

namespace Mamba
{
    class NameExpressionSyntax : public ExpressionSyntax
    {
    public:
        using Super = ExpressionSyntax;

        [[nodiscard]] NameExpressionSyntax(
            const std::shared_ptr<const class SyntaxTree> SyntaxTree,
            const std::shared_ptr<const class SyntaxToken> IdentifierToken
        ) noexcept;

        SyntaxKind Kind() const noexcept override;
        std::vector<std::shared_ptr<const class SyntaxNode>> Children() const noexcept override;

        const std::shared_ptr<const class SyntaxToken> IdentifierToken;
    };
} // namespace Mamba