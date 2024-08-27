#pragma once

#include "ExpressionSyntax.h"
#include "SyntaxKind.h"
#include "SyntaxToken.h"

namespace Mamba
{
    class AssignmentExpressionSyntax : public ExpressionSyntax
    {
    public:
        using Super = ExpressionSyntax;

        [[nodiscard]] AssignmentExpressionSyntax(
            const class SyntaxTree* SyntaxTree,
            const SyntaxToken* IdentifierToken,
            const SyntaxToken* AssignmentToken,
            const ExpressionSyntax* Expression
        ) noexcept;

        ~AssignmentExpressionSyntax() noexcept override;

        SyntaxKind Kind() const noexcept override;

        const SyntaxToken* IdentifierToken;
        const SyntaxToken* AssignmentToken;
        const ExpressionSyntax* Expression;

    private:
        std::size_t ChildrenCount() const noexcept override;
        const SyntaxNode* ChildAt(std::size_t Index) const noexcept override;
    };
} // namespace Mamba