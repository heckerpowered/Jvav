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

std::vector<const SyntaxNode*> CallExpressionSyntax::Children() const noexcept
{
    auto Result = std::vector<const SyntaxNode*>();

    // Arguments->Count() returns nodes count, so multiply by 2 to count with separators
    Result.reserve(2 + Arguments.Count() * 2);
    Result.emplace_back(Identifier);
    Result.emplace_back(OpenParenthesisToken);

#if defined(__cpp_lib_containers_ranges) && __cpp_lib_containers_ranges >= 202202L
    Result.append_range(Arguments.WithSeperators());
#else
    for (auto Argument : Arguments.WithSeperators())
    {
        Result.emplace_back(std::forward<decltype(Argument)>(Argument));
    }
#endif

    Result.emplace_back(CloseParenthesisToken);

    return Result;
}