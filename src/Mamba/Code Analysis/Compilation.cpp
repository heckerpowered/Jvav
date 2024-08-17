#include "Compilation.h"

#include <memory>

#include <fast_io.h>
#include <ranges>

#include "Binder.h"
#include "BoundCompilationUnit.h"
#include "BoundProgram.h"
#include "CompilationUnitSyntax.h"
#include "DotNet/DotNetEmitter.h"
#include "MambaCore.h"
#include "Parser.h"
#include "SourceText.h"
#include "SyntaxTree.h"

namespace Mamba
{
    std::vector<std::shared_ptr<const SourceText>> TransformToSource(const std::vector<StringView>& Sources) noexcept
    {
        auto SourceTexts = std::vector<std::shared_ptr<const SourceText>>();
        SourceTexts.reserve(Sources.size());
        for (auto&& Source : Sources)
        {
            const auto SharedSource = std::make_shared<String>(Source);
            SourceTexts.emplace_back(std::make_shared<SourceText>(SharedSource));
        }
        return SourceTexts;
    }

    std::shared_ptr<const BoundCompilationUnit> Compilation::Compile(const std::shared_ptr<class SyntaxTree> SyntaxTree) noexcept
    {
        auto Parser = ::Mamba::Parser(SyntaxTree);
        SyntaxTree->PrivateRoot = Parser.ParseCompilationUnit();
        SyntaxTree->PrivateDiagnostics = std::move(Parser.Diagnostics);

        fast_io::io::println(fast_io::mnp::code_cvt(SyntaxTree->Root()->ToString()));

        auto Binder = ::Mamba::Binder(SyntaxTree);
        return Binder.BindCompilationUnit();
    }

    void Compilation::Compile(const StringView ModuleName) noexcept
    {
        auto CompilationUnits = std::vector<std::shared_ptr<const BoundCompilationUnit>>();
        for (auto&& SyntaxTree : SyntaxTrees)
        {
            CompilationUnits.emplace_back(Compilation::Compile(SyntaxTree));
        }

        auto Emitter = DotNetEmitter();
        const auto BoundProgram = ::Mamba::BoundProgram(CompilationUnits);
        Emitter.Emit(BoundProgram, ModuleName);
    }

    std::vector<Diagnostic> Compilation::Diagnostics() const noexcept
    {
        return SyntaxTrees |
               std::views::transform([](auto&& SyntaxTree)
                                     { return SyntaxTree->PrivateDiagnostics; }) |
               std::views::join | std::ranges::to<std::vector>();
    }

    Compilation::Compilation(const std::vector<StringView>& Sources) noexcept :
        Compilation(TransformToSource(Sources))
    {
    }

    Compilation::Compilation(const std::vector<std::shared_ptr<const SourceText>>& SourceTexts) noexcept
    {
        for (auto&& SourceText : SourceTexts)
        {
            const auto SyntaxTree = std::make_shared<::Mamba::SyntaxTree>(SourceText);
            SyntaxTrees.emplace_back(SyntaxTree);
        }
    }
} // namespace Mamba