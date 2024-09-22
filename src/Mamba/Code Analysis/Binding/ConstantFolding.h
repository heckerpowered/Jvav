#pragma once

#include "BoundBinaryOperator.h"
#include "BoundExpression.h"
#include "BoundUnaryOperator.h"

namespace Mamba
{
    class ConstantFolding
    {
        ConstantFolding() = delete;
        ~ConstantFolding() = delete;

    public:
        static Constant Fold(
            const BoundUnaryOperator& Operator,
            const BoundExpression* Operand
        );

        static Constant Fold(
            const BoundExpression* Left,
            const BoundBinaryOperator& Operator,
            const BoundExpression* Right
        ) noexcept;
    };
} // namespace Mamba