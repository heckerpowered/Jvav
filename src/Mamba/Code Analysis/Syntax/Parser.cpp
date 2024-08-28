#include "Parser.h"

#include "MambaCore.h"
#include "SyntaxFacts.h"
#include "SyntaxTree.h"
#include "UnaryExpressionSyntax.h"

using namespace Mamba;

Parser::Parser(const class SyntaxTree* SyntaxTree, std::vector<SyntaxToken>&& Tokens) noexcept :
    SyntaxTree(SyntaxTree), Tokens(std::move(Tokens)), Position(0)
{
    if (this->Tokens.empty())
    {
        InternalCompilerError(std::source_location::current(), "No tokens provided to parser.");
    }
    else if (this->Tokens.back().Kind() != SyntaxKind::EndOfFileToken)
    {
        InternalCompilerError(std::source_location::current(), "Tokens do not end with EndOfFileToken.");
    }
}

Parser::Parser(Parser&& Other) noexcept :
    SyntaxTree(Other.SyntaxTree),
    Tokens(std::move(Other.Tokens)),
    Position(Other.Position)
{
    Other.SyntaxTree = {};
    Other.Position = 0;
}

const SyntaxToken* Parser::Peek(std::size_t Offset) noexcept
{
    auto Index = Position + Offset;
    if (Index >= Tokens.size())
    {
        return &Tokens.back();
    }

    return &Tokens[Index];
}

const SyntaxToken* Parser::Current() noexcept
{
    return Peek(0);
}

const SyntaxToken* Parser::NextToken() noexcept
{
    const auto Current = this->Current();
    ++Position;
    return Current;
}

const SyntaxToken* Parser::MatchToken(SyntaxKind Kind) noexcept
{
    if (Current()->Kind() == Kind)
    {
        return NextToken();
    }

    Diagnostics.ReportUnexpectedToken(Current()->Location(), Current()->Kind(), Kind);
    return Peek(1);
}

CompilationUnitSyntax* Parser::ParseCompilationUnit() noexcept
{
    auto Members = ParseMembers();
    auto EndOfFileToken = MatchToken(SyntaxKind::EndOfFileToken);
    return new CompilationUnitSyntax(SyntaxTree, std::move(Members), EndOfFileToken);
}

std::vector<MemberSyntax*> Parser::ParseMembers() noexcept
{
    auto Members = std::vector<MemberSyntax*>();

    while (Current()->Kind() != SyntaxKind::EndOfFileToken)
    {
        auto Member = ParseMember();
        Members.emplace_back(Member);
    }

    return Members;
}

MemberSyntax* Parser::ParseMember() noexcept
{
    auto StartToken = Current();
    auto Result = static_cast<MemberSyntax*>(nullptr);
    if (Current()->Kind() == SyntaxKind::FunctionKeyword)
    {
        Result = ParseFunctionDeclaration();
    }
    else
    {
        Result = ParseGlobalStatement();
    }

    if (Current() == StartToken)
    {
        NextToken();
    }

    return Result;
}

FunctionDeclarationSyntax* Parser::ParseFunctionDeclaration() noexcept
{
    auto FunctionKeyword = MatchToken(SyntaxKind::FunctionKeyword);
    auto IdentifierToken = MatchToken(SyntaxKind::IdentifierToken);
    auto OpenParenthesisToken = MatchToken(SyntaxKind::OpenParenthesisToken);
    auto Parameters = ParseParameterList();
    auto CloseParenthesisToken = MatchToken(SyntaxKind::CloseParenthesisToken);
    auto Type = ParseOptionalTypeClause();
    auto Body = ParseBlockStatement();

    return new FunctionDeclarationSyntax(
        SyntaxTree,
        FunctionKeyword,
        IdentifierToken,
        OpenParenthesisToken,
        std::move(Parameters),
        CloseParenthesisToken,
        Type,
        Body
    );
}

