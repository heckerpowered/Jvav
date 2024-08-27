#pragma once

#include "BoundNode.h"
#include "SyntaxNode.h"

namespace Mamba
{
    class BoundStatement : public BoundNode
    {
    public:
        using Super = BoundNode;

        [[nodiscard]] BoundStatement(const SyntaxNode* Syntax) noexcept;
    };
} // namespace Mamba