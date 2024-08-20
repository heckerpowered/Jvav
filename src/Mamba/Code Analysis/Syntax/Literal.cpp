#include "Literal.h"

#include <fast_io.h>

#include "MambaCore.h"

using namespace Mamba;

NullableSharedPtr<const Literal> Literal::Negative(const Literal& Literal) noexcept
{
    return std::visit([]<typename T>(const T& Value)
    {
        if constexpr(std::signed_integral<T> || std::floating_point<T>)
        {
            return std::make_shared<const struct Literal>(-Value); 
        }
        else
        {
            return std::make_shared<const struct Literal>(); 
        }
    }, Literal.Value);
}

NullableSharedPtr<const Literal> Literal::LogicalNegative(const Literal& Literal) noexcept
{
    return std::visit([]<typename T>(const T& Value)
    {
        if constexpr(std::same_as<T, LiteralType::Boolean>)
        {
            return std::make_shared<const struct Literal>(!Value); 
        }
        else
        {
            return std::make_shared<const struct Literal>(); 
        }
    }, Literal.Value);
}

NullableSharedPtr<const Literal> Literal::OnesComplement(const Literal& Literal) noexcept
{
    return std::visit([]<typename T>(const T& Value)
    {
        if constexpr(std::integral<T> && !std::same_as<T, LiteralType::Boolean> && !std::same_as<T, LiteralType::Character>)
        {
            return std::make_shared<const struct Literal>(~Value); 
        }
        else
        {
            return std::make_shared<const struct Literal>(); 
        }
    }, Literal.Value);
}

static void MatchOperandType(const Literal& Left, const Literal& Right) noexcept
{
    if (Left.Value.index() != Right.Value.index())
    {
#ifdef DEBUG
        fast_io::io::perrln(
            "Operand type not match, ",
            Left.Value.index(),
            " != ",
            Right.Value.index()
        );
#else
        std::unreachable();
#endif
    }
}

NullableSharedPtr<const Literal> Literal::operator+(const Literal& Other) const noexcept
{
    MatchOperandType(*this, Other);
    return std::visit([]<typename RT, typename LT>(const RT& Value, const LT& OtherValue)
    {
        if constexpr(!std::same_as<RT, LT>)
        {
            return std::make_shared<const struct Literal>();
        }
        else if constexpr(std::same_as<RT, LiteralType::String>)
        {
            return std::make_shared<const struct Literal>(Hatcher{ [&]{ return std::make_shared<const String>(Concat(Value, OtherValue)); } }); 
        }
        else if constexpr(std::is_arithmetic_v<RT> && !std::same_as<RT, LiteralType::Boolean> && !std::same_as<RT, LiteralType::Character>)
        {
            return std::make_shared<const struct Literal>(Value + OtherValue); 
        }
        else
        {
            return std::make_shared<const struct Literal>(); 
        }
    }, Value, Other.Value);
}

NullableSharedPtr<const Literal> Literal::operator-(const Literal& Other) const noexcept
{
    MatchOperandType(*this, Other);
    return std::visit([]<typename RT, typename LT>(const RT& Value, const LT& OtherValue)
    {
        if constexpr(!std::same_as<RT, LT>)
        {
            return std::make_shared<const struct Literal>();
        }
        else if constexpr(std::is_arithmetic_v<RT> && !std::same_as<RT, LiteralType::Boolean> && !std::same_as<RT, LiteralType::Character>)
        {
            return std::make_shared<const struct Literal>(Value - OtherValue); 
        }
        else
        {
            return std::make_shared<const struct Literal>(); 
        }
    }, Value, Other.Value);
}

NullableSharedPtr<const Literal> Literal::operator*(const Literal& Other) const noexcept
{
    MatchOperandType(*this, Other);
    return std::visit([]<typename RT, typename LT>(const RT& Value, const LT& OtherValue)
    {
        if constexpr(!std::same_as<RT, LT>)
        {
            return std::make_shared<const struct Literal>();
        }
        else if constexpr(std::is_arithmetic_v<RT> && !std::same_as<RT, LiteralType::Boolean> && !std::same_as<RT, LiteralType::Character>)
        {
            return std::make_shared<const struct Literal>(Value * OtherValue); 
        }
        else
        {
            return std::make_shared<const struct Literal>(); 
        }
    }, Value, Other.Value);
}

