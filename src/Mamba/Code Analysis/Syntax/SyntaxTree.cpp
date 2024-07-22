#include "SyntaxTree.h"

#include "CompilationUnitSyntax.h"
#include "Lexer.h"
#include "MambaCore.h"
#include "Parser.h"
#include "SourceText.h"
#include "SyntaxKind.h"
#include "SyntaxNode.h"
#include "SyntaxToken.h"

#include <fast_io.h>
#include <memory>
#include <mutex>

namespace Mamba
{
    [[nodiscard]] String ReadFile(const StringView FileName) noexcept
    {
        fast_io::native_file_loader NativeFileLoader{ FileName };
        return String(NativeFileLoader.begin(), NativeFileLoader.end());
    }

    SyntaxTree::SyntaxTree(const std::shared_ptr<const class SourceText> Text) noexcept : Text(Text) {}

    void SyntaxTree::Parse(
        const std::shared_ptr<const SyntaxTree> SyntaxTree,
        std::shared_ptr<const class CompilationUnitSyntax>& Root,
        std::vector<std::shared_ptr<const class Diagnostic>>& Diagnostics
    ) noexcept
    {
        Parser Parser(SyntaxTree);
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
            std::make_shared<const class SourceText>(Hatcher([=] { return SourceText::From(Text, FileName); }));
        return Parse(SourceText);
    }

    SyntaxTree SyntaxTree::Parse(const std::shared_ptr<const String> Text) noexcept
    {
        const auto SourceText =
            std::make_shared<const class SourceText>(Hatcher([&] { return SourceText::From(Text); }));
        return Parse(SourceText);
    }

    SyntaxTree SyntaxTree::Parse(const std::shared_ptr<const class SourceText> Text) noexcept
    {
        using SyntaxTreeType = const std::shared_ptr<const SyntaxTree>;
        using RootType = std::shared_ptr<const class CompilationUnitSyntax>&;
        using DiagnosticsType = std::vector<std::shared_ptr<const class Diagnostic>>&;

        using ParseFunctionType = void (&)(SyntaxTreeType, RootType, DiagnosticsType) noexcept;

        return SyntaxTree(Text, static_cast<ParseFunctionType>(Parse));
    }

    std::vector<std::shared_ptr<const class SyntaxToken>>
        SyntaxTree::ParseTokens(const std::shared_ptr<const String> Text, const bool IncludeEndOfFile) noexcept
    {
        const auto SourceText =
            std::make_shared<const class SourceText>(Hatcher([&] { return SourceText::From(Text); }));
        return ParseTokens(SourceText, IncludeEndOfFile);
    }

    std::vector<std::shared_ptr<const class SyntaxToken>> SyntaxTree::ParseTokens(
        const std::shared_ptr<const class SourceText> Text,
        const bool IncludeEndOfFile
    ) noexcept
    {
        return ParseTokens(Text, {}, IncludeEndOfFile);
    }

    std::vector<std::shared_ptr<const class SyntaxToken>> SyntaxTree::ParseTokens(
        const std::shared_ptr<const String> Text,
        NullablePointer<std::vector<std::shared_ptr<const class Diagnostic>>> Diagnostics,
        const bool IncludeEndOfFile
    ) noexcept
    {
        const auto SourceText =
            std::make_shared<const class SourceText>(Hatcher([&] { return SourceText::From(Text); }));
        return ParseTokens(SourceText, Diagnostics, IncludeEndOfFile);
    }

    std::vector<std::shared_ptr<const class SyntaxToken>> SyntaxTree::ParseTokens(
        const std::shared_ptr<const class SourceText> Text,
        NullablePointer<std::vector<std::shared_ptr<const class Diagnostic>>> Diagnostics,
        const bool IncludeEndOfFile
    ) noexcept
    {
        auto Tokens = std::vector<std::shared_ptr<const SyntaxToken>>();

        const auto ParseTokens = [&](const std::shared_ptr<const SyntaxTree> SyntaxTree,
                                     std::shared_ptr<const CompilationUnitSyntax>& Root,
                                     std::vector<std::shared_ptr<const class Diagnostic>>& Diagnostics) noexcept
        {
            Lexer Lexer(SyntaxTree);
            while (true)
            {
                const auto Token = Lexer.Lex();

                if (Token->Kind() != SyntaxKind::EndOfFileToken || IncludeEndOfFile)
                {
                    Tokens.emplace_back(Token);
                }

                if (Token->Kind() == SyntaxKind::EndOfFileToken)
                {
                    Root = std::make_shared<const CompilationUnitSyntax>(
                        SyntaxTree, std::vector<std::shared_ptr<const class MemberSyntax>>{}, Token
                    );
                    break;
                }
            }

            Diagnostics = std::move(Lexer.Diagnostics);
        };

        ::Mamba::SyntaxTree SyntaxTree(Text, ParseTokens);
        if (Diagnostics)
        {
            *Diagnostics = SyntaxTree.Diagnostics();
        }

        return Tokens;
    }

    NullableSharedPtr<const class SyntaxNode> SyntaxTree::GetParent(const std::shared_ptr<const class SyntaxNode> Node
    ) const noexcept
    {
        static auto Mutex = std::mutex{};

        const auto LockGuard = std::lock_guard<std::mutex>(Mutex);
        if (!Parents)
        {
            const auto Parents = CreateParentsMap(Root());
        }

        return Parents->at(Node);
    }

    SyntaxTree::ParentsMapType
        SyntaxTree::CreateParentsMap(const std::shared_ptr<const class CompilationUnitSyntax> Root) const noexcept
    {
        auto Result = ParentsMapType();
        Result.emplace(Root, nullptr);
        CreateParentsMap(Result, Root);
        return Result;
    }

    void SyntaxTree::CreateParentsMap(ParentsMapType& Result, const std::shared_ptr<const class SyntaxNode> Node)
        const noexcept
    {
        for (auto&& Child : Node->Children())
        {
            Result.emplace(Child, Node);
            CreateParentsMap(Result, Child);
        }
    }
} // namespace Mamba
