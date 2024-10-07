#include <source_location>

#include <fast_io.h>

#include "Binder.h"

#include "MambaCore.h"

#include "BoundAssignmentExpression.h"
#include "BoundBinaryExpression.h"
#include "BoundCallExpression.h"
#include "BoundCompoundAssignmentExpression.h"
#include "BoundErrorExpression.h"
#include "BoundExpression.h"
#include "BoundExpressionStatement.h"
#include "BoundScope.h"
#include "BoundUnaryExpression.h"
#include "BoundUnaryOperator.h"
#include "BoundVariableExpression.h"
#include "Constant.h"
#include "SyntaxFacts.h"
#include "TypeSymbol.h"

using namespace Mamba;

BoundCompilationUnit* Binder::BindCompilationUnit() noexcept
{
    auto Root = SyntaxTree->Root();
    for (auto&& Member : Root->Members)
    {
        BindMember(Member);
    }

    return new BoundCompilationUnit(Root, Scope);
}

void Binder::BindMember(const MemberSyntax* Member) noexcept
{
    if (auto FunctionDeclaration = dynamic_cast<const FunctionDeclarationSyntax*>(Member))
    {
        BindFunctionDeclaration(FunctionDeclaration);
    }
}

void Binder::BindFunctionDeclaration(const FunctionDeclarationSyntax* FunctionDeclaration) noexcept
{
    auto FunctionScope = EnterScope();
    auto Parameters = BindParameter(FunctionDeclaration);

    auto Body = BindStatement(FunctionDeclaration->Body);
    auto BoundFunctionDeclaration = new class BoundFunctionDeclaration(FunctionDeclaration, Body);

    DeclareFunction(FunctionDeclaration, BoundFunctionDeclaration, std::move(Parameters));
}

void Binder::DeclareFunction(const FunctionDeclarationSyntax* FunctionDeclaration, const BoundFunctionDeclaration* BoundFunctionDeclaration, std::vector<const ParameterSymbol*>&& Parameters) noexcept
{
    auto Name = FunctionDeclaration->Identifier->Text();
    auto Type = BindTypeClause(FunctionDeclaration->Type);
    auto FunctionSymbol = new class FunctionSymbol(Name, std::move(Parameters), Type ? Type : &TypeSymbol::Void, BoundFunctionDeclaration);

    Scope->Declare(FunctionSymbol);
}

std::vector<const ParameterSymbol*> Binder::BindParameter(
    const FunctionDeclarationSyntax* FunctionDeclaration
) noexcept
{
    auto Parameters = FunctionDeclaration->Parameters.Nodes();
    auto ParameterSymbols = std::vector<const ParameterSymbol*>();
    ParameterSymbols.reserve(Parameters.size());

#if __cpp_lib_ranges_enumerate >= 202302L
    for (auto [Index, Parameter] :
         Parameters |
             std::views::transform([](auto&& Node) { return dynamic_cast<const ParameterSyntax*>(Node); }) | std::views::enumerate)
    {
        auto Name = Parameter->Identifier->Text();
        auto Type = new TypeSymbol(Parameter->Type->Identifier->Text());
        auto ParameterSymbol = new class ParameterSymbol(Name, Type, Index);
        Scope->Declare(ParameterSymbol);
    }
#else
    auto Index = 0;
    for (auto&& Parameter :
         Parameters | std::views::transform([](auto&& Node) { return dynamic_cast<const ParameterSyntax*>(Node); }))
    {
        ++Index;

        auto Name = Parameter->Identifier->Text();
        auto Type = new TypeSymbol(Parameter->Type->Identifier->Text());
        auto ParameterSymbol = new class ParameterSymbol(Name, Type, Index);
        Scope->Declare(ParameterSymbol);
    }

    return ParameterSymbols;
#endif
}

