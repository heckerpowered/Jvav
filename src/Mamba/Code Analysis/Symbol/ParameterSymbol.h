#pragma once

#include "MambaCore.h"
#include "VariableSymbol.h"

namespace Mamba
{
    class ParameterSymbol : public VariableSymbol
    {
    public:
        using Super = VariableSymbol;

        [[nodiscard]] ParameterSymbol(StringView Name, const TypeSymbol* Type, std::size_t Oridinal) noexcept;
        ~ParameterSymbol() noexcept override;

        SymbolKind Kind() const noexcept override;

        std::size_t Oridinal;
    };
} // namespace Mamba