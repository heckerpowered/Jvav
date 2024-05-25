/**
 * Vertex, the compiler of the Jvav programming language.
 * Jvav is a programming language first purposed by Mr. Haoyang Zhang, and first implemented by heckerpowered.
 */
#pragma once

#include <fast_io.h>
#include <string>

#include "fast_io_dsal/vector.h"
#include "fast_io_unit/string_impl/concat.h"

#define _VERTEX_BEGIN namespace Vertex {
#define _VERTEX_END }
#define _VERTEX_NAMESPACE namespace Vertex
#define _VERTEX ::Vertex::

/**
 * Character type used in Vertex, possible value are:
 * 0 - char
 * 1 - wchar_t
 * 2 - char16_t
 * 3 - char32_t
 */
#define _VERTEX_CHARACTER_TYPE 2

/**
 * char -> "..."
 * wchar_t -> L"..."
 * char16_t -> u"..."
 * char32_t -> U"..."
 */
#if _VERTEX_CHARACTER_TYPE == 0
#define _VERTEX_TEXT(x) x
#elif _VERTEX_CHARACTER_TYPE == 1
#define _VERTEX_TEXT(x) L##x
#elif _VERTEX_CHARACTER_TYPE == 2
#define _VERTEX_TEXT(x) u##x
#elif _VERTEX_CHARACTER_TYPE == 3
#define _VERTEX_TEXT(x) U##x
#endif

#define TEXT(x) _VERTEX_TEXT(x)

_VERTEX_BEGIN

#if _VERTEX_CHARACTER_TYPE == 0
using Char = char;
#elif _VERTEX_CHARACTER_TYPE == 1
using Char = wchar_t;
#elif _VERTEX_CHARACTER_TYPE == 2
using Char = char16_t;
#elif _VERTEX_CHARACTER_TYPE == 3
using Char = char32_t;
#endif

using String = std::basic_string<Char>;

template <typename T> using Vector = fast_io::vector<T>;

template <typename... T> String Concat(T &&...Args) {
    static_assert(std::is_same_v<Char, char> || std::is_same_v<Char, wchar_t> || std::is_same_v<Char, char16_t> ||
                  std::is_same_v<Char, char32_t>);

    if constexpr (std::is_same_v<Char, char>) {
        return fast_io::concat(std::forward<T>(Args)...);
    } else if constexpr (std::is_same_v<Char, wchar_t>) {
        return fast_io::wconcat(std::forward<T>(Args)...);
    } else if constexpr (std::is_same_v<Char, char16_t>) {
        return fast_io::u16concat(std::forward<T>(Args)...);
    } else if constexpr (std::is_same_v<Char, char32_t>) {
        return fast_io::u32concat(std::forward<T>(Args)...);
    }
}

_VERTEX_END