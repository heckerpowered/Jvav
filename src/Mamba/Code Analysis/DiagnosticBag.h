#pragma once

#include "MambaCore.h"
#include "SyntaxKind.h"
#include "TextLocation.h"

#include <memory>
#include <vector>

namespace Mamba
{
    class DiagnosticBag : public std::vector<std::shared_ptr<const class Diagnostic>>
    {
    public:
        using Super = std::vector<class Diagnostic>;

        void AddRange(const std::vector<std::shared_ptr<const class Diagnostic>>& Diagnostics) noexcept;

    private:
        void ReportError(const TextLocation Location, const std::shared_ptr<const String> Message) noexcept;
        void ReportWarning(const TextLocation Location, const std::shared_ptr<const String> Message) noexcept;
        void ReportInformation(const TextLocation Location, const std::shared_ptr<const String> Message) noexcept;

    public:
        void ReportInvalidCharacter(const TextLocation Location, const Char Character) noexcept;
        void ReportUnterminatedString(const TextLocation Location) noexcept;

        void ReportInvalidDecimal(const TextLocation Location, const StringView Literal) noexcept;
        void ReportInvalidHexadecimal(const TextLocation Location, const StringView Literal) noexcept;
        void ReportInvalidBinary(const TextLocation Location, const StringView Literal) noexcept;
        void ReportInvalidOctal(const TextLocation Location, const StringView Literal) noexcept;

        void ReportUnexpectedToken(const TextLocation Location, const SyntaxKind Kind,
                                   const SyntaxKind ExpectedKind) noexcept;
    };

} // namespace Mamba