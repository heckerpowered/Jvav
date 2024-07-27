#pragma once

#include <memory>

#include "BoundExpression.h"
#include "BoundExpressionStatement.h"
#include "BoundNodeKind.h"
#include "BoundStatement.h"

namespace Mamba
{
    class BoundForStatement : public BoundStatement
    {
    public:
        using Super = BoundStatement;

        [[nodiscard]] BoundForStatement(
            const std::shared_ptr<const class SyntaxNode> Syntax,
            const std::shared_ptr<const class BoundExpressionStatement> InitStatement,
            const std::shared_ptr<const class BoundExpressionStatement> ConditionStatement,
            const std::shared_ptr<const class BoundExpression> Expression,
            const std::shared_ptr<const class BoundStatement> Body
        ) noexcept;

        BoundNodeKind Kind() const noexcept override;

        const std::shared_ptr<const class BoundExpressionStatement> InitStatement;
        const std::shared_ptr<const class BoundExpressionStatement> ConditionStatement;
        const std::shared_ptr<const class BoundExpression> Expression;
        const std::shared_ptr<const class BoundStatement> Body;
    };
} // namespace Mamba