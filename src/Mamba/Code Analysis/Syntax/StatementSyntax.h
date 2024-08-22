#pragma once

#include "SyntaxNode.h"

#include <memory>

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