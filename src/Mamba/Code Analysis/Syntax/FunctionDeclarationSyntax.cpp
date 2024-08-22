#include "FunctionDeclarationSyntax.h"

using namespace Mamba;

FunctionDeclarationSyntax::FunctionDeclarationSyntax(
    const class SyntaxTree* SyntaxTree,
    const SyntaxToken* FunctionKeyword,
    const SyntaxToken* Identifier,
    const SyntaxToken* OpenParenthesisToken,
    SeperatedSyntaxList<const ParameterSyntax*>&& Parameters,
    const SyntaxToken* CloseParenthesisToken,
    NullablePointer<const TypeClauseSyntax> Type,
    const BlockStatementSyntax* Body
) noexcept :
    Super(SyntaxTree),
    FunctionKeyword(FunctionKeyword),
    Identifier(Identifier),
    OpenParenthesisToken(OpenParenthesisToken),
    Parameters(std::move(Parameters)),
    CloseParenthesisToken(CloseParenthesisToken),
    Type(Type),
    Body(Body)
{
}

FunctionDeclarationSyntax::~FunctionDeclarationSyntax() noexcept
{
    for (auto&& Parameter : Parameters.Nodes())
    {
        delete Parameter;
    }
    delete Type;
    delete Body;
}

SyntaxKind FunctionDeclarationSyntax::Kind() const noexcept
{
    return SyntaxKind::FunctionDeclaration;
}

std::vector<const SyntaxNode*> FunctionDeclarationSyntax::Children() const noexcept
{
    auto Children = std::vector<const SyntaxNode*>();
    Children.reserve(6 + Parameters.Count() * 2);

    Children.emplace_back(FunctionKeyword);
    Children.emplace_back(Identifier);
    Children.emplace_back(OpenParenthesisToken);
    for (auto Parameter : Parameters.Nodes())
    {
        Children.emplace_back(Parameter);
    }
    Children.emplace_back(CloseParenthesisToken);
    if (Type)
    {
        Children.emplace_back(Type);
    }
    Children.emplace_back(Body);

    return Children;
}