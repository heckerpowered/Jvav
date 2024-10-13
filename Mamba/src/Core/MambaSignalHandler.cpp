#include "MambaSignalHandler.h"
#include "MambaCore.h"
#include "MambaRandom.h"

#include <csignal>
#include <source_location>
#include <string_view>

using namespace std::string_view_literals;
using namespace Mamba;

void SignalHandler(int Signal) noexcept
{
    switch (Signal)
    {
        case SIGABRT:
            InternalCompilerError(std::source_location::current(), "程序终止");
        case SIGFPE:
            InternalCompilerError(std::source_location::current(), "浮点异常");
        case SIGILL:
            InternalCompilerError(std::source_location::current(), "非法指令");
        case SIGINT:
            fast_io::io::perrln("正在中断");
            break;
        case SIGSEGV:
            InternalCompilerError(std::source_location::current(), "谁在用"sv, RandomSelect("Intel"sv, "AMD"sv, "Apple Silicon"sv, "辊斤拷辊斤拷辊斤拷辊斤拷辊斤拷辊斤拷"sv), "? 害得我内存炸了"sv);
        case SIGTERM:
            InternalCompilerError(std::source_location::current(), "终止信号");
        default:
            InternalCompilerError(std::source_location::current(), "未知的信号: ", Signal);
    }
}

void Mamba::InitSignalHandler() noexcept
{
    // std::signal(SIGABRT, SignalHandler);
    // std::signal(SIGFPE, SignalHandler);
    // std::signal(SIGILL, SignalHandler);
    // std::signal(SIGINT, SignalHandler);
    std::signal(SIGSEGV, SignalHandler);
    // std::signal(SIGTERM, SignalHandler);
}