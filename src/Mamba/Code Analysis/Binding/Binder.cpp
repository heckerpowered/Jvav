#include "Binder.h"

#include <memory>
#include <ranges>

#include <fast_io.h>

#include "AssignmentExpressionSyntax.h"
#include "BinaryExpressionSyntax.h"
#include "BlockStatementSyntax.h"
#include "BoundAssignmentExpression.h"
#include "BoundBinaryExpression.h"
#include "BoundBinaryOperator.h"
#include "BoundBlockStatement.h"
#include "BoundCallExpression.h"
#include "BoundCompilationUnit.h"
#include "BoundCompoundAssignmentExpression.h"
#include "BoundConstant.h"
#include "BoundDoWhileStatement.h"
#include "BoundExpression.h"
#include "BoundExpressionStatement.h"
#include "BoundForStatement.h"
#include "BoundFunctionDeclaration.h"
#include "BoundIfStatement.h"
#include "BoundLiteralExpression.h"
#include "BoundNodeKind.h"
#include "BoundReturnStatement.h"
#include "BoundScope.h"
#include "BoundStatement.h"
#include "BoundUnaryExpression.h"
#include "BoundUnaryOperator.h"
#include "BoundVariableDeclaration.h"
#include "BoundVariableExpression.h"
#include "BoundWhileStatement.h"
#include "BreakStatementSyntax.h"
#include "CallExpressionSyntax.h"
#include "CompilationUnitSyntax.h"
#include "ContinueStatementSyntax.h"
#include "DoWhileStatementSyntax.h"
#include "ElseClauseSyntax.h"
#include "ExpressionStatementSyntax.h"
#include "ExpressionSyntax.h"
#include "ForStatementSyntax.h"
#include "FunctionDeclarationSyntax.h"
#include "FunctionSymbol.h"
#include "IfStatementSyntax.h"
#include "LiteralExpressionSyntax.h"
#include "MambaCore.h"
#include "NameExpressionSyntax.h"
#include "ParameterSymbol.h"
#include "ParameterSyntax.h"
#include "ParenthesizedExpressionSyntax.h"
#include "ReturnStatementSyntax.h"
#include "StatementSyntax.h"
#include "Symbol.h"
#include "SyntaxFacts.h"
#include "SyntaxKind.h"
#include "SyntaxTree.h"
#include "TypeClauseSyntax.h"
#include "TypeSymbol.h"
#include "UnaryExpressionSyntax.h"
#include "VariableDeclarationSyntax.h"
#include "WhileStatementSyntax.h"

using namespace Mamba;

std::shared_ptr<const BoundCompilationUnit> Binder::BindCompilationUnit() noexcept
{
    const auto Root = SyntaxTree->Root();
    for (auto&& Member : Root->Members)
    {
        BindMember(Member);
    }

    return std::make_shared<BoundCompilationUnit>(Root, Scope);
}

void Binder::BindMember(const std::shared_ptr<const class MemberSyntax> Member) noexcept
{
    if (const auto FunctionDeclaration = std::dynamic_pointer_cast<const FunctionDeclarationSyntax>(Member))
    {
        BindFunctionDeclaration(FunctionDeclaration);
    }
}

void Binder::BindFunctionDeclaration(const std::shared_ptr<const FunctionDeclarationSyntax> FunctionDeclaration) noexcept
{
    auto FunctionScope = EnterScope();

    const auto Body = BindStatement(FunctionDeclaration->Body);
    const auto BoundFunctionDeclaration = std::make_shared<class BoundFunctionDeclaration>(FunctionDeclaration, Body, Scope);

    FunctionScope.PreLeave();
    DeclareFunction(FunctionDeclaration, BoundFunctionDeclaration);
}

void Binder::DeclareFunction(
    const std::shared_ptr<const FunctionDeclarationSyntax> FunctionDeclaration,
    const std::shared_ptr<const BoundFunctionDeclaration> BoundFunctionDeclaration
) noexcept
{
    const auto Name = FunctionDeclaration->Identifier->Text;
    auto Parameters = BindParameter(FunctionDeclaration);
    const auto Type = BindTypeClause(FunctionDeclaration->Type);
    const auto FunctionSymbol = std::make_shared<class FunctionSymbol>(
        Name,
        std::move(Parameters),
        Type ? Type : TypeSymbol::Void,
        BoundFunctionDeclaration
    );

    Scope->Declare(FunctionSymbol);
}

