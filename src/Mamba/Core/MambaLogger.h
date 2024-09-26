#pragma once

#include "fast_io.h"

namespace Mamba
{
    bool VerboseLoggingEnabled = true;

    auto MambaOut() noexcept
    {
        return fast_io::out();
    }

    template<typename... T>
    void Verbose(T&&... Arguments)
    {
        if (!VerboseLoggingEnabled)
        {
            return;
        }

        fast_io::io::println(MambaOut(), std::forward<T>(Arguments)...);
    }

} // namespace Mamba