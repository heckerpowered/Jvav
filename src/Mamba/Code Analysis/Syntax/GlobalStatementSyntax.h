#pragma once

#include "MemberSyntax.h"
#include "SyntaxTree.h"
#include <memory>

namespace Mamba
{
    class GlobalStatementSyntax : public MemberSyntax
    {
    public:
        using Super = MemberSyntax;

        [[nodiscard]] GlobalStatementSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                            const std::shared_ptr<const class StatementSyntax> Statement) noexcept;

        SyntaxKind Kind() const noexcept override;
        std::vector<std::shared_ptr<const class SyntaxNode>> Children() const noexcept override;

        const std::shared_ptr<const class StatementSyntax> Statement;
    };
} // namespace Mamba