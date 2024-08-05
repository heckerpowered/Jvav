#pragma once

#include <memory>

#include "BoundNodeKind.h"
#include "BoundStatement.h"

namespace Mamba
{
    class BoundWhileStatement final : public BoundStatement
    {
    public:
        using Super = BoundStatement;

        [[nodiscard]] BoundWhileStatement(
            const std::shared_ptr<const class SyntaxNode> SyntaxNode,
            const std::shared_ptr<const class BoundExpression> Condition,
            const std::shared_ptr<const class BoundStatement> Body
        ) noexcept;

        BoundNodeKind Kind() const noexcept override;

        const std::shared_ptr<const class BoundExpression> Condition;
        const std::shared_ptr<const class BoundStatement> Body;
    };
} // namespace Mamba