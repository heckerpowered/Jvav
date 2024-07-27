#include "FunctionSymbol.h"

namespace Mamba
{
    FunctionSymbol::FunctionSymbol(
        const std::shared_ptr<const String> Name,
        const std::span<std::shared_ptr<const class ParameterSymbol>> Parameters,
        const std::shared_ptr<const class TypeSymbol> Type,
        const NullableSharedPtr<const class FunctionDeclarationSyntax> Declaration
    ) noexcept :
        Super(Name),
#if __cpp_lib_containers_ranges == 202202L
        Parameters(std::from_range, Parameters)
#else
        Parameters(Parameters.begin(), Parameters.end())
#endif
        ,
        Declaration(Declaration),
        Type(Type)
    {
    }

    FunctionSymbol::FunctionSymbol(
        const std::shared_ptr<const String> Name,
        std::vector<std::shared_ptr<const class ParameterSymbol>>&& Parameters,
        const std::shared_ptr<const class TypeSymbol> Type,
        const NullableSharedPtr<const class FunctionDeclarationSyntax> Declaration
    ) noexcept :
        Super(Name), Parameters(std::move(Parameters)), Declaration(Declaration), Type(Type)
    {
    }

    SymbolKind FunctionSymbol::Kind() const noexcept
    {
        return SymbolKind::Function;
    }
} // namespace Mamba