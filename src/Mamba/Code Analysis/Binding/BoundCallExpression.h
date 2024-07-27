#pragma once

#include "BoundExpression.h"
#include "BoundNodeKind.h"

#include <memory>
#include <vector>

namespace Mamba
{
    class BoundCallExpression : public BoundExpression
    {
    public:
        using Super = BoundExpression;

        [[nodiscard]] BoundCallExpression(
            const std::shared_ptr<const class SyntaxNode> Syntax,
            const std::shared_ptr<const class FunctionSymbol> Function,
            const std::span<std::shared_ptr<const class BoundExpression>> Arguments
        ) noexcept;

        [[nodiscard]] BoundCallExpression(
            const std::shared_ptr<const class SyntaxNode> Syntax,
            const std::shared_ptr<const class FunctionSymbol> Function,
            std::vector<std::shared_ptr<const class BoundExpression>>&& Arguments
        ) noexcept;

        BoundNodeKind Kind() const noexcept override;
        std::shared_ptr<const class TypeSymbol> Type() const noexcept override;

        const std::shared_ptr<const class FunctionSymbol> Function;
        const std::vector<std::shared_ptr<const class BoundExpression>> Arguments;
    };
} // namespace Mamba