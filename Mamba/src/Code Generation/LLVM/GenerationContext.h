#pragma once

#include "MambaCore.h"

#ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Weverything"
#endif

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#ifdef __clang__
    #pragma clang diagnostic pop
#endif

namespace Mamba
{
    struct GenerationContext
    {
        llvm::LLVMContext& Context;
        llvm::Module& Module;
        llvm::IRBuilder<>& Builder;

        std::unordered_map<StringView, llvm::AllocaInst*> NamedValues;

        llvm::AllocaInst* ReturnValue;
        llvm::BasicBlock* ReturnBlock;

        // Indicates that an unconditional jump instruction has been generated,
        // and subsequent code is considered DEAD code and should be excised
        bool IsTerminating : 1;
    };
} // namespace Mamba