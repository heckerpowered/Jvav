#pragma once

#include "MambaCore.h"

#include "SymbolKind.h"

namespace Mamba
{
    class Symbol
    {
    protected:
        [[nodiscard]] Symbol(const std::shared_ptr<const String> Name) noexcept;
        virtual ~Symbol() = default;

    public:
        const std::shared_ptr<const String> Name;

        [[nodiscard]] virtual SymbolKind Kind() const noexcept = 0;
        [[nodiscard]] bool IsFunction() const noexcept;
        [[nodiscard]] bool IsVariable() const noexcept;
        [[nodiscard]] bool IsParameter() const noexcept;
        [[nodiscard]] bool IsType() const noexcept;
    };
} // namespace Mamba