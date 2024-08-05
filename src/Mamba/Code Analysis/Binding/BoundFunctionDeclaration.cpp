#include "BoundFunctionDeclaration.h"

using namespace Mamba;

Mamba::BoundFunctionDeclaration::BoundFunctionDeclaration(
    const std::shared_ptr<const class SyntaxNode> Syntax,
    const std::shared_ptr<const class BoundStatement> Body,
    const std::shared_ptr<const class BoundScope> Scope
) noexcept :
    Super(Syntax), Body(Body), Scope(Scope)
{
}

BoundNodeKind BoundFunctionDeclaration::Kind() const noexcept
{
    return BoundNodeKind::FunctionDeclaration;
}