BoundStatement* Binder::BindStatement(const StatementSyntax* Statement) noexcept
{
    auto BoundStatement = BindStatementInternal(Statement);
    if (auto BoundExpressionStatement = dynamic_cast<const class BoundExpressionStatement*>(BoundStatement))
    {
        // The following expressions' values can be discarded by default.
        auto IsAllowedExpression =
            BoundExpressionStatement->Expression->Kind() == BoundNodeKind::AssignmentExpression ||
            BoundExpressionStatement->Expression->Kind() == BoundNodeKind::CallExpression ||
            BoundExpressionStatement->Expression->Kind() == BoundNodeKind::CompoundAssignmentExpression ||
            BoundExpressionStatement->Expression->Kind() == BoundNodeKind::ErrorExpression;
        if (!IsAllowedExpression)
        {
            // Warning: The result of the expression is discarded.
            Diagnostics.ReportDiscardExpressionValue(Statement->Location());
        }
    }

    return BoundStatement;
}

BoundStatement* Binder::BindStatementInternal(const StatementSyntax* Statement) noexcept
{
    switch (Statement->Kind())
    {
        case SyntaxKind::BlockStatement:
            return BindBlockStatement(static_cast<const BlockStatementSyntax*>(Statement));
        case SyntaxKind::VariableDeclaration:
            return BindVariableDeclaration(static_cast<const VariableDeclarationSyntax*>(Statement));
        case SyntaxKind::IfStatement:
            return BindIfStatement(static_cast<const IfStatementSyntax*>(Statement));
        case SyntaxKind::WhileStatement:
            return BindWhileStatement(static_cast<const WhileStatementSyntax*>(Statement));
        case SyntaxKind::DoWhileStatement:
            return BindDoWhileStatement(static_cast<const DoWhileStatementSyntax*>(Statement));
        case SyntaxKind::ForStatement:
            return BindForStatement(static_cast<const ForStatementSyntax*>(Statement));
        case SyntaxKind::BreakStatement:
            return BindBreakStatement(static_cast<const BreakStatementSyntax*>(Statement));
        case SyntaxKind::ContinueStatement:
            return BindContinueStatement(static_cast<const ContinueStatementSyntax*>(Statement));
        case SyntaxKind::ReturnStatement:
            return BindReturnStatement(static_cast<const ReturnStatementSyntax*>(Statement));
        case SyntaxKind::ExpressionStatement:
            return BindExpressionStatement(static_cast<const ExpressionStatementSyntax*>(Statement));
        default:
            InternalCompilerError(std::source_location::current(), "无法绑定语句，类型: ", fast_io::mnp::code_cvt(SyntaxFacts::ToString(Statement->Kind())));
    }
}

BoundBlockStatement* Binder::BindBlockStatement(const BlockStatementSyntax* BlockStatement) noexcept
{
    auto BlockScope = EnterScope();

    auto BoundStatements = BlockStatement->Statements |
                           std::views::transform([&](auto Statement) -> const BoundStatement* { return BindStatement(Statement); }) |
                           std::ranges::to<std::vector>();

    return new BoundBlockStatement(BlockStatement, std::move(BoundStatements));
}

BoundExpressionStatement* Binder::BindExpressionStatement(const ExpressionStatementSyntax* ExpressionStatement) noexcept
{
    auto Expression = BindExpression(ExpressionStatement->Expression);
    return new BoundExpressionStatement(ExpressionStatement, Expression);
}

BoundExpression* Binder::BindExpression(const ExpressionSyntax* Expression) noexcept
{
    switch (Expression->Kind())
    {
        case SyntaxKind::ParenthesizedExpression:
            return BindParenthesizedExpression(static_cast<const ParenthesizedExpressionSyntax*>(Expression));
        case SyntaxKind::LiteralExpression:
            return BindLiteralExpression(static_cast<const LiteralExpressionSyntax*>(Expression));
        case SyntaxKind::NameExpression:
            return BindNameExpression(static_cast<const NameExpressionSyntax*>(Expression));
        case SyntaxKind::AssignmentExpression:
            return BindAssignmentExpression(static_cast<const AssignmentExpressionSyntax*>(Expression));
        case SyntaxKind::UnaryExpression:
            return BindUnaryExpression(static_cast<const UnaryExpressionSyntax*>(Expression));
        case SyntaxKind::BinaryExpression:
            return BindBinaryExpression(static_cast<const BinaryExpressionSyntax*>(Expression));
        case SyntaxKind::CallExpression:
            return BindCallExpression(static_cast<const CallExpressionSyntax*>(Expression));
        default:
#ifdef DEBUG
            InternalCompilerError(
                std::source_location::current(),
                "Unexpected {}",
                fast_io::mnp::code_cvt(SyntaxFacts::ToString(Expression->Kind())),
                ", expression expected."
            );
#else
            std::unreachable();
#endif
    }
}

