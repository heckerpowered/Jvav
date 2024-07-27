#pragma once

#include <memory>

#include "MambaCore.h"

#include "BoundNode.h"

namespace Mamba
{
    class BoundExpression : public BoundNode
    {
    public:
        using Super = BoundNode;

        [[nodiscard]] BoundExpression(const std::shared_ptr<const class SyntaxNode> Syntax) noexcept;

        virtual std::shared_ptr<const class TypeSymbol> Type() const noexcept = 0;
        virtual NullableSharedPtr<const class BoundConstant> ConstantValue() const noexcept;
    };
} // namespace Mamba