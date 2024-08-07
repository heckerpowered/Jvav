#pragma once

#include "MambaCore.h"
#include "SyntaxKind.h"
#include "SyntaxNode.h"
#include "TextSpan.h"
#include <memory>

namespace Mamba
{
    class SyntaxToken : public SyntaxNode
    {
        using Super = SyntaxNode;

    public:
        [[nodiscard]] SyntaxToken(
            const std::shared_ptr<const class SyntaxTree> SyntaxTree,
            const SyntaxKind Kind,
            const std::size_t Position,
            const std::shared_ptr<const String> Text,
            const NullableSharedPtr<struct Literal> Value
        ) noexcept;

        virtual SyntaxKind Kind() const noexcept override;

        virtual TextSpan Span() const noexcept override;
        virtual TextSpan FullSpan() const noexcept override;

        virtual std::vector<std::shared_ptr<const SyntaxNode>> Children() const noexcept override;

        std::size_t Position;
        const std::shared_ptr<const String> Text;
        const NullableSharedPtr<Literal> Value;

    private:
        SyntaxKind KindValue;
    };
} // namespace Mamba