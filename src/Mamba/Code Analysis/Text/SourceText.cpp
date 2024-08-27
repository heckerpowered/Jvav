#include "SourceText.h"

#include "TextLine.h"

#include <cstddef>

using namespace Mamba;

[[nodiscard]] constexpr std::size_t GetLineBreakWidth(StringView Text, std::size_t Position) noexcept
{
    auto Character = Text[Position];
    auto LineBreakCharacter = (Position + 1 >= Text.length() ? TEXT('\0') : Text[Position + 1]);

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
        auto Index = Lower + (Upper - Lower) / 2;
        auto Start = PrivateLines[Index].Start;

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

const std::vector<TextLine>& SourceText::Lines() const noexcept
{
    return PrivateLines;
}

StringView SourceText::SubView(std::size_t Start, std::size_t Length) const noexcept
{
    auto End = Start + Length;
    auto ViewBegin = PrivateInfo.Text.data() + Start;
    auto ViewEnd = PrivateInfo.Text.data() + End;

    return StringView(ViewBegin, ViewEnd);
}

std::size_t SourceText::RelativeBegin(StringView View) const noexcept
{
    return View.data() - PrivateInfo.Text.data();
}

std::size_t SourceText::RelativeEnd(StringView View) const noexcept
{
    return RelativeBegin(View) + View.length();
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
        auto LineBreakWidth = GetLineBreakWidth(Text, Position);
        if (LineBreakWidth == 0)
        {
            ++Position;
        }
        else
        {
            auto LineLength = Position - LineStart;
            auto LineLengthIncludingLineBreak = LineLength + LineBreakWidth;
            Lines.emplace_back(LineStart, LineLength, LineLengthIncludingLineBreak);

            Position += LineBreakWidth;
            LineStart = Position;
        }
    }

    if (Position >= LineStart)
    {
        auto LineLength = Position - LineStart;
        Lines.emplace_back(LineStart, LineLength, LineLength);
    }

    return Lines;
}
