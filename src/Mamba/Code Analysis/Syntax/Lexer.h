#pragma once

#include "DiagnosticBag.h"
#include "Literal.h"
#include "MambaCore.h"
#include "SyntaxKind.h"

#include <memory>

MAMBA_NAMESPACE_BEGIN

class Lexer
{
    const std::shared_ptr<const class SyntaxTree> SyntaxTree;
    const std::shared_ptr<const class SourceText> Text;

    std::size_t Position;
    std::size_t Start;

    SyntaxKind Kind;
    NullableSharedPtr<Literal> Value;

public:
    DiagnosticBag Diagnostics;

    [[nodiscard]] Lexer(const std::shared_ptr<const class SyntaxTree> SyntaxTree);

    [[nodiscard]] std::shared_ptr<const class SyntaxToken> Lex() noexcept;

private:
    Char Current() const noexcept;
    Char Lookahead() const noexcept;
    Char Peek(const std::size_t Offset) const noexcept;

    void ReadToken() noexcept;
    void ReadString() noexcept;
    void ReadNumber() noexcept;
    void ReadIdentifierOrKeyword() noexcept;
};

MAMBA_NAMESPACE_END