#pragma once

#include "ExpressionSyntax.h"
#include "StatementSyntax.h"
#include "SyntaxKind.h"

namespace Mamba
{
    class ExpressionStatementSyntax : public StatementSyntax
    {
    public:
        using Super = StatementSyntax;

        [[nodiscard]] ExpressionStatementSyntax(
            const class SyntaxTree* SyntaxTree,
            const ExpressionSyntax* Expression
        ) noexcept;

        ~ExpressionStatementSyntax() noexcept override;

        SyntaxKind Kind() const noexcept override;
        std::vector<const SyntaxNode*> Children() const noexcept override;

        const ExpressionSyntax* Expression;
    };
} // namespace Mamba