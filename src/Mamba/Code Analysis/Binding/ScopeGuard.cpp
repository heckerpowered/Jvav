#include "ScopeGuard.h"

#include "BoundScope.h"

using namespace Mamba;

ScopeGuard::ScopeGuard(std::shared_ptr<class BoundScope>& Scope) noexcept :
    Scope(Scope), Leaved()
{
    Scope = Scope->DeclareScope();
}

ScopeGuard::~ScopeGuard() noexcept
{
    if (Leaved)
    {
        return;
    }

    PreLeave();
}

void ScopeGuard::PreLeave() noexcept
{
    // The parent of the scope is constructed in the constructor,
    // which is guaranteed to be non-const here.
    Scope = std::const_pointer_cast<BoundScope>(Scope->Parent);
    Leaved = true;
}
