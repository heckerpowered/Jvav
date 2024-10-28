#pragma once

#include <algorithm>

#include "BasicBlock.h"
#include "BoundBlockStatement.h"
#include "BoundStatement.h"

namespace Mamba
{
    struct ControlFlowGraph
    {
    };

    // Returns a view of statements containing all statements between the current statement and
    // the next jump statement (if any, including jump statements).
    std::span<const BoundStatement* const> Analyze(std::span<const BoundStatement* const> Statements) noexcept
    {
        auto Begin = Statements.begin();
        auto End = std::ranges::find_if(Statements, [](const BoundStatement* Statement) { return Statement->Kind() == BoundNodeKind::IfStatement; });
        if (End == Statements.end())
        {
            return { Begin, End };
        }

        return { Begin, std::next(End) };
    }

    ControlFlowGraph Analyze(const BoundBlockStatement* BlockStatement) noexcept
    {
        auto Manager = BasicBlockManager();
        auto Current = BlockStatement->Statements.begin();

        while (true)
        {
            auto Statements = Analyze({ Current, BlockStatement->Statements.end() });
            if (Statements.empty())
            {
                return {};
            }

            if (Statements.back()->Kind() == BoundNodeKind::IfStatement)
            {
            }
            else
            {
            }
        }
    }
} // namespace Mamba