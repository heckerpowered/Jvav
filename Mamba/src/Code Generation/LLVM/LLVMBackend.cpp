#include "BoundAssignmentExpression.h"
#include "BoundCompoundAssignmentExpression.h"
#include "BoundVariableExpression.h"
#include "CompareKind.h"
#include <optional>
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
#include <llvm/Transforms/Scalar/LoopPassManager.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>

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
#include "MambaOptions.h"
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

    switch (BinaryExpression.Operator.OperatorKind)
    {
        case BoundBinaryOperatorKind::Addition:
            return Context.Builder.CreateAdd(Left, Right, "builtin-addition");
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

Value* GenerateVariableExpression(GenerationContext& Context, const BoundVariableExpression& Expression) noexcept
{
    auto Iterator = Context.NamedValues.find(Expression.Variable->Name());
    if (Iterator == Context.NamedValues.end())
    {
        InternalCompilerError(std::source_location::current(), "无法生成变量表达式, 未找到变量: ", fast_io::mnp::code_cvt(Expression.Variable->Name()));
    }

    auto Alloca = Iterator->second;
    return Context.Builder.CreateLoad(Alloca->getAllocatedType(), Alloca, fast_io::concat(fast_io::mnp::code_cvt(Expression.Variable->Name())));
}

Value* GenerateAsssignmentExpression(GenerationContext& Context, const BoundAssignmentExpression& Expression) noexcept
{
    auto Iterator = Context.NamedValues.find(Expression.Variable->Name());
    if (Iterator == Context.NamedValues.end())
    {
        InternalCompilerError(std::source_location::current(), "无法生成赋值表达式, 未找到变量: ", fast_io::mnp::code_cvt(Expression.Variable->Name()));
    }

    auto Variable = Iterator->second;
    auto Value = GenerateExpression(Context, *Expression.Expression);
    return Context.Builder.CreateStore(Value, Variable);
}

Value* GenerateBinaryExpressionInternal(GenerationContext& Context, Value* Left, Value* Right, BoundBinaryOperatorKind Operator, CompareKind Kind = CompareKind::Signed) noexcept
{
    auto IsFloatingPoint = (Kind == CompareKind::Ordered || Kind == CompareKind::Unordered);
    switch (Operator)
    {
        case BoundBinaryOperatorKind::Addition:
            if (IsFloatingPoint)
            {
                return Context.Builder.CreateFAdd(Left, Right, "builtin-floating-addition");
            }
            return Context.Builder.CreateAdd(Left, Right);
        case BoundBinaryOperatorKind::Subtraction:
            if (IsFloatingPoint)
            {
                return Context.Builder.CreateFSub(Left, Right, "builtin-floating-subtraction");
            }
            return Context.Builder.CreateSub(Left, Right);
        case BoundBinaryOperatorKind::Multiplication:
            if (IsFloatingPoint)
            {
                return Context.Builder.CreateFMul(Left, Right, "builtin-floating-multiplication");
            }
            return Context.Builder.CreateMul(Left, Right);
        case BoundBinaryOperatorKind::Division:
            if (IsFloatingPoint)
            {
                return Context.Builder.CreateFDiv(Left, Right, "builtin-floating-division");
            }
            return Context.Builder.CreateSDiv(Left, Right);
        case BoundBinaryOperatorKind::LogicalAnd:
            return Context.Builder.CreateLogicalAnd(Left, Right, "builtin-logical-and");
        case BoundBinaryOperatorKind::LogicalOr:
            return Context.Builder.CreateLogicalOr(Left, Right, "builtin-logical-or");
        case BoundBinaryOperatorKind::BitwiseAnd:
            return Context.Builder.CreateAnd(Left, Right, "builtin-bitwise-and");
        case BoundBinaryOperatorKind::BitwiseOr:
            return Context.Builder.CreateOr(Left, Right, "builtin-bitwise-or");
        case BoundBinaryOperatorKind::BitwiseXor:
            return Context.Builder.CreateXor(Left, Right, "builtin-bitwise-xor");
        case BoundBinaryOperatorKind::Equals:
            if (Kind == CompareKind::Ordered)
            {
                return Context.Builder.CreateFCmpOEQ(Left, Right, "builtin-floating-ordered-equals");
            }
            else if (Kind == CompareKind::Unordered)
            {
                return Context.Builder.CreateFCmpUEQ(Left, Right, "builtin-floating-unordered-equals");
            }

            return Context.Builder.CreateICmpEQ(Left, Right, "builtin-integer-equals");
        case BoundBinaryOperatorKind::NotEquals:
            if (Kind == CompareKind::Ordered)
            {
                return Context.Builder.CreateFCmpONE(Left, Right, "builtin-floating-not-equals");
            }
            else if (Kind == CompareKind::Unordered)
            {
                return Context.Builder.CreateFCmpUNE(Left, Right, "builtin-floating-unordered-not-equals");
            }

            return Context.Builder.CreateICmpNE(Left, Right, "builtin-integer-not-equals");
        case BoundBinaryOperatorKind::Less:
            if (Kind == CompareKind::Ordered)
            {
                return Context.Builder.CreateFCmpOLT(Left, Right, "builtin-ordered-floating-less");
            }
            else if (Kind == CompareKind::Unordered)
            {
                return Context.Builder.CreateFCmpULT(Left, Right, "builtin-unordered-floating-less");
            }
            else if (Kind == CompareKind::Signed)
            {
                return Context.Builder.CreateICmpSLT(Left, Right, "builtin-signed-integer-less");
            }
            else if (Kind == CompareKind::Unsigned)
            {
                return Context.Builder.CreateICmpULT(Left, Right, "builtin-unsigned-integer-less");
            }

            InternalCompilerError(std::source_location::current(), "无法识别的比较类型，编号: ", fast_io::mnp::enum_int_view(Kind));
        case BoundBinaryOperatorKind::LessOrEquals:
            if (Kind == CompareKind::Ordered)
            {
                return Context.Builder.CreateFCmpOLE(Left, Right, "builtin-ordered-floating-less-or-equals");
            }
            else if (Kind == CompareKind::Unordered)
            {
                return Context.Builder.CreateFCmpULE(Left, Right, "builtin-unordered-floating-less-or-equals");
            }
            else if (Kind == CompareKind::Signed)
            {
                return Context.Builder.CreateICmpSLE(Left, Right, "builtin-signed-integer-less-or-equals");
            }
            else if (Kind == CompareKind::Unsigned)
            {
                return Context.Builder.CreateICmpULE(Left, Right, "builtin-unsigned-integer-less-or-equals");
            }
            InternalCompilerError(std::source_location::current(), "无法识别的比较类型，编号: ", fast_io::mnp::enum_int_view(Kind));

        case BoundBinaryOperatorKind::Greater:
            if (Kind == CompareKind::Ordered)
            {
                return Context.Builder.CreateFCmpOGT(Left, Right, "builtin-ordered-floating-greater");
            }
            else if (Kind == CompareKind::Unordered)
            {
                return Context.Builder.CreateFCmpOGT(Left, Right, "builtin-unordered-floating-greater");
            }
            else if (Kind == CompareKind::Signed)
            {
                return Context.Builder.CreateICmpSGT(Left, Right, "builtin-signed-integer-greater");
            }
            else if (Kind == CompareKind::Unsigned)
            {
                return Context.Builder.CreateICmpSGT(Left, Right, "builtin-signed-integer-greater");
            }

            InternalCompilerError(std::source_location::current(), "无法识别的比较类型，编号: ", fast_io::mnp::enum_int_view(Kind));

        case BoundBinaryOperatorKind::GreaterOrEquals:
            if (Kind == CompareKind::Ordered)
            {
                return Context.Builder.CreateFCmpOGE(Left, Right, "builtin-ordered-floating-greater-or-equals");
            }
            else if (Kind == CompareKind::Unordered)
            {
                return Context.Builder.CreateFCmpUGE(Left, Right, "builtin-unordered-floating-greater-or-equals");
            }
            else if (Kind == CompareKind::Signed)
            {
                return Context.Builder.CreateICmpSGE(Left, Right, "builtin-signed-integer-greater-or-equals");
            }
            else if (Kind == CompareKind::Unsigned)
            {
                return Context.Builder.CreateICmpUGE(Left, Right, "builtin-unsigned-integer-greater-or-equals");
            }

        default:
            InternalCompilerError(std::source_location::current(), "无法识别的二元运算符: ", fast_io::mnp::enum_int_view(Operator));
    }
}

Value* GenerateCompoundAssignmentExpression(GenerationContext& Context, const BoundCompoundAssignmentExpression& Expression) noexcept
{
    auto Iterator = Context.NamedValues.find(Expression.Variable->Name());
    if (Iterator == Context.NamedValues.end())
    {
        InternalCompilerError(std::source_location::current(), "无法生成复合赋值表达式, 未找到变量: ", fast_io::mnp::code_cvt(Expression.Variable->Name()));
    }

    auto Variable = Iterator->second;
    auto Value = GenerateExpression(Context, *Expression.Expression);
    auto NewValue = GenerateBinaryExpressionInternal(Context, Variable, Value, Expression.Operator.OperatorKind);
    return Context.Builder.CreateStore(Variable, NewValue);
}

Value* GenerateExpression(GenerationContext& Context, const BoundExpression& Statement) noexcept
{
    switch (Statement.Kind())
    {
        case BoundNodeKind::ErrorExpression:
            InternalCompilerError(std::source_location::current(), "正在绑定错误表达式");
        case BoundNodeKind::LiteralExpression:
            return GenerateLiteralExpression(Context, dynamic_cast<const BoundLiteralExpression&>(Statement));
        case BoundNodeKind::VariableExpression:
            return GenerateVariableExpression(Context, dynamic_cast<const BoundVariableExpression&>(Statement));
        case BoundNodeKind::AssignmentExpression:
            return GenerateAsssignmentExpression(Context, dynamic_cast<const BoundAssignmentExpression&>(Statement));
        case BoundNodeKind::CompoundAssignmentExpression:
            return GenerateCompoundAssignmentExpression(Context, dynamic_cast<const BoundCompoundAssignmentExpression&>(Statement));
        case BoundNodeKind::BinaryExpression:
            return GenerateBinaryExpression(Context, dynamic_cast<const BoundBinaryExpression&>(Statement));
        default:
            InternalCompilerError(std::source_location::current(), "无法识别的表达式, 编号: ", fast_io::mnp::enum_int_view(Statement.Kind()));
    }
    return {};
}

void GenerateReturnStatement(GenerationContext& Context, const BoundReturnStatement& Statement) noexcept
{
    Context.IsTerminating = true;

    auto Value = GenerateExpression(Context, *Statement.Expression);
    Context.Builder.CreateStore(Value, Context.ReturnValue);
    Context.Builder.CreateBr(Context.ReturnBlock);
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
    auto ElseBlock = BasicBlock::Create(Context.Context, "else");
    auto MergeBlock = static_cast<BasicBlock*>(nullptr);
    auto MergeBlockContainsPredecessor = false;

    Context.Builder.CreateCondBr(Condition, ThenBlock, ElseBlock);

    Context.Builder.SetInsertPoint(ThenBlock);
    GenerateStatement(Context, *Statement.ThenStatement);

    if (!Context.IsTerminating)
    {
        MergeBlock = BasicBlock::Create(Context.Context, "merge");
        Context.Builder.CreateBr(MergeBlock);
        MergeBlockContainsPredecessor = true;
    }

    ThenBlock = Context.Builder.GetInsertBlock();

    CurrentFunction->insert(CurrentFunction->end(), ElseBlock);
    Context.Builder.SetInsertPoint(ElseBlock);
    if (!Statement.ElseStatement)
    {
        return;
    }

    GenerateStatement(Context, *Statement.ElseStatement);

    if (!Context.IsTerminating)
    {
        if (!MergeBlock)
        {
            MergeBlock = BasicBlock::Create(Context.Context, "merge");
        }

        Context.Builder.CreateBr(MergeBlock);
        MergeBlockContainsPredecessor = true;
    }

    if (MergeBlock)
    {
        ElseBlock = Context.Builder.GetInsertBlock();
        CurrentFunction->insert(CurrentFunction->end(), MergeBlock);
        Context.Builder.SetInsertPoint(MergeBlock);
    }
}

void GenerateWhileStatement(GenerationContext& Context [[maybe_unused]], const BoundWhileStatement& Statement [[maybe_unused]]) noexcept
{
    // TODO
}

void GenerateStatement(GenerationContext& Context, const BoundStatement& Statement) noexcept
{
    Context.IsTerminating = false;
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

    auto EntryBlock = BasicBlock::Create(Context.Context, "entry", Function);
    Context.Builder.SetInsertPoint(EntryBlock);

    if (FunctionDeclaration.Type != &TypeSymbol::Void)
    {
        auto Alloca = Context.Builder.CreateAlloca(GetLLVMType(Context, FunctionDeclaration.Type), nullptr, "return-value");

        auto ReturnBlock = BasicBlock::Create(Context.Context, "return", Function);
        Context.ReturnBlock = ReturnBlock;

        Context.Builder.SetInsertPoint(ReturnBlock);

        auto ReturnValue = Context.Builder.CreateLoad(Alloca->getAllocatedType(), Alloca, "return-value");
        Context.Builder.CreateRet(ReturnValue);
        Context.ReturnValue = Alloca;
    }

    Context.Builder.SetInsertPoint(EntryBlock);

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
    auto Context = GenerationContext{ LLVMContext, LLVMModule, Builder, {}, {}, {} };

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
    auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, Options, std::nullopt, std::nullopt, CodeGenOptLevel::None);

    fast_io::io::println("目标: ", TargetTriple, "\n", "线程模型: ", Options.ThreadModel == ThreadModel::POSIX ? fast_io::mnp::os_c_str("POSIX") : fast_io::mnp::os_c_str("单线程环境"));

    LLVMModule.setDataLayout(TargetMachine->createDataLayout());
    LLVMModule.setTargetTriple(TargetTriple);

    using namespace std::string_view_literals;
    auto FileName = fast_io::concat(ModuleName, Options::EmitLLVM ? ".ll"sv : ".o"sv);

    auto ErrorCode = std::error_code();
    raw_fd_ostream Out(FileName, ErrorCode, sys::fs::OF_None);
    if (ErrorCode)
    {
        Error("无法打开输出文件: ", ErrorCode.message());
        return;
    }

    if (Options::EmitLLVM)
    {
        LLVMModule.print(Out, nullptr);
        Out.flush();
        fast_io::io::println("编译成功: ", FileName);
        return;
    }

    // https://llvm.org/docs/NewPassManager.html

    // Create the analysis managers.
    // These must be declared in this order so that they are destroyed in the
    // correct order due to inter-analysis-manager references.
    auto LoopAnalysisManager = ::llvm::LoopAnalysisManager();
    auto FunctionAnalysisManager = ::llvm::FunctionAnalysisManager();
    auto CGSCCAnalysisManager = ::llvm::CGSCCAnalysisManager();
    auto ModuleAnalysisManager = ::llvm::ModuleAnalysisManager();

    // Create the new pass manager builder.
    // Take a look at the PassBuilder constructor parameters for more
    // customization, e.g. specifying a TargetMachine or various debugging
    // options.
    auto PassBuilder = ::llvm::PassBuilder();

    // Register all the basic analyses with the managers.
    PassBuilder.registerModuleAnalyses(ModuleAnalysisManager);
    PassBuilder.registerCGSCCAnalyses(CGSCCAnalysisManager);
    PassBuilder.registerFunctionAnalyses(FunctionAnalysisManager);
    PassBuilder.registerLoopAnalyses(LoopAnalysisManager);
    PassBuilder.crossRegisterProxies(LoopAnalysisManager, FunctionAnalysisManager, CGSCCAnalysisManager, ModuleAnalysisManager);

    // Create the pass manager.
    // This one corresponds to a typical -O3 optimization pipeline.
    auto ModulePassManager = PassBuilder.buildPerModuleDefaultPipeline(OptimizationLevel::Os);

    // Optimize the IR
    ModulePassManager.run(LLVMModule, ModuleAnalysisManager);

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

std::string LLVMBackend::TargetTriple() noexcept
{
    return Triple::normalize(sys::getDefaultTargetTriple());
}

llvm::ThreadModel::Model LLVMBackend::ThreadModel() noexcept
{
    return TargetOptions().ThreadModel;
}