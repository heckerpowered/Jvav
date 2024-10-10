
#include <gtest/gtest.h>

#include "Binder.h"
#include "CompilationUnitSyntax.h"
#include "DiagnosticPrinter.h"
#include "Lexer.h"
#include "Parser.h"
#include "SourceText.h"
#include "SyntaxKind.h"
#include "SyntaxTree.h"

using namespace Mamba;

// undeclared identifier 'y'
// undefined operator '*' used for 'x' and 'y' (cannot deduce type of 'y')
String BinderSourceCode = TEXT(R"(
    fun square(x: int): int {
        return x * y
    }
)");

String BinderNormalCode = TEXT(R"(
    fun main(): int {
        
    }
)");

TEST(Compile, Binder)
{
    auto Info = SourceTextInfo{
        .FileName = TEXT("<testfile>"),
        .Text = BinderNormalCode
    };
    auto Source = SourceText(Info);
    auto Tree = new SyntaxTree(Source);
    auto Analyzer = new Lexer(Tree);

    std::vector<SyntaxToken> Tokens;
    while (true)
    {
        auto Token = Analyzer->Lex();
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

    auto Parser = ::Mamba::Parser(Tree, std::move(Tokens));

    Tree->Root() = Parser.ParseCompilationUnit();
    Tree->BuildParentsMap();

    EXPECT_EQ(Parser.Diagnostics.size(), 0);

    auto Binder = ::Mamba::Binder(Tree);
    delete Binder.BindCompilationUnit();

    EXPECT_EQ(Binder.Diagnostics.size(), 0);
}

TEST(Compile, BinderDiagnostic)
{
    auto Info = SourceTextInfo{
        .FileName = TEXT("<testfile>"),
        .Text = BinderSourceCode
    };
    auto Source = SourceText(Info);
    auto Tree = new SyntaxTree(Source);
    auto Analyzer = new Lexer(Tree);

    std::vector<SyntaxToken> Tokens;
    while (true)
    {
        auto Token = Analyzer->Lex();
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

    auto Parser = ::Mamba::Parser(Tree, std::move(Tokens));

    Tree->Root() = Parser.ParseCompilationUnit();
    Tree->BuildParentsMap();

    EXPECT_EQ(Parser.Diagnostics.size(), 0);

    auto Binder = ::Mamba::Binder(Tree);
    delete Binder.BindCompilationUnit();

    EXPECT_EQ(Binder.Diagnostics.size(), 2);
}