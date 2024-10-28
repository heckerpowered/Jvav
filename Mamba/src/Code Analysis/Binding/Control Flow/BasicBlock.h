#pragma once

#include <vector>

#include "BoundStatement.h"

namespace Mamba
{

    struct BasicBlock
    {
        bool IsEntry : 1;
        bool IsExit : 1;

        std::span<const BoundStatement* const> Statements;
        std::vector<const BasicBlock*> Predecessors;
        std::vector<const BasicBlock*> Successors;
    };

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