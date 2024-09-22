#include "BoundExpression.h"

using namespace Mamba;

BoundExpression::BoundExpression(const SyntaxNode* Syntax) noexcept :
    Super(Syntax) {}

Constant BoundExpression::ConstantValue() const noexcept
{
    return {};
}