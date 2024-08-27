#pragma once

#include "SourceText.h"

namespace Mamba
{
    struct TextLocation
    {
        SourceText Text;
        StringView View;

        template<typename SelfT>
        [[nodiscard]] decltype(auto) FileName(this SelfT&& Self) noexcept
        {
            return std::forward_like<SelfT>(Self.Text).Info().FileName;
        }

        [[nodiscard]] std::size_t StartLine() const noexcept;
        [[nodiscard]] std::size_t StartCharacter() const noexcept;
        [[nodiscard]] std::size_t EndLine() const noexcept;
        [[nodiscard]] std::size_t EndCharacter() const noexcept;
    };
} // namespace Mamba