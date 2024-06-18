#pragma once

#include "MambaCore.h"
#include "SyntaxKind.h"
#include "TextLocation.h"
#include "TextSpan.h"

#include <memory>
#include <vector>

namespace Mamba
{
    class SyntaxNode : std::enable_shared_from_this<SyntaxNode>
    {
    protected:
        [[nodiscard]] SyntaxNode(const std::shared_ptr<const class SyntaxTree> SyntaxTree) noexcept;

    public:
        const std::shared_ptr<const class SyntaxTree> SyntaxTree;

        [[nodiscard]] const NullableSharedPtr<const class SyntaxNode> Parent() const noexcept;

        [[nodiscard]] virtual SyntaxKind Kind() const noexcept = 0;
        [[nodiscard]] virtual TextSpan Span() const noexcept;
        [[nodiscard]] virtual TextSpan FullSpan() const noexcept;
        [[nodiscard]] TextLocation Location() const noexcept;

        [[nodiscard]] std::vector<std::shared_ptr<const SyntaxNode>> AncestorsAndSelf() const noexcept;
        [[nodiscard]] std::vector<std::shared_ptr<const SyntaxNode>> Ancestors() const noexcept;
        [[nodiscard]] virtual std::vector<std::shared_ptr<const SyntaxNode>> Children() const noexcept = 0;

        [[nodiscard]] std::shared_ptr<const class SyntaxToken> LastToken() const noexcept;
    };
} // namespace Mamba