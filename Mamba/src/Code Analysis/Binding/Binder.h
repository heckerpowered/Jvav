#pragma once

#include "AssignmentExpressionSyntax.h"
#include "BinaryExpressionSyntax.h"
#include "BoundAssignmentExpression.h"
#include "BoundBinaryExpression.h"
#include "BoundBlockStatement.h"
#include "BoundCallExpression.h"
#include "BoundCompilationUnit.h"
#include "BoundDoWhileStatement.h"
#include "BoundExpression.h"
#include "BoundExpressionStatement.h"
#include "BoundForStatement.h"
#include "BoundFunctionDeclaration.h"
#include "BoundIfStatement.h"
#include "BoundLiteralExpression.h"
#include "BoundReturnStatement.h"
#include "BoundScope.h"
#include "BoundStatement.h"
#include "BoundUnaryExpression.h"
#include "BoundVariableDeclaration.h"
#include "BoundVariableExpression.h"
#include "BoundWhileStatement.h"
#include "BreakStatementSyntax.h"
#include "CallExpressionSyntax.h"
#include "CompilationUnitSyntax.h"
#include "Constant.h"
#include "ContinueStatementSyntax.h"
#include "DiagnosticBag.h"
#include "DoWhileStatementSyntax.h"
#include "ExpressionStatementSyntax.h"
#include "ForStatementSyntax.h"
#include "FunctionDeclarationSyntax.h"
#include "IfStatementSyntax.h"
#include "LiteralExpressionSyntax.h"
#include "MambaCore.h"
#include "NameExpressionSyntax.h"
#include "ParameterSymbol.h"
#include "ParenthesizedExpressionSyntax.h"
#include "ReturnStatementSyntax.h"
#include "ScopeGuard.h"
#include "SyntaxTree.h"
#include "UnaryExpressionSyntax.h"
#include "VariableDeclarationSyntax.h"
#include "WhileStatementSyntax.h"

#include <vector>

namespace Mamba
{
    class Binder
    {
        BoundScope* Scope;
        const class SyntaxTree* SyntaxTree;

        void BindMember(const MemberSyntax* Member) noexcept;
        void BindFunctionDeclaration(const FunctionDeclarationSyntax* FunctionDeclaration) noexcept;

        void DeclareFunction(const FunctionDeclarationSyntax* FunctionDeclaration, const BoundFunctionDeclaration* BoundFunctionDeclaration, std::vector<const ParameterSymbol*>&& Parameters) noexcept;

        VariableSymbol* BindVariableDeclaration(const SyntaxToken* Identifier, bool IsReadOnly, const TypeSymbol* Type, Constant Constant) noexcept;
        std::vector<const ParameterSymbol*> BindParameter(const FunctionDeclarationSyntax* FunctionDeclaration) noexcept;

        BoundStatement* BindBreakStatement(const BreakStatementSyntax* BreakStatement) noexcept;
        BoundExpression* BindParenthesizedExpression(const ParenthesizedExpressionSyntax* ParenthesizedExpression) noexcept;
        BoundExpressionStatement* BindExpressionStatement(const ExpressionStatementSyntax* ExpressionStatement) noexcept;
        BoundVariableDeclaration* BindVariableDeclaration(const VariableDeclarationSyntax* VariableDeclaration) noexcept;
        NullablePointer<const TypeSymbol> BindTypeClause(NullablePointer<const TypeClauseSyntax> TypeClause) noexcept;
        BoundExpression* BindAssignmentExpression(const AssignmentExpressionSyntax* AssignmentExpression) noexcept;
        BoundLiteralExpression* BindLiteralExpression(const LiteralExpressionSyntax* LiteralExpression) noexcept;
        BoundDoWhileStatement* BindDoWhileStatement(const DoWhileStatementSyntax* DoWhileStatement) noexcept;
        BoundReturnStatement* BindReturnStatement(const ReturnStatementSyntax* ReturnStatement) noexcept;
        BoundStatement* BindContinueStatement(const ContinueStatementSyntax* ContinueStatement) noexcept;
        BoundExpression* BindBinaryExpression(const BinaryExpressionSyntax* BinaryExpression) noexcept;
        BoundBlockStatement* BindBlockStatement(const BlockStatementSyntax* BlockStatement) noexcept;
        BoundWhileStatement* BindWhileStatement(const WhileStatementSyntax* WhileStatement) noexcept;
        BoundExpression* BindUnaryExpression(const UnaryExpressionSyntax* UnaryExpression) noexcept;
        BoundExpression* BindNameExpression(const NameExpressionSyntax* NameExpression) noexcept;
        BoundExpression* BindCallExpression(const CallExpressionSyntax* CallExpression) noexcept;
        BoundForStatement* BindForStatement(const ForStatementSyntax* ForStatement) noexcept;
        BoundStatement* BindStatementInternal(const StatementSyntax* Statement) noexcept;
        BoundIfStatement* BindIfStatement(const IfStatementSyntax* IfStatement) noexcept;
        BoundExpression* BindExpression(const ExpressionSyntax* Expression) noexcept;
        BoundStatement* BindStatement(const StatementSyntax* Statement) noexcept;

        ScopeGuard EnterScope() noexcept;

        void DeclareBuiltinFunctions() noexcept;
        void DeclareBuiltinFunction() noexcept;
        void DeclareBuiltinTypes() noexcept;

    public:
        DiagnosticBag Diagnostics;

        [[nodiscard]] Binder(const class SyntaxTree* SyntaxTree) noexcept;

        BoundCompilationUnit* BindCompilationUnit() noexcept;
    };
} // namespace Mamba