#include "BoundVariableDeclaration.h"

using namespace Mamba;

BoundVariableDeclaration::BoundVariableDeclaration(
    const SyntaxNode* Syntax,
    const VariableSymbol* Variable,
    const BoundExpression* Initializer
) noexcept :
    Super(Syntax), Variable(Variable), Initializer(Initializer)
{
}

BoundVariableDeclaration::~BoundVariableDeclaration() noexcept
{
    delete Initializer;
}

BoundNodeKind BoundVariableDeclaration::Kind() const noexcept
{
    return BoundNodeKind::VariableDeclaration;
}
