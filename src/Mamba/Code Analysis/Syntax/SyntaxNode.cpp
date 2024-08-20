#include "SyntaxNode.h"

#include "fast_io.h"
#include "Literal.h"
#include "MambaCore.h"
#include "SourceText.h"
#include "SyntaxFacts.h"
#include "SyntaxKind.h"
#include "SyntaxToken.h"
#include "SyntaxTree.h"
#include <memory>
#include <ranges>
#include <sstream>

namespace Mamba
{
    SyntaxNode::SyntaxNode(const std::shared_ptr<const class SyntaxTree> SyntaxTree) noexcept :
        SyntaxTree(SyntaxTree)
    {
    }

    SyntaxNode::~SyntaxNode() noexcept {}

    const std::shared_ptr<const class SyntaxNode> SyntaxNode::Parent() const noexcept
    {
        return SyntaxTree->GetParent(shared_from_this());
    }

    TextSpan SyntaxNode::Span() const noexcept
    {
        const auto First = Children().front()->Span();
        const auto Last = Children().back()->Span();
        return TextSpan::FromBounds(First.Start, Last.End());
    }

    TextSpan SyntaxNode::FullSpan() const noexcept
    {
        const auto First = Children().front()->FullSpan();
        const auto Last = Children().back()->FullSpan();
        return TextSpan::FromBounds(First.Start, Last.End());
    }

    TextLocation SyntaxNode::Location() const noexcept
    {
        return TextLocation(SyntaxTree->Text, Span());
    }

    std::vector<std::shared_ptr<const SyntaxNode>> SyntaxNode::AncestorsAndSelf() const noexcept
    {
        auto Ancestors = std::vector<std::shared_ptr<const SyntaxNode>>();
        auto Node = shared_from_this();
        while (Node)
        {
            Ancestors.emplace_back(Node);
            Node = Node->Parent();
        }

        return Ancestors;
    }

    std::vector<std::shared_ptr<const SyntaxNode>> SyntaxNode::Ancestors() const noexcept
    {
        return AncestorsAndSelf() | std::views::drop(1) | std::ranges::to<std::vector>();
    }

    std::shared_ptr<const class SyntaxToken> SyntaxNode::LastToken() const noexcept
    {
        if (auto Token = std::dynamic_pointer_cast<const SyntaxToken>(shared_from_this()))
        {
            return Token;
        }

        // A syntax node should always contain at least 1 token.
        return Children().back()->LastToken();
    }

    String SyntaxNode::ToString() const noexcept
    {
        auto Stream = std::basic_stringstream<Char>();
        PrettyPrint(Stream, shared_from_this());
        return Stream.str();
    }

    void SyntaxNode::PrettyPrint(
        std::basic_stringstream<Char>& Stream,
        const std::shared_ptr<const SyntaxNode> Node,
        String Indent,
        const bool IsLast
    ) noexcept
    {
        const auto Token = std::dynamic_pointer_cast<const SyntaxToken>(Node);
        const auto TokenMarker = String(IsLast ? TEXT("└──") : TEXT("├──"));

        Stream.write(Indent.data(), Indent.size());
        Stream.write(TokenMarker.data(), TokenMarker.size());

        const auto KindText = SyntaxFacts::ToString(Node->Kind());
        Stream.write(KindText.data(), KindText.size());

        if (Token && Token->Value)
        {
            Stream.put(TEXT(' '));
            std::visit([&]<typename T>(const T& Value)
            {
                if constexpr(std::same_as<T, LiteralType::String>)
                {
                    const auto String = *Token->Value->StringValue;
                    Stream.write(Value.data(), Value.size());
                }
                else if constexpr(std::same_as<T, LiteralType::Character>)
                {
                    Stream.put(Value);
                }
                else if constexpr(std::same_as<T, LiteralType::UnsignedByte>)
                {
                    const auto String = Concat(static_cast<std::uint32_t>(Value));
                    Stream.write(String.data(), String.size());
                }
                else if constexpr(std::same_as<T, LiteralType::SignedByte>)
                {
                    const auto String = Concat(static_cast<std::int32_t>(Value));
                    Stream.write(String.data(), String.size());
                }
                else if constexpr(std::is_scalar_v<T>)
                {
                    const auto String = Concat(Value);
                    Stream.write(String.data(), String.size());
                }
            }, Token->Value->Value);
        }

        if (Node->Kind() == SyntaxKind::IdentifierToken)
        {
            Stream.put(TEXT(' '));
            const auto View = Node->SyntaxTree->Text->ToView(Node->Span());
            Stream.write(View.data(), View.size());
        }

        Stream.put(TEXT('\n'));

        Indent += IsLast ? TEXT("    ") : TEXT("│   ");

        const auto LastChild = Node->Children().size() == 0 ? nullptr : Node->Children().back();

        for (auto Child : Node->Children())
        {
            PrettyPrint(Stream, Child, Indent, Child == LastChild);
        }
    }
} // namespace Mamba