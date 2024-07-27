#include "DoWhileStatementSyntax.h"

#include "SyntaxToken.h"

namespace Mamba
{
    DoWhileStatementSyntax::DoWhileStatementSyntax(
        const std::shared_ptr<const class SyntaxTree> SyntaxTree,
        const std::shared_ptr<const class SyntaxToken> DoKeyword,
        const std::shared_ptr<const class StatementSyntax> Body,
        const std::shared_ptr<const class SyntaxToken> WhileKeyword,
        const std::shared_ptr<const class ExpressionSyntax> Condition
    ) noexcept :
        Super(SyntaxTree), DoKeyword(DoKeyword), Body(Body), WhileKeyword(WhileKeyword), Condition(Condition)
    {
    }

    SyntaxKind DoWhileStatementSyntax::Kind() const noexcept
    {
        return SyntaxKind::DoWhileStatement;
    }

    std::vector<std::shared_ptr<const class SyntaxNode>> DoWhileStatementSyntax::Children() const noexcept
    {
        return { DoKeyword, Body, WhileKeyword, Condition };
    }

} // namespace Mamba