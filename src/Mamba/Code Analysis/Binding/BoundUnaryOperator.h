#pragma once

#include "BoundUnaryOperatorKind.h"
#include "MambaCore.h"
#include "SyntaxKind.h"
#include "TypeSymbol.h"

namespace Mamba
{
    class BoundUnaryOperator
    {
        [[nodiscard]] BoundUnaryOperator(SyntaxKind Kind, BoundUnaryOperatorKind BoundKind, const TypeSymbol* OperandType) noexcept;
        [[nodiscard]] BoundUnaryOperator(SyntaxKind Kind, BoundUnaryOperatorKind BoundKind, const TypeSymbol* OperandType, const TypeSymbol* ResultType) noexcept;

    public:
        SyntaxKind Kind;
        BoundUnaryOperatorKind BoundKind;
        const TypeSymbol* OperandType;
        const TypeSymbol* ResultType;

    private:
        static BoundUnaryOperator Operators[];

    public:
        // when the return value is not null, the life time of the object lasts to the end of the program
        static NullablePointer<const BoundUnaryOperator> Bind(SyntaxKind Kind, const TypeSymbol* OperandType) noexcept;
    };
} // namespace Mamba