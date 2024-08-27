#pragma once

#include "BoundBinaryOperator.h"
#include "BoundExpression.h"
#include "BoundNodeKind.h"
#include "VariableSymbol.h"

namespace Mamba
{
    class BoundCompoundAssignmentExpression : public BoundExpression
    {
    public:
        using Super = BoundExpression;

        [[nodiscard]] BoundCompoundAssignmentExpression(
            const SyntaxNode* Syntax,
            const VariableSymbol* Variable,
            const BoundBinaryOperator& Operator,
            const BoundExpression* Expression
        ) noexcept;

        ~BoundCompoundAssignmentExpression() noexcept override;

        BoundNodeKind Kind() const noexcept override;
        const TypeSymbol* Type() const noexcept override;

        const VariableSymbol* Variable;
        const BoundBinaryOperator& Operator;
        const BoundExpression* Expression;
    };
} // namespace Mamba