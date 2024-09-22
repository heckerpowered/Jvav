#include "VariableDeclarationSyntax.h"

using namespace Mamba;

VariableDeclarationSyntax::VariableDeclarationSyntax(
    const class SyntaxTree* SyntaxTree,
    const SyntaxToken* Keyword,
    const SyntaxToken* Identifier,
    NullablePointer<const class TypeClauseSyntax> TypeClause,
    const SyntaxToken* EqualsToken,
    const ExpressionSyntax* Initializer
) noexcept :
    Super(SyntaxTree),
    Keyword(Keyword),
    Identifier(Identifier),
    TypeClause(TypeClause),
    EqualsToken(EqualsToken),
    Initializer(Initializer)
{
}

VariableDeclarationSyntax::~VariableDeclarationSyntax() noexcept
{
    delete TypeClause;
    delete Initializer;
}

SyntaxKind VariableDeclarationSyntax::Kind() const noexcept
{
    return SyntaxKind::VariableDeclaration;
}

std::size_t VariableDeclarationSyntax::ChildrenCount() const noexcept
{
    return TypeClause ? 5 : 4;
}

const SyntaxNode* VariableDeclarationSyntax::ChildAt(std::size_t Index) const noexcept
{
    if (TypeClause)
    {
        switch (Index)
        {
            case 0:
                return Keyword;
            case 1:
                return Identifier;
            case 2:
                return TypeClause;
            case 3:
                return EqualsToken;
            case 4:
                return Initializer;
            default:
                ReportChildrenAccessOutOfBounds(Index);
        }
    }

    switch (Index)
    {
        case 0:
            return Keyword;
        case 1:
            return Identifier;
        case 2:
            return EqualsToken;
        case 3:
            return Initializer;
        default:
            ReportChildrenAccessOutOfBounds(Index);
    }
}