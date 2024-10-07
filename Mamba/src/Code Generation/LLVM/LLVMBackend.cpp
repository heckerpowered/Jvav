#include <ranges>
#include <source_location>
#include <string>
#include <string_view>

#include <fast_io.h>

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
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Host.h>

#ifdef __clang__
    #pragma clang diagnostic pop
#endif

#include "LLVMBackend.h"

#include "MambaCore.h"

#include "BoundBinaryExpression.h"
#include "BoundBinaryOperatorKind.h"
#include "BoundBlockStatement.h"
#include "BoundExpression.h"
#include "BoundExpressionStatement.h"
#include "BoundFunctionDeclaration.h"
#include "BoundIfStatement.h"
#include "BoundLiteralExpression.h"
#include "BoundNodeKind.h"
#include "BoundReturnStatement.h"
#include "BoundStatement.h"
#include "BoundVariableDeclaration.h"
#include "BoundWhileStatement.h"
#include "Constant.h"
#include "FunctionSymbol.h"
#include "TypeSymbol.h"

using namespace std::string_literals;
using namespace llvm;
using namespace Mamba;

void InitializeLLVM() noexcept
{
    static bool Initialized = false;
    if (Initialized) [[unlikely]]
    {
        return;
    }

    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmParsers();
    InitializeAllAsmPrinters();
}

Type* GetLLVMType(GenerationContext& Context, const TypeSymbol* Type) noexcept
{
    if (Type == &TypeSymbol::Void)
    {
        return Type::getVoidTy(Context.Context);
    }
    else if (Type == &TypeSymbol::Int)
    {
        return Type::getInt32Ty(Context.Context);
    }
    else if (Type == &TypeSymbol::Bool)
    {
        return Type::getInt1Ty(Context.Context);
    }
    else if (Type == &TypeSymbol::Double)
    {
        return Type::getDoubleTy(Context.Context);
    }

    InternalCompilerError(std::source_location::current(), "无法识别的类型: ", fast_io::mnp::code_cvt(Type->Name()));
}

std::vector<Type*> GetFunctionArgumentTypes(GenerationContext& Context, const FunctionSymbol& FunctionDeclaration) noexcept
{
    return FunctionDeclaration.Parameters |
           std::views::transform([&](const ParameterSymbol* Parameter) -> Type* { return GetLLVMType(Context, Parameter->Type); }) |
           std::ranges::to<std::vector>();
}

void GenerateStatement(GenerationContext& Context, const BoundStatement& Statement) noexcept;

Value* GenerateExpression(GenerationContext& Context, const BoundExpression& Statement) noexcept;

void GenerateBlockStatement(GenerationContext& Context, const BoundBlockStatement& BlockStatement) noexcept
{
    for (auto&& Statement : BlockStatement.Statements)
    {
        GenerateStatement(Context, *Statement);
    }
}

Value* GenerateBinaryExpression(GenerationContext& Context, const BoundBinaryExpression& BinaryExpression) noexcept
{
    auto Left = GenerateExpression(Context, *BinaryExpression.Left);
    auto Right = GenerateExpression(Context, *BinaryExpression.Right);

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

    switch (BinaryExpression.Operator.OperatorKind)
    {
        case BoundBinaryOperatorKind::Addition:
            return Context.Builder.CreateAdd(Left, Right);
        case BoundBinaryOperatorKind::Subtraction:
            return Context.Builder.CreateSub(Left, Right);
        case BoundBinaryOperatorKind::Multiplication:
            return Context.Builder.CreateMul(Left, Right);
        case BoundBinaryOperatorKind::Division:
            return Context.Builder.CreateSDiv(Left, Right);
        case BoundBinaryOperatorKind::LogicalAnd:
            return Context.Builder.CreateLogicalAnd(Left, Right);
        case BoundBinaryOperatorKind::LogicalOr:
            return Context.Builder.CreateLogicalOr(Left, Right);
        case BoundBinaryOperatorKind::BitwiseAnd:
            return Context.Builder.CreateAnd(Left, Right);
        case BoundBinaryOperatorKind::BitwiseOr:
            return Context.Builder.CreateOr(Left, Right);
        case BoundBinaryOperatorKind::BitwiseXor:
            return Context.Builder.CreateXor(Left, Right);
        case BoundBinaryOperatorKind::Equals:
            return Context.Builder.CreateICmpEQ(Left, Right);
        case BoundBinaryOperatorKind::NotEquals:
            return Context.Builder.CreateICmpNE(Left, Right);
        case BoundBinaryOperatorKind::Less:
            return Context.Builder.CreateICmpSLT(Left, Right);
        case BoundBinaryOperatorKind::LessOrEquals:
            return Context.Builder.CreateICmpSLE(Left, Right);
        case BoundBinaryOperatorKind::Greater:
            return Context.Builder.CreateICmpSGT(Left, Right);
        case BoundBinaryOperatorKind::GreaterOrEquals:
            return Context.Builder.CreateICmpSGE(Left, Right);
    }

    InternalCompilerError(std::source_location::current(), "无法识别的二元运算符: ", fast_io::mnp::enum_int_view(BinaryExpression.Operator.OperatorKind));
}

