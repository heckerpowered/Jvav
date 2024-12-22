#pragma once

#include <vector>

#include "BoundStatement.h"

namespace Mamba
{
    struct BasicBlock
    {
        bool IsEntry : 1;

        // Indicates that the basic block is the end of a function, if the basic block
        // is exiting and the last statement is not a return statement, not all control
        // paths return values.
        bool IsExit : 1;

        // If Successors is not empty, the last statement should be a jump statement.
        // If it is not, the basic block may automatically fall through to the next block.
        // Additionally, if FallThrough is absent and the last statement is not a jump,
        // not all control paths return values.
        std::span<const BoundStatement* const> Statements;
        std::vector<const BasicBlock*> Predecessors;

        // Successors are the basic blocks that can be reached from this basic block,
        // such as if statement contains 2 successors.
        std::vector<const BasicBlock*> Successors;
        NullablePointer<const BasicBlock> FallThrough;
    };

    // The lifetime of BasicBlock is too complex to manage, binding all lifetime of BasicBlock
    // to a manager is easy to manage and safe.
    class BasicBlockManager
    {
        std::vector<BasicBlock> BasicBlocks;

    public:
        template<typename... T>
        BasicBlock& Construct(T&&... Arguments) noexcept
        {
            return BasicBlocks.emplace_back(std::forward<T>(Arguments)...);
        }
    };
} // namespace Mamba