#include "BoundDoWhileStatement.h"

#include "BoundExpression.h"

using namespace Mamba;

BoundDoWhileStatement::BoundDoWhileStatement(
    const SyntaxNode* SyntaxNode,
    const BoundStatement* Body,
    const BoundExpression* Condition
) noexcept :
    Super(SyntaxNode), Body(Body), Condition(Condition)
{
}

BoundDoWhileStatement::~BoundDoWhileStatement() noexcept
{
    delete Body;
    delete Condition;
}

BoundNodeKind BoundDoWhileStatement::Kind() const noexcept
{
    return BoundNodeKind::DoWhileStatement;
}
