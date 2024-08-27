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

std::size_t ForStatementSyntax::ChildrenCount() const noexcept
{
    return 9;
}

const SyntaxNode* ForStatementSyntax::ChildAt(std::size_t Index) const noexcept
{
    switch (Index)
    {
        case 0:
            return Keyword;
        case 1:
            return OpenParenthesisToken;
        case 2:
            return InitStatement;
        case 3:
            return InitStatementColonToken;
        case 4:
            return Condition;
        case 5:
            return ConditionColonToken;
        case 6:
            return Expression;
        case 7:
            return CloseParenthesisToken;
        case 8:
            return Body;
        default:
            ReportChildrenAccessOutOfBounds(Index);
    }
}