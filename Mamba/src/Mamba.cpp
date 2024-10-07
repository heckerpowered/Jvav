#include "Mamba.h"
#include "Compiler.h"
#include "MambaSignalHandler.h"

using namespace std::string_literals;
using namespace Mamba;

std::int32_t Mamba::InitMamba(int ArgumentCount, [[maybe_unused]] char* Arguments[]) noexcept
{
    InitSignalHandler();

    auto Begin = Arguments + 1;
    auto End = Arguments + ArgumentCount;
    auto ArgumentRange = std::ranges::subrange(Begin, End);

    auto Compiler = ::Mamba::Compiler();
    for (auto&& Argument : ArgumentRange)
    {
        auto ArgumentView = std::string_view(Argument);
        Compiler.AddSourceFile(ArgumentView);
    }

    Compiler.Compile();

    return 0;
}
