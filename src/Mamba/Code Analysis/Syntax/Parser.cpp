#include "Parser.h"

#include <memory>

#include "AssignmentExpressionSyntax.h"
#include "BinaryExpressionSyntax.h"
#include "BlockStatementSyntax.h"
#include "BreakStatementSyntax.h"
#include "CallExpressionSyntax.h"
#include "CompilationUnitSyntax.h"
#include "ContinueStatementSyntax.h"
#include "DoWhileStatementSyntax.h"
#include "ElseClauseSyntax.h"
#include "ExpressionStatementSyntax.h"
#include "ExpressionSyntax.h"
#include "fast_io.h"
#include "ForStatementSyntax.h"
#include "FunctionDeclarationSyntax.h"
#include "GlobalStatementSyntax.h"
#include "IfStatementSyntax.h"
#include "Lexer.h"
#include "Literal.h"
#include "LiteralExpressionSyntax.h"
#include "MambaCore.h"
#include "NameExpressionSyntax.h"
#include "ParameterSyntax.h"
#include "ParenthesizedExpressionSyntax.h"
#include "ReturnStatementSyntax.h"
#include "StatementSyntax.h"
#include "SyntaxFacts.h"
#include "SyntaxKind.h"
#include "SyntaxNode.h"
#include "SyntaxToken.h"
#include "SyntaxTree.h"
#include "TypeClauseSyntax.h"
#include "UnaryExpressionSyntax.h"
#include "VariableDeclarationSyntax.h"
#include "WhileStatementSyntax.h"

namespace Mamba
{
    Parser::Parser(const std::shared_ptr<const class SyntaxTree> SyntaxTree) noexcept :
        SyntaxTree(SyntaxTree), Text(SyntaxTree->Text), Position()
    {
        auto Lexer = ::Mamba::Lexer(SyntaxTree);
        auto Token = std::shared_ptr<const SyntaxToken>();
        do
        {
            Token = Lexer.Lex();

            if (Token->Kind() != SyntaxKind::BadToken && Token->Kind() != SyntaxKind::WhitespaceToken)
            {
                Tokens.emplace_back(Token);
            }
        } while (Token->Kind() != SyntaxKind::EndOfFileToken);
        Diagnostics = std::move(Lexer.Diagnostics);
    }

    std::shared_ptr<const SyntaxToken> Parser::Peek(const std::size_t Offset) noexcept
    {
        const auto Index = Position + Offset;
        if (Index >= Tokens.size())
        {
            return Tokens.back();
        }

        return Tokens[Index];
    }

    std::shared_ptr<const SyntaxToken> Parser::Current() noexcept
    {
        return Peek(0);
    }

    std::shared_ptr<const SyntaxToken> Parser::NextToken() noexcept
    {
        const auto Current = this->Current();
        ++Position;
        return Current;
    }

    std::shared_ptr<const SyntaxToken> Parser::MatchToken(const SyntaxKind Kind) noexcept
    {
        if (Current()->Kind() == Kind)
        {
            return NextToken();
        }

        Diagnostics.ReportUnexpectedToken(Current()->Location(), Current()->Kind(), Kind);
        return std::make_shared<SyntaxToken>(
            SyntaxTree,
            Kind,
            Current()->Position,
            std::make_shared<String>(),
            nullptr
        );
    }

    std::shared_ptr<const CompilationUnitSyntax> Parser::ParseCompilationUnit() noexcept
    {
        const auto Members = ParseMembers();
        const auto EndOfFileToken = MatchToken(SyntaxKind::EndOfFileToken);
        return std::make_shared<CompilationUnitSyntax>(SyntaxTree, std::move(Members), EndOfFileToken);
    }

    std::vector<std::shared_ptr<const MemberSyntax>> Parser::ParseMembers() noexcept
    {
        auto Members = std::vector<std::shared_ptr<const MemberSyntax>>();

        while (Current()->Kind() != SyntaxKind::EndOfFileToken)
        {
            const auto StartToken = Current();

            const auto Member = ParseMember();
            Members.emplace_back(Member);

            // If ParseMember() did not consume any tokens,
            // we need to skip the current token and continue
            // in order to avoid an infinite loop.
            //
            // We don't need to report an error, because we'll
            // already tried to parse an expression statement
            // and reported one.
            if (Current() == StartToken)
            {
                NextToken();
            }
        }

        return Members;
    }

    std::shared_ptr<const MemberSyntax> Parser::ParseMember() noexcept
    {
        if (Current()->Kind() == SyntaxKind::FunctionKeyword)
        {
            return ParseFunctionDeclaration();
        }

        return ParseGlobalStatement();
    }

