#pragma once

#include <cstdint>
#include <fast_io.h>
#include <string_view>

namespace Mamba
{
    /**
     * Initializes Mamba, the two arguments are the same as the ones passed to main() in a C++ program.
     * The function initializes singal handlers, parses the command line arguments to options and
     * source files, then compile files, if any. The first command line argument is skipped.
     */
    std::int32_t InitMamba(int ArgumentCount, [[maybe_unused]] char* Arguments[]) noexcept;

    /**
     * Just for fun, or it may do something clean when the program crashed or exited.
     */
    constexpr void MambaOut() noexcept {}

    /**
     * The version number of Mamba in the format Major.Minor.Version. Major is increased by 1 when there's
     * a major rewrite of the project, Minor is increased by 1 when there's significant updates of the project,
     * and Build is only increased when there are only minor changes to the project (such as bug fixes).
     */
    constexpr std::string_view MambaVersion = "1.0.0";
} // namespace Mamba