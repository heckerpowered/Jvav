#pragma once

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
            const NullableSharedPtr<const class FunctionDeclarationSyntax> Declaration
        ) noexcept;

        [[nodiscard]] FunctionSymbol(
            const std::shared_ptr<const String> Name,
            std::vector<std::shared_ptr<const class ParameterSymbol>>&& Parameters,
            const std::shared_ptr<const class TypeSymbol> Type,
            const NullableSharedPtr<const class FunctionDeclarationSyntax> Declaration
        ) noexcept;

        const std::vector<std::shared_ptr<const class ParameterSymbol>> Parameters;
        const NullableSharedPtr<const class FunctionDeclarationSyntax> Declaration;
        const std::shared_ptr<const class TypeSymbol> Type;

        SymbolKind Kind() const noexcept override;
    };
} // namespace Mamba