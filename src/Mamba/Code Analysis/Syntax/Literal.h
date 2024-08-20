#pragma once

#include "MambaCore.h"

#include <cstdint>
#include <memory>
#include <variant>
#include <optional>

namespace Mamba
{
    struct LiteralType
    {
        using String = StringView;
        using Character = Char;

        using UnsignedByte = std::uint8_t;
        using UnsignedShort = std::uint16_t;
        using UnsignedInt = std::uint32_t;
        using UnsignedLong = std::uint64_t;
 
        using SignedByte = std::int8_t;
        using SignedShort = std::int16_t;
        using SignedInt = std::int32_t;
        using SignedLong = std::int64_t;
 
        using Double = double;
        using Float = float;
 
        using Boolean = bool;
 
        using Empty = std::monostate;
    };

    struct Literal
    {
        using ValueVariantType = std::variant<
           LiteralType::Empty,

           LiteralType::String, 
           LiteralType::Character, 

           LiteralType::UnsignedByte,
           LiteralType::UnsignedShort,
           LiteralType::UnsignedInt,
           LiteralType::UnsignedLong,

           LiteralType::SignedByte,
           LiteralType::SignedShort,
           LiteralType::SignedInt,
           LiteralType::SignedLong,

           LiteralType::Double,
           LiteralType::Float,

           LiteralType::Boolean
        >;
        ValueVariantType Value;

        std::optional<std::shared_ptr<const String>> StringValue;
        
        [[nodiscard]] Literal()noexcept = default;
        
        template<typename T> requires std::constructible_from<ValueVariantType, const T&>
        [[nodiscard]] Literal(const T Value)noexcept : Value(Value) {}

        [[nodiscard]] Literal(const std::shared_ptr<const String> Value)noexcept : Value(*Value), StringValue(Value) {}

        template<typename T>
        constexpr const T GetValue()const
        {
            return std::get<T>(Value);
        }

        static NullableSharedPtr<const Literal> Negative(const Literal& Literal) noexcept;
        static NullableSharedPtr<const Literal> LogicalNegative(const Literal& Literal) noexcept;
        static NullableSharedPtr<const Literal> OnesComplement(const Literal& Literal) noexcept;

        NullableSharedPtr<const Literal> operator+(const Literal& Other) const noexcept;
        NullableSharedPtr<const Literal> operator-(const Literal& Other) const noexcept;
        NullableSharedPtr<const Literal> operator*(const Literal& Other) const noexcept;
        NullableSharedPtr<const Literal> operator/(const Literal& Other) const noexcept;
        NullableSharedPtr<const Literal> operator&(const Literal& Other) const noexcept;
        NullableSharedPtr<const Literal> operator|(const Literal& Other) const noexcept;
        NullableSharedPtr<const Literal> operator^(const Literal& Other) const noexcept;
        NullableSharedPtr<const Literal> operator&&(const Literal& Other) const noexcept;
        NullableSharedPtr<const Literal> operator||(const Literal& Other) const noexcept;
        NullableSharedPtr<const Literal> operator==(const Literal& Other) const noexcept;
        NullableSharedPtr<const Literal> operator!=(const Literal& Other) const noexcept;
        NullableSharedPtr<const Literal> operator<(const Literal& Other) const noexcept;
        NullableSharedPtr<const Literal> operator<=(const Literal& Other) const noexcept;
        NullableSharedPtr<const Literal> operator>(const Literal& Other) const noexcept;
        NullableSharedPtr<const Literal> operator>=(const Literal& Other) const noexcept;
    };
} // namespace Mamba