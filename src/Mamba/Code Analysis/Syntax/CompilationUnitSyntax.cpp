#include "CompilationUnitSyntax.h"
#include "MemberSyntax.h"
#include "SyntaxNode.h"
#include "SyntaxToken.h"

namespace Mamba
{

    CompilationUnitSyntax::CompilationUnitSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                                 const std::vector<std::shared_ptr<const class MemberSyntax>>& Members,
                                                 std::shared_ptr<const class SyntaxToken> EndOfFileToken) noexcept :
        Super(SyntaxTree), Members(Members), EndOfFileToken(EndOfFileToken)
    {
    }

    CompilationUnitSyntax::CompilationUnitSyntax(const std::shared_ptr<const class SyntaxTree> SyntaxTree,
                                                 std::vector<std::shared_ptr<const class MemberSyntax>>&& Members,
                                                 std::shared_ptr<const class SyntaxToken> EndOfFileToken) noexcept :
        Super(SyntaxTree), Members(std::move(Members)), EndOfFileToken(EndOfFileToken)
    {
    }

    SyntaxKind CompilationUnitSyntax::Kind() const noexcept
    {
        return SyntaxKind::CompilationUnit;
    }

    std::vector<std::shared_ptr<const SyntaxNode>> CompilationUnitSyntax::Children() const noexcept
    {
        auto Children = std::vector<std::shared_ptr<const SyntaxNode>>();
        Children.reserve(Members.size() + 1);
        Children.append_range(Members);
        Children.emplace_back(EndOfFileToken);
        return Children;
    }

} // namespace Mamba
