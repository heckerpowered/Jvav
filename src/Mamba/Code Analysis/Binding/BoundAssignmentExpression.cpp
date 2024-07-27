#include "BoundAssignmentExpression.h"

using namespace Mamba;

BoundAssignmentExpression::BoundAssignmentExpression(
    const std::shared_ptr<const class SyntaxNode> Syntax,
    const std::shared_ptr<const class VariableSymbol> Variable,
    const std::shared_ptr<const class BoundExpression> Expression
) noexcept :
    Super(Syntax), Variable(Variable), Expression(Expression)
{
}

BoundNodeKind BoundAssignmentExpression::Kind() const noexcept
{
    return BoundNodeKind::AssignmentExpression;
}

std::shared_ptr<const class TypeSymbol> BoundAssignmentExpression::Type() const noexcept
{
    return Expression->Type();
}