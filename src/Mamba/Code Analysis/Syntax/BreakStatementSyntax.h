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

        const SyntaxToken* Keyword;

    private:
        std::size_t ChildrenCount() const noexcept override;
        const SyntaxNode* ChildAt(std::size_t Index) const noexcept override;
    };
} // namespace Mamba