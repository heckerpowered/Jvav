#include "BoundFunctionDeclaration.h"

using namespace Mamba;

BoundFunctionDeclaration::BoundFunctionDeclaration(
    const SyntaxNode* Syntax,
    const BoundStatement* Body
) noexcept :
    Super(Syntax), Body(Body)
{
}

BoundFunctionDeclaration::~BoundFunctionDeclaration() noexcept
{
    delete Body;
}

BoundNodeKind BoundFunctionDeclaration::Kind() const noexcept
{
    return BoundNodeKind::FunctionDeclaration;
}
