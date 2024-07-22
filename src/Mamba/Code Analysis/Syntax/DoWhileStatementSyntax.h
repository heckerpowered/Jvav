#pragma once

#include "ExpressionSyntax.h"
#include "StatementSyntax.h"
#include "SyntaxKind.h"

#include <memory>

namespace Mamba
{
    class DoWhileStatementSyntax : public StatementSyntax
    {
    public:
        using Super = StatementSyntax;

        [[nodiscard]] DoWhileStatementSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                             const std::shared_ptr<const class SyntaxToken> DoKeyword,
                                             const std::shared_ptr<const class StatementSyntax> Body,
                                             const std::shared_ptr<const class SyntaxToken> WhileKeyword,
                                             const std::shared_ptr<const class ExpressionSyntax> Condition) noexcept;

        SyntaxKind Kind() const noexcept override;
        std::vector<std::shared_ptr<const class SyntaxNode>> Children() const noexcept override;

        const std::shared_ptr<const class SyntaxToken> DoKeyword;
        const std::shared_ptr<const class StatementSyntax> Body;
        const std::shared_ptr<const class SyntaxToken> WhileKeyword;
        const std::shared_ptr<const class ExpressionSyntax> Condition;
    };
} // namespace Mamba