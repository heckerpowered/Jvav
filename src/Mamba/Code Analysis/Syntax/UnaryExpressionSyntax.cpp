#include "UnaryExpressionSyntax.h"

using namespace Mamba;

UnaryExpressionSyntax::UnaryExpressionSyntax(
    const class SyntaxTree* SyntaxTree,
    const SyntaxToken* OperatorToken,
    const ExpressionSyntax* Operand
) noexcept :
    Super(SyntaxTree), OperatorToken(OperatorToken), Operand(Operand)
{
}

UnaryExpressionSyntax::~UnaryExpressionSyntax() noexcept
{
    delete Operand;
}

SyntaxKind UnaryExpressionSyntax::Kind() const noexcept
{
    return SyntaxKind::UnaryExpression;
}

std::size_t UnaryExpressionSyntax::ChildrenCount() const noexcept
{
    return 2;
}

const SyntaxNode* UnaryExpressionSyntax::ChildAt(std::size_t Index) const noexcept
{
    switch (Index)
    {
        case 0:
            return OperatorToken;
        case 1:
            return Operand;
        default:
            ReportChildrenAccessOutOfBounds(Index);
    }
}