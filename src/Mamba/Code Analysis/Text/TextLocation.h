#pragma once

#include "SourceText.h"
#include "TextSpan.h"

namespace Mamba
{
    struct TextLocation
    {
        SourceText Text;
        TextSpan Span;

        template<typename SelfT>
        [[nodiscard]] decltype(auto) FileName(this SelfT&& Self) noexcept
        {
            return std::forward_like<SelfT>(Self.Text.FileName);
        }

        [[nodiscard]] std::size_t StartLine() const noexcept;
        [[nodiscard]] std::size_t StartCharacter() const noexcept;
        [[nodiscard]] std::size_t EndLine() const noexcept;
        [[nodiscard]] std::size_t EndCharacter() const noexcept;
    };
} // namespace Mamba