BoundVariableDeclaration* Binder::BindVariableDeclaration(const VariableDeclarationSyntax* VariableDeclaration) noexcept
{
    auto IsReadOnly = VariableDeclaration->Keyword->Kind() == SyntaxKind::LetKeyword;
    auto Type = BindTypeClause(VariableDeclaration->TypeClause);
    auto Initializer = BindExpression(VariableDeclaration->Initializer);
    auto VariableType = Type ? Type : Initializer->Type();
    auto Variable = BindVariableDeclaration(
        VariableDeclaration->Identifier,
        IsReadOnly,
        VariableType,
        Initializer->ConstantValue()
    );

    Scope->Declare(Variable);
    return new BoundVariableDeclaration(VariableDeclaration, Variable, Initializer);
}

VariableSymbol* Binder::BindVariableDeclaration(const SyntaxToken* Identifier, bool IsReadOnly, const TypeSymbol* Type, Constant Constant) noexcept
{
    auto Name = Identifier->Text();
    if (Name.empty())
    {
        InternalCompilerError(std::source_location::current(), "标识符无效");
    }

    if (!Scope->LookupVariable(Name).empty())
    {
        Diagnostics.ReportVariableAlreadyDeclared(Identifier->Location(), Name);
    }

    return new VariableSymbol(Name, IsReadOnly, Type, Constant);
}

BoundIfStatement* Binder::BindIfStatement(const IfStatementSyntax* IfStatement) noexcept
{
    auto Condition = BindExpression(IfStatement->Condition);
    if (Condition->ConstantValue().IsValid() && Condition->ConstantValue().HoldsAlternative<ConstantType::Boolean>())
    {
        if (!Condition->ConstantValue().Get<ConstantType::Boolean>())
        {
            Diagnostics.ReportUnreachableCode(IfStatement->ElseClause->ElseStatement);
        }
        else if (IfStatement->ElseClause)
        {
            // The if condition always yields true and the else statement is unreachble.
            Diagnostics.ReportUnreachableCode(IfStatement->ElseClause->ElseStatement);
        }
    }

    if (Condition->Type() != &TypeSymbol::Bool)
    {
        Diagnostics.ReportTypeMismatch(IfStatement->Condition->Location(), TypeSymbol::Bool, *Condition->Type());
    }

    auto ThenStatement = BindStatement(IfStatement->ThenStatement);
    auto ElseStatement = IfStatement->ElseClause ? BindStatement(IfStatement->ElseClause->ElseStatement) : nullptr;
    return new BoundIfStatement(IfStatement, Condition, ThenStatement, ElseStatement);
}

BoundWhileStatement* Binder::BindWhileStatement(const WhileStatementSyntax* WhileStatement) noexcept
{
    auto WhileScope = EnterScope();

    auto Condition = BindExpression(WhileStatement->Condition);

    if (Condition->ConstantValue().IsValid() && !Condition->ConstantValue().Get<bool>())
    {
        Diagnostics.ReportUnreachableCode(WhileStatement->Body);
    }

    auto Body = BindStatement(WhileStatement->Body);
    return new BoundWhileStatement(WhileStatement, Condition, Body);
}

BoundDoWhileStatement* Binder::BindDoWhileStatement(const DoWhileStatementSyntax* DoWhileStatement) noexcept
{
    auto Body = BindStatement(DoWhileStatement->Body);
    auto Condition = BindExpression(DoWhileStatement->Condition);
    return new BoundDoWhileStatement(DoWhileStatement, Body, Condition);
}

BoundForStatement* Binder::BindForStatement(const ForStatementSyntax* ForStatement) noexcept
{
    auto ForScope = EnterScope();

    auto InitStatement = BindStatement(ForStatement->InitStatement);
    auto Condition = BindExpression(ForStatement->Condition);
    auto Expression = BindExpression(ForStatement->Expression);
    auto Body = BindStatement(ForStatement->Body);

    return new BoundForStatement(ForStatement, InitStatement, Condition, Expression, Body);
}

