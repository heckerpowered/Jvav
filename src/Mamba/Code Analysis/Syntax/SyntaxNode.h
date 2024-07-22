#pragma once

#include "SyntaxKind.h"
#include "TextLocation.h"
#include "TextSpan.h"

#include <memory>
#include <ranges>
#include <vector>

namespace Mamba
{
    class SyntaxNode : public std::enable_shared_from_this<SyntaxNode>
    {
    protected:
        [[nodiscard]] SyntaxNode(const std::shared_ptr<const class SyntaxTree> SyntaxTree) noexcept;

    public:
        virtual ~SyntaxNode() noexcept;

        const std::shared_ptr<const class SyntaxTree> SyntaxTree;

        [[nodiscard]] const NullableSharedPtr<const class SyntaxNode> Parent() const noexcept;

        [[nodiscard]] virtual SyntaxKind Kind() const noexcept = 0;
        [[nodiscard]] virtual TextSpan Span() const noexcept;
        [[nodiscard]] virtual TextSpan FullSpan() const noexcept;
        [[nodiscard]] TextLocation Location() const noexcept;

        // Gets all children of the node, children are member variables of types that indirectly or directly derived
        // from SyntaxNode, when implementing, the result sequence of children should conform to the sequence in which
        // they were defined within the type. If the member variable is a container whose element type is, or directly
        // or indirectly derived from SyntaxNode, it should be expanded and inserted in its original sequence.
        [[nodiscard]] virtual std::vector<std::shared_ptr<const SyntaxNode>> Children() const noexcept = 0;
        [[nodiscard]] std::vector<std::shared_ptr<const SyntaxNode>> AncestorsAndSelf() const noexcept;
        [[nodiscard]] std::vector<std::shared_ptr<const SyntaxNode>> Ancestors() const noexcept;

#if __cpp_lib_ranges_to_container == 202202L
        template<template<typename...> typename ContainerType> [[nodiscard]] auto Children() const noexcept
        {
            return Children() | std::ranges::to<ContainerType>();
        }
#endif

        [[nodiscard]] std::shared_ptr<const class SyntaxToken> LastToken() const noexcept;

        String ToString() const noexcept;

    private:
        static void PrettyPrint(
            std::basic_stringstream<Char>& Stream,
            const std::shared_ptr<const SyntaxNode> Node,
            String Indent = TEXT(""),
            const bool IsLast = true
        ) noexcept;
    };
} // namespace Mamba