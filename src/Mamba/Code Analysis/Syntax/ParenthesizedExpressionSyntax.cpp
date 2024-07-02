#include "ParenthesizedExpressionSyntax.h"

namespace Mamba
{
    SyntaxKind Mamba::ParenthesizedExpressionSyntax::Kind() const noexcept
    {
        return SyntaxKind::ParenthesizedExpression;
    }

    std::vector<std::shared_ptr<const class SyntaxNode>> ParenthesizedExpressionSyntax::Children() const noexcept
    {
        return { OpenParenthesisToken, Expression, CloseParenthesisToken };
    }
} // namespace Mamba