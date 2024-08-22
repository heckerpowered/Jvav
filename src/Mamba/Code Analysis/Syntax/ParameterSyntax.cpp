#include "ParameterSyntax.h"

#include "TypeClauseSyntax.h"

using namespace Mamba;

ParameterSyntax::ParameterSyntax(
    const class SyntaxTree* SyntaxTree,
    const SyntaxToken* Identifier,
    const TypeClauseSyntax* Type
) noexcept :
    Super(SyntaxTree), Identifier(Identifier), Type(Type)
{
}

ParameterSyntax::~ParameterSyntax() noexcept
{
    delete Type;
}

SyntaxKind ParameterSyntax::Kind() const noexcept
{
    return SyntaxKind::Parameter;
}

std::vector<const SyntaxNode*> ParameterSyntax::Children() const noexcept
{
    return { Identifier, Type };
}