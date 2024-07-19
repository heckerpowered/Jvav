#include "Lexer.h"
#include "DiagnosticBag.h"
#include "MambaCore.h"
#include "SourceText.h"
#include "SyntaxFacts.h"
#include "SyntaxKind.h"
#include "SyntaxToken.h"
#include "SyntaxTree.h"
#include "TextLocation.h"
#include "TextSpan.h"

#include <cstdint>
#include <limits>
#include <memory>

namespace Mamba
{
    Lexer::Lexer(const std::shared_ptr<const class SyntaxTree> SyntaxTree) :
        SyntaxTree(SyntaxTree), Text(SyntaxTree->Text), Start(), Position(), Kind(SyntaxKind::BadToken)
    {
    }

    std::shared_ptr<const class SyntaxToken> Lexer::Lex() noexcept
    {
        const auto TokenStart = Position;

        ReadToken();

        const auto TokenKind = Kind;
        const auto TokenValue = Value;
        const auto TokenLength = Position - Start;

        auto TokenText =
            std::make_shared<const String>(Hatcher([&] { return String(SyntaxFacts::GetText(TokenKind)); }));
        if (TokenText->empty())
        {
            TokenText = Text->ToString(TokenStart, TokenLength);
        }

        return std::make_shared<const SyntaxToken>(SyntaxTree, Kind, TokenStart, TokenText, TokenValue);
    }

    Char Lexer::Current() const noexcept
    {
        return Peek(0);
    }

    Char Lexer::Lookahead() const noexcept
    {
        return Peek(1);
    }

    Char Lexer::Peek(const std::size_t Offset) const noexcept
    {
        const auto Index = Position + Offset;
        if (Index >= Text->Length())
        {
            return '\0';
        }

        return (*Text)[Index];
    }

