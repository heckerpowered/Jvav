#pragma once

#include <memory>

#include "BoundNodeKind.h"

namespace Mamba
{
    class BoundNode
    {
    protected:
        [[nodiscard]] BoundNode(const std::shared_ptr<const class SyntaxNode> Syntax) noexcept;

    public:
        virtual ~BoundNode() = default;
        virtual BoundNodeKind Kind() const noexcept = 0;

        const std::shared_ptr<const class SyntaxNode> Syntax;
    };
} // namespace Mamba