BoundStatement* Binder::BindBreakStatement(const BreakStatementSyntax* BreakStatement [[maybe_unused]]) noexcept
{
    // TODO
    InternalCompilerError(std::source_location::current(), "无法绑定break语句: 尚未实现");
}

BoundStatement* Binder::BindContinueStatement(const ContinueStatementSyntax* ContinueStatement [[maybe_unused]]) noexcept
{
    // TODO
    InternalCompilerError(std::source_location::current(), "无法绑定continue语句: 尚未实现");
}

BoundReturnStatement* Binder::BindReturnStatement(const ReturnStatementSyntax* ReturnStatement) noexcept
{
    // TODO: Diagnostics
    auto Expression = BindExpression(ReturnStatement->Expression);
    return new BoundReturnStatement(ReturnStatement, Expression);
}

BoundLiteralExpression* Binder::BindLiteralExpression(const LiteralExpressionSyntax* LiteralExpression) noexcept
{
    return new BoundLiteralExpression(LiteralExpression, LiteralExpression->Value);
}

BoundExpression* Binder::BindNameExpression(const NameExpressionSyntax* NameExpression) noexcept
{
    auto Name = NameExpression->IdentifierToken->Text();
    auto Variables = Scope->LookupParameterOrVariable(Name);
    if (Variables.empty())
    {
        Diagnostics.ReportUndeclaredIdentifier(NameExpression->Location(), Name);
        return new BoundErrorExpression(NameExpression);
    }
    else if (Variables.size() > 1)
    {
        Diagnostics.ReportAmbiguousIdentifier(NameExpression->Location(), Name);
        return new BoundErrorExpression(NameExpression);
    }

    return new BoundVariableExpression(NameExpression, Variables.front());
}

BoundExpression* Binder::BindParenthesizedExpression(const ParenthesizedExpressionSyntax* ParenthesizedExpression) noexcept
{
    return BindExpression(ParenthesizedExpression->Expression);
}

BoundExpression* Binder::BindUnaryExpression(const UnaryExpressionSyntax* UnaryExpression) noexcept
{
    auto BoundOperand = BindExpression(UnaryExpression->Operand);

    // TODO: Check operand type

    auto BoundOperator = BoundUnaryOperator::Bind(
        UnaryExpression->OperatorToken->Kind(),
        BoundOperand->Type()
    );
    if (!BoundOperand)
    {
        Diagnostics.ReportUndefinedUnaryOperator(UnaryExpression->Location(), UnaryExpression->OperatorToken, *BoundOperand->Type());
        return new BoundErrorExpression(UnaryExpression);
    }

    return new BoundUnaryExpression(UnaryExpression, *BoundOperator, BoundOperand);
}

BoundExpression* Binder::BindBinaryExpression(const BinaryExpressionSyntax* BinaryExpression) noexcept
{
    auto BoundLeft = BindExpression(BinaryExpression->Left);
    auto BoundRight = BindExpression(BinaryExpression->Right);

    // TODO: Check operand types

    auto BoundOperator = BoundBinaryOperator::Bind(
        BinaryExpression->OperatorToken->Kind(),
        BoundLeft->Type(),
        BoundRight->Type()
    );
    if (!BoundOperator)
    {
        Diagnostics.ReportUndefinedBinaryOperator(BinaryExpression->Location(), *BoundLeft->Type(), BinaryExpression->OperatorToken, *BoundRight->Type());
        return new BoundErrorExpression(BinaryExpression);
    }

    return new BoundBinaryExpression(BinaryExpression, BoundLeft, *BoundOperator, BoundRight);
}

