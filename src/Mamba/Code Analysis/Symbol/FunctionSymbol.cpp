#include "FunctionSymbol.h"
#include "TypeSymbol.h"

using namespace Mamba;

FunctionSymbol::FunctionSymbol(StringView Name, std::vector<const ParameterSymbol*>&& Parameters, const TypeSymbol* Type, const BoundFunctionDeclaration* BoundDeclaration) noexcept :
    Super(Name), Parameters(std::move(Parameters)), BoundDeclaration(BoundDeclaration), Type(Type)
{
}

FunctionSymbol::~FunctionSymbol() noexcept
{
    if (!TypeSymbol::IsBuiltInType(Type))
    {
        delete Type;
    }
}

SymbolKind FunctionSymbol::Kind() const noexcept
{
    return SymbolKind::Function;
}