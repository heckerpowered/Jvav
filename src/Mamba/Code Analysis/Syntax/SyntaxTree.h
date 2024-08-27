#pragma once

#include <unordered_map>

#include "CompilationUnitSyntax.h"
#include "MambaCore.h"
#include "SourceText.h"

namespace Mamba
{
    class SourceText;

    class SyntaxTree
    {
        CompilationUnitSyntax* PrivateRoot;
        const SourceText& PrivateSourceText;

        friend class Compiler;

    public:
        [[nodiscard]] SyntaxTree(const class SourceText& SourceText) noexcept;
        ~SyntaxTree() noexcept;

        template<typename SelfT>
        [[nodiscard]] auto&& Root(this SelfT&& Self) noexcept
        {
            return std::forward<SelfT>(Self).PrivateRoot;
        }

        template<typename SelfT>
        [[nodiscard]] const SourceText& Text(this SelfT&& Self) noexcept
        {
            return std::forward<SelfT>(Self).PrivateSourceText;
        }

        NullablePointer<const SyntaxNode> Parent(const SyntaxNode& Node) const noexcept;

    private:
        void BuildParentsMap(const SyntaxNode& Root) noexcept;
        std::unordered_map<const SyntaxNode*, NullablePointer<const SyntaxNode>> ParentsMap;
    };
} // namespace Mamba