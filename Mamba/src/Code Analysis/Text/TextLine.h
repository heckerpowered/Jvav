#pragma once

#include <cstddef>

namespace Mamba
{
    class TextLine
    {
    public:
        std::size_t Start;
        std::size_t Length;
        std::size_t LengthIncludeLineBreak;

        std::size_t End() const noexcept;
        struct TextSpan Span() const noexcept;
        struct TextSpan SpanIncludeLineBreak() const noexcept;
    };
} // namespace Mamba