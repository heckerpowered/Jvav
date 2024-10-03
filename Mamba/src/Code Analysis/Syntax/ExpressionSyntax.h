#pragma once

#include "SyntaxNode.h"

namespace Mamba
{
    class ExpressionSyntax : public SyntaxNode
    {
    public:
        using Super = SyntaxNode;

    protected:
        [[nodiscard]] ExpressionSyntax(const class SyntaxTree* SyntaxTree) noexcept;
    };
} // namespace Mamba