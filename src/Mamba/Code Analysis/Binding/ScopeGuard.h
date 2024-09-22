#pragma once

#include "BoundScope.h"

namespace Mamba
{
    // Creates a scope from the current scope and enters it on construction, and exits it on destruction.
    class ScopeGuard
    {
        BoundScope*& Scope;
        bool Leaved : 1;

    public:
        [[nodiscard]] ScopeGuard(BoundScope*& Scope) noexcept;
        ~ScopeGuard() noexcept;

        void PreLeave() noexcept;
    };
}; // namespace Mamba