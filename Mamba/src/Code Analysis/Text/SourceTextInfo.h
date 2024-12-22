#pragma once

#include "MambaCore.h"

namespace Mamba
{
    struct SourceTextInfo
    {
        fast_io::native_file_loader FileLoader;
        StringView FileName;

        // The lifetime of the text is tied to the lifetime of the file loader, when the
        // file loader is empty, the lifetime is unknown.
        StringView Text;
    };
} // namespace Mamba