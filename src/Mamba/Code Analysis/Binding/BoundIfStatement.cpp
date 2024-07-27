#include "BoundIfStatement.h"

using namespace Mamba;

BoundIfStatement::BoundIfStatement(
    const std::shared_ptr<const SyntaxNode> Syntax,
    const std::shared_ptr<const BoundExpression> Condition,
    const std::shared_ptr<const BoundStatement> ThenStatement,
    const NullableSharedPtr<const BoundStatement> ElseStatement
) noexcept :
    Super(Syntax), Condition(Condition), ThenStatement(ThenStatement), ElseStatement(ElseStatement)
{
}

BoundNodeKind BoundIfStatement::Kind() const noexcept
{
    return BoundNodeKind::IfStatement;
}