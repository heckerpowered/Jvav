#pragma once

#include "MambaCore.h"

#include "SymbolKind.h"

namespace Mamba
{
    class Symbol
    {
    protected:
        [[nodiscard]] Symbol(const StringView Name) noexcept;

    public:
        const String Name;

        [[nodiscard]] virtual SymbolKind Kind() const noexcept = 0;
    };
} // namespace Mamba