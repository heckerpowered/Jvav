#include "SourceText.h"

#include "TextLine.h"
#include "TextSpan.h"

#include <cstddef>
#include <memory>

namespace Mamba
{
    SourceText::SourceText(
        const std::shared_ptr<const String> Text,
        const std::shared_ptr<const String> FileName
    ) noexcept :
        Text(Text), FileName(FileName), Lines(ParseLines(*this, *Text))
    {
    }

    SourceText SourceText::From(
        const std::shared_ptr<const String> Text,
        const std::shared_ptr<const String> FileName
    ) noexcept
    {
        return SourceText(Text, FileName);
    }

    std::vector<std::shared_ptr<const TextLine>>
        SourceText::ParseLines(const SourceText& SourceText, const String& Text) noexcept
    {
        auto Result = std::vector<std::shared_ptr<const TextLine>>();

#if __cpp_size_t_suffix == 202011L
        auto Position = 0uz;
        auto LineStart = 0uz;
#else
        auto Position = std::size_t();
        auto LineStart = std::size_t();
#endif

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

    void SourceText::AddLine(
        std::vector<std::shared_ptr<const class TextLine>>& Result,
        const SourceText& SourceText,
        const std::size_t Position,
        const std::size_t LineStart,
        const std::size_t LineBreakWidth
    ) noexcept
    {
        const auto LineLength = Position - LineStart;
        const auto LineLengthIncludingLineBreak = LineLength + LineBreakWidth;
        const auto Line = std::make_shared<TextLine>(
            std::make_shared<const class SourceText>(SourceText), LineStart, LineLength, LineLengthIncludingLineBreak
        );
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
#if __cpp_size_t_suffix == 202011L
        auto Lower = 0uz;
#else
        auto Lower = std::size_t();
#endif
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

    StringView SourceText::ToView(const std::size_t Start, const std::size_t Length) const noexcept
    {
        const auto End = Start + Length;
        const auto ViewBegin = Text->data() + Start;
        const auto ViewEnd = Text->data() + End;

        return StringView(ViewBegin, ViewEnd);
    }

    StringView SourceText::ToView(const TextSpan Span) const noexcept
    {
        return ToView(Span.Start, Span.Length);
    }
} // namespace Mamba