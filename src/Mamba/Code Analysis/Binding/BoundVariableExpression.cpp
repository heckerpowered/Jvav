#include "BoundVariableExpression.h"

#include "VariableSymbol.h"

using namespace Mamba;

BoundVariableExpression::BoundVariableExpression(const SyntaxNode* Syntax, const VariableSymbol* Variable) noexcept :
    Super(Syntax), Variable(Variable)
{
}

BoundNodeKind BoundVariableExpression::Kind() const noexcept
{
    return BoundNodeKind::VariableExpression;
}

const TypeSymbol* BoundVariableExpression::Type() const noexcept
{
    return Variable->Type;
}
