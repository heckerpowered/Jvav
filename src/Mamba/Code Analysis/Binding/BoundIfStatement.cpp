#include "BoundIfStatement.h"

using namespace Mamba;

BoundIfStatement::BoundIfStatement(const SyntaxNode* Syntax, const BoundExpression* Condition, const BoundStatement* ThenStatement, NullablePointer<const BoundStatement> ElseStatement) noexcept :
    Super(Syntax), Condition(Condition), ThenStatement(ThenStatement), ElseStatement(ElseStatement)
{
}

BoundIfStatement::~BoundIfStatement() noexcept
{
    delete Condition;
    delete ThenStatement;
    delete ElseStatement;
}

BoundNodeKind BoundIfStatement::Kind() const noexcept
{
    return BoundNodeKind::IfStatement;
}