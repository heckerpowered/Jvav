#include "BoundCompilationUnit.h"
#include "DiagnosticPrinter.h"
#include "Lexer.h"
#include "LLVMBackend.h"
#include "MambaCore.h"

#include "Binder.h"
#include "Colors.h"
#include "Compiler.h"
#include "fast_io.h"
#include "Parser.h"

using namespace fast_io::io;
using namespace Mamba;

void PrintDiagnostics(std::span<const Diagnostic> Diagnostics) noexcept
{
    for (auto&& Diagnostic : Diagnostics)
    {
        fast_io::io::println(Diagnostic);
    }

    if (!Diagnostics.empty())
    {
        exit(0);
    }
}

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

Parser Parse(std::tuple<SyntaxTree&, Lexer&> Input) noexcept
{
    auto [SyntaxTree, Lexer] = Input;
    auto Tokens = std::vector<SyntaxToken>{};
    while (true)
    {
        auto Token = Lexer.Lex();
        if (Token.Kind() == SyntaxKind::WhitespaceToken)
        {
            continue;
        }

        Tokens.emplace_back(Token);
        if (Token.Kind() == SyntaxKind::EndOfFileToken)
        {
            break;
        }
    }

    PrintDiagnostics(Lexer.Diagnostics);
    return Parser(&SyntaxTree, std::move(Tokens));
}

void Compiler::Compile() noexcept
{
    if (SourceTexts.empty())
    {
        ReportNoInputFiles();
        return;
    }

    auto SyntaxTrees = SourceTexts | std::views::transform([](auto&& SourceText) { return SyntaxTree(SourceText); }) | std::ranges::to<std::vector>();
    auto Lexers = SyntaxTrees | std::views::transform([](auto&& SyntaxTree) { return Lexer(&SyntaxTree); }) | std::ranges::to<std::vector>();
    auto Parsers = std::views::zip(SyntaxTrees, Lexers) | std::views::transform(Parse) | std::ranges::to<std::vector>();
    for (std::tuple<SyntaxTree&, Parser&> Input : std::views::zip(SyntaxTrees, Parsers))
    {
        auto&& [SyntaxTree, Parser] = Input;
        SyntaxTree.PrivateRoot = Parser.ParseCompilationUnit();
        SyntaxTree.BuildParentsMap(*SyntaxTree.Root());
        PrintDiagnostics(Parser.Diagnostics);
    }
    auto Binders = SyntaxTrees | std::views::transform([](auto&& SyntaxTree) { return Binder(&SyntaxTree); }) | std::ranges::to<std::vector>();
    auto BoundCompilationUnits = Binders | std::views::transform([](auto&& Binder) { return Binder.BindCompilationUnit(); }) | std::ranges::to<std::vector>();
    for (auto&& Binder : Binders)
    {
        PrintDiagnostics(Binder.Diagnostics);
    }

    for (auto&& BoundCompilationUnit : BoundCompilationUnits)
    {
        delete BoundCompilationUnit;
    }

    LLVMBackend::GenerateCode(BoundCompilationUnits, "Main");
}