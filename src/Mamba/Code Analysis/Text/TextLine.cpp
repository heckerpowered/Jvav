#include "TextLine.h"
#include "SourceText.h"
#include "TextSpan.h"

namespace Mamba
{
    TextLine::TextLine(
        const std::shared_ptr<const SourceText> Text,
        const std::size_t Start,
        const std::size_t Length,
        const std::size_t LengthIncludeLineBreak
    ) noexcept :
        Text(Text), Start(Start), Length(Length), LengthIncludeLineBreak(LengthIncludeLineBreak)
    {
    }

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

    std::shared_ptr<const String> TextLine::ToString() const noexcept
    {
        return Text->Text;
    }
} // namespace Mamba