#include "ExpressionSyntax.h"

#include "SyntaxTree.h"

namespace Mamba
{
    ExpressionSyntax::ExpressionSyntax(const class SyntaxTree* SyntaxTree) noexcept :
        Super(SyntaxTree)
    {
    }
} // namespace Mamba