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

std::vector<const SyntaxNode*> ContinueStatementSyntax::Children() const noexcept
{
    return { Keyword };
}