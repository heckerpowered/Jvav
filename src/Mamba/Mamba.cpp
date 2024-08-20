#include "Mamba.h"
#include "Colors.h"
#include "Compilation.h"
#include "Diagnostic.h"
#include "SourceText.h"

#include <exception>
#include <fast_io.h>
#include <string_view>
#include <vector>

using namespace Mamba;
using namespace fast_io::io;

void PrintError(const Diagnostic& Diagnostic) noexcept
{
    println(
        Color(fast_io::mnp::code_cvt(*Diagnostic.Location.FileName()), Colors::BrightForegroundWhite),
        Color(":", Colors::BrightForegroundWhite),
        Color(Diagnostic.Location.StartLine(), Colors::BrightForegroundWhite),
        Color(":", Colors::BrightForegroundWhite),
        Color(Diagnostic.Location.StartCharacter(), Colors::BrightForegroundWhite),
        " ",
        Color("error: ", Colors::BrightForegroundRed),
        Color(fast_io::mnp::code_cvt(Diagnostic.Message), Colors::BrightForegroundWhite)
    );
}

void PrintWarning(const Diagnostic& Diagnostic) noexcept
{
    println(
        Color(fast_io::mnp::code_cvt(*Diagnostic.Location.FileName()), Colors::BrightForegroundWhite),
        Color(":", Colors::BrightForegroundWhite),
        Color(Diagnostic.Location.StartLine(), Colors::BrightForegroundWhite),
        Color(":", Colors::BrightForegroundWhite),
        Color(Diagnostic.Location.StartCharacter(), Colors::BrightForegroundWhite),
        " ",
        Color("warning: ", Colors::BrightForegroundYellow),
        Color(fast_io::mnp::code_cvt(Diagnostic.Message), Colors::BrightForegroundWhite)
    );
}

void PrintInformation(const Diagnostic& Diagnostic) noexcept
{
    println(
        Color(fast_io::mnp::code_cvt(*Diagnostic.Location.FileName()), Colors::BrightForegroundWhite),
        Color(":", Colors::BrightForegroundWhite),
        Color(Diagnostic.Location.StartLine(), Colors::BrightForegroundWhite),
        Color(":", Colors::BrightForegroundWhite),
        Color(Diagnostic.Location.StartCharacter(), Colors::BrightForegroundWhite),
        " ",
        Color("note: ", Colors::BrightForegroundCyan),
        Color(fast_io::mnp::code_cvt(Diagnostic.Message), Colors::BrightForegroundWhite)
    );
}

void PrintDiagnostic(const Diagnostic& Diagnostic) noexcept
{
    switch (Diagnostic.Severity)
    {
        case DiagnosticSeverity::Error:
            PrintError(Diagnostic);
            break;
        case DiagnosticSeverity::Warning:
            PrintWarning(Diagnostic);
            break;
        case DiagnosticSeverity::Information:
            PrintInformation(Diagnostic);
            break;
        default:
            std::unreachable();
    }
}

void PrintDiagnostics(const std::span<const Diagnostic> Diagnostics) noexcept
{
    if (Diagnostics.empty())
    {
        println(Color("Compilation successful", Colors::BrightForegroundGreen));
        return;
    }

    for (auto&& Diagnostic : Diagnostics)
    {
        PrintDiagnostic(Diagnostic);
    }
}

std::shared_ptr<const SourceText> ReadFile(std::string_view FileName) noexcept
try
{
    auto FileLoader = fast_io::native_file_loader(FileName);

    const auto Text = std::make_shared<String>(FileLoader.begin(), FileLoader.end());
    auto SharedFileName = std::make_shared<const String>(FileName.begin(), FileName.end());

    return std::make_shared<const SourceText>(SourceText::From(Text, SharedFileName));
}
catch (fast_io::error error)
{
    // error: error reading '<filename>': <error message>
    perrln(
        Color("error: ", Colors::BrightForegroundRed),
        Color("error reading '", Colors::BrightForegroundWhite),
        Color(FileName, Colors::BrightForegroundWhite),
        Color("': ", Colors::BrightForegroundWhite),
        error
    );
    std::exit(-1);
}

void ProcessInputFiles(const std::span<const std::string_view> InputFiles) noexcept
{
    auto Sources = std::vector<std::shared_ptr<const SourceText>>();
    Sources.reserve(InputFiles.size());

    for (auto InputFile : InputFiles)
    {
        Sources.emplace_back(ReadFile(InputFile));
    }

    auto Compilation = ::Mamba::Compilation(Sources);
    Compilation.Compile(TEXT("a"));
    PrintDiagnostics(Compilation.Diagnostics());
}

std::int32_t Mamba::InitMamba(int ArgumentCount, [[maybe_unused]] char* Arguments[]) noexcept
{
    if (ArgumentCount <= 1)
    {
        // mamba: error: no input files
        println("mamba: ", Color("error: ", Colors::BrightForegroundRed), Color("no input files", Colors::BrightForegroundWhite));
        return -1;
    }

    const auto Begin = Arguments + 1;
    const auto End = Arguments + ArgumentCount;
    auto InputFiles = std::vector<std::string_view>(Begin, End);
    ProcessInputFiles(InputFiles);

    return 0;
}
