#include "CompilationUnitSyntax.h"
#include "fast_io_core_impl/codecvt/general.h"
#include "fast_io_core_impl/error.h"
#include "fast_io_unit/string_impl/concat.h"
#include "MambaCore.h"
#include "SyntaxTree.h"
#include <fast_io.h>
#include <memory>

int main(int argc, char* argv[])
{
    try
    {
        for (auto i = 1; i < argc; ++i)
        {
            auto Argument = fast_io::mnp::os_c_str(argv[i]);
            fast_io::io::perrln("Compiling: ", Argument);
            const auto SyntaxTree = Mamba::SyntaxTree::Load(
                std::make_shared<const Mamba::String>(fast_io::u8concat(fast_io::mnp::code_cvt(Argument))));

            fast_io::io::println(fast_io::mnp::code_cvt(SyntaxTree.Root()->ToString()));
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
