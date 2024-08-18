#pragma once

#include "Colors.h"
#include <cstdint>
#include <fast_io.h>

namespace Mamba
{
    std::int32_t InitMamba(int ArgumentCount, [[maybe_unused]] char* Arguments[]) noexcept;

    // just for fun
    constexpr void MambaOut() noexcept {};
} // namespace Mamba