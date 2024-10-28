#pragma once

#include <string_view>

#ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Weverything"
#endif

#include <llvm/IR/Type.h>
#include <llvm/Target/TargetOptions.h>

#ifdef __clang__
    #pragma clang diagnostic pop
#endif

#include "Backend.h"
#include "GenerationContext.h"

namespace Mamba
{
    struct LLVMBackend : public Backend<LLVMBackend>
    {
        static void GenerateCode(std::span<BoundCompilationUnit*> CompilationUnits, std::string_view ModuleName) noexcept;

        static std::string TargetTriple() noexcept;
        static llvm::ThreadModel::Model ThreadModel() noexcept;

        // Initialize all components of LLVM, call multiple times has no effect
        static void InitializeLLVM() noexcept;

        static llvm::Type* GetLLVMType(GenerationContext& Context, const TypeSymbol* Type) noexcept;
    };
} // namespace Mamba