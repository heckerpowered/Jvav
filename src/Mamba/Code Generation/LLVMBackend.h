#pragma once

#include <string_view>
#include <vector>

#include "Backend.h"

namespace Mamba
{
    struct LLVMBackend : public Backend<LLVMBackend>
    {
        static void GenerateCode(std::span<BoundCompilationUnit*> CompilationUnits, std::string_view ModuleName) noexcept;
    };
} // namespace Mamba