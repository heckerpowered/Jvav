#include "Literal.h"

#include <fast_io.h>

#include "MambaCore.h"

using namespace Mamba;

LiteralValue::LiteralValue() noexcept {}

LiteralValue::LiteralValue(const StringView String) noexcept : StringValue(String) {}

LiteralValue::LiteralValue(const std::int8_t Integer) noexcept : SignedByteValue(Integer) {}

LiteralValue::LiteralValue(const std::int16_t Integer) noexcept : SignedShortValue(Integer) {}

LiteralValue::LiteralValue(const std::int32_t Integer) noexcept : SignedIntValue(Integer) {}

LiteralValue::LiteralValue(const std::int64_t Integer) noexcept : SignedLongValue(Integer) {}

LiteralValue::LiteralValue(const std::uint8_t Integer) noexcept : UnsignedShortValue(Integer) {}

LiteralValue::LiteralValue(const std::uint16_t Integer) noexcept : UnsignedShortValue(Integer) {}

LiteralValue::LiteralValue(const std::uint32_t Integer) noexcept : UnsignedIntValue(Integer) {}

LiteralValue::LiteralValue(const std::uint64_t Integer) noexcept : UnsignedLongValue(Integer) {}

LiteralValue::LiteralValue(const float Value) noexcept : FloatValue(Value) {}

LiteralValue::LiteralValue(const double Value) noexcept : DoubleValue(Value) {}

LiteralValue::LiteralValue(const bool Value) noexcept : BooleanValue(Value) {}

Literal::Literal() noexcept {}

Literal::Literal(const std::shared_ptr<const String> String) noexcept :
    Value(StringView(*String)), Type(LiteralType::String), StringValue(String)
{
}

Literal::Literal(const std::int8_t Integer) noexcept : Value(Integer), Type(LiteralType::SignedByte) {}

Literal::Literal(const std::int32_t Integer) noexcept : Value(Integer), Type(LiteralType::SignedInt) {}

Literal::Literal(const std::int64_t Integer) noexcept : Value(Integer), Type(LiteralType::SignedLong) {}

Literal::Literal(const std::uint8_t Integer) noexcept : Value(Integer), Type(LiteralType::UnsignedByte) {}

Literal::Literal(const std::uint16_t Integer) noexcept : Value(Integer), Type(LiteralType::UnsignedShort) {}

Literal::Literal(const std::uint32_t Integer) noexcept : Value(Integer), Type(LiteralType::UnsignedInt) {}

Literal::Literal(const std::uint64_t Integer) noexcept : Value(Integer), Type(LiteralType::UnsignedLong) {}

Literal::Literal(const Char Character) noexcept : Value(Character), Type(LiteralType::Character) {}

Literal::Literal(const float Value) noexcept : Value(Value), Type(LiteralType::Float) {}

Literal::Literal(const double Value) noexcept : Value(Value), Type(LiteralType::Double) {}

Literal::Literal(const bool Value) noexcept : Value(Value), Type(LiteralType::Boolean) {}

NullableSharedPtr<const Literal> Literal::Negative(const Literal& Literal) noexcept
{
    switch (Literal.Type)
    {
        case LiteralType::SignedByte:
            return std::make_shared<const struct Literal>(-Literal.Value.SignedByteValue);
        case LiteralType::SignedShort:
            return std::make_shared<const struct Literal>(-Literal.Value.SignedShortValue);
        case LiteralType::SignedInt:
            return std::make_shared<const struct Literal>(-Literal.Value.SignedIntValue);
        case LiteralType::SignedLong:
            return std::make_shared<const struct Literal>(-Literal.Value.SignedLongValue);
        case LiteralType::Double:
            return std::make_shared<const struct Literal>(-Literal.Value.DoubleValue);
        case LiteralType::Float:
            return std::make_shared<const struct Literal>(-Literal.Value.FloatValue);

        case LiteralType::String:
        case LiteralType::Character:
        case LiteralType::UnsignedByte:
        case LiteralType::UnsignedShort:
        case LiteralType::UnsignedInt:
        case LiteralType::UnsignedLong:
        case LiteralType::Boolean:
        case LiteralType::Empty:
            return {};
    }
}

