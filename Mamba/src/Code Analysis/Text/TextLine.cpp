#include "TextLine.h"
#include "TextSpan.h"

using namespace Mamba;

std::size_t TextLine::End() const noexcept
{
    return Start + Length;
}

TextSpan TextLine::Span() const noexcept
{
    return TextSpan(Start, Length);
}

TextSpan TextLine::SpanIncludeLineBreak() const noexcept
{
    return TextSpan(Start, LengthIncludeLineBreak);
}