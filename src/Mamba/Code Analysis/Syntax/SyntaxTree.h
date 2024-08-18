#pragma once

#include <unordered_map>

#include "CompilationUnitSyntax.h"
#include "MambaCore.h"

namespace Mamba
{
    class SyntaxTree
    {
    public:
        CompilationUnitSyntax Root;

    private:
        std::unordered_map<SyntaxNode*, NullablePointer<SyntaxNode>> ParentsMap;
    };
} // namespace Mamba