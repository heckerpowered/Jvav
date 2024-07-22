#pragma once

#include "Symbol.h"

namespace Mamba
{
    class TypeSymbol : public Symbol
    {
    public:
        using Super = Symbol;

        [[nodiscard]] TypeSymbol(const StringView Name) noexcept;

        SymbolKind Kind() const noexcept override;
    };
} // namespace Mamba