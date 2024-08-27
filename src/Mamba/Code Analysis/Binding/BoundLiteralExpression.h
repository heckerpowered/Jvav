#pragma once

#include "BoundExpression.h"
#include "BoundNodeKind.h"
#include "Literal.h"

namespace Mamba
{
    class BoundLiteralExpression : public BoundExpression
    {
    public:
        using Super = BoundExpression;

        [[nodiscard]] BoundLiteralExpression(
            const SyntaxNode* Syntax,
            Literal Value
        ) noexcept;

        BoundNodeKind Kind() const noexcept override;
        const TypeSymbol* Type() const noexcept override;
        Constant ConstantValue() const noexcept override;

    private:
        Literal Value;
    };
} // namespace Mamba