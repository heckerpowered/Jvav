#include "CompilationUnitSyntax.h"
#include "MambaCore.h"

MAMBA_NAMESPACE_BEGIN

CompilationUnitSyntax::CompilationUnitSyntax(const std::shared_ptr<const class SyntaxTree>& SyntaxTree,
                                             const std::shared_ptr<const class SyntaxToken>& EndOfFileToken) noexcept :
    Super(SyntaxTree), EndOfFileToken(EndOfFileToken)
{
}

SyntaxKind CompilationUnitSyntax::Kind() const noexcept
{
    return SyntaxKind::CompilationUnit;
}

MAMBA_NAMESPACE_END