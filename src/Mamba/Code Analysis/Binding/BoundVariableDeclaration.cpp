#include "BoundVariableDeclaration.h"

using namespace Mamba;

BoundVariableDeclaration::BoundVariableDeclaration(
    const std::shared_ptr<const SyntaxNode> Syntax,
    const std::shared_ptr<const VariableSymbol> Variable,
    const std::shared_ptr<const BoundExpression> Initializer
) noexcept :
    Super(Syntax), Variable(Variable), Initializer(Initializer)
{
}

BoundNodeKind BoundVariableDeclaration::Kind() const noexcept
{
    return BoundNodeKind::VariableDeclaration;
}
