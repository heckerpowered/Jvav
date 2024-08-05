#include "BoundVariableExpression.h"

#include "VariableSymbol.h"

using namespace Mamba;

BoundVariableExpression::BoundVariableExpression(
    const std::shared_ptr<const class SyntaxNode> Syntax,
    const std::shared_ptr<const class VariableSymbol> Variable
) noexcept :
    Super(Syntax), Variable(Variable)
{
}

BoundNodeKind BoundVariableExpression::Kind() const noexcept
{
    return BoundNodeKind::VariableExpression;
}

std::shared_ptr<const TypeSymbol> BoundVariableExpression::Type() const noexcept
{
    return Variable->Type;
}
