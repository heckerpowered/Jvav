#include "BoundScope.h"
#include "TypeSymbol.h"

using namespace Mamba;

BoundScope::BoundScope(NullablePointer<const BoundScope> Parent) noexcept :
    Parent(Parent) {}

BoundScope::~BoundScope() noexcept
{
    for (auto Symbol : DeclaredSymbols())
    {
        if (TypeSymbol::IsBuiltInType(static_cast<const TypeSymbol*>(Symbol)))
        {
            continue;
        }

        delete Symbol;
    }

    for (auto Child : Children)
    {
        delete Child;
    }
}

void BoundScope::Declare(const Symbol* Symbol) noexcept
{
    Symbols[Symbol->Name()].emplace_back(Symbol);
}

BoundScope* BoundScope::DeclareScope() noexcept
{
    auto ChildScope = new BoundScope(this);
    Children.emplace_back(ChildScope);
    return ChildScope;
}

std::vector<const VariableSymbol*> BoundScope::LookupVariable(StringView Name) const noexcept
{
    return Lookup(Name) |
           std::views::filter([](auto Symbol) { return Symbol->IsVariable(); }) |
           std::views::transform([](auto Symbol) { return static_cast<const VariableSymbol*>(Symbol); }) |
           std::ranges::to<std::vector>();
}

std::vector<const FunctionSymbol*> BoundScope::LookupFunction(StringView Name) const noexcept
{
    return Lookup(Name) |
           std::views::filter([&](auto Symbol) { return Symbol->IsFunction(); }) |
           std::views::transform([](auto Symbol) { return static_cast<const FunctionSymbol*>(Symbol); }) |
           std::ranges::to<std::vector>();
}

std::vector<const TypeSymbol*> BoundScope::LookupType(StringView Name) const noexcept
{
    return Lookup(Name) |
           std::views::filter([](auto Symbol) { return Symbol->IsType(); }) |
           std::views::transform([](auto Symbol) { return static_cast<const TypeSymbol*>(Symbol); }) |
           std::ranges::to<std::vector>();
}

std::vector<const ParameterSymbol*> BoundScope::LookupParameter(StringView Name) const noexcept
{
    return Lookup(Name) |
           std::views::filter([](auto Symbol) { return Symbol->IsParameter(); }) |
           std::views::transform([](auto Symbol) { return static_cast<const ParameterSymbol*>(Symbol); }) |
           std::ranges::to<std::vector>();
}

std::vector<const Symbol*> BoundScope::Lookup(StringView Name) const noexcept
{
    auto Result = Symbols.find(Name);
    if (Result != Symbols.end()) [[unlikely]]
    {
        return Result->second;
    }

    return Parent ? Parent->Lookup(Name) : std::vector<const Symbol*>{};
}

std::vector<const VariableSymbol*> BoundScope::DeclaredVariables() const noexcept
{
    return Symbols |
           std::views::values |
           std::views::join |
           std::views::filter([](auto Symbol) { return Symbol->IsVariable(); }) |
           std::views::transform([](auto Symbol) { return static_cast<const VariableSymbol*>(Symbol); }) |
           std::ranges::to<std::vector>();
}

std::vector<const FunctionSymbol*> BoundScope::DeclaredFunctions() const noexcept
{
    return Symbols |
           std::views::values |
           std::views::join |
           std::views::filter([](auto Symbol) { return Symbol->IsFunction(); }) |
           std::views::transform([](auto Symbol) { return static_cast<const FunctionSymbol*>(Symbol); }) |
           std::ranges::to<std::vector>();
}

std::vector<const TypeSymbol*> BoundScope::DeclaredTypes() const noexcept
{
    return Symbols |
           std::views::values |
           std::views::join |
           std::views::filter([](auto Symbol) { return Symbol->IsType(); }) |
           std::views::transform([](auto Symbol) { return static_cast<const TypeSymbol*>(Symbol); }) |
           std::ranges::to<std::vector>();
}

std::vector<const ParameterSymbol*> BoundScope::DeclaredParameters() const noexcept
{
    return Symbols |
           std::views::values |
           std::views::join |
           std::views::filter([](auto Symbol) { return Symbol->IsParameter(); }) |
           std::views::transform([](auto Symbol) { return static_cast<const ParameterSymbol*>(Symbol); }) |
           std::ranges::to<std::vector>();
}

std::vector<const Symbol*> BoundScope::DeclaredSymbols() const noexcept
{
    return Symbols |
           std::views::values |
           std::views::join |
           std::ranges::to<std::vector>();
}
