#include "BoundReturnStatement.h"

using namespace Mamba;

BoundReturnStatement::BoundReturnStatement(
    const std::shared_ptr<const class SyntaxNode> Syntax,
    const std::shared_ptr<const class BoundExpression> Expression
) noexcept :
    Super(Syntax), Expression(Expression)
{
}

BoundNodeKind BoundReturnStatement::Kind() const noexcept
{
    return BoundNodeKind::ReturnStatement;
}
