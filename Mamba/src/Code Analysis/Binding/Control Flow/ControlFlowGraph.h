#pragma once

#include <algorithm>
#include <source_location>

#include "BasicBlock.h"
#include "BoundBlockStatement.h"
#include "BoundIfStatement.h"
#include "BoundStatement.h"

#include "fast_io_core_impl/terminate.h"

namespace Mamba
{
    struct ControlFlowGraph
    {
        BasicBlockManager Manager;
        const BasicBlock* Entry;
    };

    struct AnalyzeContext
    {
        const BoundBlockStatement* BlockStatement;
        BasicBlockManager Manager;
        decltype(BlockStatement->Statements)::const_iterator Current;
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

    void AnalyzeStatement(const BoundStatement* Statement, BasicBlock& Block) noexcept
    {
        switch (Statement->Kind())
        {
            default:
                InternalCompilerError(std::source_location::current(), "无法识别的语句类型，编号: ", fast_io::mnp::enum_int_view(Statement->Kind()));
        }
    }

    void AnalyzeIfStatement(const BoundIfStatement* Statement, BasicBlock& Block, BasicBlockManager& Manager) noexcept
    {
        if (Statement->Condition->ConstantValue().HoldsAlternative<bool>())
        {
            if (Statement->Condition->ConstantValue().Get<bool>())
            {
                auto ThenBlock = Manager.Construct();
            }
            return;
        }
    }

    // Transform a statement sequence to a *Control Flow Graph*, it has a *BasicBlock* as the entry
    // which has predecessors and successors. Predecessors is basic blocks that jumps to this basic block
    // and this basic block is a successor of the basic block which jumps to this basic block. Every basic block
    // must has at least one predecessor and successor, basic blocks that has no predecessor means it will never
    // be called (dead code), no successor means it never return. Only entry has no predecessor, and only exit has
    // no successor. Every basic block can only have one terminator. Terminator is a statement that jumps to another
    // basic block.
    ControlFlowGraph Analyze(const BoundBlockStatement* BlockStatement) noexcept
    {
        auto Manager = BasicBlockManager();
        auto Current = BlockStatement->Statements.begin();

        while (true)
        {
            auto Statements = Analyze({ Current, BlockStatement->Statements.end() });
            if (Statements.empty())
            {
                return {
                    .Manager = Manager,
                    .Entry = {}
                };
            }

            decltype(auto) BasicBlock = Manager.Construct();
            BasicBlock.Statements = Statements;

            auto JumpStatement = Statements.back();
            if (JumpStatement->Kind() == BoundNodeKind::IfStatement)
            {
                auto IfStatement = static_cast<const BoundIfStatement*>(JumpStatement);

                // Merge statements means the statements to execute after the if statement,
                // maybe not presented.
                auto MergeStatements = Analyze({ Current, BlockStatement->Statements.end() });

                if (IfStatement->Condition->ConstantValue().HoldsAlternative<bool>())
                {
                    // Flatten the if statement when the condition can be evaluated at compile time.
                    if (IfStatement->Condition->ConstantValue().Get<bool>())
                    {
                        auto TrueBlock = Manager.Construct();
                        if (IfStatement->ThenStatement)
                        {
                            TrueBlock.Statements = { IfStatement->ThenStatement };
                            BasicBlock.Successors.emplace_back(&TrueBlock);
                        }
                        else
                        {
                            // The condition of the if statement yields true, but there's no
                            // true statement.
                            fast_io::fast_terminate();
                        }
                    }
                    else
                    {
                    }
                }
            }
            else
            {
            }
        }
    }
} // namespace Mamba