#pragma once

#include <vector>

#include "SourceTextInfo.h"
#include "TextLine.h"

namespace Mamba
{
    class SourceText
    {
        SourceTextInfo PrivateInfo;
        std::vector<TextLine> PrivateLines = SplitLines(PrivateInfo);

    public:
        [[nodiscard]] SourceText(const SourceTextInfo& Info) noexcept;

        const SourceTextInfo& Info() const noexcept;

#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
        template<typename SelfT>
        [[nodiscard]] auto&& operator[](this SelfT&& Self, const std::size_t Position) noexcept
        {
            return std::forward_like<SelfT>(Self.PrivateInfo.Text[Position]);
        }
#else
        [[nodiscard]] Char operator[](const std::size_t Position) const noexcept
        {
            return PrivateInfo.Text[Position];
        }
#endif

        [[nodiscard]] std::size_t Length() const noexcept;
        [[nodiscard]] std::size_t LineIndex(const std::size_t Position) const noexcept;

        template<typename SelfT>
        [[nodiscard]] auto&& ToString(this SelfT&& Self) noexcept
        {
            return std::forward_like<SelfT>(Self.PrivateInfo.Text);
        }

        [[nodiscard]] String ToString(const std::size_t Start, const std::size_t Length) const noexcept;
        [[nodiscard]] String ToString(const TextSpan Span) const noexcept;

        [[nodiscard]] StringView ToView(const std::size_t Start, const std::size_t Length) const noexcept;
        [[nodiscard]] StringView ToView(const TextSpan Span) const noexcept;

    private:
        static std::vector<TextLine> SplitLines(const SourceTextInfo& Info);
    };
} // namespace Mamba