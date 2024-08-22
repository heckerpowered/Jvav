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

std::vector<const SyntaxNode*> BreakStatementSyntax::Children() const noexcept
{
    return { Keyword };
}