#pragma once

#include <cstdint>
#include <fast_io.h>
#include <string_view>

namespace Mamba
{
    std::int32_t InitMamba(int ArgumentCount, [[maybe_unused]] char* Arguments[]) noexcept;

    // just for fun
    constexpr void MambaOut() noexcept {}

    constexpr std::string_view MambaVersion = "Mamba 版本 1.0.0";
} // namespace Mamba