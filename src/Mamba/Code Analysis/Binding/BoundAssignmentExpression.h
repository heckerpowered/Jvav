#pragma once

#include <memory>

#include "BoundExpression.h"
#include "BoundNodeKind.h"

namespace Mamba
{
    class BoundAssignmentExpression : public BoundExpression
    {
    public:
        using Super = BoundExpression;

        [[nodiscard]] BoundAssignmentExpression(
            const std::shared_ptr<const class SyntaxNode> Syntax,
            const std::shared_ptr<const class VariableSymbol> Variable,
            const std::shared_ptr<const class BoundExpression> Expression
        ) noexcept;

        BoundNodeKind Kind() const noexcept override;
        std::shared_ptr<const class TypeSymbol> Type() const noexcept override;

        const std::shared_ptr<const class VariableSymbol> Variable;
        const std::shared_ptr<const class BoundExpression> Expression;
    };
} // namespace Mamba