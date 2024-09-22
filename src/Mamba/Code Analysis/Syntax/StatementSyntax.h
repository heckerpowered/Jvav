#pragma once

#include "SyntaxNode.h"

namespace Mamba
{
    class StatementSyntax : public SyntaxNode
    {
    public:
        using Super = SyntaxNode;

    protected:
        [[nodiscard]] StatementSyntax(const class SyntaxTree* SyntaxTree) noexcept;
    };
} // namespace Mamba