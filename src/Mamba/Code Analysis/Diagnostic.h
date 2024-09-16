#pragma once

#include "MambaCore.h"

#include "TextLocation.h"

namespace Mamba
{
    enum class DiagnosticSeverity
    {
        Error,
        Warning,
        Information
    };

    struct Diagnostic
    {
        DiagnosticSeverity Severity;
        TextLocation Location;
        String Message;

        std::size_t LineNumber() const noexcept;
        const TextLine& Line() const noexcept;
        StringView LineView() const noexcept;

        StringView FileName() const noexcept;
    };
} // namespace Mamba
