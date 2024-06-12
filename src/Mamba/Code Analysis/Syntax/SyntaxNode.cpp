#include "SyntaxNode.h"
#include "MambaCore.h"
#include "SyntaxTree.h"

#include <memory>
#include <ranges>

MAMBA_NAMESPACE_BEGIN

SyntaxNode::SyntaxNode(const std::shared_ptr<const class SyntaxTree> SyntaxTree) noexcept : SyntaxTree(SyntaxTree) {}

const std::shared_ptr<const class SyntaxNode> SyntaxNode::Parent() const noexcept
{
    return SyntaxTree->GetParent(shared_from_this());
}

TextSpan SyntaxNode::Span() const noexcept
{
    const auto First = Children().front()->Span();
    const auto Last = Children().back()->Span();
    return TextSpan::FromBounds(First.Start, Last.End());
}

TextSpan SyntaxNode::FullSpan() const noexcept
{
    const auto First = Children().front()->FullSpan();
    const auto Last = Children().back()->FullSpan();
    return TextSpan::FromBounds(First.Start, Last.End());
}

TextLocation SyntaxNode::Location() const noexcept
{
    return TextLocation(SyntaxTree->Text, Span());
}

std::vector<std::shared_ptr<const SyntaxNode>> SyntaxNode::AncestorsAndSelf() const noexcept
{
    auto Ancestors = std::vector<std::shared_ptr<const SyntaxNode>>();
    auto Node = shared_from_this();
    while (Node)
    {
        Ancestors.emplace_back(Node);
        Node = Node->Parent();
    }

    return Ancestors;
}

std::vector<std::shared_ptr<const SyntaxNode>> SyntaxNode::Ancestors() const noexcept
{
    return AncestorsAndSelf() | std::views::drop(1) | std::ranges::to<std::vector>();
}

std::shared_ptr<const class SyntaxToken> SyntaxNode::LastToken() const noexcept
{
    if (auto Token = std::dynamic_pointer_cast<class SyntaxToken>(shared_from_this()))
    {
        return Token;
    }

    // A syntax node should always contain at least 1 token.
    return Children().back()->LastToken();
}

MAMBA_NAMESPACE_END