    void Lexer::ReadToken() noexcept
    {
        Start = Position;
        Kind = SyntaxKind::BadToken;
        Value = nullptr;

        switch (Current())
        {
            case TEXT('\0'):
                Kind = SyntaxKind::EndOfFileToken;
                break;
            case TEXT('+'):
                ++Position;
                if (Current() != TEXT('='))
                {
                    Kind = SyntaxKind::PlusToken;
                }
                else
                {
                    Kind = SyntaxKind::PlusEqualsToken;
                    ++Position;
                }
                break;
            case TEXT('-'):
                ++Position;
                if (Current() != TEXT('='))
                {
                    Kind = SyntaxKind::MinusToken;
                }
                else
                {
                    Kind = SyntaxKind::MinusEqualsToken;
                    ++Position;
                }
                break;
            case TEXT('*'):
                ++Position;
                if (Current() != TEXT('='))
                {
                    Kind = SyntaxKind::StarToken;
                }
                else
                {
                    Kind = SyntaxKind::StarEqualsToken;
                    ++Position;
                }
                break;
            case TEXT('/'):
                ++Position;
                if (Current() != TEXT('='))
                {
                    Kind = SyntaxKind::SlashToken;
                }
                else
                {
                    Kind = SyntaxKind::SlashEqualsToken;
                    ++Position;
                }
                break;
            case TEXT('('):
                Kind = SyntaxKind::OpenParenthesisToken;
                ++Position;
                break;
            case TEXT(')'):
                Kind = SyntaxKind::CloseParenthesisToken;
                ++Position;
                break;
            case TEXT('{'):
                Kind = SyntaxKind::OpenBraceToken;
                ++Position;
                break;
            case TEXT('}'):
                Kind = SyntaxKind::CloseBraceToken;
                ++Position;
                break;
            case TEXT(':'):
                Kind = SyntaxKind::ColonToken;
                ++Position;
                break;
            case TEXT(','):
                Kind = SyntaxKind::CommaToken;
                ++Position;
                break;
            case TEXT('~'):
                Kind = SyntaxKind::TildeToken;
                ++Position;
                break;
            case TEXT('^'):
                ++Position;
                if (Current() != TEXT('='))
                {
                    Kind = SyntaxKind::HatToken;
                }
                else
                {
                    Kind = SyntaxKind::HatEqualsToken;
                    ++Position;
                }
                break;
            case TEXT('&'):
                ++Position;
                if (Current() == TEXT('&'))
                {
                    Kind = SyntaxKind::AmpersandAmpersandToken;
                    ++Position;
                }
                else if (Current() == TEXT('='))
                {
                    Kind = SyntaxKind::AmpersandEqualsToken;
                    ++Position;
                }
                else
                {
                    Kind = SyntaxKind::AmpersandToken;
                }
                break;
            case TEXT('|'):
                ++Position;
                if (Current() == TEXT('|'))
                {
                    Kind = SyntaxKind::PipePipeToken;
                    ++Position;
                }
                else if (Current() == TEXT('='))
                {
                    Kind = SyntaxKind::PipeEqualsToken;
                    ++Position;
                }
                else
                {
                    Kind = SyntaxKind::PipeToken;
                }
                break;
            case TEXT('='):
                ++Position;
                if (Current() != TEXT('='))
                {
                    Kind = SyntaxKind::EqualsToken;
                }
                else
                {
                    Kind = SyntaxKind::EqualsEqualsToken;
                    ++Position;
                }
                break;
            case TEXT('!'):
                ++Position;
                if (Current() != TEXT('='))
                {
                    Kind = SyntaxKind::BangToken;
                }
                else
                {
                    Kind = SyntaxKind::BangEqualsToken;
                    ++Position;
                }
                break;
            case TEXT('<'):
                ++Position;
                if (Current() != TEXT('='))
                {
                    Kind = SyntaxKind::LessToken;
                }
                else
                {
                    Kind = SyntaxKind::LessOrEqualsToken;
                    ++Position;
                }
                break;
            case TEXT('>'):
                ++Position;
                if (Current() != TEXT('='))
                {
                    Kind = SyntaxKind::GreaterToken;
                }
                else
                {
                    Kind = SyntaxKind::GreaterOrEqualsToken;
                    ++Position;
                }
                break;
            case TEXT('"'):
                ReadString();
                break;
            case TEXT('0'):
            case TEXT('1'):
            case TEXT('2'):
            case TEXT('3'):
            case TEXT('4'):
            case TEXT('5'):
            case TEXT('6'):
            case TEXT('7'):
            case TEXT('8'):
            case TEXT('9'):
                ReadNumber();
                break;
            case TEXT(' '):
            case TEXT('\t'):
            case TEXT('\r'):
                ReadWhitespace();
                break;
            case TEXT('_'):
                ReadIdentifierOrKeyword();
                break;
            default:
                if (IsLetter(Current()))
                {
                    ReadIdentifierOrKeyword();
                }
                else
                {
                    const auto Span = TextSpan(Position, 1);
                    const auto Location = TextLocation(Text, Span);
                    Diagnostics.ReportInvalidCharacter(Location, Current());
                    ++Position;
                }
                break;
        }
    }

    void Lexer::ReadString() noexcept
    {
        // Skip the current quote
        ++Position;

        bool Done = false;
        while (!Done)
        {
            switch (Current())
            {
                case TEXT('\0'):
                case TEXT('\r'):
                case TEXT('\n'):
                {
                    const auto Span = TextSpan(Start, 1);
                    const auto Location = TextLocation(Text, Span);
                    Diagnostics.ReportUnterminatedString(Location);
                    Done = true;
                    break;
                }
                case TEXT('"'):
                    if (Lookahead() == TEXT('"'))
                    {
                        Position += 2;
                    }
                    else
                    {
                        ++Position;
                        Done = true;
                    }
                    break;
                default:
                    ++Position;
                    break;
            }
        }

        const auto Length = Position - Start;

        Kind = SyntaxKind::StringToken;
        Value = std::make_shared<Literal>(Literal(Text->ToString(Start + 1, Length - 2)));
    }

    void Lexer::ReadIdentifierOrKeyword() noexcept
    {
        while (IsLetterOrDigit(Current()) || Current() == TEXT('_'))
        {
            ++Position;
        }

        const auto Length = Position - Start;
        const auto Span = TextSpan(Start, Length);
        const auto Text = this->Text->ToView(Span);
        Kind = SyntaxFacts::GetKeywordKind(Text);
    }

    void Lexer::ReadWhitespace() noexcept
    {
        while (Current() == TEXT(' ') || Current() == TEXT('\t') || Current() == TEXT('\r'))
        {
            ++Position;
        }

        Kind = SyntaxKind::WhitespaceToken;
    }

