#pragma once

#include "ExpressionSyntax.h"
#include "SyntaxKind.h"
#include "SyntaxToken.h"
#include <memory>
#include <vector>

namespace Mamba
{
    class AssignmentExpressionSyntax : public ExpressionSyntax
    {
    public:
        using Super = ExpressionSyntax;

        [[nodiscard]] AssignmentExpressionSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                                 const std::shared_ptr<const class SyntaxToken> IdentifierToken,
                                                 const std::shared_ptr<const class SyntaxToken> AssignmentToken,
                                                 const std::shared_ptr<const ExpressionSyntax> Expression) noexcept;

        virtual std::vector<std::shared_ptr<const SyntaxNode>> Children() const noexcept override;
        virtual SyntaxKind Kind() const noexcept override;

        const std::shared_ptr<const class SyntaxToken> IdentifierToken;
        const std::shared_ptr<const class SyntaxToken> AssignmentToken;
        const std::shared_ptr<const ExpressionSyntax> Expression;
    };
} // namespace Mamba