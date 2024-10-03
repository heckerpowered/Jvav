#pragma once

#include "BoundNodeKind.h"
#include "SyntaxNode.h"

namespace Mamba
{
    class BoundNode
    {
    protected:
        [[nodiscard]] BoundNode(const SyntaxNode* Syntax) noexcept;

    public:
        virtual ~BoundNode() noexcept = default;
        virtual BoundNodeKind Kind() const noexcept = 0;

        const SyntaxNode* Syntax;
    };
} // namespace Mamba