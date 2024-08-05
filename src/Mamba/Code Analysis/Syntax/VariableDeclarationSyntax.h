#pragma once

#include <memory>

#include "StatementSyntax.h"

namespace Mamba
{
    class VariableDeclarationSyntax : public StatementSyntax
    {
    public:
        using Super = StatementSyntax;

        [[nodiscard]] VariableDeclarationSyntax(
            const std::shared_ptr<const class SyntaxTree> SyntaxTree,
            const std::shared_ptr<const class SyntaxToken> Keyword,
            const std::shared_ptr<const class SyntaxToken> Identifier,
            const NullableSharedPtr<const class TypeClauseSyntax> TypeClause,
            const std::shared_ptr<const class SyntaxToken> EqualsToken,
            const std::shared_ptr<const class ExpressionSyntax> Initializer
        ) noexcept;

        SyntaxKind Kind() const noexcept override;
        std::vector<std::shared_ptr<const class SyntaxNode>> Children() const noexcept override;

        const std::shared_ptr<const class SyntaxToken> Keyword;
        const std::shared_ptr<const class SyntaxToken> Identifier;
        const NullableSharedPtr<const class TypeClauseSyntax> TypeClause;
        const std::shared_ptr<const class SyntaxToken> EqualsToken;
        const std::shared_ptr<const class ExpressionSyntax> Initializer;
    };
} // namespace Mamba