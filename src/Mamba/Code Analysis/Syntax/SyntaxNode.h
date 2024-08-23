#pragma once

#include "SyntaxKind.h"
#include "TextLocation.h"

#include <ranges>
#include <vector>

namespace Mamba
{
    class SyntaxTree;

    // SyntaxNode is the minimum meaingful unit in the compilation, represents a syntactic element in the source code.
    class SyntaxNode
    {
        const class SyntaxTree* PrivateSyntaxTree;
        TextLocation PrivateLocation;

    public:
        [[nodiscard]] SyntaxNode(const class SyntaxTree* SyntaxTree) noexcept;
        virtual ~SyntaxNode() noexcept = default;

        [[nodiscard]] NullablePointer<const SyntaxNode> Parent() const noexcept;

        [[nodiscard]] const SyntaxTree* Tree() const noexcept;
        [[nodiscard]] virtual SyntaxKind Kind() const noexcept = 0;

        // Gets the text of the node in the source code, containing the whole node, including all its children.
        [[nodiscard]] virtual StringView Text() const noexcept;

        // Gets the text location of the node in the source code, starts from the first child's beginning and
        // ends at the last child's end.
        [[nodiscard]] TextLocation Location() const noexcept;

        // Gets all children of the node, children are member variables of types that indirectly or directly derived
        // from SyntaxNode, when implementing, the result sequence of children should conform to the sequence in which
        // they were defined within the type. If the member variable is a container whose element type is, or directly
        // or indirectly derived from SyntaxNode, it should be expanded and inserted in its original sequence.
        [[nodiscard]] virtual std::vector<const SyntaxNode*> Children() const noexcept = 0;

        // Gets all the ancestors of the node, including the node itself, in the order of closest to farthest.
        [[nodiscard]] std::vector<const SyntaxNode*> AncestorsAndSelf() const noexcept;

        // Gets all the ancestors of the node, excluding the node itself, in the order of closest to farthest.
        [[nodiscard]] std::vector<const SyntaxNode*> Ancestors() const noexcept;

#if __cpp_lib_ranges_to_container >= 202202L

        template<template<typename...> typename ContainerType>
        [[nodiscard]] auto Children() const noexcept
        {
            return Children() | std::ranges::to<ContainerType>();
        }
#endif
    };
} // namespace Mamba