#pragma once

#include <memory>
#include <span>
#include <vector>

namespace Mamba
{
    class BoundProgram
    {
    public:
        [[nodiscard]] BoundProgram(const std::span<std::shared_ptr<const class BoundCompilationUnit>> CompilationUnits;
        ) noexcept;
        [[nodiscard]] BoundProgram(std::vector<std::shared_ptr<const class BoundCompilationUnit>>&& CompilationUnits;
        ) noexcept;

        std::vector<std::shared_ptr<const class BoundCompilationUnit>> CompilationUnits;
    };
} // namespace Mamba