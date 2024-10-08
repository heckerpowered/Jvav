#include <ranges>
#include <string_view>

#include "Compiler.h"
#include "Mamba.h"
#include "MambaOptions.h"
#include "MambaSignalHandler.h"

using namespace std::string_view_literals;
using namespace Mamba;

std::int32_t Mamba::InitMamba(int ArgumentCount, [[maybe_unused]] char* Arguments[]) noexcept
{
    InitSignalHandler();

    auto Begin = Arguments + 1;
    auto End = Arguments + ArgumentCount;
    auto ArgumentRange = std::ranges::subrange(Begin, End) | std::views::transform([](auto&& Range) { return std::string_view(Range); }) | std::ranges::to<std::vector>();

    InitMambaOptions(ArgumentRange);

    if (Options::SkipCompile)
    {
        return 0;
    }

    auto Compiler = ::Mamba::Compiler();
    for (auto&& Argument : Options::SourceFiles)
    {
        Compiler.AddSourceFile(Argument);
    }

    Compiler.Compile();

    return 0;
}
