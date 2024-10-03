#include "ScopeGuard.h"

using namespace Mamba;

ScopeGuard::ScopeGuard(BoundScope*& Scope) noexcept :
    Scope(Scope), Leaved()
{
    // The result of Scope.DeclareScope() is the child scope of the current scope,
    // which is deleted when the parent scope is deconstructed.
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
    Scope = const_cast<BoundScope*>(Scope->Parent);

    Leaved = true;
}
