#include "BoundVariableExpression.h"

using namespace Mamba;

BoundVariableExpression::BoundVariableExpression(
    const std::shared_ptr<const class SyntaxNode> Syntax,
    const std::shared_ptr<const class VariableSymbol> Variable
) noexcept :
    Super(Syntax), Variable(Variable)
{
}