#include "ScopeGuard.h"

using namespace Mamba;

ScopeGuard::ScopeGuard(BoundScope*& Scope) noexcept :
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
    auto PreviousScope = Scope->Parent;
    Scope = const_cast<BoundScope*>(Scope->Parent);
    delete PreviousScope;

    Leaved = true;
}
