#include "MambaCore.h"

#include "SourceText.h"
#include "TextLine.h"
#include "TextSpan.h"

#include <cstddef>
#include <memory>

MAMBA_NAMESPACE_BEGIN

SourceText::SourceText(const std::shared_ptr<const String>& Text,
                       const std::shared_ptr<const String>& FileName) noexcept :
    Text(Text), FileName(FileName), Lines(ParseLines(*this, *Text))
{
}

SourceText SourceText::From(const std::shared_ptr<const String> Text,
                            const std::shared_ptr<const String> FileName) noexcept
{
    return SourceText(Text, FileName);
}

std::vector<std::shared_ptr<const TextLine>> SourceText::ParseLines(const SourceText& SourceText,
                                                                    const String& Text) noexcept
{
    auto Result = std::vector<std::shared_ptr<const TextLine>>();

    auto Position = 0uz;
    auto LineStart = 0uz;

    while (Position < Text.length())
    {
        const auto LineBreakWidth = GetLineBreakWidth(Text, Position);

        if (LineBreakWidth == 0)
        {
            ++Position;
        }
        else
        {
            AddLine(Result, SourceText, Position, LineStart, LineBreakWidth);

            Position += LineBreakWidth;
            LineStart = Position;
        }
    }

    if (Position >= LineStart)
    {
        AddLine(Result, SourceText, Position, LineStart, 0);
    }

    return Result;
}

void SourceText::AddLine(std::vector<std::shared_ptr<const class TextLine>>& Result,
                         const SourceText& SourceText,
                         const std::size_t Position,
                         const std::size_t LineStart,
                         const std::size_t LineBreakWidth) noexcept
{
    const auto LineLength = Position - LineStart;
    const auto LineLengthIncludingLineBreak = LineLength + LineBreakWidth;
    const auto Line =
        std::make_shared<TextLine>(SourceText.shared_from_this(), LineStart, LineLength, LineLengthIncludingLineBreak);
    Result.emplace_back(Line);
}

std::size_t SourceText::GetLineBreakWidth(const String& Text, const std::size_t Position) noexcept
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
    return Text->length();
}

std::size_t SourceText::GetLineIndex(const std::size_t Position) const noexcept
{
    auto Lower = 0uz;
    auto Upper = Lines.size() - 1;

    while (Lower <= Upper)
    {
        const auto Index = Lower + (Upper - Lower) / 2;
        const auto Start = Lines[Index]->Start;

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

std::shared_ptr<const String> SourceText::ToString() const noexcept
{
    return Text;
}

std::shared_ptr<String> SourceText::ToString(const std::size_t Start, const std::size_t Length) const noexcept
{
    return std::make_shared<String>(Text->substr(Start, Length));
}

std::shared_ptr<const String> SourceText::ToString(const TextSpan Span) const noexcept
{
    return ToString(Span.Start, Span.Length);
}

MAMBA_NAMESPACE_END