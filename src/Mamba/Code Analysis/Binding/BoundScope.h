#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "MambaCore.h"

namespace Mamba
{
    // The bound scope associates symbols with scopes. Symbols with the same name may be found during name lookup.
    // Function symbols with the same name form an overload set. However, when the multi non-function symbols with the
    // same name were found during name lookup, the name is ambiguous and the program is ill-formed, diagnostics are
    // required.
    class BoundScope : public std::enable_shared_from_this<BoundScope>
    {
        std::unordered_map<std::shared_ptr<const String>, std::vector<std::shared_ptr<const class Symbol>>> Symbols;

    public:
        [[nodiscard]] BoundScope(const NullableSharedPtr<const BoundScope> Parent) noexcept;

        void Declare(const std::shared_ptr<const class Symbol> Symbol) noexcept;
        std::shared_ptr<BoundScope> DeclareScope() noexcept;

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
        std::vector<std::shared_ptr<const BoundScope>> Children;
    };
}; // namespace Mamba