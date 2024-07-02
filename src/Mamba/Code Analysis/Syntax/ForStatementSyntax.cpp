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
        const std::shared_ptr<const class SyntaxToken> PrimarySemicolonToken,
        const std::shared_ptr<const class ExpressionSyntax> Condition,
        const std::shared_ptr<const class SyntaxToken> SecondarySemicolonToken,
        const std::shared_ptr<const class ExpressionSyntax> Expression,
        const std::shared_ptr<const class SyntaxToken> CloseParenthesisToken,
        const std::shared_ptr<const class StatementSyntax> Statement) noexcept :
        Super(SyntaxTree),
        Keyword(Keyword),
        OpenParenthesisToken(OpenParenthesisToken),
        InitStatement(InitStatement),
        PrimarySemicolonToken(PrimarySemicolonToken),
        Condition(Condition),
        SecondarySemicolonToken(SecondarySemicolonToken),
        Expression(Expression),
        CloseParenthesisToken(CloseParenthesisToken),
        Statement(Statement)
    {
    }

    SyntaxKind ForStatementSyntax::Kind() const noexcept
    {
        return SyntaxKind::ForStatement;
    }

    std::vector<std::shared_ptr<const class SyntaxNode>> ForStatementSyntax::Children() const noexcept
    {
        return { Keyword,   OpenParenthesisToken,    InitStatement, PrimarySemicolonToken,
                 Condition, SecondarySemicolonToken, Expression,    CloseParenthesisToken,
                 Statement };
    }
} // namespace Mamba