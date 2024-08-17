#pragma once

#include "MambaCore.h"
#include "TextSpan.h"

#include <vector>

namespace Mamba
{
    class SourceText
    {
    public:
        const String Text;
        const String FileName;

    private:
        std::vector<class TextLine> PrivateLines;

    public:
        template<typename SelfT>
        [[nodiscard]] auto&& Lines(this SelfT&& Self) noexcept
        {
            return std::forward_like<SelfT>(Self.PrivateLines);
        }

#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
        template<typename SelfT>
        [[nodiscard]] auto&& operator[](this SelfT&& Self, const std::size_t Position) noexcept
        {
            return std::forward_like<SelfT>(Self.Text[Position]);
        }
#else
        [[nodiscard]] Char operator[](const std::size_t Position) const noexcept
        {
            return Text[Position];
        }
#endif

        [[nodiscard]] std::size_t Length() const noexcept;
        [[nodiscard]] std::size_t LineIndex(const std::size_t Position) const noexcept;

        template<typename SelfT>
        [[nodiscard]] auto&& ToString(this SelfT&& Self) noexcept
        {
            return std::forward<SelfT>(Self.Text);
        }

        [[nodiscard]] String ToString(const std::size_t Start, const std::size_t Length) const noexcept;
        [[nodiscard]] String ToString(const TextSpan Span) const noexcept;

        [[nodiscard]] StringView ToView(const std::size_t Start, const std::size_t Length) const noexcept;
        [[nodiscard]] StringView ToView(const TextSpan Span) const noexcept;

    private:
        void ParseLines() noexcept;
        void AddLine(const std::size_t Position, const std::size_t LineStart, const std::size_t LineBreakWidth) noexcept;
    };
} // namespace Mamba
