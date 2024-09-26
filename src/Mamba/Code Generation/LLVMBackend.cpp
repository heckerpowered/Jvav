#include "LLVMBackend.h"

#include "BlockStatementSyntax.h"
#include "BoundBinaryExpression.h"
#include "BoundBinaryOperatorKind.h"
#include "BoundBlockStatement.h"
#include "BoundExpression.h"
#include "BoundFunctionDeclaration.h"
#include "BoundLiteralExpression.h"
#include "BoundNodeKind.h"
#include "BoundReturnStatement.h"
#include "BoundStatement.h"
#include "Constant.h"
#include "FunctionSymbol.h"
#include "MambaCore.h"
#include "MambaLogger.h"
#include "TypeSymbol.h"

#include <ranges>
#include <source_location>
#include <string>

#include "BoundBlockStatement.h"
#include "fast_io.h"
#include "fast_io_core_impl/codecvt/general.h"

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Host.h"

using namespace Mamba;
using namespace llvm;

void InitializeLLVM() noexcept
{
    static bool Initialized = false;
    if (Initialized) [[unlikely]]
    {
        return;
    }

    Verbose("正在初始化LLVM");
    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmParsers();
    InitializeAllAsmPrinters();
}

Type* GetLLVMType(LLVMContext& Context, const TypeSymbol* Type) noexcept
{
    if (Type == &TypeSymbol::Void)
    {
        return Type::getVoidTy(Context);
    }
    else if (Type == &TypeSymbol::Int)
    {
        return Type::getInt32Ty(Context);
    }
    else if (Type == &TypeSymbol::Bool)
    {
        return Type::getInt1Ty(Context);
    }
    else if (Type == &TypeSymbol::Double)
    {
        return Type::getDoubleTy(Context);
    }

    InternalCompilerError(std::source_location::current(), "无法识别的类型: ", fast_io::mnp::code_cvt(Type->Name()));
}

std::vector<Type*> GetFunctionArgumentTypes(LLVMContext& Context, const FunctionSymbol& FunctionDeclaration) noexcept
{
    return FunctionDeclaration.Parameters |
           std::views::transform([&](const ParameterSymbol* Parameter) -> Type* { return GetLLVMType(Context, Parameter->Type); }) |
           std::ranges::to<std::vector>();
}

void GenerateStatement(LLVMContext& Context, Module& Module, IRBuilder<>& Builder, const BoundStatement& Statement) noexcept;

Value* GenerateExpression(LLVMContext& Context, Module& Module, IRBuilder<>& Builder, const BoundExpression& Statement) noexcept;

void GenerateBlockStatement(LLVMContext& Context, Module& Module, IRBuilder<>& Builder, const BoundBlockStatement& BlockStatement) noexcept
{
    for (auto&& Statement : BlockStatement.Statements)
    {
        GenerateStatement(Context, Module, Builder, *Statement);
    }
}

Value* GenerateBinaryExpression(LLVMContext& Context, Module& Module, IRBuilder<>& Builder, const BoundBinaryExpression& BinaryExpression) noexcept
{
    auto Left = GenerateExpression(Context, Module, Builder, *BinaryExpression.Left);
    auto Right = GenerateExpression(Context, Module, Builder, *BinaryExpression.Right);

    if (!Left && !Right)
    {
        InternalCompilerError(std::source_location::current(), "二元表达式左右表达式均为空");
    }
    else if (!Left)
    {
        InternalCompilerError(std::source_location::current(), "二元表达式左表达式为空");
    }
    else if (!Right)
    {
        InternalCompilerError(std::source_location::current(), "二元表达式右表达式为空");
    }

    if (BinaryExpression.Operator.OperatorKind == BoundBinaryOperatorKind::Addition)
    {
        return Builder.CreateAdd(Left, Right);
    }

    return {};
}

Value* GenerateLiteralExpression(LLVMContext& Context, Module& Module [[maybe_unused]], IRBuilder<>& Builder [[maybe_unused]], const BoundLiteralExpression& Expression) noexcept
{
    return Expression.ConstantValue().GetValue().visit([&]<typename T>(T&& Value) -> ConstantData* {
        if constexpr (std::is_same_v<std::decay_t<T>, ConstantType::Int>)
        {
            return ConstantInt::get(Type::getInt32Ty(Context), Value);
        }
        else if constexpr (std::is_same_v<std::decay_t<T>, ConstantType::ULong>)
        {
            return ConstantInt::get(Type::getInt64Ty(Context), Value);
        }

        return nullptr;
    });
}

Value* GenerateExpression(LLVMContext& Context, Module& Module, IRBuilder<>& Builder, const BoundExpression& Statement) noexcept
{
    switch (Statement.Kind())
    {
        case BoundNodeKind::BinaryExpression:
            return GenerateBinaryExpression(Context, Module, Builder, dynamic_cast<const BoundBinaryExpression&>(Statement));
        case BoundNodeKind::LiteralExpression:
            return GenerateLiteralExpression(Context, Module, Builder, dynamic_cast<const BoundLiteralExpression&>(Statement));
        default:
            break;
    }
    return {};
}

