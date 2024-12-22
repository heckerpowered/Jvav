#include <ranges>

#include "Compiler.h"
#include "fast_io_hosted/file_loaders/impl.h"
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
    auto ArgumentRange = std::ranges::subrange(Begin, End) | std::views::transform([](auto&& Range) { return fast_io::string_view(fast_io::mnp::os_c_str(Range)); }) | std::ranges::to<fast_io::vector>();

    InitMambaOptions(ArgumentRange);

    if (Options::SkipCompile && Options::SourceFiles.empty())
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
