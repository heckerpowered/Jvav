#pragma once

#include "MambaCore.h"
#include "SourceText.h"
#include <memory>

namespace Mamba
{
    //
    class Compilation final
    {
    public:
        std::vector<std::shared_ptr<class SyntaxTree>> SyntaxTrees;

        [[nodiscard]] Compilation(const std::vector<StringView>& Sources) noexcept;
        [[nodiscard]] Compilation(const std::vector<std::shared_ptr<const SourceText>>& Sources) noexcept;

    private:
        static std::shared_ptr<const class BoundCompilationUnit> Compile(const std::shared_ptr<class SyntaxTree> SyntaxTree) noexcept;

    public:
        void Compile(const StringView ModuleName) noexcept;

        std::vector<std::shared_ptr<const class Diagnostic>> Diagnostics() const noexcept;
    };
} // namespace Mamba