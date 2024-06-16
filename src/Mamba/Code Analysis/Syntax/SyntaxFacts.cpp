#include "SyntaxFacts.h"
#include "fast_io.h"
#include "MambaCore.h"

#include <unordered_map>
#include <utility>

MAMBA_NAMESPACE_BEGIN

std::size_t SyntaxFacts::GetUnaryOperatorPrecedence(const SyntaxKind Kind) noexcept
{
    switch (Kind)
    {
        case SyntaxKind::PlusToken:
        case SyntaxKind::MinusToken:
        case SyntaxKind::BangToken:
        case SyntaxKind::TildeToken:
            return 6;
        default:
            return 0;
    }
}

std::size_t SyntaxFacts::GetBinaryOperatorPrecedence(const SyntaxKind Kind) noexcept
{
    switch (Kind)
    {
        case SyntaxKind::StarToken:
        case SyntaxKind::SlashToken:
            return 5;

        case SyntaxKind::PlusToken:
        case SyntaxKind::MinusToken:
            return 4;

        case SyntaxKind::EqualsEqualsToken:
        case SyntaxKind::BangEqualsToken:
        case SyntaxKind::LessToken:
        case SyntaxKind::LessOrEqualsToken:
        case SyntaxKind::GreaterToken:
        case SyntaxKind::GreaterOrEqualsToken:
            return 3;

        case SyntaxKind::AmpersandToken:
        case SyntaxKind::AmpersandAmpersandToken:
            return 2;

        case SyntaxKind::PipeToken:
        case SyntaxKind::PipePipeToken:
        case SyntaxKind::HatToken:
            return 1;

        default:
            return 0;
    }
}

SyntaxKind SyntaxFacts::GetKeywordKind(const StringView Text) noexcept
{
    static const auto KeywordsMap = std::unordered_map<StringView, SyntaxKind>{
        std::make_pair(TEXT("if"), SyntaxKind::IfKeyword),
        std::make_pair(TEXT("else"), SyntaxKind::ElseKeyword),

        std::make_pair(TEXT("for"), SyntaxKind::ForKeyword),
        std::make_pair(TEXT("while"), SyntaxKind::WhileKeyword),
        std::make_pair(TEXT("do"), SyntaxKind::DoKeyword),

        std::make_pair(TEXT("fun"), SyntaxKind::FunctionKeyword),

        std::make_pair(TEXT("let"), SyntaxKind::LetKeyword),
        std::make_pair(TEXT("var"), SyntaxKind::VarKeyword),

        std::make_pair(TEXT("true"), SyntaxKind::TrueKeyword),
        std::make_pair(TEXT("false"), SyntaxKind::FalseKeyword),

        std::make_pair(TEXT("break"), SyntaxKind::BreakKeyword),
        std::make_pair(TEXT("continue"), SyntaxKind::ContinueKeyword),

        std::make_pair(TEXT("return"), SyntaxKind::ReturnKeyword),
    };

    const auto Iterator = KeywordsMap.find(Text);
    if (Iterator != KeywordsMap.end())
    {
        return Iterator->second;
    }

    return SyntaxKind::IdentifierToken;
}

std::vector<SyntaxKind> SyntaxFacts::GetUnaryOperatorKinds() noexcept
{
    return { SyntaxKind::PlusToken, SyntaxKind::MinusToken, SyntaxKind::BangToken, SyntaxKind::TildeToken };
}

std::vector<SyntaxKind> SyntaxFacts::GetBinaryOperatorKinds() noexcept
{
    return { SyntaxKind::StarToken,         SyntaxKind::SlashToken,
             SyntaxKind::PlusToken,         SyntaxKind::MinusToken,
             SyntaxKind::EqualsEqualsToken, SyntaxKind::BangEqualsToken,
             SyntaxKind::LessToken,         SyntaxKind::LessOrEqualsToken,
             SyntaxKind::GreaterToken,      SyntaxKind::GreaterOrEqualsToken,
             SyntaxKind::AmpersandToken,    SyntaxKind::AmpersandAmpersandToken,
             SyntaxKind::PipeToken,         SyntaxKind::PipePipeToken,
             SyntaxKind::HatToken };
}

