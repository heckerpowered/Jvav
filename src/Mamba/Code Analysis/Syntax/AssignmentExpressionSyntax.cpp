#include "AssignmentExpressionSyntax.h"

using namespace Mamba;

AssignmentExpressionSyntax::AssignmentExpressionSyntax(
    const class SyntaxTree* SyntaxTree,
    const SyntaxToken* IdentifierToken,
    const SyntaxToken* AssignmentToken,
    const ExpressionSyntax* Expression
) noexcept :
    Super(SyntaxTree), IdentifierToken(IdentifierToken), AssignmentToken(AssignmentToken), Expression(Expression)
{
}

AssignmentExpressionSyntax::~AssignmentExpressionSyntax() noexcept
{
    delete Expression;
}

SyntaxKind AssignmentExpressionSyntax::Kind() const noexcept
{
    return SyntaxKind::AssignmentExpression;
}

std::size_t AssignmentExpressionSyntax::ChildrenCount() const noexcept
{
    return 3;
}

const SyntaxNode* AssignmentExpressionSyntax::ChildAt(std::size_t Index) const noexcept
{
    switch (Index)
    {
        case 0:
            return IdentifierToken;
        case 1:
            return AssignmentToken;
        case 2:
            return Expression;
        default:
            ReportChildrenAccessOutOfBounds(Index);
    }
}
