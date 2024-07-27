#include "BoundCompoundAssignmentExpression.h"

using namespace Mamba;

BoundCompoundAssignmentExpression::BoundCompoundAssignmentExpression(
    const std::shared_ptr<const SyntaxNode> Syntax,
    const std::shared_ptr<const VariableSymbol> Variable,
    const BoundBinaryOperator& Operator,
    const std::shared_ptr<const BoundExpression> Expression
) noexcept :
    Super(Syntax), Variable(Variable), Operator(Operator), Expression(Expression)
{
}

BoundNodeKind BoundCompoundAssignmentExpression::Kind() const noexcept
{
    return BoundNodeKind::CompoundAssignmentExpression;
}

std::shared_ptr<const TypeSymbol> BoundCompoundAssignmentExpression::Type() const noexcept
{
    return Expression->Type();
}
