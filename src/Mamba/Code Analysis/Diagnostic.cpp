#include "Diagnostic.h"
#include "MambaCore.h"

MAMBA_NAMESPACE_BEGIN

Diagnostic::Diagnostic(const DiagnosticSeverity Severity,
                       const TextLocation Location,
                       const std::shared_ptr<const String>& Message) noexcept :
    Severity(Severity), Location(Location), Message(Message)
{
}

Diagnostic Diagnostic::Error(const TextLocation Location, const std::shared_ptr<const String>& Message) noexcept
{
    return Diagnostic(DiagnosticSeverity::Error, Location, Message);
}

Diagnostic Diagnostic::Warning(const TextLocation Location, const std::shared_ptr<const String>& Message) noexcept
{
    return Diagnostic(DiagnosticSeverity::Warning, Location, Message);
}

Diagnostic Diagnostic::Information(const TextLocation Location, const std::shared_ptr<const String>& Message) noexcept
{
    return Diagnostic(DiagnosticSeverity::Information, Location, Message);
}

MAMBA_NAMESPACE_END