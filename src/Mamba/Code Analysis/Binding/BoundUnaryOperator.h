#pragma once

#include "BoundUnaryOperatorKind.h"
#include "MambaCore.h"
#include "SyntaxKind.h"
#include "TypeSymbol.h"

namespace Mamba
{
    class BoundUnaryOperator
    {
        [[nodiscard]] BoundUnaryOperator(SyntaxKind SyntaxKind, BoundUnaryOperatorKind Kind, const TypeSymbol* OperandType) noexcept;
        [[nodiscard]] BoundUnaryOperator(SyntaxKind SyntaxKind, BoundUnaryOperatorKind Kind, const TypeSymbol* OperandType, const TypeSymbol* ResultType) noexcept;

    public:
        enum SyntaxKind SyntaxKind;
        BoundUnaryOperatorKind Kind;
        const TypeSymbol* OperandType;
        const TypeSymbol* ResultType;

    private:
        static BoundUnaryOperator Operators[];

    public:
        // when the return value is not null, the life time of the object lasts to the end of the program
        static NullablePointer<const BoundUnaryOperator> Bind(enum SyntaxKind SyntaxKind, const TypeSymbol* OperandType) noexcept;
    };
} // namespace Mamba