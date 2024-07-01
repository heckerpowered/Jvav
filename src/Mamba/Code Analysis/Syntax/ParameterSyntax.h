#pragma once

#include "SyntaxNode.h"
#include "SyntaxToken.h"
#include "SyntaxTree.h"
#include <memory>

namespace Mamba
{
    class ParameterSyntax : public SyntaxNode
    {
    public:
        using Super = SyntaxNode;

        [[nodiscard]] ParameterSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                      const std::shared_ptr<const class SyntaxToken> Identifier,
                                      const std::shared_ptr<const class TypeClauseSyntax> Type) noexcept;

        virtual SyntaxKind Kind() const noexcept override;
        virtual std::vector<std::shared_ptr<const class SyntaxNode>> Children() const noexcept override;

        const std::shared_ptr<const class SyntaxToken> Identifier;
        const std::shared_ptr<const class TypeClauseSyntax> Type;
    };
} // namespace Mamba