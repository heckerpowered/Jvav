#include "ScopeGuard.h"

#include "BoundScope.h"

using namespace Mamba;

ScopeGuard::ScopeGuard(std::shared_ptr<class BoundScope>& Scope) noexcept :
    Scope(Scope), Leaved()
{
    Scope = Scope->DeclareScope();

    fast_io::io::perrln("Enter scope from ", fast_io::mnp::pointervw(&*Scope->Parent), " to ", fast_io::mnp::pointervw(&*Scope));
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
    fast_io::io::perrln("Leave scope from ", fast_io::mnp::pointervw(&*Scope), " to ", fast_io::mnp::pointervw(&*Scope->Parent));

    // The parent of the scope is constructed in the constructor,
    // which is guaranteed to be non-const here.
    Scope = std::const_pointer_cast<BoundScope>(Scope->Parent);
    Leaved = true;
}
