#include "ConstantFolding.h"

using namespace Mamba;

Constant ConstantFolding::Fold(
    const BoundUnaryOperator& Operator,
    const BoundExpression* Operand
)
{
    if (!Operand->ConstantValue().IsValid())
    {
        return {};
    }

    switch (Operator.BoundKind)
    {
        case BoundUnaryOperatorKind::Identity:
            return Operand->ConstantValue();
        case BoundUnaryOperatorKind::Negation:
            return -Operand->ConstantValue();
        case BoundUnaryOperatorKind::LogicalNegation:
            return !Operand->ConstantValue();
        case BoundUnaryOperatorKind::OnesComplement:
            return ~Operand->ConstantValue();
        default:
            return {};
    }
}

Constant ConstantFolding::Fold(
    const BoundExpression* Left,
    const BoundBinaryOperator& Operator,
    const BoundExpression* Right
) noexcept
{
    auto LeftConstant = Left->ConstantValue();
    auto RightConstant = Right->ConstantValue();

    // Special case && and || because there are cases where only one
    // side needs to be known.

    if (Operator.OperatorKind == BoundBinaryOperatorKind::LogicalAnd)
    {
        if ((LeftConstant.IsValid() && !LeftConstant.Get<bool>()) ||
            (RightConstant.IsValid() && !RightConstant.Get<bool>()))
        {
            return false;
        }
    }

    if (Operator.OperatorKind == BoundBinaryOperatorKind::LogicalOr)
    {
        if ((LeftConstant.IsValid() && LeftConstant.Get<bool>()) ||
            (RightConstant.IsValid() && RightConstant.Get<bool>()))
        {
            return true;
        }
    }

    if (!LeftConstant.IsValid() || !RightConstant.IsValid())
    {
        return {};
    }

    switch (Operator.OperatorKind)
    {
        case BoundBinaryOperatorKind::Addition:
            return LeftConstant + RightConstant;
        case BoundBinaryOperatorKind::Subtraction:
            return LeftConstant - RightConstant;
        case BoundBinaryOperatorKind::Multiplication:
            return LeftConstant * RightConstant;
        case BoundBinaryOperatorKind::Division:
            return LeftConstant / RightConstant;
        case BoundBinaryOperatorKind::LogicalAnd:
            return LeftConstant && RightConstant;
        case BoundBinaryOperatorKind::LogicalOr:
            return LeftConstant || RightConstant;
        case BoundBinaryOperatorKind::BitwiseAnd:
            return LeftConstant & RightConstant;
        case BoundBinaryOperatorKind::BitwiseOr:
            return LeftConstant | RightConstant;
        case BoundBinaryOperatorKind::BitwiseXor:
            return LeftConstant ^ RightConstant;
        case BoundBinaryOperatorKind::Equals:
            return LeftConstant == RightConstant;
        case BoundBinaryOperatorKind::NotEquals:
            return LeftConstant != RightConstant;
        case BoundBinaryOperatorKind::Less:
            return LeftConstant < RightConstant;
        case BoundBinaryOperatorKind::LessOrEquals:
            return LeftConstant <= RightConstant;
        case BoundBinaryOperatorKind::Greater:
            return LeftConstant > RightConstant;
        case BoundBinaryOperatorKind::GreaterOrEquals:
            return LeftConstant >= RightConstant;
    }

    return {};
}