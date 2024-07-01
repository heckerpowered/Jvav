#include "ExpressionStatementSyntax.h"

#include "ExpressionSyntax.h"

namespace Mamba
{
    ExpressionStatementSyntax::ExpressionStatementSyntax(
        const std::shared_ptr<const class SyntaxTree> SyntaxTree,
        const std::shared_ptr<const class ExpressionSyntax> Expression) noexcept :
        Super(SyntaxTree), Expression(Expression)
    {
    }

    SyntaxKind ExpressionStatementSyntax::Kind() const noexcept
    {
        return SyntaxKind::ExpressionStatement;
    }

    std::vector<std::shared_ptr<const class SyntaxNode>> ExpressionStatementSyntax::Children() const noexcept
    {
        return { Expression };
    }
} // namespace Mamba