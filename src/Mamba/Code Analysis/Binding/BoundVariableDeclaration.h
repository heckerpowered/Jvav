#pragma once

#include <memory>

#include "BoundStatement.h"

namespace Mamba
{
    class BoundVariableDeclaration : public BoundStatement
    {
    public:
        using Super = BoundStatement;

        [[nodiscard]] BoundVariableDeclaration(
            const std::shared_ptr<const class SyntaxNode> Syntax,
            const std::shared_ptr<const class VariableSymbol> Variable,
            const std::shared_ptr<const class BoundExpression> Initializer
        ) noexcept;

        BoundNodeKind Kind() const noexcept override;

        const std::shared_ptr<const class VariableSymbol> Variable;
        const std::shared_ptr<const class BoundExpression> Initializer;
    };
} // namespace Mamba