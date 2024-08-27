#include "TypeSymbol.h"

using namespace Mamba;

TypeSymbol::TypeSymbol(StringView Name) noexcept :
    Super(Name) {}

SymbolKind TypeSymbol::Kind() const noexcept
{
    return SymbolKind::Type;
}

bool TypeSymbol::IsBuiltInType(const TypeSymbol* Type) noexcept
{
    return Type == &TypeSymbol::Int ||
           Type == &TypeSymbol::Bool ||
           Type == &TypeSymbol::String ||
           Type == &TypeSymbol::Void;
}

const TypeSymbol TypeSymbol::Int = TypeSymbol(TEXT("int"));
const TypeSymbol TypeSymbol::Bool = TypeSymbol(TEXT("bool"));
const TypeSymbol TypeSymbol::String = TypeSymbol(TEXT("string"));
const TypeSymbol TypeSymbol::Void = TypeSymbol(TEXT("void"));
const TypeSymbol TypeSymbol::Double = TypeSymbol(TEXT("double"));