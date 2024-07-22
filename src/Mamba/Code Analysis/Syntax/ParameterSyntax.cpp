#include "ParameterSyntax.h"

#include "TypeClauseSyntax.h"

namespace Mamba
{
    ParameterSyntax::ParameterSyntax(
        const std::shared_ptr<const class SyntaxTree> SyntaxTree,
        const std::shared_ptr<const class SyntaxToken> Identifier,
        const std::shared_ptr<const class TypeClauseSyntax> Type
    ) noexcept :
        Super(SyntaxTree), Identifier(Identifier), Type(Type)
    {
    }

    SyntaxKind ParameterSyntax::Kind() const noexcept
    {
        return SyntaxKind::Parameter;
    }

    std::vector<std::shared_ptr<const class SyntaxNode>> ParameterSyntax::Children() const noexcept
    {
        return { Identifier, Type };
    }
} // namespace Mamba