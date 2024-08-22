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

WhileStatementSyntax::~WhileStatementSyntax() noexcept
{
}

std::vector<const SyntaxNode*> WhileStatementSyntax::Children() const noexcept
{
    return { WhileKeyword, Condition, Body };
}