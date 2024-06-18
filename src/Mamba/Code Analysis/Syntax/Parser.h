#pragma once

#include "CompilationUnitSyntax.h"
#include "DiagnosticBag.h"
#include "MambaCore.h"
#include "SyntaxKind.h"
#include "SyntaxToken.h"
#include <memory>

MAMBA_NAMESPACE_BEGIN

class Parser
{
    const std::shared_ptr<const class SyntaxTree> SyntaxTree;
    const std::shared_ptr<const class SourceText> Text;
    const std::vector<std::shared_ptr<class SyntaxToken>> Tokens;
    std::size_t Position;

public:
    DiagnosticBag Diagnostics;

    [[nodiscard]] Parser(const std::shared_ptr<const class SyntaxTree> SyntaxTree) noexcept;

private:
    [[nodiscard]] std::shared_ptr<class SyntaxToken> Peek(const std::size_t Offset) noexcept;
    [[nodiscard]] std::shared_ptr<class SyntaxToken> Current() noexcept;
    [[nodiscard]] std::shared_ptr<class SyntaxToken> NextToken() noexcept;
    [[nodiscard]] std::shared_ptr<class SyntaxToken> MatchToken(const SyntaxKind Kind) noexcept;

public:
    [[nodiscard]] std::shared_ptr<const class CompilationUnitSyntax> ParseCompilationUnit() noexcept;
};

MAMBA_NAMESPACE_END