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

std::vector<const SyntaxNode*> DoWhileStatementSyntax::Children() const noexcept
{
    return { DoKeyword, Body, WhileKeyword, Condition };
}