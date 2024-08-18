#pragma once

#include "MambaCore.h"

#include <memory>
#include <optional>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Mamba
{
    class Compilation;

    class SyntaxTree : std::enable_shared_from_this<SyntaxTree>
    {
        friend Compilation;
        using ParentsMapType =
            std::unordered_map<std::shared_ptr<const class SyntaxNode>, NullableSharedPtr<const class SyntaxNode>>;

        std::optional<ParentsMapType> Parents;

        [[nodiscard]] SyntaxTree(const std::shared_ptr<const class SourceText> Text, auto&& Handler) noexcept
            requires requires {
                Handler(
                    shared_from_this(),
                    std::declval<std::shared_ptr<const class CompilationUnitSyntax>&>(),
                    std::declval<std::vector<struct Diagnostic>&>()
                );
            }
            :
            Text(Text)
        {
            Handler(std::make_shared<SyntaxTree>(*this), PrivateRoot, PrivateDiagnostics);
        }

        std::shared_ptr<const class CompilationUnitSyntax> PrivateRoot;
        std::vector<struct Diagnostic> PrivateDiagnostics;

        static void Parse(
            const std::shared_ptr<const SyntaxTree> SyntaxTree,
            std::shared_ptr<const class CompilationUnitSyntax>& Root,
            std::vector<struct Diagnostic>& Diagnostics
        ) noexcept;

    public:
        [[nodiscard]] SyntaxTree(const std::shared_ptr<const class SourceText> Text) noexcept;

        const std::shared_ptr<const class SourceText> Text;
        [[nodiscard]] const std::shared_ptr<const class CompilationUnitSyntax> Root() const noexcept;
        [[nodiscard]] const std::vector<struct Diagnostic>& Diagnostics() const noexcept;

        [[deprecated("No longer support contructing SyntaxTree from file directly.")]] [[nodiscard]] static SyntaxTree
            Load(const std::shared_ptr<const String> FileName) noexcept;

        [[nodiscard]] static SyntaxTree Parse(const std::shared_ptr<const String> Text) noexcept;
        [[nodiscard]] static SyntaxTree Parse(const std::shared_ptr<const class SourceText> Text) noexcept;

        [[nodiscard]] static std::vector<std::shared_ptr<const class SyntaxToken>> ParseTokens(
            const std::shared_ptr<const String> Text,
            const bool IncludeEndOfFile = false
        ) noexcept;

        [[nodiscard]] static std::vector<std::shared_ptr<const class SyntaxToken>> ParseTokens(
            const std::shared_ptr<const String> Text,
            NullablePointer<std::vector<struct Diagnostic>> Diagnostics,
            const bool IncludeEndOfFile = false
        ) noexcept;

        [[nodiscard]] static std::vector<std::shared_ptr<const class SyntaxToken>> ParseTokens(
            const std::shared_ptr<const class SourceText> Text,
            const bool IncludeEndOfFile = false
        ) noexcept;
        [[nodiscard]] static std::vector<std::shared_ptr<const class SyntaxToken>> ParseTokens(
            const std::shared_ptr<const class SourceText> Text,
            NullablePointer<std::vector<struct Diagnostic>> Diagnostics,
            const bool IncludeEndOfFile = false
        ) noexcept;

        [[nodiscard]] NullableSharedPtr<const class SyntaxNode>
            GetParent(const std::shared_ptr<const class SyntaxNode> Node) const noexcept;

    private:
        [[nodiscard]] ParentsMapType CreateParentsMap(const std::shared_ptr<const class CompilationUnitSyntax> Root
        ) const noexcept;

        void
            CreateParentsMap(ParentsMapType& Result, const std::shared_ptr<const class SyntaxNode> Node) const noexcept;
    };
} // namespace Mamba