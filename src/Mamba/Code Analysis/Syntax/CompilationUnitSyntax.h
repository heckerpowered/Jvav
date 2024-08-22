#pragma once

#include "MemberSyntax.h"
#include "SyntaxNode.h"
#include "SyntaxToken.h"

namespace Mamba
{

    class CompilationUnitSyntax : public SyntaxNode
    {
    public:
        using Super = SyntaxNode;

        [[nodiscard]] CompilationUnitSyntax(
            const class SyntaxTree* SyntaxTree,
            std::vector<MemberSyntax*>&& Members,
            const SyntaxToken* EndOfFileToken
        ) noexcept;

        ~CompilationUnitSyntax() noexcept override;

        SyntaxKind Kind() const noexcept override;
        std::vector<const SyntaxNode*> Children() const noexcept override;

        std::vector<MemberSyntax*> Members;
        const SyntaxToken* EndOfFileToken;
    };

} // namespace Mamba