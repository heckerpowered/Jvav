#pragma once

#include "BoundBinaryOperator.h"
#include "BoundExpression.h"
#include "BoundNodeKind.h"

namespace Mamba
{
    class BoundBinaryExpression : public BoundExpression
    {
    public:
        using Super = BoundExpression;

        [[nodiscard]] BoundBinaryExpression(const SyntaxNode* Syntax, const BoundExpression* Left, const BoundBinaryOperator& Operator, const BoundExpression* Right) noexcept;
        ~BoundBinaryExpression() noexcept override;

        BoundNodeKind Kind() const noexcept override;
        const TypeSymbol* Type() const noexcept override;
        Constant ConstantValue() const noexcept override;

        const BoundExpression* Left;
        const BoundBinaryOperator& Operator;
        const BoundExpression* Right;
    };
} // namespace Mamba