    std::shared_ptr<const FunctionDeclarationSyntax> Parser::ParseFunctionDeclaration() noexcept
    {
        const auto FunctionKeyword = MatchToken(SyntaxKind::FunctionKeyword);
        const auto IdentifierToken = MatchToken(SyntaxKind::IdentifierToken);
        const auto OpenParenthesisToken = MatchToken(SyntaxKind::OpenParenthesisToken);
        const auto Parameters = ParseParameterList();
        const auto CloseParenthesisToken = MatchToken(SyntaxKind::CloseParenthesisToken);
        const auto Type = ParseOptionalTypeClause();
        const auto Body = ParseBlockStatement();

        return std::make_shared<FunctionDeclarationSyntax>(
            SyntaxTree,
            FunctionKeyword,
            IdentifierToken,
            OpenParenthesisToken,
            Parameters,
            CloseParenthesisToken,
            Type,
            Body
        );
    }

    std::shared_ptr<const SeperatedSyntaxList<std::shared_ptr<const SyntaxNode>>> Parser::ParseParameterList() noexcept
    {
        auto NodesAndSeperators = std::vector<std::shared_ptr<const SyntaxNode>>();

        auto ParseNextParameter = true;
        while (ParseNextParameter && Current()->Kind() != SyntaxKind::CloseParenthesisToken && Current()->Kind() != SyntaxKind::EndOfFileToken)
        {
            NodesAndSeperators.emplace_back(
                Hatcher([&]
                        { return std::dynamic_pointer_cast<const SyntaxNode>(ParseParameter()); })
            );

            if (Current()->Kind() == SyntaxKind::CommaToken)
            {
                NodesAndSeperators.emplace_back(Hatcher(
                    [&]
                    { return std::dynamic_pointer_cast<const SyntaxNode>(MatchToken(SyntaxKind::CommaToken)); }
                ));
            }
            else
            {
                ParseNextParameter = false;
            }
        }

        return std::make_shared<const SeperatedSyntaxList<std::shared_ptr<const SyntaxNode>>>(
            std::move(NodesAndSeperators)
        );
    }

    std::shared_ptr<const ParameterSyntax> Parser::ParseParameter() noexcept
    {
        const auto Identifier = MatchToken(SyntaxKind::IdentifierToken);
        const auto Type = ParseTypeClause();
        return std::make_shared<const ParameterSyntax>(SyntaxTree, Identifier, Type);
    }

    std::shared_ptr<const MemberSyntax> Parser::ParseGlobalStatement() noexcept
    {
        const auto Statement = ParseStatement();
        return std::make_shared<GlobalStatementSyntax>(SyntaxTree, Statement);
    }

    std::shared_ptr<const StatementSyntax> Parser::ParseStatement() noexcept
    {
        switch (Current()->Kind())
        {
            case SyntaxKind::OpenBraceToken:
                return ParseBlockStatement();
            case SyntaxKind::LetKeyword:
            case SyntaxKind::VarKeyword:
                return ParseVariableDeclaration();
            case SyntaxKind::IfKeyword:
                return ParseIfStatement();
            case SyntaxKind::WhileKeyword:
                return ParseWhileStatement();
            case SyntaxKind::DoKeyword:
                return ParseDoWhileStatement();
            case SyntaxKind::ForKeyword:
                return ParseForStatement();
            case SyntaxKind::BreakKeyword:
                return ParseBreakStatement();
            case SyntaxKind::ContinueKeyword:
                return ParseContinueStatement();
            case SyntaxKind::ReturnKeyword:
                return ParseReturnStatement();
            default:
                return ParseExpressionStatement();
        }
    }

    std::shared_ptr<const BlockStatementSyntax> Parser::ParseBlockStatement() noexcept
    {
        auto Statements = std::vector<std::shared_ptr<const StatementSyntax>>();

        const auto OpenBraceToken = MatchToken(SyntaxKind::OpenBraceToken);

        while (Current()->Kind() != SyntaxKind::EndOfFileToken && Current()->Kind() != SyntaxKind::CloseBraceToken)
        {
            const auto StartToken = Current();

            Statements.emplace_back(Hatcher([&]
                                            { return ParseStatement(); }));

            // If ParseStatement() did not consume any tokens,
            // we need to skip the current token and continue
            // in order to avoid an infinite loop.
            //
            // We don't need to report an error, because we'll
            // already tried to parse an expression statement
            // and reported one.
            if (Current() == StartToken)
            {
                NextToken();
            }
        }

        const auto CloseBraceToken = MatchToken(SyntaxKind::CloseBraceToken);
        return std::make_shared<const BlockStatementSyntax>(
            SyntaxTree,
            OpenBraceToken,
            std::move(Statements),
            CloseBraceToken
        );
    }

