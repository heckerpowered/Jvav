#include "TextLocation.h"

#include "SourceText.h"
#include "TextLine.h"
#include "TextSpan.h"

#include <memory>

MAMBA_NAMESPACE_BEGIN

TextLocation::TextLocation(const std::shared_ptr<const class SourceText> Text, const TextSpan Span) noexcept :
    Text(Text), Span(Span)
{
}

std::shared_ptr<const String> TextLocation::FileName() const noexcept
{
    return Text->FileName;
}

std::size_t TextLocation::StartLine() const noexcept
{
    return Text->GetLineIndex(Span.Start);
}

std::size_t TextLocation::StartCharacter() const noexcept
{
    return Span.Start - Text->Lines[StartLine()]->Start;
}

std::size_t TextLocation::EndLine() const noexcept
{
    return Text->GetLineIndex(Span.End());
}

std::size_t TextLocation::EndCharacter() const noexcept
{
    return Span.End() - Text->Lines[EndLine()]->Start;
}

MAMBA_NAMESPACE_END