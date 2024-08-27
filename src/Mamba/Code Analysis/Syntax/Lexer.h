#pragma once

#include "DiagnosticBag.h"
#include "Literal.h"
#include "MambaCore.h"
#include "SourceText.h"
#include "SyntaxKind.h"
#include "SyntaxToken.h"

namespace Mamba
{
    class Lexer
    {
        const class SyntaxTree* SyntaxTree;

        std::size_t Position;
        std::size_t Start;

        SyntaxKind Kind;
        Literal Value;

    public:
        DiagnosticBag Diagnostics;

        [[nodiscard]] Lexer(const class SyntaxTree* SyntaxTree);

        [[nodiscard]] SyntaxToken Lex() noexcept;

    private:
        const SourceText& Text() const noexcept;

        [[nodiscard]] Char Current() const noexcept;
        [[nodiscard]] Char Lookahead() const noexcept;
        [[nodiscard]] Char Peek(std::size_t Offset) const noexcept;

        void ReadToken() noexcept;
        void ReadString() noexcept;
        void ReadIdentifierOrKeyword() noexcept;
        void ReadWhitespace() noexcept;

        void ReadNumber() noexcept;
        void ReadDecimal() noexcept;
        void ReadHexadecimal() noexcept;
        void ReadBinary() noexcept;
        void ReadOctal() noexcept;

        [[nodiscard]] static bool IsLetter(Char Character) noexcept;
        [[nodiscard]] static bool IsLetterOrDigit(Char Character) noexcept;
        [[nodiscard]] static bool IsDecimalDigit(Char Character) noexcept;
        [[nodiscard]] static bool IsHexadecimalDigit(Char Character) noexcept;
        [[nodiscard]] static bool IsBinaryDigit(Char Character) noexcept;
        [[nodiscard]] static bool IsOctalDigit(Char Character) noexcept;

        // Parse a literal number with a specified base, diagnostics are required if any error occurs.
        template<std::size_t Base>
        [[nodiscard]] std::uint64_t ParseNumber(StringView LiteralText) noexcept
            requires(Base == 2 || Base == 8 || Base == 10 || Base == 16)
        {
            auto Result = 0ULL;

            try
            {
                fast_io::io::scan(fast_io::basic_ibuffer_view<Char>(LiteralText), fast_io::mnp::base_get<Base>(Result));
            }
            catch (fast_io::error error)
            {
                ReportInvalidNumber<Base>(LiteralText);
            }

            return Result;
        }

        // Report an invalid number diagnostic with the specified base and literal
        template<std::size_t Base>
        void ReportInvalidNumber(StringView LiteralText)
            requires(Base == 2 || Base == 8 || Base == 10 || Base == 16)
        {
            const auto Location = TextLocation(Text(), LiteralText);
            if constexpr (Base == 10)
            {
                Diagnostics.ReportInvalidDecimal(Location, LiteralText);
            }
            else if constexpr (Base == 16)
            {
                Diagnostics.ReportInvalidHexadecimal(Location, LiteralText);
            }
            else if constexpr (Base == 2)
            {
                Diagnostics.ReportInvalidBinary(Location, LiteralText);
            }
            else if constexpr (Base == 8)
            {
                Diagnostics.ReportInvalidOctal(Location, LiteralText);
            }
        }
    };
} // namespace Mamba