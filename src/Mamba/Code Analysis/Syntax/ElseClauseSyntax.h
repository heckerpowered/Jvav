#pragma once

#include "MambaCore.h"

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
            NullablePointer<const StatementSyntax> ElseStatement
        ) noexcept;

        ~ElseClauseSyntax() noexcept override;

        SyntaxKind Kind() const noexcept override;

        const SyntaxToken* ElseKeyword;
        NullablePointer<const StatementSyntax> ElseStatement;

    private:
        std::size_t ChildrenCount() const noexcept override;
        const SyntaxNode* ChildAt(std::size_t Index) const noexcept override;
    };
} // namespace Mamba