NullableSharedPtr<const Literal> Literal::operator/(const Literal& Other) const noexcept
{
    MatchOperandType(*this, Other);
    return std::visit([]<typename RT, typename LT>(const RT& Value, const LT& OtherValue)
    {
        if constexpr(!std::same_as<RT, LT>)
        {
            return std::make_shared<const struct Literal>();
        }
        else if constexpr(std::is_arithmetic_v<RT> && !std::same_as<RT, LiteralType::Boolean> && !std::same_as<RT, LiteralType::Character>)
        {
            return std::make_shared<const struct Literal>(Value / OtherValue); 
        }
        else
        {
            return std::make_shared<const struct Literal>(); 
        }
    }, Value, Other.Value);
}

NullableSharedPtr<const Literal> Literal::operator&(const Literal& Other) const noexcept
{
    MatchOperandType(*this, Other);
    return std::visit([]<typename RT, typename LT>(const RT& Value, const LT& OtherValue)
    {
        if constexpr(!std::same_as<RT, LT>)
        {
            return std::make_shared<const struct Literal>();
        }
        else if constexpr(std::integral<RT> && !std::same_as<RT, LiteralType::Character>)
        {
            return std::make_shared<const struct Literal>(Value & OtherValue); 
        }
        else
        {
            return std::make_shared<const struct Literal>(); 
        }
    }, Value, Other.Value);
}

NullableSharedPtr<const Literal> Literal::operator|(const Literal& Other) const noexcept
{
    MatchOperandType(*this, Other);
    return std::visit([]<typename RT, typename LT>(const RT& Value, const LT& OtherValue)
    {
        if constexpr(!std::same_as<RT, LT>)
        {
            return std::make_shared<const struct Literal>();
        }
        else if constexpr(std::integral<RT> && !std::same_as<RT, LiteralType::Character>)
        {
            return std::make_shared<const struct Literal>(Value | OtherValue); 
        }
        else
        {
            return std::make_shared<const struct Literal>(); 
        }
    }, Value, Other.Value);
}

NullableSharedPtr<const Literal> Literal::operator^(const Literal& Other) const noexcept
{
    MatchOperandType(*this, Other);
    return std::visit([]<typename RT, typename LT>(const RT& Value, const LT& OtherValue)
    {
        if constexpr(!std::same_as<RT, LT>)
        {
            return std::make_shared<const struct Literal>();
        }
        else if constexpr(std::integral<RT> && !std::same_as<RT, LiteralType::Character>)
        {
            return std::make_shared<const struct Literal>(Value ^ OtherValue); 
        }
        else
        {
            return std::make_shared<const struct Literal>(); 
        }
    }, Value, Other.Value);
}

NullableSharedPtr<const Literal> Literal::operator&&(const Literal& Other) const noexcept
{
    MatchOperandType(*this, Other);
    return std::visit([]<typename RT, typename LT>(const RT& Value, const LT& OtherValue)
    {
        if constexpr(!std::same_as<RT, LT>)
        {
            return std::make_shared<const struct Literal>();
        }
        else if constexpr(std::same_as<RT, LiteralType::Boolean>)
        {
            return std::make_shared<const struct Literal>(Value && OtherValue); 
        }
        else
        {
            return std::make_shared<const struct Literal>(); 
        }
    }, Value, Other.Value);
}

NullableSharedPtr<const Literal> Literal::operator||(const Literal& Other) const noexcept
{
    MatchOperandType(*this, Other);
    return std::visit([]<typename RT, typename LT>(const RT& Value, const LT& OtherValue)
    {
        if constexpr(!std::same_as<RT, LT>)
        {
            return std::make_shared<const struct Literal>();
        }
        else if constexpr(std::same_as<RT, LiteralType::Boolean>)
        {
            return std::make_shared<const struct Literal>(Value || OtherValue); 
        }
        else
        {
            return std::make_shared<const struct Literal>(); 
        }
    }, Value, Other.Value);
}

