#pragma once

#include "BoundExpression.h"
#include "BoundNodeKind.h"
#include "VariableSymbol.h"

namespace Mamba
{
    class BoundAssignmentExpression : public BoundExpression
    {
    public:
        using Super = BoundExpression;

        [[nodiscard]] BoundAssignmentExpression(
            const SyntaxNode* Syntax,
            const VariableSymbol* Variable,
            const BoundExpression* Expression
        ) noexcept;

        ~BoundAssignmentExpression() noexcept override;

        BoundNodeKind Kind() const noexcept override;
        const TypeSymbol* Type() const noexcept override;

        const VariableSymbol* Variable;
        const BoundExpression* Expression;
    };
} // namespace Mamba