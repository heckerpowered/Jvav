#include "BoundWhileStatement.h"

#include "BoundExpression.h"

using namespace Mamba;

BoundWhileStatement::BoundWhileStatement(
    const SyntaxNode* SyntaxNode,
    const BoundExpression* Condition,
    const BoundStatement* Body
) noexcept :
    Super(SyntaxNode), Condition(Condition), Body(Body)
{
}

BoundWhileStatement::~BoundWhileStatement() noexcept
{
    delete Condition;
    delete Body;
}

BoundNodeKind BoundWhileStatement::Kind() const noexcept
{
    return BoundNodeKind::WhileStatement;
}
