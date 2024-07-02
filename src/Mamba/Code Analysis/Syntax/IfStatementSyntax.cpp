#include "IfStatementSyntax.h"

#include "ElseClauseSyntax.h"
#include "ExpressionSyntax.h"

namespace Mamba
{
    IfStatementSyntax::IfStatementSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                         const std::shared_ptr<const class SyntaxToken> IfKeyword,
                                         const NullableSharedPtr<const class SyntaxToken> OpenParenthesisToken,
                                         const std::shared_ptr<const class ExpressionSyntax> Condition,
                                         const NullableSharedPtr<const class SyntaxToken> CloseParenthesisToken,
                                         const std::shared_ptr<const class StatementSyntax> ThenStatement,
                                         const NullableSharedPtr<const class ElseClauseSyntax> ElseClause) noexcept :
        Super(SyntaxTree),
        IfKeyword(IfKeyword),
        OpenParenthesisToken(OpenParenthesisToken),
        Condition(Condition),
        CloseParenthesisToken(CloseParenthesisToken),
        ThenStatement(ThenStatement),
        ElseClause(ElseClause)
    {
    }

    SyntaxKind IfStatementSyntax::Kind() const noexcept
    {
        return SyntaxKind::IfStatement;
    }

    std::vector<std::shared_ptr<const class SyntaxNode>> IfStatementSyntax::Children() const noexcept
    {
        // Calculates the number of children, in for statement, parentheses and else clause are optional,
        // so there are at least 3 children
        auto ChildrenCount = 3;
        if (OpenParenthesisToken)
        {
            ++ChildrenCount;
        }
        if (CloseParenthesisToken)
        {
            ++ChildrenCount;
        }
        if (ElseClause)
        {
            ++ChildrenCount;
        }

        auto Children = std::vector<std::shared_ptr<const class SyntaxNode>>();
        Children.reserve(ChildrenCount);

        Children.emplace_back(IfKeyword);
        if (OpenParenthesisToken)
        {
            Children.emplace_back(OpenParenthesisToken);
        }
        Children.emplace_back(Condition);
        if (CloseParenthesisToken)
        {
            Children.emplace_back(CloseParenthesisToken);
        }
        Children.emplace_back(ThenStatement);
        if (ElseClause)
        {
            Children.emplace_back(ElseClause);
        }

        return Children;
    }
} // namespace Mamba