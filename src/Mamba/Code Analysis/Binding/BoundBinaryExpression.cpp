#include "BoundBinaryExpression.h"

#include "BoundConstant.h"
#include "ConstantFolding.h"

using namespace Mamba;

BoundBinaryExpression::BoundBinaryExpression(
    const std::shared_ptr<const SyntaxNode> Syntax,
    const std::shared_ptr<const BoundExpression> Left,
    const BoundBinaryOperator& Operator,
    const std::shared_ptr<const BoundExpression> Right
) noexcept :
    Super(Syntax), Left(Left), Operator(Operator), Right(Right)
{
}

BoundNodeKind BoundBinaryExpression::Kind() const noexcept
{
    return BoundNodeKind::BinaryExpression;
}

NullableSharedPtr<const class BoundConstant> BoundBinaryExpression::ConstantValue() const noexcept
{
    return ConstantFolding::Fold(Left, Operator, Right);
}