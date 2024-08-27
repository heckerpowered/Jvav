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
    };
} // namespace Mamba
