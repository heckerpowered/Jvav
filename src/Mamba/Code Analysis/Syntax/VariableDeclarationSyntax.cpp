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

std::vector<const SyntaxNode*> VariableDeclarationSyntax::Children() const noexcept
{
    return { Keyword, Identifier, TypeClause, EqualsToken, Initializer };
}