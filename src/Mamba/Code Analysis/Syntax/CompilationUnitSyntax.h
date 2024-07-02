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
                                            const std::shared_ptr<const class SyntaxToken> EndOfFileToken) noexcept;

        virtual SyntaxKind Kind() const noexcept override;
        virtual std::vector<std::shared_ptr<const SyntaxNode>> Children() const noexcept override;

        const std::shared_ptr<const class SyntaxToken> EndOfFileToken;
    };

} // namespace Mamba