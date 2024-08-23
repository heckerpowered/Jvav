#pragma once

#include "MambaCore.h"

#include "SymbolKind.h"

namespace Mamba
{
    class Symbol
    {
        StringView PrivateName;

    protected:
        [[nodiscard]] Symbol(StringView Name) noexcept;
        virtual ~Symbol() = default;

    public:
        [[nodiscard]] StringView Name() const noexcept;
        [[nodiscard]] virtual SymbolKind Kind() const noexcept = 0;
        [[nodiscard]] bool IsFunction() const noexcept;
        [[nodiscard]] bool IsVariable() const noexcept;
        [[nodiscard]] bool IsParameter() const noexcept;
        [[nodiscard]] bool IsType() const noexcept;
    };
} // namespace Mamba