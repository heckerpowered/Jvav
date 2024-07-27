#include "BoundUnaryExpression.h"

#include "BoundConstant.h"
#include "ConstantFolding.h"

using namespace Mamba;

BoundUnaryExpression::BoundUnaryExpression(
    const std::shared_ptr<const SyntaxNode> Syntax,
    const BoundUnaryOperator& Operator,
    const std::shared_ptr<const BoundExpression> Operand
) noexcept :
    Super(Syntax), Operator(Operator), Operand(Operand)
{
}

BoundNodeKind BoundUnaryExpression::Kind() const noexcept
{
    return BoundNodeKind::UnaryExpression;
}

std::shared_ptr<const TypeSymbol> BoundUnaryExpression::Type() const noexcept
{
    return Operand->Type();
}

NullableSharedPtr<const class BoundConstant> BoundUnaryExpression::ConstantValue() const noexcept
{
    return ConstantFolding::Fold(Operator, Operand);
}