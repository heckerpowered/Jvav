#pragma once

#include <vector>

#include "Diagnostic.h"
#include "SyntaxKind.h"
#include "SyntaxNode.h"
#include "TextLocation.h"
#include "TypeSymbol.h"

namespace Mamba
{
    class DiagnosticBag : public std::vector<struct Diagnostic>
    {
    public:
        using Super = std::vector<struct Diagnostic>;

    private:
        template<typename... T>
        void ReportError(TextLocation Location, T&&... Args) noexcept
        {
            emplace_back(DiagnosticSeverity::Error, Location, Concat(std::forward<T>(Args)...));
        }

        template<typename... T>
        void ReportWarning(TextLocation Location, T&&... Args) noexcept
        {
            emplace_back(DiagnosticSeverity::Warning, Location, Concat(std::forward<T>(Args)...));
        }

        template<typename... T>
        void ReportInformation(TextLocation Location, T&&... Args) noexcept
        {
            emplace_back(DiagnosticSeverity::Warning, Location, Concat(std::forward<T>(Args)...));
        }

    public:
        void ReportInvalidCharacter(TextLocation Location, Char Character) noexcept;
        void ReportUnterminatedString(TextLocation Location) noexcept;

        void ReportInvalidDecimal(TextLocation Location, StringView Literal) noexcept;
        void ReportInvalidHexadecimal(TextLocation Location, StringView Literal) noexcept;
        void ReportInvalidBinary(TextLocation Location, StringView Literal) noexcept;
        void ReportInvalidOctal(TextLocation Location, StringView Literal) noexcept;

        void ReportUnexpectedToken(TextLocation Location, SyntaxKind Kind, SyntaxKind ExpectedKind) noexcept;

        void ReportDiscardExpressionValue(TextLocation Location) noexcept;
        void ReportVariableAlreadyDeclared(TextLocation Location, StringView Name) noexcept;
        void ReportUnreachableCode(TextLocation Location) noexcept;
        void ReportUnreachableCode(const SyntaxNode* Node) noexcept;

        void ReportUndeclaredIdentifier(TextLocation Location, StringView Name) noexcept;
        void ReportAmbiguousIdentifier(TextLocation Location, StringView Name) noexcept;

        void ReportTypeMismatch(TextLocation Location, const TypeSymbol& ExpectedType, const TypeSymbol& ActualType) noexcept;
    };

} // namespace Mamba