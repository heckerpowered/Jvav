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
    auto First = Children().front()->Text();
    auto Last = Children().back()->Text();
    return { First.begin(), Last.end() };
}

TextLocation SyntaxNode::Location() const noexcept
{
    return TextLocation{
        .Text = Tree()->Text(),
        .View = Text(),
    };
}

std::ranges::subrange<SyntaxNode::ChildIterator> SyntaxNode::Children() const noexcept
{
    auto Begin = ChildIterator{ this, 0 };
    auto End = ChildIterator{ this, ChildrenCount() };
    return std::ranges::subrange(Begin, End);
}

std::ranges::subrange<SyntaxNode::AncestorsIterator> SyntaxNode::AncestorsAndSelf() const noexcept
{
    return std::ranges::subrange(AncestorsIterator(this), AncestorsIterator());
}

std::ranges::subrange<SyntaxNode::AncestorsIterator> SyntaxNode::Ancestors() const noexcept
{
    return AncestorsAndSelf() | std::views::drop(1);
}

void SyntaxNode::ReportChildrenAccessOutOfBounds(std::size_t Index, std::source_location Location) noexcept
{
    InternalCompilerError(Location, "Children access out of bounds: ", Index);
}

SyntaxNode::AncestorsIterator& SyntaxNode::AncestorsIterator::operator++() noexcept
{
    Current = Current->Parent();
    return *this;
}

SyntaxNode::AncestorsIterator SyntaxNode::AncestorsIterator::operator++(int) noexcept
{
    auto Temp = *this;
    ++*this;
    return Temp;
}

const SyntaxNode* SyntaxNode::AncestorsIterator::operator*() const
{
    return Current;
}

SyntaxNode::ChildIterator& SyntaxNode::ChildIterator::operator++() noexcept
{
    ++Index;
    return *this;
}

SyntaxNode::ChildIterator SyntaxNode::ChildIterator::operator++(int) noexcept
{
    auto Temp = *this;
    ++*this;
    return Temp;
}

SyntaxNode::ChildIterator& SyntaxNode::ChildIterator::operator+=(difference_type Offset) noexcept
{
    Index += Offset;
    return *this;
}

SyntaxNode::ChildIterator SyntaxNode::ChildIterator::operator+(const ChildIterator& Other) const noexcept
{
    auto Temp = *this;
    Temp += Other.Index;
    return Temp;
}

SyntaxNode::ChildIterator& SyntaxNode::ChildIterator::operator--() noexcept
{
    --Index;
    return *this;
}

SyntaxNode::ChildIterator SyntaxNode::ChildIterator::operator--(int) noexcept
{
    auto Temp = *this;
    --*this;
    return Temp;
}

SyntaxNode::ChildIterator& SyntaxNode::ChildIterator::operator-=(difference_type Offset) noexcept
{
    Index -= Offset;
    return *this;
}

SyntaxNode::ChildIterator SyntaxNode::ChildIterator::operator-(difference_type Offset) const noexcept
{
    auto Temp = *this;
    Temp -= Offset;
    return Temp;
}

SyntaxNode::ChildIterator::difference_type SyntaxNode::ChildIterator::operator-(const ChildIterator& Other) const noexcept
{
    return Index - Other.Index;
}

const SyntaxNode* SyntaxNode::ChildIterator::operator->() const noexcept
{
    return **this;
}

const SyntaxNode* Mamba::SyntaxNode::ChildIterator::operator[](difference_type Index) const noexcept
{
    return Node->ChildAt(Index);
}

const SyntaxNode* SyntaxNode::ChildIterator::operator*() const
{
    return Node->ChildAt(Index);
}