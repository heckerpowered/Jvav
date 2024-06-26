#include "TextLocation.h"

#include "SourceText.h"
#include "TextSpan.h"

#include <memory>

_VERTEX_BEGIN

TextLocation::TextLocation(const std::shared_ptr<const class SourceText> Text, const TextSpan &Span) noexcept
    : Text(Text), Span(std::make_unique<TextSpan>(Span)) {}

std::shared_ptr<const String> TextLocation::FileName() const noexcept { return Text->FileName; }

std::size_t TextLocation::StartLine() const noexcept { return Text->GetLineIndex(Span->Start); }

std::size_t TextLocation::StartCharacter() const noexcept { return std::size_t(); }

_VERTEX_END