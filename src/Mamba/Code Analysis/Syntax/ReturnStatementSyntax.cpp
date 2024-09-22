#include "ReturnStatementSyntax.h"

using namespace Mamba;

ReturnStatementSyntax::ReturnStatementSyntax(
    const class SyntaxTree* SyntaxTree,
    const SyntaxToken* ReturnKeyword,
    NullablePointer<const ExpressionSyntax> Expression
) noexcept :
    Super(SyntaxTree), ReturnKeyword(ReturnKeyword), Expression(Expression)
{
}

ReturnStatementSyntax::~ReturnStatementSyntax() noexcept
{
    delete Expression;
}

SyntaxKind ReturnStatementSyntax::Kind() const noexcept
{
    return SyntaxKind::ReturnStatement;
}

std::size_t ReturnStatementSyntax::ChildrenCount() const noexcept
{
    return 2;
}

const SyntaxNode* ReturnStatementSyntax::ChildAt(std::size_t Index) const noexcept
{
    switch (Index)
    {
        case 0:
            return ReturnKeyword;
        case 1:
            return Expression;
        default:
            ReportChildrenAccessOutOfBounds(Index);
    }
}