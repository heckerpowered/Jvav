#pragma once

#include "SyntaxKind.h"
#include "TextLocation.h"

#include <memory>
#include <ranges>
#include <vector>

namespace Mamba
{
    class SyntaxTree;
    class SyntaxToken;

    class SyntaxNode
    {
        const class SyntaxTree* PrivateSyntaxTree;

    public:
        [[nodiscard]] SyntaxNode(const class SyntaxTree* SyntaxTree) noexcept;
        virtual ~SyntaxNode() noexcept = default;

        [[nodiscard]] NullablePointer<const SyntaxNode> Parent() const noexcept;

        [[nodiscard]] const SyntaxTree* Tree() const noexcept;
        [[nodiscard]] virtual SyntaxKind Kind() const noexcept = 0;
        [[nodiscard]] virtual StringView Text() const noexcept;
        [[nodiscard]] TextLocation Location() const noexcept;

        // Gets all children of the node, children are member variables of types that indirectly or directly derived
        // from SyntaxNode, when implementing, the result sequence of children should conform to the sequence in which
        // they were defined within the type. If the member variable is a container whose element type is, or directly
        // or indirectly derived from SyntaxNode, it should be expanded and inserted in its original sequence.
        [[nodiscard]] virtual std::vector<const SyntaxNode*> Children() const noexcept = 0;
        [[nodiscard]] std::vector<const SyntaxNode*> AncestorsAndSelf() const noexcept;
        [[nodiscard]] std::vector<const SyntaxNode*> Ancestors() const noexcept;

#if defined(__cpp_lib_ranges_to_container) && __cpp_lib_ranges_to_container >= 202202L
        template<template<typename...> typename ContainerType>
        [[nodiscard]] auto Children() const noexcept
        {
            return Children() | std::ranges::to<ContainerType>();
        }
#endif

        [[nodiscard]] const SyntaxToken* LastToken() const noexcept;
    };
} // namespace Mamba