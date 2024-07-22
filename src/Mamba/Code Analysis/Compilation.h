#pragma once

#include "SourceText.h"
#include <memory>

namespace Mamba
{
    //
    class Compilation final
    {
        std::vector<std::shared_ptr<class SyntaxTree>> SyntaxTrees;

    public:
        [[nodiscard]] Compilation(const std::vector<StringView>& Sources) noexcept;
        [[nodiscard]] Compilation(const std::vector<std::shared_ptr<const SourceText>>& Sources) noexcept;

    private:
        static void Compile(const std::shared_ptr<class SyntaxTree> SyntaxTree) noexcept;

    public:
        void Compile() noexcept;

        std::vector<std::shared_ptr<const class Diagnostic>> Diagnostics() const noexcept;
    };
} // namespace Mamba