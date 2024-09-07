#include "BoundUnaryOperator.h"

#include <algorithm>

using namespace Mamba;

BoundUnaryOperator::BoundUnaryOperator(SyntaxKind Kind, const BoundUnaryOperatorKind BoundKind, const TypeSymbol* OperandType) noexcept :
    Kind(Kind), BoundKind(BoundKind), OperandType(OperandType)
{
}

BoundUnaryOperator::BoundUnaryOperator(SyntaxKind Kind, const BoundUnaryOperatorKind BoundKind, const TypeSymbol* OperandType, const TypeSymbol* ResultType) noexcept :
    Kind(Kind), BoundKind(BoundKind), OperandType(OperandType), ResultType(ResultType) {};

BoundUnaryOperator BoundUnaryOperator::Operators[]{
    BoundUnaryOperator(SyntaxKind::BangToken, BoundUnaryOperatorKind::LogicalNegation, &TypeSymbol::Bool),

    BoundUnaryOperator(SyntaxKind::PlusToken, BoundUnaryOperatorKind::Identity, &TypeSymbol::Int),
    BoundUnaryOperator(SyntaxKind::MinusToken, BoundUnaryOperatorKind::Negation, &TypeSymbol::Int),
    BoundUnaryOperator(SyntaxKind::TildeToken, BoundUnaryOperatorKind::OnesComplement, &TypeSymbol::Int)
};

NullablePointer<const BoundUnaryOperator> BoundUnaryOperator::Bind(SyntaxKind Kind, const TypeSymbol* OperandType) noexcept
{
    auto Result = std::ranges::find_if(
        std::ranges::begin(Operators),
        std::ranges::end(Operators),
        [=](auto Operator) { return Operator.Kind == Kind && Operator.OperandType == OperandType; }
    );
    if (Result == std::ranges::end(Operators))
    {
        return {};
    }

    return &*Result;
}