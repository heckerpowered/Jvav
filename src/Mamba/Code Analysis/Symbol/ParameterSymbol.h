#pragma once

#include <memory>

#include "MambaCore.h"
#include "VariableSymbol.h"

namespace Mamba
{
    class ParameterSymbol : public VariableSymbol
    {
    public:
        using Super = VariableSymbol;

        [[nodiscard]] ParameterSymbol(
            const StringView Name,
            const std::shared_ptr<const class TypeSymbol> Type,
            const std::size_t Oridinal
        ) noexcept;

        const std::size_t Oridinal;

        SymbolKind Kind() const noexcept override;
    };
} // namespace Mamba