#include "BoundUnaryExpression.h"

#include "ConstantFolding.h"
#include "TypeSymbol.h"

using namespace Mamba;

BoundUnaryExpression::BoundUnaryExpression(
    const SyntaxNode* Syntax,
    const BoundUnaryOperator& Operator,
    const BoundExpression* Operand
) noexcept :
    Super(Syntax), Operator(Operator), Operand(Operand)
{
}

BoundUnaryExpression::~BoundUnaryExpression() noexcept
{
    delete Operand;
}

BoundNodeKind BoundUnaryExpression::Kind() const noexcept
{
    return BoundNodeKind::UnaryExpression;
}

const TypeSymbol* BoundUnaryExpression::Type() const noexcept
{
    return Operand->Type();
}

Constant BoundUnaryExpression::ConstantValue() const noexcept
{
    return ConstantFolding::Fold(Operator, Operand);
}