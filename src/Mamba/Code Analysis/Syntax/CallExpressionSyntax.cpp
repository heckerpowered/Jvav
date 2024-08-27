#include "CallExpressionSyntax.h"

using namespace Mamba;

CallExpressionSyntax::CallExpressionSyntax(
    const class SyntaxTree* SyntaxTree,
    const SyntaxToken* Identifier,
    const SyntaxToken* OpenParenthesisToken,
    SeperatedSyntaxList<const ExpressionSyntax*> Arguments,
    const SyntaxToken* CloseParenthesisToken
) noexcept :
    Super(SyntaxTree),
    Identifier(Identifier),
    OpenParenthesisToken(OpenParenthesisToken),
    Arguments(Arguments),
    CloseParenthesisToken(CloseParenthesisToken)
{
}

CallExpressionSyntax::~CallExpressionSyntax() noexcept
{
    for (auto Argument : Arguments.Nodes())
    {
        delete Argument;
    }
}

SyntaxKind CallExpressionSyntax::Kind() const noexcept
{
    return SyntaxKind::CallExpression;
}

std::size_t CallExpressionSyntax::ChildrenCount() const noexcept
{
    return 2 + Arguments.size();
}

const SyntaxNode* CallExpressionSyntax::ChildAt(std::size_t Index) const noexcept
{
    if (Index == 0)
    {
        return Identifier;
    }
    else if (Index == 1)
    {
        return OpenParenthesisToken;
    }
    else if (Index < Arguments.size() + 2)
    {
        return Arguments[Index - 2];
    }
    else if (Index == Arguments.size() + 2)
    {
        return CloseParenthesisToken;
    }

    ReportChildrenAccessOutOfBounds(Index);
}