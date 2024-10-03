#pragma once

#include "BoundExpression.h"
#include "BoundNodeKind.h"
#include "BoundStatement.h"

namespace Mamba
{
    class BoundDoWhileStatement final : public BoundStatement
    {
    public:
        using Super = BoundStatement;

        [[nodiscard]] BoundDoWhileStatement(
            const SyntaxNode* SyntaxNode,
            const BoundStatement* Body,
            const BoundExpression* Condition
        ) noexcept;

        ~BoundDoWhileStatement() noexcept override;

        BoundNodeKind Kind() const noexcept override;

        const BoundStatement* Body;
        const BoundExpression* Condition;
    };
} // namespace Mamba