#pragma once

#include "BlockStatementSyntax.h"
#include "MemberSyntax.h"
#include "ParameterSyntax.h"
#include "SeperatedSyntaxList.h"
#include "SyntaxKind.h"
#include "SyntaxToken.h"
#include "TypeClauseSyntax.h"

namespace Mamba
{
    class FunctionDeclarationSyntax : public MemberSyntax
    {
    public:
        using Super = MemberSyntax;

        [[nodiscard]] FunctionDeclarationSyntax(
            const class SyntaxTree* SyntaxTree,
            const SyntaxToken* FunctionKeyword,
            const SyntaxToken* Identifier,
            const SyntaxToken* OpenParenthesisToken,
            SeperatedSyntaxList<const ParameterSyntax*>&& Parameters,
            const SyntaxToken* CloseParenthesisToken,
            NullablePointer<const TypeClauseSyntax> Type,
            const BlockStatementSyntax* Body
        ) noexcept;

        ~FunctionDeclarationSyntax() noexcept override;

        SyntaxKind Kind() const noexcept override;

        const SyntaxToken* FunctionKeyword;
        const SyntaxToken* Identifier;
        const SyntaxToken* OpenParenthesisToken;
        SeperatedSyntaxList<const ParameterSyntax*> Parameters;
        const SyntaxToken* CloseParenthesisToken;
        NullablePointer<const TypeClauseSyntax> Type;
        const BlockStatementSyntax* Body;

    private:
        std::size_t ChildrenCount() const noexcept override;
        const SyntaxNode* ChildAt(std::size_t Index) const noexcept override;
    };
} // namespace Mamba