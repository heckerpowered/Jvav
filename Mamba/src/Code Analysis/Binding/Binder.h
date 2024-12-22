#pragma once

#include "AssignmentExpressionSyntax.h"
#include "BinaryExpressionSyntax.h"
#include "BoundBlockStatement.h"
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
#include "BoundVariableDeclaration.h"
#include "BoundWhileStatement.h"
#include "BreakStatementSyntax.h"
#include "CallExpressionSyntax.h"
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
    // Binder associates syntax sequences, associates parameter with it's name and function. Associates
    // declarations with it's name and scope, so that entities can be accessed by it's identifier. Associates
    // the break/continue statement with a loop, associates type clauses with declarations.
    class Binder
    {
        // The current scope, where each symbol is stored, and where symbols can be looked up by name in scope.
        // Direct modification of this data member is not permitted; enter and exit the scope safely by modifying
        // this data member to the return value of member function *EnterScope*.
        BoundScope* Scope;

        // Stores a *SyntaxTree* whose data member *Root* is treated as the compilation unit and is only used when
        // binding the compilation unit (calling member function *BindCompilationUnit*). Do not modify this data member
        // unless this binder is being reused.
        const class SyntaxTree* SyntaxTree;

        // Bind according to the actual type of the member. This function determines the type by trying to convert it,
        // instead of the member function *Kind*.
        void BindMember(const MemberSyntax* Member) noexcept;

        // Binding a function declaration creates a scope whose parameter declarations and all statements are inside this
        // scope, and the function's BlockStatement creates a new scope inside the function's scope.
        void BindFunctionDeclaration(const FunctionDeclarationSyntax* FunctionDeclaration) noexcept;

        //
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