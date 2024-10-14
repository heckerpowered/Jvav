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

        Ignore
    };

    constexpr bool IsForeground(Colors Color) noexcept
    {
        return (Color >= Colors::ForegroundBlack && Color <= Colors::ForegroundWhite) ||
               (Color >= Colors::BrightForegroundBlack && Color <= Colors::BrightForegroundWhite);
    }

    constexpr bool IsBackground(Colors Color) noexcept
    {
        return (Color >= Colors::BackgroundBlack && Color <= Colors::BackgroundWhite) ||
               (Color >= Colors::BrightBackgroundBlack && Color <= Colors::BrightBackgroundWhite);
    }

    struct SelectGraphicRendition
    {
        Colors Color;
        bool Bold : 1;
        bool Underline : 1;
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
        struct SelectGraphicRenditionWrapper
        {
            T Reference;
            SelectGraphicRendition Rendition;
        };

        template<typename T>
        constexpr SelectGraphicRenditionWrapper<T> Color(T&& Reference, Colors Color, bool Bold = false, bool Underline = false) noexcept
        {
            return {
                .Reference = std::forward<T>(Reference),
                .Rendition = {
                              .Color = Color,
                              .Bold = Bold,
                              .Underline = Underline,
                              }
            };
        }

        template<std::integral char_type, typename T>
        constexpr void print_define(fast_io::io_reserve_type_t<char_type, ::Mamba::ColorT<T>>, auto Out [[maybe_unused]], const ::Mamba::ColorT<T>& Color) noexcept
        {
            // \033[<n>m<TEXT>\033[0m
            if (Color.IsForeground())
            {
                ::fast_io::io::print(Out, "\033[", static_cast<std::uint8_t>(Color.Color), "m", Color.Reference, "\033[39m");
                return;
            }
            else if (Color.IsBackground())
            {
                ::fast_io::io::print(Out, "\033[", static_cast<std::uint8_t>(Color.Color), "m", Color.Reference, "\033[49m");
                return;
            }

            std::unreachable();
        }

        template<std::integral char_type, typename T>
        constexpr void print_define(fast_io::io_reserve_type_t<char_type, ::Mamba::SelectGraphicRenditionWrapper<T>>, auto Out, const ::Mamba::SelectGraphicRenditionWrapper<T>& Wrapper) noexcept
        {
            using namespace fast_io::io;

            print(Out, "\033[");
            if (Wrapper.Rendition.Color != Colors::Ignore)
            {
                print(Out, static_cast<std::uint8_t>(Wrapper.Rendition.Color));
            }
            if (Wrapper.Rendition.Bold)
            {
                print(Out, ";1");
            }
            if (Wrapper.Rendition.Underline)
            {
                print(Out, ";4");
            }
            print(Out, "m", Wrapper.Reference, "\033[0m");
        }
    } // namespace FastIO

} // namespace Mamba
