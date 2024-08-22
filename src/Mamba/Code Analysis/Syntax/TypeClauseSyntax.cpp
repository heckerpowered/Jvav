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

std::vector<const SyntaxNode*> TypeClauseSyntax::Children() const noexcept
{
    return { ColonToken, Identifier };
}