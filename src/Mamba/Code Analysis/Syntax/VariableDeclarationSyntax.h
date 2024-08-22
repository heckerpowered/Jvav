#pragma once

#include "ExpressionSyntax.h"
#include "StatementSyntax.h"
#include "SyntaxToken.h"
#include "TypeClauseSyntax.h"

namespace Mamba
{
    class VariableDeclarationSyntax : public StatementSyntax
    {
    public:
        using Super = StatementSyntax;

        [[nodiscard]] VariableDeclarationSyntax(
            const class SyntaxTree* SyntaxTree,
            const SyntaxToken* Keyword,
            const SyntaxToken* Identifier,
            NullablePointer<const TypeClauseSyntax> TypeClause,
            const SyntaxToken* EqualsToken,
            const ExpressionSyntax* Initializer
        ) noexcept;

        ~VariableDeclarationSyntax() noexcept override;

        SyntaxKind Kind() const noexcept override;
        std::vector<const SyntaxNode*> Children() const noexcept override;

        const SyntaxToken* Keyword;
        const SyntaxToken* Identifier;
        NullablePointer<const TypeClauseSyntax> TypeClause;
        const SyntaxToken* EqualsToken;
        const ExpressionSyntax* Initializer;
    };
} // namespace Mamba