NullableSharedPtr<const Literal> Literal::LogicalNegative(const Literal& Literal) noexcept
{
    switch (Literal.Type)
    {
        case LiteralType::Boolean:
            return std::make_shared<const struct Literal>(!Literal.Value.BooleanValue);

        case LiteralType::SignedByte:
        case LiteralType::SignedShort:
        case LiteralType::SignedInt:
        case LiteralType::SignedLong:
        case LiteralType::Double:
        case LiteralType::Float:
        case LiteralType::String:
        case LiteralType::Character:
        case LiteralType::UnsignedByte:
        case LiteralType::UnsignedShort:
        case LiteralType::UnsignedInt:
        case LiteralType::UnsignedLong:
        case LiteralType::Empty:
            return {};
    }
}

NullableSharedPtr<const Literal> Literal::OnesComplement(const Literal& Literal) noexcept
{
    switch (Literal.Type)
    {
        case LiteralType::UnsignedByte:
            return std::make_shared<const struct Literal>(~Literal.Value.UnsignedByteValue);
        case LiteralType::UnsignedShort:
            return std::make_shared<const struct Literal>(~Literal.Value.UnsignedShortValue);
        case LiteralType::UnsignedInt:
            return std::make_shared<const struct Literal>(~Literal.Value.UnsignedIntValue);
        case LiteralType::UnsignedLong:
            return std::make_shared<const struct Literal>(~Literal.Value.UnsignedLongValue);
        case LiteralType::SignedByte:
            return std::make_shared<const struct Literal>(~Literal.Value.SignedByteValue);
        case LiteralType::SignedShort:
            return std::make_shared<const struct Literal>(~Literal.Value.SignedShortValue);
        case LiteralType::SignedInt:
            return std::make_shared<const struct Literal>(~Literal.Value.SignedIntValue);
        case LiteralType::SignedLong:
            return std::make_shared<const struct Literal>(~Literal.Value.SignedLongValue);

        case LiteralType::Double:
        case LiteralType::Float:
        case LiteralType::String:
        case LiteralType::Character:
        case LiteralType::Empty:
        case LiteralType::Boolean:
            return {};
    }
}

static void MatchOperandType(const LiteralType Left, const LiteralType Right) noexcept
{
    if (Left != Right)
    {
#ifdef DEBUG
        fast_io::io::perrln(
            "Operand type not match, ",
            static_cast<std::uint32_t>(Left),
            " != ",
            static_cast<std::uint32_t>(Right)
        );
#endif
        std::unreachable();
    }
}

NullableSharedPtr<const Literal> Literal::operator+(const Literal& Other) const noexcept
{
    MatchOperandType(Type, Other.Type);
    switch (Type)
    {
        case LiteralType::String:
            return std::make_shared<const struct Literal>(Hatcher(
                [&] { return std::make_shared<const String>(Concat(Value.StringValue, Other.Value.StringValue)); }
            ));
        case LiteralType::UnsignedByte:
            return std::make_shared<const struct Literal>(Value.UnsignedByteValue + Other.Value.UnsignedByteValue);
        case LiteralType::UnsignedShort:
            return std::make_shared<const struct Literal>(Value.UnsignedShortValue + Other.Value.UnsignedShortValue);
        case LiteralType::UnsignedInt:
            return std::make_shared<const struct Literal>(Value.UnsignedIntValue + Other.Value.UnsignedIntValue);
        case LiteralType::UnsignedLong:
            return std::make_shared<const struct Literal>(Value.UnsignedLongValue + Other.Value.UnsignedLongValue);
        case LiteralType::SignedByte:
            return std::make_shared<const struct Literal>(Value.SignedByteValue + Other.Value.SignedByteValue);
        case LiteralType::SignedShort:
            return std::make_shared<const struct Literal>(Value.SignedShortValue + Other.Value.SignedShortValue);
        case LiteralType::SignedInt:
            return std::make_shared<const struct Literal>(Value.SignedIntValue + Other.Value.SignedIntValue);
        case LiteralType::SignedLong:
            return std::make_shared<const struct Literal>(Value.SignedLongValue + Other.Value.SignedLongValue);
        case LiteralType::Double:
            return std::make_shared<const struct Literal>(Value.DoubleValue + Other.Value.DoubleValue);
        case LiteralType::Float:
            return std::make_shared<const struct Literal>(Value.FloatValue + Other.Value.FloatValue);
        case LiteralType::Character:
        case LiteralType::Boolean:
        case LiteralType::Empty:
            return {};
    }
}

