#pragma once

#include "SyntaxNode.h"
#include "SyntaxToken.h"

#include <memory>

namespace Mamba
{
    class ElseClauseSyntax : public SyntaxNode
    {
    public:
        using Super = SyntaxNode;

        [[nodiscard]] ElseClauseSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                       const std::shared_ptr<const class SyntaxToken> ElseKeyword,
                                       const std::shared_ptr<const class StatementSyntax> ElseStatement) noexcept;

        virtual SyntaxKind Kind() const noexcept override;
        virtual std::vector<std::shared_ptr<const SyntaxNode>> Children() const noexcept override;

        const std::shared_ptr<const class SyntaxToken> ElseKeyword;
        const std::shared_ptr<const class StatementSyntax> ElseStatement;
    };
} // namespace Mamba