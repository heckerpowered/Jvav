#include "SyntaxNode.h"

#include "SyntaxToken.h"
#include "SyntaxTree.h"

using namespace Mamba;

SyntaxNode::SyntaxNode(const class SyntaxTree* SyntaxTree) noexcept :
    PrivateSyntaxTree(SyntaxTree)
{
}

NullablePointer<const SyntaxNode> SyntaxNode::Parent() const noexcept
{
    return Tree()->Parent(*this);
}

const SyntaxTree* Mamba::SyntaxNode::Tree() const noexcept
{
    return PrivateSyntaxTree;
}

StringView SyntaxNode::Text() const noexcept
{
    const auto First = Children().front()->Text();
    const auto Last = Children().back()->Text();
    return { First.begin(), Last.end() };
}

TextLocation SyntaxNode::Location() const noexcept
{
    return TextLocation{
        .Text = Tree()->Text(),
        .View = Text(),
    };
}

std::vector<const SyntaxNode*> SyntaxNode::AncestorsAndSelf() const noexcept
{
    auto Ancestors = std::vector<const SyntaxNode*>();
    auto Node = this;
    while (Node)
    {
        Ancestors.emplace_back(Node);
        Node = Node->Parent();
    }

    return Ancestors;
}

std::vector<const SyntaxNode*> SyntaxNode::Ancestors() const noexcept
{
    return AncestorsAndSelf() | std::views::drop(1) | std::ranges::to<std::vector>();
}