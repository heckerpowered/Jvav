#pragma once

#include "fast_io.h"
#include "Literal.h"
#include "MambaCore.h"
#include "SyntaxKind.h"
#include "SyntaxNode.h"

namespace Mamba
{
    class SyntaxTree;

    class SyntaxToken : public SyntaxNode
    {
        StringView PrivateText;
        Literal PrivateValue;
        SyntaxKind PrivateKind;

    public:
        using Super = SyntaxNode;

        [[nodiscard]] SyntaxToken(const class SyntaxTree* SyntaxTree, SyntaxKind Kind, StringView Text, Literal Value = {}) noexcept;

        virtual SyntaxKind Kind() const noexcept override;
        virtual StringView Text() const noexcept override;

        std::size_t ChildrenCount() const noexcept override;
        const SyntaxNode* ChildAt(std::size_t Index) const noexcept override;

        [[nodiscard]] std::size_t Position() const noexcept;
        [[nodiscard]] Literal Value() const noexcept;
    };
} // namespace Mamba