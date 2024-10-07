#include "SyntaxFacts.h"
#include "fast_io.h"
#include "MambaCore.h"
#include "SyntaxKind.h"

#include <unordered_map>
#include <utility>

namespace Mamba
{
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
        static auto KeywordsMap = std::unordered_map<StringView, SyntaxKind>{
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

            std::make_pair(TEXT("mut"), SyntaxKind::MutKeyword),
            std::make_pair(TEXT("pure"), SyntaxKind::PureKeyword),
            std::make_pair(TEXT("const"), SyntaxKind::ConstKeyword)
        };

        auto Iterator = KeywordsMap.find(Text);
        if (Iterator != KeywordsMap.end())
        {
            return Iterator->second;
        }

        return SyntaxKind::IdentifierToken;
    }

    std::vector<SyntaxKind> SyntaxFacts::GetUnaryOperatorKinds() noexcept
    {
        return {
            SyntaxKind::PlusToken,
            SyntaxKind::MinusToken,
            SyntaxKind::BangToken,
            SyntaxKind::TildeToken
        };
    }

    std::vector<SyntaxKind> SyntaxFacts::GetBinaryOperatorKinds() noexcept
    {
        return {
            SyntaxKind::StarToken,
            SyntaxKind::SlashToken,
            SyntaxKind::PlusToken,
            SyntaxKind::MinusToken,
            SyntaxKind::EqualsEqualsToken,
            SyntaxKind::BangEqualsToken,
            SyntaxKind::LessToken,
            SyntaxKind::LessOrEqualsToken,
            SyntaxKind::GreaterToken,
            SyntaxKind::GreaterOrEqualsToken,
            SyntaxKind::AmpersandToken,
            SyntaxKind::AmpersandAmpersandToken,
            SyntaxKind::PipeToken,
            SyntaxKind::PipePipeToken,
            SyntaxKind::HatToken
        };
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
            case SyntaxKind::VarKeyword:
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

    StringView SyntaxFacts::ToString(const SyntaxKind Kind) noexcept
    {
        switch (Kind)
        {
            case SyntaxKind::BadToken:
                return TEXT("BadToken");
            case SyntaxKind::EndOfFileToken:
                return TEXT("EndOfFileToken");
            case SyntaxKind::NumberToken:
                return TEXT("NumberToken");
            case SyntaxKind::StringToken:
                return TEXT("StringToken");
            case SyntaxKind::PlusToken:
                return TEXT("PlusToken");
            case SyntaxKind::PlusEqualsToken:
                return TEXT("PlusEqualsToken");
            case SyntaxKind::MinusToken:
                return TEXT("MinusToken");
            case SyntaxKind::MinusEqualsToken:
                return TEXT("MinusEqualsToken");
            case SyntaxKind::StarToken:
                return TEXT("StarToken");
            case SyntaxKind::StarEqualsToken:
                return TEXT("StarEqualsToken");
            case SyntaxKind::SlashToken:
                return TEXT("SlashToken");
            case SyntaxKind::SlashEqualsToken:
                return TEXT("SlashEqualsToken");
            case SyntaxKind::BangToken:
                return TEXT("BangToken");
            case SyntaxKind::EqualsToken:
                return TEXT("EqualsToken");
            case SyntaxKind::TildeToken:
                return TEXT("TildeToken");
            case SyntaxKind::HatToken:
                return TEXT("HatToken");
            case SyntaxKind::HatEqualsToken:
                return TEXT("HatEqualsToken");
            case SyntaxKind::AmpersandToken:
                return TEXT("AmpersandToken");
            case SyntaxKind::AmpersandAmpersandToken:
                return TEXT("AmpersandAmpersandToken");
            case SyntaxKind::AmpersandEqualsToken:
                return TEXT("AmpersandEqualsToken");
            case SyntaxKind::PipeToken:
                return TEXT("PipeToken");
            case SyntaxKind::PipeEqualsToken:
                return TEXT("PipeEqualsToken");
            case SyntaxKind::PipePipeToken:
                return TEXT("PipePipeToken");
            case SyntaxKind::EqualsEqualsToken:
                return TEXT("EqualsEqualsToken");
            case SyntaxKind::BangEqualsToken:
                return TEXT("BangEqualsToken");
            case SyntaxKind::LessToken:
                return TEXT("LessToken");
            case SyntaxKind::LessOrEqualsToken:
                return TEXT("LessOrEqualsToken");
            case SyntaxKind::GreaterToken:
                return TEXT("GreaterToken");
            case SyntaxKind::GreaterOrEqualsToken:
                return TEXT("GreaterOrEqualsToken");
            case SyntaxKind::OpenParenthesisToken:
                return TEXT("OpenParenthesisToken");
            case SyntaxKind::CloseParenthesisToken:
                return TEXT("CloseParenthesisToken");
            case SyntaxKind::OpenBraceToken:
                return TEXT("OpenBraceToken");
            case SyntaxKind::CloseBraceToken:
                return TEXT("CloseBraceToken");
            case SyntaxKind::ColonToken:
                return TEXT("ColonToken");
            case SyntaxKind::CommaToken:
                return TEXT("CommaToken");
            case SyntaxKind::IdentifierToken:
                return TEXT("IdentifierToken");
            case SyntaxKind::WhitespaceToken:
                return TEXT("WhitespaceToken");
            case SyntaxKind::BreakKeyword:
                return TEXT("BreakKeyword");
            case SyntaxKind::ContinueKeyword:
                return TEXT("ContinueKeyword");
            case SyntaxKind::ElseKeyword:
                return TEXT("ElseKeyword");
            case SyntaxKind::FalseKeyword:
                return TEXT("FalseKeyword");
            case SyntaxKind::ForKeyword:
                return TEXT("ForKeyword");
            case SyntaxKind::FunctionKeyword:
                return TEXT("FunctionKeyword");
            case SyntaxKind::IfKeyword:
                return TEXT("IfKeyword");
            case SyntaxKind::LetKeyword:
                return TEXT("LetKeyword");
            case SyntaxKind::VarKeyword:
                return TEXT("VarKeyword");
            case SyntaxKind::ReturnKeyword:
                return TEXT("ReturnKeyword");
            case SyntaxKind::ToKeyword:
                return TEXT("ToKeyword");
            case SyntaxKind::TrueKeyword:
                return TEXT("TrueKeyword");
            case SyntaxKind::WhileKeyword:
                return TEXT("WhileKeyword");
            case SyntaxKind::DoKeyword:
                return TEXT("DoKeyword");
            case SyntaxKind::CompilationUnit:
                return TEXT("CompilationUnit");
            case SyntaxKind::FunctionDeclaration:
                return TEXT("FunctionDeclaration");
            case SyntaxKind::GlobalStatement:
                return TEXT("GlobalStatement");
            case SyntaxKind::Parameter:
                return TEXT("Parameter");
            case SyntaxKind::TypeClause:
                return TEXT("TypeClause");
            case SyntaxKind::ElseClause:
                return TEXT("ElseClause");
            case SyntaxKind::BlockStatement:
                return TEXT("BlockStatement");
            case SyntaxKind::VariableDeclaration:
                return TEXT("VariableDeclaration");
            case SyntaxKind::IfStatement:
                return TEXT("IfStatement");
            case SyntaxKind::WhileStatement:
                return TEXT("WhileStatement");
            case SyntaxKind::DoWhileStatement:
                return TEXT("DoWhileStatement");
            case SyntaxKind::ForStatement:
                return TEXT("ForStatement");
            case SyntaxKind::ReturnStatement:
                return TEXT("ReturnStatement");
            case SyntaxKind::ExpressionStatement:
                return TEXT("ExpressionStatement");
            case SyntaxKind::ContinueStatement:
                return TEXT("ContinueStatement");
            case SyntaxKind::BreakStatement:
                return TEXT("BreakStatement");
            case SyntaxKind::LiteralExpression:
                return TEXT("LiteralExpression");
            case SyntaxKind::NameExpression:
                return TEXT("NameExpression");
            case SyntaxKind::UnaryExpression:
                return TEXT("UnaryExpression");
            case SyntaxKind::BinaryExpression:
                return TEXT("BinaryExpression");
            case SyntaxKind::CompoundAssignmentExpression:
                return TEXT("CompoundAssignmentExpression");
            case SyntaxKind::ParenthesizedExpression:
                return TEXT("ParenthesizedExpression");
            case SyntaxKind::AssignmentExpression:
                return TEXT("AssignmentExpression");
            case SyntaxKind::CallExpression:
                return TEXT("CallExpression");
            default:
                return TEXT("Unknown");
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
                auto Message =
                    Concat(TEXT("No binary operator for assignment operator: '"), GetText(Kind), TEXT("'"));
                fast_io::io::perrln(fast_io::mnp::code_cvt(Message));
#endif
                std::unreachable();
        }
    }
} // namespace Mamba