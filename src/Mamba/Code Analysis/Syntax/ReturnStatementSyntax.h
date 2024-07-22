#pragma once

#include "StatementSyntax.h"

#include <memory>

namespace Mamba
{
    class ReturnStatementSyntax : public StatementSyntax
    {
    public:
        using Super = StatementSyntax;

        [[nodiscard]] ReturnStatementSyntax(
            const std::shared_ptr<const class SyntaxTree> SyntaxTree,
            const std::shared_ptr<const class SyntaxToken> ReturnKeyword,
            const NullableSharedPtr<const class ExpressionSyntax> Expression
        ) noexcept;

        SyntaxKind Kind() const noexcept override;
        std::vector<std::shared_ptr<const class SyntaxNode>> Children() const noexcept override;

        const std::shared_ptr<const class SyntaxToken> ReturnKeyword;
        const NullableSharedPtr<const class ExpressionSyntax> Expression;
    };
} // namespace Mamba