NullableSharedPtr<const Literal> Literal::operator-(const Literal& Other) const noexcept
{
    MatchOperandType(Type, Other.Type);
    switch (Type)
    {
        case LiteralType::UnsignedByte:
            return std::make_shared<const struct Literal>(Value.UnsignedByteValue - Other.Value.UnsignedByteValue);
        case LiteralType::UnsignedShort:
            return std::make_shared<const struct Literal>(Value.UnsignedShortValue - Other.Value.UnsignedShortValue);
        case LiteralType::UnsignedInt:
            return std::make_shared<const struct Literal>(Value.UnsignedIntValue - Other.Value.UnsignedIntValue);
        case LiteralType::UnsignedLong:
            return std::make_shared<const struct Literal>(Value.UnsignedLongValue - Other.Value.UnsignedLongValue);
        case LiteralType::SignedByte:
            return std::make_shared<const struct Literal>(Value.SignedByteValue - Other.Value.SignedByteValue);
        case LiteralType::SignedShort:
            return std::make_shared<const struct Literal>(Value.SignedShortValue - Other.Value.SignedShortValue);
        case LiteralType::SignedInt:
            return std::make_shared<const struct Literal>(Value.SignedIntValue - Other.Value.SignedIntValue);
        case LiteralType::SignedLong:
            return std::make_shared<const struct Literal>(Value.SignedLongValue - Other.Value.SignedLongValue);
        case LiteralType::Double:
            return std::make_shared<const struct Literal>(Value.DoubleValue - Other.Value.DoubleValue);
        case LiteralType::Float:
            return std::make_shared<const struct Literal>(Value.FloatValue - Other.Value.FloatValue);

        case LiteralType::String:
        case LiteralType::Character:
        case LiteralType::Boolean:
        case LiteralType::Empty:
            return {};
    }
}

NullableSharedPtr<const Literal> Literal::operator*(const Literal& Other) const noexcept
{
    MatchOperandType(Type, Other.Type);
    switch (Type)
    {
        case LiteralType::UnsignedByte:
            return std::make_shared<const struct Literal>(Value.UnsignedByteValue * Other.Value.UnsignedByteValue);
        case LiteralType::UnsignedShort:
            return std::make_shared<const struct Literal>(Value.UnsignedShortValue * Other.Value.UnsignedShortValue);
        case LiteralType::UnsignedInt:
            return std::make_shared<const struct Literal>(Value.UnsignedIntValue * Other.Value.UnsignedIntValue);
        case LiteralType::UnsignedLong:
            return std::make_shared<const struct Literal>(Value.UnsignedLongValue * Other.Value.UnsignedLongValue);
        case LiteralType::SignedByte:
            return std::make_shared<const struct Literal>(Value.SignedByteValue * Other.Value.SignedByteValue);
        case LiteralType::SignedShort:
            return std::make_shared<const struct Literal>(Value.SignedShortValue * Other.Value.SignedShortValue);
        case LiteralType::SignedInt:
            return std::make_shared<const struct Literal>(Value.SignedIntValue * Other.Value.SignedIntValue);
        case LiteralType::SignedLong:
            return std::make_shared<const struct Literal>(Value.SignedLongValue * Other.Value.SignedLongValue);
        case LiteralType::Double:
            return std::make_shared<const struct Literal>(Value.DoubleValue * Other.Value.DoubleValue);
        case LiteralType::Float:
            return std::make_shared<const struct Literal>(Value.FloatValue * Other.Value.FloatValue);

        case LiteralType::String:
        case LiteralType::Character:
        case LiteralType::Boolean:
        case LiteralType::Empty:
            return {};
    }
}

