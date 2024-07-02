#include "ReturnStatementSyntax.h"

#include "ExpressionSyntax.h"
#include "SyntaxToken.h"

namespace Mamba
{
    ReturnStatementSyntax::ReturnStatementSyntax(
        const std::shared_ptr<const class SyntaxTree> SyntaxTree,
        const std::shared_ptr<const class SyntaxToken> ReturnKeyword,
        const NullableSharedPtr<const class ExpressionSyntax> Expression) noexcept :
        Super(SyntaxTree), ReturnKeyword(ReturnKeyword), Expression(Expression)
    {
    }

    SyntaxKind ReturnStatementSyntax::Kind() const noexcept
    {
        return SyntaxKind::ReturnStatement;
    }

    std::vector<std::shared_ptr<const class SyntaxNode>> ReturnStatementSyntax::Children() const noexcept
    {
        return { ReturnKeyword, Expression };
    }
} // namespace Mamba