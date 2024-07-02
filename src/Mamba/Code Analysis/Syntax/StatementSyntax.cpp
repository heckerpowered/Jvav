#include "StatementSyntax.h"

namespace Mamba
{
    StatementSyntax::StatementSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree) noexcept :
        Super(SyntaxTree)
    {
    }
} // namespace Mamba