#pragma once

#include "BoundFunctionDeclaration.h"
#include "MambaCore.h"
#include "Symbol.h"
#include <memory>
#include <vector>

namespace Mamba
{
    class FunctionSymbol : public Symbol
    {
    public:
        using Super = Symbol;

        [[nodiscard]] FunctionSymbol(
            const std::shared_ptr<const String> Name,
            const std::span<std::shared_ptr<const class ParameterSymbol>> Parameters,
            const std::shared_ptr<const class TypeSymbol> Type,
            const std::shared_ptr<const class BoundFunctionDeclaration> BoundDeclaration
        ) noexcept;

        [[nodiscard]] FunctionSymbol(
            const std::shared_ptr<const String> Name,
            std::vector<std::shared_ptr<const class ParameterSymbol>>&& Parameters,
            const std::shared_ptr<const class TypeSymbol> Type,
            const std::shared_ptr<const class BoundFunctionDeclaration> BoundDeclaration
        ) noexcept;

        const std::vector<std::shared_ptr<const class ParameterSymbol>> Parameters;
        std::shared_ptr<const class BoundFunctionDeclaration> BoundDeclaration;
        const std::shared_ptr<const class TypeSymbol> Type;

        SymbolKind Kind() const noexcept override;
    };
} // namespace Mamba