#pragma once

#include "BoundExpression.h"
#include "BoundNodeKind.h"
#include "BoundStatement.h"

namespace Mamba
{
    class BoundWhileStatement final : public BoundStatement
    {
    public:
        using Super = BoundStatement;

        [[nodiscard]] BoundWhileStatement(
            const SyntaxNode* SyntaxNode,
            const BoundExpression* Condition,
            const BoundStatement* Body
        ) noexcept;

        ~BoundWhileStatement() noexcept override;

        BoundNodeKind Kind() const noexcept override;

        const BoundExpression* Condition;
        const BoundStatement* Body;
    };
} // namespace Mamba