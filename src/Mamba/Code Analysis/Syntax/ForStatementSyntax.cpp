#include "ForStatementSyntax.h"

#include "ExpressionSyntax.h"
#include "StatementSyntax.h"

namespace Mamba
{
    Mamba::ForStatementSyntax::ForStatementSyntax(
        const std::shared_ptr<const class SyntaxTree> SyntaxTree,
        const std::shared_ptr<const class SyntaxToken> Keyword,
        const std::shared_ptr<const class SyntaxToken> OpenParenthesisToken,
        const std::shared_ptr<const class StatementSyntax> InitStatement,
        const std::shared_ptr<const class SyntaxToken> InitStatementColonToken,
        const std::shared_ptr<const class ExpressionSyntax> Condition,
        const std::shared_ptr<const class SyntaxToken> ConditionColonToken,
        const std::shared_ptr<const class ExpressionSyntax> Expression,
        const std::shared_ptr<const class SyntaxToken> CloseParenthesisToken,
        const std::shared_ptr<const class StatementSyntax> Body) noexcept :
        Super(SyntaxTree),
        Keyword(Keyword),
        OpenParenthesisToken(OpenParenthesisToken),
        InitStatement(InitStatement),
        InitStatementColonToken(InitStatementColonToken),
        Condition(Condition),
        ConditionColonToken(ConditionColonToken),
        Expression(Expression),
        CloseParenthesisToken(CloseParenthesisToken),
        Body(Body)
    {
    }

    SyntaxKind ForStatementSyntax::Kind() const noexcept
    {
        return SyntaxKind::ForStatement;
    }

    std::vector<std::shared_ptr<const class SyntaxNode>> ForStatementSyntax::Children() const noexcept
    {
        return { Keyword,   OpenParenthesisToken, InitStatement, InitStatementColonToken,
                 Condition, ConditionColonToken,  Expression,    CloseParenthesisToken,
                 Body };
    }
} // namespace Mamba