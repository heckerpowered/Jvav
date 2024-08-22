#include "SyntaxToken.h"
#include "MambaCore.h"
#include "SyntaxTree.h"

using namespace Mamba;

SyntaxToken::SyntaxToken(const SyntaxTree* SyntaxTree, SyntaxKind Kind, StringView Text, Literal Value) noexcept :
    Super(SyntaxTree), PrivateKind(Kind), PrivateText(Text), PrivateValue(Value)
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

std::vector<const SyntaxNode*> SyntaxToken::Children() const noexcept
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