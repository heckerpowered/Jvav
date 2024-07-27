#include "BoundScope.h"
#include "FunctionSymbol.h"
#include "TypeSymbol.h"
#include <memory>
#include <ranges>

using namespace Mamba;

BoundScope::BoundScope(const NullableSharedPtr<const BoundScope> Parent) noexcept : Parent(Parent) {}

bool BoundScope::Declare(const std::shared_ptr<const Symbol> Symbol) noexcept
{
    if (Symbols.contains(Symbol->Name))
    {
        return false;
    }

    Symbols.emplace(Symbol->Name, Symbol);
}

std::vector<std::shared_ptr<const VariableSymbol>> BoundScope::LookupVariable(const std::shared_ptr<const String> Name
) const noexcept
{
    return Lookup(Name) | std::views::filter([](auto Symbol) { return Symbol->IsVariable(); })
         | std::views::transform([](auto Symbol) { return std::static_pointer_cast<const VariableSymbol>(Symbol); })
         | std::ranges::to<std::vector>();
}

std::vector<std::shared_ptr<const FunctionSymbol>> BoundScope::LookupFunction(const std::shared_ptr<const String> Name
) const noexcept
{
    return Lookup(Name) | std::views::filter([](auto Symbol) { return Symbol->IsFunction(); })
         | std::views::transform([](auto Symbol) { return std::static_pointer_cast<const FunctionSymbol>(Symbol); })
         | std::ranges::to<std::vector>();
}

std::vector<std::shared_ptr<const class TypeSymbol>> BoundScope::LookupType(const std::shared_ptr<const String> Name
) const noexcept
{
    return Lookup(Name) | std::views::filter([](auto Symbol) { return Symbol->IsType(); })
         | std::views::transform([](auto Symbol) { return std::static_pointer_cast<const TypeSymbol>(Symbol); })
         | std::ranges::to<std::vector>();
}

std::vector<std::shared_ptr<const class TypeSymbol>>
    BoundScope::LookupParameter(const std::shared_ptr<const String> Name) const noexcept
{
    return Lookup(Name) | std::views::filter([](auto Symbol) { return Symbol->IsParameter(); })
         | std::views::transform([](auto Symbol) { return std::static_pointer_cast<const TypeSymbol>(Symbol); })
         | std::ranges::to<std::vector>();
}

std::vector<std::shared_ptr<const Symbol>> BoundScope::Lookup(const std::shared_ptr<const String> Name) const noexcept
{
    const auto Result = Symbols.find(Name);
    if (Result != Symbols.end())
    {
        return Result->second;
    }

    return Parent ? Parent->Lookup(Name) : std::vector<std::shared_ptr<const Symbol>>{};
}

std::vector<std::shared_ptr<const class VariableSymbol>> Mamba::BoundScope::DeclaredVariables() const noexcept
{
    return std::vector<std::shared_ptr<const class VariableSymbol>>();
}

std::vector<std::shared_ptr<const class FunctionSymbol>> Mamba::BoundScope::DeclaredFunctions() const noexcept
{
    return std::vector<std::shared_ptr<const class FunctionSymbol>>();
}
