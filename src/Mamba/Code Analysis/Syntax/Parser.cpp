#include "Parser.h"
#include "MambaCore.h"
#include "SyntaxToken.h"
#include <memory>

MAMBA_NAMESPACE_BEGIN

std::shared_ptr<class SyntaxToken> Parser::Peek(const std::size_t Offset) noexcept
{
    const auto Index = Position + Offset;
    if (Index >= Tokens.size())
    {
        return Tokens.back();
    }

    return Tokens[Index];
}

std::shared_ptr<class SyntaxToken> Parser::Current() noexcept
{
    return Peek(0);
}

std::shared_ptr<class SyntaxToken> Parser::NextToken() noexcept
{
    const auto Current = this->Current();
    ++Position;
    return Current;
}

std::shared_ptr<class SyntaxToken> Parser::MatchToken(const SyntaxKind Kind) noexcept
{
    if (Current()->Kind() == Kind)
    {
        return NextToken();
    }

    Diagnostics.ReportUnexpectedToken(Current()->Location(), Current()->Kind(), Kind);
    return std::make_shared<SyntaxToken>(SyntaxTree, Kind, Current()->Position, std::make_shared<String>(), nullptr);
}

MAMBA_NAMESPACE_END