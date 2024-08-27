#pragma once

#include "BoundExpression.h"
#include "BoundNodeKind.h"
#include "BoundStatement.h"
#include "MambaCore.h"

namespace Mamba
{
    class BoundIfStatement : public BoundStatement
    {
    public:
        using Super = BoundStatement;

        [[nodiscard]] BoundIfStatement(const SyntaxNode* Syntax, const BoundExpression* Condition, const BoundStatement* ThenStatement, NullablePointer<const BoundStatement> ElseStatement) noexcept;
        ~BoundIfStatement() noexcept override;

        BoundNodeKind Kind() const noexcept override;

        const BoundExpression* Condition;
        const BoundStatement* ThenStatement;
        NullablePointer<const BoundStatement> ElseStatement;
    };
} // namespace Mamba