#include "TextSpan.h"

using namespace Mamba;

std::size_t TextSpan::End() const noexcept
{
    return Start + Length;
}

TextSpan TextSpan::FromBounds(const std::size_t Start, const std::size_t End) noexcept
{
    auto Length = End - Start;
    return TextSpan(Start, Length);
}

bool TextSpan::OverlapsWith(const TextSpan Span) const noexcept
{
    return Start < Span.End() && End() > Span.Start;
}

String TextSpan::ToString() const noexcept
{
    return Concat(Start, TEXT(".."), End());
}