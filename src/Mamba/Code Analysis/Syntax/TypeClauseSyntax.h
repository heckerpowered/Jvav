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
        std::vector<const SyntaxNode*> Children() const noexcept override;

        const SyntaxToken* ColonToken;
        const SyntaxToken* Identifier;
    };
} // namespace Mamba