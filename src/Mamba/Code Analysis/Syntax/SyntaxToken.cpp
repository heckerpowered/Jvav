#include "SyntaxToken.h"
#include "MambaCore.h"
#include "TextSpan.h"

namespace Mamba
{
    SyntaxToken::SyntaxToken(
        const std::shared_ptr<const class SyntaxTree> SyntaxTree,
        const SyntaxKind Kind,
        const std::size_t Position,
        const std::shared_ptr<const String> Text,
        const NullableSharedPtr<Literal> Value
    ) noexcept :
        Super(SyntaxTree), Position(Position), Text(Text), Value(Value), KindValue(Kind)
    {
    }

    SyntaxKind SyntaxToken::Kind() const noexcept
    {
        return KindValue;
    }

    TextSpan SyntaxToken::Span() const noexcept
    {
        return TextSpan(Position, Text->length());
    }

    TextSpan SyntaxToken::FullSpan() const noexcept
    {
        const auto Start = Span().Start;
        const auto End = Span().End();
        return TextSpan::FromBounds(Start, End);
    }

    std::vector<std::shared_ptr<const SyntaxNode>> SyntaxToken::Children() const noexcept
    {
        return {};
    }
} // namespace Mamba