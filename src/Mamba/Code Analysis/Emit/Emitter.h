#pragma once

#include <memory>

#include "BoundProgram.h"
#include "Diagnostic.h"

namespace Mamba
{
    class
#if defined(MSVC_VER) || (defined(__has_declspec_attribute) && __has_declspec_attribute(novtable))
    // Emitter is a pure
    #define __declspec(novtable)
#endif
        Emitter
    {
        virtual std::vector<Diagnostic> Emit(const BoundProgram& Program, const StringView ModuleName) noexcept = 0;
    };
} // namespace Mamba