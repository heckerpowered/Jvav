#pragma once

#include "DiagnosticBag.h"
#include "SourceText.h"
#include "SyntaxTree.h"

namespace Mamba
{
    class Compiler
    {
        std::vector<SourceText> SourceTexts;
        DiagnosticBag Diagnostics;

        void ReportNoInputFiles() noexcept;

    public:
        void AddSourceFile(StringView FileName) noexcept;
        void Compile() noexcept;
    };
} // namespace Mamba