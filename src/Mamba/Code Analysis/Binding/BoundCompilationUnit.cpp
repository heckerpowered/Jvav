#include "BoundCompilationUnit.h"

using namespace Mamba;

BoundCompilationUnit::BoundCompilationUnit(
    const SyntaxNode* SyntaxNode,
    const BoundScope* GlobalScope
) noexcept :
    Super(SyntaxNode), GlobalScope(GlobalScope)
{
}

BoundNodeKind BoundCompilationUnit::Kind() const noexcept
{
    return BoundNodeKind::CompilationUnit;
}