SeperatedSyntaxList<ParameterSyntax*> Parser::ParseParameterList() noexcept
{
    auto NodesAndSeperators = std::vector<const SyntaxNode*>();

    auto ParseNextParameter = true;
    while (ParseNextParameter &&
           Current()->Kind() != SyntaxKind::CloseParenthesisToken &&
           Current()->Kind() != SyntaxKind::EndOfFileToken)
    {
        auto Parameter = ParseParameter();
        NodesAndSeperators.emplace_back(Parameter);

        if (Current()->Kind() == SyntaxKind::CommaToken)
        {
            auto Seperator = MatchToken(SyntaxKind::CommaToken);
            NodesAndSeperators.emplace_back(Seperator);
        }
        else
        {
            ParseNextParameter = false;
        }
    }

    return SeperatedSyntaxList<ParameterSyntax*>(std::move(NodesAndSeperators));
}

ParameterSyntax* Parser::ParseParameter() noexcept
{
    auto Identifier = MatchToken(SyntaxKind::IdentifierToken);
    auto Type = ParseTypeClause();
    return new ParameterSyntax(SyntaxTree, Identifier, Type);
}

MemberSyntax* Parser::ParseGlobalStatement() noexcept
{
    const auto Statement = ParseStatement();
    return new GlobalStatementSyntax(SyntaxTree, Statement);
}

StatementSyntax* Parser::ParseStatement() noexcept
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

