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

std::size_t FunctionDeclarationSyntax::ChildrenCount() const noexcept
{
    return (Type ? 6 : 5) + Parameters.size();
}

const SyntaxNode* FunctionDeclarationSyntax::ChildAt(std::size_t Index) const noexcept
{
    if (Index == 0)
    {
        return FunctionKeyword;
    }
    else if (Index == 1)
    {
        return Identifier;
    }
    else if (Index == 2)
    {
        return OpenParenthesisToken;
    }
    else if (Index < Parameters.size() + 3)
    {
        return Parameters.at(Index - 3);
    }
    else if (Index == Parameters.size() + 3)
    {
        return CloseParenthesisToken;
    }
    else if (Index == Parameters.size() + 4)
    {
        return Type ? static_cast<const SyntaxNode*>(Type) : Body;
    }
    else if (Index == Parameters.size() + 5 && Type)
    {
        return Body;
    }

    ReportChildrenAccessOutOfBounds(Index);
}