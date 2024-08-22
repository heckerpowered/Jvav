#pragma once

#include "StatementSyntax.h"
#include "SyntaxKind.h"
#include "SyntaxToken.h"

#include <vector>

namespace Mamba
{
    class BlockStatementSyntax : public StatementSyntax
    {
    public:
        using Super = StatementSyntax;

        [[nodiscard]] BlockStatementSyntax(
            const class SyntaxTree* SyntaxTree,
            const SyntaxToken* OpenBraceToken,
            std::vector<StatementSyntax*>&& Statements,
            const SyntaxToken* CloseBraceToken
        ) noexcept;

        ~BlockStatementSyntax() noexcept override;

        SyntaxKind Kind() const noexcept override;
        std::vector<const SyntaxNode*> Children() const noexcept override;

        const SyntaxToken* OpenBraceToken;
        std::vector<StatementSyntax*> Statements;
        const SyntaxToken* CloseBraceToken;
    };
} // namespace Mamba