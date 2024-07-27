#pragma once

#include <memory>

#include "BoundNodeKind.h"
#include "BoundStatement.h"

namespace Mamba
{
    class BoundDoWhileStatement final : public BoundStatement
    {
    public:
        using Super = BoundStatement;

        [[nodiscard]] BoundDoWhileStatement(
            const std::shared_ptr<const class SyntaxNode> SyntaxNode,
            const std::shared_ptr<const class BoundStatement> Body,
            const std::shared_ptr<const class BoundStatement> Condition
        ) noexcept;

        BoundNodeKind Kind() const noexcept override;

        const std::shared_ptr<const class BoundStatement> Body;
        const std::shared_ptr<const class BoundStatement> Condition;
    };
} // namespace Mamba