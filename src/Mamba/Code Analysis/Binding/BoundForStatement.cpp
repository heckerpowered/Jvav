#include "BoundForStatement.h"

#include "BoundExpression.h"
#include "BoundExpressionStatement.h"

using namespace Mamba;

BoundForStatement::BoundForStatement(
    const std::shared_ptr<const class SyntaxNode> Syntax,
    const std::shared_ptr<const class BoundStatement> InitStatement,
    const std::shared_ptr<const class BoundExpression> Condition,
    const std::shared_ptr<const class BoundExpression> Expression,
    const std::shared_ptr<const class BoundStatement> Body
) noexcept :
    Super(Syntax),
    InitStatement(InitStatement),
    Condition(Condition),
    Expression(Expression),
    Body(Body)
{
}

BoundNodeKind Mamba::BoundForStatement::Kind() const noexcept
{
    return BoundNodeKind::ForStatement;
}
