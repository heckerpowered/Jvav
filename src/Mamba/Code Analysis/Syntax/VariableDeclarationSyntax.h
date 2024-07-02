#pragma once

#include "MambaCore.h"
#include "StatementSyntax.h"
#include "TypeClauseSyntax.h"

#include <memory>

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
            const NullableSharedPtr<const class TypeClauseSyntax> TypeClauseSyntax,
            const std::shared_ptr<const class SyntaxToken> EqualsToken,
            const std::shared_ptr<const class ExpressionSyntax> Initializer) noexcept;

        virtual SyntaxKind Kind() const noexcept override;
        virtual std::vector<std::shared_ptr<const class SyntaxNode>> Children() const noexcept override;

        const std::shared_ptr<const class SyntaxToken> Keyword;
        const std::shared_ptr<const class SyntaxToken> Identifier;
        const NullableSharedPtr<const class TypeClauseSyntax> TypeClauseSyntax;
        const std::shared_ptr<const class SyntaxToken> EqualsToken;
        const std::shared_ptr<const class ExpressionSyntax> Initializer;
    };
} // namespace Mamba