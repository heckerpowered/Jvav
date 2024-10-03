#include "DoWhileStatementSyntax.h"

using namespace Mamba;

DoWhileStatementSyntax::DoWhileStatementSyntax(
    const class SyntaxTree* SyntaxTree,
    const SyntaxToken* DoKeyword,
    const StatementSyntax* Body,
    const SyntaxToken* WhileKeyword,
    const ExpressionSyntax* Condition
) noexcept :
    Super(SyntaxTree), DoKeyword(DoKeyword), Body(Body), WhileKeyword(WhileKeyword), Condition(Condition)
{
}

DoWhileStatementSyntax::~DoWhileStatementSyntax() noexcept
{
    delete Body;
    delete Condition;
}

SyntaxKind DoWhileStatementSyntax::Kind() const noexcept
{
    return SyntaxKind::DoWhileStatement;
}

std::size_t DoWhileStatementSyntax::ChildrenCount() const noexcept
{
    return 4;
}

const SyntaxNode* Mamba::DoWhileStatementSyntax::ChildAt(std::size_t Index) const noexcept
{
    switch (Index)
    {
        case 0:
            return DoKeyword;
        case 1:
            return Body;
        case 2:
            return WhileKeyword;
        case 3:
            return Condition;
        default:
            ReportChildrenAccessOutOfBounds(Index);
    }
}