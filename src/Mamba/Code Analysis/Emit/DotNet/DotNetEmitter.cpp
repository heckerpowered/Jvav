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

std::vector<Diagnostic> DotNetEmitter::Emit(const BoundProgram& Program, const StringView ModuleName) noexcept
{
    fast_io::io::println("Linking ", fast_io::mnp::code_cvt(ModuleName));
    if (Program.CompilationUnits.empty())
    {
        fast_io::io::perrln("error: no compilation units found");
    }
    Serializer.Write(LinkIdentifier::ModuleName);
    Serializer.Write(ModuleName);
    for (auto&& CompilationUnit : Program.CompilationUnits)
    {
#ifdef DEBUG
        fast_io::io::println("Emit compilation unit");
#endif
        EmitCompilationUnit(*CompilationUnit);
    }

    ::fast_io::obuf_file file(::fast_io::concat(::fast_io::mnp::code_cvt(ModuleName)));
    ::fast_io::write(file, Serializer.Data.begin(), Serializer.Data.end());

    fast_io::io::perrln("Serialized data size: ", Serializer.Data.size());
    return {};
}

void DotNetEmitter::EmitCompilationUnit(const BoundCompilationUnit& CompilationUnit) noexcept
{
    fast_io::io::perrln(fast_io::mnp::pointervw(&*CompilationUnit.GlobalScope), ", ", fast_io::mnp::pointervw(&*CompilationUnit.GlobalScope->Parent));
    EmitScope(*CompilationUnit.GlobalScope);
}

void DotNetEmitter::EmitScope(const BoundScope& Scope) noexcept
{
#ifdef DEBUG
    if (Scope.Parent != nullptr)
    {
        fast_io::io::println("warning: the supplied global scope is actually not a global scope");

        fast_io::io::perrln(fast_io::mnp::pointervw(&Scope), ", ", fast_io::mnp::pointervw(&*Scope.Parent));
    }
    fast_io::io::println("Emit scope, ", Scope.DeclaredSymbols().size(), " symbols");
#endif
    for (auto&& Symbol : Scope.DeclaredSymbols())
    {
        switch (Symbol->Kind())
        {
            case SymbolKind::Function:
                EmitFunction(*std::static_pointer_cast<const FunctionSymbol>(Symbol));
                break;
            case SymbolKind::Variable:
                EmitVariable(*std::static_pointer_cast<const VariableSymbol>(Symbol));
                break;
            case SymbolKind::Parameter:
            case SymbolKind::Type:
            default:
#ifdef DEBUG
                fast_io::io::print("Unsupported symbol");
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

void Mamba::DotNetEmitter::EmitFunction(const FunctionSymbol& Function) noexcept
{
#ifdef DEBUG
    fast_io::io::print(
        "Emit function: ",
        fast_io::mnp::pointervw(&Function.Type),
        " ",
        fast_io::mnp::pointervw(&Function),
        "("
    );

    fast_io::io::perrln("Size: ", Function.Parameters.size());

    auto IsFirst = true;
    for (auto&& Parameter : Function.Parameters)
    {
        if (!IsFirst)
        {
            fast_io::io::print(", ");
        }
        IsFirst = false;
        fast_io::io::perrln(fast_io::mnp::pointervw(&Parameter));
        // fast_io::io::print(fast_io::mnp::code_cvt(*Parameter->Type->Name));
        break;
    }
    fast_io::io::println(")");
#endif
    Serializer.Write(LinkIdentifier::Function);
    Serializer.Write(*Function.Type->Name);
    Serializer.Write(*Function.Name);
    for (auto&& Parameter : Function.Parameters)
    {
        Serializer.Write(*Parameter->Type->Name);
    }
    Serializer.Write(LinkIdentifier::Instruction);
    EmitStatement(*Function.BoundDeclaration->Body);
}

void DotNetEmitter::EmitStatement(const BoundStatement& Statement) noexcept
{
    fast_io::io::perrln("Emit statement: ", static_cast<int>(Statement.Kind()));
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
    Serializer.Write(LinkIdentifier::Instruction);
    for (auto&& Argument : Expression.Arguments)
    {
        EmitExpression(*Argument);
    }

    Serializer.Write(DotNet::OpCodes::Call);
    Serializer.Write(*Expression.Function->Type->Name);
}

void DotNetEmitter::EmitExpression(const BoundExpression& Expression) noexcept
{
#ifdef DEBUG
    fast_io::io::perrln("Emit expression ", static_cast<int>(Expression.Kind()));
#endif
    if (Expression.ConstantValue())
    {
#ifdef DEBUG
        fast_io::io::perrln("Emit constant");
#endif
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
        Serializer.Write(DotNet::OpCodes::Pop);
    }
}

void DotNetEmitter::EmitConstantExpression(const BoundExpression& Expression) noexcept
{
    if (Expression.Type() == TypeSymbol::String)
    {
        Serializer.Write(DotNet::OpCodes::Ldstr);
        Serializer.Write(**Expression.ConstantValue()->Value->StringValue);
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
