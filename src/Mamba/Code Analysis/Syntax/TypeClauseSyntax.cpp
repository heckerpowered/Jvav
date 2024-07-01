#include "TypeClauseSyntax.h"

#include "SyntaxToken.h"

namespace Mamba
{
    TypeClauseSyntax::TypeClauseSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                       const std::shared_ptr<const class SyntaxToken> ColonToken,
                                       const std::shared_ptr<const class SyntaxToken> Identifier) noexcept :
        Super(SyntaxTree), ColonToken(ColonToken), Identifier(Identifier)
    {
    }

    SyntaxKind TypeClauseSyntax::Kind() const noexcept
    {
        return SyntaxKind::TypeClause;
    }

    std::vector<std::shared_ptr<const class SyntaxNode>> TypeClauseSyntax::Children() const noexcept
    {
        return { ColonToken, Identifier };
    }
} // namespace Mamba