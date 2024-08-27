#include "BoundCompilationUnit.h"
#include "Lexer.h"
#include "LLVMBackend.h"
#include "MambaCore.h"

#include "Binder.h"
#include "Colors.h"
#include "Compiler.h"
#include "Parser.h"

using namespace fast_io::io;
using namespace Mamba;

void PrintError(const Diagnostic& Diagnostic) noexcept
{
    println(
        Color(fast_io::mnp::code_cvt(Diagnostic.Location.FileName()), Colors::BrightForegroundWhite),
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
        Color(fast_io::mnp::code_cvt(Diagnostic.Location.FileName()), Colors::BrightForegroundWhite),
        Color(":", Colors::BrightForegroundWhite),
        Color(Diagnostic.Location.StartLine(), Colors::BrightForegroundWhite),
        Color(":", Colors::BrightForegroundWhite),
        Color(Diagnostic.Location.StartCharacter(), Colors::BrightForegroundWhite),
        " ",
        Color("warning: ", Colors::BrightForegroundWhite),
        Color(fast_io::mnp::code_cvt(Diagnostic.Message), Colors::BrightForegroundWhite)
    );
}

void PrintInformation(const Diagnostic& Diagnostic) noexcept
{
    println(
        Color(fast_io::mnp::code_cvt(Diagnostic.Location.FileName()), Colors::BrightForegroundWhite),
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

void PrintDiagnostics(std::span<const Diagnostic> Diagnostics) noexcept
{
    for (auto&& Diagnostic : Diagnostics)
    {
        PrintDiagnostic(Diagnostic);
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

Parser Parse(const std::tuple<SyntaxTree, Lexer>& Input) noexcept
{
    auto [SyntaxTree, Lexer] = Input;
    auto Tokens = std::vector<const SyntaxToken>{};
    auto Token = Lexer.Lex();
    do
    {
        Tokens.emplace_back(Token);
        Token = Lexer.Lex();
    } while (Token.Kind() != SyntaxKind::EndOfFileToken);
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

    auto SyntaxTrees = SourceTexts | std::views::transform([](auto SourceText) { return SyntaxTree(SourceText); });
    auto Lexers = SyntaxTrees | std::views::transform([](auto SyntaxTree) { return Lexer(&SyntaxTree); });
    auto Parsers = std::views::zip(SyntaxTrees, Lexers) | std::views::transform(Parse);
    for (const std::tuple<SyntaxTree, Parser>& Input : std::views::zip(SyntaxTrees, Parsers))
    {
        auto [SyntaxTree, Parser] = Input;
        SyntaxTree.PrivateRoot = Parser.ParseCompilationUnit();
        SyntaxTree.BuildParentsMap(*SyntaxTree.Root());
        PrintDiagnostics(Parser.Diagnostics);
    }
    auto Binders = SyntaxTrees | std::views::transform([](auto&& SyntaxTree) { return Binder(&SyntaxTree); });
    auto BoundCompilationUnits = Binders | std::views::transform([](auto&& Binder) { return Binder.BindCompilationUnit(); });
    for (auto&& Binder : Binders)
    {
        PrintDiagnostics(Binder.Diagnostics);
    }

    for (auto&& BoundCompilationUnit : BoundCompilationUnits)
    {
        delete BoundCompilationUnit;
    }

    auto vector = BoundCompilationUnits | std::ranges::to<std::vector>();
    auto span = std::span(vector);
    LLVMBackend::GenerateCode(span, "Main");
}