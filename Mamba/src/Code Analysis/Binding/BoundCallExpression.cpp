#include "BoundCallExpression.h"
#include "FunctionSymbol.h"

using namespace Mamba;

BoundCallExpression::BoundCallExpression(
    const SyntaxNode* Syntax,
    const FunctionSymbol* Function,
    std::vector<const BoundExpression*>&& Arguments
) noexcept :
    Super(Syntax), Function(Function), Arguments(std::move(Arguments))
{
}

BoundCallExpression::~BoundCallExpression() noexcept
{
    for (auto Argument : Arguments)
    {
        delete Argument;
    }
}

BoundNodeKind BoundCallExpression::Kind() const noexcept
{
    return BoundNodeKind::CallExpression;
}

const TypeSymbol* BoundCallExpression::Type() const noexcept
{
    return Function->Type;
}
