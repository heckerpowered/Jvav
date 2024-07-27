#pragma once

#include <memory>

#include "BoundExpression.h"
#include "BoundNodeKind.h"

namespace Mamba
{
    class BoundCompoundAssignmentExpression : public BoundExpression
    {
    public:
        using Super = BoundExpression;

        [[nodiscard]] BoundCompoundAssignmentExpression(
            const std::shared_ptr<const class SyntaxNode> Syntax,
            const std::shared_ptr<const class VariableSymbol> Variable,
            const class BoundBinaryOperator& Operator,
            const std::shared_ptr<const class BoundExpression> Expression
        ) noexcept;

        BoundNodeKind Kind() const noexcept override;
        std::shared_ptr<const class TypeSymbol> Type() const noexcept override;

        const std::shared_ptr<const class VariableSymbol> Variable;
        const class BoundBinaryOperator& Operator;
        const std::shared_ptr<const class BoundExpression> Expression;
    };
} // namespace Mamba