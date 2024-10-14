#pragma once

#include "BasicBlock.h"

namespace Mamba
{
    struct ControlFlowGraph
    {
        BasicBlock Entry;
        BasicBlock Exit;
    };
} // namespace Mamba