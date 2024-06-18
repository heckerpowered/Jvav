#include "Literal.h"
#include "MambaCore.h"

namespace Mamba
{
    LiteralValue::LiteralValue() noexcept {}

    LiteralValue::LiteralValue(const StringView String) noexcept : StringValue(String) {}

    LiteralValue::LiteralValue(const std::int32_t Integer) noexcept : SignedIntValue(Integer) {}

    LiteralValue::LiteralValue(const std::int64_t Integer) noexcept : SignedLongValue(Integer) {}

    LiteralValue::LiteralValue(const std::uint64_t Integer) noexcept : UnsignedLongValue(Integer) {}

    Literal::Literal() noexcept {}

    Literal::Literal(const std::shared_ptr<String> String) noexcept :
        StringValue(String), Value(StringView(*String)), Type(LiteralType::String)
    {
    }

    Literal::Literal(const std::int32_t Integer) noexcept : Value(Integer), Type(LiteralType::SignedInt) {}

    Literal::Literal(const std::int64_t Integer) noexcept : Value(Integer), Type(LiteralType::SignedLong) {}

    Literal::Literal(const std::uint64_t Integer) noexcept : Value(Integer), Type(LiteralType::UnsignedLong) {}
} // namespace Mamba
