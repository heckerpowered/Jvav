#pragma once

#include <memory>

#include "BoundUnaryOperatorKind.h"
#include "MambaCore.h"
#include "SyntaxKind.h"

namespace Mamba
{
    class BoundUnaryOperator
    {
        [[nodiscard]] BoundUnaryOperator(
            const SyntaxKind SyntaxKind,
            const BoundUnaryOperatorKind Kind,
            const std::shared_ptr<const class TypeSymbol> OperandType
        ) noexcept;

        [[nodiscard]] BoundUnaryOperator(
            const SyntaxKind SyntaxKind,
            const BoundUnaryOperatorKind Kind,
            const std::shared_ptr<const class TypeSymbol> OperandType,
            const std::shared_ptr<const class TypeSymbol> ResultType
        ) noexcept;

    public:
        const SyntaxKind SyntaxKind;
        const BoundUnaryOperatorKind Kind;
        const std::shared_ptr<const class TypeSymbol> OperandType;
        const std::shared_ptr<const class TypeSymbol> ResultType;

    private:
        static BoundUnaryOperator Operators[];

        // when the return value is not null, the life time of the object lasts to the end of the program
        static NullablePointer<const BoundUnaryOperator>
            Bind(const enum SyntaxKind SyntaxKind, const std::shared_ptr<const class TypeSymbol> OperandType) noexcept;
    };
} // namespace Mamba