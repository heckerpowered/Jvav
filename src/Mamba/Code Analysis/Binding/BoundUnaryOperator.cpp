#include "BoundUnaryOperator.h"

#include <algorithm>

#include "BoundUnaryOperatorKind.h"
#include "MambaCore.h"
#include "SyntaxKind.h"
#include "TypeSymbol.h"

using namespace Mamba;

BoundUnaryOperator::BoundUnaryOperator(
    const enum SyntaxKind SyntaxKind,
    const BoundUnaryOperatorKind Kind,
    const std::shared_ptr<const class TypeSymbol> OperandType
) noexcept :
    SyntaxKind(SyntaxKind), Kind(Kind), OperandType(OperandType)
{
}

BoundUnaryOperator::BoundUnaryOperator(
    const enum SyntaxKind SyntaxKind,
    const BoundUnaryOperatorKind Kind,
    const std::shared_ptr<const class TypeSymbol> OperandType,
    const std::shared_ptr<const class TypeSymbol> ResultType
) noexcept :
    SyntaxKind(SyntaxKind), Kind(Kind), OperandType(OperandType), ResultType(ResultType) {};

BoundUnaryOperator BoundUnaryOperator::Operators[]{
    BoundUnaryOperator(SyntaxKind::BangToken, BoundUnaryOperatorKind::LogicalNegation, TypeSymbol::Bool),

    BoundUnaryOperator(SyntaxKind::PlusToken, BoundUnaryOperatorKind::Identity, TypeSymbol::Int),
    BoundUnaryOperator(SyntaxKind::MinusToken, BoundUnaryOperatorKind::Negation, TypeSymbol::Int),
    BoundUnaryOperator(SyntaxKind::TildeToken, BoundUnaryOperatorKind::OnesComplement, TypeSymbol::Int)
};

NullablePointer<const BoundUnaryOperator> BoundUnaryOperator::Bind(
    const enum SyntaxKind SyntaxKind,
    const std::shared_ptr<const class TypeSymbol> OperandType
) noexcept
{
    const auto Result = std::ranges::find_if(
        std::ranges::begin(Operators),
        std::ranges::end(Operators),
        [=](const BoundUnaryOperator& Operator)
        { return Operator.SyntaxKind == SyntaxKind && Operator.OperandType == OperandType; }
    );
    if (Result == std::ranges::end(Operators))
    {
        return {};
    }

    return &*Result;
}