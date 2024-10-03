#pragma once

#include "BoundExpression.h"
#include "BoundNodeKind.h"
#include "BoundStatement.h"

namespace Mamba
{
    class BoundExpressionStatement : public BoundStatement
    {
    public:
        using Super = BoundStatement;

        [[nodiscard]] BoundExpressionStatement(
            const SyntaxNode* Syntax,
            const BoundExpression* Expression
        ) noexcept;
        ~BoundExpressionStatement() noexcept override;

        BoundNodeKind Kind() const noexcept override;

        const BoundExpression* Expression;
    };
} // namespace Mamba