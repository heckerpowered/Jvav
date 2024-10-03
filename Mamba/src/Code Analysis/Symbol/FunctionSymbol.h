#pragma once

#include "BoundFunctionDeclaration.h"
#include "MambaCore.h"
#include "ParameterSymbol.h"
#include "Symbol.h"

namespace Mamba
{
    class FunctionSymbol : public Symbol
    {
    public:
        using Super = Symbol;

        [[nodiscard]] FunctionSymbol(StringView Name, std::vector<const ParameterSymbol*>&& Parameters, const TypeSymbol* Type, const BoundFunctionDeclaration* BoundDeclaration) noexcept;
        ~FunctionSymbol() noexcept override;

        SymbolKind Kind() const noexcept override;

        std::vector<const ParameterSymbol*> Parameters;
        const BoundFunctionDeclaration* BoundDeclaration;
        const TypeSymbol* Type;
    };
} // namespace Mamba