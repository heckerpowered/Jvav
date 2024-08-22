#pragma once

#include "ExpressionSyntax.h"
#include "StatementSyntax.h"
#include "SyntaxToken.h"
#include "SyntaxTree.h"

namespace Mamba
{
    // Condintionally executes a statement repeatedly, while the statement does not need to manage the loop condition
    // Syntax: `for (init-statement; condition; expression) statement`
    class ForStatementSyntax : public StatementSyntax
    {
    public:
        using Super = StatementSyntax;

        [[nodiscard]] ForStatementSyntax(
            const class SyntaxTree* SyntaxTree,
            const SyntaxToken* Keyword,
            const SyntaxToken* OpenParenthesisToken,
            const StatementSyntax* InitStatement,
            const SyntaxToken* InitStatementColonToken,
            const ExpressionSyntax* Condition,
            const SyntaxToken* ConditionColonToken,
            const ExpressionSyntax* Expression,
            const SyntaxToken* CloseParenthesisToken,
            const StatementSyntax* Body
        ) noexcept;

        ~ForStatementSyntax() noexcept override;

        SyntaxKind Kind() const noexcept override;
        std::vector<const SyntaxNode*> Children() const noexcept override;

        const SyntaxToken* Keyword;
        const SyntaxToken* OpenParenthesisToken;
        const StatementSyntax* InitStatement;
        const SyntaxToken* InitStatementColonToken;
        const ExpressionSyntax* Condition;
        const SyntaxToken* ConditionColonToken;
        const ExpressionSyntax* Expression;
        const SyntaxToken* CloseParenthesisToken;
        const StatementSyntax* Body;
    };
} // namespace Mamba