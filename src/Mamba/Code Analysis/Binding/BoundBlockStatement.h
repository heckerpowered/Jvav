#pragma once

#include "BoundNodeKind.h"
#include "BoundStatement.h"

namespace Mamba
{
    class BoundBlockStatement : public BoundStatement
    {
    public:
        using Super = BoundStatement;

        [[nodiscard]] BoundBlockStatement(const SyntaxNode* Syntax, std::vector<const BoundStatement*>&& Statements) noexcept;
        ~BoundBlockStatement() noexcept override;

        BoundNodeKind Kind() const noexcept override;

        std::vector<const BoundStatement*> Statements;
    };
}; // namespace Mamba