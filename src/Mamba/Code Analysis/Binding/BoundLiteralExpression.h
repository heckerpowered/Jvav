#pragma once

#include <memory>

#include "BoundExpression.h"
#include "BoundNodeKind.h"

namespace Mamba
{
    class BoundLiteralExpression : public BoundExpression
    {
    public:
        using Super = BoundExpression;

        [[nodiscard]] BoundLiteralExpression(
            const std::shared_ptr<const class SyntaxNode> Syntax,
            const std::shared_ptr<const struct Literal> Value
        ) noexcept;

        BoundNodeKind Kind() const noexcept override;
        std::shared_ptr<const class TypeSymbol> Type() const noexcept override;
        std::shared_ptr<const class BoundConstant> ConstantValue() const noexcept override;

    private:
        const std::shared_ptr<const class TypeSymbol> PrivateType;
        const std::shared_ptr<const struct Literal> Value;
    };
} // namespace Mamba