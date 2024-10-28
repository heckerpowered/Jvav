#pragma once

#include "TypeSymbol.h"
#ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Weverything"
#endif

#include <LLVM/GenerationContext.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/Transforms/Scalar/DCE.h>
#include <llvm/Transforms/Scalar/LoopPassManager.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>

#ifdef __clang__
    #pragma clang diagnostic pop
#endif

#include "BoundCompilationUnit.h"

namespace Mamba
{
    class LLVMGenerator
    {
        llvm::LLVMContext& Context;
        llvm::Module& Module;
        llvm::IRBuilder<>& Builder;

        std::unordered_map<StringView, llvm::AllocaInst*> NamedValues;

        std::string_view ModuleName;

        [[nodiscard]] LLVMGenerator(std::span<const BoundCompilationUnit*> CompilationUnits, std::string_view ModuleName) noexcept;

        // Map a Jvav type to an LLVM type
        llvm::Type* ToLLVMType(const TypeSymbol* Type) noexcept;
    };
} // namespace Mamba