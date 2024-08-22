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

std::vector<const SyntaxNode*> GlobalStatementSyntax::Children() const noexcept
{
    return { Statement };
}