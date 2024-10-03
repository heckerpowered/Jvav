#pragma once

#include "BoundExpression.h"
#include "BoundStatement.h"

namespace Mamba
{
    class BoundReturnStatement : public BoundStatement
    {
    public:
        using Super = BoundStatement;

        [[nodiscard]] BoundReturnStatement(
            const SyntaxNode* Syntax,
            const BoundExpression* Expression
        ) noexcept;
        ~BoundReturnStatement() noexcept override;

        BoundNodeKind Kind() const noexcept override;

        const BoundExpression* Expression;
    };
} // namespace Mamba