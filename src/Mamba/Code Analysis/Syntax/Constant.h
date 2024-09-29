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

        template<typename T>
            requires std::constructible_from<ValueType, T>
        [[nodiscard]] constexpr Constant(T Value) noexcept :
            Value(Value){};

        template<typename T>
        [[nodiscard]] constexpr T Get() const noexcept
        {
            return std::get<T>(Value);
        }

        template<typename T>
        [[nodiscard]] constexpr bool HoldsAlternative() const noexcept
        {
            return std::holds_alternative<T>(Value);
        }

        [[nodiscard]] constexpr ValueType& GetValue() noexcept
        {
            return Value;
        }

        [[nodiscard]] constexpr bool IsValid() const noexcept
        {
            return !Value.valueless_by_exception();
        }

        [[nodiscard]] constexpr Constant operator-() noexcept
        {
            return std::visit(
                []<typename T>(T Value) -> Constant {
                    if constexpr (requires { -Value; })
                    {
                        return -Value;
                    }
                    else
                    {
                        return {};
                    }
                },
                Value
            );
        }

        [[nodiscard]] constexpr Constant operator!() noexcept
        {
            return std::visit(
                []<typename T>(T Value) -> Constant {
                    if constexpr (requires { !Value; })
                    {
                        return !Value;
                    }
                    else
                    {
                        return {};
                    }
                },
                Value
            );
        }

        [[nodiscard]] constexpr Constant operator~() noexcept
        {
            return std::visit(
                []<typename T>(T Value) -> Constant {
                    if constexpr (requires { ~Value; })
                    {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wbool-operation"
                        return ~Value;
#pragma clang diagnostic pop
                    }
                    else
                    {
                        return {};
                    }
                },
                Value
            );
        }

        [[nodiscard]] constexpr Constant operator+(Constant Other) const noexcept
        {
            return std::visit(
                [](auto Left, auto Right) -> Constant {
                    if constexpr (requires { Left + Right; })
                    {
                        return Left + Right;
                    }
                    else
                    {
                        return {};
                    }
                },
                Value,
                Other.Value
            );
        }

        [[nodiscard]] constexpr Constant operator-(Constant Other) const noexcept
        {
            return std::visit(
                [](auto Left, auto Right) -> Constant {
                    if constexpr (requires { Left - Right; })
                    {
                        return Left - Right;
                    }
                    else
                    {
                        return {};
                    }
                },
                Value,
                Other.Value
            );
        }

        [[nodiscard]] constexpr Constant operator*(Constant Other) const noexcept
        {
            return std::visit(
                [](auto Left, auto Right) -> Constant {
                    if constexpr (requires { Left* Right; })
                    {
                        return Left * Right;
                    }
                    else
                    {
                        return {};
                    }
                },
                Value,
                Other.Value
            );
        }

        [[nodiscard]] constexpr Constant operator/(Constant Other) const noexcept
        {
            return std::visit(
                [](auto Left, auto Right) -> Constant {
                    if constexpr (requires { Left + Right; })
                    {
                        return Left + Right;
                    }
                    else
                    {
                        return {};
                    }
                },
                Value,
                Other.Value
            );
        }

        [[nodiscard]] constexpr Constant operator&&(Constant Other) const noexcept
        {
            return std::visit(
                [](auto Left, auto Right) -> Constant {
                    if constexpr (requires { Left&& Right; })
                    {
                        return Left && Right;
                    }
                    else
                    {
                        return {};
                    }
                },
                Value,
                Other.Value
            );
        }

        [[nodiscard]] constexpr Constant operator||(Constant Other) const noexcept
        {
            return std::visit(
                [](auto Left, auto Right) -> Constant {
                    if constexpr (requires { Left || Right; })
                    {
                        return Left || Right;
                    }
                    else
                    {
                        return {};
                    }
                },
                Value,
                Other.Value
            );
        }

        [[nodiscard]] constexpr Constant operator&(Constant Other) const noexcept
        {
            return std::visit(
                [](auto Left, auto Right) -> Constant {
                    if constexpr (requires { Left & Right; })
                    {
                        return Left & Right;
                    }
                    else
                    {
                        return {};
                    }
                },
                Value,
                Other.Value
            );
        }

        [[nodiscard]] constexpr Constant operator|(Constant Other) const noexcept
        {
            return std::visit(
                [](auto Left, auto Right) -> Constant {
                    if constexpr (requires { Left | Right; })
                    {
                        return Left | Right;
                    }
                    else
                    {
                        return {};
                    }
                },
                Value,
                Other.Value
            );
        }

        [[nodiscard]] constexpr Constant operator^(Constant Other) const noexcept
        {
            return std::visit(
                [](auto Left, auto Right) -> Constant {
                    if constexpr (requires { Left ^ Right; })
                    {
                        return Left ^ Right;
                    }
                    else
                    {
                        return {};
                    }
                },
                Value,
                Other.Value
            );
        }

        [[nodiscard]] constexpr Constant operator==(Constant Other) const noexcept
        {
            return Value == Other.Value;
        }

        [[nodiscard]] constexpr Constant operator!=(Constant Other) const noexcept
        {
            return Value != Other.Value;
        }

        [[nodiscard]] constexpr Constant operator<(Constant Other) const noexcept
        {
            return std::visit(
                [](auto Left, auto Right) -> Constant {
                    if constexpr (requires { Left < Right; })
                    {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-compare"
                        return Left < Right;
#pragma clang diagnostic pop
                    }
                    else
                    {
                        return {};
                    }
                },
                Value,
                Other.Value
            );
        }

        [[nodiscard]] constexpr Constant operator<=(Constant Other) const noexcept
        {
            return std::visit(
                [](auto Left, auto Right) -> Constant {
                    if constexpr (requires { Left <= Right; })
                    {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-compare"
                        return Left < Right;
#pragma clang diagnostic pop
                    }
                    else
                    {
                        return {};
                    }
                },
                Value,
                Other.Value
            );
        }

        [[nodiscard]] constexpr Constant operator>(Constant Other) const noexcept
        {
            return std::visit(
                [](auto Left, auto Right) -> Constant {
                    if constexpr (requires { Left > Right; })
                    {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-compare"
                        return Left > Right;
#pragma clang diagnostic pop
                    }
                    else
                    {
                        return {};
                    }
                },
                Value,
                Other.Value
            );
        }

        [[nodiscard]] constexpr Constant operator>=(Constant Other) const noexcept
        {
            return std::visit(
                [](auto Left, auto Right) -> Constant {
                    if constexpr (requires { Left >= Right; })
                    {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-compare"
                        return Left > Right;
#pragma clang diagnostic pop
                    }
                    else
                    {
                        return {};
                    }
                },
                Value,
                Other.Value
            );
        }

    private:
        ValueType Value;
    };
} // namespace Mamba
