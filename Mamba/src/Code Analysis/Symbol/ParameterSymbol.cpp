#include "ParameterSymbol.h"

#include "TypeSymbol.h"

using namespace Mamba;

ParameterSymbol::ParameterSymbol(StringView Name, const TypeSymbol* Type, std::size_t Oridinal) noexcept :
    Super(Name, false, Type, {}), Oridinal(Oridinal)
{
}

ParameterSymbol::~ParameterSymbol() noexcept
{
    if (!TypeSymbol::IsBuiltInType(Type))
    {
        delete Type;
    }
}

SymbolKind ParameterSymbol::Kind() const noexcept
{
    return SymbolKind::Parameter;
}