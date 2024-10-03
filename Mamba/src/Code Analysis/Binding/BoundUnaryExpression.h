#pragma once

#include "BoundExpression.h"
#include "BoundNodeKind.h"
#include "BoundUnaryOperator.h"

namespace Mamba
{
    class BoundUnaryExpression : public BoundExpression
    {
    public:
        using Super = BoundExpression;

        [[nodiscard]] BoundUnaryExpression(
            const SyntaxNode* Syntax,
            const BoundUnaryOperator& Operator,
            const BoundExpression* Operand
        ) noexcept;

        ~BoundUnaryExpression() noexcept override;

        BoundNodeKind Kind() const noexcept override;
        const TypeSymbol* Type() const noexcept override;
        Constant ConstantValue() const noexcept override;

        const BoundUnaryOperator& Operator;
        const BoundExpression* Operand;
    };
} // namespace Mamba