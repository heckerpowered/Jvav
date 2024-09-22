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

std::size_t BinaryExpressionSyntax::ChildrenCount() const noexcept
{
    return 3;
}

const SyntaxNode* BinaryExpressionSyntax::ChildAt(std::size_t Index) const noexcept
{
    switch (Index)
    {
        case 0:
            return Left;
        case 1:
            return OperatorToken;
        case 2:
            return Right;
        default:
            ReportChildrenAccessOutOfBounds(Index);
    }
}
