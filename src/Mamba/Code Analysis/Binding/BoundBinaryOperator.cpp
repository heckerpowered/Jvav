#include "BoundBinaryOperator.h"

#include <algorithm>

#include "SyntaxKind.h"
#include "TypeSymbol.h"

using namespace Mamba;

BoundBinaryOperator::BoundBinaryOperator(
    const enum SyntaxKind SyntaxKind,
    const BoundBinaryOperatorKind Kind,
    const std::shared_ptr<const class TypeSymbol> Type
) noexcept :
    BoundBinaryOperator(SyntaxKind, Kind, Type, Type, Type)
{
}

BoundBinaryOperator::BoundBinaryOperator(
    const enum SyntaxKind SyntaxKind,
    const BoundBinaryOperatorKind Kind,
    const std::shared_ptr<const class TypeSymbol> OperandType,
    const std::shared_ptr<const class TypeSymbol> ResultType
) noexcept :
    BoundBinaryOperator(SyntaxKind, Kind, OperandType, OperandType, ResultType)
{
}

BoundBinaryOperator::BoundBinaryOperator(
    const enum SyntaxKind SyntaxKind,
    const BoundBinaryOperatorKind Kind,
    const std::shared_ptr<const class TypeSymbol> LeftType,
    const std::shared_ptr<const class TypeSymbol> RightType,
    const std::shared_ptr<const class TypeSymbol> ResultType
) noexcept :
    SyntaxKind(SyntaxKind), Kind(Kind), LeftType(LeftType), RightType(RightType), Type(ResultType)
{
}

BoundBinaryOperator BoundBinaryOperator::Operators[]{
    BoundBinaryOperator(SyntaxKind::PlusToken, BoundBinaryOperatorKind::Addition, TypeSymbol::Int),
    BoundBinaryOperator(SyntaxKind::MinusToken, BoundBinaryOperatorKind::Subtraction, TypeSymbol::Int),
    BoundBinaryOperator(SyntaxKind::StarToken, BoundBinaryOperatorKind::Multiplication, TypeSymbol::Int),
    BoundBinaryOperator(SyntaxKind::SlashToken, BoundBinaryOperatorKind::Division, TypeSymbol::Int),
    BoundBinaryOperator(SyntaxKind::AmpersandToken, BoundBinaryOperatorKind::BitwiseAnd, TypeSymbol::Int),
    BoundBinaryOperator(SyntaxKind::PipeToken, BoundBinaryOperatorKind::BitwiseOr, TypeSymbol::Int),
    BoundBinaryOperator(SyntaxKind::HatToken, BoundBinaryOperatorKind::BitwiseXor, TypeSymbol::Int),
    BoundBinaryOperator(
        SyntaxKind::EqualsEqualsToken,
        BoundBinaryOperatorKind::Equals,
        TypeSymbol::Int,
        TypeSymbol::Bool
    ),
    BoundBinaryOperator(
        SyntaxKind::BangEqualsToken,
        BoundBinaryOperatorKind::NotEquals,
        TypeSymbol::Int,
        TypeSymbol::Bool
    ),
    BoundBinaryOperator(SyntaxKind::LessToken, BoundBinaryOperatorKind::Less, TypeSymbol::Int, TypeSymbol::Bool),
    BoundBinaryOperator(
        SyntaxKind::LessOrEqualsToken,
        BoundBinaryOperatorKind::LessOrEquals,
        TypeSymbol::Int,
        TypeSymbol::Bool
    ),
    BoundBinaryOperator(SyntaxKind::GreaterToken, BoundBinaryOperatorKind::Greater, TypeSymbol::Int, TypeSymbol::Bool),
    BoundBinaryOperator(
        SyntaxKind::GreaterOrEqualsToken,
        BoundBinaryOperatorKind::GreaterOrEquals,
        TypeSymbol::Int,
        TypeSymbol::Bool
    ),

    BoundBinaryOperator(SyntaxKind::AmpersandToken, BoundBinaryOperatorKind::BitwiseAnd, TypeSymbol::Bool),
    BoundBinaryOperator(SyntaxKind::AmpersandAmpersandToken, BoundBinaryOperatorKind::LogicalAnd, TypeSymbol::Bool),
    BoundBinaryOperator(SyntaxKind::PipeToken, BoundBinaryOperatorKind::BitwiseOr, TypeSymbol::Bool),
    BoundBinaryOperator(SyntaxKind::PipePipeToken, BoundBinaryOperatorKind::LogicalOr, TypeSymbol::Bool),
    BoundBinaryOperator(SyntaxKind::HatToken, BoundBinaryOperatorKind::BitwiseXor, TypeSymbol::Bool),
    BoundBinaryOperator(SyntaxKind::EqualsEqualsToken, BoundBinaryOperatorKind::Equals, TypeSymbol::Bool),
    BoundBinaryOperator(SyntaxKind::BangEqualsToken, BoundBinaryOperatorKind::NotEquals, TypeSymbol::Bool),

    BoundBinaryOperator(SyntaxKind::PlusToken, BoundBinaryOperatorKind::Addition, TypeSymbol::String),
    BoundBinaryOperator(
        SyntaxKind::EqualsEqualsToken,
        BoundBinaryOperatorKind::Equals,
        TypeSymbol::String,
        TypeSymbol::Bool
    ),
    BoundBinaryOperator(
        SyntaxKind::BangEqualsToken,
        BoundBinaryOperatorKind::NotEquals,
        TypeSymbol::String,
        TypeSymbol::Bool
    ),
};

NullablePointer<const BoundBinaryOperator> BoundBinaryOperator::Bind(
    const enum SyntaxKind SyntaxKind,
    const std::shared_ptr<const class TypeSymbol> LeftType,
    const std::shared_ptr<const class TypeSymbol> RightType
) noexcept
{
    const auto Result = std::ranges::find_if(
        std::ranges::begin(Operators),
        std::ranges::end(Operators),
        [=](const BoundBinaryOperator& Operator) {
            return Operator.SyntaxKind == SyntaxKind && Operator.LeftType == LeftType
                && Operator.RightType == RightType;
        }
    );
    if (Result == std::ranges::end(Operators))
    {
        return {};
    }

    return &*Result;
}