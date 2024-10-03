#include "Lexer.h"

#include "DiagnosticBag.h"
#include "fast_io.h"
#include "Literal.h"
#include "SourceText.h"
#include "SyntaxFacts.h"
#include "SyntaxKind.h"
#include "SyntaxToken.h"
#include "SyntaxTree.h"

namespace Mamba
{
    Lexer::Lexer(const class SyntaxTree* SyntaxTree) :
        SyntaxTree(SyntaxTree), Position(), Start(), Kind(SyntaxKind::BadToken)
    {
    }

    SyntaxToken Lexer::Lex() noexcept
    {
        auto TokenStart = Position;

        ReadToken();

        auto TokenValue = Value;
        auto TokenLength = Position - Start;

        auto TokenText = Text().SubView(TokenStart, TokenLength);

        return SyntaxToken(SyntaxTree, Kind, TokenText, TokenValue);
    }

    const SourceText& Lexer::Text() const noexcept
    {
        return SyntaxTree->Text();
    }

    Char Lexer::Current() const noexcept
    {
        return Peek(0);
    }

    Char Lexer::Lookahead() const noexcept
    {
        return Peek(1);
    }

    Char Lexer::Peek(std::size_t Offset) const noexcept
    {
        auto Index = Position + Offset;
        if (Index >= Text().Length())
        {
            return '\0';
        }

        return Text()[Index];
    }

    void Lexer::ReadToken() noexcept
    {
        Start = Position;
        Kind = SyntaxKind::BadToken;
        Value = {};

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
            case TEXT('\n'):
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
                    auto View = Text().SubView(Position, 1);
                    auto Location = TextLocation(Text(), View);

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
                    auto View = Text().SubView(Position, 1);
                    auto Location = TextLocation(Text(), View);
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

        auto Length = Position - Start;

        Kind = SyntaxKind::StringToken;
        Value = Text().SubView(Start + 1, Length - 2);
    }

    void Lexer::ReadIdentifierOrKeyword() noexcept
    {
        while (IsLetterOrDigit(Current()) || Current() == TEXT('_'))
        {
            ++Position;
        }

        auto Length = Position - Start;
        auto Text = this->Text().SubView(Start, Length);
        Kind = SyntaxFacts::GetKeywordKind(Text);
    }

    void Lexer::ReadWhitespace() noexcept
    {
        while (Current() == TEXT(' ') ||
               Current() == TEXT('\n') ||
               Current() == TEXT('\t') ||
               Current() == TEXT('\r'))
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
        if (Current() == TEXT('0') && IsOctalDigit(Lookahead()))
        {
            ReadOctal();
        }
        else if (Lookahead() == TEXT('x') || Lookahead() == TEXT('X'))
        {
            ReadHexadecimal();
        }
        else if (Lookahead() == TEXT('b') || Lookahead() == TEXT('B'))
        {
            ReadBinary();
        }
        else
        {
            ReadDecimal();
        }

        Kind = SyntaxKind::NumberToken;
    }

    void Lexer::ReadDecimal() noexcept
    {
        while (IsDecimalDigit(Current()))
        {
            ++Position;
        }

        auto Length = Position - Start;
        auto View = Text().SubView(Start, Length);
        Value = ParseNumber<10>(View);
    }

    void Lexer::ReadHexadecimal() noexcept
    {
        while (IsHexadecimalDigit(Current()))
        {
            ++Position;
        }

        // Skip the '0x' or '0X' character sequence
        auto Length = Position - Start;
        auto View = Text().SubView(Start + 2, Length);
        Value = ParseNumber<16>(View);
    }

    void Lexer::ReadBinary() noexcept
    {
        while (IsHexadecimalDigit(Current()))
        {
            ++Position;
        }

        // Skip the '0b' or '0B' character sequence
        auto Length = Position - Start;
        auto View = Text().SubView(Start + 2, Length);
        Value = ParseNumber<2>(View);
    }

    void Lexer::ReadOctal() noexcept
    {
        while (IsOctalDigit(Current()))
        {
            ++Position;
        }

        // Skip the 0 prefix
        auto Length = Position - Start;
        auto View = Text().SubView(Start + 1, Length);
        Value = ParseNumber<8>(View);
    }

    bool Lexer::IsLetter(Char Character) noexcept
    {
        return (Character >= TEXT('a') && Character <= TEXT('z')) ||
               (Character >= TEXT('A') && Character <= TEXT('Z'));
    }

    bool Lexer::IsLetterOrDigit(Char Character) noexcept
    {
        return IsLetter(Character) || IsDecimalDigit(Character);
    }

    bool Lexer::IsDecimalDigit(Char Character) noexcept
    {
        return Character >= TEXT('0') && Character <= TEXT('9');
    }

    bool Lexer::IsHexadecimalDigit(Char Character) noexcept
    {
        return (Character >= TEXT('0') && Character <= TEXT('9')) ||
               (Character >= TEXT('a') && Character <= TEXT('f')) ||
               (Character >= TEXT('A') && Character <= TEXT('F'));
    }

    bool Lexer::IsBinaryDigit(Char Character) noexcept
    {
        return Character == TEXT('0') || Character == TEXT('1');
    }

    bool Lexer::IsOctalDigit(Char Character) noexcept
    {
        return Character >= TEXT('0') && Character <= TEXT('7');
    }
} // namespace Mamba