void GenerateReturnStatement(LLVMContext& Context, Module& Module, IRBuilder<>& Builder, const BoundReturnStatement& Statement) noexcept
{
    auto Value = GenerateExpression(Context, Module, Builder, *Statement.Expression);
    Builder.CreateRet(Value);
}

void GenerateStatement(LLVMContext& Context, Module& Module, IRBuilder<>& Builder, const BoundStatement& Statement) noexcept
{
    switch (Statement.Kind())
    {
        case BoundNodeKind::BlockStatement:
            GenerateBlockStatement(Context, Module, Builder, dynamic_cast<const BoundBlockStatement&>(Statement));
            break;
        case BoundNodeKind::ExpressionStatement:
            GenerateExpression(Context, Module, Builder, dynamic_cast<const BoundExpression&>(Statement));
            break;
        case BoundNodeKind::ReturnStatement:
            GenerateReturnStatement(Context, Module, Builder, dynamic_cast<const BoundReturnStatement&>(Statement));
            break;
        default:
            break;
    }
}

void GenerateFunctionBody(LLVMContext& Context, Module& Module, IRBuilder<>& Builder, const BoundFunctionDeclaration& FunctionDeclaration) noexcept
{
    GenerateStatement(Context, Module, Builder, *FunctionDeclaration.Body);
}

void GenerateFunction(LLVMContext& Context, Module& Module, IRBuilder<>& Builder, const FunctionSymbol& FunctionDeclaration) noexcept
{
    auto ReturnType = GetLLVMType(Context, FunctionDeclaration.Type);
    auto ParameterTypes = GetFunctionArgumentTypes(Context, FunctionDeclaration);

    auto FunctionType = FunctionType::get(ReturnType, ParameterTypes, false);
    auto Function = Function::Create(FunctionType, GlobalValue::ExternalLinkage, fast_io::concat(fast_io::mnp::code_cvt(FunctionDeclaration.Name())), &Module);

    auto Index = std::size_t();
    for (auto&& Argument : Function->args())
    {
        Argument.setName(fast_io::concat(fast_io::mnp::code_cvt(FunctionDeclaration.Parameters[Index++]->Name())));
    }

    auto Block = BasicBlock::Create(Context, "entry", Function);
    Builder.SetInsertPoint(Block);
    GenerateFunctionBody(Context, Module, Builder, *FunctionDeclaration.BoundDeclaration);
}

void GenerateCompilationUnit(LLVMContext& Context, Module& Module, IRBuilder<>& Builder, BoundCompilationUnit& CompilationUnit) noexcept
{
    for (auto&& Function : CompilationUnit.GlobalScope->DeclaredFunctions())
    {
        GenerateFunction(Context, Module, Builder, *Function);
    }
}

void LLVMBackend::GenerateCode(std::span<BoundCompilationUnit*> CompilationUnits, std::string_view ModuleName) noexcept
{
    InitializeLLVM();

    auto Context = LLVMContext();
    auto LLVMModule = Module(ModuleName, Context);
    auto Builder = IRBuilder<>(Context);

    for (auto&& CompilationUnit : CompilationUnits)
    {
        GenerateCompilationUnit(Context, LLVMModule, Builder, *CompilationUnit);
    }

    auto TargetTriple = sys::getDefaultTargetTriple();
    auto ErrorString = std::string();
    auto Target = TargetRegistry::lookupTarget(TargetTriple, ErrorString);
    if (!Target)
    {
        InternalCompilerError(std::source_location::current(), "无法找到请求的目标三元组: ", ErrorString);
    }

    fast_io::io::println("编译到目标: ", TargetTriple);

    constexpr auto CPU = "generic";
    constexpr auto Features = "";

    auto Options = TargetOptions();
    auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, Options, Reloc::PIC_);

    LLVMModule.setDataLayout(TargetMachine->createDataLayout());
    LLVMModule.setTargetTriple(TargetTriple);

    // LLVMModule.print(errs(), nullptr);

    auto FileName = fast_io::concat(ModuleName, ".o");

    auto error_code = std::error_code();
    raw_fd_ostream dest(FileName, error_code, sys::fs::OF_None);
    if (error_code)
    {
        Error("无法打开输出文件: ", error_code.message());
        return;
    }

    auto PassManager = legacy::PassManager();
    constexpr auto FileType = CodeGenFileType::ObjectFile;

    if (TargetMachine->addPassesToEmitFile(PassManager, dest, {}, FileType))
    {
        InternalCompilerError(std::source_location::current(), "无法为目标生成代码");
    }

    PassManager.run(LLVMModule);
    dest.flush();
    fast_io::io::println("编译成功: ", FileName);
}