    std::shared_ptr<const StatementSyntax> Parser::ParseVariableDeclaration() noexcept
    {
        const auto Expected =
            Current()->Kind() == SyntaxKind::LetKeyword ? SyntaxKind::LetKeyword : SyntaxKind::VarKeyword;
        const auto Keyword = MatchToken(Expected);
        const auto Identifier = MatchToken(SyntaxKind::IdentifierToken);
        const auto TypeClause = ParseOptionalTypeClause();
        const auto Equals = MatchToken(SyntaxKind::EqualsToken);
        const auto Initializer = ParseExpression();
        return std::make_shared<VariableDeclarationSyntax>(
            SyntaxTree,
            Keyword,
            Identifier,
            TypeClause,
            Equals,
            Initializer
        );
    }

    NullableSharedPtr<const TypeClauseSyntax> Parser::ParseOptionalTypeClause() noexcept
    {
        if (Current()->Kind() != SyntaxKind::ColonToken)
        {
            return nullptr;
        }

        return ParseTypeClause();
    }

    std::shared_ptr<const TypeClauseSyntax> Parser::ParseTypeClause() noexcept
    {
        const auto ColonToken = MatchToken(SyntaxKind::ColonToken);
        const auto Identifier = MatchToken(SyntaxKind::IdentifierToken);
        return std::make_shared<const TypeClauseSyntax>(SyntaxTree, ColonToken, Identifier);
    }

    std::shared_ptr<const StatementSyntax> Parser::ParseIfStatement() noexcept
    {
        const auto Keyword = MatchToken(SyntaxKind::IfKeyword);
        const auto Condition = ParseExpression();
        const auto OpenParenthesisToken = Current()->Kind() == SyntaxKind::OpenParenthesisToken
                                              ? MatchToken(SyntaxKind::OpenParenthesisToken)
                                              : nullptr;
        const auto Statement = ParseStatement();
        const auto CloseParenthesisToken =
            OpenParenthesisToken ? MatchToken(SyntaxKind::CloseParenthesisToken) : nullptr;
        const auto ElseClause = ParseOptionalElseClause();
        return std::make_shared<const IfStatementSyntax>(
            SyntaxTree,
            Keyword,
            OpenParenthesisToken,
            Condition,
            CloseParenthesisToken,
            Statement,
            ElseClause
        );
    }

    NullableSharedPtr<const ElseClauseSyntax> Parser::ParseOptionalElseClause() noexcept
    {
        if (Current()->Kind() != SyntaxKind::ElseKeyword)
        {
            return nullptr;
        }

        const auto Keyword = MatchToken(SyntaxKind::ElseKeyword);
        const auto Statement = ParseStatement();
        return std::make_shared<ElseClauseSyntax>(SyntaxTree, Keyword, Statement);
    }

    std::shared_ptr<const StatementSyntax> Parser::ParseWhileStatement() noexcept
    {
        const auto Keyword = MatchToken(SyntaxKind::WhileKeyword);
        const auto Condition = ParseExpression();
        const auto Body = ParseStatement();
        return std::make_shared<WhileStatementSyntax>(SyntaxTree, Keyword, Condition, Body);
    }

    std::shared_ptr<const StatementSyntax> Parser::ParseDoWhileStatement() noexcept
    {
        const auto DoKeyword = MatchToken(SyntaxKind::DoKeyword);
        const auto Body = ParseStatement();
        const auto WhileKeyword = MatchToken(SyntaxKind::WhileKeyword);
        const auto Condition = ParseExpression();
        return std::make_shared<DoWhileStatementSyntax>(SyntaxTree, DoKeyword, Body, WhileKeyword, Condition);
    }

    std::shared_ptr<const StatementSyntax> Parser::ParseForStatement() noexcept
    {
        const auto Keyword = MatchToken(SyntaxKind::ForKeyword);
        const auto OpenParenthesisToken = MatchToken(SyntaxKind::OpenParenthesisToken);
        const auto InitStatement = ParseStatement();
        const auto InitStatementColonToken = MatchToken(SyntaxKind::ColonToken);
        const auto Condition = ParseExpression();
        const auto ConditionColonToken = MatchToken(SyntaxKind::ColonToken);
        const auto Expression = ParseExpression();
        const auto CloseParenthesisToken = MatchToken(SyntaxKind::CloseParenthesisToken);
        const auto Body = ParseStatement();
        return std::make_shared<ForStatementSyntax>(
            SyntaxTree,
            Keyword,
            OpenParenthesisToken,
            InitStatement,
            InitStatementColonToken,
            Condition,
            ConditionColonToken,
            Expression,
            CloseParenthesisToken,
            Body
        );
    }

