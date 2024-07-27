#pragma once

#include <memory>

#include "MambaCore.h"
#include "Symbol.h"
#include "SymbolKind.h"

namespace Mamba
{
    class VariableSymbol : public Symbol
    {
    public:
        using Super = Symbol;

        [[nodiscard]] VariableSymbol(
            const std::shared_ptr<const String> Name,
            const bool IsConstant,
            const std::shared_ptr<const class TypeSymbol> Type,
            const NullableSharedPtr<const class BoundConstant> Constant
        ) noexcept;

        SymbolKind Kind() const noexcept override;

        const bool IsConstant;
        const std::shared_ptr<const class TypeSymbol> Type;
        const NullableSharedPtr<const class BoundConstant> Constant;
    };
} // namespace Mamba