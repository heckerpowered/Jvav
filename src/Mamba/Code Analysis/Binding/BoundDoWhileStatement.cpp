#include "BoundDoWhileStatement.h"

#include "BoundExpression.h"

using namespace Mamba;

BoundDoWhileStatement::BoundDoWhileStatement(
    const std::shared_ptr<const class SyntaxNode> SyntaxNode,
    const std::shared_ptr<const class BoundStatement> Body,
    const std::shared_ptr<const class BoundExpression> Condition
) noexcept :
    Super(SyntaxNode), Body(Body), Condition(Condition)
{
}

BoundNodeKind Mamba::BoundDoWhileStatement::Kind() const noexcept
{
    return BoundNodeKind::DoWhileStatement;
}
