#pragma once

#include <cstdint>

namespace Mamba
{
    enum class LinkIdentifier : std::uint8_t
    {
        Function,
        Variable,
        Type,
        Instruction,
        String
    };
} // namespace Mamba