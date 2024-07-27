#pragma once

#include <memory>

#include "BoundExpression.h"
#include "BoundNodeKind.h"
#include "MambaCore.h"

namespace Mamba
{
    class BoundBinaryExpression : public BoundExpression
    {
    public:
        using Super = BoundExpression;

        [[nodiscard]] BoundBinaryExpression(
            const std::shared_ptr<const class SyntaxNode> Syntax,
            const std::shared_ptr<const class BoundExpression> Left,
            const class BoundBinaryOperator& Operator,
            const std::shared_ptr<const class BoundExpression> Right
        ) noexcept;

        BoundNodeKind Kind() const noexcept override;
        std::shared_ptr<const class TypeSymbol> Type() const noexcept override;
        NullableSharedPtr<const class BoundConstant> ConstantValue() const noexcept override;

        std::shared_ptr<const class BoundExpression> Left;
        const class BoundBinaryOperator& Operator;
        const std::shared_ptr<const class BoundExpression> Right;
    };
} // namespace Mamba