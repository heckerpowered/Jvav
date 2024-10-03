#pragma once

#include "BoundExpression.h"
#include "BoundNodeKind.h"
#include "BoundStatement.h"

namespace Mamba
{
    class BoundForStatement : public BoundStatement
    {
    public:
        using Super = BoundStatement;

        [[nodiscard]] BoundForStatement(
            const SyntaxNode* Syntax,
            const BoundStatement* InitStatement,
            const BoundExpression* Condition,
            const BoundExpression* Expression,
            const BoundStatement* Body
        ) noexcept;
        ~BoundForStatement() noexcept override;

        BoundNodeKind Kind() const noexcept override;

        const BoundStatement* InitStatement;
        const BoundExpression* Condition;
        const BoundExpression* Expression;
        const BoundStatement* Body;
    };
} // namespace Mamba