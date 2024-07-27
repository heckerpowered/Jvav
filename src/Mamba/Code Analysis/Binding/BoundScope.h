#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

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

        std::vector<std::shared_ptr<const class VariableSymbol>> DeclaredVariables() const noexcept;
        std::vector<std::shared_ptr<const class FunctionSymbol>> DeclaredFunctions() const noexcept;
        std::vector<std::shared_ptr<const class TypeSymbol>> DeclaredTypes() const noexcept;
        std::vector<std::shared_ptr<const class ParameterSymbol>> DeclaredParameters() const noexcept;
        std::vector<std::shared_ptr<const class Symbol>> DeclaredSymbols() const noexcept;

        NullableSharedPtr<const BoundScope> Parent;
    };
}; // namespace Mamba