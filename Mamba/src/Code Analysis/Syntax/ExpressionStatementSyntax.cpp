#include "ExpressionStatementSyntax.h"

#include "ExpressionSyntax.h"

using namespace Mamba;

ExpressionStatementSyntax::ExpressionStatementSyntax(
    const class SyntaxTree* SyntaxTree,
    const ExpressionSyntax* Expression
) noexcept :
    Super(SyntaxTree), Expression(Expression)
{
}

ExpressionStatementSyntax::~ExpressionStatementSyntax() noexcept
{
    delete Expression;
}

SyntaxKind ExpressionStatementSyntax::Kind() const noexcept
{
    return SyntaxKind::ExpressionStatement;
}

std::size_t ExpressionStatementSyntax::ChildrenCount() const noexcept
{
    return 1;
}

const SyntaxNode* ExpressionStatementSyntax::ChildAt(std::size_t Index) const noexcept
{
    if (Index != 0)
    {
        ReportChildrenAccessOutOfBounds(Index);
    }

    return Expression;
}
