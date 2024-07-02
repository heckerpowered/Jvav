#pragma once

#include "SyntaxNode.h"

namespace Mamba
{
    class ExpressionSyntax : public SyntaxNode
    {
    public:
        using Super = SyntaxNode;

    protected:
        [[nodiscard]] ExpressionSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree) noexcept;
    };
} // namespace Mamba