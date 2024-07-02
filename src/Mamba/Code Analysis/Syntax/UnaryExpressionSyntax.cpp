#include "UnaryExpressionSyntax.h"

namespace Mamba
{
    UnaryExpressionSyntax::UnaryExpressionSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                                 const std::shared_ptr<const class SyntaxToken> OperatorToken,
                                                 const std::shared_ptr<const class ExpressionSyntax> Operand) noexcept :
        Super(SyntaxTree), OperatorToken(OperatorToken), Operand(Operand)
    {
    }

    SyntaxKind UnaryExpressionSyntax::Kind() const noexcept
    {
        return SyntaxKind::UnaryExpression;
    }

    std::vector<std::shared_ptr<const class SyntaxNode>> UnaryExpressionSyntax::Children() const noexcept
    {
        return { OperatorToken, Operand };
    }
} // namespace Mamba