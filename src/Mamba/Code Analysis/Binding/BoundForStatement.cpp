#include "BoundForStatement.h"

#include "BoundExpression.h"

using namespace Mamba;

BoundForStatement::BoundForStatement(
    const SyntaxNode* Syntax,
    const BoundStatement* InitStatement,
    const BoundExpression* Condition,
    const BoundExpression* Expression,
    const BoundStatement* Body
) noexcept :
    Super(Syntax),
    InitStatement(InitStatement),
    Condition(Condition),
    Expression(Expression),
    Body(Body)
{
}

BoundForStatement::~BoundForStatement() noexcept
{
    delete InitStatement;
    delete Condition;
    delete Expression;
    delete Body;
}

BoundNodeKind BoundForStatement::Kind() const noexcept
{
    return BoundNodeKind::ForStatement;
}
