#pragma once

#include <memory>

#include "BoundNodeKind.h"
#include "BoundStatement.h"
#include "MambaCore.h"

namespace Mamba
{
    class BoundIfStatement : public BoundStatement
    {
    public:
        using Super = BoundStatement;

        [[nodiscard]] BoundIfStatement(
            const std::shared_ptr<const class SyntaxNode> Syntax,
            const std::shared_ptr<const class BoundExpression> Condition,
            const std::shared_ptr<const class BoundStatement> ThenStatement,
            const NullableSharedPtr<const class BoundStatement> ElseStatement
        ) noexcept;

        BoundNodeKind Kind() const noexcept override;

        const std::shared_ptr<const class BoundExpression> Condition;
        const std::shared_ptr<const class BoundStatement> ThenStatement;
        const NullableSharedPtr<const class BoundStatement> ElseStatement;
    };
} // namespace Mamba