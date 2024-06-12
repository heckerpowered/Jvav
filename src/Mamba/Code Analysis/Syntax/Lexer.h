#pragma once

#include "DiagnosticBag.h"
#include "MambaCore.h"
#include <memory>

MAMBA_NAMESPACE_BEGIN

class Lexer
{
public:
    DiagnosticBag Diagnostics;

    [[nodiscard]] Lexer(const std::shared_ptr<const class SyntaxTree> SyntaxTree);

    [[nodiscard]] std::shared_ptr<const class SyntaxToken> Lex() noexcept;
};

MAMBA_NAMESPACE_END