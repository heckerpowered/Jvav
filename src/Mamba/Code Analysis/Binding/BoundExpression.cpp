#include "BoundExpression.h"

using namespace Mamba;

BoundExpression::BoundExpression(const std::shared_ptr<const class SyntaxNode> Syntax) noexcept :
    Super(Syntax) {}

Constant BoundExpression::ConstantValue() const noexcept
{
    return {};
}