#include "BlockStatementSyntax.h"

#include "SyntaxNode.h"
#include "SyntaxToken.h"

#include <memory>

namespace Mamba
{
    BlockStatementSyntax::BlockStatementSyntax(
        const std::shared_ptr<const class SyntaxTree> SyntaxTree,
        const std::shared_ptr<const class SyntaxToken> OpenBraceToken,
        const std::vector<std::shared_ptr<const class StatementSyntax>>& Statements,
        const std::shared_ptr<const class SyntaxToken> CloseBraceToken) noexcept :
        Super(SyntaxTree), OpenBraceToken(OpenBraceToken), Statements(Statements), CloseBraceToken(CloseBraceToken)
    {
    }

    BlockStatementSyntax::BlockStatementSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                               const std::shared_ptr<const class SyntaxToken> OpenBraceToken,
                                               std::vector<std::shared_ptr<const class StatementSyntax>>&& Statements,
                                               const std::shared_ptr<const class SyntaxToken> CloseBraceToken) noexcept
        :
        Super(SyntaxTree),
        OpenBraceToken(OpenBraceToken),
        Statements(std::move(Statements)),
        CloseBraceToken(CloseBraceToken)
    {
    }

    SyntaxKind BlockStatementSyntax::Kind() const noexcept
    {
        return SyntaxKind::BlockStatement;
    }

    std::vector<std::shared_ptr<const class SyntaxNode>> BlockStatementSyntax::Children() const noexcept
    {
        auto Result = std::vector<std::shared_ptr<const class SyntaxNode>>();
        Result.reserve(Statements.size() + 2);

        Result.emplace_back(OpenBraceToken);
        Result.append_range(Statements);
        Result.emplace_back(CloseBraceToken);

        return Result;
    }
} // namespace Mamba