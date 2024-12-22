#include <gtest/gtest.h>

#include "Lexer.h"
#include "SourceText.h"
#include "SyntaxKind.h"
#include "SyntaxTree.h"

using namespace Mamba;

StringView LexerSourceCode = TEXT(R"(
    fun main(): int {
        $
    }
)");

StringView LexerNormalCode = TEXT(R"(
    fun main(): int {
        
    }
)");

TEST(Compile, Lexer)
{
    auto Info = SourceTextInfo{
        .FileLoader = fast_io::native_file_loader(),
        .FileName = StringView(TEXT("<testfile>")),
        .Text = LexerNormalCode
    };
    auto Source = SourceText(std::move(Info));
    auto Tree = new SyntaxTree(Source);
    auto Analyzer = new Lexer(Tree);

    while (true)
    {
        auto Token = Analyzer->Lex();
        if (Token.Kind() == SyntaxKind::EndOfFileToken)
        {
            break;
        }
    }

    EXPECT_EQ(Analyzer->Diagnostics.size(), 0);
}

TEST(Compile, LexerDiagnostic)
{
    auto Info = SourceTextInfo{
        .FileLoader = fast_io::native_file_loader(),
        .FileName = TEXT("<testfile>"),
        .Text = LexerSourceCode
    };
    auto Source = SourceText(std::move(Info));
    auto Tree = new SyntaxTree(Source);
    auto Analyzer = new Lexer(Tree);

    while (true)
    {
        auto Token = Analyzer->Lex();
        if (Token.Kind() == SyntaxKind::EndOfFileToken)
        {
            break;
        }
    }

    EXPECT_EQ(Analyzer->Diagnostics.size(), 1);
}