NullableSharedPtr<const Literal> Literal::operator/(const Literal& Other) const noexcept
{
    MatchOperandType(Type, Other.Type);
    switch (Type)
    {
        case LiteralType::UnsignedByte:
            return std::make_shared<const struct Literal>(Value.UnsignedByteValue / Other.Value.UnsignedByteValue);
        case LiteralType::UnsignedShort:
            return std::make_shared<const struct Literal>(Value.UnsignedShortValue / Other.Value.UnsignedShortValue);
        case LiteralType::UnsignedInt:
            return std::make_shared<const struct Literal>(Value.UnsignedIntValue / Other.Value.UnsignedIntValue);
        case LiteralType::UnsignedLong:
            return std::make_shared<const struct Literal>(Value.UnsignedLongValue / Other.Value.UnsignedLongValue);
        case LiteralType::SignedByte:
            return std::make_shared<const struct Literal>(Value.SignedByteValue / Other.Value.SignedByteValue);
        case LiteralType::SignedShort:
            return std::make_shared<const struct Literal>(Value.SignedShortValue / Other.Value.SignedShortValue);
        case LiteralType::SignedInt:
            return std::make_shared<const struct Literal>(Value.SignedIntValue / Other.Value.SignedIntValue);
        case LiteralType::SignedLong:
            return std::make_shared<const struct Literal>(Value.SignedLongValue / Other.Value.SignedLongValue);
        case LiteralType::Double:
            return std::make_shared<const struct Literal>(Value.DoubleValue / Other.Value.DoubleValue);
        case LiteralType::Float:
            return std::make_shared<const struct Literal>(Value.FloatValue / Other.Value.FloatValue);

        case LiteralType::Boolean:
        case LiteralType::Empty:
        case LiteralType::String:
        case LiteralType::Character:
            return {};
    }
}

NullableSharedPtr<const Literal> Literal::operator&(const Literal& Other) const noexcept
{
    MatchOperandType(Type, Other.Type);
    switch (Type)
    {
        case LiteralType::UnsignedByte:
            return std::make_shared<const struct Literal>(Value.UnsignedByteValue & Other.Value.UnsignedByteValue);
        case LiteralType::UnsignedShort:
            return std::make_shared<const struct Literal>(Value.UnsignedShortValue & Other.Value.UnsignedShortValue);
        case LiteralType::UnsignedInt:
            return std::make_shared<const struct Literal>(Value.UnsignedIntValue & Other.Value.UnsignedIntValue);
        case LiteralType::UnsignedLong:
            return std::make_shared<const struct Literal>(Value.UnsignedLongValue & Other.Value.UnsignedLongValue);
        case LiteralType::SignedByte:
            return std::make_shared<const struct Literal>(Value.SignedByteValue & Other.Value.SignedByteValue);
        case LiteralType::SignedShort:
            return std::make_shared<const struct Literal>(Value.SignedShortValue & Other.Value.SignedShortValue);
        case LiteralType::SignedInt:
            return std::make_shared<const struct Literal>(Value.SignedIntValue & Other.Value.SignedIntValue);
        case LiteralType::SignedLong:
            return std::make_shared<const struct Literal>(Value.SignedLongValue & Other.Value.SignedLongValue);

        case LiteralType::String:
        case LiteralType::Character:
        case LiteralType::Double:
        case LiteralType::Float:
        case LiteralType::Boolean:
        case LiteralType::Empty:
            return {};
    }
}

