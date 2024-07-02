#pragma once

#include "SyntaxNode.h"

#include <memory>

namespace Mamba
{
    class TypeClauseSyntax : public SyntaxNode
    {
    public:
        using Super = SyntaxNode;

        [[nodiscard]] TypeClauseSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                       const std::shared_ptr<const class SyntaxToken> ColonToken,
                                       const std::shared_ptr<const class SyntaxToken> Identifier) noexcept;

        virtual SyntaxKind Kind() const noexcept override;
        virtual std::vector<std::shared_ptr<const class SyntaxNode>> Children() const noexcept override;

        const std::shared_ptr<const class SyntaxToken> ColonToken;
        const std::shared_ptr<const class SyntaxToken> Identifier;
    };
} // namespace Mamba