#include "TextLocation.h"

#include "SourceText.h"
#include "TextLine.h"
#include "TextSpan.h"

using namespace Mamba;

std::size_t TextLocation::StartLine() const noexcept
{
    return Text.LineIndex(Span.Start);
}

std::size_t TextLocation::StartCharacter() const noexcept
{
    return Span.Start - Text.Lines()[StartLine()].Start;
}

std::size_t TextLocation::EndLine() const noexcept
{
    return Text.LineIndex(Span.End());
}

std::size_t TextLocation::EndCharacter() const noexcept
{
    return Span.End() - Text.Lines()[EndLine()].Start;
}