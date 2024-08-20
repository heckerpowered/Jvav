#include "DotNetEmitter.h"
#include "BoundCallExpression.h"
#include "BoundConstant.h"
#include "BoundExpressionStatement.h"
#include "BoundNodeKind.h"
#include "BoundScope.h"
#include "DotNet/OpCodes.h"
#include "fast_io_core_impl/integers/impl.h"
#include "LinkIdentifier.h"
#include "ParameterSymbol.h"
#include "TypeSymbol.h"

#include <fast_io.h>
#include <fast_io_device.h>

#include <memory>

using namespace Mamba;

constexpr auto LinkerVersion = 1uz;

std::vector<Diagnostic> DotNetEmitter::Emit(const BoundProgram& Program, const StringView ModuleName) noexcept
{
    Serializer.Write(LinkerVersion);
    Serializer.Write(ModuleName);
    for (auto&& CompilationUnit : Program.CompilationUnits)
    {
        EmitCompilationUnit(*CompilationUnit);
    }

    ::fast_io::obuf_file file(::fast_io::concat(::fast_io::mnp::code_cvt(ModuleName)));
    ::fast_io::write(file, Serializer.Data.begin(), Serializer.Data.end());

    return {};
}

void DotNetEmitter::EmitCompilationUnit(const BoundCompilationUnit& CompilationUnit) noexcept
{
    EmitScope(*CompilationUnit.GlobalScope);
}

void DotNetEmitter::EmitScope(const BoundScope& Scope) noexcept
{
    for (auto&& Symbol : Scope.DeclaredSymbols())
    {
        switch (Symbol->Kind())
        {
            case SymbolKind::Function:
                EmitFunction(std::static_pointer_cast<const FunctionSymbol>(Symbol));
                break;
            case SymbolKind::Variable:
                EmitVariable(*std::static_pointer_cast<const VariableSymbol>(Symbol));
                break;
            case SymbolKind::Parameter:
                break;
            case SymbolKind::Type:
                break;
            default:
#ifdef DEBUG
                fast_io::io::println("Unsupported symbol.");
#endif
                break;
        }
    }

    for (auto&& Scope : Scope.Children)
    {
        EmitScope(*Scope);
    }
}

void DotNetEmitter::EmitVariable(const VariableSymbol& VariableSymbol) noexcept
{
    Serializer.Write(LinkIdentifier::Variable);
    Serializer.Write(*VariableSymbol.Type->Name);
    Serializer.Write(*VariableSymbol.Name);
}

void DotNetEmitter::EmitFunction(const std::shared_ptr<const FunctionSymbol> Function) noexcept
{
    Serializer.Write(LinkIdentifier::Function);
    Serializer.Write(*Function->Type->Name);
    Serializer.Write(*Function->Name);
    Serializer.Write(Function->Parameters.size());
    for (auto&& Parameter : Function->Parameters)
    {
        Serializer.Write(*Parameter->Type->Name);
    }
    EmitStatement(*Function->BoundDeclaration->Body);
}

void DotNetEmitter::EmitStatement(const BoundStatement& Statement) noexcept
{
    switch (Statement.Kind())
    {
        case BoundNodeKind::ExpressionStatement:
            EmitExpressionStatement(static_cast<const BoundExpressionStatement&>(Statement));
            break;
        case BoundNodeKind::BlockStatement:
            EmitBlockStatement(static_cast<const BoundBlockStatement&>(Statement));
            break;
        default:
            fast_io::io::perrln("Unsupported statement: ", static_cast<int>(Statement.Kind()));
            break;
    }
}

void DotNetEmitter::EmitCallExpression(const BoundCallExpression& Expression) noexcept
{
    for (auto&& Argument : Expression.Arguments)
    {
        EmitExpression(*Argument);
    }

    Serializer.Write(LinkIdentifier::Instruction);
    Serializer.Write(DotNet::OpCodes::Call);
    Serializer.Write(LinkIdentifier::String);
    Serializer.Write(*Expression.Function->Name);
}

void DotNetEmitter::EmitExpression(const BoundExpression& Expression) noexcept
{
    if (Expression.ConstantValue())
    {
        EmitConstantExpression(Expression);
        return;
    }

    switch (Expression.Kind())
    {
        case BoundNodeKind::CallExpression:
            EmitCallExpression(static_cast<const BoundCallExpression&>(Expression));
            break;
        default:
            fast_io::io::perrln("Unsupported expression");
    }
}

void DotNetEmitter::EmitExpressionStatement(const BoundExpressionStatement& Statement) noexcept
{
    EmitExpression(*Statement.Expression);
    if (Statement.Expression->Type() != TypeSymbol::Void)
    {
        Serializer.Write(LinkIdentifier::Instruction);
        Serializer.Write(DotNet::OpCodes::Pop);
    }
}

void DotNetEmitter::EmitConstantExpression(const BoundExpression& Expression) noexcept
{
    if (Expression.Type() == TypeSymbol::String)
    {
        Serializer.Write(LinkIdentifier::Instruction);
        Serializer.Write(DotNet::OpCodes::Ldstr);
        Serializer.Write(LinkIdentifier::String);
        Serializer.Write(StringView(**Expression.ConstantValue()->Value->StringValue));
        return;
    }

    fast_io::io::perrln("Unsupported constant expression");
}

void DotNetEmitter::EmitBlockStatement(const BoundBlockStatement& BlockStatement)
{
    for (auto&& Statement : BlockStatement.Statements)
    {
        EmitStatement(*Statement);
    }
}
