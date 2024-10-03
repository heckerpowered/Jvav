#include "BoundErrorExpression.h"
#include "TypeSymbol.h"

using namespace Mamba;

BoundErrorExpression::BoundErrorExpression(const SyntaxNode* Syntax) noexcept :
    Super(Syntax) {}

BoundNodeKind BoundErrorExpression::Kind() const noexcept
{
    return BoundNodeKind::ErrorExpression;
}

const TypeSymbol* BoundErrorExpression::Type() const noexcept
{
    return &TypeSymbol::Error;
}