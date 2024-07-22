#include "NameExpressionSyntax.h"

namespace Mamba
{
    NameExpressionSyntax::NameExpressionSyntax(
        const std::shared_ptr<const class SyntaxTree> SyntaxTree,
        const std::shared_ptr<const class SyntaxToken> IdentifierToken
    ) noexcept :
        Super(SyntaxTree), IdentifierToken(IdentifierToken)
    {
    }

    SyntaxKind NameExpressionSyntax::Kind() const noexcept
    {
        return SyntaxKind::NameExpression;
    }

    std::vector<std::shared_ptr<const class SyntaxNode>> NameExpressionSyntax::Children() const noexcept
    {
        return { IdentifierToken };
    }
} // namespace Mamba