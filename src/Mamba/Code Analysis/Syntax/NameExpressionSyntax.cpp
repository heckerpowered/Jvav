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

std::size_t NameExpressionSyntax::ChildrenCount() const noexcept
{
    return 1;
}

const SyntaxNode* NameExpressionSyntax::ChildAt(std::size_t Index) const noexcept
{
    if (Index != 0)
    {
        ReportChildrenAccessOutOfBounds(Index);
    }

    return IdentifierToken;
}
