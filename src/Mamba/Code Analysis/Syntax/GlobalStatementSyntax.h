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
        std::vector<const SyntaxNode*> Children() const noexcept override;

        const StatementSyntax* Statement;
    };
} // namespace Mamba