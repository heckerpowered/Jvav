#pragma once

#include "MambaCore.h"
#include "TextLine.h"

#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

MAMBA_NAMESPACE_BEGIN

class SyntaxTree : std::enable_shared_from_this<SyntaxTree>
{
    using ParentsMapType =
        std::unordered_map<std::shared_ptr<const class SyntaxNode>, NullableSharedPtr<const class SyntaxNode>>;

    ParentsMapType Parents;

    [[nodiscard]] SyntaxTree(const std::shared_ptr<const class SourceText> Text, auto&& Handler) noexcept
        requires requires {
            Handler(shared_from_this(),
                    std::declval<std::shared_ptr<const class CompilationUnitSyntax>&>(),
                    std::declval<std::vector<std::shared_ptr<const class Diagnostic>>&>());
        }
        : Text(Text)
    {
        Handler(shared_from_this(), PrivateRoot, PrivateDiagnostics);
    }

    std::shared_ptr<const class CompilationUnitSyntax> PrivateRoot;
    std::vector<std::shared_ptr<const class Diagnostic>> PrivateDiagnostics;

    static void Parse(const std::shared_ptr<const SyntaxTree> SyntaxTree,
                      std::shared_ptr<const class CompilationUnitSyntax>& Root,
                      std::vector<std::shared_ptr<const class Diagnostic>>& Diagnostics) noexcept;

public:
    const std::shared_ptr<const class SourceText> Text;
    [[nodiscard]] const std::shared_ptr<const class CompilationUnitSyntax> Root() const noexcept;
    [[nodiscard]] const std::vector<std::shared_ptr<const class Diagnostic>>& Diagnostics() const noexcept;

    [[nodiscard]] static SyntaxTree Load(const StringView FileName) noexcept;

    [[nodiscard]] static SyntaxTree Parse(const StringView Text) noexcept;
    [[nodiscard]] static SyntaxTree Parse(const std::shared_ptr<const class SourceText> Text) noexcept;

    [[nodiscard]] static SyntaxTree ParseTokens(const StringView Text, const bool IncludeEndOfFile = false) noexcept;
    [[nodiscard]] static SyntaxTree ParseTokens(const StringView Text,
                                                std::vector<std::shared_ptr<const class Diagnostic>>& Diagnostics,
                                                const bool IncludeEndOfFile = false) noexcept;

    [[nodiscard]] static SyntaxTree ParseTokens(const std::shared_ptr<const class SourceText> Text,
                                                const bool IncludeEndOfFile = false) noexcept;
    [[nodiscard]] static SyntaxTree ParseTokens(const std::shared_ptr<const class SourceText> Text,
                                                std::vector<std::shared_ptr<const class Diagnostic>>& Diagnostics,
                                                const bool IncludeEndOfFile = false) noexcept;

    [[nodiscard]] NullableSharedPtr<const class SyntaxNode>
        GetParent(const std::shared_ptr<const class SyntaxNode> Node) const noexcept;

private:
    [[nodiscard]] ParentsMapType
        CreateParentsMap(const std::shared_ptr<const class CompilationUnitSyntax> Root) const noexcept;

    void CreateParentsMap(ParentsMapType& Result, const std::shared_ptr<const class SyntaxNode> Node) const noexcept;
};

MAMBA_NAMESPACE_END