#include "ConstantFolding.h"

#include "BoundBinaryOperatorKind.h"
#include "BoundConstant.h"
#include "Literal.h"
#include "TypeSymbol.h"

using namespace Mamba;

NullableSharedPtr<const class BoundConstant> ConstantFolding::Fold(
    const BoundUnaryOperator& Operator,
    const std::shared_ptr<const class BoundExpression> Operand
)
{
    if (!Operand->ConstantValue())
    {
        return nullptr;
    }

    switch (Operator.Kind)
    {
        case BoundUnaryOperatorKind::Identity:
            return Operand->ConstantValue();
        case BoundUnaryOperatorKind::Negation:
            return std::make_shared<const BoundConstant>(Literal::Negative(*Operand->ConstantValue()->Value));
        case BoundUnaryOperatorKind::LogicalNegation:
            return std::make_shared<const BoundConstant>(Literal::LogicalNegative(*Operand->ConstantValue()->Value));
        case BoundUnaryOperatorKind::OnesComplement:
            return std::make_shared<const BoundConstant>(Literal::OnesComplement(*Operand->ConstantValue()->Value));
    }
}

NullableSharedPtr<const class BoundConstant> ConstantFolding::Fold(
    const std::shared_ptr<const class BoundExpression>& Left,
    const BoundBinaryOperator& Operator,
    const std::shared_ptr<const class BoundExpression> Right
) noexcept
{
    const auto LeftConstant = Left->ConstantValue();
    const auto RightConstant = Right->ConstantValue();

    // Special case && and || because there are cases where only one
    // side needs to be known.

    if (Operator.Kind == BoundBinaryOperatorKind::LogicalAnd)
    {
        if ((LeftConstant && !(*LeftConstant)->BooleanValue) // Wrap
            || (RightConstant && !(*RightConstant)->BooleanValue))
        {
            return std::make_shared<const BoundConstant>(std::make_shared<const Literal>(false));
        }
    }

    if (Operator.Kind == BoundBinaryOperatorKind::LogicalOr)
    {
        if ((LeftConstant && (*LeftConstant)->BooleanValue) // Wrap
            || (RightConstant && (*RightConstant)->BooleanValue))
        {
            return std::make_shared<const BoundConstant>(std::make_shared<const Literal>(true));
        }
    }

    if (!LeftConstant || !RightConstant)
    {
        return {};
    }

    const auto LeftValue = LeftConstant->Value;
    const auto RightValue = RightConstant->Value;

    switch (Operator.Kind)
    {
        case BoundBinaryOperatorKind::Addition:
            if (Left->Type() == TypeSymbol::Int || Left->Type() == TypeSymbol::String)
            {
                return std::make_shared<const BoundConstant>(*LeftValue + *RightValue);
            }
            [[fallthrough]];
        case BoundBinaryOperatorKind::Subtraction:
            if (Left->Type() == TypeSymbol::Int)
            {
                return std::make_shared<const BoundConstant>(*LeftValue - *RightValue);
            }
            [[fallthrough]];
        case BoundBinaryOperatorKind::Multiplication:
            if (Left->Type() == TypeSymbol::Int)
            {
                return std::make_shared<const BoundConstant>(*LeftValue * *RightValue);
            }
            [[fallthrough]];
        case BoundBinaryOperatorKind::Division:
            if (Left->Type() == TypeSymbol::Int)
            {
                return std::make_shared<const BoundConstant>(*LeftValue / *RightValue);
            }
            [[fallthrough]];
        case BoundBinaryOperatorKind::LogicalAnd:
            return std::make_shared<const BoundConstant>(*LeftValue && *RightValue);
        case BoundBinaryOperatorKind::LogicalOr:
            return std::make_shared<const BoundConstant>(*LeftValue || *RightValue);
        case BoundBinaryOperatorKind::BitwiseAnd:
            return std::make_shared<const BoundConstant>(*LeftValue & *RightValue);
        case BoundBinaryOperatorKind::BitwiseOr:
            return std::make_shared<const BoundConstant>(*LeftValue | *RightValue);
        case BoundBinaryOperatorKind::BitwiseXor:
            return std::make_shared<const BoundConstant>(*LeftValue ^ *RightValue);
        case BoundBinaryOperatorKind::Equals:
            return std::make_shared<const BoundConstant>(*LeftValue == *RightValue);
        case BoundBinaryOperatorKind::NotEquals:
            return std::make_shared<const BoundConstant>(*LeftValue != *RightValue);
        case BoundBinaryOperatorKind::Less:
            return std::make_shared<const BoundConstant>(*LeftValue < *RightValue);
        case BoundBinaryOperatorKind::LessOrEquals:
            return std::make_shared<const BoundConstant>(*LeftValue <= *RightValue);
        case BoundBinaryOperatorKind::Greater:
            return std::make_shared<const BoundConstant>(*LeftValue > *RightValue);
        case BoundBinaryOperatorKind::GreaterOrEquals:
            return std::make_shared<const BoundConstant>(*LeftValue >= *RightValue);
    }

    std::unreachable();
}