NullableSharedPtr<const Literal> Literal::operator|(const Literal& Other) const noexcept
{
    MatchOperandType(Type, Other.Type);
    switch (Type)
    {
        case LiteralType::UnsignedByte:
            return std::make_shared<const struct Literal>(Value.UnsignedByteValue | Other.Value.UnsignedByteValue);
        case LiteralType::UnsignedShort:
            return std::make_shared<const struct Literal>(Value.UnsignedShortValue | Other.Value.UnsignedShortValue);
        case LiteralType::UnsignedInt:
            return std::make_shared<const struct Literal>(Value.UnsignedIntValue | Other.Value.UnsignedIntValue);
        case LiteralType::UnsignedLong:
            return std::make_shared<const struct Literal>(Value.UnsignedLongValue | Other.Value.UnsignedLongValue);
        case LiteralType::SignedByte:
            return std::make_shared<const struct Literal>(Value.SignedByteValue | Other.Value.SignedByteValue);
        case LiteralType::SignedShort:
            return std::make_shared<const struct Literal>(Value.SignedShortValue | Other.Value.SignedShortValue);
        case LiteralType::SignedInt:
            return std::make_shared<const struct Literal>(Value.SignedIntValue | Other.Value.SignedIntValue);
        case LiteralType::SignedLong:
            return std::make_shared<const struct Literal>(Value.SignedLongValue | Other.Value.SignedLongValue);
        case LiteralType::Boolean:
            return std::make_shared<const struct Literal>(Value.BooleanValue | Other.Value.BooleanValue);

        case LiteralType::String:
        case LiteralType::Character:
        case LiteralType::Double:
        case LiteralType::Float:
        case LiteralType::Empty:
            return {};
    }
}

NullableSharedPtr<const Literal> Literal::operator^(const Literal& Other) const noexcept
{
    MatchOperandType(Type, Other.Type);
    switch (Type)
    {
        case LiteralType::UnsignedByte:
            return std::make_shared<const struct Literal>(Value.UnsignedByteValue ^ Other.Value.UnsignedByteValue);
        case LiteralType::UnsignedShort:
            return std::make_shared<const struct Literal>(Value.UnsignedShortValue ^ Other.Value.UnsignedShortValue);
        case LiteralType::UnsignedInt:
            return std::make_shared<const struct Literal>(Value.UnsignedIntValue ^ Other.Value.UnsignedIntValue);
        case LiteralType::UnsignedLong:
            return std::make_shared<const struct Literal>(Value.UnsignedLongValue ^ Other.Value.UnsignedLongValue);
        case LiteralType::SignedByte:
            return std::make_shared<const struct Literal>(Value.SignedByteValue ^ Other.Value.SignedByteValue);
        case LiteralType::SignedShort:
            return std::make_shared<const struct Literal>(Value.SignedShortValue ^ Other.Value.SignedShortValue);

        case LiteralType::SignedInt:
            return std::make_shared<const struct Literal>(Value.SignedIntValue ^ Other.Value.SignedIntValue);
        case LiteralType::SignedLong:
            return std::make_shared<const struct Literal>(Value.SignedLongValue ^ Other.Value.SignedLongValue);

        case LiteralType::String:
        case LiteralType::Character:
        case LiteralType::Double:
        case LiteralType::Float:
        case LiteralType::Boolean:
        case LiteralType::Empty:
            return {};
    }
}

NullableSharedPtr<const Literal> Literal::operator&&(const Literal& Other) const noexcept
{
    MatchOperandType(Type, Other.Type);
    switch (Type)
    {
        case LiteralType::Boolean:
            return std::make_shared<const struct Literal>(Value.BooleanValue && Other.Value.BooleanValue);

        case LiteralType::String:
        case LiteralType::Character:
        case LiteralType::UnsignedByte:
        case LiteralType::UnsignedShort:
        case LiteralType::UnsignedInt:
        case LiteralType::UnsignedLong:
        case LiteralType::SignedByte:
        case LiteralType::SignedShort:
        case LiteralType::SignedInt:
        case LiteralType::SignedLong:
        case LiteralType::Double:
        case LiteralType::Float:
        case LiteralType::Empty:
            return {};
    }
}

