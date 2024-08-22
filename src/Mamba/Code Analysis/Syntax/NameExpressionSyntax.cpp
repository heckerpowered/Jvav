#include "NameExpressionSyntax.h"

using namespace Mamba;

NameExpressionSyntax::NameExpressionSyntax(
    const class SyntaxTree* SyntaxTree,
    const SyntaxToken* IdentifierToken
) noexcept :
    Super(SyntaxTree), IdentifierToken(IdentifierToken)
{
}

SyntaxKind NameExpressionSyntax::Kind() const noexcept
{
    return SyntaxKind::NameExpression;
}

std::vector<const SyntaxNode*> NameExpressionSyntax::Children() const noexcept
{
    return { IdentifierToken };
}