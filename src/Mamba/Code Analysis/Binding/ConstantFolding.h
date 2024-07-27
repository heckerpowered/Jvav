#pragma once

#include <memory>

#include "BoundBinaryOperator.h"
#include "MambaCore.h"

#include "BoundExpression.h"
#include "BoundUnaryOperator.h"

namespace Mamba
{
    class ConstantFolding
    {
        ConstantFolding() = delete;
        ~ConstantFolding() = delete;

    public:
        static NullableSharedPtr<const class BoundConstant>
            Fold(const BoundUnaryOperator& Operator, const std::shared_ptr<const class BoundExpression> Operand);

        static NullableSharedPtr<const class BoundConstant> Fold(
            const std::shared_ptr<const class BoundExpression>& Left,
            const BoundBinaryOperator& Operator,
            const std::shared_ptr<const class BoundExpression> Right
        ) noexcept;
    };
} // namespace Mamba