    void Lexer::ReadNumber() noexcept
    {
        // Decimal-literal is a non-zero decimal digit, followed by zero or more decimal digits.
        // Octal-literal is digit zero (0) followed by zero or more octal digits (0-7).
        // Hex-literal is the character sequence 0x or the character sequence 0X followed by one or more hexadecimal
        // digits (0-9, a-f, A-F). Binary-literal is the character sequence 0b or the character sequence 0B followed by
        // one or more binary digits (0, 1)

        // The current character is guaranteed to be a digit
        if (Current() != TEXT('0'))
        {
            ReadDecimal();
        }
        else if (Lookahead() == TEXT('x') || Lookahead() == TEXT('X'))
        {
            ReadHexadecimal();
        }
        else if (Lookahead() == TEXT('b') || Lookahead() == TEXT('B'))
        {
            ReadBinary();
        }

        ReadOctal();

        Kind = SyntaxKind::NumberToken;
    }

    void Lexer::ReadDecimal() noexcept
    {
        while (IsDecimalDigit(Current()))
        {
            ++Position;
        }

        const auto Length = Position - Start;
        const auto Span = TextSpan(Start, Length);
        const auto Number = ParseNumber<10>(Span);
        NarrowNumber(Number);
    }

    void Lexer::ReadHexadecimal() noexcept
    {
        while (IsHexadecimalDigit(Current()))
        {
            ++Position;
        }

        // Skip the '0x' or '0X' character sequence
        const auto Length = Position - Start;
        const auto Span = TextSpan(Start + 2, Length);
        const auto Number = ParseNumber<16>(Span);
        NarrowNumber(Number);
    }

    void Lexer::ReadBinary() noexcept
    {
        while (IsHexadecimalDigit(Current()))
        {
            ++Position;
        }

        // Skip the '0b' or '0B' character sequence
        const auto Length = Position - Start;
        const auto Span = TextSpan(Start + 2, Length);
        const auto Number = ParseNumber<2>(Span);
        NarrowNumber(Number);
    }

    void Lexer::ReadOctal() noexcept
    {
        while (IsHexadecimalDigit(Current()))
        {
            ++Position;
        }

        // Skip the 0 prefix
        const auto Length = Position - Start;
        const auto Span = TextSpan(Start + 1, Length);
        const auto Number = ParseNumber<8>(Span);
        NarrowNumber(Number);
    }

    bool Lexer::IsLetter(const Char Character) noexcept
    {
        return (Character >= TEXT('a') && Character <= TEXT('z')) || (Character >= TEXT('A') && Character <= TEXT('Z'));
    }

    bool Lexer::IsLetterOrDigit(const Char Character) noexcept
    {
        return IsLetter(Character) || IsDecimalDigit(Character);
    }

    bool Lexer::IsDecimalDigit(const Char Character) noexcept
    {
        return Character >= TEXT('0') && Character <= TEXT('9');
    }

    bool Lexer::IsHexadecimalDigit(const Char Character) noexcept
    {
        return (Character >= TEXT('0') && Character <= TEXT('9')) || (Character >= TEXT('a') && Character <= TEXT('f'))
            || (Character >= TEXT('A') && Character <= TEXT('F'));
    }

    bool Lexer::IsBinaryDigit(const Char Character) noexcept
    {
        return Character == TEXT('0') || Character == TEXT('1');
    }

    bool Lexer::IsOctalDigit(const Char Character) noexcept
    {
        return Character >= TEXT('0') && Character <= TEXT('7');
    }

    void Lexer::NarrowNumber(const std::uint64_t Value) noexcept
    {
        // Literals can be represented by int32 if they are capable of being represented by int32,
        // otherwise they are int64. If the literal represents a value outside the range of int64,
        // it is represented by unsigned int64, otherwise diagnostics are required.
        if (Value <= std::numeric_limits<std::int32_t>::max())
        {
            this->Value = std::make_shared<Literal>(static_cast<std::int32_t>(Value));
        }
        else if (Value <= std::numeric_limits<std::int64_t>::max())
        {
            this->Value = std::make_shared<Literal>(static_cast<std::int64_t>(Value));
        }

        this->Value = std::make_shared<Literal>(Value);
    }
} // namespace Mamba
