#pragma once

#include <concepts>
#include <cstddef>
#include <ranges>
#include <vector>

#include "SyntaxNode.h"

namespace Mamba
{
    template<typename T, template<typename...> typename Container = std::vector>
        requires requires {
            std::is_pointer_v<T>;
            std::derived_from<typename std::pointer_traits<T>::element_type, SyntaxNode>;
        }
    class SeperatedSyntaxList : public Container<const SyntaxNode*>
    {
        [[nodiscard]] constexpr Container<T> SkipSeperators(const Container<const SyntaxNode*>& NodesAndSeparators) const noexcept
        {
#if __cpp_lib_ranges_enumerate >= 202302L
            // NodesAndSeparators | std::views::enumerate;
#endif
            if (NodesAndSeparators.size() == 1)
            {
                return Container<T>{ static_cast<T>(NodesAndSeparators.front()) };
            }
            auto Result = Container<T>(NodesAndSeparators.size() / 2);
#if __cpp_size_t_suffix >= 202011L
            for (auto Index = 0uz; Index < NodesAndSeparators.size() / 2; ++Index)
#else
            for (auto Index = std::size_t(); Index < NodesAndSeparators.size() / 2; ++Index)
#endif
            {
                Result[Index] = static_cast<T>(NodesAndSeparators[Index * 2]);
            }
            return Result;
        }

    public:
        using Super = Container<const SyntaxNode*>;
        using ValueType = std::decay_t<T>;

        [[nodiscard]] constexpr SeperatedSyntaxList(Container<const SyntaxNode*>&& NodesAndSeparators) noexcept :
            Super(std::forward<Container<const SyntaxNode*>>(NodesAndSeparators))
        {
        }

        template<typename SelfType>
        [[nodiscard]] auto operator[](this SelfType&& Self, std::size_t Index)
            noexcept(noexcept(std::forward<SelfType>(Self).Super::operator[](Index))
            ) -> decltype(std::forward<SelfType>(Self).Super::operator[](Index))
        {
            return static_cast<ValueType>(std::forward<SelfType>(Self).Super::operator[](Index * 2));
        }

        [[nodiscard]] constexpr std::size_t Count() const noexcept
        {
            return Super::size() / 2;
        }

        [[nodiscard]] constexpr Container<T> Nodes() const noexcept
        {
            return SkipSeperators(*this);
        }

        // TODO: use ranges instead
        [[nodiscard]] constexpr T GetSeparator(const std::size_t Index) const noexcept

        {
            return (*this)[Index * 2 + 1];
        }

        [[nodiscard]] constexpr const Container<const SyntaxNode*>& WithSeperators() const noexcept
        {
            return *this;
        }

        template<template<typename...> typename ToContainer>
        [[nodiscard]] constexpr ToContainer<const SyntaxNode*> WithSeperators() const noexcept
        {
            return *this | std::ranges::to<ToContainer>();
        }
    };
} // namespace Mamba