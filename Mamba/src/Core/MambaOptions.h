#pragma once

#include <span>
#include <string_view>
#include <vector>

namespace Mamba
{
    namespace Options
    {
        extern bool SkipCompile;
        extern std::vector<std::string_view> SourceFiles;
    } // namespace Options

    void InitMambaOptions(std::span<std::string_view> Arguments) noexcept;
} // namespace Mamba