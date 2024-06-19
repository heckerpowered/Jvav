#include "CompilationUnitSyntax.h"
#include "SyntaxNode.h"
#include "SyntaxToken.h"

namespace Mamba
{

    CompilationUnitSyntax::CompilationUnitSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                                 const std::shared_ptr<const class SyntaxToken> EndOfFileToken) noexcept
        :
        Super(SyntaxTree), EndOfFileToken(EndOfFileToken)
    {
    }

    SyntaxKind CompilationUnitSyntax::Kind() const noexcept
    {
        return SyntaxKind::CompilationUnit;
    }

    std::vector<std::shared_ptr<const SyntaxNode>> CompilationUnitSyntax::Children() const noexcept
    {
        return { std::dynamic_pointer_cast<const SyntaxNode>(EndOfFileToken) };
    }

} // namespace Mamba
