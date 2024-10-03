#include "VariableSymbol.h"

using namespace Mamba;

VariableSymbol::VariableSymbol(StringView Name, bool IsConstant, const TypeSymbol* Type, struct Constant Constant) noexcept :
    Super(Name), IsConstant(IsConstant), Type(Type), Constant(Constant)
{
}

VariableSymbol::~VariableSymbol() noexcept
{
    if (!TypeSymbol::IsBuiltInType(Type))
    {
        delete Type;
    }
}

SymbolKind VariableSymbol::Kind() const noexcept
{
    return SymbolKind::Variable;
}