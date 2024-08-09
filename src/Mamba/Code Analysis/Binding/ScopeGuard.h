#pragma once

#include <memory>

namespace Mamba
{
    // Creates a scope from the current scope and enters it on construction, and exits it on destruction.
    class ScopeGuard
    {
        std::shared_ptr<class BoundScope>& Scope;
        bool Leaved : 1;

    public:
        [[nodiscard]] ScopeGuard(std::shared_ptr<class BoundScope>& Scope) noexcept;
        ~ScopeGuard() noexcept;

        void PreLeave() noexcept;
    };
}; // namespace Mamba