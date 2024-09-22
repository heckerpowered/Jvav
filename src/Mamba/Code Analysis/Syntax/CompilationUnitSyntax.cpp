#include "CompilationUnitSyntax.h"

using namespace Mamba;

CompilationUnitSyntax::CompilationUnitSyntax(const SyntaxTree* SyntaxTree, std::vector<MemberSyntax*>&& Members, const SyntaxToken* EndOfFileToken) noexcept :
    Super(SyntaxTree), Members(std::move(Members)), EndOfFileToken(EndOfFileToken)
{
}

CompilationUnitSyntax::~CompilationUnitSyntax() noexcept
{
    for (auto&& Member : Members)
    {
        delete Member;
    }
}

SyntaxKind CompilationUnitSyntax::Kind() const noexcept
{
    return SyntaxKind::CompilationUnit;
}

std::size_t CompilationUnitSyntax::ChildrenCount() const noexcept
{
    return Members.size() + 1;
}

const SyntaxNode* CompilationUnitSyntax::ChildAt(std::size_t Index) const noexcept
{
    if (Index < Members.size())
    {
        return Members[Index];
    }
    else if (Index == Members.size())
    {
        return EndOfFileToken;
    }

    ReportChildrenAccessOutOfBounds(Index);
}