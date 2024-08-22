#include "BlockStatementSyntax.h"

using namespace Mamba;

BlockStatementSyntax::BlockStatementSyntax(
    const class SyntaxTree* SyntaxTree,
    const SyntaxToken* OpenBraceToken,
    std::vector<StatementSyntax*>&& Statements,
    const SyntaxToken* CloseBraceToken
) noexcept :
    Super(SyntaxTree),
    OpenBraceToken(OpenBraceToken),
    Statements(std::move(Statements)),
    CloseBraceToken(CloseBraceToken)
{
}

BlockStatementSyntax::~BlockStatementSyntax() noexcept
{
    for (auto&& Statement : Statements)
    {
        delete Statement;
    }
}

SyntaxKind BlockStatementSyntax::Kind() const noexcept
{
    return SyntaxKind::BlockStatement;
}

std::vector<const SyntaxNode*> BlockStatementSyntax::Children() const noexcept
{
    auto Result = std::vector<const SyntaxNode*>();
    Result.reserve(Statements.size() + 2);

    Result.emplace_back(OpenBraceToken);

#if defined(__cpp_lib_containers_ranges) && __cpp_lib_containers_ranges >= 202202L
    Result.append_range(Statements);
#else
    for (auto Statement : Statements)
    {
        Result.emplace_back(Statement);
    }
#endif

    Result.emplace_back(CloseBraceToken);

    return Result;
}