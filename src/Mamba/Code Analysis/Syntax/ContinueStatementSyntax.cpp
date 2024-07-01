#include "ContinueStatementSyntax.h"

#include "SyntaxToken.h"

namespace Mamba
{
    ContinueStatementSyntax::ContinueStatementSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                                     const std::shared_ptr<const class SyntaxToken> Keyword) noexcept :
        Super(SyntaxTree), Keyword(Keyword)
    {
    }

    SyntaxKind ContinueStatementSyntax::Kind() const noexcept
    {
        return SyntaxKind::ContinueStatement;
    }

    std::vector<std::shared_ptr<const class SyntaxNode>> ContinueStatementSyntax::Children() const noexcept
    {
        return { Keyword };
    }

} // namespace Mamba