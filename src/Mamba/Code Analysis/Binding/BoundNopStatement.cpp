#include "BoundNopStatement.h"

using namespace Mamba;

BoundNopStatement::BoundNopStatement(const std::shared_ptr<const class SyntaxNode> Syntax) noexcept : Super(Syntax) {}

BoundNodeKind BoundNopStatement::Kind() const noexcept
{
    return BoundNodeKind::NopStatement;
}
