#include "BoundAssignmentExpression.h"

using namespace Mamba;

BoundAssignmentExpression::BoundAssignmentExpression(
    const SyntaxNode* Syntax,
    const VariableSymbol* Variable,
    const BoundExpression* Expression
) noexcept :
    Super(Syntax), Variable(Variable), Expression(Expression)
{
}

BoundAssignmentExpression::~BoundAssignmentExpression() noexcept
{
    delete Expression;
}

BoundNodeKind BoundAssignmentExpression::Kind() const noexcept
{
    return BoundNodeKind::AssignmentExpression;
}

const TypeSymbol* BoundAssignmentExpression::Type() const noexcept
{
    return Expression->Type();
}