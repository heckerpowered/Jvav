#pragma once

#include "ExpressionSyntax.h"
#include "SyntaxToken.h"
#include "SyntaxTree.h"

namespace Mamba
{
    class NameExpressionSyntax : public ExpressionSyntax
    {
    public:
        using Super = ExpressionSyntax;

        [[nodiscard]] NameExpressionSyntax(
            const class SyntaxTree* SyntaxTree,
            const SyntaxToken* IdentifierToken
        ) noexcept;

        SyntaxKind Kind() const noexcept override;
        std::vector<const SyntaxNode*> Children() const noexcept override;

        const SyntaxToken* IdentifierToken;
    };
} // namespace Mamba