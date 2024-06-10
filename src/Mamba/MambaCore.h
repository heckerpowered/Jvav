/**
 * Mamba, the compiler of the Jvav programming language.
 * Jvav is a programming language first purposed by Mr. Haoyang Zhang, and first implemented by heckerpowered.
 */
#pragma once

#include <fast_io.h>
#include <string>

#include "fast_io_unit/string_impl/concat.h"

#define MAMBA_NAMESPACE_BEGIN \
    namespace Mamba           \
    {
#define MAMBA_NAMESPACE_END }
#define MAMBA ::Mamba::

/**
 * Character type used in Vertex, possible value are:
 * 0 - char
 * 1 - wchar_t
 * 2 - char8_t
 * 3 - char16_t
 * 4 - char32_t
 */
#define MAMBA_CHARACTER_TYPE 2

/**
 * char -> "..."
 * wchar_t -> L"..."
 * char8_t -> u8"..."
 * char16_t -> u"..."
 * char32_t -> U"..."
 */
#if MAMBA_CHARACTER_TYPE == 0
    #define MAMBA_TEXT(x) x
#elif MAMBA_CHARACTER_TYPE == 1
    #define MAMBA_TEXT(x) L##x
#elif MAMBA_CHARACTER_TYPE == 2
    #define MAMBA_TEXT(x) u8##x
#elif MAMBA_CHARACTER_TYPE == 3
    #define MAMBA_TEXT(x) u##x
#elif MAMBA_CHARACTER_TYPE == 4
    #define MAMBA_TEXT(x) U##x
#endif

#define TEXT(x) MAMBA_TEXT(x)

MAMBA_NAMESPACE_BEGIN

#if MAMBA_CHARACTER_TYPE == 0
using Char = char;
#elif MAMBA_CHARACTER_TYPE == 1
using Char = wchar_t;
#elif MAMBA_CHARACTER_TYPE == 2
using Char = char8_t;
#elif MAMBA_CHARACTER_TYPE == 3
using Char = char16_t;
#elif MAMBA_CHARACTER_TYPE == 4
using Char = char32_t;
#endif

using String = std::basic_string<Char>;

template<typename... T> String Concat(T&&... Args)
{
    static_assert(std::is_same_v<Char, char> || std::is_same_v<Char, wchar_t> || std::is_same_v<Char, char8_t>
                  || std::is_same_v<Char, char16_t> || std::is_same_v<Char, char32_t>);
    if constexpr (std::is_same_v<Char, char>)
    {
        return fast_io::concat(std::forward<T>(Args)...);
    }
    else if constexpr (std::is_same_v<Char, wchar_t>)
    {
        return fast_io::wconcat(std::forward<T>(Args)...);
    }
    else if constexpr (std::is_same_v<Char, char8_t>)
    {
        return fast_io::u8concat(std::forward<T>(Args)...);
    }
    else if constexpr (std::is_same_v<Char, char16_t>)
    {
        return fast_io::u16concat(std::forward<T>(Args)...);
    }
    else if constexpr (std::is_same_v<Char, char32_t>)
    {
        return fast_io::u32concat(std::forward<T>(Args)...);
    }
}

template<typename T> struct Hatcher
{
    T Hatcher;

    constexpr operator decltype(std::declval<T&>()())() noexcept(noexcept(std::declval<T&>()()))
        requires std::same_as<MAMBA Hatcher<T>&, decltype(*this)>
    {
        return Hatcher();
    }

    constexpr operator decltype(std::declval<const T&>()())() noexcept(noexcept(std::declval<const T&>()()))
        requires std::same_as<const MAMBA Hatcher<T>&, decltype(*this)>
    {
        return Hatcher();
    }

    constexpr operator decltype(std::declval<T&&>()())() noexcept(noexcept(std::declval<T&&>()()))
        requires std::same_as<MAMBA Hatcher<T>&&, decltype(*this)>
    {
        return std::move(Hatcher)();
    }

    constexpr operator decltype(std::declval<const T&&>()())() noexcept(noexcept(std::declval<const T&&>()()))
        requires std::same_as<const MAMBA Hatcher<T>&&, decltype(*this)>
    {
        return std::move(Hatcher)();
    }
};

MAMBA_NAMESPACE_END