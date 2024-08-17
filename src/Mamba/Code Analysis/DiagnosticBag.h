#pragma once

#include <memory>
#include <vector>

#include "Diagnostic.h"
#include "SyntaxKind.h"
#include "TextLocation.h"

namespace Mamba
{
    class DiagnosticBag : public std::vector<struct Diagnostic>
    {
    public:
        using Super = std::vector<struct Diagnostic>;

        void AddRange(const std::vector<struct Diagnostic>& Diagnostics) noexcept;

    private:
        template<typename... T>
        void ReportError(const TextLocation Location, T&&... Args) noexcept
        {
            emplace_back(DiagnosticSeverity::Error, Location, Concat(std::forward<T>(Args)...));
        }

        template<typename... T>
        void ReportWarning(const TextLocation Location, T&&... Args) noexcept
        {
            emplace_back(DiagnosticSeverity::Warning, Location, Concat(std::forward<T>(Args)...));
        }

        template<typename... T>
        void ReportInformation(const TextLocation Location, T&&... Args) noexcept
        {
            emplace_back(DiagnosticSeverity::Warning, Location, Concat(std::forward<T>(Args)...));
        }

    public:
        void ReportInvalidCharacter(const TextLocation Location, const Char Character) noexcept;
        void ReportUnterminatedString(const TextLocation Location) noexcept;

        void ReportInvalidDecimal(const TextLocation Location, const StringView Literal) noexcept;
        void ReportInvalidHexadecimal(const TextLocation Location, const StringView Literal) noexcept;
        void ReportInvalidBinary(const TextLocation Location, const StringView Literal) noexcept;
        void ReportInvalidOctal(const TextLocation Location, const StringView Literal) noexcept;

        void ReportUnexpectedToken(
            const TextLocation Location,
            const SyntaxKind Kind,
            const SyntaxKind ExpectedKind
        ) noexcept;

        void ReportDiscardExpressionValue(const TextLocation Location) noexcept;
        void ReportVariableAlreadyDeclared(const TextLocation Location, const StringView Name) noexcept;
        void ReportUnreachableCode(const TextLocation Location) noexcept;
        void ReportUnreachableCode(const std::shared_ptr<const class SyntaxNode> Node) noexcept;
    };

} // namespace Mamba