#pragma once

#include <memory>
#include <unordered_map>

#include "FunctionSymbol.h"
#include "MambaCore.h"

namespace Mamba
{
    class BoundScope
    {
        std::unordered_map<std::shared_ptr<const String>, std::vector<std::shared_ptr<const class Symbol>>> Symbols;

    public:
        [[nodiscard]] BoundScope(const NullableSharedPtr<const BoundScope> Parent) noexcept;

        void Declare(const std::shared_ptr<const class Symbol> Symbol) noexcept;

        std::vector<std::shared_ptr<const class VariableSymbol>> LookupVariable(const std::shared_ptr<const String> Name
        ) const noexcept;
        std::vector<std::shared_ptr<const class FunctionSymbol>> LookupFunction(const std::shared_ptr<const String> Name
        ) const noexcept;
        std::vector<std::shared_ptr<const class TypeSymbol>> LookupType(const std::shared_ptr<const String> Name
        ) const noexcept;
        std::vector<std::shared_ptr<const class TypeSymbol>> LookupParameter(const std::shared_ptr<const String> Name
        ) const noexcept;
        std::vector<std::shared_ptr<const class Symbol>> Lookup(const std::shared_ptr<const String> Name
        ) const noexcept;

        template<typename T>
        std::vector<std::shared_ptr<const T>> Lookup(const std::shared_ptr<const String> Name) noexcept
            requires std::is_base_of_v<Symbol, T>
        {
            return Lookup(Name)
                 | std::views::transform([](auto Symbol) { return std::dynamic_pointer_cast<const T>(Symbol); })
                 | std::views::filter([](auto Symbol) { return Symbol != nullptr; }) | std::ranges::to<std::vector>();
        }

        std::vector<std::shared_ptr<const class VariableSymbol>> DeclaredVariables() const noexcept;
        std::vector<std::shared_ptr<const class FunctionSymbol>> DeclaredFunctions() const noexcept;
        std::vector<std::shared_ptr<const class FunctionSymbol>> DeclaredTypes() const noexcept;
        std::vector<std::shared_ptr<const class FunctionSymbol>> DeclaredParameters() const noexcept;
        std::vector<std::shared_ptr<const class FunctionSymbol>> DeclaredSymbols() const noexcept;

        NullableSharedPtr<const BoundScope> Parent;
    };
}; // namespace Mamba