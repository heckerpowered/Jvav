#pragma once

#include "VertexCore.h"

#include <memory>

MAMBA_NAMESPACE_BEGIN

struct TextLocation
{
    [[nodiscard]] TextLocation(const std::shared_ptr<const class SourceText> Text, const class TextSpan& Span) noexcept;

    const std::shared_ptr<const class SourceText> Text;
    const std::unique_ptr<const class TextSpan> Span;

    std::shared_ptr<const String> FileName() const noexcept;

    /**
     * Get the line index of the start line
     */
    std::size_t StartLine() const noexcept;

    /**
     * Get position of the start character
     */
    std::size_t StartCharacter() const noexcept;

    /**
     * Get the end line
     */
    std::size_t EndLine() const noexcept;

    /**
     * Get the end position of the character
     */
    std::size_t EndCharacter() const noexcept;
};

MAMBA_NAMESPACE_END