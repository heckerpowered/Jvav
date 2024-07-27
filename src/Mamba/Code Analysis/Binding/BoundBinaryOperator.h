#pragma once

#include <memory>

#include "MambaCore.h"

#include "BoundBinaryOperatorKind.h"
#include "SyntaxKind.h"

namespace Mamba
{
    class BoundBinaryOperator
    {
        [[nodiscard]] BoundBinaryOperator(
            const SyntaxKind SyntaxKind,
            const BoundBinaryOperatorKind Kind,
            const std::shared_ptr<const class TypeSymbol> Type
        ) noexcept;

        [[nodiscard]] BoundBinaryOperator(
            const SyntaxKind SyntaxKind,
            const BoundBinaryOperatorKind Kind,
            const std::shared_ptr<const class TypeSymbol> OperandType,
            const std::shared_ptr<const class TypeSymbol> ResultType
        ) noexcept;

        [[nodiscard]] BoundBinaryOperator(
            const SyntaxKind SyntaxKind,
            const BoundBinaryOperatorKind Kind,
            const std::shared_ptr<const class TypeSymbol> LeftType,
            const std::shared_ptr<const class TypeSymbol> RightType,
            const std::shared_ptr<const class TypeSymbol> ResultType
        ) noexcept;

        BoundBinaryOperator(const BoundBinaryOperator&) = delete;
        BoundBinaryOperator(BoundBinaryOperator&&) = delete;

        BoundBinaryOperator& operator=(const BoundBinaryOperator&) = delete;
        BoundBinaryOperator& operator=(BoundBinaryOperator&&) = delete;

    public:
        const enum SyntaxKind SyntaxKind;
        const BoundBinaryOperatorKind Kind;
        const std::shared_ptr<const class TypeSymbol> LeftType;
        const std::shared_ptr<const class TypeSymbol> RightType;
        const std::shared_ptr<const class TypeSymbol> Type;

    private:
        static BoundBinaryOperator Operators[];

    public:
        static NullablePointer<const BoundBinaryOperator> Bind(
            const enum SyntaxKind SyntaxKind,
            const std::shared_ptr<const class TypeSymbol> LeftType,
            const std::shared_ptr<const class TypeSymbol> RightType
        ) noexcept;
    };
} // namespace Mamba