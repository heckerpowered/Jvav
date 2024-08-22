#pragma once

#include "ExpressionSyntax.h"
#include "SyntaxKind.h"
#include "SyntaxToken.h"
#include <vector>

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

        std::vector<const SyntaxNode*> Children() const noexcept override;
        SyntaxKind Kind() const noexcept override;

        const SyntaxToken* IdentifierToken;
        const SyntaxToken* AssignmentToken;
        const ExpressionSyntax* Expression;
    };
} // namespace Mamba