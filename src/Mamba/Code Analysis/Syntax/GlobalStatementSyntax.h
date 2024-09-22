#pragma once

#include "MemberSyntax.h"
#include "StatementSyntax.h"
#include "SyntaxTree.h"

namespace Mamba
{
    class GlobalStatementSyntax : public MemberSyntax
    {
    public:
        using Super = MemberSyntax;

        [[nodiscard]] GlobalStatementSyntax(
            const class SyntaxTree* SyntaxTree,
            const StatementSyntax* Statement
        ) noexcept;

        ~GlobalStatementSyntax() noexcept override;

        SyntaxKind Kind() const noexcept override;

        const StatementSyntax* Statement;

    private:
        std::size_t ChildrenCount() const noexcept override;
        const SyntaxNode* ChildAt(std::size_t Index) const noexcept override;
    };
} // namespace Mamba