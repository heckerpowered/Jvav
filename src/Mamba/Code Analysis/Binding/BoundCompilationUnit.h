#pragma once

#include <memory>

#include "BoundNode.h"

namespace Mamba
{
    class BoundCompilationUnit : public BoundNode
    {
    public:
        using Super = BoundNode;

        [[nodiscard]] BoundCompilationUnit(
            const std::shared_ptr<const class SyntaxNode> SyntaxNode,
            const std::shared_ptr<const class BoundScope> GlobalScope
        ) noexcept;

        virtual BoundNodeKind Kind() const noexcept override;

        const std::shared_ptr<const class BoundScope> GlobalScope;
    };
} // namespace Mamba