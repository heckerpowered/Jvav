#pragma once

#include "VertexCore.h"
#include <memory>

_VERTEX_BEGIN

struct TextSpan {
    const std::size_t Start;
    const std::size_t Length;

    [[nodiscard]] std::size_t End() const noexcept;

    [[nodiscard]] TextSpan(const std::size_t Start, const std::size_t Length) noexcept;

    static TextSpan FromBounds(const std::size_t Start, const std::size_t End) noexcept;

    [[nodiscard]] bool OverlapsWith(const TextSpan &Span) const noexcept;

    [[nodiscard]] std::shared_ptr<String> ToString() const noexcept;
};

_VERTEX_END