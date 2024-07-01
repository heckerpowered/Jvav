#pragma once

#include "StatementSyntax.h"
#include "SyntaxKind.h"

#include <memory>

namespace Mamba
{
    class ExpressionStatementSyntax : public StatementSyntax
    {
    public:
        using Super = StatementSyntax;

        [[nodiscard]] ExpressionStatementSyntax(
            const std::shared_ptr<const class SyntaxTree> SyntaxTree,
            const std::shared_ptr<const class ExpressionSyntax> Expression) noexcept;

        virtual SyntaxKind Kind() const noexcept override;
        virtual std::vector<std::shared_ptr<const class SyntaxNode>> Children() const noexcept override;

        const std::shared_ptr<const class ExpressionSyntax> Expression;
    };
} // namespace Mamba