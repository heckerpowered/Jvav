#include "ContinueStatementSyntax.h"

#include "SyntaxToken.h"

using namespace Mamba;

ContinueStatementSyntax::ContinueStatementSyntax(const class SyntaxTree* SyntaxTree, const SyntaxToken* Keyword) noexcept :
    Super(SyntaxTree), Keyword(Keyword)
{
}

SyntaxKind ContinueStatementSyntax::Kind() const noexcept
{
    return SyntaxKind::ContinueStatement;
}

std::size_t ContinueStatementSyntax::ChildrenCount() const noexcept
{
    return 1;
}

const SyntaxNode* ContinueStatementSyntax::ChildAt(std::size_t Index) const noexcept
{
    if (Index != 0)
    {
        ReportChildrenAccessOutOfBounds(Index);
    }

    return Keyword;
}
