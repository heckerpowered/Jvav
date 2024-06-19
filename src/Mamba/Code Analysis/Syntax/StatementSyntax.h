#pragma once

#include "SyntaxNode.h"

#include <memory>

namespace Mamba
{
    class StatementSyntax : public SyntaxNode
    {
    public:
        using Super = SyntaxNode;

    protected:
        [[nodiscard]] StatementSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree) noexcept;
    };
} // namespace Mamba