std::vector<std::shared_ptr<const ParameterSymbol>> Binder::BindParameter(
    const std::shared_ptr<const FunctionDeclarationSyntax> FunctionDeclaration
) noexcept
{
    const auto Parameters = FunctionDeclaration->Parameters->Nodes();

    auto ParameterSymbols = std::vector<std::shared_ptr<const ParameterSymbol>>();
    ParameterSymbols.reserve(Parameters.size());

    auto Index = 0;
    for (auto&& Parameter : Parameters | std::views::transform([](auto&& Node)
                                                               { return std::dynamic_pointer_cast<const ParameterSyntax>(Node); }))
    {
        ++Index;

        const auto Name = Parameter->Identifier->Text;
        const auto Type = std::make_shared<TypeSymbol>(Parameter->Type->Identifier->Text);
        const auto ParameterSymbol = std::make_shared<class ParameterSymbol>(Name, Type, Index);
        Scope->Declare(ParameterSymbol);
    }

    return ParameterSymbols;
}

std::shared_ptr<const BoundStatement> Binder::BindStatement(const std::shared_ptr<const class StatementSyntax> Statement) noexcept
{
    const auto BoundStatement = BindStatementInternal(Statement);
    if (const auto BoundExpressionStatement = std::dynamic_pointer_cast<const class BoundExpressionStatement>(BoundStatement))
    {
        // The following expressions' values can be discarded by default.
        const auto IsAllowedExpression =
            BoundExpressionStatement->Expression->Kind() == BoundNodeKind::AssignmentExpression ||
            BoundExpressionStatement->Expression->Kind() == BoundNodeKind::CallExpression ||
            BoundExpressionStatement->Expression->Kind() == BoundNodeKind::CompoundAssignmentExpression;
        if (!IsAllowedExpression)
        {
            // Warning: The result of the expression is discarded.
            Diagnostics.ReportDiscardExpressionValue(Statement->Location());
        }
    }

    return BoundStatement;
}

std::shared_ptr<const BoundStatement> Binder::BindStatementInternal(const std::shared_ptr<const class StatementSyntax> Statement) noexcept
{
    switch (Statement->Kind())
    {
        case SyntaxKind::BlockStatement:
            return BindBlockStatement(std::dynamic_pointer_cast<const BlockStatementSyntax>(Statement));
        case SyntaxKind::VariableDeclaration:
            return BindVariableDeclaration(std::dynamic_pointer_cast<const VariableDeclarationSyntax>(Statement));
        case SyntaxKind::IfStatement:
            return BindIfStatement(std::dynamic_pointer_cast<const IfStatementSyntax>(Statement));
        case SyntaxKind::WhileStatement:
            return BindWhileStatement(std::dynamic_pointer_cast<const WhileStatementSyntax>(Statement));
        case SyntaxKind::DoWhileStatement:
            return BindDoWhileStatement(std::dynamic_pointer_cast<const DoWhileStatementSyntax>(Statement));
        case SyntaxKind::ForStatement:
            return BindForStatement(std::dynamic_pointer_cast<const ForStatementSyntax>(Statement));
        case SyntaxKind::BreakStatement:
            return BindBreakStatement(std::dynamic_pointer_cast<const BreakStatementSyntax>(Statement));
        case SyntaxKind::ContinueStatement:
            return BindContinueStatement(std::dynamic_pointer_cast<const ContinueStatementSyntax>(Statement));
        case SyntaxKind::ReturnStatement:
            return BindReturnStatement(std::dynamic_pointer_cast<const ReturnStatementSyntax>(Statement));
        case SyntaxKind::ExpressionStatement:
            return BindExpressionStatement(std::dynamic_pointer_cast<const ExpressionStatementSyntax>(Statement));
        default:
            fast_io::io::perrln("Unexpected syntax: {}", fast_io::mnp::code_cvt(SyntaxFacts::ToString(Statement->Kind())));
            std::unreachable();
    }
}

std::shared_ptr<const BoundStatement> Binder::BindBlockStatement(const std::shared_ptr<const BlockStatementSyntax> BlockStatement) noexcept
{
    auto BoundStatements = std::vector<std::shared_ptr<const BoundStatement>>();
    BoundStatements.reserve(BlockStatement->Statements.size());

    const auto BlockScope = EnterScope();

    for (auto&& Statement : BlockStatement->Statements)
    {
        BoundStatements.emplace_back(Hatcher([&]
                                             { return BindStatement(Statement); }));
    }

    return std::make_shared<BoundBlockStatement>(BlockStatement, BoundStatements);
}

