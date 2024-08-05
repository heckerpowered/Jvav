#include "BoundWhileStatement.h"

#include "BoundExpression.h"

using namespace Mamba;

BoundWhileStatement::BoundWhileStatement(
    const std::shared_ptr<const SyntaxNode> SyntaxNode,
    const std::shared_ptr<const class BoundExpression> Condition,
    const std::shared_ptr<const class BoundStatement> Body
) noexcept :
    Super(SyntaxNode), Condition(Condition), Body(Body)
{
}

BoundNodeKind BoundWhileStatement::Kind() const noexcept
{
    return BoundNodeKind::WhileStatement;
}
