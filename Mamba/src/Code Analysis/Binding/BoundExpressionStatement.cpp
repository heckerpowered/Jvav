#include "BoundExpressionStatement.h"

using namespace Mamba;

BoundExpressionStatement::BoundExpressionStatement(
    const SyntaxNode* Syntax,
    const BoundExpression* Expression
) noexcept :
    Super(Syntax), Expression(Expression)
{
}

BoundExpressionStatement::~BoundExpressionStatement() noexcept
{
    delete Expression;
}

BoundNodeKind BoundExpressionStatement::Kind() const noexcept
{
    return BoundNodeKind::ExpressionStatement;
}