StringView SyntaxFacts::GetText(const SyntaxKind Kind) noexcept
{
    switch (Kind)
    {
        case SyntaxKind::PlusToken:
            return TEXT("+");
        case SyntaxKind::PlusEqualsToken:
            return TEXT("+=");
        case SyntaxKind::MinusToken:
            return TEXT("-");
        case SyntaxKind::MinusEqualsToken:
            return TEXT("-=");
        case SyntaxKind::StarToken:
            return TEXT("*");
        case SyntaxKind::StarEqualsToken:
            return TEXT("*=");
        case SyntaxKind::SlashToken:
            return TEXT("/");
        case SyntaxKind::SlashEqualsToken:
            return TEXT("/=");
        case SyntaxKind::BangToken:
            return TEXT("!");
        case SyntaxKind::EqualsToken:
            return TEXT("=");
        case SyntaxKind::TildeToken:
            return TEXT("~");
        case SyntaxKind::LessToken:
            return TEXT("<");
        case SyntaxKind::LessOrEqualsToken:
            return TEXT("<=");
        case SyntaxKind::GreaterToken:
            return TEXT(">");
        case SyntaxKind::GreaterOrEqualsToken:
            return TEXT(">=");
        case SyntaxKind::AmpersandToken:
            return TEXT("&");
        case SyntaxKind::AmpersandAmpersandToken:
            return TEXT("&&");
        case SyntaxKind::AmpersandEqualsToken:
            return TEXT("&=");
        case SyntaxKind::PipeToken:
            return TEXT("|");
        case SyntaxKind::PipeEqualsToken:
            return TEXT("|=");
        case SyntaxKind::PipePipeToken:
            return TEXT("||");
        case SyntaxKind::HatToken:
            return TEXT("^");
        case SyntaxKind::HatEqualsToken:
            return TEXT("^=");
        case SyntaxKind::EqualsEqualsToken:
            return TEXT("==");
        case SyntaxKind::BangEqualsToken:
            return TEXT("!=");
        case SyntaxKind::OpenParenthesisToken:
            return TEXT("(");
        case SyntaxKind::CloseParenthesisToken:
            return TEXT(")");
        case SyntaxKind::OpenBraceToken:
            return TEXT("{");
        case SyntaxKind::CloseBraceToken:
            return TEXT("}");
        case SyntaxKind::ColonToken:
            return TEXT(":");
        case SyntaxKind::CommaToken:
            return TEXT(",");
        case SyntaxKind::BreakKeyword:
            return TEXT("break");
        case SyntaxKind::ContinueKeyword:
            return TEXT("continue");
        case SyntaxKind::ElseKeyword:
            return TEXT("else");
        case SyntaxKind::FalseKeyword:
            return TEXT("false");
        case SyntaxKind::ForKeyword:
            return TEXT("for");
        case SyntaxKind::FunctionKeyword:
            return TEXT("fun");
        case SyntaxKind::IfKeyword:
            return TEXT("if");
        case SyntaxKind::LetKeyword:
            return TEXT("let");
        case Mamba::SyntaxKind::VarKeyword:
            return TEXT("var");
        case SyntaxKind::ReturnKeyword:
            return TEXT("return");
        case SyntaxKind::TrueKeyword:
            return TEXT("true");
        case SyntaxKind::WhileKeyword:
            return TEXT("while");
        case SyntaxKind::DoKeyword:
            return TEXT("do");
        default:
            return {};
    }
}

bool SyntaxFacts::IsKeyword(const SyntaxKind Kind) noexcept
{
    return GetText(Kind).ends_with(TEXT("Keyword"));
}

bool SyntaxFacts::IsToken(const SyntaxKind Kind) noexcept
{
    return IsKeyword(Kind) || GetText(Kind).ends_with(TEXT("Token"));
}

SyntaxKind SyntaxFacts::GetBinaryOperatorOfAssignmentOperator(const SyntaxKind Kind) noexcept
{
    switch (Kind)
    {
        case SyntaxKind::PlusEqualsToken:
            return SyntaxKind::PlusToken;
        case SyntaxKind::MinusEqualsToken:
            return SyntaxKind::MinusToken;
        case SyntaxKind::StarEqualsToken:
            return SyntaxKind::StarToken;
        case SyntaxKind::SlashEqualsToken:
            return SyntaxKind::SlashToken;
        case SyntaxKind::AmpersandEqualsToken:
            return SyntaxKind::AmpersandToken;
        case SyntaxKind::PipeEqualsToken:
            return SyntaxKind::PipeToken;
        case SyntaxKind::HatEqualsToken:
            return SyntaxKind::HatToken;
        default:
#ifdef DEBUG
            const auto Message =
                Concat(TEXT("No binary operator for assignment operator: '"), GetText(Kind), TEXT("'"));
            fast_io::io::perrln(fast_io::mnp::code_cvt(Message));
#endif
            std::unreachable();
    }
}

MAMBA_NAMESPACE_END