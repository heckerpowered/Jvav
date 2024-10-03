#pragma once

#include "BoundExpression.h"
#include "BoundStatement.h"
#include "VariableSymbol.h"

namespace Mamba
{
    class BoundVariableDeclaration : public BoundStatement
    {
    public:
        using Super = BoundStatement;

        [[nodiscard]] BoundVariableDeclaration(
            const SyntaxNode* Syntax,
            const VariableSymbol* Variable,
            const BoundExpression* Initializer
        ) noexcept;

        ~BoundVariableDeclaration() noexcept override;

        BoundNodeKind Kind() const noexcept override;

        const VariableSymbol* Variable;
        const BoundExpression* Initializer;
    };
} // namespace Mamba