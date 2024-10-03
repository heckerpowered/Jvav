#include "BoundProgram.h"

using namespace Mamba;

BoundProgram::BoundProgram(const std::span<const std::shared_ptr<const BoundCompilationUnit>> CompilationUnits) noexcept :
#if __cpp_lib_containers_ranges == 202202L
    CompilationUnits(std::from_range, CompilationUnits)
#else
    CompilationUnits(CompilationUnits.begin(), CompilationUnits.end())
#endif
{
}

BoundProgram::BoundProgram(std::vector<std::shared_ptr<const BoundCompilationUnit>>&& CompilationUnits) noexcept :
    CompilationUnits(std::move(CompilationUnits))
{
}
