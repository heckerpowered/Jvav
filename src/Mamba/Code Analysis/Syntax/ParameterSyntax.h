#pragma once

#include "SyntaxNode.h"
#include "SyntaxToken.h"
#include "SyntaxTree.h"
#include "TypeClauseSyntax.h"

namespace Mamba
{
    class ParameterSyntax : public SyntaxNode
    {
    public:
        using Super = SyntaxNode;

        [[nodiscard]] ParameterSyntax(
            const class SyntaxTree* SyntaxTree,
            const SyntaxToken* Identifier,
            const TypeClauseSyntax* Type
        ) noexcept;

        ~ParameterSyntax() noexcept override;

        SyntaxKind Kind() const noexcept override;
        std::vector<const SyntaxNode*> Children() const noexcept override;

        const SyntaxToken* Identifier;
        const class TypeClauseSyntax* Type;
    };
} // namespace Mamba