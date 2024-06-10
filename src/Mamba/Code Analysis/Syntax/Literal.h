#pragma once

#include "MambaCore.h"
#include <cstdint>
#include <memory>

MAMBA_NAMESPACE_BEGIN

union LiteralValue
{
    std::shared_ptr<const String> StringValue;
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

    Boolean
};

struct Literal
{
    LiteralValue Value;
    LiteralType Type;
};

MAMBA_NAMESPACE_END