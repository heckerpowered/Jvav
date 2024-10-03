#pragma once

#include "BoundExpression.h"
#include "BoundNodeKind.h"
#include "FunctionSymbol.h"

#include <vector>

namespace Mamba
{
    class BoundCallExpression : public BoundExpression
    {
    public:
        using Super = BoundExpression;

        [[nodiscard]] BoundCallExpression(
            const SyntaxNode* Syntax,
            const FunctionSymbol* Function,
            std::vector<const BoundExpression*>&& Arguments
        ) noexcept;

        ~BoundCallExpression() noexcept override;

        BoundNodeKind Kind() const noexcept override;
        const TypeSymbol* Type() const noexcept override;

        const FunctionSymbol* Function;
        const std::vector<const BoundExpression*> Arguments;
    };
} // namespace Mamba