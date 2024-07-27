#include "BoundCallExpression.h"

using namespace Mamba;

BoundCallExpression::BoundCallExpression(
    const std::shared_ptr<const class SyntaxNode> Syntax,
    const std::shared_ptr<const class FunctionSymbol> Function,
    const std::span<std::shared_ptr<const class BoundExpression>> Arguments
) noexcept :
    Super(Syntax),
    Function(Function),
#if __cpp_lib_containers_ranges == 202202L
    Arguments(std::from_range, Arguments)
#else
    Arguments(Arguments.begin(), Arguments.end())
#endif
{
}

BoundCallExpression::BoundCallExpression(
    const std::shared_ptr<const class SyntaxNode> Syntax,
    const std::shared_ptr<const class FunctionSymbol> Function,
    std::vector<std::shared_ptr<const class BoundExpression>>&& Arguments
) noexcept :
    Super(Syntax), Function(Function), Arguments(std::move(Arguments))
{
}

BoundNodeKind BoundCallExpression::Kind() const noexcept
{
    return BoundNodeKind::CallExpression;
}
