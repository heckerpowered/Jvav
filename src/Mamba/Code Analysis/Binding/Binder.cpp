#include "Binder.h"
#include "BoundCallExpression.h"
#include "BoundCompoundAssignmentExpression.h"
#include "BoundExpressionStatement.h"
#include "MambaCore.h"
#include "SyntaxFacts.h"
#include <source_location>

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

    auto Body = BindStatement(FunctionDeclaration->Body);
    auto BoundFunctionDeclaration = new class BoundFunctionDeclaration(FunctionDeclaration, Body);

    FunctionScope.PreLeave();

    DeclareFunction(FunctionDeclaration, BoundFunctionDeclaration);
}

void Binder::DeclareFunction(
    const FunctionDeclarationSyntax* FunctionDeclaration,
    const BoundFunctionDeclaration* BoundFunctionDeclaration
) noexcept
{
    auto Name = FunctionDeclaration->Identifier->Text();
    auto Parameters = BindParameter(FunctionDeclaration);
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
            BoundExpressionStatement->Expression->Kind() == BoundNodeKind::CompoundAssignmentExpression;
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
#ifdef DEBUG
            fast_io::io::perrln("Unexpected syntax: {}", fast_io::mnp::code_cvt(SyntaxFacts::ToString(Statement->Kind())));
            fast_io::fast_terminate();
#else
            std::unreachable();
#endif
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
#ifdef DEBUG
        fast_io::io::perrln("Failed to bind variable declaration: empty identifier.");
        fast_io::fast_terminate();
#else
        std::unreachable();
#endif
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
    if (Condition->ConstantValue().IsValid())
    {
        if (!Condition->ConstantValue().Get<bool>())
        {
            Diagnostics.ReportUnreachableCode(IfStatement->ElseClause->ElseStatement);
        }
        else if (IfStatement->ElseClause)
        {
            // The if condition always yields true and the else statement is unreachble.
            Diagnostics.ReportUnreachableCode(IfStatement->ElseClause->ElseStatement);
        }
    }

    auto ThenStatement = BindStatement(IfStatement->ThenStatement);
    auto ElseStatement = IfStatement->ElseClause ? nullptr : BindStatement(IfStatement->ElseClause->ElseStatement);
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

BoundStatement* Binder::BindBreakStatement(const BreakStatementSyntax* BreakStatement) noexcept
{
    fast_io::io::perrln("Failed to bind break statement: not implemented yet.");
    return {};
}

BoundStatement* Binder::BindContinueStatement(const ContinueStatementSyntax* ContinueStatement) noexcept
{
    fast_io::io::perrln("Failed to bind continue statement: not implemented yet.");
    return {};
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

BoundVariableExpression* Binder::BindNameExpression(const NameExpressionSyntax* NameExpression) noexcept
{
    auto Name = NameExpression->IdentifierToken->Text();
    auto Variables = Scope->LookupVariable(Name);
    if (Variables.empty())
    {
        auto ErrorVariable = new VariableSymbol(TEXT("<error>"), false, &TypeSymbol::Void, {});
        Scope->Declare(ErrorVariable);

        Diagnostics.ReportUndeclaredIdentifier(NameExpression->Location(), Name);
        return new BoundVariableExpression(NameExpression, ErrorVariable);
    }
    else if (Variables.size() > 1)
    {
        Diagnostics.ReportAmbiguousIdentifier(NameExpression->Location(), Name);
        return new BoundVariableExpression(NameExpression, Variables.front());
    }

    return new BoundVariableExpression(NameExpression, Variables.front());
}

BoundExpression* Binder::BindParenthesizedExpression(const ParenthesizedExpressionSyntax* ParenthesizedExpression) noexcept
{
    return BindExpression(ParenthesizedExpression->Expression);
}

BoundUnaryExpression* Binder::BindUnaryExpression(const UnaryExpressionSyntax* UnaryExpression) noexcept
{
    auto BoundOperand = BindExpression(UnaryExpression->Operand);

    // TODO: Check operand type

    auto BoundOperator = BoundUnaryOperator::Bind(
        UnaryExpression->OperatorToken->Kind(),
        BoundOperand->Type()
    );
    if (!BoundOperand)
    {
        // TODO: Diagnostics - undefined unary operator
        return {};
    }

    return new BoundUnaryExpression(UnaryExpression, *BoundOperator, BoundOperand);
}

BoundBinaryExpression* Binder::BindBinaryExpression(const BinaryExpressionSyntax* BinaryExpression) noexcept
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
        // TODO: Diagnostics - undefined binary operator
        return {};
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
        // TODO: Diagnostics - undeclaraed identifier
        return {};
    }
    else if (Variables.size() > 1)
    {
        // TODO: Diagnostics - ambiguous identifier
        return {};
    }

    auto Variable = Variables.front();
    if (Variable->IsConstant)
    {
        // TODO: Diagnostics - constant variable cannot be assigned
    }

    if (AssignmentExpression->AssignmentToken->Kind() != SyntaxKind::EqualsToken)
    {
        auto EquivalentOperatorTokenKind = SyntaxFacts::GetBinaryOperatorOfAssignmentOperator(AssignmentExpression->AssignmentToken->Kind());
        auto BoundOperator = BoundBinaryOperator::Bind(EquivalentOperatorTokenKind, Variable->Type, BoundExpression->Type());

        if (!BoundOperator)
        {
            // TODO: Report undefined binary operator
            return {};
        }

        // TODO: implement conversion
        return new BoundCompoundAssignmentExpression(AssignmentExpression, Variable, *BoundOperator, BoundExpression);
    }

    return new BoundAssignmentExpression(AssignmentExpression, Variable, BoundExpression);
}

BoundCallExpression* Binder::BindCallExpression(const CallExpressionSyntax* CallExpression) noexcept
{
    auto BoundArguments = std::vector<const BoundExpression*>();
    BoundArguments.reserve(CallExpression->Arguments.Count());

    for (auto&& Argument : CallExpression->Arguments.Nodes())
    {
        // Argument is guaranteed to be a expression by the parser.
        auto BoundArgument = BindExpression(static_cast<const ExpressionSyntax*>(Argument));

        // TODO: Use hatcher to avoid copy
        BoundArguments.emplace_back(BoundArgument);
    }

    // TODO: Refinement Jvav standard
    // [basic.lookup]/? : If the declarations found by name lookup all denote functions,
    //                    the declarations are said to form an overload set.
    auto FunctionSet = Scope->LookupFunction(CallExpression->Identifier->Text());
    if (FunctionSet.empty())
    {
        // TODO: Diagnostics - undeclaraed function
        fast_io::io::perrln("Undeclaraed function '", fast_io::mnp::code_cvt(CallExpression->Identifier->Text()), "'");
        return {};
    }

    // TODO: Overload resolution. Before overload resolution is avaiable, function override is not permitted.
    auto Function = FunctionSet.front();

    if (CallExpression->Arguments.Count() != Function->Parameters.size())
    {
        // TODO: Report argument count mismatch
        fast_io::io::perrln("Failed to bind call expression: argument count mismatch.");
        return {};
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
    Scope(new BoundScope(nullptr)), SyntaxTree(SyntaxTree)
{
}
