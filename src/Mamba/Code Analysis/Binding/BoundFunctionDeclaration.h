#pragma once

#include <memory>

#include "BoundStatement.h"

namespace Mamba
{
    class BoundFunctionDeclaration : public BoundStatement
    {
    public:
        using Super = BoundStatement;

        [[nodiscard]] BoundFunctionDeclaration(
            const std::shared_ptr<const class SyntaxNode> Syntax,
            const std::shared_ptr<const class BoundStatement> Body,
            const std::shared_ptr<const class BoundScope> Scope
        ) noexcept;

        virtual BoundNodeKind Kind() const noexcept override;

        std::shared_ptr<const class BoundStatement> Body;
        std::shared_ptr<const class BoundScope> Scope;
    };
}; // namespace Mamba