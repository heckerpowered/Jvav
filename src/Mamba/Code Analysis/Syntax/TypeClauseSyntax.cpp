#include "TypeClauseSyntax.h"

#include "SyntaxToken.h"

using namespace Mamba;

TypeClauseSyntax::TypeClauseSyntax(
    const class SyntaxTree* SyntaxTree,
    const SyntaxToken* ColonToken,
    const SyntaxToken* Identifier
) noexcept :
    Super(SyntaxTree), ColonToken(ColonToken), Identifier(Identifier)
{
}

SyntaxKind TypeClauseSyntax::Kind() const noexcept
{
    return SyntaxKind::TypeClause;
}

std::size_t TypeClauseSyntax::ChildrenCount() const noexcept
{
    return 2;
}

const SyntaxNode* TypeClauseSyntax::ChildAt(std::size_t Index) const noexcept
{
    switch (Index)
    {
        case 0:
            return ColonToken;
        case 1:
            return Identifier;
        default:
            ReportChildrenAccessOutOfBounds(Index);
    }
}