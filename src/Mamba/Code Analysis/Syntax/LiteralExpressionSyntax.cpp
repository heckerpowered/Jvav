#include "LiteralExpressionSyntax.h"

using namespace Mamba;

LiteralExpressionSyntax::LiteralExpressionSyntax(
    const SyntaxTree* SyntaxTree,
    const SyntaxToken* LiteralToken
) noexcept :
    Super(SyntaxTree), LiteralToken(LiteralToken), Value(LiteralToken->Value())
{
}

LiteralExpressionSyntax::LiteralExpressionSyntax(
    const class SyntaxTree* SyntaxTree,
    const SyntaxToken* LiteralToken,
    Literal Value
) noexcept :
    Super(SyntaxTree), LiteralToken(LiteralToken), Value(Value)
{
}

SyntaxKind LiteralExpressionSyntax::Kind() const noexcept
{
    return SyntaxKind::LiteralExpression;
}

std::vector<const SyntaxNode*> LiteralExpressionSyntax::Children() const noexcept
{
    return { LiteralToken };
}