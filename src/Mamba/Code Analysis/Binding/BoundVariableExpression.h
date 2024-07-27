#pragma once

#include <memory>

#include "BoundExpression.h"

namespace Mamba
{
    class BoundVariableExpression : public BoundExpression
    {
    public:
        using Super = BoundExpression;

        [[nodiscard]] BoundVariableExpression(
            const std::shared_ptr<const class SyntaxNode> Syntax,
            const std::shared_ptr<const class VariableSymbol> Variable
        ) noexcept;

        BoundNodeKind Kind() const noexcept override;

        const std::shared_ptr<const class VariableSymbol> Variable;
    };
} // namespace Mamba