#pragma once

#include <initializer_list>
#include <random>

namespace Mamba
{
    extern std::random_device RandomDevice;

    template<typename... T>
    auto RandomSelect(T&&... Arguments) noexcept
    {
        std::uniform_int_distribution<int> Distribution(0, sizeof...(Arguments) - 1);
        auto Index = Distribution(RandomDevice);
        return std::initializer_list{ std::forward<T>(Arguments)... }.begin()[Index];
    }
} // namespace Mamba