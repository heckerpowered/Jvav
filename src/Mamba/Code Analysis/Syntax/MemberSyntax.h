#pragma once

#include "SyntaxNode.h"

#include <memory>

namespace Mamba
{
    class MemberSyntax : public SyntaxNode
    {
    public:
        using Super = SyntaxNode;

    protected:
        MemberSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree) noexcept;
    };
} // namespace Mamba