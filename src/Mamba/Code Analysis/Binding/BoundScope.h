#pragma once

#include <unordered_map>
#include <vector>

#include "FunctionSymbol.h"
#include "MambaCore.h"
#include "ParameterSymbol.h"
#include "Symbol.h"
#include "TypeSymbol.h"
#include "VariableSymbol.h"

namespace Mamba
{
    // The bound scope associates symbols with scopes. Symbols with the same name may be found during name lookup.
    // Function symbols with the same name form an overload set. However, when the multi non-function symbols with the
    // same name were found during name lookup, the name is ambiguous and the program is ill-formed, diagnostics are
    // required.
    class BoundScope
    {
        std::unordered_map<StringView, std::vector<const Symbol*>> Symbols;

    public:
        [[nodiscard]] BoundScope(NullablePointer<const BoundScope> Parent) noexcept;
        ~BoundScope() noexcept;

        void Declare(const Symbol* Symbol) noexcept;
        BoundScope* DeclareScope() noexcept;

        std::vector<const VariableSymbol*> LookupParameterOrVariable(StringView Name) const noexcept;
        std::vector<const ParameterSymbol*> LookupParameter(StringView Name) const noexcept;
        std::vector<const VariableSymbol*> LookupVariable(StringView Name) const noexcept;
        std::vector<const FunctionSymbol*> LookupFunction(StringView Name) const noexcept;
        std::vector<const TypeSymbol*> LookupType(StringView Name) const noexcept;
        std::vector<const Symbol*> Lookup(const StringView) const noexcept;

        std::vector<const VariableSymbol*> DeclaredVariables() const noexcept;
        std::vector<const FunctionSymbol*> DeclaredFunctions() const noexcept;
        std::vector<const TypeSymbol*> DeclaredTypes() const noexcept;
        std::vector<const ParameterSymbol*> DeclaredParameters() const noexcept;
        std::vector<const Symbol*> DeclaredSymbols() const noexcept;

        NullablePointer<const BoundScope> Parent;
        std::vector<const BoundScope*> Children;
    };
}; // namespace Mamba