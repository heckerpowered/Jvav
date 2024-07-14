#include "FunctionDeclarationSyntax.h"

#include "BlockStatementSyntax.h"
#include "ParameterSyntax.h"
#include "SyntaxToken.h"
#include "TypeClauseSyntax.h"

namespace Mamba
{
    FunctionDeclarationSyntax::FunctionDeclarationSyntax(
        const std::shared_ptr<const class SyntaxTree> SyntaxTree,
        const std::shared_ptr<const class SyntaxToken> FunctionKeyword,
        const std::shared_ptr<const class SyntaxToken> Identifier,
        const std::shared_ptr<const class SyntaxToken> OpenParenthesisToken,
        const std::shared_ptr<const SeperatedSyntaxList<const std::shared_ptr<const SyntaxNode>>> Parameters,
        const std::shared_ptr<const class SyntaxToken> CloseParenthesisToken,
        const NullableSharedPtr<const class TypeClauseSyntax> Type,
        const std::shared_ptr<const class BlockStatementSyntax> Body) noexcept :
        Super(SyntaxTree),
        FunctionKeyword(FunctionKeyword),
        Identifier(Identifier),
        OpenParenthesisToken(OpenParenthesisToken),
        Parameters(Parameters),
        CloseParenthesisToken(CloseParenthesisToken),
        Type(Type),
        Body(Body)
    {
    }

    SyntaxKind FunctionDeclarationSyntax::Kind() const noexcept
    {
        return SyntaxKind::FunctionDeclaration;
    }

    std::vector<std::shared_ptr<const class SyntaxNode>> FunctionDeclarationSyntax::Children() const noexcept
    {
        auto Children = std::vector<std::shared_ptr<const class SyntaxNode>>();
        Children.reserve(6 + Parameters->Count() * 2);

        Children.emplace_back(FunctionKeyword);
        Children.emplace_back(Identifier);
        Children.emplace_back(OpenParenthesisToken);
        Children.emplace_back(CloseParenthesisToken);
        if (Type)
        {
            Children.emplace_back(Type);
        }
        Children.emplace_back(Body);

        return Children;
    }
} // namespace Mamba