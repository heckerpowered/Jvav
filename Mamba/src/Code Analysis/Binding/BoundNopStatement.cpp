#include "BoundNopStatement.h"

using namespace Mamba;

BoundNopStatement::BoundNopStatement(const SyntaxNode* Syntax) noexcept :
    Super(Syntax) {}

BoundNodeKind BoundNopStatement::Kind() const noexcept
{
    return BoundNodeKind::NopStatement;
}
