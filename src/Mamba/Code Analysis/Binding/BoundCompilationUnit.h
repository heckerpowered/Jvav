#pragma once

#include "BoundNode.h"
#include "BoundScope.h"
#include "SyntaxNode.h"

namespace Mamba
{
    class BoundCompilationUnit : public BoundNode
    {
    public:
        using Super = BoundNode;

        [[nodiscard]] BoundCompilationUnit(
            const SyntaxNode* SyntaxNode,
            const BoundScope* GlobalScope
        ) noexcept;

        virtual BoundNodeKind Kind() const noexcept override;

        const BoundScope* GlobalScope;
    };
} // namespace Mamba