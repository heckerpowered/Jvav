#pragma once

#include "SyntaxKind.h"
#include "TextLocation.h"

#include <ranges>
#include <source_location>

namespace Mamba
{
    class SyntaxTree;

    // SyntaxNode is the minimum meaingful unit in the compilation, represents a syntactic element in the source code.
    class SyntaxNode
    {
        const class SyntaxTree* PrivateSyntaxTree;

    public:
        struct AncestorsIterator
        {
            using difference_type = std::ptrdiff_t;

            const SyntaxNode* Current;

            AncestorsIterator& operator++() noexcept;
            AncestorsIterator operator++(int) noexcept;

            const SyntaxNode* operator*() const;

            friend bool operator==(const AncestorsIterator& Left, const AncestorsIterator& Right) noexcept = default;
        };

        struct ChildIterator
        {
            using difference_type = std::ptrdiff_t;
            using value_type = const SyntaxNode*;

            const SyntaxNode* Node;
            std::size_t Index;

            ChildIterator& operator++() noexcept;
            ChildIterator operator++(int) noexcept;
            ChildIterator& operator+=(difference_type Offset) noexcept;
            ChildIterator operator+(const ChildIterator& Other) const noexcept;

            ChildIterator& operator--() noexcept;
            ChildIterator operator--(int) noexcept;
            ChildIterator& operator-=(difference_type Offset) noexcept;
            ChildIterator operator-(difference_type Offset) const noexcept;

            difference_type operator-(const ChildIterator& Other) const noexcept;

            friend std::partial_ordering operator<=>(const ChildIterator& Left, const ChildIterator& Right) noexcept
            {
                return Left.Index <=> Right.Index;
            }

            const SyntaxNode* operator->() const noexcept;
            const SyntaxNode* operator[](difference_type Index) const noexcept;

            const SyntaxNode* operator*() const;

            friend bool operator==(const ChildIterator& Left, const ChildIterator& Right) noexcept = default;

            friend ChildIterator operator+(const ChildIterator& Left, difference_type Offset) noexcept
            {
                auto Temp = Left;
                Temp += Offset;
                return Temp;
            }

            friend ChildIterator operator+(difference_type Offset, const ChildIterator& Right) noexcept
            {
                auto Temp = Right;
                Temp += Offset;
                return Temp;
            }
        };

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
        [[nodiscard]] std::ranges::subrange<ChildIterator> Children() const noexcept;

        // Gets all the ancestors of the node, including the node itself, in the order of closest to farthest.
        [[nodiscard]] std::ranges::subrange<AncestorsIterator> AncestorsAndSelf() const noexcept;

        // Gets all the ancestors of the node, excluding the node itself, in the order of closest to farthest.
        [[nodiscard]] std::ranges::subrange<AncestorsIterator> Ancestors() const noexcept;

    private:
        [[nodiscard]] virtual std::size_t ChildrenCount() const noexcept = 0;
        [[nodiscard]] virtual const SyntaxNode* ChildAt(std::size_t Index) const noexcept = 0;

    protected:
        [[noreturn]] static void ReportChildrenAccessOutOfBounds(std::size_t Index, std::source_location Location = std::source_location::current()) noexcept;
    };

    static_assert(std::input_or_output_iterator<SyntaxNode::AncestorsIterator>);
    static_assert(std::forward_iterator<SyntaxNode::ChildIterator>);
    static_assert(std::bidirectional_iterator<SyntaxNode::ChildIterator>);
    static_assert(std::ranges::common_range<std::ranges::subrange<SyntaxNode::ChildIterator>>);
    static_assert(std::random_access_iterator<SyntaxNode::ChildIterator>);
} // namespace Mamba