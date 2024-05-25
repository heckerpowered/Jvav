#pragma once

#include "VertexCore.h"
#include <cstddef>
#include <memory>

_VERTEX_BEGIN

class TextLine {
  public:
    const std::shared_ptr<const class SourceText> Text;
    const std::size_t Start;
    const std::size_t Length;

    std::size_t End() const noexcept;
    const std::size_t LengthIncludeLineBreak;

    class TextSpan Span() const noexcept;
    class TextSpan SpanIncludeLineBreak() const noexcept;

    [[nodiscard]] TextLine(const std::shared_ptr<const class SourceText> &Text, std::size_t Start, std::size_t Length,
                           std::size_t LengthIncludeLineBreak) noexcept;

    [[nodiscard]] std::shared_ptr<const String> ToString() const noexcept;
};

_VERTEX_END