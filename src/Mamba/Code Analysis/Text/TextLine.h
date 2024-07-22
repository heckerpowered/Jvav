#pragma once

#include "MambaCore.h"
#include <cstddef>
#include <memory>

namespace Mamba
{
    class TextLine
    {
    public:
        const std::shared_ptr<const class SourceText> Text;
        const std::size_t Start;
        const std::size_t Length;

        std::size_t End() const noexcept;
        const std::size_t LengthIncludeLineBreak;

        struct TextSpan Span() const noexcept;
        struct TextSpan SpanIncludeLineBreak() const noexcept;

        [[nodiscard]] TextLine(
            const std::shared_ptr<const class SourceText> Text,
            const std::size_t Start,
            const std::size_t Length,
            const std::size_t LengthIncludeLineBreak
        ) noexcept;

        [[nodiscard]] std::shared_ptr<const String> ToString() const noexcept;
    };
} // namespace Mamba