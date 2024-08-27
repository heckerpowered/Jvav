#include "LLVMBackend.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

using namespace Mamba;
using namespace llvm;

void LLVMBackend::GenerateCode(std::span<BoundCompilationUnit*> CompilationUnits, std::string_view ModuleName) noexcept
{
    auto Context = LLVMContext();
    auto LLVMModule = Module(ModuleName, Context);

    auto Builder = IRBuilder<>(Context);

    LLVMModule.print(errs(), {});
}