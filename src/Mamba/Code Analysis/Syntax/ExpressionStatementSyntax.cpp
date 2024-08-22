#include "ExpressionStatementSyntax.h"

#include "ExpressionSyntax.h"

using namespace Mamba;

ExpressionStatementSyntax::ExpressionStatementSyntax(
    const class SyntaxTree* SyntaxTree,
    const ExpressionSyntax* Expression
) noexcept :
    Super(SyntaxTree), Expression(Expression)
{
}

ExpressionStatementSyntax::~ExpressionStatementSyntax() noexcept
{
    delete Expression;
}

SyntaxKind ExpressionStatementSyntax::Kind() const noexcept
{
    return SyntaxKind::ExpressionStatement;
}

std::vector<const SyntaxNode*> ExpressionStatementSyntax::Children() const noexcept
{
    return { Expression };
}