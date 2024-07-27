#include "BoundVariableDeclaration.h"

using namespace Mamba;

BoundVariableDeclaration::BoundVariableDeclaration(
    const std::shared_ptr<const class SyntaxNode> Syntax,
    const std::shared_ptr<const class VariableSymbol> Variable,
    const std::shared_ptr<const class BoundExpression> Initializer
) noexcept :
    Super(Syntax), Variable(Variable), Initializer(Initializer)
{
}