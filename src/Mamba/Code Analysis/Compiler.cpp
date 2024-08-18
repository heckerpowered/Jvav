#include "MambaCore.h"

#include "Colors.h"
#include "Compiler.h"

using namespace fast_io::io;
using namespace Mamba;

void Compiler::PrivateAddSourceFile(const std::string_view FileName) noexcept
{
    try
    {
        const auto FileLoader = fast_io::native_file_loader(FileName);

        const auto Info = SourceTextInfo{
            .FileName = Concat(fast_io::mnp::code_cvt(FileName)),
            .Text = String(FileLoader.begin(), FileLoader.end())
        };
        SourceTexts.emplace_back(Info);
    }
    catch (fast_io::error error)
    {
        perrln(
            Color("error: ", Colors::BrightForegroundRed),
            Color("error reading '", Colors::BrightForegroundWhite),
            Color(FileName, Colors::BrightForegroundWhite),
            Color("': ", Colors::BrightForegroundWhite),
            Color(error, Colors::BrightForegroundWhite)
        );
    }
}

void Compiler::ReportNoInputFiles() noexcept
{
    perrln(
        "mamba: ",
        Color("error: ", Colors::BrightForegroundRed),
        Color("no input files", Colors::BrightForegroundWhite)
    );
}
