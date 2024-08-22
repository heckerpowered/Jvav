#pragma once

#include "ElseClauseSyntax.h"
#include "ExpressionSyntax.h"
#include "StatementSyntax.h"
#include "SyntaxToken.h"

namespace Mamba
{
    class IfStatementSyntax : public StatementSyntax
    {
    public:
        using Super = StatementSyntax;

        [[nodiscard]] IfStatementSyntax(
            const class SyntaxTree* SyntaxTree,
            const SyntaxToken* IfKeyword,
            const ExpressionSyntax* Condition,
            const StatementSyntax* ThenStatement,
            NullablePointer<const ElseClauseSyntax> ElseClause
        ) noexcept;

        ~IfStatementSyntax() noexcept override;

        SyntaxKind Kind() const noexcept override;
        std::vector<const SyntaxNode*> Children() const noexcept override;

        const SyntaxToken* IfKeyword;
        const ExpressionSyntax* Condition;
        const StatementSyntax* ThenStatement;
        NullablePointer<const ElseClauseSyntax> ElseClause;
    };
} // namespace Mamba