#include "SyntaxTree.h"
#include "MambaCore.h"

MAMBA_NAMESPACE_BEGIN

void Parse(const std::shared_ptr<const SyntaxTree> SyntaxTree,
           std::shared_ptr<const class CompilationUnitSyntax>& Root,
           std::vector<std::shared_ptr<const class Diagnostic>>& Diagnostics) noexcept
{
}

void SyntaxTree::Parse(const std::shared_ptr<const SyntaxTree> SyntaxTree,
                       std::shared_ptr<const class CompilationUnitSyntax>& Root,
                       std::vector<std::shared_ptr<const class Diagnostic>>& Diagnostics) noexcept
{
}

MAMBA_NAMESPACE_END
