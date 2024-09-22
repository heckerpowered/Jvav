#include "WhileStatementSyntax.h"

using namespace Mamba;

WhileStatementSyntax::WhileStatementSyntax(
    const class SyntaxTree* SyntaxTree,
    const SyntaxToken* WhileKeyword,
    const ExpressionSyntax* Condition,
    const StatementSyntax* Body
) noexcept :
    Super(SyntaxTree), WhileKeyword(WhileKeyword), Condition(Condition), Body(Body)
{
}

SyntaxKind WhileStatementSyntax::Kind() const noexcept
{
    return SyntaxKind::WhileStatement;
}

std::size_t WhileStatementSyntax::ChildrenCount() const noexcept
{
    return 3;
}

const SyntaxNode* WhileStatementSyntax::ChildAt(std::size_t Index) const noexcept
{
    switch (Index)
    {
        case 0:
            return WhileKeyword;
        case 1:
            return Condition;
        case 2:
            return Body;
        default:
            ReportChildrenAccessOutOfBounds(Index);
    }
}

WhileStatementSyntax::~WhileStatementSyntax() noexcept
{
    delete Condition;
    delete Body;
}