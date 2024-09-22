#include "Diagnostic.h"
#include "TextLine.h"

using namespace Mamba;

std::size_t Diagnostic::LineNumber() const noexcept
{
    return Location.StartLine() + 1;
}

const TextLine& Diagnostic::Line() const noexcept
{
    auto LineIndex = Location.StartLine();
    return Location.Text.Lines()[LineIndex];
}

StringView Diagnostic::LineView() const noexcept
{
    auto CurrentLine = Line();
    return Location.Text.SubView(CurrentLine.Start, CurrentLine.Length);
}