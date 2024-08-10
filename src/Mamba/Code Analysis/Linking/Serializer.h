#pragma once

#include "MambaCore.h"
#include <algorithm>
#include <bit>
#include <span>
#include <type_traits>
#include <vector>

namespace Mamba
{
    class Serializer final
    {
    public:
        std::vector<std::byte> Data;

        void Write(const std::span<const std::byte> DataView) noexcept;

        template<typename T>
            requires std::is_trivially_copyable_v<T> && (!std::is_integral_v<T>)
        void Write(const T& Data) noexcept
        {
            using ByteArray = std::array<std::byte, sizeof(T)>;
            const auto ValueRepresentation = std::bit_cast<ByteArray>(Data);
            Write(std::span<const std::byte>(ValueRepresentation));
        }

        template<std::integral T>
        void Write(const T Data) noexcept
        {
            static_assert(
                std::endian::native == std::endian::little || std::endian::native == std::endian::big,
                "Unsupported endianness"
            );
            static_assert(std::has_unique_object_representations_v<T>, "T may not have padding bits");

            using ByteArray = std::array<std::byte, sizeof(T)>;
            auto ValueRepresentation = std::bit_cast<ByteArray>(Data);
            if constexpr (std::endian::native == std::endian::big)
            {
                // Dotnet linker reads integers as little-endian
                std::ranges::reverse(ValueRepresentation);
            }

            Write(ValueRepresentation);
        }

        void Write(const StringView String) noexcept;
    };
} // namespace Mamba