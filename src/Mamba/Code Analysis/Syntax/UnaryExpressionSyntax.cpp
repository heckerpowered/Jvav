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

std::vector<const SyntaxNode*> UnaryExpressionSyntax::Children() const noexcept
{
    return { OperatorToken, Operand };
}