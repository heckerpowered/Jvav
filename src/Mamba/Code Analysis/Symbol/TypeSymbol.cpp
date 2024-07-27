#include "TypeSymbol.h"

using namespace Mamba;

// why qualified name is required here?
TypeSymbol::TypeSymbol(const std::shared_ptr<const ::Mamba::String> Name) noexcept : Super(Name) {}

Mamba::TypeSymbol::~TypeSymbol() noexcept {}

SymbolKind TypeSymbol::Kind() const noexcept
{
    return SymbolKind::Type;
}

const std::shared_ptr<const TypeSymbol> TypeSymbol::Int =
    std::make_shared<const TypeSymbol>(std::make_shared<const ::Mamba::String>(TEXT("int")));
const std::shared_ptr<const TypeSymbol> TypeSymbol::Bool =
    std::make_shared<const TypeSymbol>(std::make_shared<const ::Mamba::String>(TEXT("bool")));
const std::shared_ptr<const TypeSymbol> TypeSymbol::String =
    std::make_shared<const TypeSymbol>(std::make_shared<const ::Mamba::String>(TEXT("string")));
const std::shared_ptr<const TypeSymbol> TypeSymbol::Void =
    std::make_shared<const TypeSymbol>(std::make_shared<const ::Mamba::String>(TEXT("void")));