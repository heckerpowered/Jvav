#include <cmath>
#include <fast_io.h>
#include <memory>
#include <ranges>

#include "Compilation.h"
#include "Diagnostic.h"
#include "MambaCore.h"
#include "SourceText.h"
#include "SyntaxTree.h"

#include "CompilationUnitSyntax.h"

using namespace Mamba;

int main(int argc, char* argv[])
{
    auto Sources = std::vector<std::shared_ptr<const SourceText>>();
    for (auto i = 1; i < argc; ++i)
    {
        auto Argument = fast_io::mnp::os_c_str(argv[i]);
        fast_io::native_file_loader Loader(Argument);
        const auto Text = String(Loader.begin(), Loader.end());
        fast_io::io::println("Compiling: ", Argument);

        const auto Source = std::make_shared<const SourceText>(
            SourceText::From(
                std::make_shared<const String>(Text),
                std::make_shared<const String>(
                    Concat(fast_io::mnp::code_cvt(Argument))
                )
            )
        );
        Sources.emplace_back(Source);
    }

    auto Compilation = Mamba::Compilation(Sources);
    Compilation.Compile(TEXT("main.jink"));

    for (auto&& Diagnostic : Compilation.Diagnostics())
    {
        const auto Location = Diagnostic->Location;
        const auto FileName = Location.FileName() ? Location.FileName() : std::make_shared<const String>(TEXT("<unknown>"));
        fast_io::io::print(
            fast_io::mnp::code_cvt(*FileName),
            ":",
            Diagnostic->Location.StartLine(),
            ":",
            Diagnostic->Location.StartCharacter(),
            ": "
        );
        if (Diagnostic->Severity == Mamba::DiagnosticSeverity::Error)
        {
            fast_io::io::println("error: ", fast_io::mnp::code_cvt(*Diagnostic->Message));
        }
        else if (Diagnostic->Severity == Mamba::DiagnosticSeverity::Warning)
        {
            fast_io::io::println("warning: ", fast_io::mnp::code_cvt(*Diagnostic->Message));
        }
        else
        {
            fast_io::io::println("info: ", fast_io::mnp::code_cvt(*Diagnostic->Message));
        }
    }

    if (Compilation.Diagnostics().empty())
    {
        fast_io::io::println("Compilation successful");
    }
}