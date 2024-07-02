#include "TextLocation.h"

#include "SourceText.h"
#include "TextLine.h"
#include "TextSpan.h"

#include <memory>

namespace Mamba
{
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
} // namespace Mamba