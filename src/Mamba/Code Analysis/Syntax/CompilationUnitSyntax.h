#pragma once

#include "SyntaxNode.h"

#include <memory>

namespace Mamba
{

    class CompilationUnitSyntax : public SyntaxNode
    {
    public:
        using Super = SyntaxNode;

        [[nodiscard]] CompilationUnitSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                            const std::vector<std::shared_ptr<const class MemberSyntax>>& Members,
                                            std::shared_ptr<const class SyntaxToken> EndOfFileToken) noexcept;

        [[nodiscard]] CompilationUnitSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                            std::vector<std::shared_ptr<const class MemberSyntax>>&& Members,
                                            std::shared_ptr<const class SyntaxToken> EndOfFileToken) noexcept;

        SyntaxKind Kind() const noexcept override;
        std::vector<std::shared_ptr<const SyntaxNode>> Children() const noexcept override;

        const std::vector<std::shared_ptr<const class MemberSyntax>> Members;
        std::shared_ptr<const class SyntaxToken> EndOfFileToken;
    };

} // namespace Mamba