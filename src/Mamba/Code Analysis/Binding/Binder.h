#pragma once

#include "DiagnosticBag.h"
#include "MambaCore.h"
#include "ScopeGuard.h"
#include <memory>
#include <vector>

namespace Mamba
{
    class Binder
    {
        std::shared_ptr<class BoundScope> Scope;
        std::shared_ptr<const class SyntaxTree> SyntaxTree;
        DiagnosticBag Diagnostics;

        void BindMember(const std::shared_ptr<const class MemberSyntax> Member) noexcept;
        void BindFunctionDeclaration(const std::shared_ptr<const class FunctionDeclarationSyntax> FunctionDeclaration) noexcept;

        void DeclareFunction(
            const std::shared_ptr<const class FunctionDeclarationSyntax> FunctionDeclaration,
            const std::shared_ptr<const class BoundFunctionDeclaration> BoundFunctionDeclaration
        ) noexcept;

        std::vector<std::shared_ptr<const class ParameterSymbol>> BindParameter(
            const std::shared_ptr<const class FunctionDeclarationSyntax> FunctionDeclaration
        ) noexcept;

        std::shared_ptr<const class BoundStatement> BindStatement(const std::shared_ptr<const class StatementSyntax> Statement) noexcept;
        std::shared_ptr<const class BoundStatement> BindStatementInternal(const std::shared_ptr<const class StatementSyntax> Statement) noexcept;
        std::shared_ptr<const class BoundStatement> BindBlockStatement(const std::shared_ptr<const class BlockStatementSyntax> BlockStatement) noexcept;
        std::shared_ptr<const class BoundStatement> BindExpressionStatement(const std::shared_ptr<const class ExpressionStatementSyntax> ExpressionStatement) noexcept;
        std::shared_ptr<const class BoundExpression> BindExpression(const std::shared_ptr<const class ExpressionSyntax> Expression) noexcept;
        std::shared_ptr<const class BoundStatement> BindVariableDeclaration(const std::shared_ptr<const class VariableDeclarationSyntax> VariableDeclaration) noexcept;
        std::shared_ptr<const class VariableSymbol> BindVariableDeclaration(
            const std::shared_ptr<const class SyntaxToken> Identifier,
            const bool IsReadOnly,
            const std::shared_ptr<const class TypeSymbol> Type,
            const NullableSharedPtr<const class BoundConstant> Constant
        ) noexcept;
        std::shared_ptr<const class BoundStatement> BindIfStatement(const std::shared_ptr<const class IfStatementSyntax> IfStatement) noexcept;
        std::shared_ptr<const class BoundStatement> BindWhileStatement(const std::shared_ptr<const class WhileStatementSyntax> WhileStatement) noexcept;
        std::shared_ptr<const class BoundStatement> BindDoWhileStatement(const std::shared_ptr<const class DoWhileStatementSyntax> DoWhileStatement) noexcept;
        std::shared_ptr<const class BoundStatement> BindForStatement(const std::shared_ptr<const class ForStatementSyntax> ForStatement) noexcept;
        std::shared_ptr<const class BoundStatement> BindBreakStatement(const std::shared_ptr<const class BreakStatementSyntax> BreakStatement) noexcept;
        std::shared_ptr<const class BoundStatement> BindContinueStatement(const std::shared_ptr<const class ContinueStatementSyntax> ContinueStatement) noexcept;
        std::shared_ptr<const class BoundStatement> BindReturnStatement(const std::shared_ptr<const class ReturnStatementSyntax> ReturnStatement) noexcept;
        std::shared_ptr<const class BoundExpression> BindLiteralExpression(const std::shared_ptr<const class LiteralExpressionSyntax> LiteralExpression) noexcept;
        std::shared_ptr<const class BoundExpression> BindNameExpression(const std::shared_ptr<const class NameExpressionSyntax> NameExpression) noexcept;
        std::shared_ptr<const class BoundExpression> BindParenthesizedExpression(const std::shared_ptr<const class ParenthesizedExpressionSyntax> ParenthesizedExpression) noexcept;
        std::shared_ptr<const class BoundExpression> BindUnaryExpression(const std::shared_ptr<const class UnaryExpressionSyntax> UnaryExpression) noexcept;
        std::shared_ptr<const class BoundExpression> BindBinaryExpression(const std::shared_ptr<const class BinaryExpressionSyntax> BinaryExpression) noexcept;
        std::shared_ptr<const class BoundExpression> BindAssignmentExpression(const std::shared_ptr<const class AssignmentExpressionSyntax> AssignmentExpression) noexcept;
        std::shared_ptr<const class BoundExpression> BindCallExpression(const std::shared_ptr<const class CallExpressionSyntax> CallExpression) noexcept;
        NullableSharedPtr<const class TypeSymbol> BindTypeClause(const NullableSharedPtr<const class TypeClauseSyntax> TypeClause) noexcept;

        ScopeGuard EnterScope() noexcept;

        void DeclareBuiltinFunctions() noexcept;
        void DeclareBuiltinFunction();

    public:
        [[nodiscard]] Binder(const std::shared_ptr<const class SyntaxTree> SyntaxTree) noexcept;

        std::shared_ptr<const class BoundCompilationUnit> BindCompilationUnit() noexcept;
    };
} // namespace Mamba