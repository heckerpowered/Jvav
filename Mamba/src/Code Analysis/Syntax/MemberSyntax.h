#pragma once

#include "SyntaxNode.h"

namespace Mamba
{
    class MemberSyntax : public SyntaxNode
    {
    public:
        using Super = SyntaxNode;

    protected:
        MemberSyntax(const class SyntaxTree* SyntaxTree) noexcept;
    };
} // namespace Mamba