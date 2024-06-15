#pragma once

#include "MambaCore.h"
#include <cstdint>
#include <memory>

MAMBA_NAMESPACE_BEGIN

union LiteralValue
{
    StringView StringValue;
    Char CharacterValue;

    std::uint8_t UnsignedByteValue;
    std::uint16_t UnsignedShortValue;
    std::uint32_t UnsignedIntValue;
    std::uint64_t UnsignedLongValue;

    std::int8_t SignedByteValue;
    std::int16_t SignedShortValue;
    std::int32_t SignedIntValue;
    std::int64_t SignedLongValue;

    double DoubleValue;
    float FloatValue;

    bool BooleanValue;

    LiteralValue() noexcept;

private:
    // In general, for some types of literal such as strings, this type only stores a view of the value, and the value
    // is generally stored in the objects that holds this object. To ensure the validity of the view, this object can
    // only be copied when the object that holds this object is copied.
    //
    // For example, Literal has a StringValue member variable, then the StringValue of this type is its view. This
    // object can be only copied if the Literal object is copied.
    LiteralValue(const LiteralValue&) = default;
    LiteralValue(LiteralValue&&) = default;

    friend class Literal;
};

enum class LiteralType
{
    String,
    Character,

    UnsignedByte,
    UnsignedShort,
    UnsignedInt,
    UnsignedLong,

    SignedByte,
    SignedShort,
    SignedInt,
    SignedLong,

    Double,
    Float,

    Boolean,

    Empty
};

struct Literal
{
    LiteralValue Value;
    LiteralType Type;

    std::optional<std::shared_ptr<const String>> StringValue;

    [[nodiscard]] Literal() noexcept;
};

MAMBA_NAMESPACE_END