NullableSharedPtr<const Literal> Literal::operator==(const Literal& Other) const noexcept
{
    MatchOperandType(*this, Other);
    return std::visit([]<typename RT, typename LT>(const RT& Value, const LT& OtherValue)
    {
        if constexpr(!std::same_as<RT, LT>)
        {
            return std::make_shared<const struct Literal>();
        }
        else if constexpr(!std::same_as<RT, LiteralType::Empty>)
        {
            return std::make_shared<const struct Literal>(Value == OtherValue); 
        }
        else
        {
            return std::make_shared<const struct Literal>(); 
        }
    }, Value, Other.Value);
}

NullableSharedPtr<const Literal> Literal::operator!=(const Literal& Other) const noexcept
{
    MatchOperandType(*this, Other);
    return std::visit([]<typename RT, typename LT>(const RT& Value, const LT& OtherValue)
    {
        if constexpr(!std::same_as<RT, LT>)
        {
            return std::make_shared<const struct Literal>();
        }
        else if constexpr(!std::same_as<RT, LiteralType::Empty>)
        {
            return std::make_shared<const struct Literal>(Value != OtherValue); 
        }
        else
        {
            return std::make_shared<const struct Literal>(); 
        }
    }, Value, Other.Value);
}

NullableSharedPtr<const Literal> Literal::operator<(const Literal& Other) const noexcept
{
    MatchOperandType(*this, Other);
    return std::visit([]<typename RT, typename LT>(const RT& Value, const LT& OtherValue)
    {
        if constexpr(!std::same_as<RT, LT>)
        {
            return std::make_shared<const struct Literal>();
        }
        else if constexpr(std::is_arithmetic_v<RT> && !std::same_as<RT, LiteralType::Boolean> && !std::same_as<RT, LiteralType::Character>)
        {
            return std::make_shared<const struct Literal>(Value < OtherValue); 
        }
        else
        {
            return std::make_shared<const struct Literal>(); 
        }
    }, Value, Other.Value);
}

NullableSharedPtr<const Literal> Literal::operator<=(const Literal& Other) const noexcept
{
    MatchOperandType(*this, Other);
    return std::visit([]<typename RT, typename LT>(const RT& Value, const LT& OtherValue)
    {
        if constexpr(!std::same_as<RT, LT>)
        {
            return std::make_shared<const struct Literal>();
        }
        else if constexpr(std::is_arithmetic_v<RT> && !std::same_as<RT, LiteralType::Boolean> && !std::same_as<RT, LiteralType::Character>)
        {
            return std::make_shared<const struct Literal>(Value <= OtherValue); 
        }
        else
        {
            return std::make_shared<const struct Literal>(); 
        }
    }, Value, Other.Value);
}

NullableSharedPtr<const Literal> Literal::operator>(const Literal& Other) const noexcept
{
    MatchOperandType(*this, Other);
    return std::visit([]<typename RT, typename LT>(const RT& Value, const LT& OtherValue)
    {
        if constexpr(!std::same_as<RT, LT>)
        {
            return std::make_shared<const struct Literal>();
        }
        else if constexpr(std::is_arithmetic_v<RT> && !std::same_as<RT, LiteralType::Boolean> && !std::same_as<RT, LiteralType::Character>)
        {
            return std::make_shared<const struct Literal>(Value > OtherValue); 
        }
        else
        {
            return std::make_shared<const struct Literal>(); 
        }
    }, Value, Other.Value);
}

NullableSharedPtr<const Literal> Literal::operator>=(const Literal& Other) const noexcept
{
    MatchOperandType(*this, Other);
    return std::visit([]<typename RT, typename LT>(const RT& Value, const LT& OtherValue)
    {
        if constexpr(!std::same_as<RT, LT>)
        {
            return std::make_shared<const struct Literal>();
        }
        else if constexpr(std::is_arithmetic_v<RT> && !std::same_as<RT, LiteralType::Boolean> && !std::same_as<RT, LiteralType::Character>)
        {
            return std::make_shared<const struct Literal>(Value >= OtherValue); 
        }
        else
        {
            return std::make_shared<const struct Literal>(); 
        }
    }, Value, Other.Value);
}