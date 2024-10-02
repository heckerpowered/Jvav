#include "IfStatementSyntax.h"

using namespace Mamba;

IfStatementSyntax::IfStatementSyntax(
    const class SyntaxTree* SyntaxTree,
    const SyntaxToken* IfKeyword,
    const ExpressionSyntax* Condition,
    const StatementSyntax* ThenStatement,
    NullablePointer<const ElseClauseSyntax> ElseClause
) noexcept :
    Super(SyntaxTree),
    IfKeyword(IfKeyword),
    Condition(Condition),
    ThenStatement(ThenStatement),
    ElseClause(ElseClause)
{
}

IfStatementSyntax::~IfStatementSyntax() noexcept
{
    delete Condition;
    delete ThenStatement;
    delete ElseClause;
}

SyntaxKind IfStatementSyntax::Kind() const noexcept
{
    return SyntaxKind::IfStatement;
}

std::size_t IfStatementSyntax::ChildrenCount() const noexcept
{
    return ElseClause ? 4 : 3;
}

const SyntaxNode* IfStatementSyntax::ChildAt(std::size_t Index) const noexcept
{
    switch (Index)
    {
        case 0:
            return IfKeyword;
        case 1:
            return Condition;
        case 2:
            return ThenStatement;
        case 3:
            if (ElseClause)
            {
                return ElseClause;
            }
            [[fallthrough]];
        default:
            ReportChildrenAccessOutOfBounds(Index);
    }
}