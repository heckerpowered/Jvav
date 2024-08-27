#include "SyntaxTree.h"

using namespace Mamba;

SyntaxTree::SyntaxTree(const SourceText& SourceText) noexcept :
    PrivateSourceText(SourceText)
{
}

SyntaxTree::~SyntaxTree() noexcept
{
    delete PrivateRoot;
}

NullablePointer<const SyntaxNode> SyntaxTree::Parent(const SyntaxNode& Node) const noexcept
{
    const auto Iterator = ParentsMap.find(&Node);
    if (Iterator == ParentsMap.end())
    {
        return {};
    }

    return Iterator->second;
}

void SyntaxTree::BuildParentsMap(const SyntaxNode& Node) noexcept
{
    for (auto&& Child : Node.Children())
    {
        ParentsMap.emplace(Child, &Node);
        BuildParentsMap(*Child);
    }
}