NullableSharedPtr<const Literal> Literal::operator||(const Literal& Other) const noexcept
{
    MatchOperandType(Type, Other.Type);
    switch (Type)
    {
        case LiteralType::Boolean:
            return std::make_shared<const struct Literal>(Value.BooleanValue || Other.Value.BooleanValue);

        case LiteralType::String:
        case LiteralType::Character:
        case LiteralType::UnsignedByte:
        case LiteralType::UnsignedShort:
        case LiteralType::UnsignedInt:
        case LiteralType::UnsignedLong:
        case LiteralType::SignedByte:
        case LiteralType::SignedShort:
        case LiteralType::SignedInt:
        case LiteralType::SignedLong:
        case LiteralType::Double:
        case LiteralType::Float:
        case LiteralType::Empty:
            return {};
    }
}

NullableSharedPtr<const Literal> Literal::operator==(const Literal& Other) const noexcept
{
    MatchOperandType(Type, Other.Type);
    switch (Type)
    {
        case LiteralType::UnsignedByte:
            return std::make_shared<const struct Literal>(Value.UnsignedByteValue == Other.Value.UnsignedByteValue);
        case LiteralType::UnsignedShort:
            return std::make_shared<const struct Literal>(Value.UnsignedShortValue == Other.Value.UnsignedShortValue);
        case LiteralType::UnsignedInt:
            return std::make_shared<const struct Literal>(Value.UnsignedIntValue == Other.Value.UnsignedIntValue);
        case LiteralType::UnsignedLong:
            return std::make_shared<const struct Literal>(Value.UnsignedLongValue == Other.Value.UnsignedLongValue);
        case LiteralType::SignedByte:
            return std::make_shared<const struct Literal>(Value.SignedByteValue == Other.Value.SignedByteValue);
        case LiteralType::SignedShort:
            return std::make_shared<const struct Literal>(Value.SignedShortValue == Other.Value.SignedShortValue);
        case LiteralType::SignedInt:
            return std::make_shared<const struct Literal>(Value.SignedIntValue == Other.Value.SignedIntValue);
        case LiteralType::SignedLong:
            return std::make_shared<const struct Literal>(Value.SignedLongValue == Other.Value.SignedLongValue);
        case LiteralType::Double:
            return std::make_shared<const struct Literal>(Value.DoubleValue == Other.Value.DoubleValue);
        case LiteralType::Float:
            return std::make_shared<const struct Literal>(Value.FloatValue == Other.Value.FloatValue);
        case LiteralType::Boolean:
            return std::make_shared<const struct Literal>(Value.BooleanValue == Other.Value.BooleanValue);
        case LiteralType::String:
            return std::make_shared<const struct Literal>(Value.StringValue == Other.Value.StringValue);
        case LiteralType::Character:
            return std::make_shared<const struct Literal>(Value.CharacterValue == Other.Value.CharacterValue);
        case LiteralType::Empty:
            return {};
    }
}

NullableSharedPtr<const Literal> Literal::operator!=(const Literal& Other) const noexcept
{
    MatchOperandType(Type, Other.Type);
    switch (Type)
    {
        case LiteralType::String:
            return std::make_shared<const struct Literal>(Value.StringValue != Other.Value.StringValue);
        case LiteralType::Character:
            return std::make_shared<const struct Literal>(Value.CharacterValue != Other.Value.CharacterValue);
        case LiteralType::UnsignedByte:
            return std::make_shared<const struct Literal>(Value.UnsignedByteValue != Other.Value.UnsignedByteValue);
        case LiteralType::UnsignedShort:
            return std::make_shared<const struct Literal>(Value.UnsignedShortValue != Other.Value.UnsignedShortValue);
        case LiteralType::UnsignedInt:
            return std::make_shared<const struct Literal>(Value.UnsignedIntValue != Other.Value.UnsignedIntValue);
        case LiteralType::UnsignedLong:
            return std::make_shared<const struct Literal>(Value.UnsignedLongValue != Other.Value.UnsignedLongValue);
        case LiteralType::SignedByte:
            return std::make_shared<const struct Literal>(Value.SignedByteValue != Other.Value.SignedByteValue);
        case LiteralType::SignedShort:
            return std::make_shared<const struct Literal>(Value.SignedShortValue != Other.Value.SignedShortValue);
        case LiteralType::SignedInt:
            return std::make_shared<const struct Literal>(Value.SignedIntValue != Other.Value.SignedIntValue);
        case LiteralType::SignedLong:
            return std::make_shared<const struct Literal>(Value.SignedLongValue != Other.Value.SignedLongValue);
        case LiteralType::Double:
            return std::make_shared<const struct Literal>(Value.DoubleValue != Other.Value.DoubleValue);
        case LiteralType::Float:
            return std::make_shared<const struct Literal>(Value.FloatValue != Other.Value.FloatValue);
        case LiteralType::Boolean:
            return std::make_shared<const struct Literal>(Value.BooleanValue != Other.Value.BooleanValue);
        case LiteralType::Empty:
            return {};
    }
}