    std::shared_ptr<const StatementSyntax> Parser::ParseBreakStatement() noexcept
    {
        const auto Keyword = MatchToken(SyntaxKind::BreakKeyword);
        return std::make_shared<BreakStatementSyntax>(SyntaxTree, Keyword);
    }

    std::shared_ptr<const StatementSyntax> Parser::ParseContinueStatement() noexcept
    {
        const auto Keyword = MatchToken(SyntaxKind::ContinueKeyword);
        return std::make_shared<ContinueStatementSyntax>(SyntaxTree, Keyword);
    }

    std::shared_ptr<const StatementSyntax> Parser::ParseReturnStatement() noexcept
    {
        const auto Keyword = MatchToken(SyntaxKind::ReturnKeyword);
        const auto KeywordLine = Text->GetLineIndex(Keyword->Span().Start);
        const auto CurrentLine = Text->GetLineIndex(Current()->Span().Start);
        const auto IsEOF = Current()->Kind() == SyntaxKind::EndOfFileToken;
        const auto SameLine = !IsEOF && KeywordLine == CurrentLine;
        const auto Expression = SameLine ? ParseExpression() : nullptr;
        return std::make_shared<ReturnStatementSyntax>(SyntaxTree, Keyword, Expression);
    }

    std::shared_ptr<const ExpressionStatementSyntax> Parser::ParseExpressionStatement() noexcept
    {
        const auto Expression = ParseExpression();
        return std::make_shared<const ExpressionStatementSyntax>(SyntaxTree, Expression);
    }

    std::shared_ptr<const ExpressionSyntax> Parser::ParseExpression() noexcept
    {
        return ParseAssignmentExpression();
    }

    std::shared_ptr<const ExpressionSyntax> Parser::ParseAssignmentExpression() noexcept
    {
        if (Current()->Kind() != SyntaxKind::IdentifierToken)
        {
            return ParseBinaryExpression();
        }

        switch (Peek(1)->Kind())
        {
            case SyntaxKind::PlusEqualsToken:
            case SyntaxKind::MinusEqualsToken:
            case SyntaxKind::StarEqualsToken:
            case SyntaxKind::SlashEqualsToken:
            case SyntaxKind::AmpersandEqualsToken:
            case SyntaxKind::PipeEqualsToken:
            case SyntaxKind::HatEqualsToken:
            case SyntaxKind::EqualsToken:
            {
                const auto IdentifierToken = NextToken();
                const auto OperatorToken = NextToken();
                const auto Right = ParseAssignmentExpression();
                return std::make_shared<AssignmentExpressionSyntax>(SyntaxTree, IdentifierToken, OperatorToken, Right);
            }
            default:
                break;
        }

        return ParseBinaryExpression();
    }

    std::shared_ptr<const ExpressionSyntax> Parser::ParseBinaryExpression(const std::size_t ParentPrecedence) noexcept
    {
        auto Left = std::shared_ptr<const ExpressionSyntax>();
        const auto UnaryOperatorPrecedence = SyntaxFacts::GetUnaryOperatorPrecedence(Current()->Kind());
        if (UnaryOperatorPrecedence != 0 && UnaryOperatorPrecedence >= ParentPrecedence)
        {
            const auto OperatorToken = NextToken();
            const auto Operand = ParseBinaryExpression(UnaryOperatorPrecedence);
            Left = std::make_shared<const UnaryExpressionSyntax>(SyntaxTree, OperatorToken, Operand);
        }
        else
        {
            Left = ParsePrimaryExpression();
        }

        while (true)
        {
            const auto Precedence = SyntaxFacts::GetBinaryOperatorPrecedence(Current()->Kind());
            if (Precedence == 0 || Precedence <= ParentPrecedence)
            {
                break;
            }

            const auto OperatorToken = NextToken();
            const auto Right = ParseBinaryExpression(Precedence);
            Left = std::make_shared<const BinaryExpressionSyntax>(SyntaxTree, Left, OperatorToken, Right);
        }

        return Left;
    }

