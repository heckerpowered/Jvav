#include "DiagnosticBag.h"
#include "Diagnostic.h"
#include "MambaCore.h"

MAMBA_NAMESPACE_BEGIN

void DiagnosticBag::AddRange(const std::vector<std::shared_ptr<const class Diagnostic>>& Diagnostics) noexcept
{
    append_range(Diagnostics);
}

void DiagnosticBag::ReportError(const TextLocation Location, const std::shared_ptr<const String> Message)
{
    emplace_back(
        Hatcher([&] { return std::make_shared<const Diagnostic>(DiagnosticSeverity::Error, Location, Message); }));
}

void DiagnosticBag::ReportWarning(const TextLocation Location, const std::shared_ptr<const String> Message)
{
    emplace_back(
        Hatcher([&] { return std::make_shared<const Diagnostic>(DiagnosticSeverity::Warning, Location, Message); }));
}

void DiagnosticBag::ReportInformation(const TextLocation Location, const std::shared_ptr<const String> Message)
{
    emplace_back(Hatcher(
        [&] { return std::make_shared<const Diagnostic>(DiagnosticSeverity::Information, Location, Message); }));
}

void DiagnosticBag::ReportInvalidCharacter(const TextLocation Location, const Char Character)
{
    const auto Message = std::make_shared<const String>(
        Hatcher([&] { return Concat(TEXT("Invalid character '"), Character, TEXT("'.")); }));
    ReportError(Location, Message);
}

void DiagnosticBag::ReportUnterminatedString(const TextLocation Location)
{
    const auto Message = std::make_shared<const String>(TEXT("Unterminated string literal."));
    ReportError(Location, Message);
}

MAMBA_NAMESPACE_END