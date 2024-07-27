#pragma once

#include "BoundNode.h"
#include "SyntaxNode.h"
#include <memory>

namespace Mamba
{
    class BoundStatement : public BoundNode
    {
    public:
        using Super = BoundNode;

        [[nodiscard]] BoundStatement(const std::shared_ptr<const class SyntaxNode> Syntax) noexcept;
    };
} // namespace Mamba