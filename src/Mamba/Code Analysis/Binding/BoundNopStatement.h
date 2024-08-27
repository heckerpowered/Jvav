#pragma once

#include "BoundNodeKind.h"
#include "BoundStatement.h"

#include <memory>

namespace Mamba
{
    class BoundNopStatement : public BoundStatement
    {
    public:
        using Super = BoundStatement;

        [[nodiscard]] BoundNopStatement(const SyntaxNode* Syntax) noexcept;

        BoundNodeKind Kind() const noexcept override;
    };
} // namespace Mamba