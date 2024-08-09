#include "BoundExpressionStatement.h"

using namespace Mamba;

BoundExpressionStatement::BoundExpressionStatement(
    const std::shared_ptr<const class SyntaxNode> Syntax,
    const std::shared_ptr<const class BoundExpression> Expression
) noexcept :
    Super(Syntax), Expression(Expression)
{
}

BoundNodeKind BoundExpressionStatement::Kind() const noexcept
{
    return BoundNodeKind::ExpressionStatement;
}