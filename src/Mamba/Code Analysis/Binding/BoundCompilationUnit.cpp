#include "BoundCompilationUnit.h"

using namespace Mamba;

BoundCompilationUnit::BoundCompilationUnit(
    const std::shared_ptr<const class SyntaxNode> SyntaxNode,
    const std::shared_ptr<const class BoundScope> GlobalScope
) noexcept :
    Super(SyntaxNode), GlobalScope(GlobalScope)
{
}

BoundNodeKind BoundCompilationUnit::Kind() const noexcept
{
    return BoundNodeKind::CompilationUnit;
}
