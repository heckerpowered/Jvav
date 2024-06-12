#include "SyntaxTree.h"
#include "MambaCore.h"
#include "Parser.h"
#include "SourceText.h"

#include <fast_io.h>

MAMBA_NAMESPACE_BEGIN

[[nodiscard]] String ReadFile(const StringView FileName) noexcept
{
    auto NativeFileLoader = fast_io::native_file_loader(FileName);
    return String(NativeFileLoader.begin(), NativeFileLoader.end());
}

void Parse(const std::shared_ptr<const SyntaxTree> SyntaxTree,
           std::shared_ptr<const class CompilationUnitSyntax>& Root,
           std::vector<std::shared_ptr<const class Diagnostic>>& Diagnostics) noexcept
{
    auto Parser = MAMBA Parser(SyntaxTree);
    Root = Parser.ParseCompilationUnit();
    Diagnostics = std::move(Parser.Diagnostics);
}

const std::shared_ptr<const class CompilationUnitSyntax> SyntaxTree::Root() const noexcept
{
    return PrivateRoot;
}

const std::vector<std::shared_ptr<const class Diagnostic>>& SyntaxTree::Diagnostics() const noexcept
{
    return PrivateDiagnostics;
}

SyntaxTree SyntaxTree::Load(const std::shared_ptr<const String> FileName) noexcept
{
    const auto Text = std::make_shared<const String>(Hatcher([&] { return ReadFile(*FileName); }));
    const auto SourceText =
        std::make_shared<const class SourceText>(Hatcher([&] { return SourceText::From(Text, FileName); }));
    return Parse(SourceText);
}

SyntaxTree SyntaxTree::Parse(const std::shared_ptr<const String> Text) noexcept
{
    // return SyntaxTree();
}

MAMBA_NAMESPACE_END
