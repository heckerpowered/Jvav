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

std::size_t ParenthesizedExpressionSyntax::ChildrenCount() const noexcept
{
    return 3;
}

const SyntaxNode* ParenthesizedExpressionSyntax::ChildAt(std::size_t Index) const noexcept
{
    switch (Index)
    {
        case 0:
            return OpenParenthesisToken;
        case 1:
            return Expression;
        case 2:
            return CloseParenthesisToken;
        default:
            ReportChildrenAccessOutOfBounds(Index);
    }
}
