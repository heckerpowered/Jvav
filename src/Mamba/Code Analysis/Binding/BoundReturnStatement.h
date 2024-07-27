#pragma once

#include <memory>

#include "BoundStatement.h"

namespace Mamba
{
    class BoundReturnStatement : public BoundStatement
    {
    public:
        using Super = BoundStatement;

        [[nodiscard]] BoundReturnStatement(
            const std::shared_ptr<const class SyntaxNode> Syntax,
            const std::shared_ptr<const class BoundExpression> Expression
        ) noexcept;

        BoundNodeKind Kind() const noexcept override;

        const std::shared_ptr<const class BoundExpression> Expression;
    };
} // namespace Mamba