#pragma once

#include "BoundNodeKind.h"
#include "BoundStatement.h"
#include <memory>

namespace Mamba
{
    class BoundBlockStatement : public BoundStatement
    {
    public:
        using Super = BoundStatement;

        [[nodiscard]] BoundBlockStatement(
            const std::shared_ptr<const class SyntaxNode> Syntax,
            const std::span<const std::shared_ptr<const class BoundStatement>> Statements
        ) noexcept;

        [[nodiscard]] BoundBlockStatement(
            const std::shared_ptr<const class SyntaxNode> Syntax,
            std::vector<std::shared_ptr<const class BoundStatement>>&& Statements
        ) noexcept;

        BoundNodeKind Kind() const noexcept override;

        const std::vector<std::shared_ptr<const class BoundStatement>> Statements;
    };
}; // namespace Mamba