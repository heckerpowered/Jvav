#include "BreakStatementSyntax.h"

#include "SyntaxToken.h"

using namespace Mamba;

BreakStatementSyntax::BreakStatementSyntax(const class SyntaxTree* SyntaxTree, const SyntaxToken* Keyword) noexcept :
    Super(SyntaxTree), Keyword(Keyword)
{
}

SyntaxKind BreakStatementSyntax::Kind() const noexcept
{
    return SyntaxKind::BreakStatement;
}

std::size_t BreakStatementSyntax::ChildrenCount() const noexcept
{
    return 1;
}

const SyntaxNode* BreakStatementSyntax::ChildAt(std::size_t Index) const noexcept
{
    if (Index != 0)
    {
        ReportChildrenAccessOutOfBounds(Index);
    }

    return Keyword;
}