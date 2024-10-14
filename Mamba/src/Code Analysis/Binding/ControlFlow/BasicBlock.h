#pragma once

#include <memory>

#include "BoundStatement.h"
#include "MambaCore.h"

namespace Mamba
{
    struct BasicBlock
    {
        std::span<const BoundStatement*> Statements;

        std::unique_ptr<Nullable<BasicBlock>> Successor;
        std::unique_ptr<Nullable<BasicBlock>> Predecessor;
    };
} // namespace Mamba