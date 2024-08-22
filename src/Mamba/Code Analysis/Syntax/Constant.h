#pragma once

#include "MambaCore.h"
#include <cstdint>
#include <variant>

namespace Mamba
{
    struct ConstantType
    {
        using String = StringView;
        using Char = char;

        using Boolean = bool;

        using Float = float;
        using Double = double;

        using Byte = std::byte;
        using Short = std::int16_t;
        using Int = std::int32_t;
        using Long = std::int64_t;

        using UByte = std::uint8_t;
        using UShort = std::uint16_t;
        using UInt = std::uint32_t;
        using ULong = std::uint64_t;

        using Empty = std::monostate;
    };

    struct Constant
    {
        using ValueType = std::variant<ConstantType::String, ConstantType::Char, ConstantType::Boolean, ConstantType::Float, ConstantType::Double, ConstantType::Byte, ConstantType::Short, ConstantType::Int, ConstantType::Long, ConstantType::UByte, ConstantType::UShort, ConstantType::UInt, ConstantType::ULong, ConstantType::Empty>;

        [[nodiscard]] constexpr Constant() noexcept = default;
        template<std::constructible_from<ValueType> T>

        [[nodiscard]] constexpr Constant(T Value) noexcept :
            Value(Value){};

        template<typename T>
        [[nodiscard]] constexpr T Get() const noexcept
        {
            return std::get<T>(Value);
        }

    private:
        ValueType Value;
    };
} // namespace Mamba