#pragma once

#include "BoundExpression.h"
#include "VariableSymbol.h"

namespace Mamba
{
    class BoundVariableExpression : public BoundExpression
    {
    public:
        using Super = BoundExpression;

        [[nodiscard]] BoundVariableExpression(
            const SyntaxNode* Syntax,
            const VariableSymbol* Variable
        ) noexcept;

        BoundNodeKind Kind() const noexcept override;
        const TypeSymbol* Type() const noexcept override;

        const VariableSymbol* Variable;
    };
} // namespace Mamba