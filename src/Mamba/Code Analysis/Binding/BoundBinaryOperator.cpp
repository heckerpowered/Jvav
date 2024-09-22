#include "BoundBinaryOperator.h"

#include <algorithm>

using namespace Mamba;

BoundBinaryOperator::BoundBinaryOperator(SyntaxKind Kind, BoundBinaryOperatorKind BoundKind, const TypeSymbol* Type) noexcept :
    BoundBinaryOperator(Kind, BoundKind, Type, Type, Type)
{
}

BoundBinaryOperator::BoundBinaryOperator(SyntaxKind Kind, BoundBinaryOperatorKind BoundKind, const TypeSymbol* OperandType, const TypeSymbol* ResultType) noexcept :
    BoundBinaryOperator(Kind, BoundKind, OperandType, OperandType, ResultType)
{
}

BoundBinaryOperator::BoundBinaryOperator(SyntaxKind Kind, BoundBinaryOperatorKind BoundKind, const TypeSymbol* LeftType, const TypeSymbol* RightType, const TypeSymbol* ResultType) noexcept :
    Kind(Kind), BoundKind(BoundKind), LeftType(LeftType), RightType(RightType), Type(ResultType)
{
}

BoundBinaryOperator BoundBinaryOperator::Operators[]{
    BoundBinaryOperator(SyntaxKind::PlusToken, BoundBinaryOperatorKind::Addition, &TypeSymbol::Int),
    BoundBinaryOperator(SyntaxKind::MinusToken, BoundBinaryOperatorKind::Subtraction, &TypeSymbol::Int),
    BoundBinaryOperator(SyntaxKind::StarToken, BoundBinaryOperatorKind::Multiplication, &TypeSymbol::Int),
    BoundBinaryOperator(SyntaxKind::SlashToken, BoundBinaryOperatorKind::Division, &TypeSymbol::Int),
    BoundBinaryOperator(SyntaxKind::AmpersandToken, BoundBinaryOperatorKind::BitwiseAnd, &TypeSymbol::Int),
    BoundBinaryOperator(SyntaxKind::PipeToken, BoundBinaryOperatorKind::BitwiseOr, &TypeSymbol::Int),
    BoundBinaryOperator(SyntaxKind::HatToken, BoundBinaryOperatorKind::BitwiseXor, &TypeSymbol::Int),
    BoundBinaryOperator(SyntaxKind::EqualsEqualsToken, BoundBinaryOperatorKind::Equals, &TypeSymbol::Int, &TypeSymbol::Bool),
    BoundBinaryOperator(SyntaxKind::BangEqualsToken, BoundBinaryOperatorKind::NotEquals, &TypeSymbol::Int, &TypeSymbol::Bool),
    BoundBinaryOperator(SyntaxKind::LessToken, BoundBinaryOperatorKind::Less, &TypeSymbol::Int, &TypeSymbol::Bool),
    BoundBinaryOperator(SyntaxKind::LessOrEqualsToken, BoundBinaryOperatorKind::LessOrEquals, &TypeSymbol::Int, &TypeSymbol::Bool),
    BoundBinaryOperator(SyntaxKind::GreaterToken, BoundBinaryOperatorKind::Greater, &TypeSymbol::Int, &TypeSymbol::Bool),
    BoundBinaryOperator(SyntaxKind::GreaterOrEqualsToken, BoundBinaryOperatorKind::GreaterOrEquals, &TypeSymbol::Int, &TypeSymbol::Bool),

    BoundBinaryOperator(SyntaxKind::AmpersandToken, BoundBinaryOperatorKind::BitwiseAnd, &TypeSymbol::Bool),
    BoundBinaryOperator(SyntaxKind::AmpersandAmpersandToken, BoundBinaryOperatorKind::LogicalAnd, &TypeSymbol::Bool),
    BoundBinaryOperator(SyntaxKind::PipeToken, BoundBinaryOperatorKind::BitwiseOr, &TypeSymbol::Bool),
    BoundBinaryOperator(SyntaxKind::PipePipeToken, BoundBinaryOperatorKind::LogicalOr, &TypeSymbol::Bool),
    BoundBinaryOperator(SyntaxKind::HatToken, BoundBinaryOperatorKind::BitwiseXor, &TypeSymbol::Bool),
    BoundBinaryOperator(SyntaxKind::EqualsEqualsToken, BoundBinaryOperatorKind::Equals, &TypeSymbol::Bool),
    BoundBinaryOperator(SyntaxKind::BangEqualsToken, BoundBinaryOperatorKind::NotEquals, &TypeSymbol::Bool),

    BoundBinaryOperator(SyntaxKind::PlusToken, BoundBinaryOperatorKind::Addition, &TypeSymbol::String),
    BoundBinaryOperator(SyntaxKind::EqualsEqualsToken, BoundBinaryOperatorKind::Equals, &TypeSymbol::String, &TypeSymbol::Bool),
    BoundBinaryOperator(SyntaxKind::BangEqualsToken, BoundBinaryOperatorKind::NotEquals, &TypeSymbol::String, &TypeSymbol::Bool),
};

NullablePointer<const BoundBinaryOperator> BoundBinaryOperator::Bind(SyntaxKind Kind, const TypeSymbol* LeftType, const TypeSymbol* RightType) noexcept
{
    auto Result = std::ranges::find_if(
        std::ranges::begin(Operators),
        std::ranges::end(Operators),
        [=](const BoundBinaryOperator& Operator) { return Operator.Kind == Kind && Operator.LeftType == LeftType && Operator.RightType == RightType; }
    );
    if (Result == std::ranges::end(Operators))
    {
        return {};
    }

    return &*Result;
}