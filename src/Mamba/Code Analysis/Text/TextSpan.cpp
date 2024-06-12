#include "TextSpan.h"
#include <memory>

MAMBA_NAMESPACE_BEGIN

TextSpan::TextSpan(const std::size_t Start, const std::size_t Length) noexcept : Start(Start), Length(Length) {}

std::size_t TextSpan::End() const noexcept
{
    return Start + Length;
}

TextSpan TextSpan::FromBounds(const std::size_t Start, const std::size_t End) noexcept
{
    const auto Length = End - Start;
    return TextSpan(Start, Length);
}

bool TextSpan::OverlapsWith(const TextSpan Span) const noexcept
{
    return Start < Span.End() && End() > Span.Start;
}

std::shared_ptr<String> TextSpan::ToString() const noexcept
{
    return std::make_shared<String>(Concat(Start, TEXT(".."), End()));
}

MAMBA_NAMESPACE_END