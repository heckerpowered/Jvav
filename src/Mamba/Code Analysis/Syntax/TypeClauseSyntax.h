#pragma once

#include "SyntaxNode.h"
#include "SyntaxToken.h"

namespace Mamba
{
    class TypeClauseSyntax : public SyntaxNode
    {
    public:
        using Super = SyntaxNode;

        [[nodiscard]] TypeClauseSyntax(
            const class SyntaxTree* SyntaxTree,
            const SyntaxToken* ColonToken,
            const SyntaxToken* Identifier
        ) noexcept;

        SyntaxKind Kind() const noexcept override;

        const SyntaxToken* ColonToken;
        const SyntaxToken* Identifier;

    private:
        std::size_t ChildrenCount() const noexcept override;
        const SyntaxNode* ChildAt(std::size_t Index) const noexcept override;
    };
} // namespace Mamba