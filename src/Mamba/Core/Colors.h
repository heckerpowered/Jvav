#pragma once

#include <cstdint>
#include <fast_io.h>
#include <utility>

namespace Mamba
{
    enum class Colors : std::uint8_t
    {
        // Foreground colors, which is non-bold and non-bright
        ForegroundBlack = 30,
        ForegroundRed,
        ForegroundGreen,
        ForegroundYellow,
        ForegroundBlue,
        ForegroundMagenta,
        ForegroundCyan,
        ForegroundWhite,
        ForegroundDefault = 39,

        // Background colors, which is non-bold and non-bright
        BackgroundBlack,
        BackgroundRed,
        BackgroundGreen,
        BackgroundYellow,
        BackgroundBlue,
        BackgroundMagenta,
        BackgroundCyan,
        BackgroundWhite,
        BackgroundDefault = 49,

        // Bright foreground colors, which is bold/bright
        BrightForegroundBlack = 90,
        BrightForegroundRed,
        BrightForegroundGreen,
        BrightForegroundYellow,
        BrightForegroundBlue,
        BrightForegroundMagenta,
        BrightForegroundCyan,
        BrightForegroundWhite,

        // Bright background colors, which is bold/bright
        BrightBackgroundBlack = 100,
        BrightBackgroundRed,
        BrightBackgroundGreen,
        BrightBackgroundYellow,
        BrightBackgroundBlue,
        BrightBackgroundMagenta,
        BrightBackgroundCyan,
        BrightBackgroundWhite,
    };

    // Extension namespace for fast_io
    inline namespace FastIO
    {
        template<typename T>
        struct ColorT
        {
            T Reference;
            Colors Color;

            bool IsForeground() const noexcept
            {
                return (Color >= Colors::ForegroundBlack && Color <= Colors::ForegroundWhite) ||
                       (Color >= Colors::BrightForegroundBlack && Color <= Colors::BrightForegroundWhite);
            }

            bool IsBackground() const noexcept
            {
                return (Color >= Colors::BackgroundBlack && Color <= Colors::BackgroundWhite) ||
                       (Color >= Colors::BrightBackgroundBlack && Color <= Colors::BrightBackgroundWhite);
            }
        };

        template<typename T>
        constexpr ColorT<T> Color(T&& Reference, const Colors Color) noexcept
        {
            return {
                .Reference = std::forward<T>(Reference),
                .Color = Color
            };
        }

        template<std::integral char_type, typename T>
        constexpr void print_define(fast_io::io_reserve_type_t<char_type, ::Mamba::ColorT<T>>, auto out [[maybe_unused]], const ::Mamba::ColorT<T>& color) noexcept
        {
            // \033[<n>m<TEXT>\033[0m
            if (color.IsForeground())
            {
                ::fast_io::io::print(out, "\033[", static_cast<std::uint8_t>(color.Color), "m", color.Reference, "\033[39m");
                return;
            }
            else if (color.IsBackground())
            {
                ::fast_io::io::print(out, "\033[", static_cast<std::uint8_t>(color.Color), "m", color.Reference, "\033[49m");
                return;
            }

            std::unreachable();
        }
    } // namespace FastIO

} // namespace Mamba
