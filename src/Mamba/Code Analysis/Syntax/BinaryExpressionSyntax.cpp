#include "BinaryExpressionSyntax.h"

namespace Mamba
{
    BinaryExpressionSyntax::BinaryExpressionSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                                   const std::shared_ptr<const class ExpressionSyntax> Left,
                                                   const std::shared_ptr<const class SyntaxToken> OperatorToken,
                                                   const std::shared_ptr<const class ExpressionSyntax> Right) noexcept :
        Super(SyntaxTree), Left(Left), OperatorToken(OperatorToken), Right(Right)
    {
    }

    SyntaxKind BinaryExpressionSyntax::Kind() const noexcept
    {
        return SyntaxKind::BinaryExpression;
    }

    std::vector<std::shared_ptr<const class SyntaxNode>> BinaryExpressionSyntax::Children() const noexcept
    {
        return { Left, OperatorToken, Right };
    }
} // namespace Mamba