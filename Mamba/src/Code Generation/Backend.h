#pragma once

#include "BoundCompilationUnit.h"

namespace Mamba
{
    template<typename T>
    struct Backend
    {
        static void GenerateCode(std::span<BoundCompilationUnit*> CompilationUnits, StringView ModuleName) noexcept
        {
            T::GenerateCode(CompilationUnits, ModuleName);
        }
    };
} // namespace Mamba