NullableSharedPtr<const Literal> Literal::operator<(const Literal& Other) const noexcept
{
    MatchOperandType(Type, Other.Type);
    switch (Type)
    {
        case LiteralType::UnsignedByte:
            return std::make_shared<const struct Literal>(Value.UnsignedByteValue < Other.Value.UnsignedByteValue);
        case LiteralType::UnsignedShort:
            return std::make_shared<const struct Literal>(Value.UnsignedShortValue < Other.Value.UnsignedShortValue);
        case LiteralType::UnsignedInt:
            return std::make_shared<const struct Literal>(Value.UnsignedIntValue < Other.Value.UnsignedIntValue);
        case LiteralType::UnsignedLong:
            return std::make_shared<const struct Literal>(Value.UnsignedLongValue < Other.Value.UnsignedLongValue);
        case LiteralType::SignedByte:
            return std::make_shared<const struct Literal>(Value.SignedByteValue < Other.Value.SignedByteValue);
        case LiteralType::SignedShort:
            return std::make_shared<const struct Literal>(Value.SignedShortValue < Other.Value.SignedShortValue);
        case LiteralType::SignedInt:
            return std::make_shared<const struct Literal>(Value.SignedIntValue < Other.Value.SignedIntValue);
        case LiteralType::SignedLong:
            return std::make_shared<const struct Literal>(Value.SignedLongValue < Other.Value.SignedLongValue);
        case LiteralType::Double:
            return std::make_shared<const struct Literal>(Value.DoubleValue < Other.Value.DoubleValue);
        case LiteralType::Float:
            return std::make_shared<const struct Literal>(Value.FloatValue < Other.Value.FloatValue);

        case LiteralType::String:
        case LiteralType::Character:
        case LiteralType::Boolean:
        case LiteralType::Empty:
            return {};
    }
}

NullableSharedPtr<const Literal> Literal::operator<=(const Literal& Other) const noexcept
{
    MatchOperandType(Type, Other.Type);
    switch (Type)
    {
        case LiteralType::UnsignedByte:
            return std::make_shared<const struct Literal>(Value.UnsignedByteValue <= Other.Value.UnsignedByteValue);
        case LiteralType::UnsignedShort:
            return std::make_shared<const struct Literal>(Value.UnsignedShortValue <= Other.Value.UnsignedShortValue);
        case LiteralType::UnsignedInt:
            return std::make_shared<const struct Literal>(Value.UnsignedIntValue <= Other.Value.UnsignedIntValue);
        case LiteralType::UnsignedLong:
            return std::make_shared<const struct Literal>(Value.UnsignedLongValue <= Other.Value.UnsignedLongValue);
        case LiteralType::SignedByte:
            return std::make_shared<const struct Literal>(Value.SignedByteValue <= Other.Value.SignedByteValue);
        case LiteralType::SignedShort:
            return std::make_shared<const struct Literal>(Value.SignedShortValue <= Other.Value.SignedShortValue);
        case LiteralType::SignedInt:
            return std::make_shared<const struct Literal>(Value.SignedIntValue <= Other.Value.SignedIntValue);
        case LiteralType::SignedLong:
            return std::make_shared<const struct Literal>(Value.SignedLongValue <= Other.Value.SignedLongValue);
        case LiteralType::Double:
            return std::make_shared<const struct Literal>(Value.DoubleValue <= Other.Value.DoubleValue);
        case LiteralType::Float:
            return std::make_shared<const struct Literal>(Value.FloatValue <= Other.Value.FloatValue);

        case LiteralType::String:
        case LiteralType::Character:
        case LiteralType::Boolean:
        case LiteralType::Empty:
            return {};
    }
}

