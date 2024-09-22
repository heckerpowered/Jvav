#include "BoundBinaryExpression.h"

#include "ConstantFolding.h"

using namespace Mamba;

BoundBinaryExpression::BoundBinaryExpression(
    const SyntaxNode* Syntax,
    const BoundExpression* Left,
    const BoundBinaryOperator& Operator,
    const BoundExpression* Right
) noexcept :
    Super(Syntax), Left(Left), Operator(Operator), Right(Right)
{
}

BoundBinaryExpression::~BoundBinaryExpression() noexcept
{
    delete Left;
    delete Right;
}

BoundNodeKind BoundBinaryExpression::Kind() const noexcept
{
    return BoundNodeKind::BinaryExpression;
}

const TypeSymbol* BoundBinaryExpression::Type() const noexcept
{
    return Operator.Type;
}

Constant BoundBinaryExpression::ConstantValue() const noexcept
{
    return ConstantFolding::Fold(Left, Operator, Right);
}