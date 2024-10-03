#include "BoundCompoundAssignmentExpression.h"

using namespace Mamba;

BoundCompoundAssignmentExpression::BoundCompoundAssignmentExpression(
    const SyntaxNode* Syntax,
    const VariableSymbol* Variable,
    const BoundBinaryOperator& Operator,
    const BoundExpression* Expression
) noexcept :
    Super(Syntax), Variable(Variable), Operator(Operator), Expression(Expression)
{
}

BoundCompoundAssignmentExpression::~BoundCompoundAssignmentExpression() noexcept
{
    delete Expression;
}

BoundNodeKind BoundCompoundAssignmentExpression::Kind() const noexcept
{
    return BoundNodeKind::CompoundAssignmentExpression;
}

const TypeSymbol* BoundCompoundAssignmentExpression::Type() const noexcept
{
    return Expression->Type();
}
