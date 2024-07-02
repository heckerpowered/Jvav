#include "AssignmentExpressionSyntax.h"

namespace Mamba
{
    AssignmentExpressionSyntax::AssignmentExpressionSyntax(
        const std::shared_ptr<const class SyntaxTree> SyntaxTree,
        const std::shared_ptr<const class SyntaxToken> IdentifierToken,
        const std::shared_ptr<const class SyntaxToken> AssignmentToken,
        const std::shared_ptr<const ExpressionSyntax> Expression) noexcept :
        Super(SyntaxTree), IdentifierToken(IdentifierToken), AssignmentToken(AssignmentToken), Expression(Expression)
    {
    }

    std::vector<std::shared_ptr<const SyntaxNode>> AssignmentExpressionSyntax::Children() const noexcept
    {
        return { IdentifierToken, AssignmentToken, Expression };
    }

    SyntaxKind AssignmentExpressionSyntax::Kind() const noexcept
    {
        return SyntaxKind::AssignmentExpression;
    }
} // namespace Mamba