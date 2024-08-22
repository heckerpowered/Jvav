#pragma once

#include "StatementSyntax.h"
#include "SyntaxNode.h"
#include "SyntaxToken.h"

namespace Mamba
{
    class ElseClauseSyntax : public SyntaxNode
    {
    public:
        using Super = SyntaxNode;

        [[nodiscard]] ElseClauseSyntax(
            const class SyntaxTree* SyntaxTree,
            const SyntaxToken* ElseKeyword,
            const StatementSyntax* ElseStatement
        ) noexcept;

        ~ElseClauseSyntax() noexcept override;

        SyntaxKind Kind() const noexcept override;
        std::vector<const SyntaxNode*> Children() const noexcept override;

        const SyntaxToken* ElseKeyword;
        const StatementSyntax* ElseStatement;
    };
} // namespace Mamba