#pragma once

#include "BoundNode.h"
#include "Constant.h"
#include "TypeSymbol.h"

namespace Mamba
{
    class BoundExpression : public BoundNode
    {
    public:
        using Super = BoundNode;

        [[nodiscard]] BoundExpression(const SyntaxNode* Syntax) noexcept;

        virtual const TypeSymbol* Type() const noexcept = 0;
        virtual Constant ConstantValue() const noexcept;
    };
} // namespace Mamba