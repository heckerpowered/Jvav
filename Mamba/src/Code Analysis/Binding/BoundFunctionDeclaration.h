#pragma once

#include "BoundStatement.h"

namespace Mamba
{
    class BoundFunctionDeclaration : public BoundStatement
    {
    public:
        using Super = BoundStatement;

        [[nodiscard]] BoundFunctionDeclaration(
            const SyntaxNode* Syntax,
            const BoundStatement* Body
        ) noexcept;

        ~BoundFunctionDeclaration() noexcept override;

        virtual BoundNodeKind Kind() const noexcept override;

        const BoundStatement* Body;
    };
}; // namespace Mamba