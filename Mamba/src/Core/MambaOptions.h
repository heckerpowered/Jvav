#pragma once

#include <fast_io_dsal/span.h>
#include <fast_io_dsal/string_view.h>
#include <fast_io_dsal/vector.h>

namespace Mamba
{
    namespace Options
    {
        extern bool SkipCompile;
        extern fast_io::vector<fast_io::string_view> SourceFiles;
        extern bool EmitLLVM;
    } // namespace Options

    void InitMambaOptions(fast_io::span<fast_io::string_view> Arguments) noexcept;
} // namespace Mamba