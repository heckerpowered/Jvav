#pragma once

#include "MambaCore.h"

#include "BoundBinaryOperatorKind.h"
#include "SyntaxKind.h"
#include "TypeSymbol.h"

namespace Mamba
{
    class BoundBinaryOperator
    {
        [[nodiscard]] BoundBinaryOperator(SyntaxKind SyntaxKind, BoundBinaryOperatorKind Kind, const TypeSymbol* Type) noexcept;
        [[nodiscard]] BoundBinaryOperator(SyntaxKind SyntaxKind, BoundBinaryOperatorKind Kind, const TypeSymbol* OperandType, const TypeSymbol* ResultType) noexcept;
        [[nodiscard]] BoundBinaryOperator(SyntaxKind SyntaxKind, BoundBinaryOperatorKind Kind, const TypeSymbol* LeftType, const TypeSymbol* RightType, const TypeSymbol* ResultType) noexcept;

        BoundBinaryOperator(const BoundBinaryOperator&) = delete;
        BoundBinaryOperator(BoundBinaryOperator&&) = delete;

        BoundBinaryOperator& operator=(const BoundBinaryOperator&) = delete;
        BoundBinaryOperator& operator=(BoundBinaryOperator&&) = delete;

    public:
        enum SyntaxKind Kind;
        BoundBinaryOperatorKind BoundKind;
        const TypeSymbol* LeftType;
        const TypeSymbol* RightType;
        const TypeSymbol* Type;

    private:
        static BoundBinaryOperator Operators[];

    public:
        static NullablePointer<const BoundBinaryOperator> Bind(
            SyntaxKind Kind,
            const TypeSymbol* LeftType,
            const TypeSymbol* RightType
        ) noexcept;
    };
} // namespace Mamba