BlockStatementSyntax* Parser::ParseBlockStatement() noexcept
{
    auto Statements = std::vector<StatementSyntax*>();

    auto OpenBraceToken = MatchToken(SyntaxKind::OpenBraceToken);

    while (Current()->Kind() != SyntaxKind::EndOfFileToken &&
           Current()->Kind() != SyntaxKind::CloseBraceToken)
    {
        auto StartToken = Current();

        auto Statement = ParseStatement();
        Statements.emplace_back(Statement);

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

    auto CloseBraceToken = MatchToken(SyntaxKind::CloseBraceToken);
    return new BlockStatementSyntax(SyntaxTree, OpenBraceToken, std::move(Statements), CloseBraceToken);
}

VariableDeclarationSyntax* Parser::ParseVariableDeclaration() noexcept
{
    auto Expected = Current()->Kind() == SyntaxKind::LetKeyword ? SyntaxKind::LetKeyword : SyntaxKind::VarKeyword;
    auto Keyword = MatchToken(Expected);
    auto Identifier = MatchToken(SyntaxKind::IdentifierToken);
    auto TypeClause = ParseOptionalTypeClause();
    auto Equals = MatchToken(SyntaxKind::EqualsToken);
    auto Initializer = ParseExpression();
    return new VariableDeclarationSyntax(SyntaxTree, Keyword, Identifier, TypeClause, Equals, Initializer);
}

NullablePointer<TypeClauseSyntax> Parser::ParseOptionalTypeClause() noexcept
{
    if (Current()->Kind() != SyntaxKind::ColonToken)
    {
        return {};
    }

    return ParseTypeClause();
}

TypeClauseSyntax* Parser::ParseTypeClause() noexcept
{
    auto ColonToken = MatchToken(SyntaxKind::ColonToken);
    auto Identifier = MatchToken(SyntaxKind::IdentifierToken);
    return new TypeClauseSyntax(SyntaxTree, ColonToken, Identifier);
}

IfStatementSyntax* Parser::ParseIfStatement() noexcept
{
    auto Keyword = MatchToken(SyntaxKind::IfKeyword);
    auto Condition = ParseExpression();
    auto Statement = ParseStatement();
    auto ElseClause = ParseOptionalElseClause();
    return new IfStatementSyntax(SyntaxTree, Keyword, Condition, Statement, ElseClause);
}

NullablePointer<ElseClauseSyntax> Parser::ParseOptionalElseClause() noexcept
{
    if (Current()->Kind() != SyntaxKind::ElseKeyword)
    {
        return nullptr;
    }

    auto Keyword = MatchToken(SyntaxKind::ElseKeyword);
    auto Statement = ParseStatement();
    return new ElseClauseSyntax(SyntaxTree, Keyword, Statement);
}

WhileStatementSyntax* Parser::ParseWhileStatement() noexcept
{
    auto Keyword = MatchToken(SyntaxKind::WhileKeyword);
    auto Condition = ParseExpression();
    auto Body = ParseStatement();
    return new WhileStatementSyntax(SyntaxTree, Keyword, Condition, Body);
}

DoWhileStatementSyntax* Parser::ParseDoWhileStatement() noexcept
{
    auto DoKeyword = MatchToken(SyntaxKind::DoKeyword);
    auto Body = ParseStatement();
    auto WhileKeyword = MatchToken(SyntaxKind::WhileKeyword);
    auto Condition = ParseExpression();
    return new DoWhileStatementSyntax(SyntaxTree, DoKeyword, Body, WhileKeyword, Condition);
}

ForStatementSyntax* Parser::ParseForStatement() noexcept
{
    auto Keyword = MatchToken(SyntaxKind::ForKeyword);
    auto OpenParenthesisToken = MatchToken(SyntaxKind::OpenParenthesisToken);
    auto InitStatement = ParseStatement();
    auto InitStatementColonToken = MatchToken(SyntaxKind::ColonToken);
    auto Condition = ParseExpression();
    auto ConditionColonToken = MatchToken(SyntaxKind::ColonToken);
    auto Expression = ParseExpression();
    auto CloseParenthesisToken = MatchToken(SyntaxKind::CloseParenthesisToken);
    auto Body = ParseStatement();
    return new ForStatementSyntax(
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

BreakStatementSyntax* Parser::ParseBreakStatement() noexcept
{
    auto Keyword = MatchToken(SyntaxKind::BreakKeyword);
    return new BreakStatementSyntax(SyntaxTree, Keyword);
}

ContinueStatementSyntax* Parser::ParseContinueStatement() noexcept
{
    auto Keyword = MatchToken(SyntaxKind::ContinueKeyword);
    return new ContinueStatementSyntax(SyntaxTree, Keyword);
}

ReturnStatementSyntax* Parser::ParseReturnStatement() noexcept
{
    auto Text = SyntaxTree->Text();

    auto Keyword = MatchToken(SyntaxKind::ReturnKeyword);
    auto KeywordLine = Text.LineIndex(Text.RelativeBegin(Keyword->Text()));
    auto CurrentLine = Text.LineIndex(Text.RelativeBegin(Current()->Text()));
    auto IsEOF = Current()->Kind() == SyntaxKind::EndOfFileToken;
    auto SameLine = !IsEOF && KeywordLine == CurrentLine;
    auto Expression = SameLine ? ParseExpression() : nullptr;
    return new ReturnStatementSyntax(SyntaxTree, Keyword, Expression);
}

ExpressionStatementSyntax* Parser::ParseExpressionStatement() noexcept
{
    auto Expression = ParseExpression();
    return new ExpressionStatementSyntax(SyntaxTree, Expression);
}

ExpressionSyntax* Parser::ParseExpression() noexcept
{
    return ParseAssignmentExpression();
}

ExpressionSyntax* Parser::ParseAssignmentExpression() noexcept
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
            return new AssignmentExpressionSyntax(SyntaxTree, IdentifierToken, OperatorToken, Right);
        }
        default:
            break;
    }

    return ParseBinaryExpression();
}

ExpressionSyntax* Parser::ParseUnaryOrPrimaryExpression(std::size_t ParentPrecedence) noexcept
{
    auto UnaryOperatorPrecedence = SyntaxFacts::GetUnaryOperatorPrecedence(Current()->Kind());
    if (UnaryOperatorPrecedence != 0 && UnaryOperatorPrecedence >= ParentPrecedence)
    {
        auto OperatorToken = NextToken();
        auto Operand = ParseBinaryExpression(UnaryOperatorPrecedence);
        return new UnaryExpressionSyntax(SyntaxTree, OperatorToken, Operand);
    }
    else
    {
        return ParsePrimaryExpression();
    }
}

