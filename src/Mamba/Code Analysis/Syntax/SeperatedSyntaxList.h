#pragma once

#include <cstddef>
#include <ranges>
#include <vector>

namespace Mamba
{
    template<typename T, template<typename...> typename Container = std::vector>
    class SeperatedSyntaxList : Container<T>
    {
        [[nodiscard]] constexpr Container<T> SkipSeperators(const Container<T>& NodesAndSeparators) const noexcept
        {
#if __cpp_lib_ranges_enumerate == 202302L
            NodesAndSeparators | std::views::enumerate;
#endif

            auto Result = Container<T>(NodesAndSeparators.size() / 2);
#if __cpp_size_t_suffix == 202011L
            for (auto Index = 0uz; Index < NodesAndSeparators.size() / 2; ++Index)
#else
            for (auto Index = std::size_t(); Index < NodesAndSeparators.size() / 2; ++Index)
#endif
            {
                Result[Index] = NodesAndSeparators[Index * 2];
            }
            return Result;
        }

    public:
        using Super = Container<T>;

        [[nodiscard]] constexpr SeperatedSyntaxList(Container<T>&& NodesAndSeparators) noexcept :
            Super(std::forward<Container<T>>(NodesAndSeparators))
        {
        }

        template<typename SelfType>
        [[nodiscard]] auto operator[](this SelfType&& Self, const std::size_t Index)
            noexcept(noexcept(std::forward<SelfType>(Self).Super::operator[](Index))
            ) -> decltype(std::forward<SelfType>(Self).Super::operator[](Index))
        {
            return std::forward<SelfType>(Self).Super::operator[](Index * 2);
        }

        [[nodiscard]] constexpr std::size_t Count() const noexcept
        {
            return Super::size() / 2;
        }

        [[nodiscard]] constexpr Container<T> Nodes() const noexcept
        {
            return SkipSeperators(*this);
        }

        [[nodiscard]] constexpr T GetSeparator(const std::size_t Index) const noexcept

        {
            return (*this)[Index * 2 + 1];
        }

        [[nodiscard]] constexpr const Container<T>& WithSeperators() const noexcept
        {
            return *this;
        }

        template<template<typename...> typename ToContainer>
        [[nodiscard]] constexpr ToContainer<T> WithSeperators() const noexcept
        {
            return *this | std::ranges::to<ToContainer>();
        }
    };
} // namespace Mamba