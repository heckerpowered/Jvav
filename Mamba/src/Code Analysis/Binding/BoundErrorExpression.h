#pragma once

#include "BoundExpression.h"
#include "BoundNodeKind.h"

namespace Mamba
{
    class BoundErrorExpression : public BoundExpression
    {
    public:
        using Super = BoundExpression;

        [[nodiscard]] BoundErrorExpression(const SyntaxNode* Syntax) noexcept;

        BoundNodeKind Kind() const noexcept override;
        const TypeSymbol* Type() const noexcept override;
    };
} // namespace Mamba