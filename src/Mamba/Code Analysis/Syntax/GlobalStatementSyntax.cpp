#include "GlobalStatementSyntax.h"

#include "StatementSyntax.h"

namespace Mamba
{
    GlobalStatementSyntax::GlobalStatementSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                                 const std::shared_ptr<const class StatementSyntax> Statement) noexcept
        :
        Super(SyntaxTree), Statement(Statement)
    {
    }

    SyntaxKind GlobalStatementSyntax::Kind() const noexcept
    {
        return SyntaxKind::GlobalStatement;
    }

    std::vector<std::shared_ptr<const class SyntaxNode>> GlobalStatementSyntax::Children() const noexcept
    {
        return { Statement };
    }
} // namespace Mamba