BoundExpression* Binder::BindAssignmentExpression(const AssignmentExpressionSyntax* AssignmentExpression) noexcept
{
    auto Name = AssignmentExpression->IdentifierToken->Text();
    auto BoundExpression = BindExpression(AssignmentExpression->Expression);

    auto Variables = Scope->LookupVariable(Name);
    if (Variables.empty())
    {
        Diagnostics.ReportUndeclaredIdentifier(AssignmentExpression->IdentifierToken->Location(), AssignmentExpression->IdentifierToken->Text());
        return new BoundErrorExpression(AssignmentExpression);
    }
    else if (Variables.size() > 1)
    {
        Diagnostics.ReportAmbiguousIdentifier(AssignmentExpression->IdentifierToken->Location(), AssignmentExpression->IdentifierToken->Text());
        return new BoundErrorExpression(AssignmentExpression);
    }

    auto Variable = Variables.front();
    if (Variable->IsConstant)
    {
        Diagnostics.ReportVariableImmutable(AssignmentExpression->Location(), AssignmentExpression->IdentifierToken->Text());
        return new BoundErrorExpression(AssignmentExpression);
    }

    if (AssignmentExpression->AssignmentToken->Kind() != SyntaxKind::EqualsToken)
    {
        auto EquivalentOperatorTokenKind = SyntaxFacts::GetBinaryOperatorOfAssignmentOperator(AssignmentExpression->AssignmentToken->Kind());
        auto BoundOperator = BoundBinaryOperator::Bind(EquivalentOperatorTokenKind, Variable->Type, BoundExpression->Type());

        if (!BoundOperator)
        {
            Diagnostics.ReportUndefinedBinaryOperator(AssignmentExpression->AssignmentToken->Location(), *Variable->Type, AssignmentExpression->AssignmentToken, *BoundExpression->Type());
            return new BoundErrorExpression(AssignmentExpression);
        }

        // TODO: implement conversion
        return new BoundCompoundAssignmentExpression(AssignmentExpression, Variable, *BoundOperator, BoundExpression);
    }

    return new BoundAssignmentExpression(AssignmentExpression, Variable, BoundExpression);
}

BoundExpression* Binder::BindCallExpression(const CallExpressionSyntax* CallExpression) noexcept
{
    auto BoundArguments = std::vector<const BoundExpression*>();
    BoundArguments.reserve(CallExpression->Arguments.Count());

    for (auto&& Argument : CallExpression->Arguments.Nodes())
    {
        // Argument is guaranteed to be a expression by the parser.
        auto BoundArgument = BindExpression(static_cast<const ExpressionSyntax*>(Argument));
        BoundArguments.emplace_back(BoundArgument);
    }

    // TODO: Refinement Jvav standard
    // [basic.lookup]/? : If the declarations found by name lookup all denote functions,
    //                    the declarations are said to form an overload set.
    auto FunctionSet = Scope->LookupFunction(CallExpression->Identifier->Text());
    if (FunctionSet.empty())
    {
        Diagnostics.ReportUndeclaredIdentifier(CallExpression->Identifier->Location(), CallExpression->Identifier->Text());
        return new BoundErrorExpression(CallExpression);
    }

    // TODO: Overload resolution. Before overload resolution is avaiable, function override is not permitted.
    auto Function = FunctionSet.front();

    if (CallExpression->Arguments.Count() != Function->Parameters.size())
    {
        Diagnostics.ReportArgumentCountMismatch(CallExpression->Location(), Function->Parameters.size(), CallExpression->Arguments.Count());
        return new BoundErrorExpression(CallExpression);
    }

    // TODO: implement conversion

    return new BoundCallExpression(CallExpression, Function, std::move(BoundArguments));
}

NullablePointer<const TypeSymbol> Binder::BindTypeClause(NullablePointer<const TypeClauseSyntax> TypeClause) noexcept
{
    if (!TypeClause)
    {
        return {};
    }

    auto Types = Scope->LookupType(TypeClause->Identifier->Text());
    if (Types.empty())
    {
        Diagnostics.ReportUndeclaredIdentifier(TypeClause->Identifier->Location(), TypeClause->Identifier->Text());
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

void Binder::DeclareBuiltinTypes() noexcept
{
    Scope->Declare(&TypeSymbol::Int);
    Scope->Declare(&TypeSymbol::Bool);
    Scope->Declare(&TypeSymbol::String);
    Scope->Declare(&TypeSymbol::Void);
    Scope->Declare(&TypeSymbol::Double);
}

Binder::Binder(const class SyntaxTree* SyntaxTree) noexcept :
    Scope(new BoundScope(nullptr)), SyntaxTree(SyntaxTree)
{
    DeclareBuiltinTypes();
}
