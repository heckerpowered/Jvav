#pragma once

#include "MambaCore.h"
#include <concepts>

namespace Mamba
{
    struct LiteralType
    {
        using String = StringView;
        using FloatingPoint = double;
        using Number = std::uint64_t;
        using Boolean = bool;
        using Empty = std::monostate;
    };

    struct Literal
    {
        using ValueType = std::variant<LiteralType::String, LiteralType::FloatingPoint, LiteralType::Number, LiteralType::Boolean, LiteralType::Empty>;

        ValueType Value;

        [[nodiscard]] constexpr Literal() noexcept = default;

        template<typename T>
            requires std::constructible_from<ValueType, T>
        [[nodiscard]] constexpr Literal(T Value) noexcept :
            Value(Value)
        {
        }

        template<typename T>
        [[nodiscard]] constexpr T Get() const noexcept
        {
            return std::get<T>(Value);
        }
    };
} // namespace Mamba