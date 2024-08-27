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

std::size_t ParameterSyntax::ChildrenCount() const noexcept
{
    return 2;
}

const SyntaxNode* ParameterSyntax::ChildAt(std::size_t Index) const noexcept
{
    switch (Index)
    {
        case 0:
            return Identifier;
        case 1:
            return Type;
        default:
            ReportChildrenAccessOutOfBounds(Index);
    }
}