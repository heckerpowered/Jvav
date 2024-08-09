#include "BoundScope.h"

#include <memory>
#include <ranges>

#include "fast_io.h"
#include "fast_io_core_impl/codecvt/general.h"
#include "fast_io_core_impl/integers/impl.h"
#include "FunctionSymbol.h"
#include "ParameterSymbol.h"
#include "TypeSymbol.h"

using namespace Mamba;

BoundScope::BoundScope(const NullableSharedPtr<const BoundScope> Parent) noexcept :
    Parent(Parent) {}

void BoundScope::Declare(const std::shared_ptr<const Symbol> Symbol) noexcept
{
    Symbols[*Symbol->Name].emplace_back(Symbol);
}

std::shared_ptr<BoundScope> BoundScope::DeclareScope() noexcept
{
    const auto ChildScope = std::make_shared<BoundScope>(shared_from_this());
    Children.emplace_back(ChildScope);
    return ChildScope;
}

std::vector<std::shared_ptr<const VariableSymbol>> BoundScope::LookupVariable(const std::shared_ptr<const String> Name
) const noexcept
{
    return Lookup(Name) | std::views::filter([](auto Symbol)
                                             { return Symbol->IsVariable(); }) |
           std::views::transform([](auto Symbol)
                                 { return std::static_pointer_cast<const VariableSymbol>(Symbol); }) |
           std::ranges::to<std::vector>();
}

std::vector<std::shared_ptr<const FunctionSymbol>> BoundScope::LookupFunction(const std::shared_ptr<const String> Name
) const noexcept
{
    return Lookup(Name) | std::views::filter([&](auto Symbol)
                                             { return Symbol->IsFunction(); }) |
           std::views::transform([](auto Symbol)
                                 { return std::static_pointer_cast<const FunctionSymbol>(Symbol); }) |
           std::ranges::to<std::vector>();
}

std::vector<std::shared_ptr<const class TypeSymbol>> BoundScope::LookupType(const std::shared_ptr<const String> Name
) const noexcept
{
    return Lookup(Name) | std::views::filter([](auto Symbol)
                                             { return Symbol->IsType(); }) |
           std::views::transform([](auto Symbol)
                                 { return std::static_pointer_cast<const TypeSymbol>(Symbol); }) |
           std::ranges::to<std::vector>();
}

std::vector<std::shared_ptr<const class TypeSymbol>>
    BoundScope::LookupParameter(const std::shared_ptr<const String> Name) const noexcept
{
    return Lookup(Name) | std::views::filter([](auto Symbol)
                                             { return Symbol->IsParameter(); }) |
           std::views::transform([](auto Symbol)
                                 { return std::static_pointer_cast<const TypeSymbol>(Symbol); }) |
           std::ranges::to<std::vector>();
}

std::vector<std::shared_ptr<const Symbol>> BoundScope::Lookup(const std::shared_ptr<const String> Name) const noexcept
{
    const auto Result = Symbols.find(*Name);
    if (Result != Symbols.end())
    {
        return Result->second;
    }

    return Parent ? Parent->Lookup(Name) : std::vector<std::shared_ptr<const Symbol>>{};
}

std::vector<std::shared_ptr<const VariableSymbol>> BoundScope::DeclaredVariables() const noexcept
{
    return Symbols | std::views::values | std::views::join |
           std::views::filter([](auto Symbol)
                              { return Symbol->IsVariable(); }) |
           std::views::transform([](auto Symbol)
                                 { return std::static_pointer_cast<const VariableSymbol>(Symbol); }) |
           std::ranges::to<std::vector>();
}

std::vector<std::shared_ptr<const FunctionSymbol>> BoundScope::DeclaredFunctions() const noexcept
{
    return Symbols | std::views::values | std::views::join |
           std::views::filter([](auto Symbol)
                              { return Symbol->IsFunction(); }) |
           std::views::transform([](auto Symbol)
                                 { return std::static_pointer_cast<const FunctionSymbol>(Symbol); }) |
           std::ranges::to<std::vector>();
}

std::vector<std::shared_ptr<const TypeSymbol>> BoundScope::DeclaredTypes() const noexcept
{
    return Symbols | std::views::values | std::views::join |
           std::views::filter([](auto Symbol)
                              { return Symbol->IsType(); }) |
           std::views::transform([](auto Symbol)
                                 { return std::static_pointer_cast<const TypeSymbol>(Symbol); }) |
           std::ranges::to<std::vector>();
}

std::vector<std::shared_ptr<const ParameterSymbol>> BoundScope::DeclaredParameters() const noexcept
{
    return Symbols | std::views::values | std::views::join |
           std::views::filter([](auto Symbol)
                              { return Symbol->IsParameter(); }) |
           std::views::transform([](auto Symbol)
                                 { return std::static_pointer_cast<const ParameterSymbol>(Symbol); }) |
           std::ranges::to<std::vector>();
}

std::vector<std::shared_ptr<const Symbol>> BoundScope::DeclaredSymbols() const noexcept
{
    return Symbols | std::views::values | std::views::join | std::ranges::to<std::vector>();
}
