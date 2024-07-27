#include <cmath>
#include <fast_io.h>
#include <memory>
#include <print>

#include "Diagnostic.h"
#include "MambaCore.h"
#include "SyntaxTree.h"

#include "CompilationUnitSyntax.h"

int main(int argc, char* argv[])
{
    auto vec = std::vector<char>();
#if __cpp_lib_containers_ranges == 202202L
    auto str = std::string(std::from_range, vec);
#else
    auto str = std::string(vec.begin(), vec.end());
#endif

    try
    {
        for (auto i = 1; i < argc; ++i)
        {
            auto Argument = fast_io::mnp::os_c_str(argv[i]);
            fast_io::io::println("Compiling: ", Argument);
            const auto SyntaxTree = Mamba::SyntaxTree::Load(
                std::make_shared<const Mamba::String>(fast_io::u8concat(fast_io::mnp::code_cvt(Argument)))
            );

            fast_io::io::println(fast_io::mnp::code_cvt(SyntaxTree.Root()->ToString()));

            if (!SyntaxTree.Diagnostics().empty())
            {
                for (auto&& Diagnostic : SyntaxTree.Diagnostics())
                {
                    if (Diagnostic->Severity == Mamba::DiagnosticSeverity::Error)
                    {
                        fast_io::io::println("Error: ", fast_io::mnp::code_cvt(*Diagnostic->Message));
                    }
                    else if (Diagnostic->Severity == Mamba::DiagnosticSeverity::Warning)
                    {
                        fast_io::io::println("Warning: ", fast_io::mnp::code_cvt(*Diagnostic->Message));
                    }
                    else
                    {
                        fast_io::io::println("Info: ", fast_io::mnp::code_cvt(*Diagnostic->Message));
                    }
                }
            }
        }
    }
    catch (const std::exception& exception)
    {
        fast_io::io::perrln(fast_io::mnp::os_c_str(exception.what()));
    }
    catch (const fast_io::error& exception)
    {
        fast_io::io::perrln(exception);
    }
}