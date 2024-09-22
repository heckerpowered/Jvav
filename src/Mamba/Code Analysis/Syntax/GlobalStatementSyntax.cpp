#include "GlobalStatementSyntax.h"

#include "StatementSyntax.h"

using namespace Mamba;

GlobalStatementSyntax::GlobalStatementSyntax(
    const class SyntaxTree* SyntaxTree,
    const StatementSyntax* Statement
) noexcept :
    Super(SyntaxTree), Statement(Statement)
{
}

GlobalStatementSyntax::~GlobalStatementSyntax() noexcept
{
    delete Statement;
}

SyntaxKind GlobalStatementSyntax::Kind() const noexcept
{
    return SyntaxKind::GlobalStatement;
}

std::size_t GlobalStatementSyntax::ChildrenCount() const noexcept
{
    return 1;
}

const SyntaxNode* GlobalStatementSyntax::ChildAt(std::size_t Index) const noexcept
{
    if (Index != 0)
    {
        ReportChildrenAccessOutOfBounds(Index);
    }

    return Statement;
}
