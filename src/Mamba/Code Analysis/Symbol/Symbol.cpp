#include "Symbol.h"

using namespace Mamba;

Symbol::Symbol(StringView Name) noexcept :
    PrivateName(Name) {}

StringView Symbol::Name() const noexcept
{
    return PrivateName;
}

bool Symbol::IsFunction() const noexcept
{
    return Kind() == SymbolKind::Function;
}

bool Symbol::IsVariable() const noexcept
{
    return Kind() == SymbolKind::Variable;
}

bool Symbol::IsParameter() const noexcept
{
    return Kind() == SymbolKind::Parameter;
}

bool Symbol::IsType() const noexcept
{
    return Kind() == SymbolKind::Type;
}