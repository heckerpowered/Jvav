#include "Lexer.h"
#include "DiagnosticBag.h"
#include "MambaCore.h"
#include "SourceText.h"
#include "SyntaxFacts.h"
#include "SyntaxKind.h"
#include "SyntaxToken.h"
#include "TextLocation.h"
#include "TextSpan.h"

#include <memory>

MAMBA_NAMESPACE_BEGIN

Lexer::Lexer(const std::shared_ptr<const class SyntaxTree> SyntaxTree) : SyntaxTree(SyntaxTree) {}

std::shared_ptr<const class SyntaxToken> Lexer::Lex() noexcept
{
    const auto TokenStart = Position;

    ReadToken();

    const auto TokenKind = Kind;
    const auto TokenValue = Value;
    const auto TokenLength = Position - Start;

    auto TokenText = std::make_shared<const String>(Hatcher([&] { return String(SyntaxFacts::GetText(TokenKind)); }));
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
        case TEXT('_'):
            ReadIdentifierOrKeyword();
            break;
        default:
            const auto IsLetter = (Current() >= TEXT('a') && Current() <= TEXT('z'))
                               || (Current() >= TEXT('A') && Current() <= TEXT('Z'));
            if (IsLetter)
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

MAMBA_NAMESPACE_END
