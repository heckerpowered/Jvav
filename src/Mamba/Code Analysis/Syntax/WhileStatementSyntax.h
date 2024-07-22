#pragma once

#include "StatementSyntax.h"

#include <memory>

namespace Mamba
{
    class WhileStatementSyntax : public StatementSyntax
    {
    public:
        using Super = StatementSyntax;

        [[nodiscard]] WhileStatementSyntax(
            const std::shared_ptr<const class SyntaxTree> SyntaxTree,
            const std::shared_ptr<const class SyntaxToken> WhileKeyword,
            const std::shared_ptr<const class ExpressionSyntax> Condition,
            const std::shared_ptr<const class StatementSyntax> Body
        ) noexcept;

        SyntaxKind Kind() const noexcept override;
        std::vector<std::shared_ptr<const class SyntaxNode>> Children() const noexcept override;

        const std::shared_ptr<const class SyntaxToken> WhileKeyword;
        const std::shared_ptr<const class ExpressionSyntax> Condition;
        const std::shared_ptr<const class StatementSyntax> Body;
    };
} // namespace Mamba