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

std::size_t BlockStatementSyntax::ChildrenCount() const noexcept
{
    return Statements.size() + 2;
}

const SyntaxNode* BlockStatementSyntax::ChildAt(std::size_t Index) const noexcept
{
    if (Index == 0)
    {
        return OpenBraceToken;
    }
    else if (Index < Statements.size() + 1)
    {
        return Statements[Index - 1];
    }
    else if (Index == Statements.size() + 1)
    {
        return CloseBraceToken;
    }

    ReportChildrenAccessOutOfBounds(Index);
}