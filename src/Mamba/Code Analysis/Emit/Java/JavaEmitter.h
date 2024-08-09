#pragma once

#include "BoundBinaryExpression.h"
#include "BoundBinaryOperator.h"
#include "BoundBinaryOperatorKind.h"
#include "Emitter.h"

namespace Mamba
{
    class JavaEmitter final : public Emitter
    {
    public:
        void EmitBinaryExpression(const BoundBinaryExpression& Expression)
        {
            if (Expression.Operator.Kind == BoundBinaryOperatorKind::Addition)
            {
                // ldc insn Expression.Left
                // ldc insn Expression.Right
                // add
            }
        }
    };
} // namespace Mamba