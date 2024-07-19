#pragma once

#include "DiagnosticBag.h"
#include "MambaCore.h"
#include "SourceText.h"
#include "SyntaxKind.h"
#include "TextSpan.h"

#include <memory>

namespace Mamba
{
    class Lexer
    {
        const std::shared_ptr<const class SyntaxTree> SyntaxTree;
        const std::shared_ptr<const class SourceText> Text;

        std::size_t Position;
        std::size_t Start;

        SyntaxKind Kind;
        NullableSharedPtr<class Literal> Value;

    public:
        DiagnosticBag Diagnostics;

        [[nodiscard]] Lexer(const std::shared_ptr<const class SyntaxTree> SyntaxTree);

        [[nodiscard]] std::shared_ptr<const class SyntaxToken> Lex() noexcept;

    private:
        [[nodiscard]] Char Current() const noexcept;
        [[nodiscard]] Char Lookahead() const noexcept;
        [[nodiscard]] Char Peek(const std::size_t Offset) const noexcept;

        void ReadToken() noexcept;
        void ReadString() noexcept;
        void ReadIdentifierOrKeyword() noexcept;
        void ReadWhitespace() noexcept;

        void ReadNumber() noexcept;
        void ReadDecimal() noexcept;
        void ReadHexadecimal() noexcept;
        void ReadBinary() noexcept;
        void ReadOctal() noexcept;

        [[nodiscard]] static bool IsLetter(const Char Character) noexcept;
        [[nodiscard]] static bool IsLetterOrDigit(const Char Character) noexcept;
        [[nodiscard]] static bool IsDecimalDigit(const Char Character) noexcept;
        [[nodiscard]] static bool IsHexadecimalDigit(const Char Character) noexcept;
        [[nodiscard]] static bool IsBinaryDigit(const Char Character) noexcept;
        [[nodiscard]] static bool IsOctalDigit(const Char Character) noexcept;

        // Narrow down the supplied number, choose the smallest type that can represent it,
        // and save it to the member variable 'Value'.
        void NarrowNumber(const std::uint64_t Value) noexcept;

        // Parse a literal number with a specified base, diagnostics are required if any error occurs.
        template<std::size_t Base>
        [[nodiscard]] std::uint64_t ParseNumber(const TextSpan Span) noexcept
            requires(Base == 2 || Base == 8 || Base == 10 || Base == 16)
        {
            const auto Literal = Text->ToView(Span);

            auto Result = 0ULL;

#ifdef MAMBA_NO_EXCEPTIONS
            fast_io::io::scan(fast_io::basic_ibuffer_view<Char>(Literal), fast_io::mnp::base_get<Base>(Result));
#else
            try
            {
                fast_io::io::scan(fast_io::basic_ibuffer_view<Char>(Literal), fast_io::mnp::base_get<Base>(Result));
            }
            catch (const fast_io::error& error)
            {
                ReportInvalidNumber<Base>(Span, Literal);
                fast_io::io::perrln(error);
            }
#endif

            return Result;
        }

        // Report an invalid number diagnostic with the specified base and literal
        template<std::size_t Base>
        void ReportInvalidNumber(const TextSpan Span, const StringView Literal)

            requires(Base == 2 || Base == 8 || Base == 10 || Base == 16)
        {
            const auto Location = TextLocation(Text, Span);
            if constexpr (Base == 10)
            {
                Diagnostics.ReportInvalidDecimal(Location, Literal);
            }
            else if constexpr (Base == 16)
            {
                Diagnostics.ReportInvalidHexadecimal(Location, Literal);
            }
            else if constexpr (Base == 2)
            {
                Diagnostics.ReportInvalidBinary(Location, Literal);
            }
            else if constexpr (Base == 8)
            {
                Diagnostics.ReportInvalidOctal(Location, Literal);
            }
        }
    };
} // namespace Mamba