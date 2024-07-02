#include "DiagnosticBag.h"
#include "Diagnostic.h"
#include "MambaCore.h"
#include "SyntaxFacts.h"

namespace Mamba
{
    void DiagnosticBag::AddRange(const std::vector<std::shared_ptr<const class Diagnostic>>& Diagnostics) noexcept
    {
        append_range(Diagnostics);
    }

    void DiagnosticBag::ReportError(const TextLocation Location, const std::shared_ptr<const String> Message) noexcept
    {
        emplace_back(
            Hatcher([&] { return std::make_shared<const Diagnostic>(DiagnosticSeverity::Error, Location, Message); }));
    }

    void DiagnosticBag::ReportWarning(const TextLocation Location, const std::shared_ptr<const String> Message) noexcept
    {
        emplace_back(Hatcher(
            [&] { return std::make_shared<const Diagnostic>(DiagnosticSeverity::Warning, Location, Message); }));
    }

    void DiagnosticBag::ReportInformation(const TextLocation Location,
                                          const std::shared_ptr<const String> Message) noexcept
    {
        emplace_back(Hatcher(
            [&] { return std::make_shared<const Diagnostic>(DiagnosticSeverity::Information, Location, Message); }));
    }

    void DiagnosticBag::ReportInvalidCharacter(const TextLocation Location, const Char Character) noexcept
    {
        const auto Message = std::make_shared<const String>(
            Hatcher([&] { return Concat(TEXT("Invalid character '"), Character, TEXT("'.")); }));
        ReportError(Location, Message);
    }

    void DiagnosticBag::ReportUnterminatedString(const TextLocation Location) noexcept
    {
        const auto Message = std::make_shared<const String>(TEXT("Unterminated string literal."));
        ReportError(Location, Message);
    }

    void DiagnosticBag::ReportInvalidDecimal(const TextLocation Location, const StringView Literal) noexcept
    {
        const auto Message =
            std::make_shared<const String>(Concat(TEXT("Invalid decimal number '"), Literal, TEXT("'.")));
        ReportError(Location, Message);
    }

    void DiagnosticBag::ReportInvalidHexadecimal(const TextLocation Location, const StringView Literal) noexcept
    {
        const auto Message =
            std::make_shared<const String>(Concat(TEXT("Invalid hexadecimal number '"), Literal, TEXT("'.")));
        ReportError(Location, Message);
    }

    void DiagnosticBag::ReportInvalidBinary(const TextLocation Location, const StringView Literal) noexcept
    {
        const auto Message =
            std::make_shared<const String>(Concat(TEXT("Invalid binary number '"), Literal, TEXT("'.")));
        ReportError(Location, Message);
    }

    void DiagnosticBag::ReportInvalidOctal(const TextLocation Location, const StringView Literal) noexcept
    {
        const auto Message =
            std::make_shared<const String>(Concat(TEXT("Invalid octal number '"), Literal, TEXT("'.")));
        ReportError(Location, Message);
    }

    void DiagnosticBag::ReportUnexpectedToken(const TextLocation Location,
                                              const SyntaxKind Kind,
                                              const SyntaxKind ExpectedKind) noexcept
    {
        // Unexpected token 'Kind', Expected: 'ExpectedKind'.
        const auto Message = std::make_shared<const String>(Concat(TEXT("Unexpected token '"),
                                                                   SyntaxFacts::GetText(Kind),
                                                                   TEXT("'"),
                                                                   TEXT("Expected: '"),
                                                                   SyntaxFacts::GetText(ExpectedKind),
                                                                   TEXT("'.")));
        ReportError(Location, Message);
    }
} // namespace Mamba