Value* GenerateLiteralExpression(GenerationContext& Context, const BoundLiteralExpression& Expression) noexcept
{
    return std::visit(
        [&]<typename T>(T&& Value) -> ConstantData* {
            if constexpr (std::is_same_v<std::decay_t<T>, ConstantType::Int>)
            {
                return ConstantInt::get(Type::getInt32Ty(Context.Context), Value);
            }
            else if constexpr (std::is_same_v<std::decay_t<T>, ConstantType::ULong>)
            {
                return ConstantInt::get(Type::getInt64Ty(Context.Context), Value);
            }
            else if constexpr (std::is_same_v<std::decay_t<T>, ConstantType::Boolean>)
            {
                return ConstantInt::get(Type::getInt1Ty(Context.Context), Value);
            }
            else if constexpr (std::is_same_v<std::decay_t<T>, ConstantType::Double>)
            {
                return ConstantFP::get(Context.Context, APFloat(Value));
            }

            return nullptr;
        },
        Expression.ConstantValue().GetValue()
    );
}

Value* GenerateExpression(GenerationContext& Context, const BoundExpression& Statement) noexcept
{
    switch (Statement.Kind())
    {
        case BoundNodeKind::BinaryExpression:
            return GenerateBinaryExpression(Context, dynamic_cast<const BoundBinaryExpression&>(Statement));
        case BoundNodeKind::LiteralExpression:
            return GenerateLiteralExpression(Context, dynamic_cast<const BoundLiteralExpression&>(Statement));
        default:
            break;
    }
    return {};
}

void GenerateReturnStatement(GenerationContext& Context, const BoundReturnStatement& Statement) noexcept
{
    auto Value = GenerateExpression(Context, *Statement.Expression);
    Context.Builder.CreateRet(Value);
}

void GenerateVariableDeclaration(GenerationContext& Context, const BoundVariableDeclaration& Statement) noexcept
{
    auto CurrentFunction = Context.Builder.GetInsertBlock()->getParent();
    auto BlockBuilder = IRBuilder<>(&CurrentFunction->getEntryBlock(), CurrentFunction->getEntryBlock().begin());
    auto Alloca = BlockBuilder.CreateAlloca(GetLLVMType(Context, Statement.Variable->Type), nullptr, fast_io::concat(fast_io::mnp::code_cvt(Statement.Variable->Name())));
    Context.NamedValues[Statement.Variable->Name()] = Alloca;

    auto InitValue = GenerateExpression(Context, *Statement.Initializer);
    if (!InitValue)
    {
        return;
    }

    BlockBuilder.CreateStore(InitValue, Alloca);
}

void GenerateIfStatement(GenerationContext& Context, const BoundIfStatement& Statement) noexcept
{
    auto Condition = GenerateExpression(Context, *Statement.Condition);
    if (!Condition)
    {
        return;
    }

    Condition = Context.Builder.CreateICmpNE(Condition, ConstantInt::getFalse(Context.Context));

    auto CurrentFunction = Context.Builder.GetInsertBlock()->getParent();
    auto ThenBlock = BasicBlock::Create(Context.Context, "then", CurrentFunction);
    auto ElseBlock = BasicBlock::Create(Context.Context, "else", CurrentFunction);
    auto MergeBlock = BasicBlock::Create(Context.Context, "merge", CurrentFunction);

    Context.Builder.CreateCondBr(Condition, ThenBlock, ElseBlock);

    Context.Builder.SetInsertPoint(ThenBlock);
    GenerateStatement(Context, *Statement.ThenStatement);

    Context.Builder.CreateBr(MergeBlock);
    ThenBlock = Context.Builder.GetInsertBlock();

    CurrentFunction->insert(CurrentFunction->end(), ElseBlock);
    Context.Builder.SetInsertPoint(ElseBlock);
    if (!Statement.ElseStatement)
    {
        return;
    }

    GenerateStatement(Context, *Statement.ElseStatement);

    Context.Builder.CreateBr(MergeBlock);
    ElseBlock = Context.Builder.GetInsertBlock();

    CurrentFunction->insert(CurrentFunction->end(), MergeBlock);
    Context.Builder.SetInsertPoint(MergeBlock);
}

void GenerateWhileStatement(GenerationContext& Context [[maybe_unused]], const BoundWhileStatement& Statement [[maybe_unused]]) noexcept
{
    // TODO
}