std::shared_ptr<const BoundStatement> Binder::BindExpressionStatement(const std::shared_ptr<const ExpressionStatementSyntax> ExpressionStatement) noexcept
{
    const auto Expression = BindExpression(ExpressionStatement->Expression);
    return std::make_shared<BoundExpressionStatement>(ExpressionStatement, Expression);
}

std::shared_ptr<const BoundExpression> Binder::BindExpression(const std::shared_ptr<const ExpressionSyntax> Expression) noexcept
{
    switch (Expression->Kind())
    {
        case SyntaxKind::ParenthesizedExpression:
            return BindParenthesizedExpression(std::dynamic_pointer_cast<const ParenthesizedExpressionSyntax>(Expression));
        case SyntaxKind::LiteralExpression:
            return BindLiteralExpression(std::dynamic_pointer_cast<const LiteralExpressionSyntax>(Expression));
        case SyntaxKind::NameExpression:
            return BindNameExpression(std::dynamic_pointer_cast<const NameExpressionSyntax>(Expression));
        case SyntaxKind::AssignmentExpression:
            return BindAssignmentExpression(std::dynamic_pointer_cast<const AssignmentExpressionSyntax>(Expression));
        case SyntaxKind::UnaryExpression:
            return BindUnaryExpression(std::dynamic_pointer_cast<const UnaryExpressionSyntax>(Expression));
        case SyntaxKind::BinaryExpression:
            return BindBinaryExpression(std::dynamic_pointer_cast<const BinaryExpressionSyntax>(Expression));
        case SyntaxKind::CallExpression:
            return BindCallExpression(std::dynamic_pointer_cast<const CallExpressionSyntax>(Expression));
        default:
            fast_io::io::perrln("Unexpected {}", fast_io::mnp::code_cvt(SyntaxFacts::ToString(Expression->Kind())), ", expression expected.");
            std::unreachable();
    }
}

std::shared_ptr<const BoundStatement> Binder::BindVariableDeclaration(const std::shared_ptr<const VariableDeclarationSyntax> VariableDeclaration) noexcept
{
    const auto IsReadOnly = VariableDeclaration->Keyword->Kind() == SyntaxKind::LetKeyword;
    const auto Type = BindTypeClause(VariableDeclaration->TypeClause);
    const auto Initializer = BindExpression(VariableDeclaration->Initializer);
    const auto VariableType = Type ? Type : Initializer->Type();
    const auto Variable = BindVariableDeclaration(
        VariableDeclaration->Identifier,
        IsReadOnly,
        VariableType,
        Initializer->ConstantValue()
    );

    return std::make_shared<BoundVariableDeclaration>(VariableDeclaration, Variable, Initializer);
}

std::shared_ptr<const VariableSymbol> Binder::BindVariableDeclaration(
    const std::shared_ptr<const SyntaxToken> Identifier,
    const bool IsReadOnly,
    const std::shared_ptr<const TypeSymbol> Type,
    const NullableSharedPtr<const BoundConstant> Constant
) noexcept
{
    const auto Name = Identifier->Text;
    if (!Name)
    {
        return std::make_shared<class VariableSymbol>(std::make_shared<String>(TEXT("?")), IsReadOnly, Type, Constant);
    }

    if (!Scope->LookupVariable(Name).empty())
    {
        Diagnostics.ReportVariableAlreadyDeclared(Identifier->Location(), *Name);
    }

    const auto Variable = std::make_shared<class VariableSymbol>(Name, IsReadOnly, Type, Constant);

    return Variable;
}

std::shared_ptr<const BoundStatement> Binder::BindIfStatement(const std::shared_ptr<const IfStatementSyntax> IfStatement) noexcept
{
    const auto Condition = BindExpression(IfStatement->Condition);

    if (Condition->ConstantValue())
    {
        if (!Condition->ConstantValue()->Value->Value.BooleanValue)
        {
            Diagnostics.ReportUnreachableCode(IfStatement->ElseClause->ElseStatement);
        }
        else if (IfStatement->ElseClause)
        {
            // The if condition always yields true and the else statement is unreachble.
            Diagnostics.ReportUnreachableCode(IfStatement->ElseClause->ElseStatement);
        }
    }

    const auto ThenStatement = BindStatement(IfStatement->ThenStatement);
    const auto ElseStatement = IfStatement->ElseClause ? nullptr : BindStatement(IfStatement->ElseClause->ElseStatement);
    return std::make_shared<BoundIfStatement>(IfStatement, Condition, ThenStatement, ElseStatement);
}

