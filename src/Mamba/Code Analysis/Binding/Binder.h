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
        DiagnosticBag Diagnostics;

        void BindMember(const MemberSyntax* Member) noexcept;
        void BindFunctionDeclaration(const FunctionDeclarationSyntax* FunctionDeclaration) noexcept;

        void DeclareFunction(
            const FunctionDeclarationSyntax* FunctionDeclaration,
            const BoundFunctionDeclaration* BoundFunctionDeclaration
        ) noexcept;

        VariableSymbol* BindVariableDeclaration(const SyntaxToken* Identifier, bool IsReadOnly, const TypeSymbol* Type, Constant Constant) noexcept;
        std::vector<ParameterSymbol*> BindParameter(const FunctionDeclarationSyntax* FunctionDeclaration) noexcept;

        BoundStatement* BindBreakStatement(const std::shared_ptr<const class BreakStatementSyntax> BreakStatement) noexcept;
        BoundExpression* BindParenthesizedExpression(const ParenthesizedExpressionSyntax* ParenthesizedExpression) noexcept;
        BoundAssignmentExpression* BindAssignmentExpression(const AssignmentExpressionSyntax* AssignmentExpression) noexcept;
        BoundExpressionStatement* BindExpressionStatement(const ExpressionStatementSyntax* ExpressionStatement) noexcept;
        BoundVariableDeclaration* BindVariableDeclaration(const VariableDeclarationSyntax* VariableDeclaration) noexcept;
        NullablePointer<TypeSymbol*> BindTypeClause(NullablePointer<const TypeClauseSyntax> TypeClause) noexcept;
        BoundLiteralExpression* BindLiteralExpression(const LiteralExpressionSyntax* LiteralExpression) noexcept;
        BoundDoWhileStatement* BindDoWhileStatement(const DoWhileStatementSyntax* DoWhileStatement) noexcept;
        BoundBinaryExpression* BindBinaryExpression(const BinaryExpressionSyntax* BinaryExpression) noexcept;
        BoundUnaryExpression* BindUnaryExpression(const UnaryExpressionSyntax* UnaryExpression) noexcept;
        BoundReturnStatement* BindReturnStatement(const ReturnStatementSyntax* ReturnStatement) noexcept;
        BoundVariableExpression* BindNameExpression(const NameExpressionSyntax* NameExpression) noexcept;
        BoundStatement* BindContinueStatement(const ContinueStatementSyntax* ContinueStatement) noexcept;
        BoundBlockStatement* BindBlockStatement(const BlockStatementSyntax* BlockStatement) noexcept;
        BoundWhileStatement* BindWhileStatement(const WhileStatementSyntax* WhileStatement) noexcept;
        BoundCallExpression* BindCallExpression(const CallExpressionSyntax* CallExpression) noexcept;
        BoundForStatement* BindForStatement(const ForStatementSyntax* ForStatement) noexcept;
        BoundStatement* BindStatementInternal(const StatementSyntax* Statement) noexcept;
        BoundIfStatement* BindIfStatement(const IfStatementSyntax* IfStatement) noexcept;
        BoundExpression* BindExpression(const ExpressionSyntax* Expression) noexcept;
        BoundStatement* BindStatement(const StatementSyntax* Statement) noexcept;

        ScopeGuard EnterScope() noexcept;

        void DeclareBuiltinFunctions() noexcept;
        void DeclareBuiltinFunction();

    public:
        [[nodiscard]] Binder(const class SyntaxTree* SyntaxTree) noexcept;

        BoundCompilationUnit* BindCompilationUnit() noexcept;
    };
} // namespace Mamba