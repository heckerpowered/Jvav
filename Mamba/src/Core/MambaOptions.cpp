#include <filesystem>
#include <string_view>
#include <unordered_map>

#include "LLVMBackend.h"
#include "Mamba.h"
#include "MambaOptions.h"
#include "Scanner.h"

using namespace Mamba;
using namespace std::string_view_literals;

namespace Mamba::Options
{
    bool SkipCompile;
    std::vector<std::string_view> SourceFiles;
    bool EmitLLVM;
} // namespace Mamba::Options

struct Command
{
    std::string_view Description;
    PackagedScanner Handler;
};

std::unordered_map<std::string_view, Command> Commands;

template<typename T>
void RegisterCommand(std::string_view Name, std::string_view ShortName, T&& Handler, std::string_view Description) noexcept
{
    Commands.emplace(Name, Command{ Description, PackagedScanner{ std::forward<T>(Handler) } });

    if (ShortName.empty())
    {
        return;
    }
    Commands.emplace(ShortName, Command{ Description, PackagedScanner{ std::forward<T>(Handler) } });
}

void Mamba::InitMambaOptions(std::span<std::string_view> Arguments) noexcept
{
    RegisterCommand(
        "--version"sv,
        "-v"sv,
        Scanner([] {
            fast_io::io::println(
                MambaVersion,
                "\n目标: ",
                LLVMBackend::TargetTriple(),
                "\n",
                "线程模型: ",
                LLVMBackend::ThreadModel() == llvm::ThreadModel::POSIX ? "POSIX"sv : "单线程"sv,
                "\n",
                "安装目录: ",
                std::filesystem::current_path().string()
            );
            Options::SkipCompile = true;
        }),
        "查看版本信息"sv
    );

    RegisterCommand(
        "--emit-llvm"sv,
        ""sv,
        Scanner([] {
            Options::EmitLLVM = true;
        }),
        ""sv
    );

    for (auto&& Argument : Arguments)
    {
        if (!Argument.starts_with("-"))
        {
            Options::SourceFiles.emplace_back(Argument);
            continue;
        }

        auto Command = Commands.find(Argument);
        if (Command == Commands.end())
        {
            continue;
        }

        Command->second.Handler();
    }
}