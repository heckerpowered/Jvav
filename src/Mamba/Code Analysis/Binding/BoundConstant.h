#pragma once

#include "Constant.h"

namespace Mamba
{
    class BoundConstant
    {
        Constant PrivateConstant;

    public:
        [[nodiscard]] BoundConstant(struct Constant Constant) noexcept;

        Constant Constant() noexcept;
    };
}; // namespace Mamba