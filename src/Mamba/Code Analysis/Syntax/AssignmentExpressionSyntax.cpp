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

std::vector<const SyntaxNode*> AssignmentExpressionSyntax::Children() const noexcept
{
    return { IdentifierToken, AssignmentToken, Expression };
}

SyntaxKind AssignmentExpressionSyntax::Kind() const noexcept
{
    return SyntaxKind::AssignmentExpression;
}