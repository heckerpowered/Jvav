#include "ElseClauseSyntax.h"

using namespace Mamba;

ElseClauseSyntax::ElseClauseSyntax(
    const class SyntaxTree* SyntaxTree,
    const SyntaxToken* ElseKeyword,
    const StatementSyntax* ElseStatement
) noexcept :
    Super(SyntaxTree), ElseKeyword(ElseKeyword), ElseStatement(ElseStatement)
{
}

ElseClauseSyntax::~ElseClauseSyntax() noexcept
{
    delete ElseStatement;
}

SyntaxKind ElseClauseSyntax::Kind() const noexcept
{
    return SyntaxKind::ElseClause;
}

std::vector<const SyntaxNode*> ElseClauseSyntax::Children() const noexcept
{
    return { ElseKeyword, ElseStatement };
}