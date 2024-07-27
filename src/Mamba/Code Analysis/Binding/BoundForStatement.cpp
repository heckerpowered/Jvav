#include "BoundForStatement.h"

using namespace Mamba;

BoundForStatement::BoundForStatement(
    const std::shared_ptr<const class SyntaxNode> Syntax,
    const std::shared_ptr<const class BoundExpressionStatement> InitStatement,
    const std::shared_ptr<const class BoundExpressionStatement> ConditionStatement,
    const std::shared_ptr<const class BoundExpression> Expression,
    const std::shared_ptr<const class BoundStatement> Body
) noexcept :
    Super(Syntax),
    InitStatement(InitStatement),
    ConditionStatement(ConditionStatement),
    Expression(Expression),
    Body(Body)
{
}

BoundNodeKind Mamba::BoundForStatement::Kind() const noexcept
{
    return BoundNodeKind::ForStatement;
}
