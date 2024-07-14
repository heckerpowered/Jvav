#include "BreakStatementSyntax.h"

#include "SyntaxToken.h"

namespace Mamba
{
    BreakStatementSyntax::BreakStatementSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                               const std::shared_ptr<const class SyntaxToken> Keyword) noexcept :
        Super(SyntaxTree), Keyword(Keyword)
    {
    }

    SyntaxKind BreakStatementSyntax::Kind() const noexcept
    {
        return SyntaxKind::BreakStatement;
    }

    std::vector<std::shared_ptr<const class SyntaxNode>> BreakStatementSyntax::Children() const noexcept
    {
        return { Keyword };
    }
} // namespace Mamba