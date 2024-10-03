#include "TextLocation.h"
#include "SourceText.h"

using namespace Mamba;

std::size_t TextLocation::StartLine() const noexcept
{
    return Text.LineIndex(Text.RelativeBegin(View));
}

std::size_t TextLocation::AbsoluteStartCharacter() const noexcept
{
    return Text.RelativeBegin(View);
}

std::size_t TextLocation::RelativeStartCharacter() const noexcept
{
    auto Line = Text.Lines()[StartLine()];
    return AbsoluteStartCharacter() - Line.Start;
}

std::size_t TextLocation::EndLine() const noexcept
{
    return Text.LineIndex(Text.RelativeEnd(View));
}

std::size_t TextLocation::AbsoluteEndCharacter() const noexcept
{
    return Text.RelativeEnd(View);
}

std::size_t TextLocation::RelativeEndCharacter() const noexcept
{
    auto Line = Text.Lines()[EndLine()];
    return AbsoluteEndCharacter() - Line.End();
}