void GenerateStatement(GenerationContext& Context, const BoundStatement& Statement) noexcept
{
    switch (Statement.Kind())
    {
        case BoundNodeKind::BlockStatement:
            GenerateBlockStatement(Context, dynamic_cast<const BoundBlockStatement&>(Statement));
            break;
        case BoundNodeKind::ExpressionStatement:
            GenerateExpression(Context, *dynamic_cast<const BoundExpressionStatement&>(Statement).Expression);
            break;
        case BoundNodeKind::ReturnStatement:
            GenerateReturnStatement(Context, dynamic_cast<const BoundReturnStatement&>(Statement));
            break;
        case BoundNodeKind::NopStatement:
            break;
        case BoundNodeKind::VariableDeclaration:
            GenerateVariableDeclaration(Context, dynamic_cast<const BoundVariableDeclaration&>(Statement));
            break;
        case BoundNodeKind::IfStatement:
            GenerateIfStatement(Context, dynamic_cast<const BoundIfStatement&>(Statement));
            break;
        case BoundNodeKind::WhileStatement:

        case BoundNodeKind::DoWhileStatement:
        case BoundNodeKind::ForStatement:
        case BoundNodeKind::LabelStatement:
        case BoundNodeKind::GotoStatement:
        case BoundNodeKind::ConditionalGotoStatement:
        case BoundNodeKind::FunctionDeclaration:
        case BoundNodeKind::ErrorExpression:
        case BoundNodeKind::LiteralExpression:
        case BoundNodeKind::VariableExpression:
        case BoundNodeKind::AssignmentExpression:
        case BoundNodeKind::CompoundAssignmentExpression:
        case BoundNodeKind::UnaryExpression:
        case BoundNodeKind::BinaryExpression:
        case BoundNodeKind::CallExpression:
        case BoundNodeKind::ConversionExpression:
        case BoundNodeKind::CompilationUnit:
            break;
    }
}

void GenerateFunctionBody(GenerationContext& Context, const BoundFunctionDeclaration& FunctionDeclaration) noexcept
{
    GenerateStatement(Context, *FunctionDeclaration.Body);
}

void GenerateFunction(GenerationContext& Context, const FunctionSymbol& FunctionDeclaration) noexcept
{
    auto ReturnType = GetLLVMType(Context, FunctionDeclaration.Type);
    auto ParameterTypes = GetFunctionArgumentTypes(Context, FunctionDeclaration);

    auto FunctionType = FunctionType::get(ReturnType, ParameterTypes, false);
    auto Function = Function::Create(FunctionType, GlobalValue::ExternalLinkage, fast_io::concat(fast_io::mnp::code_cvt(FunctionDeclaration.Name())), &Context.Module);

    auto Index = std::size_t();
    for (auto&& Argument : Function->args())
    {
        Argument.setName(fast_io::concat(fast_io::mnp::code_cvt(FunctionDeclaration.Parameters[Index++]->Name())));
    }

    auto Block = BasicBlock::Create(Context.Context, "entry", Function);
    Context.Builder.SetInsertPoint(Block);
    GenerateFunctionBody(Context, *FunctionDeclaration.BoundDeclaration);

    verifyFunction(*Function);
}

void GenerateCompilationUnit(GenerationContext& Context, BoundCompilationUnit& CompilationUnit) noexcept
{
    for (auto&& Function : CompilationUnit.GlobalScope->DeclaredFunctions())
    {
        GenerateFunction(Context, *Function);
    }
}

void LLVMBackend::GenerateCode(std::span<BoundCompilationUnit*> CompilationUnits, std::string_view ModuleName) noexcept
{
    InitializeLLVM();

    auto LLVMContext = ::llvm::LLVMContext();
    auto LLVMModule = Module(ModuleName, LLVMContext);
    auto Builder = IRBuilder<>(LLVMContext);

    auto Context = GenerationContext{ LLVMContext, LLVMModule, Builder, {} };

    for (auto&& CompilationUnit : CompilationUnits)
    {
        GenerateCompilationUnit(Context, *CompilationUnit);
    }

    auto TargetTriple = Triple::normalize(sys::getDefaultTargetTriple());
    auto ErrorString = std::string();
    auto Target = TargetRegistry::lookupTarget(TargetTriple, ErrorString);
    if (!Target)
    {
        InternalCompilerError(std::source_location::current(), "无法找到请求的目标: ", ErrorString);
    }

    constexpr auto CPU = "";
    constexpr auto Features = "";

    auto Options = TargetOptions();
    auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, Options, std::nullopt);

    fast_io::io::println("目标: ", TargetTriple, "\n", "线程模型: ", Options.ThreadModel == ThreadModel::POSIX ? fast_io::mnp::os_c_str("POSIX") : fast_io::mnp::os_c_str("单线程环境"));

    LLVMModule.setDataLayout(TargetMachine->createDataLayout());
    LLVMModule.setTargetTriple(TargetTriple);

    LLVMModule.print(errs(), nullptr);

    auto FileName = fast_io::concat(ModuleName, ".o");

    auto error_code = std::error_code();
    raw_fd_ostream Out(FileName, error_code, sys::fs::OF_None);
    if (error_code)
    {
        Error("无法打开输出文件: ", error_code.message());
        return;
    }

    auto PassManager = legacy::PassManager();
    constexpr auto FileType = CodeGenFileType::ObjectFile;

    if (TargetMachine->addPassesToEmitFile(PassManager, Out, {}, FileType, false))
    {
        InternalCompilerError(std::source_location::current(), "无法为目标生成代码");
    }

    PassManager.run(LLVMModule);
    Out.flush();

    fast_io::io::println("编译成功: ", FileName);
}