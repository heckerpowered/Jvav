#pragma once

#include "BasicBlock.h"
#include "BoundExpression.h"

namespace Mamba
{
    struct BasicBlockBranch : public BasicBlock
    {
        const BoundExpression* Condition;
    };
} // namespace Mamba