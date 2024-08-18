#include "SourceText.h"

#include "TextLine.h"
#include "TextSpan.h"

#include <cstddef>

using namespace Mamba;

[[nodiscard]] constexpr std::size_t GetLineBreakWidth(const StringView Text, const std::size_t Position) noexcept
{
    const auto Character = Text[Position];
    const auto LineBreakCharacter = (Position + 1 >= Text.length() ? TEXT('\0') : Text[Position + 1]);

    if (Character == TEXT('\r') && LineBreakCharacter == TEXT('\n'))
    {
        return 2;
    }

    if (Character == TEXT('\r') || LineBreakCharacter == TEXT('\n'))
    {
        return 1;
    }

    return 0;
}

std::size_t SourceText::Length() const noexcept
{
    return PrivateInfo.Text.length();
}

std::size_t SourceText::LineIndex(const std::size_t Position) const noexcept
{
#if __cpp_size_t_suffix == 202011L
    auto Lower = 0uz;
#else
    auto Lower = std::size_t();
#endif
    auto Upper = PrivateLines.size() - 1;

    while (Lower <= Upper)
    {
        const auto Index = Lower + (Upper - Lower) / 2;
        const auto Start = PrivateLines[Index].Start;

        if (Position == Start)
        {
            return Index;
        }

        if (Start > Position)
        {
            Upper = Index - 1;
        }
        else
        {
            Lower = Index + 1;
        }
    }

    return Lower - 1;
}

String SourceText::ToString(const std::size_t Start, const std::size_t Length) const noexcept
{
    return PrivateInfo.Text.substr(Start, Length);
}

String SourceText::ToString(const TextSpan Span) const noexcept
{
    return ToString(Span.Start, Span.Length);
}

StringView SourceText::ToView(const std::size_t Start, const std::size_t Length) const noexcept
{
    const auto End = Start + Length;
    const auto ViewBegin = PrivateInfo.Text.data() + Start;
    const auto ViewEnd = PrivateInfo.Text.data() + End;

    return StringView(ViewBegin, ViewEnd);
}

StringView SourceText::ToView(const TextSpan Span) const noexcept
{
    return ToView(Span.Start, Span.Length);
}

SourceText::SourceText(const SourceTextInfo& Info) noexcept :
    PrivateInfo(Info)
{
}

const SourceTextInfo& SourceText::Info() const noexcept
{
    return PrivateInfo;
}

std::vector<TextLine> SourceText::SplitLines(const SourceTextInfo& Info)
{
    auto Lines = std::vector<TextLine>();

#if __cpp_size_t_suffix == 202011L
    auto Position = 0uz;
    auto LineStart = 0uz;
#else
    auto Position = std::size_t();
    auto LineStart = std::size_t();
#endif

    const auto& Text = Info.Text;

    while (Position < Text.length())
    {
        const auto LineBreakWidth = GetLineBreakWidth(Text, Position);
        if (LineBreakWidth == 0)
        {
            ++Position;
        }
        else
        {
            const auto LineLength = Position - LineStart;
            const auto LineLengthIncludingLineBreak = LineLength + LineBreakWidth;
            Lines.emplace_back(LineStart, LineLength, LineLengthIncludingLineBreak);

            Position += LineBreakWidth;
            LineStart = Position;
        }
    }

    if (Position >= LineStart)
    {
        const auto LineLength = Position - LineStart;
        Lines.emplace_back(LineStart, LineLength, LineLength);
    }

    return Lines;
}