    std::shared_ptr<const class ExpressionSyntax> Parser::ParsePrimaryExpression() noexcept
    {
        switch (Current()->Kind())
        {
            case SyntaxKind::OpenParenthesisToken:
                return ParseParenthesizedExpression();

            case SyntaxKind::FalseKeyword:
            case SyntaxKind::TrueKeyword:
                return ParseBooleanLiteral();

            case SyntaxKind::NumberToken:
                return ParseNumericLiteral();

            case SyntaxKind::StringToken:
                return ParseStringLiteral();

            case SyntaxKind::IdentifierToken:
            default:
                return ParseNameOrCallExpression();
        }
    }

    std::shared_ptr<const class ExpressionSyntax> Parser::ParseParenthesizedExpression() noexcept
    {
        const auto Left = MatchToken(SyntaxKind::OpenParenthesisToken);
        const auto Expression = ParseExpression();
        const auto Right = MatchToken(SyntaxKind::CloseParenthesisToken);
        return std::make_shared<const ParenthesizedExpressionSyntax>(SyntaxTree, Left, Expression, Right);
    }

    std::shared_ptr<const class ExpressionSyntax> Parser::ParseBooleanLiteral() noexcept
    {
        const auto IsTrue = Current()->Kind() == SyntaxKind::TrueKeyword;
        const auto KeywordToken = MatchToken(IsTrue ? SyntaxKind::TrueKeyword : SyntaxKind::FalseKeyword);
        return std::make_shared<const LiteralExpressionSyntax>(
            SyntaxTree,
            KeywordToken,
            std::make_shared<const Literal>(IsTrue)
        );
    }

    std::shared_ptr<const class ExpressionSyntax> Parser::ParseNumericLiteral() noexcept
    {
        const auto NumberToken = MatchToken(SyntaxKind::NumberToken);
        return std::make_shared<const LiteralExpressionSyntax>(SyntaxTree, NumberToken);
    }

    std::shared_ptr<const class ExpressionSyntax> Parser::ParseStringLiteral() noexcept
    {
        const auto StringToken = MatchToken(SyntaxKind::StringToken);
        return std::make_shared<const LiteralExpressionSyntax>(SyntaxTree, StringToken);
    }

    std::shared_ptr<const class ExpressionSyntax> Parser::ParseNameOrCallExpression() noexcept
    {
        if (Peek(0)->Kind() == SyntaxKind::IdentifierToken && Peek(1)->Kind() == SyntaxKind::OpenParenthesisToken)
        {
            return ParseCallExpression();
        }

        return ParseNameExpression();
    }

    std::shared_ptr<const class ExpressionSyntax> Parser::ParseCallExpression() noexcept
    {
        const auto Identifier = MatchToken(SyntaxKind::IdentifierToken);
        const auto OpenParenthesisToken = MatchToken(SyntaxKind::OpenParenthesisToken);
        const auto Arguments = ParseArguments();
        const auto CloseParenthesisToken = MatchToken(SyntaxKind::CloseParenthesisToken);
        return std::make_shared<const CallExpressionSyntax>(
            SyntaxTree,
            Identifier,
            OpenParenthesisToken,
            Arguments,
            CloseParenthesisToken
        );
    }

    std::shared_ptr<const SeperatedSyntaxList<std::shared_ptr<const class SyntaxNode>>>
        Parser::ParseArguments() noexcept
    {
        auto NodesAndSeperators = std::vector<std::shared_ptr<const SyntaxNode>>();

        auto ParseNextArgument = true;
        while (ParseNextArgument && Current()->Kind() != SyntaxKind::CloseParenthesisToken && Current()->Kind() != SyntaxKind::EndOfFileToken)
        {
            NodesAndSeperators.emplace_back(
                Hatcher([&]
                        { return std::dynamic_pointer_cast<const SyntaxNode>(ParseExpression()); })
            );

            if (Current()->Kind() == SyntaxKind::CommaToken)
            {
                NodesAndSeperators.emplace_back(Hatcher(
                    [&]
                    { return std::dynamic_pointer_cast<const SyntaxNode>(MatchToken(SyntaxKind::CommaToken)); }
                ));
            }
            else
            {
                ParseNextArgument = false;
            }
        }

        return std::make_shared<SeperatedSyntaxList<std::shared_ptr<const SyntaxNode>>>(std::move(NodesAndSeperators));
    }

    std::shared_ptr<const class ExpressionSyntax> Parser::ParseNameExpression() noexcept
    {
        const auto IdentifierToken = MatchToken(SyntaxKind::IdentifierToken);
        return std::make_shared<NameExpressionSyntax>(SyntaxTree, IdentifierToken);
    }
} // namespace Mamba