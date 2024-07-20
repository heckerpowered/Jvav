#include "ExpressionSyntax.h"

#include "SyntaxTree.h"

namespace Mamba
{
    ExpressionSyntax::ExpressionSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree) noexcept :
        Super(SyntaxTree)
    {
    }
} // namespace Mamba