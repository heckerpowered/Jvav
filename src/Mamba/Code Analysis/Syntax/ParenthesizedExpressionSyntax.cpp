#include "ParenthesizedExpressionSyntax.h"

using namespace Mamba;

ParenthesizedExpressionSyntax::ParenthesizedExpressionSyntax(
    const class SyntaxTree* SyntaxTree,
    const SyntaxToken* OpenParenthesisToken,
    const ExpressionSyntax* Expression,
    const SyntaxToken* CloseParenthesisToken
) noexcept :
    Super(SyntaxTree),
    OpenParenthesisToken(OpenParenthesisToken),
    Expression(Expression),
    CloseParenthesisToken(CloseParenthesisToken)
{
}

ParenthesizedExpressionSyntax::~ParenthesizedExpressionSyntax() noexcept
{
    delete Expression;
}

SyntaxKind ParenthesizedExpressionSyntax::Kind() const noexcept
{
    return SyntaxKind::ParenthesizedExpression;
}

std::vector<const SyntaxNode*> ParenthesizedExpressionSyntax::Children() const noexcept
{
    return { OpenParenthesisToken, Expression, CloseParenthesisToken };
}