#pragma once

#include "StatementSyntax.h"
#include "SyntaxToken.h"
#include "SyntaxTree.h"
#include <memory>

namespace Mamba
{
    // Condintionally executes a statement repeatedly, while the statement does not need to manage the loop condition
    // Syntax: `for (init-statement; condition; expression) statement`
    class ForStatementSyntax : public StatementSyntax
    {
    public:
        using Super = StatementSyntax;

        [[nodiscard]] ForStatementSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                         const std::shared_ptr<const class SyntaxToken> Keyword,
                                         const std::shared_ptr<const class SyntaxToken> OpenParenthesisToken,
                                         const std::shared_ptr<const class StatementSyntax> InitStatement,
                                         const std::shared_ptr<const class SyntaxToken> InitStatementColonToken,
                                         const std::shared_ptr<const class ExpressionSyntax> Condition,
                                         const std::shared_ptr<const class SyntaxToken> ConditionColonToken,
                                         const std::shared_ptr<const class ExpressionSyntax> Expression,
                                         const std::shared_ptr<const class SyntaxToken> CloseParenthesisToken,
                                         const std::shared_ptr<const class StatementSyntax> Body) noexcept;

        SyntaxKind Kind() const noexcept override;
        std::vector<std::shared_ptr<const class SyntaxNode>> Children() const noexcept override;

        const std::shared_ptr<const class SyntaxToken> Keyword;
        const std::shared_ptr<const class SyntaxToken> OpenParenthesisToken;
        const std::shared_ptr<const class StatementSyntax> InitStatement;
        const std::shared_ptr<const class SyntaxToken> InitStatementColonToken;
        const std::shared_ptr<const class ExpressionSyntax> Condition;
        const std::shared_ptr<const class SyntaxToken> ConditionColonToken;
        const std::shared_ptr<const class ExpressionSyntax> Expression;
        const std::shared_ptr<const class SyntaxToken> CloseParenthesisToken;
        const std::shared_ptr<const class StatementSyntax> Body;
    };
} // namespace Mamba