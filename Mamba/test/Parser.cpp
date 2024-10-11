
#include <gtest/gtest.h>

#include "DiagnosticPrinter.h"
#include "Lexer.h"
#include "Parser.h"
#include "SourceText.h"
#include "SyntaxKind.h"
#include "SyntaxTree.h"

using namespace Mamba;

// Unexpected token, expected identifier
String ParserSourceCode = TEXT(R"(
    fun main(parameter: ): int {
    }
)");

String ParserNormalCode = TEXT(R"(
    fun main(): int {
        
    }
)");

TEST(Compile, Parser)
{
    auto Info = SourceTextInfo{
        .FileName = TEXT("<testfile>"),
        .Text = ParserNormalCode
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
    static_cast<void>(Parser.ParseCompilationUnit());

    EXPECT_EQ(Parser.Diagnostics.size(), 0);
}

TEST(Compile, ParserDiagnostic)
{
    auto Info = SourceTextInfo{
        .FileName = TEXT("<testfile>"),
        .Text = ParserSourceCode
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
    static_cast<void>(Parser.ParseCompilationUnit());

    EXPECT_EQ(Parser.Diagnostics.size(), 1);
}