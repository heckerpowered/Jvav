#pragma once

#include "Constant.h"
#include "MambaCore.h"
#include "Symbol.h"
#include "SymbolKind.h"
#include "TypeSymbol.h"

namespace Mamba
{
    class VariableSymbol : public Symbol
    {
    public:
        using Super = Symbol;

        [[nodiscard]] VariableSymbol(StringView Name, bool IsConstant, const TypeSymbol* Type, struct Constant Constant) noexcept;
        ~VariableSymbol() noexcept override;

        SymbolKind Kind() const noexcept override;

        bool IsConstant;
        const TypeSymbol* Type;
        struct Constant Constant;
    };
} // namespace Mamba