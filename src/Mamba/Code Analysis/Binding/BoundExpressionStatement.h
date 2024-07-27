#pragma once

#include <memory>

#include "BoundNodeKind.h"
#include "BoundStatement.h"

namespace Mamba
{
    class BoundExpressionStatement : public BoundStatement
    {
    public:
        using Super = BoundStatement;

        [[nodiscard]] BoundExpressionStatement(
            const std::shared_ptr<const class SyntaxNode> Syntax,
            const std::shared_ptr<const class BoundExpression> Expression
        ) noexcept;

        BoundNodeKind Kind() const noexcept override;

        const std::shared_ptr<const class BoundExpression> Expression;
    };
} // namespace Mamba