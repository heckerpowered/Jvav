#include "TypeSymbol.h"
#include "MambaCore.h"

namespace Mamba
{
    TypeSymbol::TypeSymbol(const StringView Name) noexcept : Super(Name) {}

    SymbolKind TypeSymbol::Kind() const noexcept
    {
        return SymbolKind::Type;
    }
} // namespace Mamba