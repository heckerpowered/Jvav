#pragma once

#include "Literal.h"
#include <memory>

namespace Mamba
{
    class BoundConstant
    {
    public:
        [[nodiscard]] BoundConstant(const std::shared_ptr<const struct Literal> Value) noexcept;

        const std::shared_ptr<const struct Literal> Value;

        const LiteralValue* operator->() const noexcept;
    };
}; // namespace Mamba