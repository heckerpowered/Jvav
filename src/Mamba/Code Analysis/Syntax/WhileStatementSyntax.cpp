#include "WhileStatementSyntax.h"

#include "ExpressionSyntax.h"
#include "SyntaxToken.h"

namespace Mamba
{
    WhileStatementSyntax::WhileStatementSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                               const std::shared_ptr<const class SyntaxToken> WhileKeyword,
                                               const std::shared_ptr<const class ExpressionSyntax> Condition,
                                               const std::shared_ptr<const class StatementSyntax> Body) noexcept :
        Super(SyntaxTree), WhileKeyword(WhileKeyword), Condition(Condition), Body(Body)
    {
    }

    SyntaxKind WhileStatementSyntax::Kind() const noexcept
    {
        return SyntaxKind::WhileStatement;
    }

    std::vector<std::shared_ptr<const class SyntaxNode>> WhileStatementSyntax::Children() const noexcept
    {
        return { WhileKeyword, Condition, Body };
    }
} // namespace Mamba