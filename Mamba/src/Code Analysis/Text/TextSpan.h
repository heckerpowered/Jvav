#pragma once

#include "MambaCore.h"

namespace Mamba
{
    struct TextSpan
    {
        std::size_t Start;
        std::size_t Length;

        static TextSpan FromBounds(const std::size_t Start, const std::size_t End) noexcept;

        [[nodiscard]] std::size_t End() const noexcept;
        [[nodiscard]] bool OverlapsWith(const TextSpan Span) const noexcept;
        [[nodiscard]] String ToString() const noexcept;
    };
} // namespace Mamba