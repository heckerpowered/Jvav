#include "VariableSymbol.h"

#include "TypeSymbol.h"

namespace Mamba
{
    VariableSymbol::VariableSymbol(
        const std::shared_ptr<const String> Name,
        const bool IsConstant,
        const std::shared_ptr<const class TypeSymbol> Type,
        const NullableSharedPtr<const class BoundConstant> Constant
    ) noexcept :
        Super(Name), IsConstant(IsConstant), Type(Type), Constant(Constant)
    {
    }

    SymbolKind VariableSymbol::Kind() const noexcept
    {
        return SymbolKind::Variable;
    }

} // namespace Mamba