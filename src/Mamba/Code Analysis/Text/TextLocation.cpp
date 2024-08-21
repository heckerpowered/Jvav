#include "TextLocation.h"
#include "SourceText.h"

using namespace Mamba;

std::size_t TextLocation::StartLine() const noexcept
{
    return Text.LineIndex(Text.RelativeBegin(View));
}

std::size_t TextLocation::StartCharacter() const noexcept
{
    return Text.RelativeBegin(View) - Text.Lines()[StartLine()].Start;
}

std::size_t TextLocation::EndLine() const noexcept
{
    return Text.LineIndex(Text.RelativeEnd(View));
}

std::size_t TextLocation::EndCharacter() const noexcept
{
    return Text.RelativeEnd(View) - Text.Lines()[EndLine()].Start;
}