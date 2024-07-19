#pragma once

#include <memory>
#include <string>
#include <utility>

#include <fast_io.h>

#define MAMBA_NAMESPACE_BEGIN \
    namespace Mamba           \
    {
#define MAMBA_NAMESPACE_END }
#define MAMBA ::Mamba::

// Character type used in Mamba, which may be following values:
// 0 - char
// 1 - wchar_t
// 2 - char8_t
// 3 - char16_t
// 4 - char32_t
#define MAMBA_CHARACTER_TYPE 2

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

namespace Mamba
{
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

    static_assert(std::is_same_v<Char, char> || std::is_same_v<Char, wchar_t> || std::is_same_v<Char, char8_t>
                      || std::is_same_v<Char, char16_t> || std::is_same_v<Char, char32_t>,
                  "Unsupported character type");

    using String = std::basic_string<Char>;
    using StringView = std::basic_string_view<Char>;

    // Always same as std::shared_ptr<T>, but semantically nullable (although shared_ptr is also nullable)
    // Use NullableSharedPtr when you want to represent a shared_ptr with a nullable value.
    // All shared_ptr are considered valid by default in this project.
    template<typename T> using NullableSharedPtr = std::shared_ptr<T>;

    // Always same as T*, but semantically nullable (although pointer is also nullable)
    // Use NullablePointer when you want to represent a pointer with a nullable value.
    // All pointers are considered valid by default in this project.
    template<typename T> using NullablePointer = T*;

    namespace Private
    {
    } // namespace Private

    template<typename... T> constexpr String Concat(T&&... Args)
    {
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

        std::unreachable();
    }

    template<typename TargetType, typename... ArgumentTypes> constexpr void To(ArgumentTypes&&... Arguments)
    {
        if constexpr (std::is_same_v<Char, char>)
        {
            return fast_io::to<TargetType>(std::forward<ArgumentTypes>(Arguments)...);
        }
        else if constexpr (std::is_same_v<Char, wchar_t>)
        {
            return fast_io::wto<TargetType>(std::forward<ArgumentTypes>(Arguments)...);
        }
        else if constexpr (std::is_same_v<Char, char8_t>)
        {
            return fast_io::u8to<TargetType>(std::forward<ArgumentTypes>(Arguments)...);
        }
        else if constexpr (std::is_same_v<Char, char16_t>)
        {
            return fast_io::u16to<TargetType>(std::forward<ArgumentTypes>(Arguments)...);
        }
        else if constexpr (std::is_same_v<Char, char32_t>)
        {
            return fast_io::u32to<TargetType>(std::forward<ArgumentTypes>(Arguments)...);
        }

        std::unreachable();
    }

    // Hatcher stores a callable object, so that the result of the call can be emplace constructed into a container by
    // copy elision, the container must support adding new elements by forwarding arguments to the constructor
    // (emplace).
    template<typename T> struct Hatcher
    {
        T Expression;

        constexpr operator decltype(std::declval<T&>()())() noexcept(noexcept(std::declval<T&>()()))
            requires std::same_as<Hatcher<T>&, decltype(*this)>
        {
            return Expression();
        }

        constexpr operator decltype(std::declval<const T&>()())() noexcept(noexcept(std::declval<const T&>()()))
            requires std::same_as<const Hatcher<T>&, decltype(*this)>
        {
            return Expression();
        }

        constexpr operator decltype(std::declval<T&&>()())() noexcept(noexcept(std::declval<T&&>()()))
            requires std::same_as<Hatcher<T>&&, decltype(*this)>
        {
            return std::move(Expression)();
        }

        constexpr operator decltype(std::declval<const T&&>()())() noexcept(noexcept(std::declval<const T&&>()()))
            requires std::same_as<const Hatcher<T>&&, decltype(*this)>
        {
            return std::move(Expression)();
        }
    };

#ifdef __apple_build_version__
    // Potential compiler bug: Apple clang needs deduction guide to use Hatcher
    template<typename T> Hatcher(T) -> Hatcher<T>;
#endif

} // namespace Mamba

#ifdef __cpp_exceptions
    #if defined(_MSC_VER) && (!defined(_HAS_EXCEPTIONS) || _HAS_EXCEPTIONS == 0)
        #define MAMBA_NO_EXCEPTIONS
    #endif
#else
    #define MAMBA_NO_EXCEPTIONS
#endif