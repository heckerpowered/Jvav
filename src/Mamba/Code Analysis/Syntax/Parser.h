#pragma once

#include "CompilationUnitSyntax.h"
#include "DiagnosticBag.h"
#include "MambaCore.h"
#include <memory>

MAMBA_NAMESPACE_BEGIN

class Parser
{
public:
    DiagnosticBag Diagnostics;

    [[nodiscard]] Parser(const std::shared_ptr<const class SyntaxTree> SyntaxTree);

    [[nodiscard]] std::shared_ptr<const class CompilationUnitSyntax> ParseCompilationUnit() noexcept;
};

MAMBA_NAMESPACE_END