#include "BoundBlockStatement.h"

using namespace Mamba;

BoundBlockStatement::BoundBlockStatement(
    const SyntaxNode* Syntax,
    std::vector<const BoundStatement*>&& Statements
) noexcept :
    Super(Syntax), Statements(std::move(Statements))
{
}

BoundBlockStatement::~BoundBlockStatement() noexcept
{
    for (auto&& Statement : Statements)
    {
        delete Statement;
    }
}

BoundNodeKind BoundBlockStatement::Kind() const noexcept
{
    return BoundNodeKind::BlockStatement;
}