std::shared_ptr<const BoundStatement> Binder::BindWhileStatement(const std::shared_ptr<const WhileStatementSyntax> WhileStatement) noexcept
{
    const auto WhileScope = EnterScope();

    const auto Condition = BindExpression(WhileStatement->Condition);

    if (Condition->ConstantValue() && !Condition->ConstantValue()->Value->Value.BooleanValue)
    {
        Diagnostics.ReportUnreachableCode(WhileStatement->Body);
    }

    const auto Body = BindStatement(WhileStatement->Body);
    return std::make_shared<BoundWhileStatement>(WhileStatement, Condition, Body);
}

std::shared_ptr<const BoundStatement> Binder::BindDoWhileStatement(const std::shared_ptr<const DoWhileStatementSyntax> DoWhileStatement) noexcept
{
    const auto Body = BindStatement(DoWhileStatement->Body);
    const auto Condition = BindExpression(DoWhileStatement->Condition);
    return std::make_shared<BoundDoWhileStatement>(DoWhileStatement, Body, Condition);
}

std::shared_ptr<const BoundStatement> Binder::BindForStatement(const std::shared_ptr<const ForStatementSyntax> ForStatement) noexcept
{
    const auto ForScope = EnterScope();

    const auto InitStatement = BindStatement(ForStatement->InitStatement);
    const auto Condition = BindExpression(ForStatement->Condition);
    const auto Expression = BindExpression(ForStatement->Expression);
    const auto Body = BindStatement(ForStatement->Body);

    return std::make_shared<BoundForStatement>(ForStatement, InitStatement, Condition, Expression, Body);
}

std::shared_ptr<const BoundStatement> Binder::BindBreakStatement(const std::shared_ptr<const BreakStatementSyntax> BreakStatement) noexcept
{
    fast_io::io::perrln("Failed to bind break statement: not implemented yet.");
    return {};
}

std::shared_ptr<const BoundStatement> Binder::BindContinueStatement(const std::shared_ptr<const ContinueStatementSyntax> ContinueStatement) noexcept
{
    fast_io::io::perrln("Failed to bind continue statement: not implemented yet.");
    return {};
}

std::shared_ptr<const BoundStatement> Binder::BindReturnStatement(const std::shared_ptr<const ReturnStatementSyntax> ReturnStatement) noexcept
{
    // TODO: Diagnostics
    const auto Expression = BindExpression(ReturnStatement->Expression);
    return std::make_shared<BoundReturnStatement>(ReturnStatement, Expression);
}

std::shared_ptr<const BoundExpression> Binder::BindLiteralExpression(const std::shared_ptr<const LiteralExpressionSyntax> LiteralExpression) noexcept
{
    return std::make_shared<BoundLiteralExpression>(LiteralExpression, LiteralExpression->Value);
}

std::shared_ptr<const BoundExpression> Binder::BindNameExpression(const std::shared_ptr<const NameExpressionSyntax> NameExpression) noexcept
{
    const auto Name = NameExpression->IdentifierToken->Text;
    const auto Variables = Scope->LookupVariable(Name);
    if (Variables.empty())
    {
        // TODO: Diagnostics - undeclaraed identifier
        return {};
    }
    else if (Variables.size() > 1)
    {
        // TODO: Diagnostics - ambiguous identifier
        return {};
    }

    return std::make_shared<BoundVariableExpression>(NameExpression, Variables.front());
}

std::shared_ptr<const BoundExpression> Binder::BindParenthesizedExpression(const std::shared_ptr<const ParenthesizedExpressionSyntax> ParenthesizedExpression) noexcept
{
    return BindExpression(ParenthesizedExpression->Expression);
}

std::shared_ptr<const BoundExpression> Binder::BindUnaryExpression(const std::shared_ptr<const UnaryExpressionSyntax> UnaryExpression) noexcept
{
    const auto BoundOperand = BindExpression(UnaryExpression->Operand);

    // TODO: Check operand type

    const auto BoundOperator = BoundUnaryOperator::Bind(
        UnaryExpression->OperatorToken->Kind(),
        BoundOperand->Type()
    );
    if (!BoundOperand)
    {
        // TODO: Diagnostics - undefined unary operator
        return {};
    }

    return std::make_shared<BoundUnaryExpression>(UnaryExpression, *BoundOperator, BoundOperand);
}

