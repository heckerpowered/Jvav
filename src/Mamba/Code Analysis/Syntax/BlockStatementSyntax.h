#pragma once

#include "StatementSyntax.h"
#include "SyntaxKind.h"

#include <memory>
#include <vector>

namespace Mamba
{
    class BlockStatementSyntax : public StatementSyntax
    {
    public:
        using Super = StatementSyntax;

        [[nodiscard]] BlockStatementSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                           const std::shared_ptr<const class SyntaxToken> OpenBraceToken,
                                           const std::vector<std::shared_ptr<const class StatementSyntax>>& Statements,
                                           const std::shared_ptr<const class SyntaxToken> CloseBraceToken) noexcept;

        [[nodiscard]] BlockStatementSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                           const std::shared_ptr<const class SyntaxToken> OpenBraceToken,
                                           std::vector<std::shared_ptr<const class StatementSyntax>>&& Statements,
                                           const std::shared_ptr<const class SyntaxToken> CloseBraceToken) noexcept;

        virtual SyntaxKind Kind() const noexcept override;
        virtual std::vector<std::shared_ptr<const class SyntaxNode>> Children() const noexcept override;

        const std::shared_ptr<const class SyntaxToken> OpenBraceToken;
        const std::vector<std::shared_ptr<const class StatementSyntax>> Statements;
        const std::shared_ptr<const class SyntaxToken> CloseBraceToken;
    };
} // namespace Mamba