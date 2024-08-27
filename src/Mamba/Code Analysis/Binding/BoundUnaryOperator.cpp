#include "BoundUnaryOperator.h"

#include <algorithm>

using namespace Mamba;

BoundUnaryOperator::BoundUnaryOperator(enum SyntaxKind SyntaxKind, const BoundUnaryOperatorKind Kind, const TypeSymbol* OperandType) noexcept :
    SyntaxKind(SyntaxKind), Kind(Kind), OperandType(OperandType)
{
}

BoundUnaryOperator::BoundUnaryOperator(enum SyntaxKind SyntaxKind, const BoundUnaryOperatorKind Kind, const TypeSymbol* OperandType, const TypeSymbol* ResultType) noexcept :
    SyntaxKind(SyntaxKind), Kind(Kind), OperandType(OperandType), ResultType(ResultType) {};

BoundUnaryOperator BoundUnaryOperator::Operators[]{
    BoundUnaryOperator(SyntaxKind::BangToken, BoundUnaryOperatorKind::LogicalNegation, &TypeSymbol::Bool),

    BoundUnaryOperator(SyntaxKind::PlusToken, BoundUnaryOperatorKind::Identity, &TypeSymbol::Int),
    BoundUnaryOperator(SyntaxKind::MinusToken, BoundUnaryOperatorKind::Negation, &TypeSymbol::Int),
    BoundUnaryOperator(SyntaxKind::TildeToken, BoundUnaryOperatorKind::OnesComplement, &TypeSymbol::Int)
};

NullablePointer<const BoundUnaryOperator> BoundUnaryOperator::Bind(enum SyntaxKind SyntaxKind, const TypeSymbol* OperandType) noexcept
{
    auto Result = std::ranges::find_if(
        std::ranges::begin(Operators),
        std::ranges::end(Operators),
        [=](auto Operator) { return Operator.SyntaxKind == SyntaxKind && Operator.OperandType == OperandType; }
    );
    if (Result == std::ranges::end(Operators))
    {
        return {};
    }

    return &*Result;
}