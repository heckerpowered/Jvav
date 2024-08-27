#include "Mamba.h"
#include "Compiler.h"

using namespace Mamba;

std::int32_t Mamba::InitMamba(int ArgumentCount, [[maybe_unused]] char* Arguments[]) noexcept
{
    auto Begin = Arguments + 1;
    auto End = Arguments + ArgumentCount;
    auto ArgumentRange = std::ranges::subrange(Begin, End);

    auto Compiler = ::Mamba::Compiler();
    for (auto&& Argument : ArgumentRange)
    {
        Compiler.AddSourceFile(std::string_view(Argument));
    }

    Compiler.Compile();

    return 0;
}
