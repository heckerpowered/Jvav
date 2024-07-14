#pragma once

#include "MemberSyntax.h"
#include "SeperatedSyntaxList.h"
#include "SyntaxKind.h"

#include <memory>

namespace Mamba
{
    class FunctionDeclarationSyntax : public MemberSyntax
    {
    public:
        using Super = MemberSyntax;

        [[nodiscard]] FunctionDeclarationSyntax(
            const std::shared_ptr<const class SyntaxTree> SyntaxTree,
            const std::shared_ptr<const class SyntaxToken> FunctionKeyword,
            const std::shared_ptr<const class SyntaxToken> Identifier,
            const std::shared_ptr<const class SyntaxToken> OpenParenthesisToken,
            const std::shared_ptr<const SeperatedSyntaxList<const std::shared_ptr<const class SyntaxNode>>> Parameters,
            const std::shared_ptr<const class SyntaxToken> CloseParenthesisToken,
            const NullableSharedPtr<const class TypeClauseSyntax> Type,
            const std::shared_ptr<const class BlockStatementSyntax> Body) noexcept;

        virtual SyntaxKind Kind() const noexcept override;
        virtual std::vector<std::shared_ptr<const class SyntaxNode>> Children() const noexcept override;

        const std::shared_ptr<const class SyntaxToken> FunctionKeyword;
        const std::shared_ptr<const class SyntaxToken> Identifier;
        const std::shared_ptr<const class SyntaxToken> OpenParenthesisToken;
        const std::shared_ptr<const SeperatedSyntaxList<const std::shared_ptr<const class SyntaxNode>>> Parameters;
        const std::shared_ptr<const class SyntaxToken> CloseParenthesisToken;
        const NullableSharedPtr<const class TypeClauseSyntax> Type;
        const std::shared_ptr<const class BlockStatementSyntax> Body;
    };
} // namespace Mamba