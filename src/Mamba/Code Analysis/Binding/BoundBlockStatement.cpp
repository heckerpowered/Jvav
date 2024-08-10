#include "BoundBlockStatement.h"

using namespace Mamba;

BoundBlockStatement::BoundBlockStatement(
    const std::shared_ptr<const class SyntaxNode> Syntax,
    const std::span<const std::shared_ptr<const class BoundStatement>> Statements
) noexcept :
    Super(Syntax),
#if __cpp_lib_containers_ranges == 202202L
    Statements(std::from_range, Statements)
#else
    Statements(Statements.begin(), Statements.end())
#endif
{
}

BoundBlockStatement::BoundBlockStatement(
    const std::shared_ptr<const class SyntaxNode> Syntax,
    std::vector<std::shared_ptr<const class BoundStatement>>&& Statements
) noexcept :
    Super(Syntax), Statements(std::move(Statements))
{
}

BoundNodeKind Mamba::BoundBlockStatement::Kind() const noexcept
{
    return BoundNodeKind::BlockStatement;
}
