#pragma once

#include <cstddef>
#include <ranges>
#include <vector>

namespace Mamba
{
    template<typename T, template<typename...> typename Container = std::vector>
    class SeperatedSyntaxList : Container<T>
    {
        [[nodiscard]] constexpr Container<T> SkipSeperators(Container<T>&& NodesAndSeparators) const noexcept
        {
            auto Result = Container<T>(NodesAndSeparators.size() / 2);
            for (auto Index = 0uz; Index < NodesAndSeparators.size() / 2; ++Index)
            {
                Result[Index] = std::move(NodesAndSeparators[Index * 2]);
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
            noexcept(noexcept(std::forward<SelfType>(Self).Super::operator[](Index)))
                -> decltype(std::forward<SelfType>(Self).Super::operator[](Index))
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