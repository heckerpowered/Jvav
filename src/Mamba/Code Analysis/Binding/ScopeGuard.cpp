#include "ScopeGuard.h"

#include "BoundScope.h"

using namespace Mamba;

ScopeGuard::ScopeGuard(std::shared_ptr<class BoundScope>& Scope) noexcept :
    Scope(Scope)
{
    Scope = Scope->DeclareScope();
}

ScopeGuard::~ScopeGuard() noexcept
{
    // The parent of the scope is constructed in the constructor,
    // which is guaranteed to be non-const here.
    Scope = std::const_pointer_cast<BoundScope>(Scope->Parent);
}
