#pragma once

#include "BoundBlockStatement.h"
#include "BoundCallExpression.h"
#include "BoundCompilationUnit.h"
#include "BoundExpression.h"
#include "BoundExpressionStatement.h"
#include "BoundNopStatement.h"
#include "BoundScope.h"
#include "DiagnosticBag.h"
#include "Emitter.h"
#include "FunctionSymbol.h"
#include "Serializer.h"
#include "VariableSymbol.h"

namespace Mamba
{
    class DotNetEmitter final : public Emitter
    {
        DiagnosticBag Diagnostics;
        ::Mamba::Serializer Serializer;

    public:
        virtual std::vector<Diagnostic> Emit(const BoundProgram& Program, const StringView ModuleName) noexcept override;

    private:
        void EmitCompilationUnit(const BoundCompilationUnit& CompilationUnit) noexcept;
        void EmitScope(const BoundScope& Scope) noexcept;
        void EmitVariable(const VariableSymbol& VariableSymbol) noexcept;
        void EmitFunction(const FunctionSymbol& Function) noexcept;
        void EmitStatement(const BoundStatement& Statement) noexcept;
        void EmitNopStatement(const BoundNopStatement& Statement) noexcept;
        void EmitCallExpression(const BoundCallExpression& Expression) noexcept;
        void EmitExpression(const BoundExpression& Expression) noexcept;
        void EmitExpressionStatement(const BoundExpressionStatement& Statement) noexcept;
        void EmitConstantExpression(const BoundExpression& Expression) noexcept;
        void EmitBlockStatement(const BoundBlockStatement& BlockStatement);
    };
} // namespace Mamba