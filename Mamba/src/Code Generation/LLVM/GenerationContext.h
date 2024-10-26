#pragma once

#include "MambaCore.h"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

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