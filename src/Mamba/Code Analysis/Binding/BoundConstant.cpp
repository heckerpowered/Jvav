#include "BoundConstant.h"

using namespace Mamba;

BoundConstant::BoundConstant(struct Constant Constant) noexcept :
    PrivateConstant(Constant)
{
}

Constant BoundConstant::Constant() noexcept
{
    return PrivateConstant
}
