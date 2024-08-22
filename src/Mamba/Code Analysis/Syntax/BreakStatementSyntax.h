#pragma once

#include "StatementSyntax.h"
#include "SyntaxToken.h"

namespace Mamba
{
    class BreakStatementSyntax : public StatementSyntax
    {
    public:
        using Super = StatementSyntax;

        [[nodiscard]] BreakStatementSyntax(const class SyntaxTree* SyntaxTree, const SyntaxToken* Keyword) noexcept;

        SyntaxKind Kind() const noexcept override;
        std::vector<const SyntaxNode*> Children() const noexcept override;

        const SyntaxToken* Keyword;
    };
} // namespace Mamba