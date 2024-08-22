#include "BinaryExpressionSyntax.h"

using namespace Mamba;

BinaryExpressionSyntax::BinaryExpressionSyntax(
    const class SyntaxTree* SyntaxTree,
    const ExpressionSyntax* Left,
    const SyntaxToken* OperatorToken,
    const ExpressionSyntax* Right
) noexcept :
    Super(SyntaxTree), Left(Left), OperatorToken(OperatorToken), Right(Right)
{
}

BinaryExpressionSyntax::~BinaryExpressionSyntax() noexcept
{
    delete Left;
    delete Right;
}

SyntaxKind BinaryExpressionSyntax::Kind() const noexcept
{
    return SyntaxKind::BinaryExpression;
}

std::vector<const SyntaxNode*> BinaryExpressionSyntax::Children() const noexcept
{
    return { Left, OperatorToken, Right };
}