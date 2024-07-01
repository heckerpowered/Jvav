#include "ElseClauseSyntax.h"

#include "StatementSyntax.h"
#include "SyntaxToken.h"

namespace Mamba
{
    ElseClauseSyntax::ElseClauseSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                       const std::shared_ptr<const class SyntaxToken> ElseKeyword,
                                       const std::shared_ptr<const class StatementSyntax> ElseStatement) noexcept :
        Super(SyntaxTree), ElseKeyword(ElseKeyword), ElseStatement(ElseStatement)
    {
    }

    SyntaxKind ElseClauseSyntax::Kind() const noexcept
    {
        return SyntaxKind::ElseClause;
    }

    std::vector<std::shared_ptr<const SyntaxNode>> ElseClauseSyntax::Children() const noexcept
    {
        return { ElseKeyword, ElseStatement };
    }
} // namespace Mamba