#include "ForStatementSyntax.h"

#include "ExpressionSyntax.h"
#include "StatementSyntax.h"

using namespace Mamba;

ForStatementSyntax::ForStatementSyntax(
    const class SyntaxTree* SyntaxTree,
    const SyntaxToken* Keyword,
    const SyntaxToken* OpenParenthesisToken,
    const StatementSyntax* InitStatement,
    const SyntaxToken* InitStatementColonToken,
    const ExpressionSyntax* Condition,
    const SyntaxToken* ConditionColonToken,
    const ExpressionSyntax* Expression,
    const SyntaxToken* CloseParenthesisToken,
    const StatementSyntax* Body
) noexcept :
    Super(SyntaxTree),
    Keyword(Keyword),
    OpenParenthesisToken(OpenParenthesisToken),
    InitStatement(InitStatement),
    InitStatementColonToken(InitStatementColonToken),
    Condition(Condition),
    ConditionColonToken(ConditionColonToken),
    Expression(Expression),
    CloseParenthesisToken(CloseParenthesisToken),
    Body(Body)
{
}

ForStatementSyntax::~ForStatementSyntax() noexcept
{
    delete InitStatement;
    delete Condition;
    delete Expression;
    delete Body;
}

SyntaxKind ForStatementSyntax::Kind() const noexcept
{
    return SyntaxKind::ForStatement;
}

std::vector<const SyntaxNode*> ForStatementSyntax::Children() const noexcept
{
    return { Keyword, OpenParenthesisToken, InitStatement, InitStatementColonToken, Condition, ConditionColonToken, Expression, CloseParenthesisToken, Body };
}