#include "ElseClauseSyntax.h"

using namespace Mamba;

ElseClauseSyntax::ElseClauseSyntax(
    const class SyntaxTree* SyntaxTree,
    const SyntaxToken* ElseKeyword,
    NullablePointer<const StatementSyntax> ElseStatement
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

std::size_t ElseClauseSyntax::ChildrenCount() const noexcept
{
    return 2;
}

const SyntaxNode* ElseClauseSyntax::ChildAt(std::size_t Index) const noexcept
{
    switch (Index)
    {
        case 0:
            return ElseKeyword;
        case 1:
            return ElseStatement;
        default:
            ReportChildrenAccessOutOfBounds(Index);
    }
}