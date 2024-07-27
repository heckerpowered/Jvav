#pragma once

#include <memory>

#include "BoundExpression.h"
#include "BoundNodeKind.h"
#include "MambaCore.h"

namespace Mamba
{
    class BoundUnaryExpression : public BoundExpression
    {
    public:
        using Super = BoundExpression;

        [[nodiscard]] BoundUnaryExpression(
            const std::shared_ptr<const class SyntaxNode> Syntax,
            const class BoundUnaryOperator& Operator,
            const std::shared_ptr<const class BoundExpression> Operand
        ) noexcept;

        BoundNodeKind Kind() const noexcept override;
        std::shared_ptr<const class TypeSymbol> Type() const noexcept override;
        NullableSharedPtr<const class BoundConstant> ConstantValue() const noexcept override;

        const class BoundUnaryOperator& Operator;
        const std::shared_ptr<const class BoundExpression> Operand;
    };
} // namespace Mamba