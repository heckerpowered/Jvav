#include "ParenthesizedExpressionSyntax.h"

namespace Mamba
{
    ParenthesizedExpressionSyntax::ParenthesizedExpressionSyntax(
        const std::shared_ptr<const class SyntaxTree> SyntaxTree,
        const std::shared_ptr<const class SyntaxToken> OpenParenthesisToken,
        const std::shared_ptr<const class ExpressionSyntax> Expression,
        const std::shared_ptr<const class SyntaxToken> CloseParenthesisToken
    ) noexcept :
        Super(SyntaxTree),
        OpenParenthesisToken(OpenParenthesisToken),
        Expression(Expression),
        CloseParenthesisToken(CloseParenthesisToken)
    {
    }

    SyntaxKind Mamba::ParenthesizedExpressionSyntax::Kind() const noexcept
    {
        return SyntaxKind::ParenthesizedExpression;
    }

    std::vector<std::shared_ptr<const class SyntaxNode>> ParenthesizedExpressionSyntax::Children() const noexcept
    {
        return { OpenParenthesisToken, Expression, CloseParenthesisToken };
    }
} // namespace Mamba