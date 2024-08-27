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

        const SyntaxToken* IdentifierToken;

    private:
        std::size_t ChildrenCount() const noexcept override;
        const SyntaxNode* ChildAt(std::size_t Index) const noexcept override;
    };
} // namespace Mamba