#include "SyntaxToken.h"
#include "SyntaxTree.h"

using namespace Mamba;

SyntaxToken::SyntaxToken(const SyntaxTree* SyntaxTree, SyntaxKind Kind, StringView Text, Literal Value) noexcept :
    Super(SyntaxTree), PrivateText(Text), PrivateValue(Value), PrivateKind(Kind)
{
}

SyntaxKind SyntaxToken::Kind() const noexcept
{
    return PrivateKind;
}

StringView SyntaxToken::Text() const noexcept
{
    return PrivateText;
}

std::size_t SyntaxToken::ChildrenCount() const noexcept
{
    return 0;
}

const SyntaxNode* SyntaxToken::ChildAt(std::size_t Index [[maybe_unused]]) const noexcept
{
    return {};
}

std::size_t SyntaxToken::Position() const noexcept
{
    return Tree()->Text().RelativeBegin(Text());
}

Literal SyntaxToken::Value() const noexcept
{
    return PrivateValue;
}