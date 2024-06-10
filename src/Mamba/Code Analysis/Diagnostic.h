#pragma once

#include "MambaCore.h"
#include "TextLocation.h"
#include <memory>

MAMBA_NAMESPACE_BEGIN

enum class DiagnosticSeverity
{
    Error,
    Warning,
    Information
};

class Diagnostic final
{
    [[nodiscard]] Diagnostic(const DiagnosticSeverity Severity,
                             const TextLocation Location,
                             const std::shared_ptr<const String>& Message) noexcept;

public:
    const DiagnosticSeverity Severity;
    const TextLocation Location;
    const std::shared_ptr<const String> Message;

    static Diagnostic Error(const TextLocation Location, const std::shared_ptr<const String>& Message) noexcept;
    static Diagnostic Warning(const TextLocation Location, const std::shared_ptr<const String>& Message) noexcept;
    static Diagnostic Information(const TextLocation Location, const std::shared_ptr<const String>& Message) noexcept;
};

MAMBA_NAMESPACE_END