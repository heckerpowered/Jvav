#pragma once

#include <cstdint>

namespace Mamba
{
    enum class LinkIdentifier : std::uint8_t
    {
        Version,
        Function,
        Variable,
        Type,
        ModuleName,
        Instruction
    };
} // namespace Mamba