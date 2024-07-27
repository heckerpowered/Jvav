#include "BoundExpression.h"

#include "BoundConstant.h"

using namespace Mamba;

BoundExpression::BoundExpression(const std::shared_ptr<const class SyntaxNode> Syntax) noexcept : Super(Syntax) {}

NullableSharedPtr<const class BoundConstant> BoundExpression::ConstantValue() const noexcept
{
    return nullptr;
}