std::shared_ptr<const BoundExpression> Binder::BindBinaryExpression(const std::shared_ptr<const BinaryExpressionSyntax> BinaryExpression) noexcept
{
    const auto BoundLeft = BindExpression(BinaryExpression->Left);
    const auto BoundRight = BindExpression(BinaryExpression->Right);

    // TODO: Check operand types

    const auto BoundOperator = BoundBinaryOperator::Bind(
        BinaryExpression->OperatorToken->Kind(),
        BoundLeft->Type(),
        BoundRight->Type()
    );
    if (!BoundOperator)
    {
        // TODO: Diagnostics - undefined binary operator
        return {};
    }

    return std::make_shared<BoundBinaryExpression>(BinaryExpression, BoundLeft, *BoundOperator, BoundRight);
}

std::shared_ptr<const BoundExpression> Binder::BindAssignmentExpression(const std::shared_ptr<const AssignmentExpressionSyntax> AssignmentExpression) noexcept
{
    const auto Name = AssignmentExpression->IdentifierToken->Text;
    const auto BoundExpression = BindExpression(AssignmentExpression->Expression);

    const auto Variables = Scope->LookupVariable(Name);
    if (Variables.empty())
    {
        // TODO: Diagnostics - undeclaraed identifier
        return {};
    }
    else if (Variables.size() > 1)
    {
        // TODO: Diagnostics - ambiguous identifier
        return {};
    }

    const auto Variable = Variables.front();
    if (Variable->IsConstant)
    {
        // TODO: Diagnostics - constant variable cannot be assigned
    }

    if (AssignmentExpression->AssignmentToken->Kind() != SyntaxKind::EqualsToken)
    {
        const auto EquivalentOperatorTokenKind = SyntaxFacts::GetBinaryOperatorOfAssignmentOperator(AssignmentExpression->AssignmentToken->Kind());
        const auto BoundOperator = BoundBinaryOperator::Bind(
            EquivalentOperatorTokenKind,
            Variable->Type,
            BoundExpression->Type()
        );

        if (!BoundOperator)
        {
            // TODO: Report undefined binary operator
            return {};
        }

        // TODO: implement conversion
        return std::make_shared<BoundCompoundAssignmentExpression>(
            AssignmentExpression,
            Variable,
            *BoundOperator,
            BoundExpression
        );
    }

    return std::make_shared<BoundAssignmentExpression>(AssignmentExpression, Variable, BoundExpression);
}

std::shared_ptr<const BoundExpression> Binder::BindCallExpression(const std::shared_ptr<const CallExpressionSyntax> CallExpression) noexcept
{
    auto BoundArguments = std::vector<std::shared_ptr<const BoundExpression>>();
    BoundArguments.reserve(CallExpression->Arguments->Count());

    for (auto&& Argument : CallExpression->Arguments->Nodes())
    {
        // Argument is guaranteed to be a expression by the parser.
        const auto BoundArgument = BindExpression(std::static_pointer_cast<const ExpressionSyntax>(Argument));

        // TODO: Use hatcher to avoid copy
        BoundArguments.emplace_back(BoundArgument);
    }

    // TODO: Refinement Jvav standard
    // [basic.lookup]/? : If the declarations found by name lookup all denote functions,
    //                    the declarations are said to form an overload set.
    const auto FunctionSet = Scope->LookupFunction(CallExpression->Identifier->Text);
    if (FunctionSet.empty())
    {
        // TODO: Diagnostics - undeclaraed function
        fast_io::io::perrln("Undeclaraed function '", fast_io::mnp::code_cvt(*CallExpression->Identifier->Text), "'");
        return {};
    }

    // TODO: Overload resolution. Before overload resolution is avaiable, function override is not permitted.
    const auto Function = FunctionSet.front();

    if (CallExpression->Arguments->Count() != Function->Parameters.size())
    {
        // TODO: Report argument count mismatch
        fast_io::io::perrln("Failed to bind call expression: argument count mismatch.");
        return {};
    }

    // TODO: implement conversion

    return std::make_shared<BoundCallExpression>(CallExpression, Function, std::move(BoundArguments));
}

NullableSharedPtr<const TypeSymbol> Binder::BindTypeClause(const NullableSharedPtr<const TypeClauseSyntax> TypeClause) noexcept
{
    if (!TypeClause)
    {
        return {};
    }

    const auto Types = Scope->LookupType(TypeClause->Identifier->Text);
    if (Types.empty())
    {
        // TODO: Diagnostics - undeclaraed type
        return {};
    }

    return Types.front();
}

ScopeGuard Binder::EnterScope() noexcept
{
    return ScopeGuard(Scope);
}

void Binder::DeclareBuiltinFunctions() noexcept
{
}

Binder::Binder(const class SyntaxTree* SyntaxTree) noexcept :
    Scope(std::make_shared<BoundScope>(nullptr)), SyntaxTree(SyntaxTree)
{
}
