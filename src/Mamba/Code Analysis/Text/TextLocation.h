#pragma once

#include "TextSpan.h"

#include <memory>

namespace Mamba
{
    struct TextLocation
    {
        [[nodiscard]] TextLocation(const std::shared_ptr<const class SourceText> Text, const TextSpan Span) noexcept;

        const std::shared_ptr<const class SourceText> Text;
        const TextSpan Span;

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
} // namespace Mamba