#include "Symbol.h"
#include "SymbolKind.h"

namespace Mamba
{
    Symbol::Symbol(const std::shared_ptr<const String> Name) noexcept : Name(Name) {}

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
} // namespace Mamba