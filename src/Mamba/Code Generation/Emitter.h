#pragma once

#include "BoundProgram.h"
#include "Diagnostic.h"

#ifndef __has_declspec_attribute
    #define __has_declspec_attribute(x) 0
#endif

namespace Mamba
{
    class
#if defined(MSVC_VER) || __has_declspec_attribute(novtable)
    // Emitter is a pure
    #define __declspec(novtable)
#endif
        Emitter
    {
    public:
        virtual std::vector<Diagnostic> Emit(const BoundProgram& Program, StringView ModuleName) noexcept = 0;
    };
} // namespace Mamba