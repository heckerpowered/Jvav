#include "CallExpressionSyntax.h"

namespace Mamba
{
    CallExpressionSyntax::CallExpressionSyntax(
        const std::shared_ptr<const class SyntaxTree> SyntaxTree,
        const std::shared_ptr<const class SyntaxToken> Identifier,
        const std::shared_ptr<const class SyntaxToken> OpenParenthesisToken,
        const std::shared_ptr<const SeperatedSyntaxList<std::shared_ptr<const class SyntaxNode>>> Arguments,
        const std::shared_ptr<const class SyntaxToken> CloseParenthesisToken) noexcept :
        Super(SyntaxTree),
        Identifier(Identifier),
        OpenParenthesisToken(OpenParenthesisToken),
        Arguments(Arguments),
        CloseParenthesisToken(CloseParenthesisToken)
    {
    }

    SyntaxKind CallExpressionSyntax::Kind() const noexcept
    {
        return SyntaxKind::CallExpression;
    }

    std::vector<std::shared_ptr<const class SyntaxNode>> CallExpressionSyntax::Children() const noexcept
    {
        auto Result = std::vector<std::shared_ptr<const class SyntaxNode>>();

        // Arguments->Count() returns nodes count, so multiply by 2 to count with separators
        Result.reserve(2 + Arguments->Count() * 2);
        Result.emplace_back(Identifier);
        Result.emplace_back(OpenParenthesisToken);
        Result.append_range(Arguments->WithSeperators());
        Result.emplace_back(CloseParenthesisToken);

        return Result;
    }
} // namespace Mamba