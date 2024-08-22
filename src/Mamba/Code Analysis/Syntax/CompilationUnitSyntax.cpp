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

std::vector<const SyntaxNode*> CompilationUnitSyntax::Children() const noexcept
{
    auto Children = std::vector<const SyntaxNode*>();
    Children.reserve(Members.size() + 1);

#if defined(__cpp_lib_containers_ranges) && __cpp_lib_containers_ranges >= 202202L
    Children.append_range(Members);
#else
    for (auto&& Member : Members)
    {
        Children.emplace_back(Member);
    }
#endif

    Children.emplace_back(EndOfFileToken);
    return Children;
}