NullableSharedPtr<const Literal> Literal::operator>(const Literal& Other) const noexcept
{
    switch (Type)
    {
        case LiteralType::UnsignedByte:
            return std::make_shared<const struct Literal>(Value.UnsignedByteValue > Other.Value.UnsignedByteValue);
        case LiteralType::UnsignedShort:
            return std::make_shared<const struct Literal>(Value.UnsignedShortValue > Other.Value.UnsignedShortValue);
        case LiteralType::UnsignedInt:
            return std::make_shared<const struct Literal>(Value.UnsignedIntValue > Other.Value.UnsignedIntValue);
        case LiteralType::UnsignedLong:
            return std::make_shared<const struct Literal>(Value.UnsignedLongValue > Other.Value.UnsignedLongValue);
        case LiteralType::SignedByte:
            return std::make_shared<const struct Literal>(Value.SignedByteValue > Other.Value.SignedByteValue);
        case LiteralType::SignedShort:
            return std::make_shared<const struct Literal>(Value.SignedShortValue > Other.Value.SignedShortValue);
        case LiteralType::SignedInt:
            return std::make_shared<const struct Literal>(Value.SignedIntValue > Other.Value.SignedIntValue);
        case LiteralType::SignedLong:
            return std::make_shared<const struct Literal>(Value.SignedLongValue > Other.Value.SignedLongValue);
        case LiteralType::Double:
            return std::make_shared<const struct Literal>(Value.DoubleValue > Other.Value.DoubleValue);
        case LiteralType::Float:
            return std::make_shared<const struct Literal>(Value.FloatValue > Other.Value.FloatValue);
        case LiteralType::String:
        case LiteralType::Character:
        case LiteralType::Boolean:
        case LiteralType::Empty:
            return {};
    }
}

NullableSharedPtr<const Literal> Literal::operator>=(const Literal& Other) const noexcept
{
    MatchOperandType(Type, Other.Type);
    switch (Type)
    {
        case LiteralType::UnsignedByte:
            return std::make_shared<const struct Literal>(Value.UnsignedByteValue >= Other.Value.UnsignedByteValue);
        case LiteralType::UnsignedShort:
            return std::make_shared<const struct Literal>(Value.UnsignedShortValue >= Other.Value.UnsignedShortValue);
        case LiteralType::UnsignedInt:
            return std::make_shared<const struct Literal>(Value.UnsignedIntValue >= Other.Value.UnsignedIntValue);
        case LiteralType::UnsignedLong:
            return std::make_shared<const struct Literal>(Value.UnsignedLongValue >= Other.Value.UnsignedLongValue);
        case LiteralType::SignedByte:
            return std::make_shared<const struct Literal>(Value.SignedByteValue >= Other.Value.SignedByteValue);
        case LiteralType::SignedShort:
            return std::make_shared<const struct Literal>(Value.SignedShortValue >= Other.Value.SignedShortValue);
        case LiteralType::SignedInt:
            return std::make_shared<const struct Literal>(Value.SignedIntValue >= Other.Value.SignedIntValue);
        case LiteralType::SignedLong:
            return std::make_shared<const struct Literal>(Value.SignedLongValue >= Other.Value.SignedLongValue);
        case LiteralType::Double:
            return std::make_shared<const struct Literal>(Value.DoubleValue >= Other.Value.DoubleValue);
        case LiteralType::Float:
            return std::make_shared<const struct Literal>(Value.FloatValue >= Other.Value.FloatValue);

        case LiteralType::String:
        case LiteralType::Character:
        case LiteralType::Boolean:
        case LiteralType::Empty:
            return {};
    }
}