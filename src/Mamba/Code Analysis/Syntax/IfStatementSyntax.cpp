#include "IfStatementSyntax.h"

using namespace Mamba;

IfStatementSyntax::IfStatementSyntax(
    const class SyntaxTree* SyntaxTree,
    const SyntaxToken* IfKeyword,
    const ExpressionSyntax* Condition,
    const StatementSyntax* ThenStatement,
    NullablePointer<const ElseClauseSyntax> ElseClause
) noexcept :
    Super(SyntaxTree),
    IfKeyword(IfKeyword),
    Condition(Condition),
    ThenStatement(ThenStatement),
    ElseClause(ElseClause)
{
}

IfStatementSyntax::~IfStatementSyntax() noexcept
{
    delete Condition;
    delete ThenStatement;
    delete ElseClause;
}

SyntaxKind IfStatementSyntax::Kind() const noexcept
{
    return SyntaxKind::IfStatement;
}

std::vector<const SyntaxNode*> IfStatementSyntax::Children() const noexcept
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

    auto Children = std::vector<const SyntaxNode*>();
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