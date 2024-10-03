#include "BoundReturnStatement.h"

using namespace Mamba;

BoundReturnStatement::BoundReturnStatement(
    const SyntaxNode* Syntax,
    const BoundExpression* Expression
) noexcept :
    Super(Syntax), Expression(Expression)
{
}

BoundNodeKind BoundReturnStatement::Kind() const noexcept
{
    return BoundNodeKind::ReturnStatement;
}

BoundReturnStatement::~BoundReturnStatement() noexcept
{
    delete Expression;
}