ExpressionSyntax* Parser::ParseBinaryExpression(std::size_t ParentPrecedence) noexcept
{
    auto Left = ParseUnaryOrPrimaryExpression();
    while (true)
    {
        auto Precedence = SyntaxFacts::GetBinaryOperatorPrecedence(Current()->Kind());
        if (Precedence == 0 || Precedence <= ParentPrecedence)
        {
            break;
        }

        auto OperatorToken = NextToken();
        auto Right = ParseBinaryExpression(Precedence);
        Left = new BinaryExpressionSyntax(SyntaxTree, Left, OperatorToken, Right);
    }

    return Left;
}

ExpressionSyntax* Parser::ParsePrimaryExpression() noexcept
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

ParenthesizedExpressionSyntax* Parser::ParseParenthesizedExpression() noexcept
{
    auto Left = MatchToken(SyntaxKind::OpenParenthesisToken);
    auto Expression = ParseExpression();
    auto Right = MatchToken(SyntaxKind::CloseParenthesisToken);
    return new ParenthesizedExpressionSyntax(SyntaxTree, Left, Expression, Right);
}

LiteralExpressionSyntax* Parser::ParseBooleanLiteral() noexcept
{
    auto IsTrue = Current()->Kind() == SyntaxKind::TrueKeyword;
    auto KeywordToken = MatchToken(IsTrue ? SyntaxKind::TrueKeyword : SyntaxKind::FalseKeyword);
    return new LiteralExpressionSyntax(SyntaxTree, KeywordToken, IsTrue);
}

LiteralExpressionSyntax* Parser::ParseNumericLiteral() noexcept
{
    auto NumberToken = MatchToken(SyntaxKind::NumberToken);
    return new LiteralExpressionSyntax(SyntaxTree, NumberToken);
}

LiteralExpressionSyntax* Parser::ParseStringLiteral() noexcept
{
    auto StringToken = MatchToken(SyntaxKind::StringToken);
    return new LiteralExpressionSyntax(SyntaxTree, StringToken);
}

ExpressionSyntax* Parser::ParseNameOrCallExpression() noexcept
{
    if (Peek(0)->Kind() == SyntaxKind::IdentifierToken && Peek(1)->Kind() == SyntaxKind::OpenParenthesisToken)
    {
        return ParseCallExpression();
    }

    return ParseNameExpression();
}

CallExpressionSyntax* Parser::ParseCallExpression() noexcept
{
    auto Identifier = MatchToken(SyntaxKind::IdentifierToken);
    auto OpenParenthesisToken = MatchToken(SyntaxKind::OpenParenthesisToken);
    auto Arguments = ParseArguments();
    auto CloseParenthesisToken = MatchToken(SyntaxKind::CloseParenthesisToken);
    return new CallExpressionSyntax(
        SyntaxTree,
        Identifier,
        OpenParenthesisToken,
        std::move(Arguments),
        CloseParenthesisToken
    );
}

SeperatedSyntaxList<ExpressionSyntax*> Parser::ParseArguments() noexcept
{
    auto NodesAndSeperators = std::vector<const SyntaxNode*>();

    auto ParseNextArgument = true;
    while (ParseNextArgument && Current()->Kind() != SyntaxKind::CloseParenthesisToken && Current()->Kind() != SyntaxKind::EndOfFileToken)
    {
        auto Expression = ParseExpression();
        NodesAndSeperators.emplace_back(Expression);

        if (Current()->Kind() == SyntaxKind::CommaToken)
        {
            auto Seperator = MatchToken(SyntaxKind::CommaToken);
            NodesAndSeperators.emplace_back(Seperator);
        }
        else
        {
            ParseNextArgument = false;
        }
    }

    return SeperatedSyntaxList<ExpressionSyntax*>(std::move(NodesAndSeperators));
}

NameExpressionSyntax* Parser::ParseNameExpression() noexcept
{
    const auto IdentifierToken = MatchToken(SyntaxKind::IdentifierToken);
    return new NameExpressionSyntax(SyntaxTree, IdentifierToken);
}