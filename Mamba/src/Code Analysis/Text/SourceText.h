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

#if __cpp_explicit_this_parameter >= 202110L
        template<typename SelfT>
        [[nodiscard]] auto&& operator[](this SelfT&& Self, std::size_t Position) noexcept
        {
            return std::forward_like<SelfT>(Self.PrivateInfo.Text[Position]);
        }
#else
        [[nodiscard]] Char operator[](std::size_t Position) const noexcept
        {
            return PrivateInfo.Text[Position];
        }
#endif

        [[nodiscard]] std::size_t Length() const noexcept;
        [[nodiscard]] std::size_t LineIndex(std::size_t Position) const noexcept;
        [[nodiscard]] const std::vector<TextLine>& Lines() const noexcept;

#if __cpp_explicit_this_parameter >= 202110L
        template<typename SelfT>
        [[nodiscard]] auto&& ToString(this SelfT&& Self) noexcept
        {
            return std::forward_like<SelfT>(Self.PrivateInfo.Text);
        }
#endif

        [[nodiscard]] StringView SubView(std::size_t Start, std::size_t Length) const noexcept;

        [[nodiscard]] std::size_t RelativeBegin(StringView View) const noexcept;
        [[nodiscard]] std::size_t RelativeEnd(StringView View) const noexcept;

    private:
        static std::vector<TextLine> SplitLines(const SourceTextInfo& Info);
    };
} // namespace Mamba