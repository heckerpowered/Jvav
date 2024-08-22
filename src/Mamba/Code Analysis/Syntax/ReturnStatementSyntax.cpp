#include "ReturnStatementSyntax.h"

using namespace Mamba;

ReturnStatementSyntax::ReturnStatementSyntax(
    const class SyntaxTree* SyntaxTree,
    const SyntaxToken* ReturnKeyword,
    NullablePointer<const ExpressionSyntax> Expression
) noexcept :
    Super(SyntaxTree), ReturnKeyword(ReturnKeyword), Expression(Expression)
{
}

ReturnStatementSyntax::~ReturnStatementSyntax() noexcept
{
    delete Expression;
}

SyntaxKind ReturnStatementSyntax::Kind() const noexcept
{
    return SyntaxKind::ReturnStatement;
}

std::vector<const SyntaxNode*> ReturnStatementSyntax::Children() const noexcept
{
    return { ReturnKeyword, Expression };
}