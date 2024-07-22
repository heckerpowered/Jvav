#pragma once

#include "StatementSyntax.h"
#include <memory>

namespace Mamba
{
    class BreakStatementSyntax : public StatementSyntax
    {
    public:
        using Super = StatementSyntax;

        [[nodiscard]] BreakStatementSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                           const std::shared_ptr<const class SyntaxToken> Keyword) noexcept;

        SyntaxKind Kind() const noexcept override;
        std::vector<std::shared_ptr<const class SyntaxNode>> Children() const noexcept override;

        const std::shared_ptr<const class SyntaxToken> Keyword;
    };
} // namespace Mamba